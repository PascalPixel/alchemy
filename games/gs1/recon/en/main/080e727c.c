#include "types.h"

void Func_080e727c(s32 blue_delta, s32 green_delta, s32 red_delta)
{
    u16 *color = (u16 *)0x05000002;
    s32 mask = 31;
    s32 i = 0;

    do {
        u16 c = *color;
        u32 shifted = (u32)c << 16;
        s32 blue = (shifted >> 26) & mask;
        s32 green = (shifted >> 21) & mask;
        s32 red = c & 31;
        u16 value;

        blue += blue_delta;
        if (blue > 31) {
            blue = 31;
        }
        green += green_delta;
        if (green > 31) {
            green = 31;
        }
        red += red_delta;
        if (red > 31) {
            red = 31;
        }

        value = (blue << 10) | (green << 5) | red;
        i++;
        *color = value;
        color++;
    } while (i != 63);
}
