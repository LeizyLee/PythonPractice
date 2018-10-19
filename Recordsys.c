#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "student.h"
#define true 1
#define false 0

void create_record(int fd, struct student record, int argc, char *argv[]){
	int id;
	if ((fd = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0640)) == -1){
		perror(argv[1]);
		exit(2);
	}
	printf("%-9s %-8s %-4s\n", "학번", "이름", "점수");
	while (true){
		scanf("%d ", &id);
    if(id == -1){
			break;
		}
		else{
			record.id = id;
			scanf("%s %d", record.name, &record.score);
			record.de = false;
	    lseek(fd, (record.id - START_ID) * sizeof(record), SEEK_SET);
			write(fd, (char *) &record, sizeof(record) );
		}
	}
	close(fd);
}

void show_record(int fd, struct student record, char *argv[]){
	int id;
	if ((fd = open(argv[2], O_RDONLY)) == -1){
		perror(argv[1]);
		exit(2);
	}
	lseek(fd,0L, SEEK_SET);
	read(fd, (char *) &record, sizeof(record));
	if(record.id != 0 && !record.de){
		printf("이름:%s\t 학번:%d\t 점수:%d\n", record.name, record.id, record.score);
	}
	int tmp = record.id;
	while(lseek(fd,sizeof(record), SEEK_CUR)){
		read(fd, (char *) &record, sizeof(record));
		if( record.id != 0 && !record.de){
			if(tmp == record.id )
				break;
			printf("이름:%s\t 학번:%d\t 점수:%d\n", record.name, record.id, record.score);

		}
		tmp = record.id;
	}
	close(fd);
}

void edit_record(int fd, struct student record, char *argv[]){
	int id;
	if ((fd = open(argv[2], O_RDWR)) == -1){
		perror(argv[1]);
		exit(2);
	}
	printf("수정할 학생의 학번 입력: ");
	if (scanf("%d", &id) == 1){
		lseek(fd, (long) (id-START_ID)*sizeof(record), SEEK_SET);
		if ((read(fd, (char *)&record, sizeof(record)) > 0 ) && (record.id != 0)) {
			printf("학번:%8d\t 이름:%4s\t 점수:%4d\n", record.id, record.name, record.score);
			printf("새로운 점수: ");
			scanf("%d", &record.score);
			lseek(fd, (long) -sizeof(record), SEEK_CUR);
			write(fd, (char*) &record, sizeof(record));
		}else printf("레코드 %d 없음\n", id);
	}else printf("입력오류\n");
	close(fd);
}

void del_record(int fd, struct student record, char *argv[]){
	int id;
	if ((fd = open(argv[2], O_RDWR)) == -1){
		perror(argv[1]);
		exit(2);
	}
	printf("삭제할 학생의 학번 입력: ");
	if (scanf("%d", &id) == 1){
		lseek(fd, (long) (id-START_ID)*sizeof(record), SEEK_SET);
		if ((read(fd, (char *)&record, sizeof(record)) > 0 ) && (record.id != 0)) {
			printf("학번:%8d\t 이름:%4s\t 점수:%4d\n", record.id, record.name, record.score);
			record.de = true;
			lseek(fd, (long) -sizeof(record), SEEK_CUR);
			write(fd, (char*) &record, sizeof(record));
		}else printf("레코드 %d 없음\n", id);
	}else printf("입력오류\n");
	close(fd);
}

int main(int argc, char *argv[]){
	int fd;
	char c;
	struct student record;

	if (argc == 2 && !strcmp(argv[1], "help")){
		printf("01. Make Record : %s mk file\n02. Show Record : %s show file\n", argv[0], argv[0]);
		printf("03. Edit Record : %s edit file\n04. Delete Data : %s rm file\n", argv[0], argv[0]);
		exit(1);
	}
	else if(argc == 3 && !strcmp(argv[1], "mk")){
		create_record(fd, record, argc, argv);
	}
	else if(argc == 3 && !strcmp(argv[1], "show")){
		show_record(fd, record, argv);
	}
	else if(argc == 3 && !strcmp(argv[1], "edit")){
		edit_record(fd, record, argv);
	}
	else if(argc == 3 && !strcmp(argv[1], "rm")){
		del_record(fd, record, argv);
	}
	else{
		fprintf(stderr, "User Guide : %s help\n", argv[0]);
		exit(1);
	}
  close(fd);
  exit(0);
}
