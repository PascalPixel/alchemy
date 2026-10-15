#include "object_runtime.h"
#include "objdisp.h"
#include "party_state.h"

void ObjectMotion_CenterOnColumn(u32 objectId, s32 action, s32 zOffset) {
  struct ObjectRuntime *object;
  s16 currentAngle;
  s32 snappedAngle;
  s16 angleRemainder;
  s32 x;
  s32 eight;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    currentAngle = *(s16 *)((u8 *)object + 0x0a);
    snappedAngle = currentAngle;
    if (currentAngle < 0) {
      snappedAngle += 15;
    }
    snappedAngle >>= 4;
    snappedAngle *= 16;
    angleRemainder = currentAngle - snappedAngle;
    object->movementState = 0;
    Object_ResetMotion(object);
    Object_SetMode(object, 2);
    x = object->x;
    eight = 8;
    /* FAKEMATCH: under the ARM9TDMI load schedule the column offset's 8 is
     * built before the x load; the barrier loads x first with neither value
     * changed. */
    __asm__("" : "+r"(eight) : "r"(x));
    Object_SetPosition(object, x + ((eight - angleRemainder) << 16), object->y,
                       object->z);
    Object_CommitPosition(object);
    ObjectMotion_SetActionVariant(objectId, action);
    Object_SetPosition(object, object->x, object->y,
                       object->z + (zOffset << 16));
  }
}

void ObjectMotion_OffsetPositionAndResetMotion(u32 objectId, s32 xOffset,
                                               s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Object_ResetMotion(object);
    Object_SetPosition(object, object->x + (xOffset << 16), object->y,
                       object->z + (zOffset << 16));
  }
}

void ObjectMotion_OffsetPositionAndReset(u32 objectId, s32 xOffset,
                                         s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                 (u32)gObject_OffsetMotionScript);
    Object_SetMode(object, 2);
    Object_SetPosition(object, object->x + (xOffset << 16), object->y,
                       object->z + (zOffset << 16));
  }
}

void ObjectMotion_CommitPositionAndActivate(u32 objectId, s32 xOffset,
                                            s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  ObjectMotion_OffsetPositionAndReset(objectId, xOffset, zOffset);
  if (object != NULL) {
    Object_CommitPosition(object);
    Object_SetMode(object, 1);
  }
}

void ObjectMotion_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration);
void ObjectMotion_MoveToObject(u32 objectId, s32 targetId);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z);
extern const s32 gData_080f32e0[];

void Func_080d3070(u32 objectId, s32 xOffset, s32 zOffset, s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    ObjectMotion_SetSpeedParameters(objectId, 0xcccc, 0x6666);
    ObjectMotion_MoveToObject(objectId, gPartyState.currentOwner);
    Object_SetPositionAndResetMotionFar(object, object->x, object->y,
                                        object->z - 0x10000);
    object->movementState = 0;
    Object_SetMode(object, 2);
    ObjectMotion_OffsetPositionAndResetMotion(objectId, xOffset, zOffset);
    ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                 gData_080f32e0);
    object->action = action;
  }
}
