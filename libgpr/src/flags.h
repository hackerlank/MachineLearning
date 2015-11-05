#ifndef _GPR_FLAGS_H
#define _GPR_FLAGS_H

#include <gflags/gflags.h>

namespace gpr {

// RBF kernel squared sigma
DECLARE_double(rbfkern_sigma_sq);
// Polynomial kernel tradeoff
DECLARE_double(polykern_tradeoff);
// Polynomial kernel order
DECLARE_int32(polykern_order);
// Noise variance.
DECLARE_double(noise_var);
// Maximum instance length allowed in training file
DECLARE_int32(inst_max_len);
// Kernel to use
DECLARE_string(kern);
// Maximum examples to use
DECLARE_int32(max_num_examples);
// Show status information
DECLARE_bool(verbose);

int setup_validators();

}

#endif
