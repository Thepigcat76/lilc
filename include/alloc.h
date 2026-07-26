#pragma once

#include "bump.h"
#include <stdlib.h>

typedef struct allocator {
  void *(*alloc)(struct allocator *, size_t bytes);
  void (*dealloc)(struct allocator *, void *ptr);
  void *(*realloc)(struct allocator *, void *prev_ptr, size_t old_size, size_t new_size);
  void *context;
} Allocator;

extern Allocator HEAP_ALLOCATOR;
extern Allocator NULL_ALLOCATOR;

void *heap_alloc(size_t bytes);
void heap_dealloc(void *ptr);

void bump_allocator_init(Allocator *allocator, Bump *bump);

void alloc_init();