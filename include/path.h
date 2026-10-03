#pragma once

#include "alloc.h"
#include "dynstr.h"

void file_path_parse(dyn_string_t *str, const char *path);

dyn_string_t file_path_makef(allocator_t *alloc, const char *fmt, ...);

void file_path_extend_front(dyn_string_t *str, const char *fmt, ...);

void file_path_extend_back(dyn_string_t *str, const char *fmt, ...);
