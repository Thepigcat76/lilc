
#include "../include/alloc.h"
#include <stdint.h>
#include <stdlib.h>

static void *heap_allocator_alloc(size_t bytes) { return malloc(bytes); }

static void heap_allocator_dealloc(void *ptr) { free(ptr); }

static void *null_allocator_alloc(size_t bytes) { return NULL; }

static void null_allocator_dealloc(void *ptr) {}

Allocator HEAP_ALLOCATOR = {.alloc = heap_allocator_alloc,
                            .dealloc = heap_allocator_dealloc};
Allocator NULL_ALLOCATOR = {.alloc = null_allocator_alloc,
                            .dealloc = null_allocator_dealloc};

void alloc_init() {}