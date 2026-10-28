// Draft, differ 34 of 61: marks the layer 2 cells ahead of and behind each
// object of a table that ends on 0xffff when its cell has attribute 0x40.
// As a goto loop (as a for loop the loop pass keeps 0xffff in r10 and the
// test is copied, 93 differ), the first store through a byte pointer and the
// second through the field (as two field stores the 255 becomes an orr, both
// through bytes they share one 255). Left: the native gives the map r0, z r2
// and x r1 (here r4, r2, r1 with the map's cells offset built in r2), and
// stores -1 built by negs where this draft ors 255 into the byte (as an s8
// store the negs comes, 61). Tried: an inline setter with the value as s32
// (55), x before z, the cells read from the map twice, map pinned to r0,
// permute 6 minutes. Func_0200074c clears the same pair.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);

void Func_020006cc(u16 *ids) {
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
  if (MapCell_GetAttributes(0, object->x >> 20, object->z >> 20) & 0x40) {
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
