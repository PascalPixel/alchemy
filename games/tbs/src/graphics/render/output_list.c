#include "types.h"
#include "render_input.h"
#include "resource.h"
#include "output.h"

#include "global_cells.h"
#include "tbs_edition.h"
extern u8 *gWindowWork[];

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
    /* FAKEMATCH: needed to match: the first-word tail store preserves the two stores in order; the typed tail-link field reverses them. */
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
            u8 *dst = gWindowWork[0];
            s32 idx = (((u8 *)&entry->table)[1] >> 4) * 2 + RENDER_PALETTE_TBL_OFS;
            *(u16 *)(dst + idx) = 0x3E7;
        }
    }
    entry->active = 0;
}
