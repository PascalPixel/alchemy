#include "TYPES.H"
#include "TBS_EDITION.H"
#include "DMA.H"
#include "RUNTIME_MEM.H"
#include "MENU_LIST.H"

struct CenteredTextWork {
    u8 unknown_000[RENDER_ENTRY_TBL_OFS];
    u16 entries[(RENDER_RESULT_OFS - RENDER_ENTRY_TBL_OFS) / 2];
    u16 cursor;
    u16 scroll;
};

struct TextRender;
s32 UiText_BuildRenderEntries(s32 message, s32 mode);
s32 UiText_GetResourceDimensions(s32 message, s32 *x, s32 *y, u32 *width, u32 *height);
struct TextRender *UiText_QueueRenderEntries(void *window, s32 entry,
    s32 x, s32 y, u16 *colours, s32 flags);
s32 UiWork_IsComplete(void);
s32 UiWork_IsIdle(void *window);

extern const u8 Func_08015570[];

/* The message lookup is ARM code (LOOKUP_SYMBOL.S) that runs from a heap copy
   of itself; the copy length is a link-time symbol. */
extern u8 UiText_LookupMessageCodeSize[];

void UiText_ShowCenteredMessage(s32 message, s32 mode, s32 y_offset)
{
    struct CenteredTextWork *work;
    struct UiWindow *window;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 entry;

    work = (struct CenteredTextWork *)gWindowWork[0];
    x = 8;
    y = 8;
    /* FAKEMATCH: the null window also supplies the zero style argument,
       retaining one register across the message and dimension lookups. */
    window = NULL;
    entry = UiText_BuildRenderEntries(message, 1);
    if (work->entries[entry] != 0) {
        UiText_GetResourceDimensions(message, &x, &y, (u32 *)&width, (u32 *)&height);
        x = (30 - width) >> 1;
        y = ((15 - height) >> 1) + y_offset;
        if (mode != 0)
            window = UiWindow_Create(x, y, width, height, (s32)window);
        else {
            window = UiWindow_Create(x, y, 0, 0, 2);
            window->width = mode;
            window->height = mode;
        }
        if (UiText_QueueRenderEntries(window, entry, 0, 0, 0, 0) == 0)
            UiWork_Finalize(window, 1);
        else {
            while (UiWork_IsComplete() == 0)
                WaitFrames(1);
            if (mode != 0) {
                UiWork_Finalize(window, 0);
                while (UiWork_IsIdle(window) == 0)
                    WaitFrames(1);
            } else
                UiWork_Finalize(window, 1);
            work->cursor = 0;
            work->scroll = 0;
        }
    }
}

s32 UiText_BuildRenderEntriesMode1(s32 arg0)
{
    return UiText_BuildRenderEntries(arg0, 1);
}

/* Positions a text reader at the start of a message. */
void UiText_LookupMessage(s32 first, s32 second)
{
    void (*routine)(s32, s32);
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)UiText_LookupMessageCodeSize;
    } while (0);
    routine = (void (*)(s32, s32))Runtime_BumpAllocate(size);
    Dma_Set((const void *)Func_08015570, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    routine(first, second);
    Sys_Free(routine);
}
