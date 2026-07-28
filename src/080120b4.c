#include "types.h"

s32 Func_080120b4(s32 x, s32 y)
{
    s32 tile_x = x / 16;
    s32 tile_y = y / 16;
    u8 *entry = (u8 *)0x02010000 + (tile_x + tile_y * 128) * 4;

    return entry[1] >> 6;
}
