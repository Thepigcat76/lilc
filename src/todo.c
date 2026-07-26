#include "../include/todo.h"
#include "../include/ansi.h"
#include <execinfo.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

void *_internal_todo(i32 line, const char *file, const char *fmt, ...) {
  if (fmt != NULL) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
  }
  printf(ANSI_RED "TODO" ANSI_RESET " at %s:%d with ", file, line);
  stack_trace_print();
  exit(1);
}
