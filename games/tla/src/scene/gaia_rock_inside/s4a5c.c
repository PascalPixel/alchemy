#include "types.h"
#include "system/ram_buffer.h"
#include "map.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);

void Func_02004a5c(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object = Actor_GetById(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[2].cells;
  s32 column = x / 0x100000;
  s32 row = (z - y + 0x7fff) / 0x100000;

  cell += column + (row << 7);
  cell->collisionCode = 255;
  object->x = (x & 0xfff00000) + 0x80000;
  object->z = (z & 0xfff00000) + 0x80000;
  object->y = object->z - (row << 20);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  Actor_SetSingleChildFlags(object, 0);
}
