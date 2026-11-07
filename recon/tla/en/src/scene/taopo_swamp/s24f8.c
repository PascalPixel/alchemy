#include "../../../../games/tla/src/scene/taopo_swamp/local.h"

void Func_020027f8(struct Actor *object, s32 a);

void Func_020024f8(struct Actor *object, s32 priority) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->priorityFlags = 2;
  ((struct FieldSprite *)object->animation)->priority = priority;
  Actor_SetSingleChildFlags(object, 0);
  Func_020027f8(object, 2);
  Func_02002800(object, gData_02002af4);
  Actor_SetPartAttribute(object, 10);
  object->action = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}
