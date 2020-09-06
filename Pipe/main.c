/**
 * Name: Shashank ***
 * Branch: TY B.Tech
 * Roll No: 015
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // Initialize a char array for the string
    char str[150];

    // Create a pipe for transferring the string.
    int spipe[2];

    // Return with exit code 1 if the pipe fails to initialize.
    if (pipe(spipe) == -1) {
        printf("Initialization of Parent Pipe failed.");
        return 1;
    }

    // Take input of the string from the user.
    printf("Enter the string: ");
    scanf("%[^\n]", str);

    // Fork the process and obtain the PID of the process.
    pid_t pid = fork();

    // The pid will be -1 if the fork was unsuccessful, in which case, return with exit code 1
    if (pid < 0) {
        printf("Forking failed");
        return 1;

    } else if (pid > 0) {
        // if the pid > 0 then the parent process is executing.
        // Initialize a char array for the result.
        char res[150];

        // Send the input string to the child process
        write(spipe[1], str, strlen(str));

        // Wait for the child process to finish
        wait(NULL);

        // Read the result string from the pipe and print it out.
        read(spipe[0], res, 150);
        printf("%s\n", res);

    } else {
        // If the pid is 0 then its a child process.
        // Initialize a char array to receive the input string.
        char toMutate[150];

        // Read the string from the pipe.
        read(spipe[0], toMutate, 150);

        // Algorithm for reversing the case of each letter.
        // We first skip any spaces. Next, we check the ascii code
        // of the character and replace it with the ascii code of the
        // corresponding character with reversed case.
        for (int i = 0; i < strlen(toMutate); i++) {
            if (toMutate[i] == ' ') continue;

            int code = (int)toMutate[i];

            if (code <= 90) {
                toMutate[i] = (char)(code + 32);
                continue;
            }

            toMutate[i] = (char)(code - 32);
        }

        // Send the result string back to the parent process.
        write(spipe[1], toMutate, strlen(toMutate));

        return 0;
    }

}
