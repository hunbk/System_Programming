#include <stdio.h>
#include "longest.h"

char line[MAXLINE];
char longest[MAXLINE];

int main() {
	int len;
	int max = 0;
	while (mygets(line, sizeof(line)) != NULL) {
		len = strlen(line);
		if (len > max) {
			max = len;
			copy(line, longest);
																	}
	}
	if (max > 0) { // 입력 줄이 있었다면
		printf("%s", longest);
	}
	return 0;
}
