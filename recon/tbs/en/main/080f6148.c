/* Draft, not exact (2026-09-24): 158 of 160 bytes, 65 halfwords off in
   the offset view; both loops match instruction for instruction except
   that the mask and the counter take r6 and r7 the other way round (as in
   Palette_BrightenBgEntries, 080e727c) and the mid-loop pool lands one
   clamp earlier. The halfword mask constant is the Value_0000001f
   FAKEMATCH from that draft; declaration and assignment orders do not
   swap the registers. */
#include "TYPES.H"

extern u8 Value_0000001f;

void Unnamed_080f6148(void)
{
    u16 *color;
    s32 i;
    u16 mask;

    color = (u16 *)0x05000140;
    mask = (u16)(u32)&Value_0000001f;
    i = 0;
    do {
        u16 c = *color;
        u32 shifted = (u32)c << 16;
        s32 blue = (shifted >> 26) & mask;
        s32 green = (shifted >> 21) & mask;
        s32 red = c & 31;

        blue--;
        green--;
        red--;
        if (blue < 0)
            blue = 0;
        if (green < 0)
            green = 0;
        if (red < 0)
            red = 0;
        i++;
        *color = (blue << 10) | (green << 5) | red;
        color++;
    } while (i != 16);
    color = (u16 *)0x05000202;
    mask = (u16)(u32)&Value_0000001f;
    i = 0;
    do {
        u16 c = *color;
        u32 shifted = (u32)c << 16;
        s32 blue = (shifted >> 26) & mask;
        s32 green = (shifted >> 21) & mask;
        s32 red = c & 31;

        blue--;
        green--;
        red--;
        if (blue < 0)
            blue = 0;
        if (green < 0)
            green = 0;
        if (red < 0)
            red = 0;
        i++;
        *color = (blue << 10) | (green << 5) | red;
        color++;
    } while (i != 239);
}
