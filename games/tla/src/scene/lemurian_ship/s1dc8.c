#include "lemurian_ship.h"

void Func_02001dc8(s32 id) {
  struct LemurianShipObject *object = Actor_GetById(id);

  Actor_GetById(id)->priorityFlags |= 0x20;
  Actor_SetSpritePriority(id, 1);
  object->motionFlags = 0;
  object->y = -0x200000;
  object->height = -0x200000;
}
