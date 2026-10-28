#include "actor.h"
#include "objdisp.h"
#include "main_routine_work.h"

void Actor_CenterOnColumn(u32 objectId, s32 action, s32 zOffset) {
  struct Actor *object;
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
    Actor_ResetMotion(object);
    Actor_SetMode(object, 2);
    x = object->x;
    eight = 8;
    /* FAKEMATCH: under the ARM9TDMI load schedule the column offset's 8 is
     * built before the x load; the barrier loads x first with neither value
     * changed. */
    __asm__("" : "+r"(eight) : "r"(x));
    Actor_SetTargetPosition(object, x + ((eight - angleRemainder) << 16), object->y,
                       object->z);
    Actor_CommitPosition(object);
    Actor_SetSpritePriority(objectId, action);
    Actor_SetTargetPosition(object, object->x, object->y,
                       object->z + (zOffset << 16));
  }
}

void Actor_OffsetPositionAndResetMotion(u32 objectId, s32 xOffset,
                                               s32 zOffset) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Actor_ResetMotion(object);
    Actor_SetTargetPosition(object, object->x + (xOffset << 16), object->y,
                       object->z + (zOffset << 16));
  }
}

void Actor_OffsetPositionAndReset(u32 objectId, s32 xOffset,
                                         s32 zOffset) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Actor_StartScriptFar((struct ActorView *)object,
                                 (u32)gActor_OffsetScript);
    Actor_SetMode(object, 2);
    Actor_SetTargetPosition(object, object->x + (xOffset << 16), object->y,
                       object->z + (zOffset << 16));
  }
}

void Actor_CommitPositionAndActivate(u32 objectId, s32 xOffset,
                                            s32 zOffset) {
  struct Actor *object = ObjectTable_Get(objectId);

  Actor_OffsetPositionAndReset(objectId, xOffset, zOffset);
  if (object != NULL) {
    Actor_CommitPosition(object);
    Actor_SetMode(object, 1);
  }
}

void Actor_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration);
void Actor_MoveToObject(u32 objectId, s32 targetId);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x,
                                         s32 y, s32 z);
extern const s32 gData_080f32e0[];

void Func_080d3070(u32 objectId, s32 xOffset, s32 zOffset, s32 action) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    Actor_SetSpeedParameters(objectId, 0xcccc, 0x6666);
    Actor_MoveToObject(objectId, gMainRoutineWork.currentOwner);
    Actor_SetPositionAndResetMotionFar(object, object->x, object->y,
                                        object->z - 0x10000);
    object->movementState = 0;
    Actor_SetMode(object, 2);
    Actor_OffsetPositionAndResetMotion(objectId, xOffset, zOffset);
    Actor_StartScriptFar((struct ActorView *)object,
                                 gData_080f32e0);
    object->action = action;
  }
}
