#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

void create_record(int fd, int id, struct student record, int argc, char *argv[]){
  if ((fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0640)) == -1){
		perror(argv[1]);
		exit(2);
	}
	printf("%-9s %-8s %-4s\n", "학번", "이름", "점수");
	while (scanf("%d %s %d", &record.id, record.name, &record.score) == 3){
    if(record.id == -1)
        return;
    lseek(fd, (record.id - START_ID) * sizeof(record), SEEK_SET);
		write(fd, (char *) &record, sizeof(record) );
	}
}

int main(int argc, char *argv[]){
    int fd, id;
    char c;
    struct student record;
    if (argc <2){
      fprintf(stderr, "user Guide : %s file\n", argv[0]);
      exit(1);
    }
    create_record(fd, id, record, argc, argv);
}
