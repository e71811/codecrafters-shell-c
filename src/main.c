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
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <sys/stat.h>

// strtok doesnt work well with ' or " so i implemnted better function
int BetterStrTok(char* buffer, char** seperatedWords)
{
    int quoteMode = 0; // 0: none, '\'': single, '\"': double
    int wordsCount = 0;
    char cWord[1000];
    int i = 0, j = 0;

    while (buffer[i] != '\0')
    {
        char c = buffer[i];

        if (quoteMode == 0)
        {
            if (c == '\\')
            {
                // Backslash outside quotes
                i++;
                if (buffer[i] != '\0') cWord[j++] = buffer[i];
            }
            else if (c == '\'')
            {
                quoteMode = '\'';
            }
            else if (c == '\"')
            {
                quoteMode = '\"';
            }
            else if (c == ' ')
            {
                if (j > 0)
                {
                    cWord[j] = '\0';
                    seperatedWords[wordsCount++] = strdup(cWord);
                    j = 0;
                }
            }
            else
            {
                cWord[j++] = c;
            }
        }
        else if (quoteMode == '\'')
        {
            if (c == '\'') quoteMode = 0;
            else cWord[j++] = c;
        }
        else if (quoteMode == '\"')
        {
            if (c == '\\')
            {
                // Backslash inside double quotes
                char next = buffer[i + 1];
                if (next == '\"' || next == '\\' || next == '$' || next == '`')
                {
                    i++;
                    cWord[j++] = buffer[i];
                }
                else
                {
                    cWord[j++] = c;
                }
            }
            else if (c == '\"')
            {
                quoteMode = 0;
            }
            else
            {
                cWord[j++] = c;
            }
        }
        i++;
    }

    if (j > 0)
    {
        cWord[j] = '\0';
        seperatedWords[wordsCount++] = strdup(cWord);
    }
    seperatedWords[wordsCount] = NULL;
    return wordsCount;
}
void Echo(char** seperatedWords, int numArgs)
{
    for (int i = 1; seperatedWords[i] != NULL; i++)
    {
        printf("%s", seperatedWords[i]);
        // adds " " for all the words except the last word
        if (seperatedWords[i + 1] != NULL)
        {
            printf(" ");
        }
    }
    printf("\n");
}
void Type(char** seperatedWords, int numArgs, char** builtins) {
            if (numArgs < 2)
            {
                //  if the input is only type without anything after i will just stop
                return;
            }

            char* cmd;
            cmd = seperatedWords[1];
            int flag = 0;
            for (int i = 0; builtins[i] != NULL; i++)
            {
                if (strcmp(cmd, builtins[i]) == 0)
                {
                    printf("%s ", builtins[i]);
                    printf("is a shell builtin\n");
                    flag = 1;
                    break;
                }
            }
            if (flag == 0 && cmd != NULL)
            {
                // first i need to get the specific path that i want to check
                char* path = getenv("PATH");
                // create a copy of the path so strtok wont make chamges the original path for future usage
                char* cpath = strdup(path);
                // directory that in each iteration will change accordingly
                char* directory = strtok(cpath, ":");
                // empty plain for the snprintf
                char currentPath[1024];
                const char s[2] = ":";
                while (directory != NULL)
                {
                    snprintf(currentPath, sizeof(currentPath), "%s/%s", directory, cmd);
                    /// checks if the input exists in the path in a specific section
                    if (access(currentPath,F_OK) == 0)
                    {
                        // we found it but we need to make sure it has execute permissions
                        if (access(currentPath,X_OK) == 0)
                        {
                            printf("%s is %s \n", cmd, currentPath);
                            flag = 1;
                            break;
                        }
                        else { directory = strtok(NULL, s); }
                    }
                    else
                    {
                        directory = strtok(NULL, s);
                    }
                }
                free(cpath);
            }
            if (flag == 0 && cmd != NULL)
            {
                printf("%s: not found\n", cmd);
            }
            }
int history_start_index = 0;
void history(char** seperatedWords, int numArgs)
{
    HIST_ENTRY **list = history_list();
    if (!list)
    {
        return;
    }

    // gets the legnth of the entire history
    int total = 0;
    while (list[total])
    {
        total++;
    }
    // if the user only typed history without anything else
    if (numArgs == 1)
    {
        // get history
        for (int i = 0; list[i]; i++) {
            // print each history part by order
            printf("%5d  %s\n", i + history_base, list[i]->line);
        }
    }else if (numArgs == 2 && atoi(seperatedWords[1]) > 0) {
            int n = atoi(seperatedWords[1]);

            // calculate exatly which history parts to show the user
            int start = (total > n) ? (total - n) : 0;

            for (int i = start; i < total; i++) {
                printf("%5d  %s\n", i + history_base, list[i]->line);
            }
        }// lets the user decide where the the history will be saved and saves it at that moment
    else if (numArgs >= 3 && strcmp(seperatedWords[1], "-w") == 0) {
        char* custom_path = seperatedWords[2];

        if (write_history(custom_path) != 0) {
            fprintf(stderr, "history: %s: Cannot write to file\n", custom_path);
        }
    }
    // get history of a specific file
    else if (numArgs >= 3 && strcmp(seperatedWords[1], "-r") == 0) {
        char* custom_path = seperatedWords[2];

        if (read_history(custom_path) != 0) {

            fprintf(stderr, "history: %s: No such file or directory\n", custom_path);
        }
    }
    else if (numArgs >= 3 && strcmp(seperatedWords[1], "-a") == 0)
    {
        char* custom_path = seperatedWords[2];
        int lines_to_append = history_length - history_start_index;

        if (lines_to_append > 0)
        {
            // dont delete the history only add in the end
            if (append_history(lines_to_append, custom_path) != 0) {
                fprintf(stderr, "history: %s: Cannot append to file\n", custom_path);
            } else {
                // update the index
                history_start_index = history_length;
            }
        }
    }
}
char* fileCompletion(const char* text, int state) {
    static DIR *d;
    static struct dirent *dir;
    static int len;
    static char directory[1024];
    static char prefix[1024];
    static const char* fileNamePrefix;
    // open the current directory and calc the length of the text
    if (!state) {
        if (d)
        {
            closedir(d);
        }
        char* slash = strrchr(text, '/');
        if (slash == NULL) {
            // if theres no /
            strcpy(directory, ".");
            prefix[0] = '\0';
            fileNamePrefix = text;
        } else {
            int length = (slash - text)+1;
            strncpy(prefix, text, length);
            prefix[length] = '\0';
            if(length >1)
            {
                strncpy(directory, text, length - 1);
                directory[length - 1] = '\0';
            } else
            {
                strcpy(directory, "/");
            }

            fileNamePrefix = slash + 1;
        }
        d = opendir(directory);
        len = strlen(fileNamePrefix);
    }

    if (d) {
        // goes through the files and folders in dir
        while ((dir = readdir(d)) != NULL) {
            // skip current folder and father folder
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue;

            //checks if any start of a file name fits the input
            if (strncmp(dir->d_name, fileNamePrefix, len) == 0) {
                char fullPath[2048];
                snprintf(fullPath, sizeof(fullPath), "%s/%s", directory, dir->d_name);
                struct stat st;
                int isDir = 0;
                if (stat(fullPath, &st) == 0 && S_ISDIR(st.st_mode)) {
                    isDir = 1;
                }
                char* completion = malloc(strlen(prefix) + strlen(dir->d_name) + 1);
                sprintf(completion, "%s%s", prefix, dir->d_name);
                if (isDir) {
                    strcat(completion, "/");
                    rl_completion_append_character = '\0';
                } else {
                    rl_completion_append_character = ' ';
                }
                return completion;
            }
        }
        closedir(d);
        d = NULL;
    }
    return NULL;
}
// this function will tell me if there < command and if so will clean the seperated words accordingly
// also checks if <2
// also checks for append
char* redirectionFunc(char** seperatedWords, int* oneOrTwo, int* isAppend)
{
    int i = 0;
    *oneOrTwo = 1;
    *isAppend = 0;
    while (seperatedWords[i] != NULL)
    {
        // checks if there is a > command
        if (strcmp(seperatedWords[i], ">") == 0 || strcmp(seperatedWords[i], "1>") == 0)
        {
            *oneOrTwo = 1;

        }else if (strcmp(seperatedWords[i], ">>") == 0 || strcmp(seperatedWords[i], "1>>") == 0)
        {
            // we turn on the is append
            *oneOrTwo = 1;
            *isAppend = 1;
        }else if (strcmp(seperatedWords[i], "2>") == 0)
        {
            // making sure append is turned off
            *isAppend = 0;
            *oneOrTwo = 2;
        }else if (strcmp(seperatedWords[i], "2>>") == 0)
        {
            *isAppend = 1;
            *oneOrTwo = 2;
        }else
        {
            i++;
            continue;
        }
        // right after > we supposed to see the fileName
        char* fileName = seperatedWords[i + 1];
        // check if there is actual filename
        if (fileName == NULL)
        {
            fprintf(stderr, "no file name \n");
            return NULL;
        }
        free(seperatedWords[i]);
        // delete the >
        seperatedWords[i] = NULL;
        return fileName;
    }
    //no >
    return NULL;
}
// manages all the free that needed to be done
void free_args(char** seperatedWords, int numArgs) {
    for (int i = 0; i < numArgs; i++) {
        if (seperatedWords[i] != NULL) {
            free(seperatedWords[i]);
        }
    }
}

// saves code duplication
int applyRedirection(char* fileName, int target, int append, int* savedPipeLine, char** seperatedWords, int numArgs)
{
    // if there is a < command
    if (fileName != NULL)
    {
        int flags = O_WRONLY | O_CREAT;
        //if append is turned on we need to not delete the entire file
        if (append)
        {
            flags |= O_APPEND;
        }
        else
        {
            flags |= O_TRUNC;
        }

        *savedPipeLine = dup(target);

        //O_WRONLY == only writing
        //O_CREAT == if the file doesnt exists create it
        // O_TRUNC == if exists delete content inside
        int folder = open(fileName, flags, 0644);

        if (folder != -1)
        {
            dup2(folder, target);
            close(folder);
        }
        else
        {
            perror("open failed");
            free_args(seperatedWords, numArgs);
            return 1; // continue in while loop in main
        }
    }
    return 0;
}

char* builtinCompletion(const char* text, int state) {

    static int list_index, len;
    char* builtins[] = {"exit", "echo", "type", "pwd", "cd", NULL};

    if (!state) {
        //if state is 0 it means we got a new word so we want to calc her length
        list_index = 0;
        len = strlen(text);
    }

    char* name;
    while ((name = builtins[list_index++])) {
        // checks if any of the builtins commands start with the text characters
        if (strncmp(name, text, len) == 0) {
            // if we found a match we just need to return the actual builtin command
            return strdup(name);
        }
    }
    return NULL;
}
// checks each directory in the path for a file that starts with the input
char* pathCompletionGenerator(const char* text, int state)
{
    static int list_index;
    static char *path_copy, *directory;
    static DIR *d;
    static int len;

    // first tab press and only the first
    if (!state) {
        // close any directory who may be still open
        if (d) closedir(d);
        d = NULL;
        free(path_copy);
        path_copy = strdup(getenv("PATH"));
        // first directory
        directory = strtok(path_copy, ":");
        len = strlen(text);
    }

    struct dirent *dir;
    while (directory != NULL) {
        if (!d) {
            d = opendir(directory);
        }

        if (d) {
            // goes through all the files in the path
            while ((dir = readdir(d)) != NULL) {
                // checks if any of the files has the same start as what the input is
                if (strncmp(dir->d_name, text, len) == 0 && dir->d_name[0] != '.') {

                    char full_path[1024];
                    snprintf(full_path, sizeof(full_path), "%s/%s", directory, dir->d_name);

                    if (access(full_path, X_OK) == 0) {
                        return strdup(dir->d_name);
                    }
                }
            }
            closedir(d);
            d = NULL;
        }
        // goes to the next directory to check there for file
        directory = strtok(NULL, ":");
    }

    return NULL;
}
// checks first for builtin and then goes through all the path
char** commandCompletionGenerator(const char* text, int start, int end) {
    // this line says if you didn't find any building command that fits don't try to find  names from the file
    rl_attempted_completion_over = 1;
    if(start==0)
    {
        // first checks if any buildin fits
        char* builtin_match = builtinCompletion(text, 0);
        if (builtin_match != NULL) {
            free(builtin_match);
            return rl_completion_matches(text, builtinCompletion);
        }
        // if no builtin fits we will check any file in each directory in the path
        return rl_completion_matches(text, pathCompletionGenerator);
    } else
    {
        return rl_completion_matches(text, fileCompletion);
    }

}
// saves the duplication code of running non builtin commands
void runNonBuiltIn(char** args, int numArgs) {
    char* startCommand = args[0];

    if (startCommand == NULL) {
        exit(EXIT_SUCCESS);
    }
    int target = 1;
    int append = 0;
    // clean any < if needed
    char* fileName = redirectionFunc(args, &target, &append);
    char* path = getenv("PATH");
    // creates a copy of path because iam going to edit cpath and i dont want to change the original path
    char* cpath = strdup(path);
    // get the first word before :
    char* directory = strtok(cpath, ":");
    char currentPath[1024];
    int found = 0;

    // for each subsection in the path we need to check if the program is in this specific subsection
    while (directory != NULL) {
        // creates path + command/program
        snprintf(currentPath, sizeof(currentPath), "%s/%s", directory, startCommand);

        // checks if executable and exists
        if (access(currentPath, X_OK) == 0) {
            found = 1;


            int dummy = -1;
            if (applyRedirection(fileName, target, append, &dummy, args, numArgs) == 1) {
                free(cpath);
                exit(EXIT_FAILURE);
            }

            // runs the program on the current process (which is already a son)
            if (execv(currentPath, args) == -1) {
                perror("execv");
                free(cpath);
                exit(EXIT_FAILURE);
            }
        }
        directory = strtok(NULL, ":");
    }

    if (found == 0) {
        printf("%s: not found\n", startCommand);
    }

    free(cpath);
    exit(127);
}
void decisionMaker(char** seperatedWords, int numArgs,char** builtins) {
    char* cmd = seperatedWords[0];

    int dummy = -1;
    int target = 1;
    int append = 0;
    int savedPipeLine = -1;
    char* fileName = redirectionFunc(seperatedWords, &target, &append);
    applyRedirection(fileName, target, append, &savedPipeLine, seperatedWords, numArgs);
    //checks if redirection exists
    if (fileName != NULL) {
        applyRedirection(fileName, target, append, &dummy, seperatedWords, numArgs);
    }

    // checks for builtins
    if (strcmp(cmd, "echo") == 0) {
        Echo(seperatedWords, numArgs);
    }
    else if (strcmp(cmd, "type") == 0) {
        Type(seperatedWords, numArgs, builtins);
    }
    else if (strcmp(cmd, "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s\n", cwd);
        }
    }
    // if not buildin runNonBuiltIn
    else {
        runNonBuiltIn(seperatedWords, numArgs);
    }
    if (savedPipeLine != -1) {
        dup2(savedPipeLine, target);
        close(savedPipeLine);
    }
}

void doPipeLine(char** firstCmd, char** secondCmd,int numArgs, char** builtins)
{
    int pipeInAndPipeOut[2];
    //creates a entry point and delivery point / write end and read end
    if (pipe(pipeInAndPipeOut) ==-1)
    {
        perror("pipeInAndPipeOut");
        return;
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        //  Making the output to the screen go insted to the write end
        dup2(pipeInAndPipeOut[1], STDOUT_FILENO);

        //after all data has been recived and delivered close the pipeLine
        close(pipeInAndPipeOut[0]);
        close(pipeInAndPipeOut[1]);
        // decide which type of command the user used (builtin/non builtin)
        decisionMaker(firstCmd, numArgs, builtins);
        exit(EXIT_SUCCESS);
    }
    pid_t pid2 =fork();
    if (pid2 == 0)
    {
        //  now insted of waiting to the keyboard insted take the input from the read end
        dup2(pipeInAndPipeOut[0], STDIN_FILENO);

        close(pipeInAndPipeOut[0]);
        close(pipeInAndPipeOut[1]);
        decisionMaker(secondCmd, numArgs, builtins);
        exit(EXIT_SUCCESS);
    }
    close(pipeInAndPipeOut[0]);
    close(pipeInAndPipeOut[1]);
    waitpid(pid,NULL,0);
    waitpid(pid2,NULL,0);
}
int findPipeLine(int numArgs, char *seperateWords[],char** builtins)
{
    int pipeIndex = -1;
    for (int i = 0; i < numArgs; i++)
    {
        // checks if there is a pipeline sign if so saves it location
        if (strcmp(seperateWords[i],"|")==0)
        {
            pipeIndex = i;
            break;
        }
    }
    if (pipeIndex != -1)
    {
        free(seperateWords[pipeIndex]);
        //we put insted of | a NUll so we can know which part is the giver and which the taker
        seperateWords[pipeIndex] = NULL;
        char** firstCmd = seperateWords;
        char** secondCmd = &seperateWords[pipeIndex + 1];

        doPipeLine(firstCmd, secondCmd, numArgs, builtins);
        // found pipeline
        return 1 ;
    }else
    {
        //there is no pipeline
        return 0;
    }
}


int main(int argc, char* argv[])
{
    // setting up the showing up history of a specific file
    char history_path[1024];
    char* histfile = getenv("HISTFILE");
    if (histfile != NULL)
    {
        strncpy(history_path, histfile, sizeof(history_path) - 1);
        history_path[sizeof(history_path) - 1] = '\0';
    }else
    {
        // we need the home directory so we can access history no matter where which folder we are on
        char* home = getenv("HOME");
        if (home) {
            // connect the shell history to HOME also . means it wont show up regulary in home to get a cleaner home folder
            snprintf(history_path, sizeof(history_path), "%s/.shell_history", home);
        } else {
            // if HOME doesnt exist just create in the folder you are in right now
            strncpy(history_path, ".shell_history", sizeof(history_path));
        }
    }
    setbuf(stdout, NULL);
    char* builtins[] = {"exit", "echo", "type", "pwd", "cd","history", NULL};
    rl_attempted_completion_function = commandCompletionGenerator;
    read_history(history_path);
    history_start_index = history_length;
    while (1)
    {
        char *input = readline("$ ");
        // if the user presses ctrl +D (eof)
        if (!input) {
            write_history(history_path);
            break;
        }
        // add anything the user typed
        if (*input) {
            add_history(input);
        }
        //sets the buffer
        char buffer[1000];
        strncpy(buffer, input, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        free(input);
        // strspn calc the amount of chars until \n then we want to change it specifically
        buffer[strcspn(buffer, "\n")] = '\0';
        char* seperatedWords[100];
        int numArgs = BetterStrTok(buffer, seperatedWords);

        // checks if the user didn't accidentally entered enter
        if (numArgs == 0) {
            continue;
        }
        // ---exit command---
        if (strcmp(seperatedWords[0], "exit") == 0)
        {
            // when the user finishes write down the history
            write_history(history_path);
            return 0;
        }else if (strcmp(seperatedWords[0], "history") == 0)
        {
            history(seperatedWords, numArgs);
        }
        // ---change directory command---
        else if (strcmp(seperatedWords[0], "cd") == 0)
        {
            // if the user types only cd  then continue
            if (numArgs < 2 || seperatedWords[1] == NULL)
            {
                free_args(seperatedWords, numArgs);
                continue;
            }
            char* target = seperatedWords[1];
            char newpath[1024];
            // if the user uses ~
            if (target[0] == '~')
            {
                char* Home = getenv("HOME");
                if (Home != NULL)
                {
                    // i use what the user has typed after ~ and add it after the newpath
                    snprintf(newpath, sizeof(newpath), "%s%s", Home, target + 1);
                    target = newpath;
                }
                else
                {
                    fprintf(stderr, "cd: HOME environment variable not set\n");
                    free_args(seperatedWords, numArgs);
                    continue;
                }
            }
            if (chdir(target) == -1)
            {
                // if theres an eror with the chdir
                printf("cd: %s: No such file or directory\n", seperatedWords[1]);
            }
            // checks if pipeline | exists
        }
        else if (findPipeLine(numArgs, seperatedWords, builtins)){}
        //---running programs---
        else
        {
            pid_t pid = fork();
            if (pid == 0) {
                decisionMaker(seperatedWords, numArgs, builtins);
                exit(EXIT_SUCCESS);
            } else {
                waitpid(pid, NULL, 0);
            }
        }
        for (int i = 0; i < numArgs; i++)
        {
            free(seperatedWords[i]);
        }
    }
    return 0;
}
