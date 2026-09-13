#include "../include/args.h"
#include "../include/str.h"
#include <stddef.h>

bool args_contains(usz arg_count, char *const *args, const char *val,
                   isz *out_idx) {
  if (out_idx != NULL)
    *out_idx = -1;
  for (usz i = 0; i < arg_count; i++) {
    if (str_eq(args[i], val)) {
      if (out_idx != NULL)
        *out_idx = i;
      return true;
    }
  }
  return false;
}

bool arg_eq(usz arg_count, char *const *args, usz idx, const char *val) {
  if (idx < arg_count) {
    return str_eq(args[idx], val);
  }
  return false;
}
