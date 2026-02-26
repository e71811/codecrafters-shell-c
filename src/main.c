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


int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
    while(1)
    {
      printf("$ ");
      //sets the buffer
      char buffer[1000];
      //1 gives the source of input 2 whats the max size of the input can be
       fgets(buffer, sizeof(buffer), stdin);
      //sets the input so i can interact with it later
         char userInput[sizeof(buffer)];
      // sets buit in commands
      char *builtins[] = {"exit", "echo", "type", NULL};
      int i ;
      // copying  the buffer content and making sure that in the end of it theres \0
      for(i = 0; i < strlen(buffer); i++) {
        if(buffer[i] == '\n') break;
        userInput[i] = buffer[i];
      }
      userInput[i] = '\0';

             // ---exit command---
             if (strcmp(userInput,"exit")==0)
             {
               return 0 ;
               // ---echo command---
             } else if (userInput[0] == 'e' && userInput[1] == 'c' && userInput[2]
               == 'h' && userInput[3] == 'o')
             {
               for (int i = 5; i < strlen(userInput); i++)
               {
                 printf("%c", userInput[i]);
               }
               printf("\n");
             }

             // ---Type command---
             else if (strncmp(userInput,"type ",5) == 0)
             {
               const char s[2] = " ";
               char *cmd;
               cmd = strtok(userInput, s);
               //gets the word after type (its supposed to go after on of the directories that exists in path )
               cmd = strtok(NULL, s);
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

             //---running programs---
             else if(true)
             {
               char *args[64];
               int argumentCount = 0;
               char *currentArgument = strtok(userInput, " ");
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
               }
  return 0;
}