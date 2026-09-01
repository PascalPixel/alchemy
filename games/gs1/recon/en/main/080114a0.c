#include "types.h"

struct MapPosition_080114a0 {
    s32 x;
    s32 y;
    s32 z;
};

struct MapTileWindow_080114a0 {
    struct MapPosition_080114a0 *position;
    u8 unknown_004[0x134];
    u16 tiles[2][160];
};

extern struct MapTileWindow_080114a0 *Data_03001e70;

s32 Func_080108e4(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Func_080114a0(void)
{
    struct MapTileWindow_080114a0 *window;
    struct MapPosition_080114a0 *position;
    s32 origin_x;
    s32 origin_y;
    s32 layer_offset;
    u32 layer;
    u32 row;
    u32 column;

    window = Data_03001e70;
    origin_x = 0;
    origin_y = 0;
    position = window->position;
    if (position != 0) {
        origin_x = position->x;
        origin_y = position->z;
    }

    origin_x = (origin_x - 0x01000000) >> 25;
    origin_y = (origin_y - 0x01400000) >> 25;

    layer_offset = 0;
    layer = 0;
    do {
        row = 0;
        do {
            column = 0;
            do {
                s32 x = origin_x + column;
                s32 y = origin_y + row;
                s32 tile = *(u16 *)((u8 *)window + 0x138 + layer_offset
                    + ((((y & 15) << 4) + (x & 15)) << 1));

                if (Func_080108e4(layer, x, y, tile, 0) != 0)
                    return;
                column++;
            } while (column <= 1);
            row++;
        } while (row <= 1);
        layer_offset += 320;
        layer++;
    } while (layer <= 1);
}
