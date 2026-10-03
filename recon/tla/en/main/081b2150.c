/* Near miss: 156 bytes, score 120 (two reordered instructions). Each loop
   seeds the red mask after the green shift; the listing seeds it immediately
   after the colour load. A 30-second permuter search and extraction-order,
   split-mask, pixel-sharing and dependency variants found no improvement. */
#include "TYPES.H"

/* Darken background entries 160-175 and object entries 1-239 by one step
   per channel, stopping each 5-bit channel at 0. */

void Palette_DarkenSceneStep(void)
{
    u16 *color;
    s32 i;

    color = (u16 *)0x05000140;
    for (i = 0; i != 16; i++) {
        s32 blue = (*color >> 10) & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 red = *color & 0x1f;

        blue--;
        green--;
        red--;

        if (blue < 0)
            blue = 0;
        if (green < 0)
            green = 0;
        if (red < 0)
            red = 0;
        *color++ = (blue << 10) | (green << 5) | red;
    }
    color = (u16 *)0x05000202;
    for (i = 0; i != 239; i++) {
        s32 blue = (*color >> 10) & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 red = *color & 0x1f;

        blue--;
        green--;
        red--;

        if (blue < 0)
            blue = 0;
        if (green < 0)
            green = 0;
        if (red < 0)
            red = 0;
        *color++ = (blue << 10) | (green << 5) | red;
    }
}
