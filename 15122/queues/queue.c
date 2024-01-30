#include <stdlib.h>        // Standard C library: free(), NULL...
#include <stdbool.h>       // Standard true, false, and bool type
#include "lib/contracts.h" // Our contracts library
#include "lib/xalloc.h"    // Our allocation library
#include "queue.h"         // The queue interface

// typedef void print_data_fn(void *x); // may be useful for debugging
// typedef bool check_property_fn(void *x);
// typedef void *iterate_fn(void *accum, void *x);

/************************* End Client Interface *************************/
/************************************************************************/

/************************************************************************/
/************************* BEGIN IMPLEMENTATION *************************/

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

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> WRITE YOUR CODE ONLY BETWEEN HERE */

bool is_inclusive_segment(list *start, list *end, int i)
{
    /* FIX ME */
    if (i < 0)
    {
        return false;
    }
    if (start == NULL)
    {
        return (i == 0);
        // return true;
    }
    if ((start == end))
    {
        // if (start->next != NULL || i != 1){
        return (start->next == NULL && i == 1);
    }
    int num = 0;
    // return is_inclusive_segment(start->next, end, i+1);
    while (start != NULL && num < i)
    {
        start = start->next;
        num += 1;
    }

    if (num == i && start == NULL)
    {
        return true;
    }
    // return (num == i && start == NULL);
    return false;

    // return true;
}

bool is_queue(queue *Q)
{
    /* FIX ME */
    return Q != NULL && is_inclusive_segment(Q->front, Q->back, Q->size);
}

/* Other functions go here */

/* AND HERE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

// Client type
// typedef queue *queue_t;

/************************** END IMPLEMENTATION **************************/
/************************************************************************/

/************************************************************************/
/*************************** Library Interface **************************/

// typedef ______* queue_t;

/* O(1) */
queue_t queue_new()
/*@ensures is_queue(\result); @*/
{
    queue_t Q = xmalloc(sizeof(queue));
    // list* L = alloc(list);
    Q->front = NULL;
    // Q->back = L;
    Q->size = 0;
    ENSURES(is_queue(Q));
    return Q;
}

/* O(1) */
size_t queue_size(queue_t Q)
/*@requires is_queue(Q); @*/
/*@ensures \result >= 0; @*/
{
    REQUIRES(is_queue(Q));
    return Q->size;
}

/* O(1) -- adds an item to the back of the queue */
void enq(queue_t Q, void *x)
/*@requires is_queue(Q); @*/
//@ensures Q->size >= 0;
{
    REQUIRES(is_queue(Q));
    if (Q->size == 0)
    {
        list *l = xmalloc(sizeof(list));
        l->data = x;
        l->next = NULL;
        Q->front = l;
        Q->back = Q->front;
        Q->size += 1;
    }
    else
    {
        list *l = xmalloc(sizeof(list));
        l->data = x;
        l->next = NULL;
        Q->back->next = l;
        Q->back = l;
        Q->size += 1;
    }
}

/* O(1) -- removes an item from the front of the queue */
void *deq(queue_t Q)
/*@requires queue_size(Q) > 0 && is_queue(Q); @*/
{
    REQUIRES(queue_size(Q) > 0 && is_queue(Q));
    list *l = Q->front;
    Q->front = Q->front->next;
    l->next = NULL;
    void *elem = l->data;
    Q->size -= 1;
    free(l);
    // Q->front = Q->front->next;
    return elem;
}

/* O(i) -- doesn't remove the item from the queue */
void *queue_peek(queue_t Q, size_t i)
/*@requires Q != NULL && 0 <= i && i < queue_size(Q) && is_queue(Q); @*/
{
    REQUIRES(is_queue(Q) && i < queue_size(Q));
    list *temp = Q->front;
    // size_t num = 0;
    while (i > 0)
    {
        temp = temp->next;
        i -= 1;
    }
    return temp->data;
}

/* O(n) */
// problomatic
void queue_reverse(queue_t Q)
/*@requires is_queue(Q); @*/
{
    REQUIRES(is_queue(Q));
    if (Q->size > 1)
    {
        Q->back = Q->front;
        list *prev = NULL;
        list *curr = Q->front;
        list *next = Q->front;
        while (next != NULL)
        {
            next = next->next;
            // prev = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        Q->front = prev;
    }
}

void queue_free(queue_t Q, free_fn *F)
/*@requires Q != NULL; @*/
{
    REQUIRES(is_queue(Q));
    while (Q->front != NULL)
    {
        list *temp = Q->front;
        if (F != NULL && temp->data != NULL)
        {
            (*F)(temp->data);
        }
        Q->front = Q->front->next;
        free(temp);
    }
    free(Q);
}

/* O(n) worst case, assuming P is O(1) */
bool queue_all(queue_t Q, check_property_fn *P)
/*@requires (Qis_queue) && P != NULL; @*/
{
    REQUIRES(is_queue(Q) && P != NULL);
    list *temp = Q->front;
    // int num = 0;
    while (temp != NULL)
    {
        // void* elem = queue_peek(temp, num);
        // num += 1;
        if (!(*P)(temp->data))
        {
            return false;
        }
        temp = temp->next;
    }
    return true;
}

/* O(n) worst case, assuming F is O(1) */
void *queue_iterate(queue_t Q, void *base, iterate_fn *F)
/*@requires is_queue(Q) && F != NULL; @*/
{
    REQUIRES(is_queue(Q) && F != NULL);
    list *temp = Q->front;
    void *init = base;
    while (temp != NULL)
    {
        init = (*F)(init, (temp->data));
        temp = temp->next;
    }
    return init;
}