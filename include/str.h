#pragma once

#include "alloc.h"
#include "array.h"
#include "dynstr.h"
#include "numbers.h"
#include <stdbool.h>

usz str_len(const char *str);

array_t(dyn_string_t) str_split(const char *string, char delimiter, allocator_t *allocator);

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 1, 2)))
#endif
char *str_fmt_temp(const char *fmt, ...);

char *str_lower_temp(const char *fmt);

char *str_upper_temp(const char *fmt);

char *str_dup(const char *src, allocator_t *alloc);

char *str_dup_len(const char *src, usz len, allocator_t *alloc);

bool str_eq(const char *str_a, const char *str_b);

bool str_eq_len(const char *str_a, const char *str_b, usz len);

bool str_contains_ch(const char *str, char ch, const char **found_idx);

bool str_contains_ch_len(const char *str, char ch, usz len, const char **found_idx);

bool str_contains_str(const char *str, const char *query, const char **found_idx);

bool str_contains_str_len(const char *str, const char *query, usz len, const char **found_idx);
