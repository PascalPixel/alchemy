/* Draft, differ 12 of 65: as Func_08164abc from the battle work's saved
 * palette (BtlWork.palette) into BG palette entries 96 to 223. The loop
 * matches; left is the prologue's order: the native copies the first
 * delta, the third, then loads the battle work and starts 0x56c and the
 * pooled 0x1f mask before 0x050000c0. */
#include "types.h"
#include "btlwork.h"

void Func_08164b2c(s32 red_delta, s32 green_delta, s32 blue_delta)
{
    u16 *source = Ram_HeapSlots->battle_work->palette;
    u16 *color = (u16 *)0x050000c0;
    s32 i;

    for (i = 0; i != 128; i++) {
        s32 blue = (*source >> 10) & 0x1f;
        s32 green = (*source >> 5) & 0x1f;
        s32 red = *source & 0x1f;

        red += red_delta;
        green += green_delta;
        blue += blue_delta;
        if (red > 31)
            red = 31;
        if (green > 31)
            green = 31;
        if (blue > 31)
            blue = 31;
        if (red < 0)
            red = 0;
        if (green < 0)
            green = 0;
        if (blue < 0)
            blue = 0;
        *color++ = (blue << 10) | (green << 5) | red;
        source++;
    }
}
