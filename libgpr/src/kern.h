#ifndef _GPR_KERN_H
#define _GPR_KERN_H

#include <gsl/gsl_matrix.h>

namespace gpr {

class kernel {
public:
	typedef double (*dot_func_t)(const gsl_vector *, const gsl_vector *);

	virtual ~kernel() { }

	virtual double
	operator()(const gsl_vector *,
		   const gsl_vector *,
		   dot_func_t df = def_dot) const = 0;

protected:
	// default dot product function
	static double def_dot(const gsl_vector *, const gsl_vector *);
};

class rbf_kern : public kernel {
public:
	rbf_kern(double sigma_sq)
	: _sigma_sq(sigma_sq) { }

	virtual ~rbf_kern() { }

	virtual double
	operator()(const gsl_vector *,
		   const gsl_vector *,
		   dot_func_t df = def_dot) const;

private:
	double _sigma_sq;
};

class poly_kern : public kernel {
public:
	poly_kern(double tradeoff, double order)
	: _tradeoff(tradeoff), _order(order) { }

	virtual ~poly_kern() { }

	virtual double
	operator()(const gsl_vector *,
		   const gsl_vector *,
		   dot_func_t df = def_dot) const;

private:
	double _tradeoff;
	double _order;
};

// compute a kernel matrix using the given kernel
// mat1: left feature matrix
// mat2: right feature matrix
// kern: kernel instance
gsl_matrix *comp_kern_mat(
	const gsl_matrix *mat1,
	const gsl_matrix *mat2,
	const kernel *kern);

// compute square kernel matrix
gsl_matrix *comp_kern_mat(const gsl_matrix *mat1, const kernel *kern);

}

#endif  // _GPR_KERN_H
