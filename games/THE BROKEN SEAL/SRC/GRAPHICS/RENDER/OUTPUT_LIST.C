#include "TYPES.H"
#include "RENDER_INPUT.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
extern u8 Data_03001e8c[];

struct RenderOutputList {
    struct RenderOutput *next;
    struct RenderOutput *tail;
};

void RenderOutput_ReleaseFree(struct RenderOutput *entry);
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
    RenderOutput_ReleaseFree(entry);
    if ((u8)entry->kind != 0) {
        Resource_ResetEntry((u8)entry->index);
        if ((u8)entry->kind == 2) {
            u8 *dst = (u8 *)(*(s32 *)((u32)&Data_03001e8c));
            s32 idx = ((u32)((u8 *)&entry->table)[1] >> 4) * 2 + RENDER_PALETTE_TBL_OFS;
            *(u16 *)(dst + idx) = 0x3E7;
        }
    }
    entry->active = 0;
}
