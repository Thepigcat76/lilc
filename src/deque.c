#include "../include/deque.h"
#include "../include/numbers.h"
#include <stdio.h>
#include <string.h>

struct deque_header {
  size_t head_index;
  size_t tail_index;

  size_t len;
  size_t capacity;
  size_t item_size;
  Allocator *allocator;
};

void _internal_deque_init(void **deque_ptr, size_t item_size, size_t capacity,
                          Allocator *allocator) {
  struct deque_header *deque = allocator->alloc(
      allocator, item_size * capacity + sizeof(struct deque_header));

  if (deque) {
    deque->head_index = 0;
    deque->tail_index = 0;

    deque->item_size = item_size;
    deque->len = 0;
    deque->capacity = capacity;
    deque->allocator = allocator;

    *deque_ptr = deque + 1;
  }
}

static void _internal_deque_resize(void **deque_ptr, size_t new_capacity) {
  struct deque_header *header = ((struct deque_header *)*deque_ptr) - 1;
  struct deque_header old_header = *header;

  void *old_deque = *deque_ptr;

  header->capacity = new_capacity;
  struct deque_header *new_header = header->allocator->alloc(
      header->allocator,
      sizeof(struct deque_header) + header->capacity * header->item_size);
  memcpy(new_header, header, sizeof(struct deque_header));

  void *new_deque = new_header + 1;

  size_t front_elems_amount =
      (old_header.capacity - old_header.head_index) % old_header.capacity;
  size_t back_elems_amount = old_header.tail_index;

  //printf(
  //    "[Resizing] Front elems amount: %zu, front index: %zu, capacity: %zu\n",
  //    front_elems_amount, header->head_index, header->capacity);
  //printf("[Resizing] Back elems amount: %zu, back index: %zu, capacity: %zu\n",
  //       back_elems_amount, header->tail_index, header->capacity);

  // Copy front elements
  memcpy(new_deque + (new_header->capacity - front_elems_amount) *
                         new_header->item_size,
         old_deque + old_header.head_index * new_header->item_size,
         front_elems_amount * new_header->item_size);

  // Copy back elements
  memcpy(new_deque, old_deque, back_elems_amount * new_header->item_size);

  new_header->head_index = new_header->capacity - front_elems_amount;

  *deque_ptr = new_deque;
}

void _internal_deque_push_front(void **deque_ptr, void *item) {
  struct deque_header prev_h = *(((struct deque_header *)*deque_ptr) - 1);
  if (prev_h.len + 1 >= prev_h.capacity) {
    _internal_deque_resize(
        deque_ptr, (((struct deque_header *)*deque_ptr) - 1)->capacity * 2);
  }

  void *deque = *deque_ptr;
  struct deque_header *h = ((struct deque_header *)deque) - 1;

  size_t head_index = (h->head_index - 1 + h->capacity) % h->capacity;

  u8 *deque_mem = (u8 *)deque;
  memcpy(deque_mem + head_index * h->item_size, item, h->item_size);

  h->head_index = head_index;
  h->len++;
}

void _internal_deque_push_back(void **deque_ptr, void *item) {
  struct deque_header prev_h = *(((struct deque_header *)*deque_ptr) - 1);
  if (prev_h.len + 1 >= prev_h.capacity) {
    _internal_deque_resize(
        deque_ptr, (((struct deque_header *)*deque_ptr) - 1)->capacity * 2);
  }

  void *deque = *deque_ptr;
  struct deque_header *h = ((struct deque_header *)deque) - 1;

  size_t tail_index = h->tail_index;

  u8 *deque_mem = (u8 *)deque;
  memcpy(deque_mem + tail_index * h->item_size, item, h->item_size);

  h->tail_index = (tail_index + 1) % h->capacity;
  h->len++;
}

void *_internal_deque_at(void *deque, size_t index) {
  struct deque_header *h = ((struct deque_header *)deque) - 1;

  size_t head_index = (h->head_index + index) % h->capacity;

  if (index < h->len) {
    return ((u8 *)deque) + head_index * h->item_size;
  }
  return NULL;
}

void *deque_front(void *deque) {
  struct deque_header *h = ((struct deque_header *)deque) - 1;
  if (h->len == 0) {
    return NULL;
  }
  
  return ((u8 *)deque) + h->head_index * h->item_size;
}

void *deque_back(void *deque) {
  struct deque_header *h = ((struct deque_header *)deque) - 1;
  if (h->len == 0) {
    return NULL;
  }

  return ((u8 *)deque) + ((h->tail_index - 1) % h->capacity) * h->item_size;
}

size_t deque_len(void *deque) {
  struct deque_header *h = ((struct deque_header *)deque) - 1;
  return h->len;
}

void deque_deinit(void *deque) {
  struct deque_header *h = ((struct deque_header *)deque) - 1;
  h->allocator->dealloc(h->allocator, h);
}

void *_internal_deque_pop_back(void *deque) {
  struct deque_header *h = ((struct deque_header *)deque) - 1;

  --h->len;
  return ((u8 *)deque) + (((--h->tail_index) % h->capacity) * h->item_size);
}

void *_internal_deque_pop_front(void *deque) {
  struct deque_header *h = ((struct deque_header *)deque) - 1;

  --h->len;
  return ((u8 *)deque) + (((h->head_index++) % h->capacity) * h->item_size);
}
