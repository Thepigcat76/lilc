// To build the project, compile this file with a compiler of your choice and
// run the compiled executable. The project also requires the gurd header, which
// can be found at <https://github.com/Thepigcat76/gurd/blob/main/gurd.h>

#include ".gurd/helper.h"
#include <dirent.h>
#include <errno.h>
#include <gurd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef enum {
  TARGET_LINUX,
  TARGET_WIN,
} BuildTarget;

typedef struct {
  char *compiler;
  bool debug;
  bool release;
  char *std;
  BuildTarget target;
  char *out_dir;
  char *out_name;
  char *libraries[64];
  char *define_flags[64];
  char *extra_flags[64];
} BuildOptions;

static BuildOptions OPTS = {.compiler = "clang",
                            .debug = true,
                            .release = false,
                            .std = "gnu23",
                            .target = TARGET_LINUX,
                            .out_dir = "./build/",
                            .out_name = "lilc"};

static void publish();

static int move_file(const char *src, const char *dst);

int main(int argc, char **argv) {
  publish();
  if (argc >= 2) {
    if (STR_CMP_OR(argv[1], "-r", "--release")) {
      OPTS.release = true;
      OPTS.debug = false;
    }
  }

  char *compiler = build_compiler(OPTS.compiler, OPTS.target);
  collect_src_files_for_cache("./src/");
  char *libraries = link_libs_target(OPTS.libraries, OPTS.target);
  char *flags = build_flags(&OPTS);
  char *out_name = build_name(OPTS.out_name, OPTS.target);
  char *defined_flags = build_def_flags(OPTS.define_flags);
  char *extra_flags = build_ex_flags(OPTS.extra_flags);

  make_dir(OPTS.out_dir);
  cached_compile(compiler, libraries, flags, defined_flags, extra_flags,
                 OPTS.out_dir, out_name);
  printf("Comand: %s\n", _internal_cmd_buf);

  if (move_file("./build/lilc.a", "/usr/lib/liblilc.a") == -1) {
    perror("rename failed");
    return EXIT_FAILURE;
  }

  if (argc >= 2) {
    if (STR_CMP_OR(argv[1], "r", "run")) {
      return run(OPTS.out_dir, out_name, argc, argv);
    } else if (STR_CMP_OR(argv[1], "d", "dbg")) {
      dbg(OPTS.out_dir, out_name, OPTS.debug);
      return 0;
    } else if (STR_CMP_OR(argv[1], "p", "publish")) {
      publish();
      return 0;
    } else if (STR_CMP_OR(argv[1], "v", "vg")) {
      system(str_fmt("valgrind --leak-check=full --show-leak-kinds=all ./%s/%s",
                     OPTS.out_dir, out_name));
      return 0;
    } else {
      fprintf(stderr, "[Error]: Invalid first arg: %s\n", argv[1]);
      return 1;
    }
  }
}

static void publish() {
  const char *include_dir = "/usr/include/";

  if (mkdir(str_fmt("%s/%s", include_dir, OPTS.out_name), 0777)) {
    // perror("mkdir");
    // exit(1);
  }

  DIR *local_include_dir = opendir("./include/");
  struct dirent *entry;

  while ((entry = readdir(local_include_dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    FILE *local_file = fopen(str_fmt("%s/%s", "include", entry->d_name), "rb");
    if (!local_file) {
      perror("fopen local_file");
      exit(1);
    }

    FILE *include_dir_file = fopen(
        str_fmt("%s/%s/%s", include_dir, OPTS.out_name, entry->d_name), "wb");
    if (!include_dir_file) {
      perror("fopen include_dir_file");
      fclose(local_file);
      exit(1);
    }

    char buf[8192];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), local_file)) > 0) {
      fwrite(buf, 1, n, include_dir_file);
    }

    fclose(local_file);
    fclose(include_dir_file);
  }
}

static int move_file(const char *src, const char *dst) {
  if (rename(src, dst) == 0) {
    return 0; // success
  }
  if (errno != EXDEV) {
    return -1; // some other error
  }

  // Cross-device: copy + unlink
  FILE *in = fopen(src, "rb");
  if (!in)
    return -1;
  FILE *out = fopen(dst, "wb");
  if (!out) {
    fclose(in);
    return -1;
  }

  char buf[8192];
  size_t n;
  while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
    if (fwrite(buf, 1, n, out) != n) {
      fclose(in);
      fclose(out);
      return -1;
    }
  }

  fclose(in);
  fclose(out);

  // Delete old file
  if (unlink(src) == -1)
    return -1;

  return 0;
}

static char *build_name(char *default_name, int target) {
  if (target == TARGET_WIN) {
    strcpy(_internal_build_name_buf, str_fmt("%s.exe", default_name));
    return _internal_build_name_buf;
  }
  return default_name;
}

static char *build_compiler(char *default_compiler, int target) {
  if (target == TARGET_WIN) {
    return "x86_64-w64-mingw32-gcc";
  }
  return default_compiler;
}

static char *build_flags(void *_opts) {
  BuildOptions *opts = (BuildOptions *)_opts;
  _internal_flags_buf[0] = '\0';

  if (opts->debug) {
    strcat(_internal_flags_buf, "-g ");
    strcat(_internal_flags_buf, "-DDEBUG_BUILD ");
  } else if (opts->release) {
    strcat(_internal_flags_buf, "-O3 ");
  }

  if (opts->std != NULL) {
    strcat(_internal_flags_buf, str_fmt("-std=%s ", opts->std));
  }

  return _internal_flags_buf;
}
