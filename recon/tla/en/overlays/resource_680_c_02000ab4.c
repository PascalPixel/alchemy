// Near miss: Func_02000ab4 differs 34 of 53 and Func_02000b2c 28 of 66. The
// listing builds each structure offset afresh in its own register; the draft
// derives one from another.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_work.h"
#include "scene_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);

void Func_02000ab4(void) {
  if (gPartyState.renderMode == 2) {
    struct EventWork *work = Ram_HeapSlots->eventWork;
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
    s32 x = object->x / 0x100000;
    s32 z = (object->z - object->y + 0x7fff) / 0x100000;
    s32 code = map->layers[2].cells[x + z * 128].collisionCode;

    if ((u32)(code - 1) <= 229)
      work->exit = code;
  }
}

void Func_02000b2c(s32 id, s32 x, s32 y, s32 z) {
  struct ObjectRuntime *object = Object_GetById(id);
  struct MapCell *cells = Ram_HeapSlots->mapWork->layers[2].cells;
  s32 cellX = x / 0x100000;
  s32 cellZ = (z - y + 0x7fff) / 0x100000;

  cells[cellX + cellZ * 128].collisionCode = 255;
  object->x = (x & 0xfff00000) + 0x80000;
  object->z = (z & 0xfff00000) + 0x80000;
  object->y = object->z - (cellZ << 20);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  ObjectDispatch_SetSingleChildFlags(object, 0);
}
