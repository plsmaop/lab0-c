#include "queue.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (NULL == q)
        return NULL;

    // init
    memset(q, 0, sizeof(queue_t));
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* What should you do if the q is NULL? */
    if (NULL == q)
        return false;

    list_ele_t *newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (NULL == newh)
        return false;

    memset(newh, 0, sizeof(list_ele_t));

    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    size_t stringLen = strlen(s);
    newh->value = (char *) malloc(stringLen + 1);
    if (NULL == newh->value) {
        free(newh);
        return false;
    }

    memset(newh->value, 0, stringLen + 1);
    memcpy(newh->value, s, stringLen);
    newh->value_size = stringLen;

    newh->next = q->head;
    if (NULL == q->tail) {
        q->tail = newh;
    }

    q->head = newh;
    q->q_size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* Remember: It should operate in O(1) time */
    if (NULL == q)
        return false;

    list_ele_t *newTail = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (NULL == newTail)
        return false;

    memset(newTail, 0, sizeof(list_ele_t));

    size_t stringLen = strlen(s);
    newTail->value = (char *) malloc(stringLen + 1);
    if (NULL == newTail->value) {
        free(newTail);
        return false;
    }

    memset(newTail->value, 0, stringLen + 1);
    memcpy(newTail->value, s, stringLen);
    newTail->value_size = stringLen;

    if (NULL == q->head)
        q->head = newTail;
    else
        q->tail->next = newTail;

    q->tail = newTail;
    q->q_size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* You need to fix up this code. */
    if (NULL == q || 0 == q->q_size || NULL == q->head)
        return false;
    list_ele_t *del = q->head;

    if (del == q->tail) {
        q->tail = NULL;
    }

    q->head = q->head->next;

    if (NULL != sp) {
        memset(sp, 0, bufsize);

        size_t copy_size =
            (bufsize - 1) < del->value_size ? (bufsize - 1) : del->value_size;
        memcpy(sp, del->value, copy_size);
    }
    free(del->value);
    free(del);
    q->q_size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    if (NULL == q)
        return 0;
    return q->q_size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
}
