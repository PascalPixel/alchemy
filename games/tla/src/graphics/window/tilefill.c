#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "callback_scheduler.h"
#include "ram_buffer.h"
#include "window.h"


void Scheduler_ScheduleCallbackAAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback((s32)PaletteGlow_UpdateSine, 0x480);
}

void Scheduler_ScheduleCallbackA(void)
{
    Scheduler_RemoveCallback((u32)((s32)PaletteGlow_UpdateSine));
}

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
