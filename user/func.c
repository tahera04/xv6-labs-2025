#include "kernel/kernel.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main (void)
{
	int pid = getpid();
	sleep(1);
	void *m = sbrk(1);
	(void)m;
	int n = getsyscallcount();
	printf("syscallcount: ",n);
	exit(0);
}
