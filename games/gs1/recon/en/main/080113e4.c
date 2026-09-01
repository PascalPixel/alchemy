#include "types.h"

struct MapPosition_080113e4 {
    s32 x;
    s32 y;
    s32 z;
};

struct MapTileWindow_080113e4 {
    struct MapPosition_080113e4 *position;
    u8 unknown_004[0x134];
    u16 tiles[2][160];
};

extern struct MapTileWindow_080113e4 *Data_03001e70;

void Func_080108e4(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Func_080113e4(void)
{
    struct MapTileWindow_080113e4 *window;
    struct MapPosition_080113e4 *position;
    s32 origin_x;
    s32 origin_y;
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

    layer = 0;
    do {
        row = 0;
        do {
            column = 0;
            do {
                s32 x = origin_x + column;
                s32 y = origin_y + row;
                s32 tile = window->tiles[layer][
                    ((y & 15) << 4) + (x & 15)];

                Func_080108e4(layer, x, y, tile, 1);
                column++;
            } while (column <= 1);
            row++;
        } while (row <= 1);
        layer++;
    } while (layer <= 1);
}
