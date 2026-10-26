// Under -mthumb-slow-loads (2026-10-26): differ 4 of 40; left: as overlay 678's
// Func_020024f8 (resource_678_c_020024f8.c).
// Draft, differ 4: the listing loads the sprite and its attribute byte one
// instruction later, around movs r3, #3 and ands.
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
