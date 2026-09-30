#include "TYPES.H"

struct RenderOutput {
    struct RenderOutput *next;
    u8 kind;
    s8 active;
    u8 unk06[8];
    u8 index;
    u8 unk0f[10];
    u8 palette;
};

struct RenderOutputList {
    struct RenderOutput *next;
    struct RenderOutput *tail;
};

void UiWork_UpdateListTail(struct RenderOutputList *list)
{
    struct RenderOutput *prev;
    struct RenderOutput *node;

    node = list->next;
    prev = (struct RenderOutput *)list;
    if (node != NULL) {
        do {
            prev = node;
            node = prev->next;
        } while (node != NULL);
    }
    list->tail = prev;
}

void RenderOutput_AppendToList(struct RenderOutputList *list, struct RenderOutput *node)
{
    if (list != NULL) {
        list->tail->next = node;
        list->tail = node;
    }
}
