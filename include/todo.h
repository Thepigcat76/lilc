#pragma once

#define TODO(...) _internal_todo(__VA_ARGS__ __VA_OPT__(,) NULL);

void *_internal_todo(const char *fmt, ...);
