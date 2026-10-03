#include "TYPES.H"
#include "RENDER_INPUT.H"
#include "RESOURCE.H"

void RenderOutput_ReleaseFree(struct RenderOutput *entry);
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
extern u8 Data_03001e8c[];

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
