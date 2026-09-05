#include "types.h"
#include "global_cells.h"

#define Map_GetCellAttributeLowNibble Func_08011fd8

s32 Map_GetCellAttributeLowNibble(s32 index, s32 x, s32 y)
{
    u8 *state = *(u8 **)ADDR_03001E70;
    u8 *map;
    s32 off;
    s32 col;
    s32 row;
    u32 attr;

    x >>= 16;
    y >>= 16;
    map = (u8 *)0x02010000;
    if (state != 0) {
        off = (index & 3) * 48 + 304;
        map = *(u8 **)(state + off);
    }
    col = x / 16;
    row = y / 16;
    map += (col + (row << 7)) * 4;
    attr = map[3];
    return *(u8 *)(0x0202c000 + attr * 4) & 15;
}
