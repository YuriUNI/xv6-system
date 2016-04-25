#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;
  
  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  
  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}
char*
translateMode(uint mode)
{
	static char cmode[9];
	cmode[0] = (mode >> 10 & 1) != 0 ? 'r' : '-';
	cmode[1] = (mode >> 9 & 1) != 0 ? 'w' : '-';
	cmode[2] = (mode >> 8 & 1) != 0 ? 'x' : '-';
	cmode[3] = (mode >> 6 & 1) != 0 ? 'r' : '-';
	cmode[4] = (mode >> 5 & 1) != 0 ? 'w' : '-';
	cmode[5] = (mode >> 4 & 1) != 0 ? 'x' : '-';
	cmode[6] = (mode >> 2 & 1) != 0 ? 'r' : '-';
	cmode[7] = (mode >> 1 & 1) != 0 ? 'w' : '-';
	cmode[8] = (mode & 1) != 0 ? 'x' : '-';

	return cmode;
}


void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  
  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }
  
  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
  
  switch(st.type){
  case T_FILE:
    printf(1, "%s %d %d %d %d %d \n", fmtname(path), st.type, st.ino, st.size, st.ownerId, st.groupId);
    break;
  
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    printf(1,"name|type|inode number|size|mode|ownerId|groupId\n");
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }

      printf(1, "%s %d %d %d %x %s %d %d\n", fmtname(buf), st.type, st.ino, st.size,st.mode,translateMode(st.mode), st.ownerId, st.groupId);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit();
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit();
}
