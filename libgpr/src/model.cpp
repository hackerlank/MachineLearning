#include "model.h"
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_permutation.h>
#include <ulib/math_factorial.h>
#include "common.h"
#include "debug.h"
#include "flags.h"

using namespace std;

namespace gpr {

model::~model()
{
	gsl_vector_free(_tv);
	gsl_matrix_free(_fm);
	gsl_vector_free(_col_mean);
	gsl_vector_free(_col_sd);
	gsl_matrix_free(_ikm);
}

int model::alloc_training_data(int nrow, int ncol)
{
	gsl_vector_free(_tv);
	gsl_matrix_free(_fm);

	_tv = gsl_vector_alloc(nrow);
	if (_tv == NULL) {
		ULIB_FATAL("couldn't allocate target value vector");
		return -1;
	}
	_fm = gsl_matrix_alloc(nrow, ncol);
	if (_fm == NULL) {
		ULIB_FATAL("couldn't allocate feature matrix");
		_tv = NULL;
		return -1;
	}

	return 0;
}

int model::load_training_data(const dataset &ds)
{
	int nrow, ncol;

        nrow = ds.ins_num();
        ncol = ds.fea_num();

	if (nrow <= 0 || ncol < 1) {
		ULIB_FATAL("invalid training data dimensions");
		return -1;
	}
        if (nrow > FLAGS_max_num_examples)
                nrow = FLAGS_max_num_examples;

	if (alloc_training_data(nrow, ncol)) {
		ULIB_FATAL("couldn't allocate training data");
		return -1;
	}

        double tavg = 0;
        double tvar = 0;
	for (int i = 0; i < nrow; ++i) {
		double t = ds.get_tgv(i);
                tavg += t;
                tvar += t*t;
		gsl_vector_set(_tv, i, t);
                for (int j = 0; j < ncol; ++j)
                        gsl_matrix_set(_fm, i, j, ds.get_fea(i, j));
	}
        _t_avg = tavg/nrow;
        _t_std = sqrt(tvar/nrow - _t_avg*_t_avg);

        return 0;
}

int model::get_col_mean()
{
	size_t nrow = _fm->size1;
	size_t ncol = _fm->size2;

	gsl_vector_free(_col_mean);

	gsl_vector *mean = gsl_vector_alloc(ncol);
	if (mean == NULL) {
		ULIB_FATAL("couldn't allocate vector");
		return -1;
	}

	for (size_t j = 0; j < ncol; ++j) {
		double sum = 0;
		for (size_t i = 0; i < nrow; ++i)
			sum += gsl_matrix_get(_fm, i, j);
		sum /= nrow;
		gsl_vector_set(mean, j, sum);
	}

	_col_mean = mean;

	return 0;
}

int model::get_col_sd()
{
	size_t nrow = _fm->size1;
	size_t ncol = _fm->size2;

	gsl_vector_free(_col_sd);

	gsl_vector *sd = gsl_vector_alloc(ncol);
	if (sd == NULL) {
		ULIB_FATAL("couldn't allocate vector");
		return -1;
	}

	double k = sqrt(nrow);
	for (size_t j = 0; j < ncol; ++j) {
		gsl_vector_const_view cv = gsl_matrix_const_column(_fm, j);
		double d = gsl_blas_dnrm2(&cv.vector);
		gsl_vector_set(sd, j, d/k);
	}

	_col_sd = sd;

	return 0;
}

inline void model::zero_out_mat(gsl_matrix *mat)
{
	size_t ncol = mat->size2;

	for (size_t j = 0; j < ncol; ++j) {
		gsl_vector_view cv = gsl_matrix_column(mat, j);
		gsl_vector_add_constant(&cv.vector, -gsl_vector_get(_col_mean, j));
	}
}

inline void model::norm_mat(gsl_matrix *mat)
{
	size_t ncol = mat->size2;

	for (size_t j = 0; j < ncol; ++j) {
		gsl_vector_view cv = gsl_matrix_column(mat, j);
		double d = gsl_vector_get(_col_sd, j);
		if (d > 1e-6)
			gsl_vector_scale(&cv.vector, 1/d);
	}
}

int model::train(const dataset &ds)
{
	int ret = -1;
	gsl_matrix *km = NULL;
	gsl_matrix *ikm = NULL;
	gsl_permutation *perm = NULL;
	gsl_vector_view dv;

	gsl_matrix_free(_ikm);

	if (load_training_data(ds)) {
		ULIB_FATAL("couldn't load training data");
		goto done;
	}
	dbg_print_mat(_fm, "Feature Matrix:");
	if (get_col_mean()) {
		ULIB_FATAL("couldn't get feature column means");
		goto done;
	}
	zero_out_mat(_fm);
	if (get_col_sd()) {
		ULIB_FATAL("couldn't get feature column standard deviations");
		goto done;
	}
	norm_mat(_fm);
	dbg_print_mat(_fm, "Normalized Feature Matrix:");

	km = comp_kern_mat(_fm, _kern);
	dbg_print_mat(km, "Kernel Matrix:");
	if (km == NULL) {
		ULIB_FATAL("couldn't compute kernel matrix");
		goto done;
	}

	dv = gsl_matrix_diagonal(km);
	gsl_vector_add_constant(&dv.vector, _noise_var);

	ikm = gsl_matrix_alloc(km->size1, km->size2);
	if (ikm == NULL) {
		ULIB_FATAL("couldn't allocate cost model");
		goto done;
	}

	int signum;
	perm = gsl_permutation_alloc(ikm->size1);
	if (perm == NULL) {
		ULIB_FATAL("couldn't allocate cost model");
		goto done;
	}
	gsl_linalg_LU_decomp(km, perm, &signum);
	gsl_linalg_LU_invert(km, perm, ikm);
        gsl_vector_add_constant(_tv, -_t_avg);

	_ikm = ikm;
	ikm = NULL;
	ret = 0;
done:
	gsl_permutation_free(perm);
	gsl_matrix_free(km);
	gsl_matrix_free(ikm);

	return ret;
}

int model::predict(const dataset &tds, gsl_matrix **pp)
{
	int ret = -1;

	gsl_matrix *mat = NULL;
	gsl_matrix *ptv = NULL;
	gsl_matrix *km1 = NULL;
	gsl_matrix *km2 = NULL;
	gsl_matrix *res = NULL;
	gsl_matrix *stm = NULL;

        gsl_vector_view avg_col;
        gsl_vector_view dv;
        
	if (tds.ins_num() <= 0 || tds.fea_num() != (int)_col_mean->size) {
		ULIB_FATAL("invalid test dimensions, (ins_num=%d,fea_num=%d)",
                           tds.ins_num(), tds.fea_num());
		goto done;
	}

	mat = gsl_matrix_alloc(tds.ins_num(), tds.fea_num());
	if (mat == NULL) {
		ULIB_FATAL("couldn't allocate test feature matrix");
		goto done;
	}
	ptv = gsl_matrix_alloc(tds.ins_num(), 2);
	if (ptv == NULL) {
		ULIB_FATAL("couldn't allocate prediction matrix");
		goto done;
	}
        if (tds.get_matrix(mat)) {
                ULIB_FATAL("couldn't get test matrix");
                goto done;
        }
	dbg_print_mat(mat, "Test Matrix:");

	zero_out_mat(mat);
	norm_mat(mat);

	dbg_print_mat(mat, "Normalized Test Matrix:");

	km1 = comp_kern_mat(mat, _fm, _kern);
	if (km1 == NULL) {
		ULIB_FATAL("couldn't compute test1 kernel matrix");
		goto done;
	}
	dbg_print_mat(km1, "Test Kernel Matrix:");

	km2 = comp_kern_mat(mat, mat, _kern);
	if (km2 == NULL) {
		ULIB_FATAL("couldn't compute test2 kernel matrix");
		goto done;
	}
	dbg_print_mat(km1, "Test Kernel Matrix:");
	dv = gsl_matrix_diagonal(km2);

	res = gsl_matrix_alloc(km1->size1, _ikm->size2);
	if (res == NULL) {
		ULIB_FATAL("couldn't allocate temporary matrix");
		goto done;
	}
	stm = gsl_matrix_alloc(km2->size1, km2->size2);
	if (stm == NULL) {
		ULIB_FATAL("couldn't allocate std matrix");
		goto done;
	}

	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, km1, _ikm, 0.0, res);
	gsl_blas_dgemm(CblasNoTrans, CblasTrans, 1.0, res, km1, 0.0, stm);
        gsl_matrix_sub(km2, stm);
	dbg_print_mat(res, "Predictive Matrix:");
        avg_col = gsl_matrix_column(ptv, 0);
	gsl_blas_dgemv(CblasNoTrans, 1.0, res, _tv, 0.0, &avg_col.vector);
        gsl_vector_add_constant(&avg_col.vector, _t_avg);
        gsl_matrix_scale(km2, _t_std*_t_std);
	gsl_vector_add_constant(&dv.vector, _noise_var);
        for (size_t i = 0; i < km2->size1; ++i)
                gsl_matrix_set(ptv, i, 1, sqrt(gsl_vector_get(&dv.vector, i)));
	*pp = ptv;
	ptv = NULL;
	ret = 0;
done:
	gsl_matrix_free(mat);
	gsl_matrix_free(ptv);
	gsl_matrix_free(km1);
	gsl_matrix_free(km2);
	gsl_matrix_free(res);
	gsl_matrix_free(stm);
	return ret;
}

}
