/* NONMATCHING: 192 of 200 bytes, 87 differing halfwords, 66 halfword
 * edits (2026-09-25). Corrected the layer offset: the reference adds it
 * to the loaded tile ID, not to the address of the tile. The source tile
 * map is a shared 16-by-16 grid. Remaining: coordinate spilling, initial
 * position post-increment load, and loop register allocation. Aggregate
 * coordinates and goto loops did not improve the match.
 * 2026-09-29 (alchemy permute scorer): the draft scored 2774 (20
 * register-only, 11 operand, 15 reordered, 5 inserted, 10 deleted), the
 * callee under its build name Map_WriteLayerCellTile and the work pointer
 * as gMapWork. A 300-second search (23,000 candidates) reached this
 * spelling at 2212 (17 register-only, 2 stack-only, 4 operand, 15
 * reordered, 3 inserted, 8 deleted): the position test as an assignment in
 * the condition, the call result tested through a local, and a register
 * window pointer; the remaining difference is still the coordinate spill
 * slots and the three nested loops' register allocation.
 */
#include "TYPES.H"

struct MapPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct MapTileWindow {
    struct MapPosition *position;
    u8 unknown_004[0x134];
    u16 tiles[256];
};

extern struct MapTileWindow *gMapWork;

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Map_UpdateCurrentTileBlockUntilBlocked(void)
{
    register struct MapTileWindow *window;
    struct MapPosition *position;
    s32 origin_x;
    s32 origin_y;
    s32 layer_offset;
    u32 layer;
    u32 row;
    u32 column;
    s32 tmp;

    origin_x = 0;
    layer = 0;
    window = gMapWork;
    origin_y = 0;
    if (position = window->position) {
        origin_x = position->x;
        origin_y = position->z;
    }
    origin_x = (origin_x - 0x01000000) >> 25;
    tmp = origin_y - 0x01400000;
    layer_offset = 0;
    origin_y = tmp >> 25;
    do {
        row = 0;
        do {
            column = 0;
            do {
                s32 x = origin_x + column;
                s32 y = origin_y + row;
                s32 tile = *(u16 *)((u8 *)window + 0x138 + ((((y & 15) << 4) + (x & 15)) << 1)) + layer_offset;
                s32 tmp2;
                tmp2 = Map_WriteLayerCellTile(layer, x, y, tile, 0) != 0;
                if (tmp2)
                    return;
                ++column;
            } while (column <= 1);
            row += 1;
        } while (1 >= row);
        layer_offset = layer_offset + 320;
        layer++;
    } while (layer <= 1);
}
