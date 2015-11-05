#include "dataset.h"
#include <assert.h>
#include <gsl/gsl_matrix.h>
#include <ulib/util_log.h>

namespace gpr {

int dataset::get_matrix(gsl_matrix *m) const
{
        int nrow = ins_num();
        int ncol = fea_num();

        if ((int)m->size1 != nrow
            || (int)m->size2 != ncol) {
                ULIB_FATAL("dataset dims do not match the output matrix");
                return -1;
        }

        for (int i = 0; i < nrow; ++i)
                for (int j = 0; j < ncol; ++j)
                        gsl_matrix_set(m, i, j, get_fea(i, j));

        return 0;
}

int dataset::get_target(gsl_vector *v) const
{
        int nrow = ins_num();

        if ((int)v->size != nrow) {
                ULIB_FATAL("dataset dims do not match the target vector");
                return -1;
        }

        for (int i = 0; i < nrow; ++i)
                gsl_vector_set(v, i, get_tgv(i));

        return 0;
}

}
