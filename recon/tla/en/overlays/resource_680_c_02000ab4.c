// Draft, differ 2 of 66, skeleton gap 1 (tla-en, 2026-11-01)
// Func_02000ab4 matches: differ 0. Func_02000b2c is left with one scheduling
// pair: the listing stores motionFlags before it copies object for the
// priorityFlags address, this copies first. Rewriting the |= 2 (spelled out, a
// flags local, a pointer) does not move it, and storing priorityFlags first or
// moving motionFlags above the y store costs far more. The cell += form and
// the two masked sums written straight into object are what give the listing's
// registers; x &= masks or a local for cellZ << 20 undo them.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_work.h"
#include "scene_ids.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Task_Add(void (*func)(void), s32 priority);

void Func_02000ab4(void) {
  if (gMainRoutineWork.renderMode == 2) {
    struct EventWork *work = Ram_HeapSlots->eventWork;
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
    s32 x = object->x / 0x100000;
    s32 z = (object->z - object->y + 0x7fff) / 0x100000;
    struct MapCell *cells = map->layers[2].cells;
    struct MapCell *cell = &cells[x + (z << 7)];
    s32 code = cell->collisionCode;

    if (code >= 1 && code <= 230)
      work->exit = code;
  }
}

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
