#include "../include/_path.h"
#include "../include/log.h"
#include "../include/numbers.h"
#include "../include/str.h"
#include <limits.h>
#include <linux/limits.h>
#include <stdarg.h>
#include <stdio.h>

void file_path_init(file_path_t *filepath, allocator_t *alloc) {
  filepath->alloc = alloc;
  deque_init(filepath->parts, alloc);
  filepath->literal_path = NULL;
}

void file_path_deinit(file_path_t *filepath) {
  char **part;
  deque_foreach(filepath->parts, part) {
    filepath->alloc->dealloc(filepath->alloc, *part);
  }
  deque_deinit(filepath->parts);
  filepath->alloc->dealloc(filepath->alloc, filepath->literal_path);
}

static bool _internal_file_path_parse(file_path_t *filepath,
                                      const char *path_literal,
                                      bool append_front) {
  usz len = str_len(path_literal);

  if (len <= 0) {
    return true;
  }

  if (len > PATH_MAX) {
    return false;
  }

  bool path_empty = filepath->literal_path == NULL;

  if (filepath->literal_path != NULL) {
    usz old_len = str_len(filepath->literal_path);
    if (!append_front) {
      bool insert_slash =
          filepath->literal_path[old_len - 1] != '/' && path_literal[0] != '/';
      filepath->literal_path = filepath->alloc->realloc(
          filepath->alloc, filepath->literal_path, old_len + 1,
          old_len + len + (insert_slash ? 2 : 1));
      if (insert_slash) {
        strcat(filepath->literal_path, "/");
      }
      strcat(filepath->literal_path, path_literal);
    } else {
      bool insert_slash =
          filepath->literal_path[0] != '/' && path_literal[len - 1] != '/';
      char *tmp = filepath->literal_path;
      filepath->literal_path = filepath->alloc->alloc(
          filepath->alloc, old_len + len + (insert_slash ? 2 : 1));

      strncpy(filepath->literal_path, path_literal, len);
      if (insert_slash) {
        strcat(filepath->literal_path, "/");
      }
      strcat(filepath->literal_path, tmp);
      filepath->alloc->dealloc(filepath->alloc, tmp);
    }
  } else {
    filepath->literal_path = str_dup(path_literal, filepath->alloc);
  }

  bool part_init = false;
  dyn_string_t part = {0};

  if ((append_front || path_empty) && path_literal[0] == '/') {
    filepath->absolute_path = true;
  }

  array_t(char *) append_buf;
  if (append_front)
    append_buf = array_new(char *, &HEAP_ALLOCATOR);

  for (usz ichar = 0; ichar < len; ichar++) {
    if (path_literal[ichar] == '/') {
      if (part_init) {
        char *part_cpy = str_dup(part.string, filepath->alloc);
        if (!append_front)
          deque_push_back(filepath->parts, part_cpy);
        else
          array_add(append_buf, part_cpy);
        dyn_string_free(&part);
        part_init = false;
      }
      continue;
    }

    if (!part_init) {
      dyn_string_init(&part, filepath->alloc);
      part_init = true;
    }

    dyn_string_add_char(&part, path_literal[ichar]);
  }
  if (part_init) {
    char *part_cpy = str_dup(part.string, filepath->alloc);
    if (!append_front)
      deque_push_back(filepath->parts, part_cpy);
    else
      array_add(append_buf, part_cpy);
    dyn_string_free(&part);
    part_init = false;
  }

  if (append_front) {
    for (isz i = array_len(append_buf); i-- > 0;) {
      char *part_cpy = append_buf[i];
      deque_push_front(filepath->parts, part_cpy);
    }

    array_free(append_buf);
  }

  return true;
}

bool file_path_parse(file_path_t *filepath, const char *path_literal) {
  if (filepath->literal_path != NULL) {
    filepath->alloc->dealloc(filepath->alloc, filepath->literal_path);
    filepath->literal_path = NULL;
  }

  deque_reset(filepath->parts);
  filepath->absolute_path = false;

  return _internal_file_path_parse(filepath, path_literal, false);
}

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 2, 3)))
#endif
void file_path_extend_back(file_path_t *filepath, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  char buf[PATH_MAX + 1];
  usz len = vsnprintf(buf, PATH_MAX, fmt, args);
  if (len > PATH_MAX) {
    va_end(args);
    log_error("Path longer than PATH_MAX");
    return;
  }

  va_end(args);

  _internal_file_path_parse(filepath, buf, false);
}

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 2, 3)))
#endif
void file_path_extend_front(file_path_t *filepath, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  char buf[PATH_MAX + 1];
  usz len = vsnprintf(buf, PATH_MAX, fmt, args);
  if (len > PATH_MAX) {
    va_end(args);
    log_error("Path longer than PATH_MAX");
    return;
  }

  va_end(args);

  _internal_file_path_parse(filepath, buf, true);
}

void file_path_push_back(file_path_t *filepath, const char *part) {
  deque_push_back(filepath->parts, str_dup(part, filepath->alloc));
}

void file_path_push_front(file_path_t *filepath, const char *part) {
  deque_push_front(filepath->parts, str_dup(part, filepath->alloc));
}

char *file_path_pop_back(file_path_t *filepath) {
  return (char *)deque_pop_back(filepath->parts);
}

char *file_path_pop_front(file_path_t *filepath) {
  return (char *)deque_pop_front(filepath->parts);
}

dyn_string_t file_path_format(const file_path_t *filepath, allocator_t *alloc) {
  dyn_string_t out = {0};
  dyn_string_init(&out, alloc);

  if (filepath->absolute_path) {
    dyn_string_add_char(&out, '/');
  }

  char **part;
  deque_foreach(filepath->parts, part) {
    dyn_string_add_str(&out, *part);
    if (_internal_deque_idx != deque_len(filepath->parts) - 1) {
      dyn_string_add_char(&out, '/');
    }
  }

  return out;
}

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 2, 3)))
#endif
file_path_t file_path_makef(allocator_t *alloc, const char *fmt, ...) {
  file_path_t path = {0};
  file_path_init(&path, alloc);

  va_list args;
  va_start(args, fmt);

  char buf[PATH_MAX + 1];
  usz len = vsnprintf(buf, PATH_MAX, fmt, NULL);
  if (len > PATH_MAX) {
    va_end(args);
    log_error("Path longer than PATH_MAX");
    return path;
  }

  va_end(args);

  file_path_parse(&path, buf);

  return path;
}

bool file_path_eq(const file_path_t *path0, const file_path_t *path1) {
  if (path0->absolute_path != path1->absolute_path) {
    return false;
  }
  usz len = deque_len(path0->parts);
  if (len != deque_len(path1->parts)) {
    return false;
  }

  for (usz i = 0; i < len; i++) {
    char *part0 = *deque_at(path0->parts, i);
    char *part1 = *deque_at(path1->parts, i);

    if (!str_eq(part0, part1)) {
      return false;
    }
  }

  return true;
}

bool file_path_eq_strict(const file_path_t *path0, const file_path_t *path1) {
  if (path0->absolute_path != path1->absolute_path) {
    return false;
  }
  usz len = deque_len(path0->parts);
  if (len != deque_len(path1->parts)) {
    return false;
  }

  if (!str_eq(path0->literal_path, path1->literal_path)) {
    return false;
  }

  for (usz i = 0; i < len; i++) {
    char *part0 = *deque_at(path0->parts, i);
    char *part1 = *deque_at(path1->parts, i);

    if (!str_eq(part0, part1)) {
      return false;
    }
  }

  return true;
}

void file_path_copy(file_path_t *dest, file_path_t *src) {
  char **part;
  deque_foreach(src->parts, part) { deque_push_back(dest->parts, *part); }

  if (dest->literal_path != NULL) {
    dest->alloc->dealloc(dest->alloc, dest->literal_path);
  }
  dest->literal_path = str_dup(src->literal_path, dest->alloc);
  dest->absolute_path = src->absolute_path;
}
