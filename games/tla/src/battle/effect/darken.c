#include "types.h"

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
