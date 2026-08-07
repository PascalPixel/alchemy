#include "types.h"

s32 Func_08011fd8(s32 index, s32 x, s32 y)
{
    u8 *table = *(u8 **)0x03001e70;
    u8 *map;
    s32 offset;
    s32 column;
    s32 row;
    u32 entry;

    x >>= 16;
    y >>= 16;
    map = (u8 *)0x02010000;
    if (table != 0) {
        offset = (index & 3) * 48 + 304;
        map = *(u8 **)(table + offset);
    }
    column = x / 16;
    row = y / 16;
    map += (column + (row << 7)) * 4;
    entry = map[3];
    return *(u8 *)(0x0202c000 + entry * 4) & 15;
}
