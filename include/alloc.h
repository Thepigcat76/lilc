#pragma once

#include "bump.h"
#include <stdlib.h>

typedef struct allocator {
  void *(*alloc)(const struct allocator *, size_t bytes);
  void (*dealloc)(const struct allocator *, void *ptr);
  void *context;
} Allocator;

extern Allocator HEAP_ALLOCATOR;
extern Allocator NULL_ALLOCATOR;

void *allocator_realloc(const Allocator *allocator, void *ptr, size_t old_size, size_t bytes);

void *heap_alloc(size_t bytes);
void heap_dealloc(void *ptr);

void bump_allocator_init(Allocator *allocator, Bump *bump);

void alloc_init();