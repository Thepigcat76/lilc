#include "../include/dynstr.h"
#include "../include/log.h"

int main(void) {
  dyn_string_t str = {0};
  dyn_string_init(&str, &HEAP_ALLOCATOR);

  //dyn_string_copy_str(&str, "BALLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLZ");

  log_info("Dyn str cap: %zu", str.capacity);

  dyn_string_printf(&str, "lol %s", "hahaha");

  log_info("Dyn str cap: %zu", str.capacity);

  dyn_string_add_strf(&str, ". STOP %s :%d", "James", 3);

  log_info("Dyn str cap: %zu, dyn str len: %zu", str.capacity, str.len);

  log_info("STRING: %s", str.string);

  dyn_string_remove_last_char(&str);

  log_info("STRING: %s", str.string);

  dyn_string_free(&str);
}