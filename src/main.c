#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
    while(1)
    {
      printf("$ ");
      char buffer[1000];
       fgets(buffer, sizeof(buffer), stdin);
         char userInput[sizeof(buffer)];
         for(int i = 0; i < strlen(buffer); i++)
         {
           if(buffer[i] == '\n')
           {
             userInput[i] = '\0';
             printf("{");
               printf("%s",userInput);
             printf("}");
             printf(": command not found \n");
             break;
           }
             userInput[i]=buffer[i];
         }

    }

  return 0;
}
