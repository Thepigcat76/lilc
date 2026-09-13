#pragma once

#include "numbers.h"
#include <stdbool.h>

// idx can be NULL
bool args_contains(usz arg_count, char *const *args, const char *val, isz *out_idx);

bool arg_eq(usz arg_count, char *const *args, usz idx, const char *val);
