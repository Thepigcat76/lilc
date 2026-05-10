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
} Bump;

void bump_init(Bump *bump, size_t bump_node_capacity);

void *bump_alloc(Bump *bump, size_t bytes);

void bump_free(Bump *bump);

void bump_reset(Bump *bump);