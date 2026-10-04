/*
 * Draft: UiWindow_FillTilemapRect (☀️ WINDOW13.C) does not yet match:
 * differ 2. The native loads height from the stack right after the first
 * ldrsh; this build loads it after the second. "+r" on height and 300
 * devsearch steps did not move it. Joins WINDOW13.C after
 * Scheduler_ScheduleCallbackA when it matches.
 * Links as recon/tla/raw/080439e8.s.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "WINDOW.H"

void UiWindow_FillTilemapRect(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    s32 sum;

    sum = x + (s16)window->x;
    x = sum + 1;
    sum = y + (s16)window->y;
    y = sum + 1;

    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29)
        width = 30 - x;

    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29)
        height = 20 - y;

    if (width > 0 && height > 0) {
        s32 y_ofs = y << 6;
        s32 x_ofs = x << 1;
        s32 offset = y_ofs + ((s32)work + x_ofs);
        do {
            s32 remaining = width;
            u16 *cell = (u16 *)(offset + 8);
            if (remaining != 0) {
                do {
                    remaining--;
                    *cell++ = 0xE006;
                } while (remaining != 0);
            }
            height--;
            offset += 64;
        } while (height != 0);
        work->dirty = 1;
    }
}
