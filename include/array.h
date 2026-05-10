#pragma once

#include "alloc.h"
#include <string.h>

#define array_foreach(arr, el)                                                 \
  extern size_t _internal_array_init(void *el, void *_arr);                   \
  extern void _internal_array_advance(size_t *i, void *el, void *_arr);       \
  for (size_t _arr_foreach_idx = _internal_array_init(&el, arr);               \
       _arr_foreach_idx < array_len(arr); _internal_array_advance(&_arr_foreach_idx, &el, arr))

#define array_new(type, allocator)                                             \
  (type *)_internal_array_new(16, sizeof(type), allocator)

#define array_new_capacity(type, capacity, allocator)                          \
  (type *)_internal_array_new(capacity, sizeof(type), allocator)

#define array_free(arr) _internal_array_free(arr)

typedef struct {
  const Allocator *allocator;
  size_t capacity;
  size_t len;
  size_t item_size;
} _InternalArrayHeader;

void *_internal_array_new(size_t capacity, size_t item_size,
                          const Allocator *allocator);

void _internal_array_set_len(void *arr, size_t len);

size_t array_len(const void *arr);

void _internal_array_free(void *arr);

void _internal_array_add(void **arr_ptr, void *item, size_t item_size);

void _internal_array_remove(void *arr_ptr, size_t index);

void _internal_array_clear(void *arr_ptr);

#define array_add(arr, ...)                                                    \
  do {                                                                         \
    __typeof__(*(arr)) _tmp = (__VA_ARGS__);                                   \
    _internal_array_add((void **)&(arr), &_tmp, sizeof(_tmp));                 \
  } while (0)

#define array_remove(arr, ...) _internal_array_remove(arr, __VA_ARGS__);

#define array_clear(arr) _internal_array_clear(arr);
