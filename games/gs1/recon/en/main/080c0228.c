#include "types.h"

#define VRAM_TILEMAP ((volatile u16 *)0x06006000)

extern s32 *Data_03001ef8;

void Func_080c0228(void)
{
    s32 value;
    s32 tile;
    s32 row;
    s32 div;
    s32 i;
    volatile u16 *dst;

    value = *Data_03001ef8;
    if ((u32)value > 79)
        return;

    div = value;
    tile = (value & 7) + 0xf081;
    if (div < 0)
        div += 7;
    div >>= 3;
    row = 13 - div;
    dst = VRAM_TILEMAP + row * 32;
    i = 0;
    do {
        i++;
        *dst = (u16)tile;
        dst++;
    } while (i != 32);

    tile |= 0x800;

    div = (value >= 0) ? value : value + 7;
    div >>= 3;
    row = div + 13;
    if ((u32)row > 20)
        return;
    dst = VRAM_TILEMAP + row * 32;
    i = 0;
    do {
        i++;
        *dst = (u16)tile;
        dst++;
    } while (i != 32);
}
