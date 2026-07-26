#pragma once

#include "numbers.h"

#define TODO(...) _internal_todo(__LINE__, __FILE__, __VA_ARGS__ __VA_OPT__(,) NULL)

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 3, 4)))
#endif
void *_internal_todo(i32 line, const char *file, const char *fmt, ...);
