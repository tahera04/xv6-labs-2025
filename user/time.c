#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: time command [args...]\n");
    exit(1);
  }

  int start = uptime();  // get starting ticks

  int pid = fork();
  if(pid < 0){
    fprintf(2, "time: fork failed\n");
    exit(1);
  }

  if(pid == 0){
    // child: run the command
    exec(argv[1], argv+1);
    fprintf(2, "time: exec %s failed\n", argv[1]);
    exit(1);
  }

  wait(0);  // wait for child to finish

  int end = uptime();   // get ending ticks
  int elapsed = end - start;

  // write elapsed time into time.txt
  int fd = open("time.txt", O_CREATE | O_WRONLY);
  if(fd < 0){
    fprintf(2, "time: cannot open time.txt\n");
    exit(1);
  }

  printf("%d\n",elapsed);
  close(fd);

  exit(0);
}
