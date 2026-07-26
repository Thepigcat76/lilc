#pragma once

#include "alloc.h"
#include "str.h"
#include "numbers.h"
#include <stdbool.h>

dyn_string_t file_read_to_string(const char *filepath, Allocator *allocator);

u8 *file_read_to_bytes(const char *filepath, size_t *length, Allocator *allocator);

bool file_exists(const char *filepath);

i32 file_rename(const char *filepath, const char *new_filename);

i32 file_remove(const char *filepath);

i32 file_copy(const char *filepath, const char *dest_filepath);

i32 file_move(const char *filepath, const char *dest_filepath);

const char *file_extension(const char *filepath);

dyn_string_t file_name(const char *filepath, Allocator *allocator);
