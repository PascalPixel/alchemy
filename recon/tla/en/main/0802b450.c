// Draft, differ 192 of 148: copies the block numbers (low 11 bits) of a
// width by height rectangle of map cells from (srcX, srcY) to (dstX, dstY),
// keeping each destination's attributes, and redraws the copied blocks
// visible in any of the three BG layers, as ☀️'s
// Map_CopyMetatileIndicesRect (games/tbs/src/field/field.c, whose plain
// body differs 214 and matches only with tagged temporaries). Same loops;
// left is register choice and loop invariants: loop.c here hoists the VRAM
// and gMapBlocks constants out of the inner loops (a 48-byte frame where
// the native has 36), the native reloads them in place.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

struct TilePos {
  s32 x, y;
};

extern u32 gMapCellBuffer[];
extern u32 gMapBlocks[];

void Func_0802b450(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  u32 *source = gMapCellBuffer + (srcY * 128 + srcX);
  u32 *destination = gMapCellBuffer + (dstY * 128 + dstX);
  struct MapLayer *layer = Ram_HeapSlots->mapWork->layers;
  struct TilePos visible[3];
  struct TilePos *view;
  s32 i;
  s32 x;
  s32 y;

  view = visible;
  for (i = 2; i >= 0; i--) {
    view->x = layer->x >> 20;
    view->y = layer->y >> 20;
    view++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 cell = *source++;
      u32 block = cell & 0xfff;
      u32 offset;

      *destination = (cell & 0x7ff) | (*destination & 0xfffff800);
      destination++;
      offset = (((y & 15) << 5) + (x & 15)) * 4;
      view = visible;
      for (i = 0; i < 3; i++) {
        if (view->x <= x && view->x + 16 > x && view->y <= y &&
            view->y + 12 > y) {
          *(u32 *)(0x06002800 + offset) = gMapBlocks[block * 2];
          *(u32 *)(0x06002840 + offset) = gMapBlocks[block * 2 + 1];
          break;
        }
        offset += 0x800;
        view++;
      }
    }
    source += 128 - width;
    destination += 128 - width;
  }
}
