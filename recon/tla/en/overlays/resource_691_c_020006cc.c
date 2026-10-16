#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);

void Func_020006cc(u16 *ids) {
  for (; *ids != 0xffff; ids++) {
    struct ObjectRuntime *object = Object_GetById(*ids);
    u8 *blocked = (u8 *)object + 0x63;

    *blocked = 0;
    if (MapCell_GetAttributes(0, object->x >> 20, object->z >> 20) & 0x40) {
      struct MapWork *map = Ram_HeapSlots->mapWork;
      struct MapCell **cells;
      s32 x, z;

      if (map == NULL)
        break;
      cells = &map->layers[2].cells;
      z = (object->z - object->y) >> 20;
      x = object->x >> 20;
      (*cells)[x + (z + 1) * 128].collisionCode = 255;
      (*cells)[x + (z - 1) * 128].collisionCode = 255;
      *blocked = 1;
    }
  }
}
