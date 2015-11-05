#include "libgpr.h"
#include "flags.h"

namespace gpr {

gpr_model::gpr_model()
{
        if (FLAGS_kern == "poly")
                _kern = new poly_kern(
                    FLAGS_polykern_tradeoff,
                    FLAGS_polykern_order);
        else
                _kern = new rbf_kern(FLAGS_rbfkern_sigma_sq);

        _model = new model(FLAGS_noise_var, _kern);
}

gpr_model::~gpr_model()
{
        delete _model;
        delete _kern;
}

int gpr_model::train(const dataset &ds)
{
        return _model->train(ds);
}

int gpr_model::predict(const dataset &tds, gsl_matrix **pp)
{
        return _model->predict(tds, pp);
}

}
