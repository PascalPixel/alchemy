#include "types.h"

u16 Func_0803f974(s16 *rgb, s32 scale_r, s32 scale_g, s32 scale_b)
{
    s32 r = rgb[0] * scale_r >> 16;
    s32 g = rgb[1] * scale_g >> 16;
    s32 b = rgb[2] * scale_b >> 16;

    if (r < 0)
        r = 0;
    if (r > 31)
        r = 31;
    if (g < 0)
        g = 0;
    if (g > 31)
        g = 31;
    if (b < 0)
        b = 0;
    if (b > 31)
        b = 31;
    r |= b << 10 | g << 5;
    return r;
}
