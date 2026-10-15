/* Draft, differ 2 of 43: the native loads the render work right after the
 * heap slot base (movs r3, #192; lsls r3, #18; ldr r5, [r3, #60]; movs r2,
 * #152), this starts the result offset 0x1384 before the load. Measured: a
 * barrier or one-pass block after the load (differ 6, the stack adjustment
 * and the argument copy move up instead), the result stores in either order,
 * through a pointer, as one word or from a zero temporary (2 to 7); two
 * minutes of permuter found nothing. */
#include "types.h"
#include "window.h"
#include "ram_buffer.h"

#define UI_WORK ((struct UiRenderWork *)Ram_HeapSlots->window_tiles)

s32 UiText_BuildRenderEntries(s32 key, s32 mode);
s32 UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, u16 *bounds, s32 mode);

s32 UiText_OpenEntryMessage(s32 window, s32 argument)
{
    struct UiRenderWork *work = UI_WORK;
    s32 entry;
    s32 result = 0;
    /* FAKEMATCH: never read: the native frame keeps 8 bytes above the two outgoing arguments, exactly these four slots; without them the frame is 8 bytes, not 16 (differ 4 in tbs-en). */
    u16 entries[4];

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
