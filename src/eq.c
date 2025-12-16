#include "../include/eq.h"
#include <string.h>

bool strv_eq(const void *a, const void *b) { return strcmp(a, b) == 0; }

bool str_ptrv_eq(const void *a, const void *b) {
  return strv_eq(*(char **)a, *(char **)b);
}

bool size_tv_eq(const void *a, const void *b) {
  return *(size_t *)a == *(size_t *)b;
}
