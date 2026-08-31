#include "types.h"

#define BattleEffect_InitializeTilemap Func_080cdb24

void BattleEffect_InitializeTilemap(u32 display_control)
{
    s32 row;
    s32 tile;
    s32 palette;
    s32 first_column;
    s32 second_column;
    s32 row_tile;
    s32 tile_index;
    u8 *offset;

    Func_080cd594();
    *(volatile u16 *)0x0400000c = display_control | 0x6784;

    offset = (u8 *)0;
    row = 0;
    row_tile = 0;
    tile_index = 0;
    do {
        tile = row_tile + 0x100;
        palette = tile_index << 1;
        first_column = 0;
        do {
            first_column++;
            *(s16 *)(0x06003800 + (s32)offset) = tile | palette;
            tile += 0x200;
            palette += 2;
            offset += 2;
        } while (first_column != 8);

        second_column = 0;
        do {
            second_column++;
            *(u16 *)(0x06003800 + (s32)offset) = 0;
            offset += 2;
        } while (second_column != 8);

        row_tile += 0x1000;
        row++;
        tile_index += 8;
    } while (row != 16);
}
