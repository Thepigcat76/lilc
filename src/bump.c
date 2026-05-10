#include "../include/bump.h"
#include "../include/alloc.h"
#include "../include/numbers.h"

struct bump_node {
  u64 offset;
  u64 capacity;
  u8 *buffer;
  struct bump_node *next_node;
};

static struct bump_node *bump_alloc_node(size_t node_capacity) {
  size_t size = node_capacity + sizeof(struct bump_node);
  struct bump_node *bump_node = heap_alloc(size);

  bump_node->offset = 0;
  bump_node->capacity = node_capacity;
  bump_node->buffer = (u8 *)(bump_node + 1);
  bump_node->next_node = NULL;

  return bump_node;
}

static void bump_dealloc_node(struct bump_node *bump_node) { free(bump_node); }

void bump_init(Bump *bump, size_t bump_node_capacity) {
  size_t aligned_capacity = align_up(bump_node_capacity, 8);
  bump->node_capacity = aligned_capacity;
  bump->alloc_node = bump_alloc_node;
  bump->dealloc_node = bump_dealloc_node;
  bump->first_node = bump->alloc_node(aligned_capacity);
  bump->cur_node = bump->first_node;
}

void *bump_alloc(Bump *bump, size_t bytes) {
  if (bump->cur_node == NULL || bytes == 0)
    return NULL;

  // Size we want to allocate is too big for the current node
  if (bump->cur_node->offset + bytes > bump->cur_node->capacity) {
    // Look for a node that has enough capacity to allocate
    // the memory
    struct bump_node *prev_node = NULL;
    struct bump_node *cur_node = bump->cur_node;
    while (cur_node != NULL) {
      if (cur_node->offset + bytes <= cur_node->capacity) {
        bump->cur_node = cur_node;
        break;
      }
      prev_node = cur_node;
      cur_node = cur_node->next_node;
    }

    if (prev_node == NULL)
      return NULL;

    // If we cant find a node with enough size, we allocate a new one
    // after the last one.
    // Its size is usually the default node capacity, if that is not
    // enough, we use the size we want to allocate and align it by 8
    if (cur_node == NULL) {
      struct bump_node *new_node =
          bump->alloc_node(max(bump->node_capacity, align_up(bytes, 8)));

      if (new_node == NULL)
        return NULL;

      prev_node->next_node = new_node;
      bump->cur_node = new_node;
    }
  }

  void *ptr = bump->cur_node->buffer + bump->cur_node->offset;
  bump->cur_node->offset += bytes;
  return ptr;
}

void bump_reset(Bump *bump) {
  struct bump_node *cur_node = bump->first_node;
  while (cur_node != NULL) {
    cur_node->offset = 0;
    cur_node = cur_node->next_node;
  }

  bump->cur_node = bump->first_node;
}

void bump_free(Bump *bump) {
  struct bump_node *cur_node = bump->first_node;
  while (cur_node != NULL) {
    struct bump_node *cur_node_copy = cur_node;
    cur_node = cur_node->next_node;
    bump->dealloc_node(cur_node_copy);
  }
}