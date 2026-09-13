#pragma once

#include <stdbool.h>
#include "numbers.h"
#include "alloc.h"
#include "array.h"

bool dir_exists(const char *path);

i32 dir_create(const char *path);

i32 dirs_create(const char *path);

array_t(char *) dir_entries_list(const char *path, allocator_t *alloc);
