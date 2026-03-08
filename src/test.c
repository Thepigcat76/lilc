#include "../include/hashmap.h"
#include "../include/hash.h"
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

int _main(void) {
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