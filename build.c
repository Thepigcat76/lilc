// To build the project, compile this file with a compiler of your choice and
// run the compiled executable. The project also requires the gurd header, which
// can be found at <https://github.com/Thepigcat76/gurd/blob/main/gurd.h>

#include "gurd.h"

#define COMPILER "clang"
#define STANDARD "gnu23"
#define DEBUG true
#define OUT_NAME "build/test"
#define LIB_OUT_NAME "build/lilc.a"

static Cmd cmd = {0};

static bool build_lib = false;

static void visit_lib_entry(struct file_entry entry) {
  if (strcmp(entry.file_ext, "c") != 0 || strcmp(entry.name, "test.c") == 0 || entry.name[0] == '_')
    return;

  Cmd lib_compile_cmd = {0};

  cmd_appendf(&lib_compile_cmd, COMPILER);
  // Flags
  cmd_appendf(&lib_compile_cmd, "-g");
  cmd_appendf(&lib_compile_cmd, "-c");
  cmd_appendf(&lib_compile_cmd, "-std=%s", STANDARD);
  // Output location
  cmd_appendf(&lib_compile_cmd, "-o");

  cmd_appendf(&lib_compile_cmd, "./build/%s.o", entry.name);

  cmd_appendf(&lib_compile_cmd, "%s", entry.path);

  cmd_execute(&lib_compile_cmd);

  cmd_appendf(&cmd, "./build/%s.o", entry.name);

  printf("Compiled %s\n", entry.path);
}

static void visit_entry(struct file_entry entry) {
  if (strcmp(entry.file_ext, "c") != 0 || entry.name[0] == '_')
    return;
  cmd_appendf(&cmd, "%s", entry.path);
}

int main(int argc, char **argv) {
  build_lib = argc == 2 && strcmp(argv[1], "p") == 0;

  if (build_lib) {
    cmd_appendf(&cmd, "ar rcs " LIB_OUT_NAME);

    walk_dir("src", visit_lib_entry);

    cmd_execute(&cmd);

    systemf("./_publish.sh");
  } else { // The compiler to use
    cmd_appendf(&cmd, COMPILER);
    // Flags
    cmd_appendf(&cmd, "-g");
    cmd_appendf(&cmd, "-std=%s", STANDARD);
    // Output location
    cmd_appendf(&cmd, "-o");
    cmd_appendf(&cmd, OUT_NAME);

    // Adding src files
    walk_dir("src", visit_entry);

    // Run the command
    cmd_execute(&cmd);

    if (argc > 1) {
      if (strcmp(argv[1], "r") == 0) {
        systemf("./%s", OUT_NAME);
      }
    }
  }
}
