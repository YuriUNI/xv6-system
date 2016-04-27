#include "syscall.h"
#include "traps.h"

#define SYSCALL(name) \
  .globl name; \
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
SYSCALL(exit)
SYSCALL(wait)
SYSCALL(pipe)
SYSCALL(read)
SYSCALL(write)
SYSCALL(close)
SYSCALL(kill)
SYSCALL(exec)
SYSCALL(open)
SYSCALL(mknod)
SYSCALL(unlink)
SYSCALL(fstat)
SYSCALL(link)
SYSCALL(mkdir)
SYSCALL(chdir)
SYSCALL(dup)
SYSCALL(getpid)
SYSCALL(sbrk)
SYSCALL(sleep)
SYSCALL(uptime)
SYSCALL(date)
SYSCALL(chmod)
SYSCALL(halt)
SYSCALL(mkfile)
SYSCALL(getuid)
SYSCALL(setuid)
SYSCALL(fsck)
SYSCALL(isdir)
SYSCALL(read2)
SYSCALL(getinode)
SYSCALL(tree)
SYSCALL(getgid)
SYSCALL(setgid)
SYSCALL(setusername)
SYSCALL(setgroupname)
SYSCALL(getusername)
SYSCALL(getgroupname)




