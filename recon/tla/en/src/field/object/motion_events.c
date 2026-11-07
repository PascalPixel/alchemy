#include "event_work.h"
#include "actor.h"
#include "objdisp.h"
#include "sprite.h"
#include "system.h"

/// ⚓️'s animation group: the number last applied and its entries.
struct AnimationGroup {
  u8 unk00[0x18];
  u8 lastNo;
  u8 unk19[0x0f];
  struct AnimationEntry *entries[4];
};

/// Waits, at most 90 frames, for an actor's animation to change or for its
/// first entry's script to reach code 241.
void Actor_WaitForAnimationChange(u32 objectId) {
  struct Actor *object = ObjectTable_Get(objectId);
  struct AnimationGroup *group;
  u32 saved;
  /* FAKEMATCH(pin): the saved number stored and read again through a pointer
   * held in r6, as the native's str and ldr through r6 show; as a plain
   * pointer, 12 of 37 instructions differ in tla-en. */
  register u32 *savedNo asm("r6");
  s32 elapsedFrames;

  if (object != NULL && object->animationKind == 1) {
    group = object->animation;
    savedNo = &saved;
    *savedNo = group->lastNo;
    for (elapsedFrames = 0; elapsedFrames <= 89; elapsedFrames++) {
      wait(1);
      if (*savedNo != group->lastNo)
        break;
      if (group->entries[0] != NULL && group->entries[0]->script != NULL &&
          group->entries[0]->script[group->entries[0]->pos] == 241)
        break;
    }
  }
}

void Motion_SetModeAndWaitAnimation(u32 objectId, s32 action) {
  Actor_SetModeById(objectId, action);
  Actor_WaitForAnimationChange(objectId);
}

void Actor_Jump(u32 objectId, s32 speed, s32 eventId) {
  struct Actor *object;
  u8 *objectFlags;
  s32 updatedFlags;
  s32 verticalVelocity;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    objectFlags = &object->motionFlags;
    updatedFlags = *objectFlags | 2;
    verticalVelocity = speed << 16;
    *objectFlags = updatedFlags;
    object->velocityY = verticalVelocity;
    if (speed > 5) {
      Sound_PlayFar(0x99);
    } else {
      Sound_PlayFar(0x98);
    }
    Event_Wait(eventId);
  }
}

void Actor_StartRepeatedMotion(u32 objectId, s32 variant) {
  struct Actor *object;

  object = ObjectTable_Get(objectId);
  if (object != NULL && variant > 0) {
    if (variant > 3)
      variant = 3;
    Actor_StartScriptFar(
        (struct ActorView *)object,
        (u32)(gActor_VariantScripts + ((3 - variant) << 7)));
  }
}

void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant) {
  Actor_StartRepeatedMotion(objectId, variant);
  Actor_RefreshSelectorById(objectId);
}

void FieldEffect_UpdateParticleLinearMotion(struct Actor *object) {
  s32 driftX;
  s32 driftZ;
  s32 pos;

  driftX = object->speedLimit;
  pos = object->x;
  driftZ = object->acceleration;
  object->x = pos + driftX;
  object->z += driftZ;
  object->y += 0x400;
  object->speedLimit = driftX - driftX / 18;
  object->acceleration = driftZ - driftZ / 16;
}
