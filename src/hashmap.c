#include "../include/hashmap.h"
#include "../include/alloc.h"
#include "../include/array.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _internal_hashmap
_internal_hashmap_new(struct _internal_hashmap_header h) {
  struct _internal_hashmap map = (struct _internal_hashmap){
      .keys = _internal_array_new(h.capacity, h.key_type_size, h.allocator),
      .values = h.allocator->alloc(h.capacity *
                                   sizeof(struct _internal_hashmap_node)),
      .header = h,
  };
  memset(map.values, 0, h.capacity * sizeof(struct _internal_hashmap_node));
  return map;
}

static void
_internal_hashmap_node_init(struct _internal_hashmap_node *node,
                            const struct _internal_hashmap_header *h) {
  node->initialized = true;
  node->keys = _internal_array_new(16, h->key_type_size, h->allocator);
  node->values = _internal_array_new(16, h->value_type_size, h->allocator);
}

void *_internal_hashmap_key_at_index(const struct _internal_hashmap *hashmap,
                                     size_t index) {
  return ((void *)((uint8_t *)hashmap->keys) +
          index * hashmap->header.key_type_size);
}

/* Cannot be static, needs to be accessible in different file */
ssize_t
_internal_hashmap_node_contains_key(const struct _internal_hashmap_node *node,
                                    const void *key,
                                    const struct _internal_hashmap_header *h) {
  if (node->keys == NULL) return -1;

  for (size_t i = 0; i < array_len(node->keys); i++) {
    void *key_at_index = (void *)((uint8_t *)node->keys) + i * h->key_type_size;
    if (h->key_eq_func(key, key_at_index)) {
      return i;
    }
  }
  return -1;
}

bool hashmap_insert(struct _internal_hashmap *hashmap, void *key, void *val) {
  // size_t len = hashmap->len;
  // size_t prev_capacity = hashmap->header.capacity;
  // size_t new_capacity = prev_capacity * 2;
  // Allocator *allocator = hashmap->header.allocator;
  // size_t new_key_size = hashmap->header.key_type_size * new_capacity;
  // size_t new_value_size = hashmap->header.value_type_size * new_capacity;
  // if (len >= prev_capacity) {
  //   void *temp_keys = allocator->alloc(new_key_size);
  //   void *temp_values = allocator->alloc(new_value_size);
  //
  //  memcpy(temp_keys, hashmap->keys, len * hashmap->header.key_type_size);
  //  memcpy(temp_values, hashmap->values, len *
  //  hashmap->header.value_type_size); allocator->dealloc(hashmap->keys);
  //  allocator->dealloc(hashmap->values);
  //  hashmap->keys = temp_keys;
  //  hashmap->values = temp_values;
  //}
  //
  // memcpy((char *)hashmap->keys + len * hashmap->header.key_type_size, key,
  // hashmap->header.key_type_size); memcpy((char *)hashmap->values + len *
  // hashmap->header.value_type_size, val, hashmap->header.value_type_size);
  // hashmap->len++;
  if (key == NULL)
    return false;

  int hash = hashmap->header.key_hash_func(key);
  size_t index = hash % hashmap->header.capacity;

  struct _internal_hashmap_node *node = &hashmap->values[index];

  // Initialize the array of the node at index if neccessary
  if (!hashmap->values[index].initialized) {
    _internal_hashmap_node_init(node, &hashmap->header);
  }

  // Check if key is present
  size_t key_index =
      _internal_hashmap_node_contains_key(node, key, &hashmap->header);
  if (key_index != -1) {
    // Key is present
    // Add the value
    memcpy(((uint8_t *)node->values) +
               key_index * hashmap->header.value_type_size,
           val, hashmap->header.value_type_size);
    return true;
  } else {
    // Key is not present
    struct _internal_hashmap_node *node = &hashmap->values[index];
    printf("Isnerting Key: %s\n", *(char **) key);
    _internal_array_add(&node->keys, key, hashmap->header.key_type_size);
    printf("Inserted into nodes\n");
    _internal_array_add(&node->values, val, hashmap->header.value_type_size);
    _internal_array_add(&hashmap->keys, key, hashmap->header.key_type_size);
    hashmap->len++;
    return false;
  }
}

bool hashmap_contains(struct _internal_hashmap *hashmap, const void *key) {
  if (key == NULL)
    return false;

  int hash = hashmap->header.key_hash_func(key);
  size_t index = hash % hashmap->header.capacity;
  return _internal_hashmap_node_contains_key(&hashmap->values[index], key,
                                             &hashmap->header) != -1;
}

void *_internal_hashmap_value(struct _internal_hashmap *hashmap,
                              const void *key) {
  if (key == NULL)
    return NULL;

  int hash = hashmap->header.key_hash_func(key);
  size_t index = hash % hashmap->header.capacity;

  struct _internal_hashmap_node *node = &hashmap->values[index];

  ssize_t key_index =
      _internal_hashmap_node_contains_key(node, key, &hashmap->header);
  if (key_index == -1)
    return NULL;

  void *val = ((void *) ((uint8_t *) node->values) + key_index * hashmap->header.value_type_size);

  return val;
}

void hashmap_free(struct _internal_hashmap *hashmap) {
  //free(hashmap->keys);
  //free(hashmap->values);
}