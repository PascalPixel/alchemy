#include "ride.h"

void Func_02001234(s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  if (object != NULL) {
    ObjectDispatch_SetSingleChildFlags(object, 0);
    object->collisionFlags = 0;
    object->motionFlags = 0;
  }
}
