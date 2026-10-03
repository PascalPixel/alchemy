#include "RENDER_INPUT.H"

void UiWork_UpdateListTail(struct RenderOutputList *list)
{
    struct RenderOutput *node = list->head;
    struct RenderOutput **tail = &list->head;

    while (node != NULL) {
        tail = &node->next;
        node = node->next;
    }
    list->tail_link = tail;
}

void RenderOutput_AppendToList(struct RenderOutputList *list, struct RenderOutput *node)
{
    /* FAKEMATCH: retain the existing first-word pointer store at the tail
       cell. Assigning the typed tail-link field reverses the two stores in
       all six editions; the tail still names the next cell, including NULL. */
    if (list != NULL) {
        *list->tail_link = node;
        *(struct RenderOutput **)&list->tail_link = node;
    }
}
