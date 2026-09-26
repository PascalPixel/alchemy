/* Draft, not exact (2026-09-26): 184 of 188 bytes, 75 differing halfwords.
   The ROM indexes one shared tile table and adds a 320-tile bias for the
   second layer, not a second table. The origin pointer walks after x and
   the row coordinate advances independently of its row counter. Retained
   the canonical s32 tile service declaration. Goto loops give 172 bytes;
   separate mask regions and a void result-discarding wrapper give the same
   184-byte output. Remaining: cached mask in r9, origin x kept in fp rather
   than its stack slot, and a spilled row counter instead of the ROM's sl.
 */
#include "TYPES.H"

struct MapPosition_080113e4 {
    s32 x;
    s32 y;
    s32 z;
};

struct MapTileWindow_080113e4 {
    struct MapPosition_080113e4 *position;
    u8 unknown_004[0x134];
    u16 tiles[256];
};

extern struct MapTileWindow_080113e4 *Data_03001e70;

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Func_080113e4(void)
{
    struct MapTileWindow_080113e4 *window;
    struct MapPosition_080113e4 *position;
    s32 origin_x;
    s32 origin_y;
    u32 layer;
    u32 row;
    u32 column;
    u32 tile_bias;

    window = Data_03001e70;
    origin_x = 0;
    origin_y = 0;
    position = window->position;
    if (position != 0) {
        s32 *walk = &position->x;
        origin_x = *walk++;
        origin_y = walk[1];
    }

    origin_x = (origin_x - 0x01000000) >> 25;
    origin_y = (origin_y - 0x01400000) >> 25;

    layer = 0;
    tile_bias = 0;
    do {
        s32 y = origin_y;

        row = 0;
        do {
            s32 tile_row = (y & 15) << 4;

            column = 0;
            do {
                s32 x = origin_x + column;
                s32 tile_col = x & 15;
                s32 tile = window->tiles[tile_row + tile_col] + tile_bias;

                Map_WriteLayerCellTile(layer, x, y, tile, 1);
                column++;
            } while (column <= 1);
            row++;
            y++;
        } while (row <= 1);
        layer++;
        tile_bias += 320;
    } while (layer <= 1);
}
