#include "../include/str.h"
#include "../include/array.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

usz str_len(const char *str) {
  if (str == NULL)
    return 0;

  return strlen(str);
}

dyn_string_t *str_split(const char *string, char delimiter,
                        allocator_t *allocator) {
  dyn_string_t *strs = array_new(dyn_string_t, allocator);

  size_t len = str_len(string);

  dyn_string_t cur_str = {0};
  dyn_string_init(&cur_str, allocator);
  for (size_t i = 0; i < len; i++) {
    if (string[i] == delimiter && cur_str.len > 0) {
      dyn_string_t new_str = {0};
      dyn_string_init(&new_str, allocator);

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

char *str_lower_temp(const char *str) {
  static char temp_lower_buffer[4096];

  temp_lower_buffer[0] = '\0';

  usz len = str_len(str);
  if (len > sizeof(temp_lower_buffer))
    len = 4095;

  for (usz i = 0; i < len; i++) {
    temp_lower_buffer[i] = tolower(str[i]);
  }
  temp_lower_buffer[len] = '\0';

  return temp_lower_buffer;
}

char *str_upper_temp(const char *str) {
  static char temp_upper_buffer[4096];

  temp_upper_buffer[0] = '\0';

  usz len = str_len(str);
  if (len > sizeof(temp_upper_buffer))
    len = 4095;

  for (usz i = 0; i < len; i++) {
    temp_upper_buffer[i] = tolower(str[i]);
  }
  temp_upper_buffer[len] = '\0';

  return temp_upper_buffer;
}

char *str_dup(const char *src, allocator_t *alloc) {
  return str_dup_len(src, str_len(src), alloc);
}

char *str_dup_len(const char *src, usz len, allocator_t *alloc) {
  char *dest_buf = alloc->alloc(alloc, len + 1);
  strncpy(dest_buf, src, len + 1);
  return dest_buf;
}

bool str_eq(const char *str_a, const char *str_b) {
  usz len_a = str_len(str_a);
  usz len_b = str_len(str_b);
  return str_eq_len(str_a, str_b, max(len_a, len_b));
}

bool str_eq_len(const char *str_a, const char *str_b, usz len) {
  if (str_a == NULL || str_b == NULL)
    return false;

  return strncmp(str_a, str_b, len) == 0;
}

bool str_contains_ch(const char *str, char ch, const char **found_idx) {
  return str_contains_ch_len(str, ch, str_len(str), found_idx);
}

bool str_contains_ch_len(const char *str, char ch, usz len, const char **found_idx) {
  if (str == NULL)
    return false;

  usz slen = str_len(str);
  if (slen == 0)
    return false;

  if (len > slen)
    len = slen;

  for (usz ichar = 0; ichar < len; ichar++) {
    if (str[ichar] == ch) {
      if (found_idx != NULL) {
        *found_idx = &str[ichar];
      }
      return true;
    }
  }

  return false;
}

bool str_contains_str(const char *str, const char *query, const char **found_idx) {
  return str_contains_str_len(str, query, str_len(str), found_idx);
}

bool str_contains_str_len(const char *str, const char *query, usz len,
                          const char **found_idx) {
  if (str == NULL)
    return false;

  usz slen = str_len(str);
  if (slen == 0)
    return false;

  if (len > slen)
    len = slen;

  usz query_len = str_len(query);

  if (query_len > len)
    return false;

  bool query_begin = false;
  usz query_idx = 0;
  for (usz ichar = 0; ichar < len; ichar++) {
    if (!query_begin && str[ichar] == query[0]) {
      query_begin = true;
      query_idx = 0;
    }

    if (query_begin) {
      if (query_idx == query_len) {
        if (found_idx != NULL) {
          *found_idx = &str[ichar];
        }
        return true;
      }

      if (str[ichar] != query[query_idx]) {
        query_begin = false;
        query_idx = 0;
      } else {
        ++query_idx;
      }
    }
  }

  return false;
}
