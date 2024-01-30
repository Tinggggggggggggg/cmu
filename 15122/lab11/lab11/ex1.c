#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main()
{
  // Declare and initialize strings on the stack
  char buf[8] = "honking";
  char word[4] = {'h', 'o', 'n', 'k'};

  printf("buf string: %s\n", buf);
  printf("word string: %s\n", word);
  printf("word characters: '%c' '%c' '%c' '%c'\n",
     word[0], word[1], word[2], word[3]);
  printf("word ascii values: %d %d %d %d\n",
     word[0], word[1], word[2], word[3]);

  return 0;
}
