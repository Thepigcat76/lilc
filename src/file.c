#include "../include/file.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

dyn_string_t file_read_to_string(const char *filename, Allocator *allocator) {
  dyn_string_t str = {0};

  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    fprintf(stderr, "Error opening file %s - ", filename);
    perror("");
    return str;
  }

  // Seek to the end of the file to get the file size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET); // Go back to the beginning of the file

  if (file_size < 0) {
    perror("Error determining file size");
    fclose(file);
    return str;
  }

  // Allocate memory for the file content + null terminator
  char *buffer = (char *)allocator->alloc(allocator, file_size + 1);
  if (buffer == NULL) {
    perror("Error allocating memory");
    fclose(file);
    return str;
  }

  // Read the file content into the buffer
  size_t read_size = fread(buffer, 1, file_size, file);
  if (read_size != file_size) {
    perror("Error reading file");
    allocator->dealloc(allocator, buffer);
    fclose(file);
    return str;
  }

  // Null-terminate the string
  buffer[file_size] = '\0';

  fclose(file);

  str.allocator = allocator;
  str.capacity = file_size + 1;
  str.len = file_size;
  str.string = buffer;
  str.term_len = file_size + 1;

  return str;
}

u8 *file_read_to_bytes(const char *filename, size_t *length,
                       Allocator *allocator) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    fprintf(stderr, "Error opening file %s - ", filename);
    perror("");
    return NULL;
  }

  // Seek to the end to get size
  if (fseek(file, 0, SEEK_END) != 0) {
    fprintf(stderr, "Error seeking file %s - ", filename);
    perror("");
    fclose(file);
    return NULL;
  }

  long file_size = ftell(file);
  *length = file_size;

  if (file_size < 0) {
    fprintf(stderr, "Error determining file size %s - ", filename);
    perror("");
    fclose(file);
    return NULL;
  }

  if (fseek(file, 0, SEEK_SET) != 0) {
    fprintf(stderr, "Error seeking file %s - ", filename);
    perror("");
    fclose(file);
    return NULL;
  }

  // Allocate exactly file_size bytes (no terminator for raw bytes)
  u8 *buffer = (u8 *)allocator->alloc(allocator, (size_t)file_size);
  if (buffer == NULL) {
    fprintf(stderr, "Error allocating %ld bytes for %s - ", file_size,
            filename);
    perror("");
    fclose(file);
    return NULL;
  }

  size_t read_size = fread(buffer, 1, (size_t)file_size, file);
  if (read_size != (size_t)file_size) {
    fprintf(stderr, "Error reading file %s - ", filename);
    perror("");
    allocator->dealloc(allocator, buffer);
    fclose(file);
    return NULL;
  }

  fclose(file);

  return buffer;
}

bool file_exists(const char *filepath) {
  FILE *f = fopen(filepath, "r");
  bool exists = f != NULL;
  if (exists) {
    fclose(f);
  }
  return exists;
}

i32 file_rename(const char *filepath, const char *new_filename) {
  if (filepath == NULL)
    return 1;

  return rename(filepath, new_filename);
}

i32 file_remove(const char *filepath) {
  if (filepath == NULL)
    return 1;

  return remove(filepath);
}

i32 file_copy(const char *filepath, const char *dest_filepath) {
  FILE *out = fopen(dest_filepath, "wb");
  if (out == NULL) {
    fclose(out);
    return -1;
  }

  size_t in_content_len = 0;
  u8 *in_content =
      file_read_to_bytes(filepath, &in_content_len, &HEAP_ALLOCATOR);

  if (fwrite(in_content, 1, in_content_len, out) != in_content_len) {
    fclose(out);
    heap_dealloc(in_content);
    errno = EIO;
    return -1;
  }

  heap_dealloc(in_content);
  if (fclose(out) != 0)
    return -1;

  return 0;
}

i32 file_move(const char *filepath, const char *dest_filepath) {

  if (filepath == NULL)
    return 1;

  return rename(filepath, dest_filepath);
}

const char *file_extension(const char *filepath) {
  const char *dot = strrchr(filepath, '.');

  if (dot == NULL || strlen(dot) <= 1) {
    return NULL;
  }

  return dot + 1;
}

dyn_string_t file_name(const char *filepath, Allocator *allocator) {
  if (!filepath)
    return (dyn_string_t){0};

  const char *slash = strrchr(filepath, '/');

  size_t len_after_slash;
  if (slash != NULL) {
    len_after_slash = strlen(slash);
  } else {
    len_after_slash = strlen(filepath);
  }

  if (len_after_slash <= 1) {
    return (dyn_string_t){0};
  }

  const char *base;
  if (slash != NULL) {
    base = slash + 1;
  } else {
    base = filepath;
  }

  const char *dot = strrchr(base, '.');
  if (dot == NULL) {
    dot = slash + len_after_slash;
  }

  size_t len = dot - base;

  dyn_string_t str = {0};
  dyn_string_init(&str, allocator);

  dyn_string_copy_str_len(&str, base, len);

  return str;
}
