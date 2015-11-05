#ifndef _GPR_COMMON_H
#define _GPR_COMMON_H

#include <ulib/util_log.h>

#define RETURN_IF_TRUE(cond, ret, msg, ...) do {	\
		if (cond) {				\
			ULIB_FATAL(msg, ##__VA_ARGS__);	\
			return ret;			\
		}					\
	} while (0)

#endif
