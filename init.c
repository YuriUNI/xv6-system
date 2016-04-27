// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "psw.h"

char *argv[] = { "sh", 0 };
int
getuser(char *buf, int nbuf)
{
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
	  return -1;
  return 0;
}
char*
gets2(char *buf, int max)
{
  int i, cc;
  char c;
  int ispwd=1;
  for(i=0; i+1 < max; ){
    cc = read2(0, &c, 1, ispwd);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
getpwd(char *buf, int nbuf)
{
  memset(buf, 0, nbuf);
  gets2(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}



void save(char *usr, char *pwd, uint uid, uint gid) {
	int fd;
	struct psw t;
	strcpy(t.username,usr);
	strcpy(t.password,pwd);
	t.uid = uid;
	t.gid = gid;
	fd = open("/psw", O_CREATE | O_RDWR);
	if (fd<0){
		printf(1, "error: create psw file failed\n");
		exit();
	}
	int size = sizeof(t);
	if (write(fd, &t, size) != size) {
		printf(1, "error: write to psw file failed\n");
		exit();
	}
	printf(1, "Set up %s account successfully\n", usr);
	close(fd);
}
int verifyuser(char *usr, char *pwd){
	struct psw t;
	int fd = open("psw", O_RDONLY);
	int size = sizeof(t);
	while(read(fd, &t, size) != 0) {
		if(!strcmp(t.password,pwd)&&!strcmp(t.username,usr))
			return 1;
	}
	return 0;
}


int
main(void)
{
  int pid, wpid;

  if(open("console", O_RDWR) < 0){
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  // Set up password and create passwd file
  static char user[MAXNAME]="root";
  static char pwd[MAXPSW];
  printf(1,"Set up root password:");
  getpwd(pwd, sizeof(pwd));
  pwd[strlen(pwd) - 1] = '\0';
  uint uid=0;
  uint gid=100;
  save(user, pwd, uid,gid);

  for(;;){
	int verify=0;
	while(!verify){
	printf(1, "Login:");
	getuser(user, sizeof(user));
	user[strlen(user) - 1] = '\0';
	printf(1, "Password:");
	getpwd(pwd, sizeof(pwd));
	pwd[strlen(pwd) - 1] = '\0';
	verify= verifyuser(user,pwd);
	}
	struct psw u;
	int fd = open("psw", O_RDONLY);
	int size = sizeof(u);
	while(read(fd, &u, size) != 0) {
		if(!strcmp(u.password,pwd)&&!strcmp(u.username,user))
			break;
	}

    printf(1, "init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){
      setuid(u.uid);
      setgid(u.gid);
      setusername(u.username);
	  setgroupname(u.groupname);
      exec("sh", argv);
      printf(1, "init: exec sh failed\n");
      exit();
    }
    while((wpid=wait()) >= 0 && wpid != pid)
      printf(1, "zombie!\n");
  }
}
