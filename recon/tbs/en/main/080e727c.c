/* Draft, not exact (2026-09-24): candidate=98 reference=100 differing_halfwords=7.
   The halfword mask constant reproduces the mid-function pool; residual: the
   mask and the loop counter take r6 and r7 the other way round.
   2026-09-26 bounded aggregate hypothesis: a one-halfword mask struct
   compiles byte-identically to the scalar, still 98/100 bytes and seven
   differing halfwords. It changes neither allocation nor pool placement;
   this structural family is closed, with no new byte credit. */
#include "TYPES.H"

extern u8 Value_0000001f;

struct PaletteChannelMask {
    u16 value;
};

/* Brighten background palette entries 1-63 by per-channel deltas, clamping
   each 5-bit channel at 31. */
void Palette_BrightenBgEntries(s32 blue_delta, s32 green_delta, s32 red_delta)
{
    u16 *color = (u16 *)0x05000002;
    s32 i;
    /* FAKEMATCH: a halfword aggregate preserves the mask's short pool reach. */
    struct PaletteChannelMask mask;

    mask.value = (u16)(u32)&Value_0000001f;
    i = 0;
    do {
        u16 c = *color;
        u32 shifted = (u32)c << 16;
        s32 blue = (shifted >> 26) & mask.value;
        s32 green = (shifted >> 21) & mask.value;
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
