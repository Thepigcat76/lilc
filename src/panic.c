#include "../include/panic.h"
#include "../include/ansi.h"
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

static void stack_trace_print(void) {
  void *buffer[100];
  int nptrs = backtrace(buffer, 100);
  char **symbols = backtrace_symbols(buffer, nptrs);

  if (symbols == NULL) {
    perror("backtrace_symbols");
    exit(EXIT_FAILURE);
  }

  printf("Stack trace (%d frames):\n", nptrs);
  for (int i = 0; i < nptrs; i++) {
    printf("%s\n", symbols[i]);
  }

  free(symbols);
}

void *_internal_panic(i32 line, const char *file, const char *fmt, ...) {
  printf(ANSI_RED "Program panicked" ANSI_RESET " at %s:%d with ", file, line);
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  puts("");
  stack_trace_print();
  exit(1);
}
