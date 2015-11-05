#ifndef GPR_DATASET_H
#define GPR_DATASET_H

#include <gsl/gsl_matrix.h>

namespace gpr {

class dataset {
public:
        virtual ~dataset() { }

        virtual int    fea_num() const = 0;
        virtual int    ins_num() const = 0;
        virtual int    get_target(gsl_vector *v) const;
        virtual int    get_matrix(gsl_matrix *m) const;
        // get the target value of the instance
        virtual double get_tgv(int ins_idx) const = 0;
        // get the @fea_idx-th feature of the instance
        virtual double get_fea(int ins_idx, int fea_idx) const = 0;
};

}

#endif
