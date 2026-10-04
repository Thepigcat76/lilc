#include "../include/panic.h"
#include "../include/ansi.h"
#include "../include/dynstr.h"
#include "../include/backtrace.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void *_internal_panic(i32 line, const char *file, const char *fmt, ...) {
  printf(ANSI_RED "Program panicked" ANSI_RESET " at %s:%d with\n", file, line);
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  puts("");
  
  dyn_string_t str = {0};
  dyn_string_init(&str, &HEAP_ALLOCATOR);
  usz frames = backtrace_sprint(64, &str);
  printf("Stack Trace (%zu Frames)\n", frames);
  printf("%s", str.string);
  dyn_string_free(&str);
  exit(1);
}
