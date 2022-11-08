#ifndef FEATURES_H
#define FEATURES_H

#include "../../include/features.h"

#define weak __attribute__((__weak__))
#define hidden __attribute__((__visibility__("hidden")))
#ifndef __APPLE__
#define weak_alias(old, new) \
	extern __typeof(old) new __attribute__((__weak__, __alias__(#old)))
#else
#define weak_alias(old, new) \
	__asm__(".globl _" #new); \
	__asm__("_" #new " = _" #old); \
	extern __typeof(old) new __attribute__((weak_import))
#endif
#endif
