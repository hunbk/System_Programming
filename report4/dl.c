/*************************************************************************
* dl : ls 명령어 구현하기
* 작성자 : 김병훈
* 작성일 2020.05.11
* 참조
joinc.co.kr/w/man/3/getopt
m.blog.naver.com/beyondlegend/110029848992
it-note.kr/173
dojang.io/mod/page/view.php?id=359
*************************************************************************/

#include <sys/types.h> 
#include <sys/stat.h> 
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <time.h>

char type(mode_t);
char *perm(mode_t);
void printStat(char*, char*, struct stat*);

/* 디렉터리 내의 파일 이름들을 리스트한다. */
int main(int argc, char **argv) {
	DIR *dp;
	char *dir;
	struct dirent *d;
	struct stat st;
	char path[BUFSIZ+1];
	char *isdir = "/";
	int opt;
	int a_flag = 0, l_flag = 0, i_flag = 0, p_flag = 0, flag_1 = 0, m_flag = 0;

	// 사용할 디렉터리 추출
	dir = ".";		// 현재 디렉터리를 default로 set
	if(argc > 1) {
		for(int i=1; i<argc; i++) {
			if(argv[i][0] != '-') {		// -로 시작안하는 것 == 옵션이 아닌 것
				dir = argv[i];
			}
		}
	}

	// 추출한 디렉터리 열기
	if((dp = opendir(dir)) == NULL) {
		perror(dir);
		exit(0);
	}

	// 옵션 처리
	while((opt = getopt(argc, argv, "alip1")) != -1) {
		switch(opt) {
			case 'a':	// 히든 파일 포함
				a_flag = 1;
				break;
			case 'l':	// 자세한 내용 출력
				l_flag = 1;
				break;
			case 'i':	// inode 번호 출력
				i_flag = 1;
				break;
			case 'p':	// 디렉터리 구분 문자(/) 출력
				p_flag = 1;
				break;
			case '1':	// 한줄씩 출력
				flag_1 = 1;
				break;
			case '?':	// 정의되지 않은 옵션
				printf("사용법: dl -alip1 사용\n");
				exit(0);
		}
	}

	// 디렉터리 내의 엔트리 가져오기
	while((d = readdir(dp)) != NULL) {
		// 파일 상태 정보 가져오기
		sprintf(path, "%s/%s", dir, d->d_name);
		if(lstat(path, &st) < 0) {
			perror(path);
			exit(1);
		}
		
		// 옵션에 따라 출력하기	
		// 옵션 l이 없을 때
		if(l_flag == 0) {
			if(a_flag == 0) {	// 옵션 a가 없을 때
				if(d->d_name[0] == '.') {
					continue;	// .으로 시작하는 히든파일 무시
				}
			}
			if(flag_1 == 1)	puts("");	// 옵션 1이 있을 때
			if(i_flag == 1) {	// 옵션 i가 있을 때
				printf("%17ld ", st.st_ino);
			}
			if(p_flag == 1 & S_ISDIR(st.st_mode)) {	// 옵션 p가 있을 때
				printf("%s/  ", d->d_name);
				continue;
			}
			printf("%s  ", d->d_name);
		}
		
		// 옵션 l이 있을 때
		if(l_flag == 1) {
			if(a_flag == 0) {	// 옵션 a가 없을 때
				if(d->d_name[0] == '.') {
					continue;	// .으로 시작하는 히든파일 무시
				}
			}
			if(i_flag == 1) {	// 옵션 i가 있을 때
				printf("%17ld ", st.st_ino);
			}
			if(p_flag == 1 & S_ISDIR(st.st_mode)) {	// 옵션 p가 있을 때
				printStat(path, strcat(d->d_name, isdir), &st);	// strcat 함수로 d_name 뒤에 isdir 붙임
				continue;
			}
			printStat(path, d->d_name, &st);
		}
	}

	if(l_flag == 0)	puts("");

	closedir(dp);
	exit(0);
}

/* 파일 상태 정보를 출력 */
void printStat(char *pathname, char *file, struct stat *st) {

	printf("%c%s ", type(st->st_mode), perm(st->st_mode));
	printf("%2ld ", st->st_nlink);
	printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);
	printf("%7ld ", st->st_size);
	printf("%.12s ", ctime(&st->st_mtime)+4);
	printf("%s\n", file);
}

/* 파일 타입을 리턴 */
char type(mode_t mode) {

	if (S_ISREG(mode)) 
		return('-');
	if (S_ISDIR(mode)) 
		return('d');
	if (S_ISCHR(mode)) 
		return('c');
	if (S_ISBLK(mode)) 
		return('b');
	if (S_ISLNK(mode)) 
		return('l');
	if (S_ISFIFO(mode)) 
		return('p');
	if (S_ISSOCK(mode)) 
		return('s');
}

/* 파일 허가권을 리턴 */
char* perm(mode_t mode) {
	int i;
	static char perms[10]; 

	strcpy(perms, "---------");

	for (i=0; i < 3; i++) {
		if (mode & (S_IREAD >> i*3)) 
			perms[i*3] = 'r';
		if (mode & (S_IWRITE >> i*3)) 
			perms[i*3+1] = 'w';
		if (mode & (S_IEXEC >> i*3)) 
			perms[i*3+2] = 'x';
	}
	return(perms);
}
