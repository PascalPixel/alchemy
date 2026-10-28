// Draft, differ 34 of 61: overlay 663's copy of overlay 691's Func_020006cc
// (the same listing, calling Func_02000348 where 691 calls
// MapCell_GetAttributes), whose draft holds its notes.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
s32 Func_02000348(u32 layer, s32 x, s32 z);

void Func_020003e8(u16 *ids) {
  struct Actor *object;
  u8 *blocked;
  struct MapWork *map;
  struct MapCell **cells;
  s32 x, z;

  goto check;
loop:
  object = Actor_GetById(*ids);
  blocked = (u8 *)object + 0x63;
  *blocked = 0;
  if (Func_02000348(0, object->x >> 20, object->z >> 20) & 0x40) {
    map = Ram_HeapSlots->mapWork;
    if (map == NULL)
      return;
    cells = &map->layers[2].cells;
    z = (object->z - object->y) >> 20;
    x = object->x >> 20;
    ((u8 *)&(*cells)[x + ((z + 1) << 7)])[2] = 255;
    (*cells)[x + ((z - 1) << 7)].collisionCode = -1;
    *blocked = 1;
  }
  ids++;
check:
  if (*ids != 0xffff)
    goto loop;
}
