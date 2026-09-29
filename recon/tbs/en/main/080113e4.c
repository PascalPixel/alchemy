/* 2026-09-29 alchemy permute: score 2048 to 1518 on the permuter's scorer
   (0 is exact); remaining 22 register-only, 3 stack-only, 5 operand, 16
   reordered, 2 inserted, 1 deleted. Kept rewrites: 10x reorder independent
   statements, 6x add a same-width cast, 5x drop a same-width cast, 4x
   reorder local declarations, 4x introduce a temporary, 4x change loop
   form, 4x pointer arithmetic or indexing, 4x move an assignment into or
   out of a condition, 4x test truth or compare with zero, 3x split or join
   a compound assignment, 3x toggle register, 2x swap commutative operands,
   1x remove a temporary. FAKEMATCH: the permuter's temporaries, register
   hints and swapped operand orders below only steer allocation and
   scheduling; no programmer would write them, so they stay tagged until a
   natural spelling replaces them. */
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

extern struct MapTileWindow_080113e4 *gMapWork;

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Map_UpdateCurrentTileBlock(void)
{
    struct MapTileWindow_080113e4 *window;
    struct MapPosition_080113e4 *position;
    s32 origin_y;
    s32 origin_x;
    register u32 layer;
    u32 tile_bias;
    u32 row;
    u32 column;
    s32 tmp3;
    struct MapTileWindow_080113e4 *tmp;

    origin_y = 0;
    origin_x = 0;
    tmp = gMapWork;
    window = tmp;
    position = window->position;
    if (position != 0) {
        s32 *walk = &position->x;
        s32 tmp2;
        origin_x = walk++[0];
        tmp2 = walk[1];
        origin_y = tmp2;
    }
    tmp3 = origin_x - 0x01000000;
    layer = 0;
    origin_x = tmp3 >> 25;
    origin_y = (origin_y - 0x01400000) >> 25;
    tile_bias = 0;
    while (1) {
        register s32 y = origin_y;
        row = 0;
        if (1 != 0) {
            do {
                s32 tile_row = (y & 15) << 4;
                column = 0;
                row++;
                while (1) {
                    register s32 x = origin_x + column;
                    s32 tile_col = x & 15;
                    s32 tile = *(window[0].tiles + (tile_col + tile_row)) + tile_bias;
                    Map_WriteLayerCellTile(layer, x, y, tile, 1);
                    column++;
                    if (column > 1)
                        break;
                }
                (u32)y++;
                if (row > 1)
                    break;
            } while (1);
        }
        layer++;
        tile_bias = tile_bias + 320;
        if (layer > 1)
            break;
    }
}
