#include "object_runtime.h"

void Object_SetModeById(u32 objectId, s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    Object_SetMode(object, action);
}
