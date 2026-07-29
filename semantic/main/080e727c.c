#include "types.h"

extern u8 Value_0000001f;

void Func_080e727c(s32 red_add, s32 green_add, s32 blue_add) {
    s32 index;
    volatile u16 *palette = (volatile u16 *)0x05000002;
    u32 mask = (u32)&Value_0000001f;

    index = 0;
    do {
        u32 color = *palette;
        u32 expanded = (u16)color << 16;
        s32 red = (expanded >> 26) & mask;
        s32 green = (expanded >> 21) & mask;
        s32 blue = color & mask;

        red += red_add;
        green += green_add;
        blue += blue_add;
        if (red > 31)
            red = 31;
        if (green > 31)
            green = 31;
        if (blue > 31)
            blue = 31;

        index++;
        *palette++ = (red << 10) | (green << 5) | blue;
    } while (index != 63);
}
