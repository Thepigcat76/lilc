#pragma once

#include "alloc.h"
#include "dynstr.h"

dyn_string_t *str_split(const char *string, char delimiter, Allocator *allocator);

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 1, 2)))
#endif
char *str_fmt_temp(const char *fmt, ...);

char *str_dup(const char *src, Allocator *alloc);
