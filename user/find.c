#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

// keep track exec cmd
char *g_cmd = 0;
char *g_args[MAXARG];
int arg_count = 0;

void makepath(char *buf, char *p, char *c) {
  strcpy(buf, p);
  int n = strlen(buf);
  if (buf[n-1] != '/')
    buf[n++] = '/';
  strcpy(buf+n, c);
}

// run exec on file
void runit(char *fname) {
  if (g_cmd == 0) {
    printf("%s\n", fname);
    return;
  }

  char *argv[MAXARG];
  int k;
  for (k = 0; k < arg_count; k++) {
    argv[k] = g_args[k];
  }
  argv[k++] = fname; // add file
  argv[k] = 0;

  if (fork() == 0) {
    exec(g_cmd, argv);
    exit(1); // fail case
  } else {
    wait(0);
  }
}

void look(char *dir, char *target) {
  char path[128];
  int fd;
  struct stat st;
  struct dirent de;

  fd = open(dir, 0);
  if (fd < 0) {
    fprintf(2, "cannot open %s\n", dir);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "cannot stat %s\n", dir);
    close(fd);
    return;
  }

  if (st.type == T_DIR) {
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0)
        continue;
      if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
        continue;

      makepath(path, dir, de.name);

      if (stat(path, &st) < 0) {
        fprintf(2, "cannot stat %s\n", path);
        continue;
      }

      if (st.type == T_FILE) {
        if (strcmp(de.name, target) == 0) {
          runit(path);
        }
      } else if (st.type == T_DIR) {
        look(path, target);
      }
    }
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(2, "Usage: find <dir> <name> [-exec cmd...]\n");
    exit(1);
  }

  // check for -exec
  for (int i = 3; i < argc; i++) {
    if (strcmp(argv[i], "-exec") == 0) {
      if (i+1 < argc) {
        g_cmd = argv[i+1]; // first word cmd
        for (int j = i+1; j < argc; j++) {
          g_args[arg_count++] = argv[j]; // copy rest
        }
      }
      break;
    }
  }

  look(argv[1], argv[2]);
  exit(0);
}
