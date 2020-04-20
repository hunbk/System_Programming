#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termio.h>
#define MAX 24
#define START_ID 1401001

struct student {
	char name[MAX];
	int id;
	int score;
};
int getch();
void dbcreate();
void dbquery();
void dbupdate();
void dbinsert();
void dbdelete();

int main(int argc, char *argv[]) {

	int select = 0;
	/*
	   if(argc < 2) {
	   fprintf(stderr, "사용법 : %s file\n", argv[0]);
	   exit(1);
	   }
	 */

	while(1) {
		system("clear");
		printf("메뉴\n");
		printf("(1) DB FILE 생성\n");
		printf("(2) 레코드 검색\n");
		printf("(3) 레코드 수정\n");
		printf("(4) 레코드 삽입\n");
		printf("(5) 레코드 삭제\n");
		printf("(0) 종료\n");
		printf("입력하시오>");
		select = getch();
		switch((select - 48)) {
			case 1:
				dbcreate();
				break;
			case 2:
				dbquery();
				break;
			case 3:
				dbupdate();
				break;
			case 4:
				dbinsert();
				break;
			case 5:
				dbdelete();
				break;
			case 0:
				exit(0);
			default:
				break;
		}
	}
}

/* 데이터베이스 파일을 생성하고 학생 정보를 입력받아 저장한다. */
void dbcreate() {

	int fd;
	struct student rec;
	char *argv = malloc(sizeof(char) * MAX);

	system("clear");
	printf("생성할 DB 파일이름을 입력:");
	scanf("%s", argv);
	getchar();

	if ((fd = open(argv, O_WRONLY | O_CREAT, 0640))==-1) {
		perror(argv);
		exit(2);
	}

	system("clear");
	printf("%-9s %-8s %-4s\n", "학번",  "이름",  "점수"); 
	while(scanf("%d %s %d", &rec.id, rec.name, &rec.score) == 3) {
		getchar();
		lseek(fd, (rec.id - START_ID) * sizeof(rec), SEEK_SET);
		write(fd, &rec, sizeof(rec) );
	}

	close(fd);
}

/* 학번을 입력받아 해당 학생의 레코드를 파일에서 읽어 출력한다. */
void dbquery() {
	int fd, id;
	char c;
	struct student rec;
	char *argv = malloc(sizeof(char) * MAX);

	while(1) {
		system("clear");
		printf("레코드를 검색할 DB 파일이름을 입력:");
		scanf("%s", argv);
		getchar();

		if ((fd = open(argv, O_RDONLY)) == -1) {
			//perror(argv);
			//exit(2);
			printf("해당하는 파일이 없습니다!\n");
			printf("다시 입력하시겠습니까?(Y/N):");
			c = getch();
			if(c != 'Y' & c != 'y') {
				return;
			}
		}
		else {
			break;
		}
	}

	do {
		system("clear");
		printf("검색할 학생의 학번 입력:");
		if (scanf("%d", &id) == 1) {
			getchar();
			lseek(fd, (id-START_ID)*sizeof(rec), SEEK_SET);
			if ((read(fd, &rec, sizeof(rec)) > 0) && (rec.id != 0)) {
				printf("학번:%d\t 이름:%s\t 점수:%d\n", rec.id, rec.name, rec.score);
			}
			else {
				printf("레코드 %d 없음\n", id);
			}
		}
		else {
			getchar();
			printf("입력 오류\n");
		}

		printf("계속하겠습니까?(Y/N)");
		c = getch();
	} while (c == 'Y' | c == 'y');

	close(fd);
}

/* 학번을 입력받아 해당 학생 레코드를 수정한다. */
void dbupdate()
{
	int fd, id;
	char c;
	struct student rec;
	char *argv = malloc(sizeof(char) * MAX);

	while(1) {
		system("clear");
		printf("레코드를 수정할 DB 파일이름을 입력:");
		scanf("%s", argv);
		getchar();

		if ((fd = open(argv, O_RDWR)) == -1) {
			//perror(argv);
			//exit(2);
			printf("해당하는 파일이 없습니다!\n");
			printf("다시 입력하시겠습니까?(Y/N):");
			c = getch();
			if(c != 'Y' & c != 'y') {
				return;
			}
		}
		else {
			break;
		}
	}

	do {
		system("clear");
		printf("수정할 학생의 학번 입력: ");
		if (scanf("%d", &id) == 1) {
			getchar();
			lseek(fd,  (long) (id-START_ID)*sizeof(rec), SEEK_SET);
			if ((read(fd, &rec, sizeof(rec)) > 0) && (rec.id != 0)) {
				printf("학번:%8d\t 이름:%4s\t 점수:%4d\n", rec.id, rec.name, rec.score);
				printf("\n새로운 정보를 입력하시오\n");
				printf("%-4s %-4s\n", "이름", "점수");
				scanf("%s %d", rec.name, &rec.score);
				getchar();
				lseek(fd, (long) -sizeof(rec), SEEK_CUR);
				write(fd, &rec, sizeof(rec));
			}
			else {
				printf("레코드 %d 없음\n", id);
			}
		}
		else {
			getchar();
			printf("입력오류\n");
		}
		printf("계속하겠습니까?(Y/N)");
		c = getch();
	} while (c == 'Y' | c == 'y');

	close(fd);
}

/* 학생정보를 입력받아 기존의 데이터베이스 파일에 삽입 후 저장  */
void dbinsert() {
	int fd;
	char c;
	struct student rec;
	char *argv = malloc(sizeof(char) * MAX);

	while(1) {
		system("clear");
		printf("레코드를 삽입할 DB 파일이름을 입력:");
		scanf("%s", argv);
		getchar();

		if ((fd = open(argv, O_WRONLY)) == -1) {
			//perror(argv);
			//exit(2);
			printf("해당하는 파일이 없습니다!\n");
			printf("다시 입력하시겠습니까?(Y/N):");
			c = getch();
			if(c != 'Y' & c != 'y') {
				return;
			}
		}
		else {
			break;
		}
	}

	do {
		system("clear");
		printf("주의!!) 학번은 %d 부터 시작\n", START_ID);
		printf("%-9s %-8s %-5s\n", "학번", "이름", "점수");

		if(scanf("%d %s %d", &rec.id, rec.name, &rec.score) == 3) {
			getchar();
			lseek(fd, (long)(rec.id - START_ID) * sizeof(rec) , SEEK_SET);
			write(fd, &rec, sizeof(rec));
			printf("삽입 완료\n");
		}
		else {
			getchar();
			printf("입력오류\n");
		}
		printf("계속하시겠습니까?(Y/N)");
		c = getch();
		if(c != 'Y' & c != 'y') {
			return;
		}
	} while(c == 'Y' | c == 'y');

	close(fd);
}

/* 학번을 입력받은 후 해당학생의 레코드를 삭제 */
void dbdelete() {
	int fd, id;
	char c;
	struct student rec;
	char *argv = malloc(sizeof(char) * MAX);

	while(1) {
		system("clear");
		printf("레코드를 삭제할 DB 파일이름을 입력:");
		scanf("%s", argv);
		getchar();

		if ((fd = open(argv, O_RDWR)) == -1) {
			//perror(argv);
			//exit(2);
			printf("해당하는 파일이 없습니다!\n");
			printf("다시 입력하시겠습니까?(Y/N):");
			c = getch();
			if(c != 'Y' & c != 'y') {
				return;
			}
		}
		else {
			break;
		}
	}


	do {
		system("clear");
		printf("삭제할 학생의 학번 입력: ");
		if (scanf("%d", &id) == 1) {
			getchar();
			lseek(fd,  (long) (id-START_ID)*sizeof(rec), SEEK_SET);
			if ((read(fd, &rec, sizeof(rec)) > 0) && (rec.id != 0)) {
				printf("학번:%8d\t 이름:%4s\t 점수:%4d\n", rec.id, rec.name, rec.score);
				printf("\n경고!!) 삭제한 데이터는 복구할 수 없습니다.\n");
				printf("정말 삭제하시겠습니까?(Y/N)");
				c = getch();
				if(c == 'Y' | c == 'y') {
					lseek(fd, (long) -sizeof(rec), SEEK_CUR);
					struct student rec = {0};
					write(fd, &rec, sizeof(rec));
					printf("\n삭제완료\n");
				}
				else {
					return;
				}
			}
			else {
				printf("레코드 %d 없음\n", id);
			}
		}
		else {
			getchar();
			printf("입력오류\n");
		}
		printf("계속하겠습니까?(Y/N)");
		c = getch();
	} while (c == 'Y' | c == 'y');

	close(fd);
}

/* 키보드에서 1개의 문자를 바로 입력받는 함수 */
int getch(void) {
	int ch;

	struct termios buf;
	struct termios save;

	tcgetattr(0, &save);
	buf = save;

	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;

	tcsetattr(0, TCSAFLUSH, &buf);

	ch = getchar();

	tcsetattr(0, TCSAFLUSH, &save);

	return ch;
}
