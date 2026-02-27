#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    while (1) {
        printf("$ ");
        fflush(stdout);
        char input[1024];
        fgets(input, 1024, stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit 0") == 0) {
            return 0;
        }

        printf("%s: command not found\n", input);
    }
}