/// @file
/// Clears the object's mark and the cells ahead of and behind it that the mark blocked.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "map.h"

struct Object0200074c {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x4f];
  u8 marked;
};

void Func_0200075c(struct Object0200074c *object) {
  struct MapWork *map;
  struct MapCell **cells;
  struct MapCell *cell;
  s32 x;
  s32 z;

  if (object->marked == 0)
    return;
  map = Ram_HeapSlots->mapWork;
  if (map == NULL)
    return;
  cells = &map->layers[2].cells;
  cell = *cells;
  x = object->x >> 20;
  z = (object->z - object->y) >> 20;
  cell += x + ((z + 1) << 7);
  cell->collisionCode = 0;
  cell = *cells;
  cell += x + ((z - 1) << 7);
  cell->collisionCode = 0;
  object->marked = 0;
}
