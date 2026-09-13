#pragma once

typedef bool (*eq_func_t)(const void *a, const void *b);

typedef int (*hash_func_t)(const void *a);
