/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2023                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
#ifndef PQ_H
#define PQ_H

#include <stdbool.h>


/*********************/
/* Client interface  */
/*********************/

// Type of the elements of the priority queue
typedef void* elem;  // Supplied by client

// Type of priority functions
//    f(e1,e2) returns true if e1 is STRICTLY higher priority than e2
typedef bool has_higher_priority_fn(elem e1, elem e2);
//@requires e1 != NULL && e2 != NULL;

// Type of functions used to free elements
typedef void elem_free_fn(elem e);
//@requires e != NULL;


/*********************/
/* Library interface */
/*********************/

typedef struct heap_header* pq_t;

// pq_empty(Q) returns true if Q is empty
bool pq_empty(pq_t Q)
  /*@requires Q != NULL; @*/ ;

// pq_full(Q) returns true if Q is full
bool pq_full(pq_t Q)
  /*@requires Q != NULL; @*/ ;

// pq_new(capacity, prior, fr) create a new priority queue where
// - capacity is its capacity
// - prior is its priority function
// - fr is the function that frees its elements; use NULL not to free elements
pq_t pq_new(int capacity, has_higher_priority_fn* prior, elem_free_fn* fr)
  /*@requires capacity > 0 && prior != NULL; @*/
  /*@ensures \result != NULL && pq_empty(\result); @*/ ;

// pq_add(Q, x) adds x to Q
void pq_add(pq_t Q, elem x)
  /*@requires Q != NULL && !pq_full(Q) && x != NULL; @*/ ;

// pq_rem(Q) removes from Q and returns an element with highest priority
elem pq_rem(pq_t Q)
  /*@requires Q != NULL && !pq_empty(Q); @*/
  /*@ensures \result != NULL; @*/ ;

// pq_peek(Q) returns an element with highest priority without removing it from Q
elem pq_peek(pq_t Q)
  /*@requires Q != NULL && !pq_empty(Q); @*/
  /*@ensures \result != NULL; @*/ ;

// pq_free(Q) frees the priority queue Q
void pq_free(pq_t Q)
  /*@requires Q != NULL; @*/ ;
#endif
