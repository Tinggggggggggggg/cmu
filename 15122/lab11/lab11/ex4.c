#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/xalloc.h"

char *reverse(char *s) {
  char *result = xcalloc(strlen(s), sizeof(char));  // is the length correct?
  for (size_t i = 0; i < strlen(s); i++) {
    result[i] = s[strlen(s)-i-1];
  }
  result[strlen(s)] = '\0';
  return result;
}

int main() {
  char *input = "This is Honk!";
  char *rev = reverse(input);

  printf("Our input is: %s\n", input);
  printf("We reverse to: %s\n", rev);

  return 0;
}
