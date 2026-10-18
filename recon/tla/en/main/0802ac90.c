// Draft, differ 144 of 114: the column counterpart of Func_0802abbc
// (0802abbc.c): draws 11 block rows of one tile column of BG layer layer,
// the left or right half of each block by x's low bit. No ☀️ twin as such
// (☀️'s field.c draws whole blocks). Same operations; register choice
// differs (this draft spills two values to an 8-byte frame the native does
// not have), the native reloads gMapCellBuffer for the row origin instead
// of adding to the pointer, and it addresses the bottom tiles from a pooled
// gMapBlocks + 4 (Data_02020004).
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
  u16 top[2];
  u16 bottom[2];
};

extern struct MapCellWord gMapCellBuffer[];
extern struct MapBlock gMapBlocks[];

void Func_0802ac90(s32 layer, s32 x, s32 y) {
  struct MapCellWord *cells = gMapCellBuffer;
  struct MapLayerWrap *wrap =
      (struct MapLayerWrap *)&Ram_HeapSlots->mapWork->layers[layer];
  u16 *screen = (u16 *)(0x06002800 + (layer << 11));
  u32 row = ((y / 2) & 127) << 7;
  u32 screenRow = (y & 30) << 5;
  u32 rowMask = wrap->maskY;
  u32 origin = wrap->originY;
  u32 column;
  u32 screenColumn;
  u32 columnMask;
  u32 half;
  u32 i;

  if (rowMask != 0x3f80) {
    row = (row - origin) & rowMask;
    cells = gMapCellBuffer + origin;
  }
  column = (x / 2) & 127;
  columnMask = wrap->maskX;
  screenColumn = x & 30;
  origin = wrap->originX;
  column &= columnMask;
  if (columnMask != 127) {
    column = (column - origin) & columnMask;
    cells += origin;
  }
  half = x & 1;
  for (i = 0; i < 11; i++) {
    u32 block = cells[row + column].block;
    u16 *dst = screen + screenRow + screenColumn + half;

    dst[0] = gMapBlocks[block].top[half];
    dst[32] = gMapBlocks[block].bottom[half];
    row = (row + 128) & rowMask;
    screenRow = (screenRow + 64) & 0x3c0;
  }
}
