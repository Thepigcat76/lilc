#pragma once

#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 1, 2)))
#endif
void *panic(const char *fmt, ...);
