#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

int main(int argc, char *argv[])
{
	int fd, cfd, i, j, k, tmp;
	int cache[100];
	struct student record;
	if (argc < 2){
		fprintf(stderr, "User Guide : %s file\n", argv[0]);
		exit(1);
	}
	if ((fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0640)) == -1){
		perror(argv[1]);
		exit(2);
	}
	cfd = open(strcat(argv[1],"_cache"), O_WRONLY | O_CREAT | O_EXCL, 0640);
	i = 0;

	printf("%-9s %-8s %-4s\n", "학번", "이름", "점수");
	while (scanf("%d %s %d", &record.id, record.name, &record.score) == 3){
		if (record.id == -1)
			break;
		cache[i] = record.id;
		i = i + 1;
		lseek(fd, (record.id - START_ID) * sizeof(record), SEEK_SET);
		write(fd, (char *) &record, sizeof(record) );
	}
	for(j = 0; j < i; j++){
		printf("%d\t", cache[j]);
	}
	printf("\n");
	tmp = cache[0];
	for(j = 0; j < i; j++){
		for(k = 0; k < i; k++){
			if(cache[j] > cache[i])
			{
				tmp = cache[i];
				cache[i] = cache[j];
				cache[j] = tmp;
			}
		}
	}
	for(j = 0; j < i; j++){
		printf("%d\t", cache[j]);
		sprintf(buf, "%d", cache[j]);
		write(cfd, buf, sizeof(buf));
	}
	printf("\n");
	close(cfd);
	close(fd);
	exit(0);
}
