#pragma once

#include "alloc.h"
#include <string.h>

#define array_foreach(arr, el)                                                 \
  extern size_t _internal_array_init(void *, const void *);                    \
  extern void _internal_array_advance(size_t *, void *, const void *);        \
  for (size_t _arr_foreach_idx = _internal_array_init(&el, arr);               \
       _arr_foreach_idx < array_len(arr);                                      \
       _internal_array_advance(&_arr_foreach_idx, &el, arr))

#define array_new(type, allocator)                                             \
  (type *)_internal_array_new(16, sizeof(type), allocator)

#define array_new_capacity(type, capacity, allocator)                          \
  (type *)_internal_array_new(capacity, sizeof(type), allocator)

#define array_fill(arr, len, ...)                                              \
  do {                                                                         \
    __typeof__(*(arr)) _tmp = (__VA_ARGS__);                                   \
    _internal_array_fill((void **)&(arr), len, &_tmp);                         \
  } while (0)

typedef struct {
  Allocator *allocator;
  size_t capacity;
  size_t len;
  size_t item_size;
} _InternalArrayHeader;

void *_internal_array_new(size_t capacity, size_t item_size,
                          Allocator *allocator);

void _internal_array_set_len(void *arr, size_t len);

void _internal_array_fill(void **arr_ptr, size_t len, void *item);

size_t array_len(const void *arr);

void array_free(void *arr);

void _internal_array_copy(void **dest_arr, void *src_arr);

#define array_copy(dest_arr, src_arr) _internal_array_copy((void **) &dest_arr, src_arr)

// Copy memory with the item_size of the array and the length mem_len into the
// array and adjust both length and capacity.
// !! Caller needs to ensure that allocators match !!
#define array_copy_mem(arr, mem, mem_len) _internal_array_copy_mem((void **) &arr, mem, mem_len)

void _internal_array_copy_mem(void **arr_ptr, void *mem, size_t mem_len);

void _internal_array_add(void **arr_ptr, void *item);

void _internal_array_set(void **arr_ptr, void *item, size_t index);

void _internal_array_remove(void *arr_ptr, size_t index);

void _internal_array_clear(void *arr_ptr);

#define array_add(arr, ...)                                      \
  do {                                                            \
    __auto_type _tmp_item = (__VA_ARGS__);                               \
    _internal_array_add((void**)&(arr), (void*)&_tmp_item);       \
  } while (0)

#define array_set(arr, idx, ...)                                               \
  do {                                                                         \
    __typeof__(*(arr)) _tmp = (__VA_ARGS__);                                   \
    _internal_array_set((void **)&(arr), &_tmp, idx);                          \
  } while (0)

#define array_remove(arr, ...) _internal_array_remove(arr, __VA_ARGS__);

#define array_clear(arr) _internal_array_clear(arr);
