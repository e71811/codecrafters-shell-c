#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <io.h>
    #define F_OK 0
    #define X_OK 0
#else
    #include <unistd.h>
#endif

int main(int argc, char *argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);

    char *builtins[] = {"exit", "echo", "type", NULL};

    while (1) {
        printf("$ ");

        char buffer[1000];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;

        // הסרת תו הניו-ליין מהסוף
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strlen(buffer) == 0) continue;

        // יצירת עותק לעבודה עם strtok במידת הצורך
        char userInput[1000];
        strcpy(userInput, buffer);

        // --- פקודת exit ---
        if (strcmp(userInput, "exit 0") == 0 || strcmp(userInput, "exit") == 0) {
            return 0;
        }

        // --- פקודת echo ---
        else if (strncmp(userInput, "echo ", 5) == 0) {
            printf("%s\n", userInput + 5);
        }

        // --- פקודת type ---
        else if (strncmp(userInput, "type ", 5) == 0) {
            char *cmd = userInput + 5;
            int flag = 0;

            // בדיקה אם זו פקודה מובנית (Builtin)
            for (int i = 0; builtins[i] != NULL; i++) {
                if (strcmp(cmd, builtins[i]) == 0) {
                    printf("%s is a shell builtin\n", builtins[i]);
                    flag = 1;
                    break;
                }
            }

            // בדיקה ב-PATH במידה וזו לא פקודה מובנית
            if (flag == 0) {
                char *path = getenv("PATH");
                if (path) {
                    char *cpath = strdup(path);
                    char *directory = strtok(cpath, ":");
                    while (directory != NULL) {
                        char currentPath[1024];
                        snprintf(currentPath, sizeof(currentPath), "%s/%s", directory, cmd);

                        // בדיקה אם הקובץ קיים ויש לו הרשאות הרצה
                        if (access(currentPath, X_OK) == 0) {
                            printf("%s is %s\n", cmd, currentPath);
                            flag = 1;
                            break;
                        }
                        directory = strtok(NULL, ":");
                    }
                    free(cpath);
                }
            }

            if (flag == 0) {
                printf("%s: not found\n", cmd);
            }
        }


    }
    return 0;
}