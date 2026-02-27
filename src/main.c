#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    while (1) {
        printf("$ ");
        fflush(stdout);
        char input[1024];
        fgets(input, 1024, stdin);
        input[strcspn(input, "\n")] = 0;


        char *command = strtok(input, " ");
        char *arg = strtok(NULL, "");

        if (command == NULL) continue;

        if (strcmp(command, "exit") == 0) {
            return 0;
        } else if (strcmp(command, "echo") == 0) {
            if (arg) printf("%s\n", arg);
            else printf("\n");
        } else if (strcmp(command, "pwd") == 0) {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            }
        } else if (strcmp(command, "type") == 0) {
            if (arg && (strcmp(arg, "echo") == 0 || strcmp(arg, "exit") == 0 || strcmp(arg, "pwd") == 0)) {
                printf("%s is a shell builtin\n", arg);
            } else {
                printf("%s not found\n", arg ? arg : "");
            }
        } else {
            printf("%s: command not found\n", command);
        }
    }
}