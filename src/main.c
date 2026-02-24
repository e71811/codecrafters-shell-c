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
         for(int i = 0; i < strlen(buffer); i++)
         {
           if(buffer[i] == '\n')
           {
             userInput[i] = '\0';
             // exit command
             if (strcmp(userInput,"exit")==0)
             {
               return 0 ;
               // echo command
             } else if (userInput[0] == 'e' && userInput[1] == 'c' && userInput[2] == 'h' && userInput[3] == 'o')
             {
               for (int i = 5; i < strlen(userInput); i++)
               {
                 printf("%c", userInput[i]);
               }
               printf("\n");
               break;
             }
             // Type command
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
             else {
               printf("%s",userInput);
               printf(": command not found \n");
               break;}

           }
             userInput[i]=buffer[i];
         }
    }
  return 0;
}
