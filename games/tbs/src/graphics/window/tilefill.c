#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#if EDITION_INTERNATIONAL
#include "types.h"
#include "window.h"
#include "callback_scheduler.h"
#include "tbs_edition.h"
#include "global_cells.h"


void Scheduler_ScheduleCallbackAAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback(PaletteGlow_UpdateSine, 0xC80);
}

void Scheduler_ScheduleCallbackA(void)
{
    Scheduler_RemoveCallback(PaletteGlow_UpdateSine);
}

void UiWindow_FillTilemapRect(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height)
{
    u16 *map = (u16 *)gWindowWork[0];
    s32 sum;

    sum = x + window->x;
    x = sum + 1;
    sum = y + ((struct UiWindow *)window)->y;
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
        s32 offset = y_ofs + x_ofs;
        do {
            s32 remaining = width;
            u16 *cell = (u16 *)(offset + (s32)map);
            if (remaining != 0) {
                do {
                    remaining--;
                    *cell++ = 0xE006;
                } while (remaining != 0);
            }
            height--;
            offset += 64;
        } while (height != 0);
        ((struct UiRenderWork *)map)->dirty = 1;
    }
}

#endif
