#pragma once

#include <stddef.h>
#include "alloc.h"

typedef struct {
  char *string;
  size_t len;
  // Length of the string with the null terminator
  size_t term_len;
  size_t capacity;
  Allocator *allocator;
} dyn_string_t;

void dyn_string_init(dyn_string_t *str, Allocator *allocator);

void dyn_string_add_char(dyn_string_t *str, char c);

void dyn_string_add_str(dyn_string_t *str, const char *c);

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 2, 3)))
#endif
void dyn_string_printf(dyn_string_t *str, const char *fmt, ...);

void dyn_string_copy(dyn_string_t *dest, const dyn_string_t *src);

void dyn_string_copy_str(dyn_string_t *dest, const char *src);

// Creates a temporary copy in a static string buffer and frees the original dyn string
char *dyn_string_temp_copy_and_free(dyn_string_t str);

// Null term handled automatically
void dyn_string_copy_str_len(dyn_string_t *dest, const char *src, size_t len);

void dyn_string_clear(dyn_string_t *str);

void dyn_string_free(dyn_string_t *str);

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 2, 3)))
#endif
// Helper function to quickly create a dynstr
dyn_string_t dyn_string_makef(Allocator *alloc, const char *fmt, ...);
