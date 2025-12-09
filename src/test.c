#include "../include/hashmap.h"
#include "../include/hash.h"
#include "../include/eq.h"
#include <stdio.h>

int main(void) {
  Hashmap(char *, int) map = hashmap_new(char **, int, &HEAP_ALLOCATOR, str_ptrv_hash, str_ptrv_eq, NULL);
  char *jeff_k = "Jeff";
  int jeff = 100;
  hashmap_insert(&map, &jeff_k, &jeff);
  char *x_k = "Gustafeeeeeeeeeeeeeeeee";
  int x = 400;
  hashmap_insert(&map, &x_k, &x);

  char *y_k = "Slay";
  int y = 300;
  hashmap_insert(&map, &y_k, &y);

  printf("Hello\n");

  hashmap_foreach(&map, char **key, int *val, {
    printf("Key: %s, Value: %d\n", *key, *val);
  });
  
}