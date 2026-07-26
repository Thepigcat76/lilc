
#include "../include/alloc.h"
#include "../include/panic.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void *heap_allocator_alloc(Allocator *allocator, size_t bytes) {
  return malloc(bytes);
}

static void heap_allocator_dealloc(Allocator *allocator, void *ptr) {
  free(ptr);
}

static void *heap_allocator_realloc(Allocator *allocator, void *prev_ptr,
                                    size_t old_size, size_t new_size) {
  return realloc(prev_ptr, new_size);
}

static void *null_allocator_alloc(Allocator *allocator, size_t bytes) {
  return NULL;
}

static void null_allocator_dealloc(Allocator *allocator, void *ptr) {}

static void *null_allocator_realloc(Allocator *allocator, void *prev_ptr,
                                    size_t old_size, size_t new_size) {
  return NULL;
}

Allocator HEAP_ALLOCATOR = {
    .alloc = heap_allocator_alloc,
    .dealloc = heap_allocator_dealloc,
    .realloc = heap_allocator_realloc,
};
Allocator NULL_ALLOCATOR = {
    .alloc = null_allocator_alloc,
    .dealloc = null_allocator_dealloc,
    .realloc = null_allocator_realloc,
};

void *heap_alloc(size_t bytes) {
  return HEAP_ALLOCATOR.alloc(&HEAP_ALLOCATOR, bytes);
}

void heap_dealloc(void *ptr) { HEAP_ALLOCATOR.dealloc(&HEAP_ALLOCATOR, ptr); }

static void *bump_allocator_alloc(Allocator *allocator, size_t bytes) {
  return bump_alloc(((Bump *)allocator->context), bytes);
}

static void *bump_allocator_realloc(Allocator *allocator, void *prev_ptr,
                                    size_t old_size, size_t new_size) {
  Bump *bump = (Bump *)allocator->context;
  void *new_mem = bump_alloc(bump, new_size);
  if (new_mem == NULL) {
    panic("Failed to allocate memory for realloc");
  }

  memcpy(new_mem, prev_ptr, old_size);
  return new_mem;
}

void bump_allocator_init(Allocator *allocator, Bump *bump) {
  allocator->alloc = bump_allocator_alloc;
  allocator->dealloc = null_allocator_dealloc;
  allocator->realloc = bump_allocator_realloc;
  allocator->context = bump;
}

void alloc_init() {}
