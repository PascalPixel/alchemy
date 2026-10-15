#include "types.h"
#include "window.h"
#include "ram_buffer.h"

void WaitFrames(s32 frames);
s32 UiWork_IsComplete(void);
void UiMessage_Show(s32 message);
s32 UiText_BuildRenderEntries(s32 key, s32 mode);
s32 UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, u16 *bounds, s32 mode);

void UiMessage_ShowAndWait(s32 message)
{
    while (UiWork_IsComplete() == 0)
        WaitFrames(1);
    UiMessage_Show(message);
    while (UiWork_IsComplete() == 0)
        WaitFrames(1);
}

// Builds the render entries of a message and queues them in the window;
// returns 0 when the message is empty or there is no window.
s32 UiText_OpenEntryMessage(s32 window, s32 argument)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    s32 entry;
    s32 result = 0;
    /* FAKEMATCH: never read: the native frame keeps 8 bytes above the two outgoing arguments, exactly these four slots; without them the frame is 8 bytes, not 16 (differ 4). */
    u16 entries[4];

    /* FAKEMATCH: ⚓️ loads the render work right after the heap slot base, before it starts the result offset 0x1384 in r2; the empty asm holds r2 until the work is loaded. Plainly the offset is started first (differ 2); a one-pass block or a bare barrier after the load moves the stack adjustment and the argument copies up instead (differ 6). */
    asm("" : "+l"(work) : : "r2");
    work->result[0] = 0;
    work->result[1] = 0;
    entry = UiText_BuildRenderEntries(argument, 1);
    if (work->entries[entry] == 0)
        return 0;
    if (window == 0)
        return 0;
    result = (s32)UiText_QueueRenderEntries((struct UiWindow *)window, entry, 0, 0, 0, 1);
    if (result == 0)
        return 0;
    return result;
}
