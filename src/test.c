#include "../include/dynstr.h"
#include "../include/str.h"
#include "../include/log.h"
#include "../include/array.h"

int main(void) {
  array_t(dyn_string_t) split = str_split("100, 200,30,10,", ',', &HEAP_ALLOCATOR);
  dyn_string_t *split_val;
  array_foreach(split, split_val) {
    log_info("Val: %s", split_val->string);
  }
}