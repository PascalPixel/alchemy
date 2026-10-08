#include "types.h"

void Func_08143eb4(s32 x0, s32 y0, s32 x1, s32 y1, u8 color);

void Func_08192828(s32 x, s32 y, s32 w, u8 color)
{
    s32 h = w >> 1;

    x += h;
    y -= h;
    if (h > 48)
        h = 48;
    Func_08143eb4(x, y, x - h, y + h, color);
}
