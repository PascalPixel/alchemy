#include "types.h"

extern u8 *Data_03001e8c;

void Func_0801e260(s32 x, s32 y, u32 width, u32 height);

void Func_08016178(u32 x, u32 y, u32 width, u32 height)
{
    u8 *base;
    u8 *cursor;
    u32 offset;
    u32 savedY;
    u32 tile;
    u32 row;

    base = Data_03001e8c;
    offset = y << 5;
    offset += x;
    offset <<= 1;
    cursor = base + offset;
    tile = 0xF000;

    if (y + height > 20)
        height = 20 - y;
    savedY = y;

    if (width <= 1)
        width = 2;
    if (width > 30)
        width = 30;

    if (height <= 1)
        height = 2;
    if (height > 30)
        height = 30;

    Func_0801e260(x, savedY, width, height);

    for (row = 0; row < height; row++) {
        u32 column;

        if (base[0xEA5] != 0) {
            tile = 0xF07F;
            if (savedY + row <= 16)
                tile = 0xF000;
        }

        for (column = 0; column < width; column++) {
            *(u16 *)cursor = tile;
            cursor += 2;
        }

        cursor += (32 - width) * 2;
    }

    base[0xEA3] = 1;
}
