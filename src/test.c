#include "../include/hashmap.h"
#include "../include/hash.h"
#include "../include/str.h"
#include "../include/eq.h"
#include <stdio.h>

typedef struct {
  char **path;
} Path;

int32_t module_path_ptrv_hash(const void *array) {
  const Path *path = array;

  int32_t hash = 1;
  if (path != NULL) {
    size_t len = array_len(path->path);
    for (size_t i = 0; i < len; i++) {
      char * ident = path->path[i];
      hash = 31 * hash + (ident == NULL ? 0 : strv_hash(ident));
    }
  }
  return hash;
}

bool module_path_ptrv_eq(const void *array0, const void *array1) {
  const Path *path0 = array0;
  const Path *path1 = array1;

  if (path0 == NULL || path1 == NULL)
    return false;

  size_t len0 = array_len(path0->path);
  size_t len1 = array_len(path1->path);

  if (len0 != len1)
    return false;

  for (size_t i = 0; i < len0; i++) {
    if (!strv_eq(path0->path[i], path1->path[i])) return false;
  }

  return true;
}

#define BYTEBUF_SIZE 1024

int _main(void) {
  Bump bump = {0};
  bump_init(&bump, BYTEBUF_SIZE);

  Allocator bump_alloc = {0};
  bump_allocator_init(&bump_alloc, &bump);

  char *str = bump_alloc.alloc(&bump_alloc, 128);
  strcpy(str, "Hello World");
  puts(str);

  dyn_string_t ds = {0};
  dyn_string_init(&ds, &bump_alloc);

  dyn_string_add_str(&ds, "i love cccccc");

  puts(ds.string);

  char *str0 = bump_alloc.alloc(&bump_alloc, 128);
  strcpy(str0, "Hello World 0");
  puts(str0);

  size_t *balls = array_new_capacity(size_t, 10, &bump_alloc);
  array_add(balls, 40);
  array_add(balls, 30);
  array_add(balls, 20);
  array_add(balls, 10);

  for (size_t i = 0; i < array_len(balls); i++) {
    printf("Size: %zu\n", balls[i]);
  }

  size_t *ball;
  array_foreach(balls, ball) {
    printf("Ball: %zu\n", *ball);
  }

  printf("String: %s\n", ds.string);

  bump_free(&bump);

/*
  Bump bump = {0};
  bump_init(&bump, 8);

  size_t i = 112;
  void *mem0 = bump_alloc(&bump, sizeof(size_t));
  memcpy(mem0, &i, sizeof(size_t));

  printf("Integer: %zu\n", i);
  printf("Allocated integer: %zu\n", *(size_t *) mem0);

  size_t j = 211;
  void *mem1 = bump_alloc(&bump, sizeof(size_t));
  memcpy(mem1, &j, sizeof(size_t));

  printf("Integer: %zu\n", j);
  printf("Allocated integer: %zu\n", *(size_t *) mem1);

  struct big_struct {
    size_t a;
    size_t b;
  };

  struct big_struct k = {.a = 100, .b = 200};
  struct big_struct *mem2 = bump_alloc(&bump, sizeof(struct big_struct));
  memcpy(mem2, &k, sizeof(struct big_struct));

  printf("Integer: %zu, %zu\n", k.a, k.b);
  printf("Allocated integer: %zu, %zu\n", mem2->a, mem2->b);

  printf("");
  */

  /*
  Hashmap(Path, int) map = hashmap_new(Path, int, &HEAP_ALLOCATOR, module_path_ptrv_hash, module_path_ptrv_eq, NULL);
  char *jeff_k = "Jeff";
  char *jeff_d = "Ballz";
  Path path = {.path = array_new(char *, &HEAP_ALLOCATOR)};
  array_add(path.path, jeff_k);
  array_add(path.path, jeff_d);
  int jeff = 100;
  hashmap_insert(&map, &path, &jeff);
  char *x_k = "Gustafeeeeeeeeeeeeeeeee";
  char *x_d = "wwwwwwwWwwop";
  Path path1 = {.path = array_new(char *, &HEAP_ALLOCATOR)};
  array_add(path1.path, x_k);
  array_add(path1.path, x_d);
  int x = 400;
  hashmap_insert(&map, &path1, &x);

  char *y_k = "Slay";
  char *y_d = "yals";
  Path path2 = {.path = array_new(char *, &HEAP_ALLOCATOR)};
  array_add(path2.path, y_k);
  array_add(path2.path, y_d);
  int y = 300;
  hashmap_insert(&map, &path2, &y);

  printf("Hello\n");

  hashmap_foreach(&map, Path *key, int *val, {
    printf("Key: %s/%s, Value: %d\n", key->path[0], key->path[1], *val);
  });

  Path path4 = {.path = array_new(char *, &HEAP_ALLOCATOR)};
  char *v = "Slay";
  array_add(path4.path, v);
  v = "yals";
  array_add(path4.path, v);

  int *t = hashmap_value(&map, &path4);
  printf("T: %d\n", *t);
*/


}