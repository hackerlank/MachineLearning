#include "flags.h"
#include "debug.h"

namespace gpr {

void print_mat(const gsl_matrix *mat, const char *cap)
{
	if (!FLAGS_verbose)
		return;
	if (cap)
		printf("%s\n"
		       "--------------------------\n", cap);
	else
		printf("\nMatrix:\n"
		       "--------------------------\n");
	for (size_t i = 0; i < mat->size1; ++i) {
		printf("[%zu,]\t", i);
		for (size_t j = 0; j < mat->size2; ++j) {
			printf("%10.3lf", gsl_matrix_get(mat, i, j));
			if (j == mat->size2 - 1)
				putchar('\n');
		}
	}
	printf("\n");
}

}
