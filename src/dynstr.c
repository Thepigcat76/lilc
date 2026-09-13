#include "../include/dynstr.h"
#include "../include/log.h"
#include "../include/numbers.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dyn_string_ensure_cap(dyn_string_t *str, usz expected_unterm_cap) {
  if (str->capacity <= expected_unterm_cap + 1) {
    if (str->capacity * 2 >= expected_unterm_cap + 1) {
      str->capacity *= 2;
    } else {
      str->capacity = expected_unterm_cap + 1;
    }

    str->string = str->allocator->realloc(str->allocator, str->string,
                                          str->term_len, str->capacity);
  }
}

#define DEFAULT_CAPACITY 16

void dyn_string_init(dyn_string_t *str, allocator_t *allocator) {
  if (str->capacity <= 0)
    str->capacity = DEFAULT_CAPACITY;
  str->string = allocator->alloc(allocator, str->capacity);
  str->string[0] = '\0';
  str->len = 0;
  str->term_len = 1;
  str->allocator = allocator;
}

void dyn_string_add_char(dyn_string_t *str, char c) {
  dyn_string_ensure_cap(str, str->len + 1);

  str->string[str->len] = c;
  str->string[str->term_len] = '\0';

  str->len++;
  str->term_len++;
}

void dyn_string_add_str(dyn_string_t *str, const char *c) {
  usz len = strlen(c);
  dyn_string_ensure_cap(str, len);

  for (usz i = 0; i < len; i++) {
    str->string[str->len] = c[i];
    str->len++;
    str->term_len++;
  }
  str->string[str->len] = '\0';
}

static bool dyn_string_vprintf(dyn_string_t *str, usz append_idx,
                               const char *fmt, va_list args) {
  va_list args_copy;
  va_copy(args_copy, args);

  int len = vsnprintf(NULL, 0, fmt, args_copy);
  va_end(args_copy);

  if (len < 0) {
    return false;
  }

  dyn_string_ensure_cap(str, append_idx + len);

  vsnprintf(str->string + append_idx, len + 1, fmt, args);

  str->len = append_idx + len;
  str->term_len = append_idx + len + 1;

  return true;
}

void dyn_string_add_strf(dyn_string_t *str, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  if (!dyn_string_vprintf(str, str->len, fmt, args)) {
    log_error("Failed to format dyn string");
    va_end(args);
    exit(1);
  }

  va_end(args);
}

void dyn_string_remove_last_char(dyn_string_t *str) {
  if (str->len > 0) {
    --str->len;
    --str->term_len;
    str->string[str->len] = '\0';
  }
}

char *dyn_string_temp_copy_and_free(dyn_string_t str) {
  static char dyn_string_temp_copy_buf[8192];
  strcpy(dyn_string_temp_copy_buf, str.string);
  dyn_string_free(&str);
  return dyn_string_temp_copy_buf;
}

void dyn_string_printf(dyn_string_t *str, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  if (!dyn_string_vprintf(str, 0, fmt, args)) {
    log_error("Failed to format dyn string");
    va_end(args);
    exit(1);
  }

  va_end(args);
}

dyn_string_t dyn_string_makef(allocator_t *alloc, const char *fmt, ...) {
  dyn_string_t str = {0};
  dyn_string_init(&str, alloc);

  va_list args;
  va_start(args, fmt);

  if (!dyn_string_vprintf(&str, 0, fmt, args)) {
    log_error("Failed to format dyn string");
    va_end(args);
    exit(1);
  }

  va_end(args);

  return str;
}

void dyn_string_copy(dyn_string_t *dest, const dyn_string_t *src) {
  if (src->term_len >= dest->capacity) {
    dest->capacity = src->term_len;
    dest->string = dest->allocator->realloc(dest->allocator, dest->string,
                                            dest->term_len, dest->capacity);
  }

  strcpy(dest->string, src->string);
  dest->len = src->len;
  dest->term_len = src->term_len;
}

void dyn_string_copy_str(dyn_string_t *dest, const char *src) {
  usz src_len = strlen(src);

  dyn_string_copy_str_len(dest, src, src_len);
}

void dyn_string_copy_str_len(dyn_string_t *dest, const char *src, usz len) {
  len += 1;

  if (len >= dest->capacity) {
    dest->capacity = len;
    dest->string = dest->allocator->realloc(dest->allocator, dest->string,
                                            dest->term_len, dest->capacity);
  }

  strncpy(dest->string, src, len);
  dest->string[len - 1] = '\0';
  dest->len = len - 1;
  dest->term_len = len;
}

void dyn_string_clear(dyn_string_t *str) {
  str->len = 0;
  str->term_len = 1;
  str->string[0] = '\0';
}

void dyn_string_free(dyn_string_t *str) {
  if (str->string == NULL)
    return;

  str->allocator->dealloc(str->allocator, str->string);
  str->len = 0;
  str->term_len = 0;
  str->capacity = 0;
}
