#include "render_input.h"
#include "output.h"
#include "resource.h"
#include "ram_buffer.h"
#include "edition.h"

#if EDITION_INTERNATIONAL
#define UI_PALETTE_OWNER_OFS 0x1360
#else
#define UI_PALETTE_OWNER_OFS 0x1160
#endif

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
    /* FAKEMATCH: the first-word tail pointer access preserves the two-store order. */
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
            u8 *dst = (u8 *)Ram_HeapSlots->window_tiles;
            s32 idx = entry->oam.palette * 2 + UI_PALETTE_OWNER_OFS;
            *(u16 *)(dst + idx) = 0x3E7;
        }
    }
    entry->active = 0;
}
