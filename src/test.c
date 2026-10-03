#include "../include/deque.h"
#include "../include/dynstr.h"
#include "../include/log.h"
#include "../include/path.h"
#include "../include/alloc.h"

int main(void) {
  dyn_string_t path = {0};
  dyn_string_init(&path, &HEAP_ALLOCATOR);
  
  file_path_parse(&path, "/home/thepigcat////Desktop//");

  file_path_extend_back(&path, "///.///");

  char *file_path_str = path.string;

  log_debug("File path: %s", file_path_str);
}
