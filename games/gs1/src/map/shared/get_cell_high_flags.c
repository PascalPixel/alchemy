#include "types.h"

#define Map_GetCellHighFlags Func_080120b4

s32 Map_GetCellHighFlags(s32 x, s32 y)
{
    s32 tile_x = x / 16;
    s32 tile_y = y / 16;
    u8 *cell = (u8 *)0x02010000 + (tile_x + tile_y * 128) * 4;

    return cell[1] >> 6;
}
