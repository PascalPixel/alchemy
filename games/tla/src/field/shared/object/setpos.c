#include "object_runtime.h"

void Object_LinkObjectAndSetCallback(u32 objectId, u32 linkedObjectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->watchedObject = Object_GetById(linkedObjectId);
    Object_SetActionCallback(object, (s32)gObject_LinkedMotionScript);
  }
}

void Object_RefreshSelectorById(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    Object_RefreshSelector(object);
}

void Object_SetActionCallbackAndRefreshById(u32 objectId, s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    s32 value = 1;
    value |= object->actionFlags;
    object->actionFlags = value;
    Object_SetActionCallback(object, action);
    Object_RefreshSelector(object);
  }
}

void ObjectMotion_ResetAndSetPosition(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Object_ResetMotion(object);
    Object_SetPosition(object, x << 16, object->y, z << 16);
  }
}

void ObjectMotion_SetPositionAndCommit(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Object_ResetMotion(object);
    Object_SetPosition(object, x << 16, object->y, z << 16);
    Object_CommitPosition(object);
  }
}

void ObjectMotion_ResetAndSetPositionInMode2(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Object_ResetMotion(object);
    Object_SetMode(object, 2);
    Object_SetPosition(object, x << 16, object->y, z << 16);
  }
}

void ObjectMotion_SetPositionAndReset(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Object_ResetMotion(object);
    Object_SetMode(object, 2);
    Object_SetPosition(object, x << 16, object->y, z << 16);
    Object_CommitPosition(object);
    Object_SetMode(object, 1);
  }
}
