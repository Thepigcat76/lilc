
#include "../include/alloc.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void *heap_allocator_alloc(const Allocator *allocator, size_t bytes) {
  return malloc(bytes);
}

static void heap_allocator_dealloc(const Allocator *allocator, void *ptr) {
  free(ptr);
}

static void *null_allocator_alloc(const Allocator *allocator, size_t bytes) {
  return NULL;
}

static void null_allocator_dealloc(const Allocator *allocator, void *ptr) {}

Allocator HEAP_ALLOCATOR = {.alloc = heap_allocator_alloc,
                            .dealloc = heap_allocator_dealloc};
Allocator NULL_ALLOCATOR = {.alloc = null_allocator_alloc,
                            .dealloc = null_allocator_dealloc};

void *heap_alloc(size_t bytes) {
  return HEAP_ALLOCATOR.alloc(&HEAP_ALLOCATOR, bytes);
}

void heap_dealloc(void *ptr) { HEAP_ALLOCATOR.dealloc(&HEAP_ALLOCATOR, ptr); }

static void *bump_allocator_alloc(const Allocator *allocator, size_t bytes) {
  return bump_alloc(((Bump *)allocator->context), bytes);
}

void *allocator_realloc(const Allocator *allocator, void *ptr, size_t old_size, size_t bytes) {
  void *new_mem = allocator->alloc(allocator, bytes);
  if (!new_mem) {
    perror("alloc");
    exit(1);
  }

  memcpy(new_mem, ptr, old_size);
  allocator->dealloc(allocator, ptr);

  return new_mem;
}

void bump_allocator_init(Allocator *allocator, Bump *bump) {
  allocator->alloc = bump_allocator_alloc;
  allocator->dealloc = null_allocator_dealloc;
  allocator->context = bump;
}

void alloc_init() {}
