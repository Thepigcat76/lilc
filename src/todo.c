#include "../include/todo.h"
#include "../include/ansi.h"
#include "../include/backtrace.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void *_internal_todo(i32 line, const char *file, const char *fmt, ...) {
  if (fmt != NULL) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
  }
  printf(ANSI_RED "TODO" ANSI_RESET " at %s:%d with\n", file, line);
  dyn_string_t str = {0};
  dyn_string_init(&str, &HEAP_ALLOCATOR);
  usz frames = backtrace_sprint(64, &str);
  printf("Stack Trace (%zu Frames)\n", frames);
  printf("%s", str.string);
  dyn_string_free(&str);
  exit(1);
}
