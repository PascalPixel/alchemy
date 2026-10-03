#include "TYPES.H"
#include "WINDOW.H"

/* Detach and return the head of the free list. A pointer-typed sentinel
   store adds a reload in the 52-byte body; retain its scalar word store. */
struct RenderOutput *RenderOutput_AcquireFree(void)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    struct RenderOutput *entry = work->free_head;

    if (entry != NULL) {
        if (entry->next == NULL)
            *(s32 *)&work->free_tail = (s32)&work->free_head;
        work->free_head = entry->next;
        entry->next = NULL;
    }
    return entry;
}

/* Only the records in this work block belong to its free list. */
void RenderOutput_ReleaseFree(struct RenderOutput *entry)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];

    if ((u32)entry >= (u32)work->outputs
        && (u32)entry < (u32)&work->free_head) {
        struct RenderOutput *tail = work->free_tail;

        work->free_tail = entry;
        tail->next = entry;
        entry->next = NULL;
    }
}

void UiWork_InitFreeList(void)
{
    s32 count;
    struct UiRenderWork *work;
    struct RenderOutput *entry;
    struct RenderOutput *next;

    work = (struct UiRenderWork *)gWindowWork[0];
    entry = work->outputs;
    work->free_head = entry;
    count = 0x3e;
    do {
        next = entry + 1;
        count--;
        entry->next = next;
        entry = next;
    } while (count >= 0);
    next->next = NULL;
    work->free_tail = next;
}
