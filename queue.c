#include "queue.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "strnatcmp.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    // What if malloc returned NULL?
    if (!q)
        return NULL;

    // init
    memset(q, 0, sizeof(queue_t));
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (!q)
        return;

    list_ele_t *p = q->head, *cur = NULL;
    while (p) {
        if (p->value)
            free(p->value);
        cur = p;
        p = p->next;
        free(cur);
    }
    free(q);
}

/*
 * Util function for construct a new list element
 */
list_ele_t *__q_insert_util(queue_t *q, char *s)
{
    list_ele_t *node = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (!node)
        return NULL;

    memset(node, 0, sizeof(list_ele_t));

    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    size_t stringLen = strlen(s);
    node->value = (char *) malloc(stringLen + 1);
    if (!node->value) {
        free(node);
        return NULL;
    }

    memset(node->value, 0, stringLen + 1);
    memcpy(node->value, s, stringLen);
    node->value_size = stringLen;

    return node;
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
    if (!q)
        return false;

    list_ele_t *newh = __q_insert_util(q, s);
    if (!newh) {
        return false;
    }

    newh->next = q->head;
    if (!q->tail) {
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
    if (!q)
        return false;

    list_ele_t *newTail = __q_insert_util(q, s);
    if (!newTail) {
        return false;
    }

    if (!q->head)
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
    if (!q || 0 == q->q_size || !q->head)
        return false;
    list_ele_t *del = q->head;

    if (del == q->tail) {
        q->tail = NULL;
    }

    q->head = q->head->next;

    if (sp) {
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
    /* Remember: It should operate in O(1) time */
    return q ? q->q_size : 0;
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
    if (!q || q->q_size == 0)
        return;

    list_ele_t *tmp = q->tail;
    q->tail = q->head;
    list_ele_t *cur = q->head;
    q->head = tmp;
    tmp = NULL;
    while (cur) {
        list_ele_t *next = cur->next;
        cur->next = tmp;
        tmp = cur;
        cur = next;
    }
    return;
}

/*
 * Compare function for sorting in ascending order

bool __q_sort_cmp(const char *const v1, const char *const v2)
{
    return strcmp(v1, v2) < 0;
}
*/

/*
 * Compare function for sorting in natural order
 */
bool __q_sort_nat_cmp(const char *const v1, const char *const v2)
{
    return strnatcmp(v1, v2) < 0;
}

/*
 * Merge two splited list by cmp()
 */
void __q_sort_merge(list_ele_t **n,
                    list_ele_t *l,
                    list_ele_t *r,
                    bool (*cmp_cb)(const char *const, const char *const))
{
    list_ele_t **merge = n, **next = NULL;
    while (l && r) {
        if (cmp_cb(l->value, r->value))
            next = &l;
        else
            next = &r;

        *merge = *next;
        merge = &((*merge)->next);
        *next = (*next)->next;
    }

    if (l)
        *merge = l;
    else if (r)
        *merge = r;

    return;
}

/*
 * merge sort
 */
list_ele_t *__q_sort(list_ele_t *head)
{
    if (!head || !(head->next))
        return head;

    // split list into two sublist
    list_ele_t *slow = head, *fast = head->next;
    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    list_ele_t *l = head, *r = slow->next;
    slow->next = NULL;

    l = __q_sort(l);
    r = __q_sort(r);

    __q_sort_merge(&head, l, r, __q_sort_nat_cmp);
    return head;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || 1 >= q->q_size)
        return;

    q->head = __q_sort(q->head);

    // determine new tail
    list_ele_t *p = q->head;
    while (p) {
        q->tail = p;
        p = p->next;
    }

    return;
}
