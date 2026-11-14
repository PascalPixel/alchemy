#include "system/ram_buffer.h"

// Crossbone Isle Cave (SceneId_CrossboneIsleSteppingStones to
// SceneId_CrossboneIsleDragonStatue) and Kolima Forest
// (SceneId_KolimaForestFloodgate): drawing a stretch of
// the layout, in one of the states kept elsewhere in it (such as the water
// raised or not), straight into the background's tile map.

/// Draws the width by height cells at cell (x, y) of the layout into screen
/// block 5 from cell (destinationX, destinationY), wrapped to 16 by 16 cells,
/// in band plane (16 cell rows each): each cell is its cell picture's tiles
/// from Ram_CellPictures.
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
        // A cell picture is four tile map entries, two rows of two, 8 bytes.
        u16 tile = *cell++ & 0xfff;
        u16 picture = tile * 8;
        s32 index = (plane * 16 + (row & 15)) * 32 + (col & 15);
        u32 *from;
        u32 *to;

        to = (u32 *)BG_SCREEN_BLOCK(5) + index;
        from = (u32 *)(Ram_CellPictures + picture);
        *to = *from;
        from = (u32 *)(Ram_CellPictures + 4 + picture);
        to = (u32 *)(BG_SCREEN_BLOCK(5) + 64) + index;
        *to = *from;
        /* FAKEMATCH(temporary): end takes the row's bound where the loop tests
         * it, apart from the guard's. */
      } while (end = destinationX + width, ++col < end);
    cell += 128 - width;
  }
}
