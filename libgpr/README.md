### libgpr - A C++ Gaussian Process Regression Library

### Example
See test/test.cpp.

### C++ Interface
A regression problem consists of a training and a testing
datasets. Each dataset inherits the dataset base class defined as
follows (namespace gpr).

``` c++
class dataset {
public:
        virtual ~dataset() { }
	// number of features in an instance
        virtual int    fea_num() const = 0;
	// number of instances in the dataset
        virtual int    ins_num() const = 0;
	// target value of an instance
        virtual int    get_target(gsl_vector *v) const;
	// feature matrix
        virtual int    get_matrix(gsl_matrix *m) const;
        // get the target value of the instance
        virtual double get_tgv(int ins_idx) const = 0;
        // get the @fea_idx-th feature of the instance
        virtual double get_fea(int ins_idx, int fea_idx) const = 0;
};
```

Once the datasets are defined, they can be used to model training and
prediction. The model class provides the interface.
``` c++
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
}
```

### Requirements
* gsl: http://www.gnu.org/software/gsl/gsl.html
* gflags: http://gflags.github.io/gflags/
* ulib: https://code.google.com/p/ulib/