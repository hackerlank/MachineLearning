#ifndef GPR_LIBGPR_H
#define GPR_LIBGPR_H

#include <gsl/gsl_matrix.h>
#include "dataset.h"
#include "kern.h"
#include "model.h"

namespace gpr {

class gpr_model {
public:
        gpr_model();
        ~gpr_model();

        int train(const dataset &ds);
        int predict(const dataset &tds, gsl_matrix **pp);

private:
        kernel *_kern;
        model  *_model;
};

}

#endif
