// Draft, differ 140 of 97: draws one row of 16 map blocks of BG layer
// layer into its screen block at 0x06002800, from the cell buffer wrapped
// by the layer's masks and origins, each block's top and bottom tile pairs
// from gMapBlocks. ☀️'s Map_SetCameraCenter (games/tbs/src/field/field.c)
// draws the same way through CopyMetatileToScreen. Same instructions;
// register choice differs throughout (the native holds the cell pointer in
// ip, the screen in sl, the loop count in r6), and the native addresses a
// block's second word from a pooled gMapBlocks + 4 (Data_02020004) where
// this draft adds 4 to the index (also with gMapBlocks as a u32 array).
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

struct MapLayerWrap {
  s32 x, y;
  u8 unk08[0x20];
  u16 maskX;
  u16 maskY;
  u16 originX;
  u16 originY;
  struct MapCell *cells;
  u8 *terrainIds;
};

struct MapCellWord {
  u32 block : 11;
  u32 flags : 21;
};

struct MapBlock {
  u32 top;
  u32 bottom;
};

extern struct MapCellWord gMapCellBuffer[];
extern struct MapBlock gMapBlocks[];

void Func_0802abbc(s32 layer, s32 x, s32 y) {
  struct MapCellWord *cells = gMapCellBuffer;
  struct MapLayerWrap *wrap =
      (struct MapLayerWrap *)&Ram_HeapSlots->mapWork->layers[layer];
  u8 *screen = (u8 *)0x06002800 + (layer << 11);
  u32 row = (y / 2) & 127;
  u32 screenRow = (y & 30) << 5;
  u32 mask = wrap->maskY;
  u32 origin = wrap->originY;
  u32 column;
  u32 screenColumn;
  u32 i;

  row <<= 7;
  if (mask != 0x3f80) {
    row = (row - origin) & mask;
    cells = gMapCellBuffer + origin;
  }
  column = (x / 2) & 127;
  mask = wrap->maskX;
  origin = wrap->originX;
  screenColumn = x & 30;
  column &= mask;
  if (mask != 127) {
    column = (column - origin) & mask;
    cells += origin;
  }
  for (i = 0; i < 16; i++) {
    u32 block = cells[row + column].block;
    u32 *dst = (u32 *)(screen + (screenRow + screenColumn) * 2);

    dst[0] = gMapBlocks[block].top;
    dst[16] = gMapBlocks[block].bottom;
    column = (column + 1) & mask;
    screenColumn = (screenColumn + 2) & 30;
  }
}
