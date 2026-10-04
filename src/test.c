#include "../include/deque.h"
#include "../include/dynstr.h"
#include "../include/log.h"
#include "../include/path.h"
#include "../include/alloc.h"

int main(void) {
  dyn_string_t path = {0};
  dyn_string_init(&path, &HEAP_ALLOCATOR);

  dyn_string_copy_str(&path, "/u/../pol/ui///.././.././io//");
  
  file_path_resolve(&path);

  char *file_path_str = path.string;

  log_debug("File path: %s", file_path_str);
}
