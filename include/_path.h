#pragma once

#include "alloc.h"
#include "deque.h"
#include "dynstr.h"

typedef struct {
  deque_t(char *) parts;
  char *literal_path;
  bool absolute_path;
  
  allocator_t *alloc;
} file_path_t;

void file_path_init(file_path_t *filepath, allocator_t *alloc);

void file_path_deinit(file_path_t *filepath);

bool file_path_parse(file_path_t *filepath, const char *path_literal);

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 2, 3)))
#endif
file_path_t file_path_makef(allocator_t *alloc, const char *fmt, ...);

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

char *file_path_pop_back(file_path_t *filepath);

char *file_path_pop_front(file_path_t *filepath);

void file_path_reset(file_path_t *filepath);

// Compares file path parts and whether path is abstract
bool file_path_eq(const file_path_t *path0, const file_path_t *path1);

// Compares file path parts, literal paths and whether path is abstract
bool file_path_eq_strict(const file_path_t *path0, const file_path_t *path1);

void file_path_copy(file_path_t *dest, file_path_t *src);

dyn_string_t file_path_format(const file_path_t *filepath, allocator_t *alloc);
