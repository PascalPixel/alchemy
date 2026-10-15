#include "types.h"

void UiText_PlotGlyphRows(u16 *rows, u8 *dst, s32 color)
{
    s32 y;
    u8 *p;
    s32 bits;

    for (y = 0; y != 14; y++)
    {
        bits = *rows++ & ~3;
        p = dst;
        bits <<= 16;
        while (bits != 0)
        {
            if (bits < 0)
                *p = color;
            bits <<= 1;
            p++;
        }
        dst += 192;
    }
}
