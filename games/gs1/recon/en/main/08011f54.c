#include "types.h"
#include "global_cells.h"

extern u8 Data_0202c001[];
extern s32 Data_080134fc[];

s32 Func_080072f0(u8 *, s32, s32, s32);

s32 Func_08011f54(s32 index, s32 x, s32 y)
{
    u8 *table = *(u8 **)ADDR_03001E70;
    u8 *map;
    s32 offset;
    s32 column;
    s32 row;
    u32 entry;
    u8 config;
    s32 target;
    y >>= 16;

    x >>= 16;
    map = (u8 *)0x02010000;
    if (table != 0) {
        offset = (index & 3) * 48 + 304;
        map = *(u8 **)(table + offset);
    }
    column = x / 16;
    row = y / 16;
    map += (column + (row << 7)) * 4;
    entry = map[3];
    config = *(u8 *)(0x0202c000 + entry * 4) & 15;
    x &= 15;
    y &= 15;
    target = Data_080134fc[config];
    return Func_080072f0(&Data_0202c001[entry * 4], x, y, target);
}
