#pragma once

#include "alloc.h"
#include "array.h"
#include <stdbool.h>

#define Hashmap(key_ty, val_ty) struct _internal_hashmap

typedef bool (*EqFunc)(const void *a, const void *b);

typedef int (*HashFunc)(const void *a);

struct _internal_hashmap_header {
  Allocator *allocator;
  size_t capacity;
  size_t key_type_size;
  size_t value_type_size;
  HashFunc key_hash_func;
  EqFunc key_eq_func;
  EqFunc val_eq_func;
};

struct _internal_hashmap_node {
  /* A list of keys corresponding to the index of the values */
  void *keys;
  /* A list of values corresponding to the index of the keys */
  void *values;
  /* Whether the node has been initialized */
  bool initialized;
};

struct _internal_hashmap {
  /* A list of all keys in the map*/
  void *keys;
  /* A list of nodes with all values and their keys for the corresponding hash
   */
  struct _internal_hashmap_node *values;
  /* The amount of valid entries in the hashmap */
  size_t len;
  /* Additional information about the map */
  struct _internal_hashmap_header header;
};

struct _internal_hashmap
_internal_hashmap_new(struct _internal_hashmap_header h);

void *_internal_hashmap_value(struct _internal_hashmap *hashmap,
                              const void *key);

void *_internal_hashmap_key(struct _internal_hashmap *hashmap, const void *val);

#define MAP_DEFAULT_CAPACITY 2000

#define hashmap_new(key_ty, val_ty, _allocator, _key_hash_func, _key_eq_func,  \
                    _val_eq_func)                                              \
  _internal_hashmap_new(                                                       \
      (struct _internal_hashmap_header){.allocator = _allocator,               \
                                        .capacity = MAP_DEFAULT_CAPACITY,      \
                                        .key_type_size = sizeof(key_ty),       \
                                        .value_type_size = sizeof(val_ty),     \
                                        .key_hash_func = _key_hash_func,       \
                                        .key_eq_func = _key_eq_func,           \
                                        .val_eq_func = _val_eq_func})

#define hashmap_new_capacity(key_ty, val_ty, _allocator, _key_hash_func,       \
                             _key_eq_func, _val_eq_func, _capacity)            \
  _internal_hashmap_new(                                                       \
      (struct _internal_hashmap_header){.allocator = _allocator,               \
                                        .capacity = _capacity,                 \
                                        .key_type_size = sizeof(key_ty),       \
                                        .value_type_size = sizeof(val_ty),     \
                                        .key_hash_func = _key_hash_func,       \
                                        .key_eq_func = _key_eq_func,           \
                                        .val_eq_func = _val_eq_func})

#define hashmap_value(map, key, ...)                                           \
  __VA_OPT__(*(__VA_ARGS__ *)) _internal_hashmap_value(map, key)

#define hashmap_key(map, val, ...)                                             \
  __VA_OPT__(*(__VA_ARGS__ *)) _internal_hashmap_key(map, key)

/*
 * Attempts to insert `val` for `key`. Will return true if no key was present,
 * will return false if the previous value was overriden
 */
bool hashmap_insert(struct _internal_hashmap *hashmap, void *key, void *val);

bool hashmap_contains(struct _internal_hashmap *hashmap, const void *key);

void hashmap_free(struct _internal_hashmap *hashmap);

#define hashmap_foreach(map_ptr, key, val, ...)                                \
  do {                                                                         \
    extern ssize_t _internal_hashmap_node_contains_key(                        \
        const struct _internal_hashmap_node *node, const void *_key,           \
        const struct _internal_hashmap_header *h);                             \
                                                                               \
    size_t _internal_keys_len = array_len((map_ptr)->keys);                    \
    for (size_t _internal_keys_iter_index = 0;                                 \
         _internal_keys_iter_index < _internal_keys_len;                       \
         _internal_keys_iter_index++) {                                        \
                                                                               \
      void *_internal_keys_iter_key =                                          \
          ((uint8_t *)(map_ptr)->keys) +                                       \
          _internal_keys_iter_index * (map_ptr)->header.key_type_size;         \
      int _internal_keys_iter_hash =                                           \
          (map_ptr)->header.key_hash_func(_internal_keys_iter_key);            \
      size_t _internal_keys_iter_hashed_index =                                \
          _internal_keys_iter_hash % (map_ptr)->header.capacity;               \
                                                                               \
      struct _internal_hashmap_node *node =                                    \
          &(map_ptr)->values[_internal_keys_iter_hashed_index];                \
      ssize_t _internal_keys_iter_node_key_index =                             \
          _internal_hashmap_node_contains_key(node, _internal_keys_iter_key,   \
                                              &(map_ptr)->header);             \
      if (_internal_keys_iter_node_key_index != -1) {                          \
        void *_internal_keys_iter_value =                                      \
            (void *)(((uint8_t *)node->values) +                               \
                     _internal_keys_iter_node_key_index *                      \
                         (map_ptr)->header.value_type_size);                   \
        key = _internal_keys_iter_key;                                         \
        val = _internal_keys_iter_value;                                       \
        __VA_ARGS__                                                            \
      }                                                                        \
    }                                                                          \
  } while (0)