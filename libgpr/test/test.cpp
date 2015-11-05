#include <math.h>
#include <gflags/gflags.h>
#include "libgpr.h"

using namespace gpr;
using namespace google;

class training_dataset : public dataset
{
public:
        int fea_num() const
        {
                return 1;
        }

        int ins_num() const
        {
                return 10;
        }

        double get_tgv(int idx) const
        {
                return sin(idx/2.0);
        }

        double get_fea(int ins_idx, int fea_idx) const
        {
                return ins_idx;
        }
};

class test_dataset : public dataset
{
public:
        int fea_num() const
        {
                return 1;
        }

        int ins_num() const
        {
                return 10;
        }

        double get_tgv(int idx) const
        {
                fprintf(stderr, "error\n");
                return -1.0;
        }

        double get_fea(int ins_idx, int fea_idx) const
        {
                return ins_idx + 0.5;
        }
};

int main(int argc, char *argv[])
{
        gpr_model m;
        training_dataset train;
        test_dataset test;

	ParseCommandLineFlags(&argc, &argv, true);

        if (m.train(train)) {
                fprintf(stderr, "training failed\n");
                return -1;
        }

        gsl_matrix *p = NULL;
        if (m.predict(test, &p)) {
                fprintf(stderr, "prediction failed\n");
                return -1;
        }

        for (size_t i = 0; i < p->size1; ++i) {
                printf("%lf\t%lf\t%lf\n",
                       sin((i+0.5)/2.0), gsl_matrix_get(p,i,0), gsl_matrix_get(p,i,1));
        }

        gsl_matrix_free(p);

        return 0;
}
