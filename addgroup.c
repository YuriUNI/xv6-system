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
        	printf(2,"Usage: addgroup <username>\n");
        	exit();
        }
        char *user =argv[1];
        char group[MAXLENGTH];


        char *path = "/psw";
        int fd =open(path, O_RDWR);
        struct psw t;
        int count=0;
        while(read(fd, &t, sizeof(t))!= 0){
        	count++;
        	if(!strcmp(t.username,user)){
        		break;
        	}
        }
        count--;
        close(fd);

        fd =open(path, O_RDWR);
        printf(1,"New Group:");
        gets(group,MAXLENGTH);
        group[strlen(group)-1]='\0';
        struct psw tmp;
        while(read(fd, &tmp, sizeof(tmp)) != 0){
            count--;
            if(count==0){
            	strcpy(t.password,group);
            	if((write(fd, &t, sizeof(t))!=sizeof(t))){
            	      printf(2,"error: change group fail\n");
            	      exit();
            	}
            	break;
            }
        }
        close(fd);
        printf(1,"New Group Set Sucessfully\n");
        exit();
    }
