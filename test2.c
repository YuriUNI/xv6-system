#include "types.h"
#include "user.h"
#include "date.h"
#include "types.h"
#include "user.h"
#include "fcntl.h"

#define N 100
#define MAXNAME 32
#define MAXPSW  32
struct user {
	char username[MAXNAME];
	uint uid;
	char password[MAXNAME];
};


int main(void) {
	struct user t;
	//char username[MAXNAME];
	//char password[MAXPSW];
	strcpy(t.password,"abc");
	strcpy(t.username,"abc");
	t.uid=10;
	int size =sizeof(struct user);
	int fd = open("/file", O_CREATE | O_RDWR);
	if (write(fd, &t, size) != size) {
		printf(1, "error: 1\n");
		exit();
	}
	strcpy(t.username,"defe");
	strcpy(t.password,"defe");
	t.uid=20;
	if (write(fd, &t, size) != size) {
		printf(1, "error: 1\n");
		exit();
	}
	close(fd);
	int i=0;
	fd = open("/file", O_RDWR);
	while(read(fd, &t,size)!=0){
			printf(1, "%s,%d,%s \n", t.password, t.uid, t.username);
			printf(1,"%d\n", i);
			i++;
	}
		close(fd);
		i--;
		printf(1,"\n");
	fd = open("/file", O_RDWR);
	struct user tmp;
	while(read(fd, &tmp,size)!=0){
		i--;
		printf(1,"%d\n", i);
		if(i==0){
			strcpy(tmp.username,"zzz");
			if (write(fd, &tmp, size) != size) {
				printf(1, "error: 3\n");
				exit();
			}
		}
	}
	close(fd);
	printf(1,"\n");

	fd = open("/file", O_RDWR);
	while(read(fd, &t,size)!=0){
		printf(1, "%s,%d,%s\n", t.password, t.uid, t.username);
	}
	close(fd);
	exit();
}
