#include "types.h"
#include "ram_buffer.h"
#include "map_grid.h"
#include "io_reg.h"

// Crossbone Isle Cave (MapId_CrossboneIsleSteppingStones to
// MapId_CrossboneIsleDragonStatue) and Kolima Forest
// (MapId_KolimaForestFloodgate): drawing a stretch of
// the layout, in one of the states kept elsewhere in it (such as the water
// raised or not), straight into the background's tile map.

/// Draws the width by height cells at cell (x, y) of the layout into screen
/// block 5 from cell (destinationX, destinationY), wrapped to 16 by 16 cells,
/// in band plane (16 cell rows each): each cell is its metatile's tiles from
/// Ram_Metatiles.
static void MapGrid_DrawCellPanel(s32 x, s32 y, s32 width, s32 height,
                                  s32 plane, s32 destinationX,
                                  s32 destinationY) {
  u32 *cell = (u32 *)Ram_MapGridCellBuffer + (y * 128 + x);
  s32 row;
  s32 col;
  s32 end;

  for (row = destinationY; row < destinationY + height; row++) {
    col = destinationX;
    if (col < destinationX + width)
      do {
        u16 tile = 0xfff & *cell++;
        u16 offset = tile * 8;
        s32 index = (plane * 16 + (row & 15)) * 32 + (col & 15);
        u32 *src;
        u32 *dst;

        dst = (u32 *)BG_SCREEN_BLOCK(5) + index;
        src = (u32 *)(Ram_Metatiles + offset);
        *dst = *src;
        src = (u32 *)(Ram_Metatiles + 4 + offset);
        dst = (u32 *)(BG_SCREEN_BLOCK(5) + 64) + index;
        *dst = *src;

        /* FAKEMATCH(temporary): end takes the bound where the loop tests it,
         * apart from the guard's destinationX + width; as ++col < destinationX
         * + width, 12 of 80 instructions differ in tbs-en. */
      } while (end = destinationX + width, ++col < end);
    cell += 128 - width;
  }
}
