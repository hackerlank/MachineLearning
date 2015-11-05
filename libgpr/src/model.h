#ifndef _GPR_MODEL_H
#define _GPR_MODEL_H

#include <string>
#include <gsl/gsl_matrix.h>
#include "kern.h"
#include "dataset.h"

namespace gpr {

class model {
public:
	model(double noise_var, const kernel *kern)
        : _noise_var(noise_var), _t_std(0),_kern(kern),
	  _tv(0), _fm(0), _col_mean(0), _col_sd(0), _ikm(0)
	{ }

	~model();

	int train(const dataset &ds);

        // Predict the target values for the given test instances @tds
        // if successful, @pp will be pointed to a two-column matrix,
        // where the first column corresponds to the predicted mean,
        // and the second column corresponds to the standard deviation
        // of the prediction
	int predict(const dataset &tds, gsl_matrix **pp);

private:
	int alloc_training_data(int nrow, int ncol);
	int load_training_data(const dataset &ds);
	// calculate the means of matrix columns
	int get_col_mean();
	// calculate the standard deviations of matrix columns
	int get_col_sd();
	// zero out matrix column means
	void zero_out_mat(gsl_matrix *mat);
	// normalize columns to unit variance
	void norm_mat(gsl_matrix *mat);

	double _noise_var;     // noise variance
        double _t_avg;         // target average
        double _t_std;         // target stdev
	const kernel *_kern;   // kernel object
	gsl_vector *_tv;       // target value vector
	gsl_matrix *_fm;       // feature matrix
	gsl_vector *_col_mean; // feature column means
	gsl_vector *_col_sd;   // feature column standard deviations
	gsl_matrix *_ikm;      // the model
};

}

#endif  // _GPR_MODEL_H
