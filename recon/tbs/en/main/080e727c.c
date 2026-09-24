/* Draft, not exact (2026-09-24): candidate=98 reference=100 differing_halfwords=7.
   The halfword mask constant reproduces the mid-function pool; residual: the
   mask and the loop counter take r6 and r7 the other way round. */
#include "TYPES.H"

extern u8 Value_0000001f;

/* Brighten background palette entries 1-63 by per-channel deltas, clamping
   each 5-bit channel at 31. */
void Palette_BrightenBgEntries(s32 blue_delta, s32 green_delta, s32 red_delta)
{
    u16 *color = (u16 *)0x05000002;
    s32 i;
    u16 mask = (u16)(u32)&Value_0000001f; /* FAKEMATCH: a halfword pool constant places the pool mid-function */

    i = 0;
    do {
        u16 c = *color;
        u32 shifted = (u32)c << 16;
        s32 blue = (shifted >> 26) & mask;
        s32 green = (shifted >> 21) & mask;
        s32 red = c & 31;

        blue += blue_delta;
        green += green_delta;
        red += red_delta;
        if (blue > 31)
            blue = 31;
        if (green > 31)
            green = 31;
        if (red > 31)
            red = 31;
        i++;
        *color = (blue << 10) | (green << 5) | red;
        color++;
    } while (i != 63);
}
