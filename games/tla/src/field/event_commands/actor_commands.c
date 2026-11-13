/// @file
/// Event commands on actors: placing, moving and animating them.
// Part of event_commands.c.

#include "../event_commands_internal.h"
#include "frame.h"
#include "module/field/event_commands.h"
#include "module/field/object.h"

struct Actor *Actor_GetById(u32 objectId) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object == NULL)
    return NULL;
  return object;
}

void Actor_SetSpeedParameters(u32 objectId, s32 speedLimit, s32 acceleration) {
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

void Actor_ResetTargetsAndVelocity(u32 objectId) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->actionFlags = (u8)(object->actionFlags | 1);
    Actor_ResetMotion(object);
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    SET_VELOCITY(object, 0, 0, 0);
    object->unk56 = 0;
  }
}

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

void Actor_MoveToAndWaitById(u32 objectId, s32 x, s32 z) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Actor_ResetMotion(object);
    Actor_SetTargetPosition(object, x << 16, object->y, z << 16);
    Actor_CommitPosition(object);
  }
}

void Actor_WalkToById(u32 objectId, s32 x, s32 z) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Actor_ResetMotion(object);
    Actor_SetMode(object, 2);
    Actor_SetTargetPosition(object, x << 16, object->y, z << 16);
  }
}

void Actor_WalkToAndWaitById(u32 objectId, s32 x, s32 z) {
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
    Actor_SetTargetPosition(object, x + ((eight - angleRemainder) << 16),
                            object->y, object->z);
    Actor_CommitPosition(object);
    Actor_SetSpritePriority(objectId, action);
    Actor_SetTargetPosition(object, object->x, object->y,
                            object->z + (zOffset << 16));
  }
}

void Actor_SetDestinationOffsetById(u32 objectId, s32 xOffset, s32 zOffset) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Actor_ResetMotion(object);
    Actor_SetTargetPosition(object, object->x + (xOffset << 16), object->y,
                            object->z + (zOffset << 16));
  }
}

void Actor_OffsetPositionAndReset(u32 objectId, s32 xOffset, s32 zOffset) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    Actor_StartScriptFar(object, (u32)gActor_OffsetScript);
    Actor_SetMode(object, 2);
    Actor_SetTargetPosition(object, object->x + (xOffset << 16), object->y,
                            object->z + (zOffset << 16));
  }
}

void Actor_WalkByAndWaitById(u32 objectId, s32 xOffset, s32 zOffset) {
  struct Actor *object = ObjectTable_Get(objectId);

  Actor_OffsetPositionAndReset(objectId, xOffset, zOffset);
  if (object != NULL) {
    Actor_CommitPosition(object);
    Actor_SetMode(object, 1);
  }
}

void Actor_LaunchFromLeader(u32 actorId, s32 xOffset, s32 zOffset, s32 action) {
  struct Actor *object = ObjectTable_Get(actorId);

  if (object != NULL) {
    Actor_SetSpeedParameters(actorId, 0xcccc, 0x6666);
    Actor_MoveToObject(actorId, gMainRoutineWork.currentOwner);
    Actor_SetPositionAndResetMotionFar(object, object->x, object->y,
                                       object->z - 0x10000);
    object->movementState = 0;
    Actor_SetMode(object, 2);
    Actor_SetDestinationOffsetById(actorId, xOffset, zOffset);
    Actor_StartScriptFar(object, gData_080f32e0);
    object->timer = action;
  }
}

void Actor_WaitForMoveById(u32 objectId) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    Actor_CommitPosition(object);
    Actor_SetMode(object, 1);
  }
}

#define Actor_SetPositionAndResetMotionFar                                     \
  ((s32 (*)(struct Actor *, s32, s32, s32))Actor_SetPositionAndResetMotionFar)

void Actor_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z) {
  struct Actor *object = ObjectTable_Get(objectId);
  s32 terrainHeight;

  if (object != NULL) {
    Actor_ResetMotion(object);
    Actor_SetPositionAndResetMotionFar(object, x, object->y, z);
    if (object->motionFlags & 1) {
      terrainHeight =
          MapGrid_GetTerrainHeightFar(object->terrainId, object->x, object->z);
      object->y = object->y - object->terrainHeight + terrainHeight;
      object->terrainHeight = terrainHeight;
    }
  }
}

void Actor_SetHorizontalPositionAndAngleWithTerrain(u32 objectId, s32 x, s32 z,
                                                    s16 angle) {
  struct Actor *object = ObjectTable_Get(objectId);
  s32 terrainHeight;

  if (object != NULL) {
    Actor_ResetMotion(object);
    Actor_SetPositionAndResetMotionFar(object, x, object->y, z);
    object->facing = angle;
    if (object->motionFlags & 1) {
      terrainHeight =
          MapGrid_GetTerrainHeightFar(object->terrainId, object->x, object->z);
      object->y = object->y - object->terrainHeight + terrainHeight;
      object->terrainHeight = terrainHeight;
    }
  }
}

void Actor_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z) {
  struct Actor *object = ObjectTable_Get(objectId);
  s32 terrainHeight;

  if (object != NULL) {
    Actor_ResetMotion(object);
    Actor_SetPositionAndResetMotionFar(object, x, y, z);
    if (object->motionFlags & 1) {
      terrainHeight =
          MapGrid_GetTerrainHeightFar(object->terrainId, object->x, object->z);
      object->y = object->y - object->terrainHeight + terrainHeight;
      object->terrainHeight = terrainHeight;
    }
  }
}

void Actor_MoveToObject(u32 objectId, u32 otherId) {
  struct Actor *object = ObjectTable_Get(objectId);
  struct Actor *other = ObjectTable_Get(otherId);

  if (object != NULL && other != NULL) {
    Actor_SetPositionAndResetMotionFar(object, other->x, other->y, other->z);
    object->terrainHeight = other->terrainHeight;
  }
}

void Actor_SetModeById(u32 objectId, s32 action) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL)
    Actor_SetMode(object, action);
}

#define Actor_SelectAnimationFar                                               \
  ((void (*)(struct Actor *, s32))Actor_SelectAnimationFar)

void Actor_SetActionById(u32 objectId, s32 action) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL)
    Actor_SelectAnimationFar(object, action);
}

/// ⚓️'s animation group: the number last applied and its entries.
/* VIEW(event_commands): tag struct AnimationGroup as struct
 * AnimationGroup__event_commands; module/field/field.h has another definition.
 */
struct AnimationGroup__event_commands {
  u8 unk00[0x18];
  u8 lastNo;
  u8 unk19[0x0f];
  struct AnimationEntry *entries[4];
};

/// Waits, at most 90 frames, for an actor's animation to change or for its
/// first entry's script to reach code 241.
void Actor_WaitForAnimationChange(u32 objectId) {
  struct Actor *object = ObjectTable_Get(objectId);
  struct AnimationGroup__event_commands *group;
  // volatile: saved lives in a 4-byte stack frame, stored once and loaded again
  // for each compare, as the bytes have it; as a plain u32, 29 of 37
  // instructions differ in tla-en.
  volatile u32 saved;
  s32 elapsedFrames;

  if (object != NULL && object->animationKind == 1) {
    group = object->sprite;
    saved = group->lastNo;
    for (elapsedFrames = 0; elapsedFrames <= 89; elapsedFrames++) {
      wait(1);
      if (saved != group->lastNo)
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
    Actor_StartScriptFar(object,
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

struct Actor *Actor_CreateFar(s32 kind, s32 x, s32 y, s32 z);

struct ParticleSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[16];
  u8 flags;
};

// Kicks up a dust puff (object 285) where source stands, as ☀️'s
// FieldEffect_SpawnBurstParticle.
void FieldEffect_SpawnDustPuff(struct Actor *source, s32 param) {
  struct Actor *object;
  struct ParticleSprite *child;
  s32 speed;
  s32 rate = -0x1999;

  object = Actor_CreateFar(285, source->x, source->y, source->z);
  if (object != NULL) {
    child = object->sprite;
    switch (Random16() & 1) {
    case 1:
      Actor_SetMode(object, 2);
      Actor_StartScriptFar(object, Data_080f085c);
      break;
    default:
      Actor_SetMode(object, 1);
      Actor_StartScriptFar(object, Data_080f0874);
      break;
    }

    if (param != 0)
      Actor_SetPartAttribute(object, param);

    object->motionFlags = 0;
    speed = Random16() % 10 + 5;
    object->acceleration = rate * speed;
    speed = Random16() % 15 - 7;
    speed <<= 1;
    object->speedLimit = 0x1999 * speed;
    object->timer = 0;
    object->priorityFlags |= 2;
    ((u8 *)object)[0x61] = 1;
    object->update = (void (*)(void))FieldEffect_UpdateParticleLinearMotion;
    child->flags = 0;
    child->priority = ((struct ParticleSprite *)source->sprite)->priority;
  }
}

void Actor_ArmCallback(s32 id, s32 value, s32 mode);

/// Makes an object jump down off a ledge in front of it: it plays `mode` for 10
/// frames, hops 12 pixels forward from its cell's centre line, then sinks 2
/// pixels a frame for 14 frames from cue 217, dropping a particle of kind
/// `particle` every other frame (none for -1), and jumps on 16 pixels to land,
/// waiting up to 180 frames for it unless `particle` has flag 0x1000. The
/// Broken Seal's twin is Actor_JumpDownLedge.
// The landing wait matches only as a guarded do-while: as
// while (y > ground) { wait(1); if (++i >= 180) break; } agscc keeps
// the test at the top and jumps back to it (differ 18), where the native has
// the test copied below the body as loop rotation would place it.
void Actor_JumpDownLedge(s32 id, s32 mode, s32 particle) {
  struct Actor *object = Actor_GetById(id);
  s32 z = object->z;
  s32 x = (object->x & 0xfff00000) + 0x80000;
  s32 frame;
  u32 i;

  if (object == NULL)
    return;
  frame = ((struct FieldSprite *)object->sprite)->nextFrame;
  Actor_SetMode(object, mode);
  wait(10);
  Actor_SetMode(object, 1);
  Actor_SetSingleChildFlagsFar(object, 0);
  object->motionFlags |= 2;
  object->velocityY = 0x40000;
  Actor_SetTargetPosition(object, x, object->y, z + 0xc0000);
  wait(6);
  Sound_PlayFar(217);
  Actor_ArmCallback(id, 0x5000, 0);
  object->motionFlags = 0;
  for (i = 0; i < 14; i++) {
    object->y -= 0x20000;
    object->targetY = object->y;
    wait(1);
    if (particle != -1 && (i & 1))
      FieldEffect_SpawnDustPuff(object, particle);
  }
  object->motionFlags = 3;
  object->velocityY = 0x30000;
  Actor_SetTargetPosition(object, x, object->y, z + 0x100000);
  Actor_CommitPosition(object);
  if (particle & 0x1000) {
    wait(4);
    return;
  }
  i = 0;
  if (object->y > object->terrainHeight) {
    do {
      wait(1);
      if (++i >= 180)
        break;
    } while (object->y > object->terrainHeight);
  }
  Actor_SetSingleChildFlagsFar(object, frame);
  wait(2);
  Event_NoOp();
}

void Actor_FaceActorById(s32 firstId, s32 secondId, s32 frames) {
  struct ObjectPairPosition *first;
  struct ObjectPairPosition *second;

  first = ObjectTable_Get(firstId);
  second = ObjectTable_Get(secondId);
  if (first != 0 && second != 0) {
    first->angle = ArcTan2(second->z - first->z, second->x - first->x);
    Event_Wait(frames);
  }
}

void Actor_TurnToFaceEachOther(s32 firstId, s32 secondId, s32 frames) {
  void *first = ObjectTable_Get(firstId);
  void *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    Actor_TurnPairToFaceEachOther(first, second);
    Event_Wait(frames);
  }
}

void Actor_TurnPairToFaceEachOther(struct FacingObject *first,
                                   struct FacingObject *second) {
  s32 toward;
  s32 away;
  s32 frame;
  s32 turning;
  s32 delta;

  if (first == NULL || second == NULL)
    return;
  toward = (u16)ArcTan2(second->positionZ - first->positionZ,
                        second->positionX - first->positionX);
  away = toward + 0x8000;
  for (frame = 0; frame < 60; frame++) {
    delta = (s16)(toward - first->facing);
    turning = 2;
    if (delta != 0) {
      if (delta > 0x1000)
        delta = 0x1000;
      if (delta < -0x1000)
        delta = -0x1000;
      first->facing += delta;
    } else {
      turning = 1;
    }
    delta = (s16)(away - second->facing);
    if (delta != 0) {
      if (delta > 0x1000)
        delta = 0x1000;
      if (delta < -0x1000)
        delta = -0x1000;
      second->facing += delta;
    } else {
      turning--;
    }
    if (turning == 0)
      break;
    wait(1);
  }
}

void ObjectTable_DestroyById(s32 id) {
  void *object = ObjectTable_Get(id);
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (object != NULL) {
    Actor_Destroy(object);
    work->objects[id] = NULL;
  }
}

void Actor_ConfigureSpriteColor(s32 objectId, s32 value) {
  struct Actor *object;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    if (value & 0x100) {
      object->update = Actor_ApplyIndexedChildValue;
      return;
    }
    object->update = NULL;
    Actor_SetPartAttribute(object, value);
  }
}

void Actor_ApplyIndexedChildValue(struct Actor *object) {
  struct ObjectPartList *list;
  struct ObjectPart *part;
  s32 attribute;
  s32 i;

  if ((object->animationKind & 0xf) == 1) {
    list = object->sprite;
    attribute = gActor_BlinkChildValues[(gFrameCount >> 1) & 3];
    for (i = 0; i < list->count; i++) {
      part = list->parts[i];
      if (part != 0 && part->graphics != 0) {
        part->attribute = attribute;
      }
    }
    list->dirty = 1;
  }
}

void Actor_SetPartAttribute(struct Actor *object, s32 attribute) {
  struct ObjectPartList *list;
  struct ObjectPart *part;
  s32 i;

  if ((object->animationKind & 0xf) == 1) {
    list = object->sprite;
    for (i = 0; i < list->count; i++) {
      part = list->parts[i];
      if (part != 0 && part->graphics != 0) {
        part->attribute = attribute;
      }
    }
    list->dirty = 1;
  }
}

/// Links an object to the object in the low 8 bits of targetId and starts
/// callback on it. Unless bit 16 of targetId is set it takes a follow
/// distance of 40 pixels (action 40), the target's top speed with twice its
/// acceleration, to keep up with it, and no collision flags, so it cannot
/// block the target. The Broken Seal's ObjectEvent_SetTargetAndCallback.
void Actor_SetTargetAndCallback(u32 objectId, s32 targetId,
                                const void *callback) {
  struct Actor *follower = ObjectTable_Get(objectId);
  struct Actor *target = ObjectTable_Get(targetId & 0xff);

  if (follower != NULL && target != NULL) {
    follower->watchedObject = target;
    if (!(targetId & 0x10000)) {
      u16 none = 0;

      follower->timer = 40;
      follower->acceleration = target->acceleration * 2;
      follower->speedLimit = target->speedLimit;
      follower->collisionFlags = none;
    }
    Actor_StartScriptFar(follower, callback);
  }
}
