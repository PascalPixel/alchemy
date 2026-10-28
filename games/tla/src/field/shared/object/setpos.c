#include "actor.h"

void Actor_WatchAndSetScript(u32 objectId, u32 linkedObjectId) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->watchedObject = Actor_GetById(linkedObjectId);
    Actor_SetActionCallback(object, (s32)gActor_FaceWatchedScript);
  }
}

void Actor_RefreshSelectorById(u32 objectId) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL)
    Actor_RefreshSelector(object);
}

void Actor_SetMovementTypeAndWaitById(u32 objectId, s32 action) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    s32 value = 1;
    value |= object->actionFlags;
    object->actionFlags = value;
    Actor_SetActionCallback(object, action);
    Actor_RefreshSelector(object);
  }
}

void Actor_ResetAndSetPosition(u32 objectId, s32 x, s32 z) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Actor_ResetMotion(object);
    Actor_SetTargetPosition(object, x << 16, object->y, z << 16);
  }
}

void Actor_SetPositionAndCommit(u32 objectId, s32 x, s32 z) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Actor_ResetMotion(object);
    Actor_SetTargetPosition(object, x << 16, object->y, z << 16);
    Actor_CommitPosition(object);
  }
}

void Actor_ResetAndSetPositionInMode2(u32 objectId, s32 x, s32 z) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Actor_ResetMotion(object);
    Actor_SetMode(object, 2);
    Actor_SetTargetPosition(object, x << 16, object->y, z << 16);
  }
}

void Actor_SetPositionAndReset(u32 objectId, s32 x, s32 z) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Actor_ResetMotion(object);
    Actor_SetMode(object, 2);
    Actor_SetTargetPosition(object, x << 16, object->y, z << 16);
    Actor_CommitPosition(object);
    Actor_SetMode(object, 1);
  }
}
