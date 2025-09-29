#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// sixfive
int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(2, "Usage: sixfive file\n");
    exit(1);
  }

  int fd = open(argv[1], 0);
  if (fd < 0) {
    fprintf(2, "sixfive: cannot open %s\n", argv[1]);
    exit(1);
  }

  char c;
  int number = 0;
  int in_number = 0;

  // read one char one time
  while (read(fd, &c, 1) == 1) {
    if (strchr(" -\r\t\n./,", c)) {  // if sep
      if (in_number) {
        if ((number % 5 == 0 || number % 6 == 0) && number != 0) {
          printf("%d\n", number);
        }
        number = 0;
        in_number = 0;
      }
    } else if (c >= '0' && c <= '9') {  // digit
      number = (number * 10) + (c - '0');
      in_number = 1;
    } else {
      // not digit and not sep
      if (in_number) {
        number = 0;
        in_number = 0;
      }
    }
  }

  // last number check
  if (in_number && (number % 5 == 0 || number % 6 == 0) && number != 0) {
    printf("%d\n", number);
  }

  close(fd);
  exit(0);
}
