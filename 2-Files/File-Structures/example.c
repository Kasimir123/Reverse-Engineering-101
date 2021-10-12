#include <stdio.h>

int global = 1337;

char * wowCool = "Wow, cool function! \n";

int thisIsAnotherFunction()
{
   printf(wowCool);
}

int main()
{
   printf("Hello world! \n");
   return 0;
}