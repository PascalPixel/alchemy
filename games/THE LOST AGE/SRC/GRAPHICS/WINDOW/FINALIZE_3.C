#include "RENDER_INPUT.H"

void RenderOutput_Release(struct RenderOutput *node);

void RenderOutput_ClearList(void *work)
{
    struct RenderOutputList *list = work;
    struct RenderOutput *node;
    struct RenderOutput *next;

    if (list != NULL) {
        node = list->head;
        list->tail_link = &list->head;
        list->head = NULL;
        while (node != NULL) {
            next = node->next;
            RenderOutput_Release(node);
            node = next;
        }
    }
}
