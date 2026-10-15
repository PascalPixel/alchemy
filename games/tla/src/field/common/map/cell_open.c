#include "types.h"
#include "ram_buffer.h"

s32 Func_0802da24(s32 *position)
{
    s32 x = position[0] / 65536;
    s32 z = (u32)((position[2] & 0xfff00000) - position[1]) >> 16;
    u8 *map = (u8 *)Ram_HeapSlots->map_work;
    u8 *cell;

    if (map == NULL)
        return 0;
    cell = *(u8 **)(map + 0x1a8);
    cell += (x / 16 + (z / 16) * 128) * 4;
    return (cell[2] != 255) - 1;
}
