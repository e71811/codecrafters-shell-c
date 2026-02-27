#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


char* find_in_path(char *command) {
    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    static char full_path[1024];

    while (dir != NULL) {
        sprintf(full_path, "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}

int main() {
    while (1) {
        printf("$ ");
        fflush(stdout);
        char input[1024];
        fgets(input, 1024, stdin);
        input[strcspn(input, "\n")] = 0;

        char *args[64];
        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL) continue;

        if (strcmp(args[0], "exit") == 0) {
            return 0;
        } else if (strcmp(args[0], "pwd") == 0) {
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
        } else {

            char *full_path = find_in_path(args[0]);
            if (full_path) {
                pid_t pid = fork();
                if (pid == 0) {
                    execv(full_path, args);
                    exit(1);
                } else {
                    wait(NULL);
                }
            } else {
                printf("%s: command not found\n", args[0]);
            }
        }
    }
}