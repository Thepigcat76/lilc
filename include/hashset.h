#pragma once

#include "alloc.h"
#include "numbers.h"
#include "shared.h"
#include <stdbool.h>

struct _internal_hashset;

typedef struct {
  usz len;

  struct _internal_hashset *_internal_set;
} hashset_t;

#define HASHSET_DEFAULT_CAP 256

bool hashset_insert(hashset_t *hashset, void *val);

bool hashset_contains(hashset_t *hashset, void *val);

#define hashset_init_cap(hashset_ptr, alloc_ptr, cap, v_ty, v_hash_func,       \
                         v_eq_func)                                            \
  _internal_hashset_init(hashset_ptr, alloc_ptr, cap, sizeof(v_ty),          \
                           v_hash_func, v_eq_func)

#define hashset_init(hashset_ptr, alloc_ptr, v_ty, v_hash_func, v_eq_func)     \
  hashset_init_cap(hashset_ptr, alloc_ptr, HASHSET_DEFAULT_CAP, v_ty,          \
                   v_hash_func, v_eq_func)

void _internal_hashset_init(hashset_t *hashset, allocator_t *alloc, usz capacity,
                            usz val_ty_size, hash_func_t val_hash_func,
                            eq_func_t val_eq_func);

void hashset_deinit(hashset_t *hashset);

void hashset_clear(hashset_t *hashset);
