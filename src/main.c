#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
               char *token;
               token = strtok(userInput, s);
               token = strtok(NULL, s);
               int flag = 0;
               for (int i = 0; builtins[i] !=NULL; i++)
               {
                 if(strcmp(token,builtins[i]) == 0)
                 {
                   printf("%s ", builtins[i]);
                   printf("is a shell builtin\n");
                   flag = 1;
                   break;

                 }
               }
               if (flag==0)
               {
                 printf("%s: not found\n", token);
                 break;
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
