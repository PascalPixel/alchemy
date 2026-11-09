#include "lemurian_ship.h"
#include "event_inline.h"

void Func_020023f8(void) {
  struct LemurianShipObject *leader = ((struct LemurianShipObject *)Actor_GetById(gMainRoutineWork.currentOwner));

  Actor_SetSpritePriority(8, leader->sprite->priority);
}

void Func_0200241c(void) {
  if (GameFlag_Test(8))
    MapGrid_CopyCellAttributes(14, 46, 1, 1, 16, 46);
  Func_02002dc8(gData_02003010);
  LEMURIAN_SHIP_TASK_ADD(Func_020023f8, 0x480);
}

void Func_0200245c(s32 id) {
  struct LemurianShipObject *object = ((struct LemurianShipObject *)Actor_GetById(id));

  ((struct LemurianShipObject *)Actor_GetById(id))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(id, 1);
  object->motionFlags = 0;
  object->y = 0;
  object->height = 0;
}
