#ifndef _GPR_DEBUG_H
#define _GPR_DEBUG_H

#include <stdio.h>
#include <gsl/gsl_matrix.h>

#ifdef NDEBUG
#define dbg_print_mat(mat,cap)
#else
#define dbg_print_mat(mat,cap) print_mat(mat,cap)
#endif

namespace gpr {

// print the content of a matrix
void print_mat(const gsl_matrix *mat, const char *cap = NULL);

}

#endif  // _GPR_DEBUG_H
