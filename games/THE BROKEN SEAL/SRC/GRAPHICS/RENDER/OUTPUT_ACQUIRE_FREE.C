#include "TYPES.H"
#include "WINDOW.H"

/* Detach the first free output; an empty tail names the head link. */
struct RenderOutput *RenderOutput_AcquireFree(void)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
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
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];

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

    work = (struct UiRenderWork *)gWindowWork[0];
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
