#include "actor.h"

struct Actor *Actor_GetById(u32 objectId) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object == NULL)
    return NULL;
  return object;
}

void Actor_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->acceleration = acceleration;
    object->speedLimit = speedLimit;
  }
}

void Actor_SetMovementTypeById(u32 objectId, s32 action) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    s32 value = 1;
    value |= object->actionFlags;
    object->actionFlags = value;
    Actor_SetActionCallback(object, action);
  }
}

void Actor_Stop(u32 objectId) {
  struct Actor *object = ObjectTable_Get(objectId);
  u8 enabled;

  if (object != NULL) {
    enabled = 1;
    object->actionFlags = enabled | object->actionFlags;
    Actor_ResetMotion(object);
  }
}
