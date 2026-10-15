#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapCellBuffer[];

// セルの当たりコードと属性を書き換え、元の値を返す
s32 MapCell_SetAttributes(u32 layer, s32 x, s32 z, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = (u8 *)map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (z << 7)) << 2;
  old = (cell[2] << 8) | cell[3];
  cell[2] = value >> 8;
  cell[3] = value;
  return old;
}
