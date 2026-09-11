#include "types.h"
#include "scene.h"
#include "abi/map/shared/set_window_cell_tile.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

struct MapTileWindow_08010d48 {
    s32 *position;
    u8 unknown_004[0x134];
    u16 tiles[16][16];
};

extern struct MapTileWindow_08010d48 *gCam;

void Map_Run(s32 x, s32 y, s32 px, s32 py)
{
    struct MapTileWindow_08010d48 *window;
    s32 *position;
    s32 origin_x;
    s32 origin_y;
    s32 tile;

    window = gCam;
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
        Map_SetRange(0, px / 2, py / 2, tile, 1);
        Map_SetRange(1, px / 2, py / 2, tile + 0x140, 1);
    }
}
