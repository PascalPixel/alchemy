/* Draft, differ 4 of 130: the prologue's sub sp, #36 is scheduled one
 * instruction early (before lsls r3, r3, #18 of the heap slot base) and the
 * store of y before the load of the render work; sched2 ties the stack
 * adjustment and the shift at priority 50 and takes the adjustment, which has
 * more dependants. Measured: the statement order of the first six lines, a
 * one-pass block or barrier after the work load (differ 8 to 32), the result
 * as a mask or u16 cast (4), other types for x, y, message and packed (4 to
 * 6); the permuter's only gain in three minutes was y declared long beside an
 * s32 x (differ 2, the store of y then follows the load). */
#include "types.h"
#include "window.h"
#include "ram_buffer.h"

#define UI_WORK ((struct UiRenderWork *)Ram_HeapSlots->window_tiles)

s32 UiText_BuildRenderEntries(s32 key, s32 mode);
void UiWindow_FitOnScreen(s32 entry, s32 *x, s32 *y, u32 *width, u32 *height, u16 *bounds, s32 mode);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs);
s32 UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, u16 *bounds, s32 mode);

struct UiWindow *UiText_OpenMessageWindow(s32 message, s32 x, s32 y, u32 packed)
{
    s32 entry;
    u32 height;
    u32 width;
    u16 bounds[4];
    s32 flags;
    u16 *out;
    struct UiWindow *window;
    struct UiRenderWork *work;

    work = UI_WORK;
    work->result[0] = (packed << 4) >> 20;
    window = NULL;
    work->result[1] = 0;
    packed &= 0xffff;
    flags = 0;
    entry = UiText_BuildRenderEntries(message, 1);
    if (work->entries[entry] == 0)
        return NULL;
    UiWindow_FitOnScreen(entry, &x, &y, &width, &height, (out = bounds), 0);
    if (width == 0 && height == 0)
        return NULL;
    if (!(packed & 1))
        flags |= 2;
    if (packed & 8)
        flags |= 8;
    if (packed & 16)
        flags |= 128;
    if (packed & 32)
        flags |= 256;
    window = UiWindow_Create(x, y, width, height, flags);
    if (window == NULL)
        return NULL;
    if (UiText_QueueRenderEntries(window, entry, 0, 0, out, 0) == 0) {
        UiWork_Finalize(window, 1);
        return NULL;
    }
    work->unknown_138a[0] = 0;
    work->unknown_138a[1] = 0;
    return window;
}
