// Draft, differ 8 of 131 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Near miss, differ 18 of 131 (2026-10-18): The Broken Seal's
// Map_WriteLayerCellTile; this game clears the block (IwramClearWords)
// where the tile has no data instead of decoding it, and uses heap slot 56.
// Left: the native loads the map work, the stack argument update, the block
// table and the cell buffer each a step earlier, and forms gMapBlocks in r1
// (here r2) and reloads update into r2 (here r3). Six minutes of permute: no
// better.
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "map.h"
#include "iwram_call.h"
#include "mem.h"
#include "graphics.h"

struct PerspectiveTiles {
  u8 unk000[0x114];
  u32 *tiles;
  u8 unk118[0x830];
  u16 quarterTiles[8];
};

extern u8 gMapBlocks[];

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
  decodedTile = Runtime_AllocateHeapBlock(56, 0x400);
  if (tileOffsets[tile] != 0)
    Resource_DecodeByteLzInRam((u8 *)tileOffsets + tileOffsets[tile],
                               decodedTile);
  else
    Iwram_ClearWords(decodedTile, 0x400);

  sourceTiles = decodedTile;
  quarterCells = gMapBlocks + (((layer * 2 + y) * 32 + x) << 6);
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
  Runtime_ReleaseHeapBlock(56);
  return 1;
}
