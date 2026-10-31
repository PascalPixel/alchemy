#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "map.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "graphics.h"

struct PerspectiveTiles {
  u8 unk000[0x114];
  u32 *tiles;
  u8 unk118[0x830];
  u16 quarterTiles[8];
};

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update) {
  struct PerspectiveTiles *perspective =
      (struct PerspectiveTiles *)Ram_HeapSlots->mapWork;
  u32 *tileOffsets = perspective->tiles;
  u16 *shownQuarter;
  u16 *decodedTile;
  u16 *sourceTiles;
  u8 *quarterCells;
  u16 *screenTiles;
  u32 rowIndex;
  u32 columnIndex;
  u32 cellPictureId;

  x &= 1;
  y &= 1;
  shownQuarter = &perspective->quarterTiles[(layer * 2 + y) * 2 + x];
  if (update == 0 && tile == *shownQuarter)
    return 0;
  *shownQuarter = tile;
  decodedTile = HeapSlot_Alloc(56, 0x400);
  if (tileOffsets[tile] != 0)
    Resource_DecodeByteLzInRam((u8 *)tileOffsets + tileOffsets[tile],
                               decodedTile);
  else
    Iwram_ClearWords(decodedTile, 0x400);

  sourceTiles = decodedTile;
  // The block table at its fixed address: as the symbol gMapBlocks, its
  // address forms in r2 rather than r1 and update reloads into r3, not r2.
  quarterCells = Ram_MapBlocks + (((layer * 2 + y) * 32 + x) << 6);
  for (rowIndex = 0; rowIndex < 16; rowIndex++) {
    Dma_Set(sourceTiles, quarterCells, 0x84000010, REG_DMA3);
    sourceTiles += 32;
    quarterCells += 128;
  }

  if (update != 0) {
    screenTiles = (u16 *)(0x06004000 + ((((layer * 2 + y) << 6) + x) << 5));
    sourceTiles = decodedTile;
    for (rowIndex = 0; rowIndex < 16; rowIndex++) {
      for (columnIndex = 0; columnIndex < 16; columnIndex++) {
        u16 *bottomTiles;

        cellPictureId = *sourceTiles;
        screenTiles[0] = *(u16 *)(Ram_MapCellBuffer + cellPictureId * 4);
        bottomTiles = (u16 *)(Ram_MapCellBuffer + 2);
        bottomTiles = (u16 *)((u8 *)bottomTiles + cellPictureId * 4);
        screenTiles[32] = *bottomTiles;
        screenTiles++;
        sourceTiles += 2;
      }
      screenTiles += 48;
    }
  }
  HeapSlot_Release(56);
  return 1;
}
