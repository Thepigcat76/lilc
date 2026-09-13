#include "../include/dir.h"
#include "../include/str.h"
#include "../include/array.h"

#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

i32 dir_create(const char *dir_name) {
#ifdef _WIN32
#include <direct.h> // For _mkdir on Windows
  return _mkdir(dir_name);
#else
#include <sys/stat.h>
#include <sys/types.h>
  return mkdir((dir_name), 0777);
#endif
}

bool dir_exists(const char *path) {
  bool result = false;
  DIR *dir = opendir(path);

  if (dir != NULL) {
    result = true;
    closedir(dir);
  }

  return result;
}

i32 dirs_create(const char *path) {
  if (path == NULL || !*path) {
    errno = EINVAL;
    return -1;
  }

  char tmp[PATH_MAX];
  if (strlen(path) >= sizeof(tmp)) {
    errno = ENAMETOOLONG;
    return -1;
  }
  strcpy(tmp, path);

  size_t len = strlen(tmp);
  while (len > 1 && tmp[len - 1] == '/')
    tmp[--len] = '\0';

  for (char *p = tmp + 1; *p; p++) {
    if (*p == '/') {
      *p = '\0';
      if (dir_create(tmp) != 0 && errno != EEXIST)
        return -1;
      *p = '/';
    }
  }

  if (dir_create(tmp) != 0 && errno != EEXIST)
    return -1;
  return 0;
}

char **dir_entries_list(const char *path, allocator_t *alloc) {
  DIR *dir = opendir(path);
  if (dir == NULL) {
    return NULL;
  }

  struct dirent *dir_entry = NULL;

  char **dir_entries = array_new(char *, alloc);

  while ((dir_entry = readdir(dir)) != NULL) {
    if (str_eq(dir_entry->d_name, ".") || str_eq(dir_entry->d_name, "..")) {
      continue;
    }

    array_add(dir_entries, dir_entry->d_name);
    
  }

  return dir_entries;
}
