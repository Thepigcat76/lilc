#include "../include/backtrace.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define _WIN32_WINNT 0x0600

// clang-format off
#include <windows.h>
#include <dbghelp.h>
// clang-format on

#pragma comment(lib, "dbghelp.lib")

usz backtrace_sprint(usz max_frames, dyn_string_t *buf) {
  void *frames[max_frames];
  USHORT count;

  HANDLE process = GetCurrentProcess();

  SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);

  if (!SymInitialize(process, NULL, TRUE)) {
    fprintf(stderr, "SymInitialize failed: %lu\n", GetLastError());
    return 0;
  }

  count = CaptureStackBackTrace(1, // skip print_backtrace itself
                                max_frames, frames, NULL);

  for (USHORT i = 0; i < count; ++i) {
    DWORD64 address = (DWORD64)(uintptr_t)frames[i];

    char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME];
    PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;

    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = MAX_SYM_NAME;

    DWORD64 displacement = 0;

    if (SymFromAddr(process, address, &displacement, symbol)) {
      dyn_string_add_strf(buf, "#%u %s + 0x%llx\n", i, symbol->Name,
                          (unsigned long long)displacement);
    } else {
      dyn_string_add_strf(buf, "#%u 0x%llx\n", i, (unsigned long long)address);
    }
  }

  SymCleanup(process);

  return count;
}
#else

#include <execinfo.h>
#include <unistd.h>

usz backtrace_sprint(usz max_frames, dyn_string_t *buf) {
  void *buffer[max_frames + 1];
  int nptrs = backtrace(buffer, max_frames + 1);
  char **symbols = backtrace_symbols(buffer, nptrs);

  if (symbols == NULL) {
    perror("backtrace_symbols");
    return 0
  }

  for (int i = 0; i < nptrs - 1; i++) {
    dyn_string_add_strf(buf, "#%d %s\n", symbols[i], i);
  }

  free(symbols);

  return nptrs;
}
#endif
