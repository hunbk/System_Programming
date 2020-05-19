#include <stdio.h>
 
int main(void) {
    char c1;
	char c2;
     
    printf("대문자 입력 : ");
    scanf("%c", &c1);
    printf("소문자 입력 : ");
	scanf(" %c", &c2);

	printf("\n입력한 %c의 소문자는 %c입니다.\n", c1, (c1+32));
	printf("입력한 %c의 대문자는 %c입니다.\n", c2, (c2-32));

    return 0;
}
