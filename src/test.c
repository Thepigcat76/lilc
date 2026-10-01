#include "../include/deque.h"
#include "../include/dynstr.h"
#include "../include/log.h"
#include "../include/str.h"

int main(void) {
  i64 *deque;
  deque_init(deque, &HEAP_ALLOCATOR);

  deque_push_back(deque, 10);
  deque_push_back(deque, 230);
  deque_push_front(deque, 40);
  deque_push_back(deque, 230);

  log_debug("Front: %ld", *(i64 *)deque_front(deque));

  i64 *elem1;
  deque_foreach(deque, elem1) {
    log_debug("Index: %zu, elem: %ld", _internal_deque_idx, *elem1);
  }
}
