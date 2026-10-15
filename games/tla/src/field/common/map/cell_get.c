#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapCellBuffer[];

// セルの当たりコードと属性を 16 ビットで読む
s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = (u8 *)map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (z << 7)) << 2;
  return (cell[2] << 8) | cell[3];
}
