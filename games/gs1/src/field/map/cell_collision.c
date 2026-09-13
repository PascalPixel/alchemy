#include "map.h"
#include "global_cells.h"

extern struct MapState *Data_03001e70;

u8 GetMapCellCollision(s32 layer, s32 x, s32 y)
{
    struct MapState *state;
    s32 cell_address;
    s32 layer_offset;

    state = *(struct MapState **)ADDR_03001E70;
    x >>= 20;
    y >>= 20;
    cell_address = 0x02010000;
    if (state != NULL) {
        layer_offset = (layer & 3) * sizeof(struct MapLayer) + 0x130;
        cell_address = *(s32 *)((u8 *)state + layer_offset);
    }
    cell_address += (x + (y << 7)) * sizeof(struct MapCell);
    return ((struct MapCell *)cell_address)->collision_code;
}

void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collision_code)
{
    struct MapState *state = Data_03001e70;

    x >>= 20;
    y >>= 20;
    if (state != NULL) {
        struct MapCell *cells = state->layers[layer & 3].cells;
        u32 offset = (x + (y << 7)) * sizeof(struct MapCell);
        u8 *cell = (u8 *)cells;

        cell += offset;
        cell[2] = collision_code;
    }
}

s32 Map_GetCellHighFlags(s32 x, s32 y)
{
    s32 tile_x = x / 16;
    s32 tile_y = y / 16;
    u8 *cell = (u8 *)0x02010000 + (tile_x + tile_y * 128) * 4;

    return cell[1] >> 6;
}
