#pragma once

#include <stddef.h>

typedef struct {
  size_t node_capacity;
  // The first node of the bump
  struct bump_node *first_node;
  // The current node of the bump that is free
  struct bump_node *cur_node;
  struct bump_node *(*alloc_node)(size_t node_capacity);
  void (*dealloc_node)(struct bump_node *node);
} bump_t;

#define Bump bump_t // Backwards compatability

void bump_init(bump_t *bump, size_t bump_node_capacity);

void *bump_alloc(bump_t *bump, size_t bytes);

void bump_free(bump_t *bump);

void bump_reset(bump_t *bump);