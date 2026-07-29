#include "types.h"

extern u8 Value_0000001f;

void Func_080a2144(s32 paletteIndex)
{
    u16 *palette = (u16 *)(0x05000000 + paletteIndex * 32);
    u32 destination = (u32)palette;
    volatile u32 *dma = (u32 *)0x040000d4;
#if defined(CHANNEL_ORDER_BGR)
    u32 blue;
    u32 green;
    u32 red;
#elif defined(CHANNEL_ORDER_BRG)
    u32 blue;
    u32 red;
    u32 green;
#elif defined(CHANNEL_ORDER_GRB)
    u32 green;
    u32 red;
    u32 blue;
#elif defined(CHANNEL_ORDER_GBR)
    u32 green;
    u32 blue;
    u32 red;
#elif defined(CHANNEL_ORDER_RBG)
    u32 red;
    u32 blue;
    u32 green;
#else
    u32 red;
    u32 green;
    u32 blue;
#endif
    u32 color;
    u32 shifted;

    {
        u32 source = 0x050001e0;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = 0x80000010;
    }
    {
        u32 source = 0x050001e0;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = 0x84000008;
    }

    color = palette[4];
    shifted = color << 16;
    red = shifted >> 26;
    green = (shifted >> 21) & (u32)&Value_0000001f;
    blue = color & 31;
    red += 9;
    if (red > 31)
        red = 31;
    green += 9;
    if (green > 31)
        green = 31;
    blue += 9;
    if (blue > 31)
        blue = 31;
    palette[4] = (red << 10) | (green << 5) | blue;
}
