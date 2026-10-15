/* Draft, differ 2 of 86: Map_UpdateCurrentTileBlock, written from the
 * listing. The one difference is a load order: the native
 * reloads y0 from its stack slot (ldr r6, [sp, #4]) before the zero for row
 * and the layer shift; here sched2 places it after both. */
#include "types.h"
#include "ram_buffer.h"

struct MapTileWindow {
    s32 *origin;
    u8 unknown_004[0x144];
    u16 tile_ids[32 * 32];
};

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Map_UpdateCurrentTileBlock(void)
{
    struct MapTileWindow *window = (struct MapTileWindow *)Ram_HeapSlots->map_work;
    s32 x0 = 0;
    s32 y0 = 0;
    u32 layer;
    u32 row;
    u32 col;
    s32 bias;
    s32 tile;

    if (window->origin != NULL)
    {
        s32 *p = window->origin;

        x0 = *p++;
        y0 = p[1];
    }
    x0 = (x0 - 0x1000000) >> 25;
    y0 = (y0 - 0x1400000) >> 25;
    for (layer = 0; layer < 2; layer++)
    {
        for (row = 0; row < 2; row++)
        {
            for (col = 0; col < 2; col++)
            {
                tile = (((y0 + row) & 31) << 5) + ((x0 + col) & 31);
                tile = window->tile_ids[tile];
                tile += layer << 10;
                Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 1);
            }
        }
    }
}
