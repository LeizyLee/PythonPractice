#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define true 1
#define false 0

typedef struct{
    int id;
    char name[10];
    int sclsp;
    char de;
}Record;

Record rec;
int idArr[20];

void create(FILE *fp, char *argv[]);
void show(FILE *fp, char *argv[]);
void edit(FILE *fp, char *argv[]);
void delete(FILE *fp, char *argv[]);
void add(FILE *fp, char *argv[]);
void search(FILE *fp, char *argv[]);


int main(int argc, char *argv[]){
    FILE *fp;
    int c;

    if (argc != 2){
        fprintf(stderr, "사용법: %s help\n", argv[0]);
        exit(1);
    }
    else if(!strcmp(argv[1], "help")){
        printf("%s mk = 생성\n", argv[0]);
        printf("%s show = 목록출력\n", argv[0]);
        printf("%s del = 삭제\n", argv[0]);
        printf("%s add = 추가\n", argv[0]);
        printf("%s search = 검색\n", argv[0]);
    }

    if(!strcmp(argv[1], "mk"))
        create(fp, argv);
    else if(!strcmp(argv[1], "show"))
        show(fp, argv);
    else if(!strcmp(argv[1], "edit"))
        edit(fp, argv);
    else if(!strcmp(argv[1], "del"))
        delete(fp, argv);
    else if(!strcmp(argv[1], "add"))
        add(fp, argv);
    else if(!strcmp(argv[1], "search"))
        search(fp, argv);

    return 0;
}

void create(FILE *fp, char *argv[]){
    fp = fopen(argv[2], "wb");
    int i = 0;
    printf("%-9s %-7s %-4s\n", "학번", "이름", "장학금");
    while(scanf("%d %s %d", &rec.id, rec.name, &rec.sclsp) == 3){
        if(rec.id == -1)
            break;
        fseek(fp, rec.id*sizeof(rec), SEEK_SET);
        rec.de = true;
        fwrite(&rec, sizeof(rec), 1, fp);
        idArr[i] = rec.id;
        i++;
    }
    FILE *cfp;
    cfp = fopen(strcat(argv[2], "_cache"), "wb");
    fwrite(&i, sizeof(int), 1, cfp);
    fwrite(idArr, sizeof(idArr), 1, cfp);
    fclose(fp);
    fclose(cfp);
    exit(0);
}

void show(FILE *fp, char *argv[]){
    FILE *cfp;

    fp = fopen(argv[2], "rb");
    cfp = fopen(strcat(argv[2], "_cache"), "rb");
    int flag = 0;
    fread(&flag, sizeof(int), 1, cfp);
    int tmp;
    int i = 0;

    printf("%-9s %-7s %-4s\n", "학번", "이름", "장학금");
    for(i = 0; i < flag; i++){
        fread(&tmp, sizeof(int), 1, cfp);
        fseek(fp, tmp*sizeof(rec), SEEK_SET);
        fread(&rec, sizeof(rec), 1, fp);
        if(rec.de)
            printf("%-9d %-7s %-4d\n", rec.id, rec.name, rec.sclsp);
    }
    fclose(cfp);
    fclose(fp);
}

void edit(FILE *fp, char *argv[]){
    fp = fopen(argv[2], "rb+");
    printf("수정하고자 하는 학번 입력 :");
    int tid = 0;
    scanf("%d", &tid);
    fseek(fp, tid * sizeof(rec), SEEK_SET);
    fread(&rec, sizeof(rec), 1, fp);
    printf("이름 : %s\n 장학금 : %d\n 수정 금액 ->  ", rec.name, rec.sclsp);
    scanf("%d", &rec.sclsp);
    fseek(fp, tid * sizeof(rec), SEEK_SET);
    fwrite(&rec, sizeof(rec), 1, fp);
    fclose(fp);
}

void delete(FILE *fp, char *argv[]){
    fp = fopen(argv[2], "rb+");
    printf("삭제하고자 하는 학번 입력 :");
    int tid = 0;
    scanf("%d", &tid);
    fseek(fp, tid * sizeof(rec), SEEK_SET);
    fread(&rec, sizeof(rec), 1, fp);
    rec.de = false;
    fseek(fp, tid * sizeof(rec), SEEK_SET);
    fwrite(&rec, sizeof(rec), 1, fp);
    fclose(fp);
}

void add(FILE *fp, char *argv[]){
    fp = fopen(argv[2], "rb+");
    FILE *cfp;
    cfp = fopen(strcat(argv[2],"_cache"), "rb+");
    int tarr[20];
    int flag = 0;
    int i = 0;
    fread(&flag, sizeof(int), 1, cfp);
    for(i = 0; i < flag; i++){
        fread(&tarr[i], sizeof(int), 1, cfp);
    }
    printf("학번  이름   장학금\n");
    while(scanf("%d %s %d", &rec.id, rec.name, &rec.sclsp) == 3){
        if(rec.id == -1)
            break;
        fseek(fp, rec.id*sizeof(rec), SEEK_SET);
        rec.de = true;
        fwrite(&rec, sizeof(rec), 1, fp);
        tarr[flag] = rec.id;
        flag++;
    }
    fseek(cfp, 0, SEEK_SET);
    fwrite(&flag, sizeof(int), 1, cfp);
    fwrite(tarr, sizeof(tarr), 1, cfp);
    fclose(fp);
    fclose(cfp);
    printf("\n");

}

void search(FILE *fp, char *argv[]){
    fp = fopen(argv[2], "rb");
    FILE *cfp;
    cfp = fopen(strcat(argv[2], "_cache"), "rb");
    char tmp[10];
    printf(" 검색하고자 하는 이름 입력 : ");
    scanf("%s", tmp);
    int i = 0;
    int flag = 0;
    int cache;

    fread(&flag, sizeof(int), 1, cfp);

    for(i = 0; i < flag; i++){
        fread(&cache, sizeof(int), 1, cfp);
        fseek(fp, cache*sizeof(rec), SEEK_SET);
        fread(&rec, sizeof(rec), 1, fp);
        if(!strcmp(rec.name, tmp) && rec.de)
            printf(" %d %s %d \n", rec.id, rec.name, rec.sclsp);
    }

}
