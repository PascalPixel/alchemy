#include "lemurian_ship.h"

void Func_020023f8(void) {
  struct LemurianShipObject *leader = Object_GetById(gAdventure.currentOwner);

  ObjectMotion_SetActionVariant(8, leader->sprite->priority);
}

void Func_0200241c(void) {
  if (GameFlag_Test(8))
    LemurianShip_BlendStart(14, 46, 1, 1, 16, 46);
  Func_02002dc8(gData_02003010);
  LEMURIAN_SHIP_TASK_ADD(Func_020023f8, 0x480);
}

void Func_0200245c(s32 id) {
  struct LemurianShipObject *object = Object_GetById(id);

  Object_GetById(id)->priorityFlags |= 0x20;
  ObjectMotion_SetActionVariant(id, 1);
  object->motionFlags = 0;
  object->y = 0;
  object->height = 0;
}
