#include "types.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

struct MapTileWindow_08010d48 {
    s32 *position;
    u8 unknown_004[0x134];
    u16 tiles[16][16];
};

extern struct MapTileWindow_08010d48 *Data_03001e70;

s32 Func_080108e4(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Func_08010d48(s32 x, s32 y, s32 px, s32 py)
{
    struct MapTileWindow_08010d48 *window;
    s32 *position;
    s32 origin_x;
    s32 origin_y;
    s32 tile;

    window = Data_03001e70;
    origin_x = 0;
    origin_y = 0;
    position = window->position;
    if (position != 0) {
        origin_x = *position++;
        origin_y = position[1];
    }

    origin_x >>= 24;
    origin_y >>= 24;
    x >>= 4;
    y >>= 4;
    px >>= 3;
    py >>= 3;
    tile = (y << 4) + x;
    window->tiles[(py / 2) & 15][(px / 2) & 15] = tile;

    if (ABS(origin_x - px) <= 1 && ABS(origin_y - py) <= 1) {
        Func_080108e4(0, px / 2, py / 2, tile, 1);
        Func_080108e4(1, px / 2, py / 2, tile + 0x140, 1);
    }
}
