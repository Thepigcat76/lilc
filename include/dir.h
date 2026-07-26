#pragma once

#include <stdbool.h>
#include "numbers.h"

bool dir_exists(const char *path);

i32 dir_create(const char *path);

i32 dirs_create(const char *path);
