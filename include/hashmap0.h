#pragma once

#include "alloc.h"
#include "array.h"
#include <stdbool.h>

typedef bool (*EqFunc)(const void *a, const void *b);

typedef int (*HashFunc)(const void *a);

struct _internal_hashmap;

typedef struct {
  /* The amount of valid entries in the hashmap */
  size_t len;

  struct _internal_hashmap *_internal_map;
} Hashmap;

#define HASHMAP_DEFAULT_CAP 128

#define hashmap_init_cap(hashmap_ptr, alloc_ptr, cap, k_ty, v_ty, k_hash_func, \
                         k_eq_func, v_eq_func)                                 \
  _internal_hashmap_init(hashmap_ptr, alloc_ptr, cap, sizeof(k_ty),            \
                         sizeof(v_ty), k_hash_func, k_eq_func, v_eq_func)

#define hashmap_init(hashmap_ptr, alloc_ptr, k_ty, v_ty, k_hash_func,          \
                     k_eq_func, v_eq_func)                                     \
  hashmap_init_cap(hashmap_ptr, alloc_ptr, HASHMAP_DEFAULT_CAP, k_ty, v_ty,    \
                   k_hash_func, k_eq_func, v_eq_func)

void _internal_hashmap_init(Hashmap *hashmap, Allocator *alloc,
                            size_t initial_cap, size_t key_type_size,
                            size_t val_type_size, HashFunc key_hash_func,
                            EqFunc key_eq_func, EqFunc val_eq_func);

void *hashmap_value(Hashmap *hashmap, const void *key);

void *hashmap_key(Hashmap *hashmap, const void *val);

/*
 * Inserts 'val' for 'key'. Returns NULL if no value was present before,
 * otherwise returns previous value
 */
bool hashmap_insert(Hashmap *hashmap, void *key, void *val);

bool hashmap_contains(Hashmap *hashmap, const void *key);

void hashmap_clear(Hashmap *hashmap);

void hashmap_deinit(Hashmap *hashmap);

struct _internal_hashmap_node *
_internal_hashmap_iter_next_key(Hashmap *map, void **_key, void **val,
                                size_t *_hashmap_foreach_key_idx);
size_t _internal_hashmap_iter_init_key0(Hashmap *map, void **_key, void **val,
                                        struct _internal_hashmap_node **node);
size_t _internal_hashmap_keys_len(const Hashmap *map);

#define hashmap_foreach(hashmap, key, value)                                   \
  for (u64 _iter_cur_node = 0, _hashmap_foreach_key_idx = _internal_hashmap_iter_init_key0(               \
           (hashmap), (void **)&key, (void **)&value, (struct _internal_hashmap_node **) &_iter_cur_node);        \
       _hashmap_foreach_key_idx < _internal_hashmap_keys_len(hashmap);                   \
       _iter_cur_node =                                                        \
           (u64) _internal_hashmap_iter_next_key((hashmap), (void **)&key, (void **)&value,    \
                                 &_hashmap_foreach_key_idx))                   \
    if ((struct _internal_hashmap_node *) _iter_cur_node != NULL)
