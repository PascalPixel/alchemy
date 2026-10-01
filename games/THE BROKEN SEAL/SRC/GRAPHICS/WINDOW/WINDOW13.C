#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "TBS_EDITION.H"
#include "GLOBAL_CELLS.H"

void PaletteGlow_UpdateSine(void);

extern u8 Data_03001e8c[];

void Scheduler_ScheduleCallbackAAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback((s32)PaletteGlow_UpdateSine, 0xC80);
}

void Scheduler_ScheduleCallbackA(void)
{
    Scheduler_RemoveCallback((u32)((s32)PaletteGlow_UpdateSine));
}

void UiWindow_FillTilemapRect(u8 *window, s32 x, s32 y, s32 width, s32 height)
{
    u16 *map = *(u16 **)((u32)&Data_03001e8c);
    s32 sum;

    sum = x + *(u16 *)(window + 12);
    x = sum + 1;
    sum = y + *(u16 *)(window + 14);
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
        ((u8 *)map)[RENDER_DIRTY_OFS] = 1;
    }
}
