#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/ram_buffer.h"
#include "../../../../games/tla/include/map.h"
#include "../../../../games/tla/include/actor.h"
#include "../../../../games/tla/include/main_routine_work.h"

struct Actor *Actor_GetById(u32 id);

void Func_020049e4(void) {
  if (gMainRoutineWork.renderMode == 2) {
    u8 *event = Ram_HeapSlots->eventWork;
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
    s32 x = object->x / 0x100000;
    s32 z = (object->z - object->y + 0x7fff) / 0x100000;
    struct MapCell *cells = map->layers[2].cells;
    struct MapCell *cell = &cells[x + (z << 7)];
    s32 code = cell->collisionCode;

    if (code >= 1 && code <= 230)
      *(u16 *)(event + 340) = code;
  }
}
