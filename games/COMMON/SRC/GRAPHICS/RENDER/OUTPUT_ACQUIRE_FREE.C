#include "TYPES.H"
#include "EDITION.H"
#include "WINDOW.H"
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#include "RAM_BUFFER.H"
#endif

/* Detach the first free output; an empty tail names the head link. */
struct RenderOutput *RenderOutput_AcquireFree(void)
{
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    struct UiRenderWork *work =
        (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
#else
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
#endif
    struct RenderOutput *entry = work->free_outputs.head;

    if (entry != NULL) {
        if (entry->next == NULL)
            work->free_outputs.tail_link = &work->free_outputs.head;
        work->free_outputs.head = entry->next;
        entry->next = NULL;
    }
    return entry;
}

/* Only the records in this work block belong to its free list. */
void RenderOutput_ReleaseFree(struct RenderOutput *entry)
{
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    struct UiRenderWork *work =
        (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
#else
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
#endif

    if ((u32)entry >= (u32)work->outputs
        && (u32)entry < (u32)&work->free_outputs.head) {
        struct RenderOutput **tail = work->free_outputs.tail_link;

        work->free_outputs.tail_link = &entry->next;
        *tail = entry;
        entry->next = NULL;
    }
}

void UiWork_InitFreeList(void)
{
    s32 count;
    struct UiRenderWork *work;
    struct RenderOutput *entry;
    struct RenderOutput *next;

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
#else
    work = (struct UiRenderWork *)gWindowWork[0];
#endif
    entry = work->outputs;
    work->free_outputs.head = entry;
    count = UI_OUTPUT_COUNT - 2;
    do {
        next = entry + 1;
        count--;
        entry->next = next;
        entry = next;
    } while (count >= 0);
    next->next = NULL;
    work->free_outputs.tail_link = &next->next;
}
