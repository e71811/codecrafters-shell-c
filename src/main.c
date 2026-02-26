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
// strtok doesnt work well with ' so i implemnted better function
int BetterStrTok(char *buffer ,char **seperatedWords)
{
  int flag = 0;
  int wordsCount = 0;
  char cWord[1000];
  int i= 0 ;
  int j = 0 ;
  while (buffer[i] != '\0')
  {
    // checks when we arrive to singular '
    if (buffer[i] == '\'')
    {
     flag =!flag ;
     i++;
      continue;
    }
    // checks when we arrive to space
    else if (buffer[i] == ' ')
    {
      if (flag)
      {
        // every character acts as literal so we just copy it
        cWord[j] = buffer[i];
        j++;
      }else
      {
        // if the user typed alot of spaces and then a word we dont want to save the word with all the spaces
        // (not literal)
        if(j>0)
        {
          cWord[j] = '\0';
          seperatedWords[wordsCount] = strdup(cWord);
          wordsCount++;
          j=0;
        }

      }
    }else
    {
      // we found a character which is just part of a big word so no need of doing something special
      cWord[j]=buffer[i];
       j++;
    }
    i++;
  }
  // if we are at the last word and we saw /0 the while loop will end but we need to save the last word
  if (j>0)
  {
    cWord[j] = '\0';
    seperatedWords[wordsCount] = strdup(cWord);
    wordsCount++;
  }
  // we ended with a NULL so we can easily go through the words and know when we finised
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
               char *args[64];
               int argumentCount = 0;
               char *currentArgument = seperatedWords[0];
               while(currentArgument != NULL )
               {
                 args[argumentCount] = currentArgument;
                 argumentCount++;
                 currentArgument = strtok(NULL, " ");
               }
               //for exec to work we need at the end NUll
               args[argumentCount] = NULL;
               // in case the user just entered " " without anything else after
               // also the name of the command
               char *startCommand = args[0];
               if (startCommand == NULL)
               {
                 continue;
               }

               int flag = 0;
               for (int i = 0; builtins[i] !=NULL; i++)
               {
                 if(strcmp(startCommand,builtins[i]) == 0)
                 {
                   printf("%s ", builtins[i]);
                   printf("is a shell builtin\n");
                   flag = 1;
                   break;
                 }
               }
                 if (flag==0 && startCommand != NULL )
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
                     snprintf(currentPath,sizeof(currentPath),"%s/%s",directory ,startCommand);
                     /// checks if the input exists in the path in a specific section
                     if (access(currentPath,F_OK)==0)
                     {
                       // we found it but we need to make sure it has execute permissions
                       if (access(currentPath,X_OK)==0)
                       {
                         flag = 1 ;
                         // makes a son program that run inside our parent shell program (create copy of shell)
                          pid_t pid = fork();
                         // this if statmens makes sure that we only interact with the son who get the 0 val
                         // and not with the father which has other pid val
                         if(pid == 0 )
                         {
                           // safety measure to make sure that execve did not fail for some reason
                           // and also runs the program that the user sent and close son afterward
                           if(execve(currentPath,args,NULL) == -1)
                           {
                             perror("execve");
                             exit(EXIT_FAILURE);
                           }
                           // the father has different pid address so we interact here only with the father

                         }else if (pid>0)
                         {
                           // basicly means wait until one of the sons stops it actions
                           wait(NULL);
                           flag = 1 ;
                           break;
                         } else
                         {
                           printf("fork failed ");
                         }
                       }else{directory = strtok(NULL, s);}
                     }else
                     {
                       directory = strtok(NULL, s);
                     }
                   }
                   free(cpath);
                 }
                 if(flag== 0 && startCommand !=NULL)
                 {
                   printf("%s: not found\n", startCommand);
                 }
                 }
                 for (int i = 0; i < numArgs; i++)
                 {
                        free(seperatedWords[i]);
                 }
               }
  return 0;
}