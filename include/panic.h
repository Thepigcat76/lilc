#pragma once

#include "numbers.h"

#define panic(...) _internal_panic(__LINE__, __FILE__ __VA_OPT__(,) __VA_ARGS__)

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 3, 4)))
#endif
void *_internal_panic(i32 line, const char *file, const char *fmt, ...);
