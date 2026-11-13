#include "types.h"
#include "ram_buffer.h"
#include "map_grid.h"
#include "io_reg.h"

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
  u32 *top = (u32 *)BG_SCREEN_BLOCK(5);
  u32 *bottom = (u32 *)(BG_SCREEN_BLOCK(5) + 64);
  u32 *tiles;
  s32 index;
  s32 row;
  s32 col;

  for (row = destinationY; row < destinationY + height; row++) {
    for (col = destinationX; col < destinationX + width; col++) {
      // A cell picture is four tile map entries, two rows of two.
      tiles = (u32 *)(Ram_CellPictures + (u16)((*cell++ & 0xfff) * 8));
      index = (plane * 16 + (row & 15)) * 32 + (col & 15);
      top[index] = tiles[0];
      bottom[index] = tiles[1];
    }
    cell += 128 - width;
  }
}
