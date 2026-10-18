// Draft, differ 124 of 131: Map_WriteLayerCellTile, puts 32 by 32 pixel
// picture tile in quarter (x & 1, y & 1) of BG layer layer: unless update
// is set and the quarter already shows it, decodes the picture (or clears
// it) in heap slot 56, copies its 16 rows into gMapBlocks and, when update
// is set, redraws its tiles into the screen at 0x06004000. Called by
// games/tla/src/field/common/map/perspective.c. No ☀️ twin found. Same
// operations; left is register choice: the native keeps layer * 2 in r9
// and y in r8 and adds them again at each use, where this draft keeps the
// sum.
#include "types.h"
#include "ram_buffer.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"

struct LayerTileWork {
  u8 unk000[0x114];
  u32 *tiles;
  u8 unk118[0x830];
  u16 shown[3][2][2];
};

extern u8 gMapBlocks[];
extern u8 gMapCellBuffer[];
extern u8 Data_02010002[];

s32 Resource_DecodeByteLzInRam(const void *source, void *destination);

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update) {
  struct LayerTileWork *work = (struct LayerTileWork *)Ram_HeapSlots->mapWork;
  u32 *tiles = work->tiles;
  u16 *shown;
  u8 *buffer;
  u8 *source;
  u8 *destination;
  u16 *screen;
  u32 row;
  u32 column;

  x &= 1;
  y &= 1;
  layer *= 2;
  shown = &work->shown[0][0][0] + ((layer + y) * 2 + x);
  if (update == 0 && tile == *shown)
    return 0;
  *shown = tile;
  buffer = Runtime_AllocateHeapBlock(56, 0x400);
  if (tiles[tile] != 0)
    Resource_DecodeByteLzInRam((u8 *)tiles + tiles[tile], buffer);
  else
    Iwram_ClearWords(buffer, 0x400);
  source = buffer;
  destination = gMapBlocks + ((((layer + y) << 5) + x) << 6);
  for (row = 0; row < 16; row++) {
    Dma_Set(source, destination, 0x84000010, REG_DMA3);
    source += 64;
    destination += 128;
  }
  if (update != 0) {
    screen = (u16 *)(0x06004000 + ((((layer + y) << 6) + x) << 5));
    source = buffer;
    for (row = 0; row < 16; row++) {
      for (column = 0; column < 16; column++) {
        u32 cell = *(u16 *)source << 2;

        screen[0] = *(u16 *)(gMapCellBuffer + cell);
        screen[32] = *(u16 *)(Data_02010002 + cell);
        source += 4;
        screen++;
      }
      screen += 48;
    }
  }
  Runtime_ReleaseHeapBlock(56);
  return 1;
}
