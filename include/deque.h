#pragma once

#include "alloc.h"
#include <stddef.h>

#define DEQUE_DEFAULT_CAPACITY 32

#define deque_init(_deque, _allocator)                                         \
  deque_init_cap(_deque, DEQUE_DEFAULT_CAPACITY, _allocator)

#define deque_init_cap(_deque, _capacity, _allocator)                          \
  do {                                                                         \
    extern void _internal_deque_init(void **, size_t, size_t, Allocator *);    \
    _internal_deque_init((void **)&_deque, sizeof(typeof(*_deque)), _capacity, \
                         _allocator);                                          \
  } while (0)

#define deque_push_front(_deque, ...)                                        \
  do {                                                                         \
    extern void _internal_deque_push_front(void **, void *);                   \
    typeof(__VA_ARGS__) item = __VA_ARGS__;                                                \
    _internal_deque_push_front((void **) &_deque, &item);                                \
  } while (0)

#define deque_push_back(_deque, ...)                                         \
  do {                                                                         \
    extern void _internal_deque_push_back(void **, void *);                    \
    typeof(__VA_ARGS__) item = __VA_ARGS__;                                                \
    _internal_deque_push_back((void **) &_deque, &item);                                 \
  } while (0)

#define deque_pop_front(_deque) ((typeof(_deque)) _internal_deque_pop_front(_deque))

#define deque_pop_back(_deque) ((typeof(_deque)) _internal_deque_pop_back(_deque))

#define deque_at(_deque, index) ((typeof(_deque)) _internal_deque_at(_deque, index))

void *deque_front(void *deque);

void *deque_back(void *deque);

size_t deque_len(void *deque);

void deque_deinit(void *deque);

/* Internal functions, required decl for macros returning an expression to compile */

void *_internal_deque_pop_front(void *deque);

void *_internal_deque_pop_back(void *deque);

void *_internal_deque_at(void *deque, size_t index);
