#include "TYPES.H"

/* Brighten background palette entries 1-63 by per-channel deltas, clamping
   each 5-bit channel at 31. */

void Palette_BrightenBgEntries(s32 blue_delta, s32 green_delta, s32 red_delta)
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
        *color++ = (blue << 10) | (green << 5) | red;
    }
}
