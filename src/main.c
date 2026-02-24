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
         for(int i = 0; i < strlen(buffer); i++)
         {
           if(buffer[i] == '\n')
           {
             userInput[i] = '\0';
             if (strcmp(userInput,"exit")==0)
             {
               return 0 ;
             }
             if (userInput[0] == 'e' && userInput[1] == 'c' && userInput[2] == 'h' && userInput[3] == 'o')
             {
               for (int i = 4; i < strlen(userInput); i++)
               {
                 printf("%c", userInput[i]);
               }
               printf("\n");
               break;
             }
             printf("%s",userInput);
             printf(": command not found \n");
             break;
           }
             userInput[i]=buffer[i];
         }
    }
  return 0;
}
