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
