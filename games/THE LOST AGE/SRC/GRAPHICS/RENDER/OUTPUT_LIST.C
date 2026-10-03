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
    if (list != NULL) {
        *list->tail_link = node;
        list->tail_link = (struct RenderOutput **)node;
    }
}
