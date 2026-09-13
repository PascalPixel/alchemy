#include "types.h"
#include "global_cells.h"

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

void RenderOutput_ReleaseFree(u32 arg0);
s32 Resource_ResetEntry(u32 index);

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

void RenderOutput_Release(struct RenderOutput *entry)
{
    RenderOutput_ReleaseFree((u32)entry);
    if (entry->kind != 0) {
        Resource_ResetEntry(entry->index);
        if (entry->kind == 2) {
            u8 *dst = (u8 *)(*(s32 *)ADDR_03001E8C);
            s32 idx = ((u32)entry->palette >> 4) * 2 + 0x12D0;
            *(u16 *)(dst + idx) = 0x3E7;
        }
    }
    entry->active = 0;
}
