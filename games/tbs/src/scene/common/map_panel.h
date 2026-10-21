#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "io_reg.h"

// Crossbone Isle Cave (CrossboneIsleSteppingStones to
// CrossboneIsleDragonStatue) and Kolima Forest (scene 43): drawing a stretch of
// the map, in one of the states kept elsewhere in it (such as the water raised
// or not), straight into the background's tile map.

/// Draws the width by height cells at cell (x, y) of the map into screen block
/// 5 from cell (destX, destY), wrapped to 16 by 16 cells, in band plane (16
/// cell rows each): each cell is its block's tiles from Ram_MapBlocks.
static void Map_DrawCellPanel(s32 x, s32 y, s32 width, s32 height, s32 plane,
                              s32 destX, s32 destY) {
  u32 *cell = (u32 *)Ram_MapCellBuffer + (y * 128 + x);
  s32 row;
  s32 col;
  s32 end;

  for (row = destY; row < destY + height; row++) {
    col = destX;
    if (col < destX + width)
      do {
        u16 tile = 0xfff & *cell++;
        u16 offset = tile * 8;
        s32 index = (plane * 16 + (row & 15)) * 32 + (col & 15);
        u32 *src;
        u32 *dst;

        dst = (u32 *)BG_SCREEN_BLOCK(5) + index;
        src = (u32 *)(Ram_MapBlocks + offset);
        *dst = *src;
        src = (u32 *)(Ram_MapBlocks + 4 + offset);
        dst = (u32 *)(BG_SCREEN_BLOCK(5) + 64) + index;
        *dst = *src;

        /* FAKEMATCH(temporary): end takes the bound where the loop tests it,
         * apart from the guard's destX + width; as ++col < destX + width, 12
         * of 80 instructions differ in tbs-en. */
      } while (end = destX + width, ++col < end);
    cell += 128 - width;
  }
}
