/// @file
/// For each id of a list ended by 0xffff: clears the object's mark and, on a cell with
/// attribute bit 6, blocks the cells ahead of and behind it and sets the mark.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "event_inline.h"

s32 Func_02000658(u32 layer, s32 x, s32 z);

void Func_020006dc(u16 *ids) {
  struct Actor *object;
  u8 *blocked;
  struct MapWork *map;
  struct MapCell **cells;
  struct MapCell *cell;
  s32 x, z;

  while (*ids != 0xffff) {
  object = Actor_Get(*ids);
  blocked = (u8 *)object + 0x63;
  *blocked = 0;
  if (Func_02000658(0, object->x >> 20, object->z >> 20) & 0x40) {
    map = Ram_HeapSlots->mapWork;
    if (map == NULL)
      return;
    cells = &map->layers[2].cells;
    z = (object->z - object->y) >> 20;
    x = object->x >> 20;
    cell = *cells;
    cell += x + ((z + 1) << 7);
    cell->collisionCode = 255;
    cell = *cells;
    cell += x + ((z - 1) << 7);
    cell->collisionCode = 255;
    *blocked = 1;
  }
  ids++;
  }
}
