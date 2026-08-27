#include "types.h"

void Func_080f6148(void)
{
    u16 *pltt;
    s32 col;
    s32 mask;

    pltt = (u16 *)0x05000140;
    mask = 31;
    for (col = 0; col < 16; col++) {
        u32 color = pltt[col];
        s32 blue = (color >> 10) & mask;
        s32 green = (color >> 5) & mask;
        s32 red = color & mask;
        blue -= 1;
        green -= 1;
        red -= 1;
        if (blue < 0)
            blue = 0;
        if (green < 0)
            green = 0;
        if (red < 0)
            red = 0;
        pltt[col] = (blue << 10) | (green << 5) | red;
    }

    pltt = (u16 *)0x05000202;
    mask = 31;
    for (col = 0; col < 239; col++) {
        u32 color = pltt[col];
        s32 blue = (color >> 10) & mask;
        s32 green = (color >> 5) & mask;
        s32 red = color & mask;
        blue -= 1;
        green -= 1;
        red -= 1;
        if (blue < 0)
            blue = 0;
        if (green < 0)
            green = 0;
        if (red < 0)
            red = 0;
        pltt[col] = (blue << 10) | (green << 5) | red;
    }
}
