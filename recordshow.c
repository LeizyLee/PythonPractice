#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

int main(int argc, char *argv[]){
	int fd, tmp, flag;
	int id;
	char *buff;
	if (argc < 2){
		fprintf(stderr, "User Guide : %s file\n", argv[0]);
		exit(1);
	}
	if ((fd = open(argv[1], O_RDONLY)) == -1){
		perror(argv[1]);
		exit(2);
	}
	printf("시작~\n");
	lseek(fd,0, SEEK_SET);
	read(fd, (char *)&buff, 10*sizeof(char));
	printf("%d\n", id);
	flag = 0;
	sleep(1);
	if( id != 0 ){
		printf("학번:%d\n", id);
	}
	while(lseek(fd,7*sizeof(char), SEEK_CUR)){
		read(fd, (char *)&buff,  10*sizeof(char));
		printf("학번:%d\n", id);
		sleep(1);
		flag = id;
	}
	close(fd);
	exit(0);
}
