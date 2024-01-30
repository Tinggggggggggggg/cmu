/*
 * Generic sorting
 *
 * 15-122 Principles of Imperative Computation
 */

#include <stdlib.h>
#include <stdbool.h>

#ifndef GSORT_H
#define GSORT_H

// Compares the objects at memory locations x and y
typedef int compare_fn(void *x, void *y)
  /*@requires x != NULL && y != NULL; @*/ ;

// Swaps the objects at memory locations x and y
typedef void swap_fn(void *x, void *y)
  /*@requires x != NULL && y != NULL; @*/ ;

void gsort(void *A, size_t count, size_t elt_size,
           compare_fn *compar,
           swap_fn *swp)
  /*@requires A != NULL && compar != NULL && swp != NULL; @*/
  /* requires that A is an allocation of at least count * elt_size */ ;

#endif
