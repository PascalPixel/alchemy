#include "types.h"

struct Node_080f9a30;

struct List_080f9a30 {
    u8 unknown_00[32];
    struct Node_080f9a30 *head;
};

struct Node_080f9a30 {
    u8 unknown_00[44];
    struct List_080f9a30 *list;
    struct Node_080f9a30 *previous;
    struct Node_080f9a30 *next;
};

/*
 * Unlink a node from the doubly linked list it belongs to and mark it free.
 * A node whose list pointer is already clear is not linked and is left alone;
 * unlinking the first node moves the list head to its successor.
 * Called from 0x080f9a6a and published as a callback at 0x080fb828.
 */
void Func_080f9a30(struct Node_080f9a30 *node)
{
    struct List_080f9a30 *list = node->list;
    struct Node_080f9a30 *next;
    struct Node_080f9a30 *previous;

    if (list == 0)
        return;

    next = node->next;
    previous = node->previous;

    if (previous != 0)
        previous->next = next;
    else
        list->head = next;

    if (next != 0)
        next->previous = previous;

    node->list = 0;
}
