#include "IWRAM_CALL.H"

#define Color_ScaleComponents Func_0801cc50

u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale, s32 blue_scale)
{
    s32 red;
    s32 green;
    s32 blue;

    red = Iwram_MulQ16(rgb[0] << 16, red_scale) >> 16;
    green = Iwram_MulQ16(rgb[1] << 16, green_scale) >> 16;
    blue = Iwram_MulQ16(rgb[2] << 16, blue_scale) >> 16;
    if (red < 0)
        red = 0;
    if (red > 31)
        red = 31;
    if (green < 0)
        green = 0;
    if (green > 31)
        green = 31;
    if (blue < 0)
        blue = 0;
    if (blue > 31)
        blue = 31;
    return red + ((blue << 10) + (green << 5));
}
