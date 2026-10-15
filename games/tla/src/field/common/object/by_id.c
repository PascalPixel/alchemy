#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object == NULL)
    return NULL;
  return object;
}

void ObjectMotion_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->acceleration = acceleration;
    object->speedLimit = speedLimit;
  }
}

void ObjectMotion_EnableActionAndSetCallback(u32 objectId, s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    s32 value = 1;
    value |= object->actionFlags;
    object->actionFlags = value;
    Object_SetActionCallback(object, action);
  }
}

void ObjectMotion_EnableActionAndResetMotion(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);
  u8 enabled;

  if (object != NULL) {
    enabled = 1;
    object->actionFlags = enabled | object->actionFlags;
    Object_ResetMotion(object);
  }
}
