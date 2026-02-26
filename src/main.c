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
// strtok doesnt work well with ' or " so i implemnted better function
int BetterStrTok(char *buffer, char **seperatedWords)
{
  // 0 = no quotes, '\'' = single quotes, '\"' = double quotes
  int quoteMode = 0;
  int wordsCount = 0;
  char cWord[1000];
  int i = 0;
  int j = 0;

  while (buffer[i] != '\0')
  {
    char c = buffer[i];

    // Check if we are currently NOT inside any quotes
    if (quoteMode == 0)
    {
      // Handle backslash escaping outside of quotes
      if (c == '\\')
      {
        i++; // Skip the backslash
        if (buffer[i] != '\0')
        {
          // Treat the next character literally, no matter what it is
          cWord[j++] = buffer[i];
        }
      }
      // checks when we arrive to singular '
      else if (c == '\'')
      {
        quoteMode = '\'';
      }
      // checks when we arrive to double "
      else if (c == '\"')
      {
        quoteMode = '\"';
      }
      // checks when we arrive to space
      else if (c == ' ')
      {
        // if the user typed alot of spaces and then a word we dont want to save the word with all the spaces
        if (j > 0)
        {
          cWord[j] = '\0';
          seperatedWords[wordsCount] = strdup(cWord);
          wordsCount++;
          j = 0;
        }
      }
      else
      {
        cWord[j++] = c;
      }
    }
    // Check if we are inside single quotes
    else if (quoteMode == '\'')
    {
      if (c == '\'')
      {
        quoteMode = 0;
      }
      else
      {
        cWord[j++] = c;
      }
    }
    // Check if we are inside double quotes
    else if (quoteMode == '\"')
    {
      if (c == '\"')
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
    seperatedWords[wordsCount] = strdup(cWord);
    wordsCount++;
  }

  seperatedWords[wordsCount] = NULL;
  return wordsCount;
}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  char *builtins[] = {"exit", "echo", "type","pwd","cd", NULL};
    while(1)
    {
      printf("$ ");
      //sets the buffer
      char buffer[1000];
      //1 gives the source of input 2 whats the max size of the input can be
       fgets(buffer, sizeof(buffer), stdin);
       // strspn calc the amount of chars until \n then we want to change it specificly
        buffer[strcspn(buffer, "\n")] = '\0';
        char *seperatedWords[100];
       int numArgs = BetterStrTok(buffer, seperatedWords);

             // ---exit command---
             if (strcmp(seperatedWords[0],"exit")==0)
             {
               return 0 ;

             }

             // ---echo command---
             else if (strcmp(seperatedWords[0],"echo")==0)
             {
               for (int i = 1; seperatedWords[i] != NULL; i++)
               {
                 printf("%s", seperatedWords[i]);
                 // adds " " for all the words except the last word
                 if (seperatedWords[i+1] != NULL)
                 {
                   printf(" ");
                 }
               }
               printf("\n");
             }

             // ---Type command---
             else if (strcmp(seperatedWords[0],"type") == 0)
             {
               if (numArgs < 2) {
                 //  if the input is only type without anything after i will just continue
                 continue;
               }
               char *cmd;
               cmd = seperatedWords[1];
               int flag = 0;
               for (int i = 0; builtins[i] !=NULL; i++)
               {
                 if(strcmp(cmd,builtins[i]) == 0)
                 {
                   printf("%s ", builtins[i]);
                   printf("is a shell builtin\n");
                   flag = 1;
                   break;

                 }
               }
               if (flag==0 && cmd != NULL )
               {
                 // first i need to get the specific path that i want to check
                 char *path=getenv("PATH");
                 // create a copy of the path so strtok wont make chamges the original path for future usage
                 char *cpath=strdup(path);
                 // directory that in each iteration will change accordingly
                 char *directory = strtok(cpath, ":");
                 // empty plain for the snprintf
                 char currentPath[1024];
                 const char s[2] = ":";
                 while (directory != NULL)
                 {
                   snprintf(currentPath,sizeof(currentPath),"%s/%s",directory ,cmd);
                   /// checks if the input exists in the path in a specific section
                   if (access(currentPath,F_OK)==0)
                   {
                     // we found it but we need to make sure it has execute permissions
                     if (access(currentPath,X_OK)==0)
                     {
                       printf("%s is %s \n",cmd,currentPath);
                       flag=1 ;
                       break;
                     }else{directory = strtok(NULL, s);}
                   }else
                   {
                     directory = strtok(NULL, s);
                   }
                 }
                 free(cpath);
               }
               if(flag== 0 && cmd !=NULL)
               {
                 printf("%s: not found\n", cmd);

               }

             }

                 // ---pwd command---
              else if (strcmp(seperatedWords[0],"pwd")==0){
               char path[1024];
               if(getcwd(path,sizeof(path)) == NULL)
               {
                 perror("getcwd eror accured");
               }else
               {
                 printf("%s\n", path);
               }

             }

                // ---change directory command---
               else if(strcmp(seperatedWords[0],"cd") == 0)
             {
                 // if the user types only cd  then continue
                 if (numArgs < 2 || seperatedWords[1] == NULL) {
                   continue;
                 }
                 char *target = seperatedWords[1];
               char newpath[1024] ;
                 // if the user uses ~
                 if (target[0] == '~')
                 {
                   char *Home = getenv("HOME");
                   if (Home != NULL) {
                     // i use what the user has typed after ~ and add it after the newpath
                     snprintf(newpath, sizeof(newpath), "%s%s", Home, target + 1);
                     target = newpath;
                   } else {
                     fprintf(stderr, "cd: HOME environment variable not set\n");
                     continue;
                   }
                 }
                   if (chdir(target) == -1) {
                     // if theres an eror with the chdir
                     printf("cd: %s: No such file or directory\n", seperatedWords[1]);
                   }
             }

             //---running programs---
             else if(true)
             {
               char *startCommand = seperatedWords[0];

               if (startCommand == NULL)
               {
                 continue;
               }
               char *path = getenv("PATH");
               // creates a copy of path because iam going to edit cpath and i dont want to change the original path
               char *cpath = strdup(path);
               // get the first word before :
               char *directory = strtok(cpath, ":");
               char currentPath[1024];
               int found = 0;
               // for each subsection in the path we need to check if the program is in this specific subsection
               while (directory != NULL) {
                 // creates path + command/program
                 snprintf(currentPath, sizeof(currentPath), "%s/%s", directory, startCommand);

                 // checks if executable and exists
                 if (access(currentPath, X_OK) == 0) {
                   found = 1;
                   // creates a son for the shell
                   pid_t pid = fork();
                   // only the son has pid val of 0 the father has the address in pid so that we have a way to interact
                   // with the son seperatly from the father
                   if (pid == 0) {
                     // runs the program on the son and checks if there was any erorr
                     if (execv(currentPath, seperatedWords) == -1) {
                       perror("execv");
                       exit(EXIT_FAILURE);
                     }
                     // commands the father to wait until his son stops whats his doing
                   } else if (pid > 0) {
                     wait(NULL);
                     break;
                   }
                 }
                 directory = strtok(NULL, ":");
               }

               if (found == 0) {
                 printf("%s: not found\n", startCommand);
               }
               free(cpath);

                 }
                 for (int i = 0; i < numArgs; i++)
                 {
                        free(seperatedWords[i]);
                 }
               }
  return 0;
}