#include "types.h"

extern s32 Func_03000380(s32 channel, s32 amount);

u16 Func_020017d0(u16 colour, s32 amount)
{
    s32 red = colour & 31;
    s32 green = (colour >> 5) & 31;
    s32 blue = (colour >> 10) & 31;

    red += Func_03000380(red, amount << 2);
    green -= Func_03000380(green, amount);
    blue -= Func_03000380(blue, amount);
    if (red > 31)
        red = 31;

    return (u16)((blue << 10) | (green << 5) | red);
}
