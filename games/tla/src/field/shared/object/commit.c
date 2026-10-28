#include "object_runtime.h"

void ObjectMotion_CommitCurrentPositionAndActivate(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    Object_CommitPosition(object);
    Object_SetMode(object, 1);
  }
}
