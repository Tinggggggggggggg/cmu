#include <stdlib.h>
#include <stdbool.h>
#include "lib/gsort.h"
#include "lib/contracts.h"

void gsort(void *A, size_t count, size_t elt_size,
           compare_fn *cmp, swap_fn *swap)
{
  char *B = (char *)A;
  for (size_t i = 0; i < count * elt_size; i += elt_size)
  {
    size_t min = i * elt_size;
    for (size_t j = i + 1; j < count * elt_size; j += elt_size)
    {
      // size_t cur = j * elt_size;
      int comp = cmp(B + j, B + min);
      if (comp < 0)
      {
        min = j;
      }
    }
    swap(B + i, B + min);
  }

  /* WRITE ME */
  return;
}
