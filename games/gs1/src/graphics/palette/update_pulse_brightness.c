#include "types.h"

#define Palette_UpdatePulseBrightness Func_080b8f58

s32 Func_0800231c(s32);
s32 FixedPoint_Ratio(s32, s32);
extern s32 Data_03001e40;

void Palette_UpdatePulseBrightness(void) {
    u16 *palette;
    s32 i;
    s32 mask;

    palette = (u16 *)0x050001C0;
    mask = 31;
    i = 15;
    do {
        u32 color;
        s32 amount;
        s32 tick;
        s32 angle;
        s32 red;
        s32 green;
        s32 blue;

        tick = Data_03001e40;
        angle = (tick * 3) << 10;
        color = palette[16];
        amount = FixedPoint_Ratio(0x10000 - Func_0800231c(angle), 0x2AAA);

        red = (color >> 10) & mask;
        green = (color >> 5) & mask;
        blue = color & mask;
        red += amount;
        green += amount;
        blue += amount;

        if ((u32)red > 31)
            red = 31;
        if ((u32)green > 31)
            green = 31;
        if ((u32)blue > 31)
            blue = 31;

        *palette++ = (red << 10) | (green << 5) | blue;
        i--;
    } while (i >= 0);
}
