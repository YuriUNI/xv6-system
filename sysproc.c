#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_setuid(void)
{
	int id;
	if(argint(0, &id) < 0)
	    return -1;
  return proc->curUserId=id;
}

int
sys_getuid(void)
{
  return (int) proc->curUserId;
}

int
sys_getgid(void)
{
  return (int) proc->groupId;
}
int
sys_setgid(void)
{
	int id;
	if(argint(0, &id) < 0)
	    return -1;
  return proc->groupId=id;
}


int
sys_setgroupname(void)
{
	char *gname;
	int i = 0;
	if(argstr(0, &gname) < 0)
	    return -1;
	  while(*(gname + i) !='\0'){
		  *(proc->groupname+i) = *(gname + i);
		  i++;
	  }
  return 0;
}

int
sys_setusername(void)
{
	char *uname;
	int i=0;
	if(argstr(0, &uname) < 0)
	    return -1;
	while(*(uname+i)!='\0'){
		*(proc->username+i)=*(uname + i);
		i++;
  	}
  return 0;
}
char*
sys_getgroupname(void)
{

  return proc->groupname;

}

char*
sys_getusername(void)
{

  return proc->username;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_date(void){
  int addr; 
  if(argint(0, &addr )<0)
    return -1;
  cmostime((struct rtcdate *) addr);
  return 0;
}

int sys_halt(void){
	char *p = "Shutdown";
   	for( ; *p; p++)
   		outw(0xB004, 0x2000);
   	return 0;
}


