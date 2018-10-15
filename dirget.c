#include <sys/types.h> 
#include <sys/stat.h> 
#include <dirent.h> 
#include <pwd.h> 
#include <grp.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
 
/*
하단의 main 시작부분에 선언한 stat 구조체는 다음과 같은 구조를 지니고있다.
여기서 맨 아래의 3개 time_t 를 사용하기 위해 위에 time.h 를 포함해준다.

struct stat {
    dev_t     st_dev;     /* ID of device containing file */
    ino_t     st_ino;     /* inode number */
    mode_t    st_mode;    /* protection */
    nlink_t   st_nlink;   /* number of hard links */
    uid_t     st_uid;     /* user ID of owner */
    gid_t     st_gid;     /* group ID of owner */
    dev_t     st_rdev;    /* device ID (if special file) */
    off_t     st_size;    /* total size, in bytes */
    blksize_t st_blksize; /* blocksize for file system I/O */
    blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
    time_t    st_atime;   /* time of last access */
    time_t    st_mtime;   /* time of last modification */
    time_t    st_ctime;   /* time of last status change */
};

참고(이후 참고는 최하단에 링크만 기입) : https://linux.die.net/man/2/stat

i노드 블럭
struct dirent{
  long d_ino; //inode 번호
  off_t d_off; // offset
  unsigned short d_reclen; //d_name 길이
  char d_name[NAME_MAX+1]; //파일 이름
};

http://ehpub.co.kr/tag/dirent-%EA%B5%AC%EC%A1%B0%EC%B2%B4/
*/

char type(mode_t);
char *perm(mode_t);
void printStat(char*, char*, struct stat*);
/* 디렉터리 내용을 자세히 리스트한다. */
int main(int argc, char **argv)
{
    DIR *dp;
    char *dir;
    struct stat st; 
    struct dirent *d;
    char path[BUFSIZ + 1];
 
    if (argc == 1)
        dir = ".";
    else dir = argv[1];
    if ((dp = opendir(dir)) == NULL) // 디렉터리 열기 
        perror(dir);
    while ((d = readdir(dp)) != NULL) { // 디렉터리의 각 파일에 대해 
        sprintf(path, "%s/%s", dir, d->d_name); // 파일경로명 만들기 
        if (lstat(path, &st) < 0) // 파일 상태 정보 가져오기 
            perror(path);
        printStat(path, d->d_name, &st); // 상태 정보 출력 
        putchar('\n');
    }
    closedir(dp);
    exit(0);
}

/* 파일 상태 정보를 출력 */
/*
 기존의 코드인
 printf("%5d ", st->st_blocks);
 를 사용하게 되면, %5d 에서 오류가 난다. 원인은 st->st_blocks가 일정한 크기를 가지지않은 변수(가변크기를 가진 변수)이다. 그래서 정적 변수인
 int를 나타내는 d를 써서 오류가 발생함. 아래도 마찮가지의 이유
 고로 -- zu 라는 가변 변수를 출력하게 해주는 포맷인 z, zu를 써주면 이제 무리없이 코드가 돌아감
 
 https://stackoverflow.com/questions/21128092/d-expects-argument-of-type-int-but-argument-2-has-type-long-unsigned-int
*/
void printStat(char *pathname, char *file, struct stat *st) {
    printf("%5zu ", st->st_blocks);
    printf("%c%s ", type(st->st_mode), perm(st->st_mode));
    printf("%3zu ", st->st_nlink);
    printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);
    printf("%9zu ", st->st_size);
    printf("%.12s ", ctime(&st->st_mtime) + 4);
    printf("%s", file);
}
/* 파일 타입을 리턴 */
char type(mode_t mode) {
    if (S_ISREG(mode)) // 이것은 정규 파일인가?
        return('-');
    if (S_ISDIR(mode)) // 디렉토리인가?
        return('d');
    if (S_ISCHR(mode)) // 캐릭터 디바이스인가? 
        return('c');   // ( character device = 데이터를 문자 단위로 전달 및 읽는 디바이스 ex) 터미널, 키보드, 스캐너)
    if (S_ISBLK(mode)) // 블록 디바이스인가?
        return('b');   // Block device = 블록 단위로 데이터 전달 및 읽음
    if (S_ISLNK(mode)) // symbolic link인가?
        return('l');   // =디렉토리 참조 링크
    if (S_ISFIFO(mode)) // FIFO 인가?
        return('p');
    if (S_ISSOCK(mode)) // 소켓인가?(네트워크 관련)
        return('s');
}// https://linux.die.net/man/2/stat
/* 파일 사용권한을 리턴 */


/*
S_IRUSR = 사용자 읽기/ 사용자가 파일을 읽을 수 있음
S_IWUSR = 사용자 쓰기/ 사용자가 파일을 기록할 수 있음
S_IXUSR = 사용자 실행/ 사용자가 파일을 실행할 수 있음

하단의 함수는 해당 파일의 권한을 리턴하는 함수(권한-> ex) drwxr-xr-x, -rwxr-xr-x)

http://devanix.tistory.com/289
*/
char* perm(mode_t mode) {
    int i;
    static char perms[10] = "---------";
    for (i = 0; i < 3; i++) {
        if (mode & (S_IREAD >> i * 3)) // S_IREAD = S_IRUSR
            perms[i * 3] = 'r';
        if (mode & (S_IWRITE >> i * 3))// S_IWRITE = S_IWUSR
            perms[i * 3 + 1] = 'w';
        if (mode & (S_IEXEC >> i * 3)) // S_IEXEC = S_IXUSR
            perms[i * 3 + 2] = 'x';
    }
    return(perms);
}
