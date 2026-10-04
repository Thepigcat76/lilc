#pragma once

#include "numbers.h"

#define todo(...) _internal_todo(__LINE__, __FILE__ __VA_OPT__(,)  __VA_ARGS__ , NULL)
#define TODO(...) todo(__VA_ARGS__)

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 3, 4)))
#endif
void *_internal_todo(i32 line, const char *file, const char *fmt, ...);
