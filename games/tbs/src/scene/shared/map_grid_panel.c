/// @file
/// Crossbone Isle Cave (SceneId_CrossboneIsleSteppingStones to
/// SceneId_CrossboneIsleDragonStatue) and Kolima Forest
/// (SceneId_KolimaForestFloodgate): drawing a stretch of the layout, in one of
/// the states kept elsewhere in it (such as the water raised or not), straight
/// into the background's tile map.

#include "system/ram_buffer.h"

/// Cells across and down a screen block: 32 tiles square, two tiles a cell.
#define SCREEN_BLOCK_CELLS (BG_SCREEN_WIDTH / 2)

/// Draws the width by height cells from cell (x, y) of the layout into screen
/// block 5 at cell (destinationX, destinationY), wrapping every
/// SCREEN_BLOCK_CELLS cells. The block is cut into horizontal planes of
/// SCREEN_BLOCK_CELLS cell rows; plane picks which one is drawn into.
static void MapGrid_DrawCellPanel(s32 x, s32 y, s32 width, s32 height,
                                  s32 plane, s32 destinationX,
                                  s32 destinationY) {
  u32 *cell = (u32 *)Ram_MapGridCellBuffer + (y * MAPGRID_SIZE + x);
  s32 row;
  s32 col;
  s32 end;

  for (row = destinationY; row < destinationY + height; row++) {
    col = destinationX;
    if (col < destinationX + width)
      do {
        // A cell's tiles are four tile map entries, two rows of two, 8 bytes;
        // a cell row of the screen block is two rows of BG_SCREEN_WIDTH
        // entries, BG_SCREEN_WIDTH words.
        u16 tile = *cell++ & MAPGRID_CELL_TILES_ID_MASK;
        u16 picture = tile * 8;
        s32 index =
            (plane * SCREEN_BLOCK_CELLS + (row & (SCREEN_BLOCK_CELLS - 1))) *
                BG_SCREEN_WIDTH +
            (col & (SCREEN_BLOCK_CELLS - 1));
        u32 *from;
        u32 *to;

        to = (u32 *)BG_SCREEN_BLOCK(5) + index;
        from = (u32 *)(Ram_CellTiles + picture);
        *to = *from;
        from = (u32 *)(Ram_CellTiles + 4 + picture);
        to = (u32 *)(BG_SCREEN_BLOCK(5) + BG_SCREEN_WIDTH * 2) + index;
        *to = *from;
        /* FAKEMATCH(temporary): end takes the row's bound where the loop tests
         * it, apart from the guard's. */
      } while (end = destinationX + width, ++col < end);
    cell += MAPGRID_SIZE - width;
  }
}
