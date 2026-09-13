#pragma once

#include "alloc.h"
#include "array.h"
#include "dynstr.h"
#include <stdbool.h>

array_t(dyn_string_t) str_split(const char *string, char delimiter, allocator_t *allocator);

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 1, 2)))
#endif
char *str_fmt_temp(const char *fmt, ...);

char *str_dup(const char *src, allocator_t *alloc);

bool str_eq(const char *str_a, const char *str_b);
