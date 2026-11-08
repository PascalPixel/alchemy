#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_work.h"
#include "scene_ids.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Task_Add(void (*func)(void), s32 priority);

void Func_02000b2c(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object = Actor_GetById(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[2].cells;
  s32 cellX = x / 0x100000;
  s32 cellZ = (z - y + 0x7fff) / 0x100000;

  cell += cellX + (cellZ << 7);
  cell->collisionCode = 255;
  object->x = (x & 0xfff00000) + 0x80000;
  object->z = (z & 0xfff00000) + 0x80000;
  object->y = object->z - (cellZ << 20);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  Actor_SetSingleChildFlags(object, 0);
}
