#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "psw.h"

#define MAXLENGTH 30
    int
    main(int argc, char *argv[])
    {
        if(argc < 2){
        	printf(2,"Usage: passwd <username> ");
        	exit();
        }
        char *user =argv[1];
        char pwd[MAXLENGTH];


        char *path = "/psw";
        int fd =open(path, O_RDWR);
        struct psw t;
        int count=0;
        while((read(fd, &t, sizeof(t))) != 0){
        	if(!strcmp(t.username,user)){
        		break;
        	}
        	count++;
        }
        count--;
        close(fd);

        fd =open(path, O_RDWR);
        gets(pwd,MAXLENGTH);
        while((read(fd, &t, sizeof(t))) != 0){
            count--;
            if(count==0){
            	strcpy(t.password,pwd);
            	if((write(fd, &t, sizeof(t))!=sizeof(t))){
            	      printf(2,"error: change passwd fail");
            	      return -1;
            	}
            	break;
            }
        }
        close(fd);
        exit();
    }
