#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main() {
	char str[150];

	int pPipe[2];
	int cPipe[2];

	if (pipe(pPipe) == -1) {
		printf("Initialization of Parent Pipe failed.");
		return 1;
	}

	if (pipe(cPipe) == -1) {
		printf("Initialization of Child Pipe failed");
		return 1;
	}

	printf("Enter the string: ");
	scanf("%[^\n]", str);

	pid_t pid = fork();

	if (pid < 0) {
		printf("Forking failed");
		return 1;

	} else if (pid > 0) {
		char res[150];

		write(cPipe[1], str, strlen(str));

		wait(NULL);

		read(pPipe[0], res, 150);
		printf("%s\n", res);

	} else {
		char toMutate[150];
		read(cPipe[0], toMutate, 150);

		for (int i = 0; i < strlen(toMutate); i++) {
			if (toMutate[i] == ' ') continue;

			int code = (int)toMutate[i];

			if (code <= 90) {
				toMutate[i] = (char)(code + 32);
				continue;
			}

			toMutate[i] = (char)(code - 32);
		}

		write(pPipe[1], toMutate, strlen(toMutate));

		return 0;
	}

}
