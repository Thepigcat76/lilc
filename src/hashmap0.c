#include "../include/hashmap0.h"
#include "../include/alloc.h"
#include "../include/array.h"
#include "lilc/log.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  /* Additional information about the map */
  struct _internal_hashmap_header header;
  /* A list of all keys in the map*/
  void *keys;
  /* A list of nodes with all values and their keys for the corresponding hash
   */
  struct _internal_hashmap_node *values;
};

static void *
_internal_hashmap_key_at_index(const struct _internal_hashmap *hashmap,
                               size_t index) {
  return ((void *)((uint8_t *)hashmap->keys) +
          index * hashmap->header.key_type_size);
}

static void
_internal_hashmap_node_init(struct _internal_hashmap_node *node,
                            const struct _internal_hashmap_header *h) {
  node->initialized = true;
  node->keys = _internal_array_new(16, h->key_type_size, h->allocator);
  node->values = _internal_array_new(16, h->value_type_size, h->allocator);
}

void _internal_hashmap_init(Hashmap *hashmap, Allocator *alloc,
                            size_t initial_cap, size_t key_type_size,
                            size_t val_type_size, HashFunc key_hash_func,
                            EqFunc key_eq_func, EqFunc val_eq_func) {
  struct _internal_hashmap_header header = {
      .allocator = alloc,
      .capacity = initial_cap,
      .key_hash_func = key_hash_func,
      .key_eq_func = key_eq_func,
      .val_eq_func = val_eq_func,
      .key_type_size = key_type_size,
      .value_type_size = val_type_size,
  };
  struct _internal_hashmap _internal_hashmap = {
      .header = header,
      .keys = _internal_array_new(header.capacity, header.key_type_size, alloc),
      .values = alloc->alloc(alloc, header.capacity *
                                        sizeof(struct _internal_hashmap_node)),
  };
  memset(_internal_hashmap.values, 0,
         header.capacity * sizeof(struct _internal_hashmap_node));

  hashmap->len = 0;
  hashmap->_internal_map =
      alloc->alloc(alloc, sizeof(struct _internal_hashmap));
  *hashmap->_internal_map = _internal_hashmap;
}

static ssize_t
_internal_hashmap_node_contains_key(const struct _internal_hashmap_node *node,
                                    const void *key,
                                    const struct _internal_hashmap_header *h) {
  if (node->keys == NULL)
    return -1;

  for (size_t i = 0; i < array_len(node->keys); i++) {
    void *key_at_index = (void *)((uint8_t *)node->keys) + i * h->key_type_size;
    if (h->key_eq_func(key, key_at_index)) {
      return i;
    }
  }
  return -1;
}

bool hashmap_insert(Hashmap *hashmap, void *key, void *val) {
  if (key == NULL)
    return false;

  int hash = hashmap->_internal_map->header.key_hash_func(key);
  size_t index = hash % hashmap->_internal_map->header.capacity;

  struct _internal_hashmap_node *node = &hashmap->_internal_map->values[index];

  // Initialize the array of the node at index if neccessary
  if (!hashmap->_internal_map->values[index].initialized) {
    _internal_hashmap_node_init(node, &hashmap->_internal_map->header);
  }

  // Check if key is present
  size_t key_index = _internal_hashmap_node_contains_key(
      node, key, &hashmap->_internal_map->header);
  if (key_index != -1) {
    // Key is present
    // Add the value
    memcpy(((uint8_t *)node->values) +
               key_index * hashmap->_internal_map->header.value_type_size,
           val, hashmap->_internal_map->header.value_type_size);
    return true;
  } else {
    // Key is not present
    struct _internal_hashmap_node *node =
        &hashmap->_internal_map->values[index];
    _internal_array_add(&node->keys, key);
    _internal_array_add(&node->values, val);
    _internal_array_add(&hashmap->_internal_map->keys, key);
    hashmap->len++;
    return false;
  }
}

bool hashmap_contains(Hashmap *hashmap, const void *key) {
  if (key == NULL)
    return false;

  int hash = hashmap->_internal_map->header.key_hash_func(key);
  size_t index = hash % hashmap->_internal_map->header.capacity;
  return _internal_hashmap_node_contains_key(
             &hashmap->_internal_map->values[index], key,
             &hashmap->_internal_map->header) != -1;
}

void *hashmap_value(Hashmap *hashmap, const void *key) {
  if (key == NULL)
    return NULL;

  int hash = hashmap->_internal_map->header.key_hash_func(key);
  size_t index = hash % hashmap->_internal_map->header.capacity;

  struct _internal_hashmap_node *node = &hashmap->_internal_map->values[index];

  ssize_t key_index = _internal_hashmap_node_contains_key(
      node, key, &hashmap->_internal_map->header);
  if (key_index == -1)
    return NULL;

  void *val = ((void *)((uint8_t *)node->values) +
               key_index * hashmap->_internal_map->header.value_type_size);

  return val;
}

void hashmap_clear(Hashmap *hashmap) {

  hashmap->len = 0;
  array_clear(hashmap->_internal_map->keys);
}

void hashmap_deinit(Hashmap *hashmap) {
  size_t hashmap_keys_len = array_len(hashmap->_internal_map->keys);
  for (size_t i = 0; i < hashmap_keys_len; i++) {
    void *key = ((uint8_t *)hashmap->_internal_map->keys) +
                i * hashmap->_internal_map->header.key_type_size;
    int hash = hashmap->_internal_map->header.key_hash_func(key);
    size_t hashed_index = hash % hashmap->_internal_map->header.capacity;
    struct _internal_hashmap_node *value_node =
        &hashmap->_internal_map->values[hashed_index];

    if (value_node->initialized) {
      array_free(value_node->values);
      array_free(value_node->keys);
    }
  }
  Allocator *alloc = hashmap->_internal_map->header.allocator;
  alloc->dealloc(alloc, hashmap->_internal_map->values);
  array_free(hashmap->_internal_map->keys);
  alloc->dealloc(alloc, hashmap->_internal_map);
}

static struct _internal_hashmap_node *
hashmap_iter_init_key(Hashmap *hashmap, void **key, void **val,
                      size_t *_hashmap_foreach_key_idx) {
  *key =
      ((uint8_t *)(hashmap)->_internal_map->keys) +
      *_hashmap_foreach_key_idx * hashmap->_internal_map->header.key_type_size;
  int key_hashcode = hashmap->_internal_map->header.key_hash_func(*key);
  int key_hashed_idx = key_hashcode % hashmap->_internal_map->header.capacity;

  struct _internal_hashmap_node *node =
      &hashmap->_internal_map->values[key_hashed_idx];
  ssize_t key_iter_node_idx = _internal_hashmap_node_contains_key(
      node, *key, &hashmap->_internal_map->header);
  if (key_iter_node_idx != -1) {
    *val = (void *)(((uint8_t *)node->values) +
                    key_iter_node_idx *
                        hashmap->_internal_map->header.value_type_size);
    return node;
  }
  return NULL;
}

struct _internal_hashmap_node *
_internal_hashmap_iter_next_key(Hashmap *hashmap, void **key, void **val,
                                size_t *_hashmap_foreach_key_idx) {
  ++(*_hashmap_foreach_key_idx);
  if (*_hashmap_foreach_key_idx >= array_len(hashmap->_internal_map->keys) ||
      *_hashmap_foreach_key_idx >= hashmap->len)
    return NULL;
  struct _internal_hashmap_node *n =
      hashmap_iter_init_key(hashmap, key, val, _hashmap_foreach_key_idx);
  return n;
}

size_t _internal_hashmap_iter_init_key0(Hashmap *hashmap, void **key,
                                        void **val,
                                        struct _internal_hashmap_node **node) {
  size_t idx = 0;
  if (_internal_hashmap_keys_len(hashmap) > 0) {
    *node = hashmap_iter_init_key(hashmap, key, val, &idx);
  }
  return idx;
}

size_t _internal_hashmap_keys_len(const Hashmap *hashmap) {
  return array_len(hashmap->_internal_map->keys);
}
