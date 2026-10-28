#include "trial_road.h"

void Func_0200070c(s32 id, s32 value) {
  struct ObjectRuntime *object = Object_GetById(id);

  if (object != NULL) {
    object = object->watchedObject;
    if (object != NULL)
      OBJECT_BYTE(object, 99) = value;
  }
}
