#include "map.h"
#include "types.h"
#include "global_cells.h"

struct MapCollisionWork {
    u8 unknown_000[0x190];
    u8 *cells;
};

s32 CheckMapPositionCellOccupied(struct WorldPosition *position)
{
    s32 x;
    s32 y;
    s32 tile_x;
    s32 tile_y;
    struct MapCollisionWork *work;
    u8 *cell;

    x = position->x / 65536;
    y = (position->y - *(s32 *)((u8 *)position + 4)) / 65536;
    work = *(struct MapCollisionWork **)ADDR_03001E70;
    if (work == NULL)
        return 0;
    cell = work->cells;
    tile_x = x / 16;
    tile_y = y / 16;
    cell += (tile_x + tile_y * 128) * 4;
    return (cell[2] != 0xFF) - 1;
}
