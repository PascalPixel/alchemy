// Draft, differ 34 of 61: overlay 692's copy of the function drafted in
// resource_691_c_020006cc.c (overlay 691's Func_020006cc, differ 34 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_02000658(u32 layer, s32 x, s32 z);

void Func_020006dc(u16 *ids) {
  struct ObjectRuntime *object;
  u8 *blocked;
  struct MapWork *map;
  struct MapCell **cells;
  s32 x, z;

  goto check;
loop:
  object = Object_GetById(*ids);
  blocked = (u8 *)object + 0x63;
  *blocked = 0;
  if (Func_02000658(0, object->x >> 20, object->z >> 20) & 0x40) {
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
