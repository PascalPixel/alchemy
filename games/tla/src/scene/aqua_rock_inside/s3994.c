#include "../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_02003994(struct ObjectRuntime *object, s32 priority) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->priorityFlags = 2;
  ((struct FieldSprite *)object->animation)->priority = priority;
  ObjectDispatch_SetSingleChildFlags(object, 0);
  Func_02003cc4(object, 2);
  Func_02003ccc(object, gData_020044c8);
  Object_SetPartAttribute(object, 10);
  object->action = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}
