#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  // Make several syscalls:
  // write (via printf), getpid, sleep, sbrk, uptime
  printf("countcalls: starting\n");
  int pid = getpid();                 // syscall
  printf("my pid is %d\n", pid);      // write syscall(s) via printf
  void *m = sbrk(1);                  // syscall
  (void)m;
  uint t = uptime();                  // syscall

  // Now fetch the count. Note: we arranged NOT to count getsyscallcount itself.
  int n = getsyscallcount();

  printf("syscall count so far = %d (uptime=%d)\n", n, t);
  exit(0);
}
