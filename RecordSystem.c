#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "student.h"
#define true 1
#define false 0

/*
=======================================================

   	자주 사용되는 변수 설명

 	int cfd --- 파일_cache에 생성 혹은 접근하는 변수
	int *cache --- 학생 레코드에서 학번만 임시 저장하는 배열
 	int tmp --- 이름처럼 임시 저장소로 활용되는 변수
 	int flag --- 중단점 혹은 반복문의 수행에서 활용되는 변수
 	int i, j, k --- for문에서 활용되는 변수

=======================================================
*/



//=======================================================
//  레코드를 정렬해주는 함수
//=======================================================
void sortd_record(int *arr, int num){
	int i, j, tmp;
	for(i = 0; i < num; i++){
		for(j = i; j < num; j++){
			if(arr[i] > arr[j]){
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

//=======================================================
//  기존의 레코드에 존재했었는지 확인하는 함수
//=======================================================
int check_data(int *arr, int num, int flag){
	int i, j = 0;
	for(i = 0; i < flag; i++){
		if( arr[i] == num )
			return 1;
	}
	return 0;
}

//=======================================================
//  레코드 생성 함수
//=======================================================
void create_record(int fd, struct student record, int argc, char *argv[]){
	int id;
	int cfd;
	int cache[100];
	int i = 0;

	//=======================================================================================
	// 파일이 존재하면 에러를 표시하고 없을 때 생서

	if ((fd = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0661)) == -1){
		perror(argv[2]);
		exit(2);
	}
	if((cfd = open(strcat(argv[2], "_cache"), O_WRONLY | O_CREAT | O_EXCL, 0661)) == -1){
		perror(strcat(argv[2], "_cache"));
		exit(2);
	}
	//=======================================================================================


	//=======================================================================================
	// 레코드를 id 에 맞게 순서대로 저장하고 학번 데이터만 cache 배열에 집어 넣는다.
	// -1 입력시 입력 종료
	printf("%-9s %-8s %-4s\n", "학번", "이름", "장학금");
	while (true){
		scanf("%d", &id);
	    if(id == -1)
			break;
		else{
			record.id = id;
			cache[i] = id;
			i = i + 1;
			scanf("%s %d", record.name, &record.sclsp);
			record.de = false;
	    	lseek(fd, (record.id - START_ID) * sizeof(record), SEEK_SET);
			write(fd, (char *) &record, sizeof(record) );
			fflush(stdin);
		}
	}
	//=======================================================================================

	// 정렬해주고 순서에 맞게 *_cache안에 저장
	sortd_record(cache, i);
	write(cfd, (int *)&i, sizeof(int));
	write(cfd, cache, sizeof(cache));
	close(cfd);
	close(fd);
}

//=======================================================
// 기존의 레코드에서 데이터를 추가하는  함수
//=======================================================
void insert_data(int fd, struct student record, char *argv[]){
	int id;
	int cfd;
	int cache[100];
	int i, j = 0;

	//=======================================================================================
	// 파일을 여는데 없으면 에러 표시
	if ((fd = open(argv[2], O_RDWR)) == -1){
		perror(argv[2]);
		exit(2);
	}
	if((cfd = open(strcat(argv[2], "_cache"), O_RDWR)) == -1){
		perror(strcat(argv[2], "_cache"));
		exit(2);
	}
	//=======================================================================================

	// 잠금
	if (lockf(fd, F_LOCK, sizeof(record)) == -1){
		perror(argv[2]);
		exit(3);
	}

	// cfd 에서 id 만 불러와 빠른 탐색을 위
	// cache에 넣어주다.
	lseek(cfd,0, SEEK_SET);
	read(cfd, (int *)&i, sizeof(int));
	int tmp = 0;
	for(j = 0; j < i; j++){
		read(cfd, (int *)&tmp, sizeof(int));
		cache[j] = tmp;
	}


	printf("%-9s %-8s %-4s\n", "학번", "이름", "장학금");
	while (true){
		scanf("%d", &id);
	    if(id == -1)
			break;
		else if (check_data(cache, id, i)) {
			//=======================================================================================
			// create_record 와 거의 동일하나 차이점은 insert시 기존에 이미 있었던 데이터도 있을 수 있기에
			// 중복 삽입으로 인한 데이터 낭비를 줄이고자 확인한 뒤 존재하면 삭제마크(de)를 바꿔주어 살린 뒤 수정이 이뤄지게 한다.
			lseek(fd, (id - START_ID)* sizeof(record), SEEK_SET);
			read(fd, (char*)&record, sizeof(record));
			record.de = false;
			scanf("%s %d", record.name, &record.sclsp);
			lseek(fd,(id - START_ID)*sizeof(record), SEEK_SET);
			write(fd, (char *)&record, sizeof(record));
			//=======================================================================================
		}
		else{ // 이하는 create_record 와 동일
			record.id = id;
			cache[i] = id;
			i = i + 1;
			scanf("%s %d", record.name, &record.sclsp);
			record.de = false;
	    	lseek(fd, (record.id - START_ID) * sizeof(record), SEEK_SET);
			write(fd, (char *) &record, sizeof(record) );
			fflush(stdin);
		}
	}
	sortd_record(cache, i);

	lseek(cfd, 0, SEEK_SET);
	write(cfd, (int *)&i, sizeof(int));
	write(cfd, cache, sizeof(cache));

	// 잠금 해제
	lseek(fd, (long) (id-START_ID)*sizeof(record), SEEK_SET);
	lockf(fd, F_ULOCK, sizeof(record));

	close(cfd);
	close(fd);
}

//=======================================================
//  목록을 출력하는 함수
//=======================================================
void show_record(int fd, struct student record, char *argv[]){
	int id;
	int cfd;
	int tmp = 0;

	//=======================================================================================
	// 위 와 동일하게 파일을 여는데 없으면 에러표시
	// ( 근데 말은 같다하지만 정확히는 안에 옵션이 달라 각 목적에 맞게 파일을 열고 닫음 )
	if ((fd = open(argv[2], O_RDONLY)) == -1){
		perror(argv[2]);
		exit(2);
	}
	if((cfd = open(strcat(argv[2], "_cache"), O_RDONLY)) == -1){
		perror(strcat(argv[2], "_cache"));
		exit(2);
	}
	//=======================================================================================

	// cfd를 통해 학생 id 만 빠르게 받아와서
	lseek(cfd,0,SEEK_SET);
	read(cfd,(int *)&tmp, sizeof(int));
	int i = 0;
	for(i = 0; i < tmp; i++){
		read(cfd, (int *)&id, sizeof(int));
		lseek(fd, (id - START_ID)*sizeof(record), SEEK_SET);
		read(fd, (char *)&record, sizeof(record));
		// 삭제마크 유무를 화인 후 출력
		if( record.de == false)
			printf("이름:%s\t 학번:%d\t 장학금:%d\n", record.name, record.id, record.sclsp);
	}

	close(cfd);
	close(fd);
}

//=======================================================
//   레코드를 수정하는 함수
//=======================================================
void edit_record(int fd, struct student record, char *argv[]){
	int id;
	if ((fd = open(argv[2], O_RDWR)) == -1){
		perror(argv[2]);
		exit(2);
	}


	printf("수정할 학생의 학번 입력: ");
	if (scanf("%d", &id) == 1){
		lseek(fd, (long) (id-START_ID)*sizeof(record), SEEK_SET);
		// 수정은 그냥 바로 튕기게 TLOCK 으로 해 봄
		if (lockf(fd, F_TLOCK, sizeof(record)) == -1){
			perror(argv[2]);
			exit(3);
		}
		if ((read(fd, (char *)&record, sizeof(record)) > 0 ) && (record.id != 0)) {
			printf("학번:%8d\t 이름:%4s\t 장학금:%4d\n", record.id, record.name, record.sclsp);
			printf("새로운 장학금: ");
			scanf("%d", &record.sclsp);
			lseek(fd, (long) -sizeof(record), SEEK_CUR);
			write(fd, (char*) &record, sizeof(record));

			// 잠금 해제
			lseek(fd, (long) (id-START_ID)*sizeof(record), SEEK_SET);
			lockf(fd, F_ULOCK, sizeof(record));
		}
		else
			printf("레코드 %d 없음\n", id);
	}
	else
		printf("입력오류\n");

	close(fd);
}

//=======================================================
//  삭제 함수
//=======================================================
void del_record(int fd, struct student record, char *argv[]){
	int id;
	if ((fd = open(argv[2], O_RDWR)) == -1){
		perror(argv[1]);
		exit(2);
	}
	printf("삭제할 학생의 학번 입력: ");
	if (scanf("%d", &id) == 1){

		// 잠금 설정
		lseek(fd, (long) (id-START_ID)*sizeof(record), SEEK_SET);
		if (lockf(fd, F_LOCK, sizeof(record)) == -1){
			perror(argv[2]);
			exit(3);
		}

		//=======================================================================================
		// 실질적인 삭제가 아닌 de 를 수정하여 레코드에 표시만 안되게끔 한다.
		if ((read(fd, (char *)&record, sizeof(record)) > 0 ) && (record.id != 0)) {
			printf("학번:%8d\t 이름:%4s\t 장학금:%4d\n", record.id, record.name, record.sclsp);
			record.de = true;
			lseek(fd, (long) -sizeof(record), SEEK_CUR);
			write(fd, (char*) &record, sizeof(record));


		}
		else
			printf("레코드 %d 없음\n", id);
		//=======================================================================================


		// 잠금 해제
		lseek(fd, (long) (id-START_ID)*sizeof(record), SEEK_SET);
		lockf(fd, F_ULOCK, sizeof(record));
	}
	else
		printf("입력오류\n");

	close(fd);
}


//=======================================================
//   메인 시작!
//=======================================================
int main(int argc, char *argv[]){
int fd;
	char c;
	struct student record;

	if (argc == 2 && !strcmp(argv[1], "help")){
		printf("01. Make Record : %s mk file\n02. Show Record : %s show file\n", argv[0], argv[0]);
		printf("03. Edit Record : %s edit file\n04. Delete Data : %s rm file\n", argv[0], argv[0]);
		printf("05. Add Data : %s add file\n", argv[0]);
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
	else if(argc == 3 && !strcmp(argv[1], "add")){
		insert_data(fd, record, argv);
	}
	else{
		fprintf(stderr, "User Guide : %s help\n", argv[0]);
		exit(1);
	}
  close(fd);
  exit(0);
}
