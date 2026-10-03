#include "../include/path.h"
#include "../include/numbers.h"
#include "../include/str.h"
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>

static void _internal_file_path_parse(dyn_string_t *str, const char *path) {
  usz len = str_len(path);

  bool slash = false;

  if (str->len > 0) {
    if (str->string[str->len - 1] == '/') {
      slash = true;
    }
  }

  for (usz ichar = 0; ichar < len; ichar++) {
    if (path[ichar] == '/') {
      if (!slash) {
        dyn_string_add_char(str, '/');
        slash = true;
      }
      continue;
    }

    slash = false;
    dyn_string_add_char(str, path[ichar]);
  }
}

static void _internal_file_path_parse_va(dyn_string_t *str, bool front, const char *fmt,
                                         va_list args) {
  char buf[PATH_MAX + 1];

  vsnprintf(buf, PATH_MAX, fmt, args);

  _internal_file_path_parse(str, buf);
}

void file_path_parse(dyn_string_t *str, const char *path) {
  dyn_string_clear(str);

  _internal_file_path_parse(str, path);
}

dyn_string_t file_path_makef(allocator_t *alloc, const char *fmt, ...) {
  dyn_string_t str = {0};
  dyn_string_init(&str, alloc);

  va_list args;
  va_start(args, fmt);
  _internal_file_path_parse_va(&str, fmt, args);
  va_end(args);

  return str;
}

void file_path_extend_front(dyn_string_t *str, const char *fmt, ...) {

}

void file_path_extend_back(dyn_string_t *str, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  _internal_file_path_parse_va(str, fmt, args);
  va_end(args);
}
