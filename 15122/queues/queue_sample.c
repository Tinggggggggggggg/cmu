#include <stdlib.h>        // Standard C library: free(), NULL...
#include <stdbool.h>       // Standard true, false, and bool type
#include "lib/contracts.h" // Our contracts library
#include "lib/xalloc.h"    // Our allocation library
#include "queue.h"         // The queue interface

typedef struct list_node list;
struct list_node
{
  void *data;
  list *next;
};

typedef struct queue_header queue;
struct queue_header
{
  list *front;
  list *back;
  size_t size;
};

bool is_inclusive_segment(list *start, list *end, int i)
{
  /* FIX ME */
  if (i == 0)
  {
    if (start != NULL)
      return false;
    else
      return true;
  }
  if (i == 1)
  {
    if (start != end)
      return false;
    else
      return true;
  }
  int length = i;
  while ((start->next) != NULL && length > 1)
  {
    start = start->next;
    length--;
  } // start next !=null and length is 2
    // start = start next next length is 1
  if (length == 1 && start->next == NULL)
  {
    return true;
  }
  // length==1  or start->next == NULL
  // should
  // if length!=1 && start-> then return false
  // if length==1 && start->==NULL return true
  //::::two need to fail at same time
  // else all false because cannot have length==1 and start not null
  //                 or length!=1 start-> == NULL
  // so no need for circularity
  return false;
}

bool is_queue(queue *Q)
//@requires Q->size>=0;
{
  // REQUIRES(Q->size>=0);
  return Q != NULL && is_inclusive_segment(Q->front, Q->back, Q->size);
}
/*

queue *queue_new()
/*@ensures \result != NULL&& is_queue(\result); @*/
{
  queue *q = xmalloc(sizeof(queue));
  q->front = NULL; // dont care what end is
  q->size = 0;
  ENSURES(q != NULL && is_queue(q));
  return q;
}

size_t queue_size(queue *Q)
/*@requires Q != NULL&& is_queue(Q); @*/
/*@ensures \result >= 0; @*/
{
  REQUIRES(Q != NULL && is_queue(Q));
  // ENSURES (Q->size >= 0);
  return Q->size;
}

void enq(queue *Q, void *x)
/*@requires Q != NULL&& is_queue(Q); @*/
//@ensures Q->size>=0;
{
  // if 0 and call, front should be point to the only one
  // we cannot assume the start already have one item
  REQUIRES(Q != NULL && is_queue(Q));
  if (Q->size == 0)
  {
    Q->front = xmalloc(sizeof(list));
    Q->back = Q->front;
    Q->front->data = x;
    Q->front->next = NULL;
    Q->size += 1;
    return;
  }
  list *l = xmalloc(sizeof(list));
  l->data = x;
  l->next = NULL;
  Q->back->next = l;
  Q->back = l;

  Q->size += 1;
  // ENSURES(Q->size>=0);
  return; // dummy
}

void *deq(queue *Q)
/*@requires Q != NULL && is_queue(Q) && queue_size(Q) > 0; @*/
{
  REQUIRES(Q != NULL && is_queue(Q) && queue_size(Q) > 0);
  list *l = Q->front;
  Q->front = l->next;
  // l->next = NULL;
  Q->size -= 1;
  void *ret = l->data;
  free(l);
  return ret;
}

/* O(i) -- doesn't remove the item from the queue */
void *queue_peek(queue *Q, size_t i)
/*@requires Q != NULL&& is_queue(Q) && 0 <= i && i < queue_size(Q); @*/
{
  REQUIRES(Q != NULL && is_queue(Q) && i < queue_size(Q));
  list *l = Q->front;
  while (i > 0)
  {
    l = l->next;
    i--;
  }
  return l->data;
}

void queue_reverse(queue *Q)
/*@requires Q != NULL&& is_queue(Q); @*/
{
  REQUIRES(Q != NULL && is_queue(Q));
  if (Q->size < 2)
  {
    return;
  }
  list *pr = NULL;
  list *ne = Q->front;
  list *cur = Q->front;
  Q->back = cur;
  int i = 0;
  while (ne != NULL)
  {
    // printf("%d : %d\n",i,*(int*)cur->data);
    ne = ne->next;
    cur->next = pr;
    pr = cur;
    cur = ne;
    i++;
  }
  Q->front = pr;
}
/*
If F is NULL, then we free only the queue’s internal data structures,
and not the pointers stored in the queue.
This means that the queue’s data elements are owned elsewhere
, and some other data structure has responsibility
for freeing the pointers. */
// typedef void free_fn(void *x);

void queue_free(queue_t Q, free_fn *F)
/*@requires Q != NULL; @*/
{
  REQUIRES(Q != NULL);
  if (F == NULL)
  {
    if (Q->front == NULL)
    {
      free(Q);
      return;
    }
    list *l = Q->front;
    list *nex = NULL;
    while (l != NULL)
    {
      nex = l->next;
      free(l);
      l = nex;
    } // l == NULL
    free(Q);
  }
  else
  {
    if (Q->front == NULL)
    {
      free(Q);
      return;
    }
    list *l = Q->front;
    list *nex = NULL;
    while (l != NULL)
    {
      nex = l->next;
      (*F)(l->data);
      free(l);
      l = nex;
    } // l == NULL
    free(Q);
    // free(F);
  }
}

/* O(n) worst case, assuming P is O(1) */
bool queue_all(queue_t Q, check_property_fn *P)
/*@requires Q != NULL && is_queue(Q)&& P != NULL; @*/
{
  REQUIRES(Q != NULL && is_queue(Q) && P != NULL);
  // if(Q->size==0){return true;}
  list *l = Q->front;
  while (l != NULL)
  {
    if (!((*P)(l->data)))
      return false;
    l = l->next;
  }
  return true;
}

/* O(n) worst case, assuming F is O(1) */
void *queue_iterate(queue_t Q, void *base, iterate_fn *F)
/*@requires Q != NULL&& is_queue(Q) && F != NULL; @*/
{
  // if(Q->size==0){return true;}
  REQUIRES(Q != NULL && is_queue(Q) && F != NULL);
  list *l = Q->front;
  void *a = base;
  while (l != NULL)
  {
    a = (*F)(a, (l->data));
    l = l->next;
  }
  return a;
}
