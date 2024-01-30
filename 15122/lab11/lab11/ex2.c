#include <stdlib.h>
#include <string.h>
#include "lib/xalloc.h"

struct position_header {
  int x;
  int y;
  char s[5];  // includes a 5-element char array AS PART OF this struct
};
typedef struct position_header position;

int main() {
  position* p = xmalloc(sizeof(13));

  // DO NOT MODIFY ANYTHING BETWEEN THIS LINE
  printf("x: %d, y: %d. ", p->x, p->y);
  if (p->x == p->y) {
    printf("%s\n", p->s);
  } else {
    printf("Uh oh :(\n");
  }
  // AND THIS LINE

  return 0;
}
