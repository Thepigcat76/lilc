#include "../include/hashset.h"
#include "../include/alloc.h"
#include "../include/array.h"

struct _internal_hashset_node {
  /* A list of values */
  void *values;
  /* Whether the node has been initialized */
  bool initialized;
};

struct _internal_hashset {
  allocator_t *allocator;
  usz capacity;
  usz val_type_size;
  hash_func_t val_hash_func;
  eq_func_t val_eq_func;

  struct _internal_hashset_node *values;
};

static inline void
_internal_hashmap_node_init(struct _internal_hashset_node *node,
                            const struct _internal_hashset *h) {
  node->initialized = true;
  node->values = _internal_array_new(16, h->val_type_size, h->allocator);
}

void _internal_hashset_init(hashset_t *hashset, allocator_t *alloc, usz capacity,
                            usz val_ty_size, hash_func_t val_hash_func,
                            eq_func_t val_eq_func) {
  struct _internal_hashset internal = {
      .allocator = alloc,
      .capacity = capacity,
      .val_hash_func = val_hash_func,
      .val_eq_func = val_eq_func,
      .val_type_size = val_ty_size,
      .values =
          alloc->alloc(alloc, capacity * sizeof(struct _internal_hashset_node)),
  };
  memset(internal.values, 0, capacity * sizeof(struct _internal_hashset_node));

  hashset->len = 0;
  hashset->_internal_set =
      alloc->alloc(alloc, sizeof(struct _internal_hashset));
  *hashset->_internal_set = internal;
}

void hashset_deinit(hashset_t *hashset) {
  for (size_t i = 0; i < hashset->_internal_set->capacity; i++) {
    struct _internal_hashset_node *value_node =
        &hashset->_internal_set->values[i];

    if (value_node->initialized) {
      array_free(value_node->values);
    }
  }
  allocator_t *alloc = hashset->_internal_set->allocator;
  alloc->dealloc(alloc, hashset->_internal_set->values);
  alloc->dealloc(alloc, hashset->_internal_set);
}

void hashset_clear(hashset_t *hashset) {
  hashset->len = 0;
}

static ssize_t
_internal_hashset_node_contains_val(const struct _internal_hashset_node *node,
                                    const void *val,
                                    const struct _internal_hashset *h) {
  if (node->values == NULL)
    return -1;

  for (size_t i = 0; i < array_len(node->values); i++) {
    void *val_at_index = (void *)((u8 *)node->values) + i * h->val_type_size;
    if (h->val_eq_func(val, val_at_index)) {
      return i;
    }
  }
  return -1;
}

bool hashset_insert(hashset_t *hashset, void *val) {
  if (val == NULL)
    return false;

  int hash = hashset->_internal_set->val_hash_func(val);
  size_t index = hash % hashset->_internal_set->capacity;

  struct _internal_hashset_node *node = &hashset->_internal_set->values[index];

  // Initialize the array of the node at index if neccessary
  if (!hashset->_internal_set->values[index].initialized) {
    _internal_hashmap_node_init(node, hashset->_internal_set);
  }

  // Check if val is present
  size_t val_index =
      _internal_hashset_node_contains_val(node, val, hashset->_internal_set);
  if (val_index == -1) {
    // val is not present
    _internal_array_add(&node->values, val);
    hashset->len++;
    return true;
  }

  return false;
}

bool hashset_contains(hashset_t *hashset, void *val) {
  if (val == NULL)
    return false;

  int hash = hashset->_internal_set->val_hash_func(val);
  usz index = hash % hashset->_internal_set->capacity;
  return _internal_hashset_node_contains_val(
             &hashset->_internal_set->values[index], val,
             hashset->_internal_set) != -1;
}