/* Draft, differ 2 of 53: The Broken Seal's Palette_BrightenBgEntries over
 * BG palette entries 1 to 63, with The Lost Age's clamp at 0. Left: in the
 * prologue the native loads the hoisted 0x1f mask from the pool between
 * the two halves of 0x05000000 (movs r5, #160; ldr r7; lsls r5); this
 * shifts first. Tried: an indexed palette, the mask as a const or plain
 * local, the pointer's start in a one-pass block or after a barrier, a
 * pre-incremented pointer, alchemy permute for 3 minutes (no candidate
 * better than differ 2). */
#include "types.h"

void Func_08164a4c(s32 blue_delta, s32 green_delta, s32 red_delta)
{
    u16 *color = (u16 *)0x05000002;
    s32 i;

    for (i = 0; i != 63; i++) {
        s32 blue = (*color >> 10) & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 red = *color & 0x1f;

        blue += blue_delta;
        green += green_delta;
        red += red_delta;
        if (blue > 31)
            blue = 31;
        if (green > 31)
            green = 31;
        if (red > 31)
            red = 31;
        if (blue < 0)
            blue = 0;
        if (green < 0)
            green = 0;
        if (red < 0)
            red = 0;
        *color++ = (blue << 10) | (green << 5) | red;
    }
}
