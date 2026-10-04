#include "../include/dir.h"
#include "../include/array.h"
#include "../include/str.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#endif

#ifdef _WIN32
#define mkdir(path, ...) _mkdir(path)
#define PATH_MAX MAX_PATH
#endif

i32 dir_create(const char *dir_name) { return mkdir(dir_name, 0777); }

bool dir_exists(const char *path) {
#ifndef _WIN32
  bool result = false;
  DIR *dir = opendir(path);

  if (dir != NULL) {
    result = true;
    closedir(dir);
  }

  return result;

#else

  DWORD attributes = GetFileAttributesA(path);

  return attributes != INVALID_FILE_ATTRIBUTES &&
         (attributes & FILE_ATTRIBUTE_DIRECTORY);
#endif
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
#ifndef _WIN32
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

    array_add(dir_entries, str_dup(dir_entry->d_name, alloc));
  }

  closedir(path);

  return dir_entries;
#else
  char search_path[MAX_PATH];

  int written = snprintf(search_path, sizeof(search_path), "%s\\*", path);

  if (written < 0 || (size_t)written >= sizeof(search_path)) {
    return NULL;
  }

  WIN32_FIND_DATAA find_data;
  HANDLE find_handle = FindFirstFileA(search_path, &find_data);

  if (find_handle == INVALID_HANDLE_VALUE) {
    return NULL;
  }

  char **dir_entries = array_new(char *, alloc);

  do {
    const char *name = find_data.cFileName;

    if (str_eq(name, ".") || str_eq(name, "..")) {
      continue;
    }

    array_add(dir_entries, str_dup(name, alloc));
  } while (FindNextFileA(find_handle, &find_data));

  FindClose(find_handle);

  return dir_entries;
#endif
}
