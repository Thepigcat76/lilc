#include "../include/array.h"
#include "../include/log.h"
#include "../include/numbers.h"
#include "../include/panic.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void *_internal_array_new(size_t capacity, size_t item_size,
                          Allocator *allocator) {
  void *ptr = NULL;
  size_t size = item_size * capacity + sizeof(_InternalArrayHeader);
  _InternalArrayHeader *h = allocator->alloc(allocator, size);

  if (h) {
    h->capacity = capacity;
    h->len = 0;
    h->allocator = allocator;
    h->item_size = item_size;
    ptr = h + 1;
  }

  return ptr;
}

/* Iteration */
size_t _internal_array_init(void *el, const void *arr) {
  *((void **)el) = (u8 *) arr;
  return 0;
}

void _internal_array_advance(size_t *i, void *el, const void *arr) {
  (*i)++;
  _InternalArrayHeader *h = ((_InternalArrayHeader *)arr) - 1;
  u8 *bytes = (u8 *)arr;
  *((void **)el) = bytes + *i * h->item_size;
}

/* Size doubling and memory reallocation */
static bool _internal_array_set_capacity(void **arr_ptr, size_t new_capacity) {
  _InternalArrayHeader *h = ((_InternalArrayHeader *)*arr_ptr) - 1;
  size_t arr_len = h->len;
  size_t arr_cap = h->capacity;
  size_t arr_item_size = h->item_size;
  Allocator *arr_allocator = h->allocator;

  size_t size = arr_item_size * new_capacity + sizeof(_InternalArrayHeader);
  _InternalArrayHeader *new_h = arr_allocator->realloc(
      arr_allocator, h, sizeof(_InternalArrayHeader) + arr_cap * arr_item_size,
      size);

  if (new_h == NULL) return false;

  new_h->capacity = new_capacity;

  *arr_ptr = new_h + 1;

  return true;
}

inline void _internal_array_add(void **arr_ptr, void *item) {
  if (!arr_ptr || !*arr_ptr || !item) panic("array_add invalid args");

  _InternalArrayHeader *h = ((_InternalArrayHeader *)*arr_ptr) - 1;
  if (h->len >= h->capacity) {
    size_t new_cap = h->capacity ? h->capacity * 2 : 1;
    if (!_internal_array_set_capacity(arr_ptr, new_cap)) panic("resize failed");
    h = ((_InternalArrayHeader *)*arr_ptr) - 1;
  }

  memcpy((char *)*arr_ptr + h->len * h->item_size, item, h->item_size);
  h->len++;
}


void _internal_array_ensure_capacity(void **arr_ptr, size_t capacity) {
  void *arr = *arr_ptr;

  _InternalArrayHeader *h = ((_InternalArrayHeader *)arr) - 1;

  if (h->capacity < capacity) {
    _internal_array_set_capacity(arr_ptr, capacity);
  }
}

void _internal_array_fill(void **arr_ptr, size_t len, void *item) {

  _internal_array_ensure_capacity(arr_ptr, len);

  void *arr = *arr_ptr;
  _InternalArrayHeader *h = ((_InternalArrayHeader *)arr) - 1;

  if (h->len < len) {
    _internal_array_set_len(arr, len);
  }

  for (size_t i = 0; i < len; i++) {
    _internal_array_set(arr_ptr, item, i);
  }
}

void _internal_array_set(void **arr_ptr, void *item, size_t index) {
  void *arr = *arr_ptr;
  _InternalArrayHeader *h = ((_InternalArrayHeader *)arr) - 1;

#ifdef DEBUG_BUILD
  if (index >= h->len) {
    PANIC_FMT("Index %zu out of bounds for array of length %zu", index, h->len);
  }
#endif

  memcpy((char *)arr + index * h->item_size, item, h->item_size);
}

void _internal_array_clear(void *arr) { _internal_array_set_len(arr, 0); }

void _internal_array_remove(void *arr_ptr, size_t index) {
  if (!arr_ptr)
    return;

  _InternalArrayHeader *h = ((_InternalArrayHeader *)arr_ptr) - 1;

  if (index >= h->len) {
#ifdef SURTUR_DEBUG
#include <stdio.h>
    printf("Index %zu out of bounds for array of length %zu\n", index, h->len);
#endif
    return;
  }

  char *arr = (char *)arr_ptr;
  size_t item_size = h->item_size;

  char *dest = arr + index * item_size;
  char *src = arr + (index + 1) * item_size;
  size_t move_count = h->len - index - 1;

  if (move_count > 0) {
    memmove(dest, src, move_count * item_size);
  }

  h->len--;
}

void _internal_array_copy(void **dest_arr, void *src_arr) {
  if (dest_arr == NULL || *dest_arr == NULL || src_arr == NULL) {
    panic("[LILC] Failed to copy array contents, parameter is null");
  }

  _InternalArrayHeader *src_h = ((_InternalArrayHeader *)src_arr) - 1;
  _InternalArrayHeader *dest_h = ((_InternalArrayHeader *)*dest_arr) - 1;

  if (src_h->item_size != dest_h->item_size) {
    panic("[LILC] Failed to copy array contents, destination and source arrays "
          "have different sizes");
  }

  if (src_h->len > dest_h->capacity) {
    _InternalArrayHeader *new_dest_h = dest_h->allocator->alloc(
        dest_h->allocator,
        sizeof(_InternalArrayHeader) + src_h->capacity * dest_h->item_size);
    memcpy(new_dest_h, dest_h, sizeof(_InternalArrayHeader));
    dest_h->allocator->dealloc(dest_h->allocator, dest_h);

    dest_h = new_dest_h;
  }
  dest_h->capacity = src_h->capacity;
  dest_h->len = src_h->len;
  *dest_arr = dest_h + 1;

  memcpy(*dest_arr, src_arr, dest_h->len * dest_h->item_size);
}

void _internal_array_set_len(void *arr, size_t len) {
  _InternalArrayHeader *h = ((_InternalArrayHeader *)arr) - 1;
  h->len = len;
}

size_t array_len(const void *arr) {
  return (((_InternalArrayHeader *)arr) - 1)->len;
}

void _internal_array_copy_mem(void **arr_ptr, void *mem, size_t mem_len) {
  if (arr_ptr == NULL || *arr_ptr == NULL)
    return;

  _InternalArrayHeader *_h = ((_InternalArrayHeader *)*arr_ptr) - 1;

  _InternalArrayHeader h_copy = *_h;

  // Dealloc old data
  _h->allocator->dealloc(_h->allocator, _h);

  // Align cap by 8
  size_t cap = (mem_len + 7) & ~7;
  _InternalArrayHeader *h_new = h_copy.allocator->alloc(
      h_copy.allocator, sizeof(_InternalArrayHeader) + cap * h_copy.item_size);

  h_new->allocator = h_copy.allocator;
  h_new->item_size = h_copy.item_size;
  h_new->len = mem_len;
  h_new->capacity = cap;

  void *new_arr = h_new + 1;
  if (mem != NULL) {
    memcpy(new_arr, mem, h_new->item_size * mem_len);
  }

  *arr_ptr = new_arr;
}

void array_free(void *arr) {
  _InternalArrayHeader *h = ((_InternalArrayHeader *)arr) - 1;
  h->allocator->dealloc(h->allocator, h);
}