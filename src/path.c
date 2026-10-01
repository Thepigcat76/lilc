#include "../include/path.h"
#include "../include/numbers.h"
#include "../include/str.h"
#include <limits.h>

void file_path_init(file_path_t *filepath, allocator_t *alloc) {
  filepath->alloc = alloc;
  deque_init(filepath->parts, alloc);
  dyn_string_init(&filepath->literal_path, alloc);
}

void file_path_deinit(file_path_t *filepath) {
  dyn_string_t *back;
  while ((back = deque_pop_back(filepath->parts)) != NULL) {
    dyn_string_free(back);
  }
  deque_deinit(filepath->parts);
  dyn_string_free(&filepath->literal_path);
}

bool file_path_parse(file_path_t *filepath, const char *path_literal) {
  usz len = str_len(path_literal);

  if (len > PATH_MAX) {
    return false;
  }

  dyn_string_copy_str(&filepath->literal_path, path_literal);

  bool part_init = false;
  dyn_string_t part = {0};

  for (usz ichar = 0; ichar < len; ichar++) {
    while (path_literal[ichar] == '/') {
      if (part_init) {
        deque_push_back(filepath->parts, part);
        part_init = false;
      }
      continue;
    }

    if (!part_init) {
      dyn_string_init(&part, filepath->alloc);
    }

    dyn_string_add_char(&part, path_literal[ichar]);
  }

  return true;
}

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 2, 3)))
#endif
void file_path_extend_back(file_path_t *filepath, const char *fmt, ...);

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 2, 3)))
#endif
void file_path_extend_front(file_path_t *filepath, const char *fmt, ...);

void file_path_push_back(file_path_t *filepath, const char *part);

void file_path_push_front(file_path_t *filepath, const char *part);

void file_path_pop_back(file_path_t *filepath, const char *part);

void file_path_pop_front(file_path_t *filepath, const char *part);

dyn_string_t file_path_format(const file_path_t *filepath, allocator_t *alloc) {
  
}

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 2, 3)))
#endif
file_path_t file_path_makef(allocator_t *alloc, const char *fmt, ...);