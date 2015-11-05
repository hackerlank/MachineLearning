#include "flags.h"
#include "common.h"

using namespace std;
using namespace google;

namespace gpr {

static inline bool
validate_rbfkern_sigma_sq(const char *flagname, double value)
{
	(void)flagname;
	return value > 0;
}
DEFINE_double(rbfkern_sigma_sq, 3.0, "RBF kernel squared delta.");

static inline bool
validate_polykern_tradeoff(const char *flagname, double value)
{
	(void)flagname;
	return value >= 0;
}
DEFINE_double(polykern_tradeoff, 1.0, "Polynomial kernel tradeoff.");

static inline bool
validate_polykern_order(const char *flagname, int32 value)
{
	(void)flagname;
	return value > 0;
}
DEFINE_int32(polykern_order, 1, "Polynomial kernel order.");

static inline bool
validate_noise_var(const char *flagname, double value)
{
	(void)flagname;
	return value >= 0;
}
DEFINE_double(noise_var, 0.001, "Noise variance.");

static inline bool
validate_inst_max_len(const char *flagname, int32 value)
{
	(void)flagname;
	return value >= 64;
}
DEFINE_int32(inst_max_len, 8192, "Line max length. (>= 64)");

static inline bool
validate_kern(const char *flagname, const string &value)
{
	(void)flagname;
	return value == "poly" || value == "rbf";
}
DEFINE_string(kern, "rbf", "Kernel to use.");

static inline bool
validate_max_num_examples(const char *flagname, int32 value)
{
	(void)flagname;
	return value > 0;
}
DEFINE_int32(max_num_examples, 512, "Maximum number of examples to use. (>= 1)");

DEFINE_bool(verbose, false, "Be verbose.");

int setup_validators()
{
	RETURN_IF_TRUE(!RegisterFlagValidator(&FLAGS_rbfkern_sigma_sq, validate_rbfkern_sigma_sq),
		       -1, "rbfkern_sigma_sq is not valid");
	RETURN_IF_TRUE(!RegisterFlagValidator(&FLAGS_polykern_tradeoff, validate_polykern_tradeoff),
		       -1, "polykern_tradeoff is not valid");
	RETURN_IF_TRUE(!RegisterFlagValidator(&FLAGS_polykern_order, validate_polykern_order),
		       -1, "polykern_order is not valid");
	RETURN_IF_TRUE(!RegisterFlagValidator(&FLAGS_kern, validate_kern),
		       -1, "kern is not valid");
	RETURN_IF_TRUE(!RegisterFlagValidator(&FLAGS_noise_var, validate_noise_var),
		       -1, "noise_var is not valid");
	RETURN_IF_TRUE(!RegisterFlagValidator(&FLAGS_inst_max_len, validate_inst_max_len),
		       -1, "inst_max_len is not valid");
	RETURN_IF_TRUE(!RegisterFlagValidator(&FLAGS_max_num_examples, validate_max_num_examples),
		       -1, "max_num_examples is not valid");
	return 0;
}

}
