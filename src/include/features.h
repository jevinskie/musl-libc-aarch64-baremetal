#ifndef FEATURES_H
#define FEATURES_H

#include "../../include/features.h"

#define weak __attribute__((__weak__))
#define hidden __attribute__((__visibility__("hidden")))
#ifndef __APPLE__
#define weak_alias(old_sym, new_sym) \
	extern __typeof(old_sym) new_sym __attribute__((__weak__, __alias__(#old_sym)))
#else
#define weak_alias(old_sym, new_sym) \
	extern __typeof(old_sym) new_sym __attribute__((__weak__, weak_import))
#endif
#endif
