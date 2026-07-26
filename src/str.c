#include "../include/str.h"
#include "../include/array.h"
#include <stdarg.h>
#include <stdio.h>

dyn_string_t *str_split(const char *string, char delimiter,
                        Allocator *allocator) {
  dyn_string_t *strs = array_new(dyn_string_t, allocator);

  size_t len = strlen(string);

  dyn_string_t cur_str = {0};
  dyn_string_init(&cur_str, allocator);
  for (size_t i = 0; i < len; i++) {
    if (string[i] == delimiter && cur_str.len > 0) {
      dyn_string_t new_str = {0};
      dyn_string_copy(&new_str, &cur_str);

      array_add(strs, new_str);

      dyn_string_clear(&cur_str);
    } else {
      dyn_string_add_char(&cur_str, string[i]);
    }
  }

  if (cur_str.len > 0) {
    array_add(strs, cur_str);
  }

  return strs;
}

char *str_fmt_temp(const char *fmt, ...) {
  static char temp_fmt_buffer[4096];
  va_list list;
  va_start(list, fmt);
  vsnprintf(temp_fmt_buffer, sizeof(temp_fmt_buffer), fmt, list);
  va_end(list);
  return temp_fmt_buffer;
}

char *str_dup(const char *src, Allocator *alloc) {
  size_t str_len = strlen(src);
  char *dest_buf = alloc->alloc(alloc, str_len);
  strncpy(dest_buf, src, str_len);
  return dest_buf;
}
