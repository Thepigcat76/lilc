#include "../include/str.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CAPACITY 16

void dyn_string_init(dyn_string_t *str) {
  if (str->capacity <= 0)
    str->capacity = DEFAULT_CAPACITY;
  str->string = malloc(str->capacity);
  str->string[0] = '\0';
  str->len = 0;
  str->term_len = 1;
}

void dyn_string_add_char(dyn_string_t *str, char c) {
  if (str->term_len + 1 >= str->capacity) {
    str->capacity *= 2;
    str->string = realloc(str->string, str->capacity);
  }

  str->string[str->len] = c;
  str->string[str->term_len] = '\0';

  str->len++;
  str->term_len++;
}

void dyn_string_add_str(dyn_string_t *str, const char *c) {
  size_t len = strlen(c);
  if (str->term_len + len >= str->capacity) {
    if (str->term_len + len >= str->capacity * 2) {
      str->capacity = str->capacity + len + 1;
    } else {
      str->capacity *= 2;
    }

    str->string = realloc(str->string, str->capacity);
  }

  for (size_t i = 0; i < len; i++) {
    str->string[str->len] = c[i];
    str->len++;
    str->term_len++;
  }
  str->string[str->len] = '\0';
}

static void dyn_string_reserve(dyn_string_t *str, size_t len) {
  if (str->capacity <= len + 1) {
    str->capacity = len + 1;
    str->string = realloc(str->string, str->capacity);
  }
}

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 2, 3)))
#endif
void dyn_string_printf(dyn_string_t *str, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);

    int len = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    if (len < 0) {
        va_end(args);
        return;
    }

    dyn_string_reserve(str, str->term_len + len);

    vsnprintf(str->string, len + 1, fmt, args);

    str->len = len;
    str->term_len = len + 1;

    va_end(args);
}

void dyn_string_copy(dyn_string_t *dest, const dyn_string_t *src) {
  if (src->term_len >= dest->capacity) {
    dest->capacity = src->term_len;
    dest->string = realloc(dest->string, dest->capacity);
  }

  strcpy(dest->string, src->string);
  dest->len = src->len;
  dest->term_len = src->term_len;
}

void dyn_string_copy_str(dyn_string_t *dest, const char *src) {
  size_t src_len = strlen(src) + 1;

  if (src_len >= dest->capacity) {
    dest->capacity = src_len;
    dest->string = realloc(dest->string, dest->capacity);
  }

  strcpy(dest->string, src);
  dest->len = src_len - 1;
  dest->term_len = src_len;
}

void dyn_string_clear(dyn_string_t *str) {
  str->len = 0;
  str->term_len = 1;
  str->string[0] = '\0';
}

void dyn_string_free(dyn_string_t *str) {
  free(str->string);
  str->len = 0;
  str->term_len = 0;
  str->capacity = 0;
}
