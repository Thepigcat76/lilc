#include "../include/path.h"
#include "../include/log.h"
#include "../include/numbers.h"
#include "../include/str.h"
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#define PATH_MAX MAX_PATH
#endif

// Returns false if the normalized path would not fit in PATH_MAX.
bool file_path_resolve(dyn_string_t *str) {
  char tmp_path[PATH_MAX + 1];
  usz component_start[PATH_MAX + 1];
  usz component_count = 0;
  usz output_len = 0;
  usz input_idx = 0;

  const usz input_len = str->len;
  const bool absolute = input_len > 0 && str->string[0] == '/';
  const bool trailing_slash =
      input_len > 0 && str->string[input_len - 1] == '/';

  // Preserve the fact that the path is absolute.
  if (absolute) {
    tmp_path[output_len++] = '/';
  }

  while (input_idx < input_len) {
    // Skip repeated slashes.
    while (input_idx < input_len && str->string[input_idx] == '/') {
      input_idx++;
    }

    if (input_idx >= input_len) {
      break;
    }

    const usz component_begin = input_idx;

    while (input_idx < input_len && str->string[input_idx] != '/') {
      input_idx++;
    }

    const usz component_len = input_idx - component_begin;

    // "." has no effect.
    if (component_len == 1 &&
        str->string[component_begin] == '.') {
      continue;
    }

    // Handle "..".
    if (component_len == 2 &&
        str->string[component_begin] == '.' &&
        str->string[component_begin + 1] == '.') {
      if (component_count > 0) {
        /*
         * Remove the previous normal component.

         * component_start points to the slash before the component,
         * or to the beginning of the output for the first component.
         */
        output_len = component_start[--component_count];
        continue;
      }

      if (absolute) {
        /*
         * An absolute path cannot go above its root:
         *
         *   /../a -> /a
         */
        continue;
      }

      /*
       * For a relative path, preserve leading "..":
       *
       *   ../../a -> ../../a
       */
      if (output_len > 0 && tmp_path[output_len - 1] != '/') {
        if (output_len + 1 >= sizeof(tmp_path)) {
          return false;
        }

        tmp_path[output_len++] = '/';
      }

      if (output_len + 2 >= sizeof(tmp_path)) {
        return false;
      }

      tmp_path[output_len++] = '.';
      tmp_path[output_len++] = '.';
      continue;
    }

    /*
     * Append a normal component.

     * Record the position before its separating slash so that ".."
     * can remove both the slash and the component.
     */
    if (output_len > 0 && tmp_path[output_len - 1] != '/') {
      if (output_len + 1 >= sizeof(tmp_path)) {
        return false;
      }

      component_start[component_count++] = output_len;
      tmp_path[output_len++] = '/';
    } else {
      component_start[component_count++] = output_len;
    }

    if (output_len + component_len >= sizeof(tmp_path)) {
      return false;
    }

    memcpy(
        tmp_path + output_len,
        str->string + component_begin,
        component_len
    );

    output_len += component_len;
  }

  /*
   * "." by itself should resolve to ".".
   *
   * Empty input remains empty. An absolute path containing only "." or ".."
   * resolves to "/".
   */
  if (output_len == 0 && input_len > 0 && !absolute) {
    tmp_path[output_len++] = '.';
  }

  // Preserve a trailing slash, except for the root path.
  if (trailing_slash &&
      output_len > 0 &&
      tmp_path[output_len - 1] != '/') {
    if (output_len + 1 >= sizeof(tmp_path)) {
      return false;
    }

    tmp_path[output_len++] = '/';
  }

  tmp_path[output_len] = '\0';

  /*
   * Normalization never produces a result longer than the input path,
   * except for the special empty-to-"." case, which is intentionally avoided.
   * Therefore the existing dyn_string allocation should be sufficient.
   */
  memcpy(str->string, tmp_path, output_len + 1);
  str->len = output_len;
  str->term_len = str->len + 1;

  return true;
}

static void _internal_file_path_parse(dyn_string_t *str, bool front,
                                      bool insert_slash, const char *path) {
  usz len = str_len(path);
  if (len == 0) {
    return;
  }

  bool slash = false;

  usz str_max_len = str->len + len + 1;
  usz str_prev_len = str->len;

  if (str->len > 0) {
    if (!front && insert_slash && str->string[str->len - 1] != '/' &&
        path[0] != '/') {
      dyn_string_add_char(str, '/');
    }

    if (!front) {
      slash = str->string[str->len - 1] == '/';
    }

    // Extra +1 is for slash that we might have to insert
    dyn_string_ensure_size(str, str_max_len + 1);
  }

  bool parsing_full_component = false;
  usz full_components = 0;

  for (usz ichar = 0; ichar < len; ichar++) {
    if (path[ichar] == '/') {
      log_debug("Slash");
      if (!slash) {
        log_debug("adding Slash");
        dyn_string_add_char(str, '/');
        slash = true;
        if (parsing_full_component) {
          log_debug("incr full comps");
          full_components++;
          parsing_full_component = false;
        }
      }
      continue;
    }

    slash = false;
    dyn_string_add_char(str, path[ichar]);
    parsing_full_component = true;
    if (parsing_full_component) {
      log_debug("Full comp char: '%c'", path[ichar]);
    }
  }

  log_debug("Full components: %zu", full_components);

  if (front) {
    bool old_str_front_slash =
        str_prev_len > 0 && str->string[0] == '/' && path[len - 1] == '/';

    // TODO: Test with empty str as path
    bool insert_slash_between_strings =
        (insert_slash && str_prev_len > 0 && str->string[0] != '/' &&
         path[len - 1] != '/');

    char *new_str_begin = str->string + str_prev_len;
    usz new_str_len = str->len - str_prev_len;
    // Copy added string to tmp buf
    char new_str_buf[new_str_len + 1];
    strncpy(new_str_buf, new_str_begin, new_str_len);

    char *old_str_start = str->string + new_str_len;
    if (insert_slash_between_strings) {
      old_str_start += 1;
    }
    usz old_str_len = str_prev_len;
    if (old_str_front_slash) {
      old_str_len -= 1;
    }
    // Offset old str by len of new str
    strncpy(old_str_start, str->string + (old_str_front_slash ? 1 : 0),
            old_str_len);

    if (insert_slash_between_strings) {
      str->string[new_str_len] = '/';
    }

    // Copy new str to front
    strncpy(str->string, new_str_buf, new_str_len);

    str->string[old_str_len + new_str_len] = '\0';
    str->len = old_str_len + new_str_len;
    str->term_len = str->len + 1;
  }

  if (!file_path_resolve(str)) {
    log_error("Failed to resolve path, path too long");
  }
}

static void _internal_file_path_parse_va(dyn_string_t *str, bool front,
                                         bool insert_slash, const char *fmt,
                                         va_list args) {
  char buf[PATH_MAX + 1];

  vsnprintf(buf, PATH_MAX + 1, fmt, args);

  _internal_file_path_parse(str, front, insert_slash, buf);
}

void file_path_parse(dyn_string_t *str, const char *path) {
  dyn_string_clear(str);

  _internal_file_path_parse(str, false, false, path);
}

dyn_string_t file_path_makef(allocator_t *alloc, const char *fmt, ...) {
  dyn_string_t str = {0};
  dyn_string_init(&str, alloc);

  va_list args;
  va_start(args, fmt);
  _internal_file_path_parse_va(&str, false, false, fmt, args);
  va_end(args);

  return str;
}

void file_path_extend_front(dyn_string_t *str, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  _internal_file_path_parse_va(str, true, true, fmt, args);
  va_end(args);
}

void file_path_extend_back(dyn_string_t *str, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  _internal_file_path_parse_va(str, false, true, fmt, args);
  va_end(args);
}
