/// @file
/// Event commands on actors: placing, moving and animating them.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

/// Returns an actor, or NULL if it does not exist.
struct FieldActor *Object_GetById(s32 actor) {
  struct FieldActor *object = ObjectTable_Get(actor);

  if (object == NULL)
    return NULL;
  return object;
}

/// The event command "set actor speed": its top speed and acceleration, 16.16
/// pixels per frame.
void ObjectMotion_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->acceleration = acceleration;
    object->speedLimit = speedLimit;
  }
}

/// Gives an actor an action script (ObjectMotion_SetActionCallback) and lets
/// it run.
void ObjectMotion_EnableActionAndSetCallback(u32 objectId, s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    s32 value = 1;
    value |= object->actionFlags;
    object->actionFlags = value;
    ObjectMotion_SetActionCallback(object, action);
  }
}

/// The event command "stop actor": stops its action and puts it back under
/// event control (dispatch script 6).
void ObjectMotion_EnableActionAndResetMotion(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);
  u8 enabled;

  if (object != NULL) {
    enabled = 1;
    object->actionFlags = enabled | object->actionFlags;
    ObjectDispatch_StartIdleFar(object);
  }
}

/// Makes an actor follow another (the linked action script).
void Object_LinkObjectAndSetCallback(u32 objectId, u32 linkedObjectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->linkedObject =
        (struct ObjectRuntime *)Object_GetById(linkedObjectId);
    ObjectMotion_SetActionCallback(object,
                                   (s32)gObjectMotion_LinkedActionScript);
  }
}

/// Waits, at most 300 frames, for an actor's script to reach its end
/// (command 0x10).
void Object_RefreshSelectorById(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    ObjectDispatch_WaitForValue16Far(object);
}

/// Starts a built-in action or supplied script, then waits up to 300 frames for
/// its script to end. Scene script calls use Actor_RunActionScriptAndWait.
void Object_SetActionCallbackAndRefreshById(u32 objectId, s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    s32 value = 1;
    value |= object->actionFlags;
    object->actionFlags = value;
    ObjectMotion_SetActionCallback(object, action);
    ObjectDispatch_WaitForValue16Far(object);
  }
}

/// The event command "set actor destination": the actor heads for (x, z), in
/// pixels, keeping its current animation.
void ObjectMotion_ResetAndSetPosition(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar(object);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
  }
}

/// The event command "move actor and wait": as
/// ObjectMotion_ResetAndSetPosition, then waits (at most 600 frames) for the
/// actor to arrive.
void ObjectMotion_SetPositionAndCommit(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar(object);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
    FieldObject_WaitForMoveFar(object);
  }
}

/// The event command "walk actor to": the actor walks (animation 2) toward
/// (x, z), in pixels.
void ObjectMotion_ResetAndSetPositionInMode2(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar(object);
    ObjectDispatch_SelectAnimationFar(object, 2);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
  }
}

/// The event command "walk actor to and wait": walks the actor to (x, z), in
/// pixels, waits for it to arrive and stands it still (animation 1).
void ObjectMotion_SetPositionAndReset(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar(object);
    ObjectDispatch_SelectAnimationFar(object, 2);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
    FieldObject_WaitForMoveFar(object);
    ObjectDispatch_SelectAnimationFar(object, 1);
  }
}

/// The event command "center actor and walk": walks the actor sideways to the
/// middle of its 16-pixel column, sets its sprite priority to priority
/// (ObjectMotion_SetActionVariant), then sends it zOffset pixels along z.
void ObjectMotion_CenterOnColumn(u32 objectId, s32 priority, s32 zOffset) {
  struct ObjectRuntime *object;
  s16 pixelX;
  s32 columnX;
  s16 offsetInColumn;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    pixelX = ((union FieldCoordinate *)&object->x)->part.pixel;
    columnX = pixelX;
    if (pixelX < 0) {
      columnX += 15;
    }
    columnX >>= 4;
    columnX *= 16;
    offsetInColumn = pixelX - columnX;
    object->movementState = 0;
    ObjectDispatch_StartIdleFar(object);
    ObjectDispatch_SelectAnimationFar(object, 2);
    Object_SetMoveTargetFar(object, object->x + ((8 - offsetInColumn) << 16),
                            object->y, object->z);
    FieldObject_WaitForMoveFar(object);
    ObjectMotion_SetActionVariant(objectId, priority);
    Object_SetMoveTargetFar(object, object->x, object->y,
                            object->z + (zOffset << 16));
  }
}

/// The event command "set actor destination offset": the actor heads
/// (xOffset, zOffset) pixels from where it stands.
void ObjectMotion_OffsetPositionAndResetMotion(u32 objectId, s32 xOffset,
                                               s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar(object);
    Object_SetMoveTargetFar(object, object->x + (xOffset << 16), object->y,
                            object->z + (zOffset << 16));
  }
}

/// Walks the actor (animation 2) (xOffset, zOffset) pixels from where it
/// stands.
void ObjectMotion_OffsetPositionAndReset(u32 objectId, s32 xOffset,
                                         s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar(object);
    ObjectDispatch_SelectAnimationFar(object, 2);
    Object_SetMoveTargetFar(object, object->x + (xOffset << 16), object->y,
                            object->z + (zOffset << 16));
  }
}

/// Walks the actor (xOffset, zOffset) pixels, waits for it to arrive and
/// stands it still.
void ObjectMotion_CommitPositionAndActivate(u32 objectId, s32 xOffset,
                                            s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  ObjectMotion_OffsetPositionAndReset(objectId, xOffset, zOffset);
  if (object != NULL) {
    FieldObject_WaitForMoveFar(object);
    ObjectDispatch_SelectAnimationFar(object, 1);
  }
}

/// Places an actor on the leader and sends it walking (xOffset, zOffset)
/// pixels away at speed 0.6 and acceleration 0.3, through the launch script;
/// the actor then takes action action.
void Motion_LaunchFromFocusedObject(u32 objectId, s32 xOffset, s32 zOffset,
                                    s32 action) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    struct ObjectRuntime *leader;

    ObjectMotion_SetSpeedParameters(objectId, 0x9999, 0x4CCC);
    leader = (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
    if (leader != NULL)
      ObjectMotion_SetHorizontalPositionWithTerrain(objectId, leader->x,
                                                    leader->z);
    object->movementState = 0;
    ObjectDispatch_SelectAnimationFar(object, 2);
    ObjectMotion_OffsetPositionAndResetMotion(objectId, xOffset, zOffset);
    ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                 (const s32 *)&gObjectMotion_LaunchScript);
    object->action = action;
  }
}

/// The event command "wait for actor": waits (at most 600 frames) for the actor
/// to arrive and stands it still.
void ObjectMotion_CommitCurrentPositionAndActivate(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    FieldObject_WaitForMoveFar(object);
    ObjectDispatch_SelectAnimationFar(object, 1);
  }
}

/// The event command "set actor position": puts the actor at (x, z), 16.16
/// pixels, at rest. An actor that follows the ground (flag 1) keeps its height
/// above the terrain at the new place.
void ObjectMotion_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object;
  s32 terrainHeight;
  s32 tileX;
  s32 tileZ;
  s32 terrainId;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    ObjectDispatch_StartIdleFar(object);
    SET_VELOCITY(object, 0, 0, 0);
    object->targetY = ACTOR_NO_TARGET;
    object->targetX = ACTOR_NO_TARGET;
    object->x = x;
    object->z = z;
    if (1 & object->flags) {
      terrainId = object->terrainId;
      tileX = x;
      if (tileX < 0) {
        tileX += 0xFFFF;
      }
      tileX >>= 0x10;
      tileZ = z;
      if (tileZ < 0) {
        tileZ += 0xFFFF;
      }
      tileZ >>= 0x10;
      terrainHeight = Map_GetTerrainHeightFar(terrainId, tileX, tileZ) << 0x10;
      object->y = (object->y - object->terrainHeight) + terrainHeight;
      object->terrainHeight = terrainHeight;
    }
  }
}

/// Puts the actor at (x, y, z), 16.16 pixels, at rest; an actor that follows
/// the ground (flag 1) is raised by the terrain height there.
void ObjectMotion_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z) {
  struct ObjectRuntime *object;
  s32 terrainHeight;
  s32 tileX;
  s32 tileZ;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    ObjectDispatch_StartIdleFar(object);
    SET_VELOCITY(object, 0, 0, 0);
    object->targetY = ACTOR_NO_TARGET;
    object->targetX = ACTOR_NO_TARGET;
    SET_POSITION(object, x, y, z);
    if (1 & object->flags) {
      s32 terrainId = object->terrainId;
      tileX = x;
      if (tileX < 0) {
        tileX += 0xFFFF;
      }
      tileX >>= 0x10;
      tileZ = z;
      if (tileZ < 0) {
        tileZ += 0xFFFF;
      }
      terrainHeight = Map_GetTerrainHeightFar(terrainId, tileX, tileZ >> 0x10)
                      << 0x10;
      object->y = (object->y - object->terrainHeight) + terrainHeight;
      object->terrainHeight = terrainHeight;
    }
  }
}

/// The event command "set actor animation" (1 stands, 2 walks, 3 nods, 4 shakes
/// the head and so on).
void Object_SetModeById(u32 objectId, s32 animation) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    ObjectDispatch_SelectAnimationFar(object, animation);
}

/// Sets the animation timer steps of each of an actor's sprites
/// (AnimationObjects_SetActiveTimerSteps).
void Object_SetActionById(u32 objectId, s32 steps) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)object, steps);
}

/// Waits, at most 90 frames, for an actor's animation to change.
void ObjectMotion_WaitForAnimationChange(u32 objectId) {
  struct ObjectRuntime *object;
  struct AnimationObject *sprite;
  /* FAKEMATCH(volatile): saved lives in a 4-byte frame, stored once and
   * loaded again for each compare, as the bytes have it; as a plain s32, 29
   * of 31 instructions differ in tbs-en. */
  volatile s32 saved;
  s32 elapsedFrames;

  object = ObjectTable_Get(objectId);
  if (object != NULL && object->animationKind == 1) {
    sprite = object->animation;
    saved = sprite->lastNo;
    for (elapsedFrames = 0; elapsedFrames <= 89; elapsedFrames++) {
      WaitFrames(1);
      if (saved != sprite->lastNo) {
        break;
      }
    }
  }
}

/// The event command "set actor animation and wait": plays the animation and
/// waits for it to end.
void Motion_SetModeAndWaitAnimation(u32 objectId, s32 animation) {
  Object_SetModeById(objectId, animation);
  ObjectMotion_WaitForAnimationChange(objectId);
}

/// Does nothing.
void ObjectMotion_NoOp(void) {}

/// The event command "actor jump": throws the actor upward at speed pixels per
/// frame with a jump sound (a higher one above 5), then waits frames frames.
void ObjectMotion_Launch(u32 objectId, s32 speed, s32 frames) {
  struct ObjectRuntime *object;
  u8 *objectFlags;
  s32 updatedFlags;
  s32 verticalVelocity;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    objectFlags = &object->flags;
    updatedFlags = *objectFlags | 2;
    verticalVelocity = speed << 16;
    *objectFlags = updatedFlags;
    object->velocityY = verticalVelocity;
    if (speed > 5) {
      AudioCommand_PlayFar(SE_HIGH_JUMP);
    } else {
      AudioCommand_PlayFar(SE_JUMP);
    }
    FieldEvent_WaitFrames(frames);
  }
}

/// The event command "start repeated motion": the actor squashes and springs
/// back count times (1 to 3), through gObjectMotion_VariantScripts.
void ObjectMotion_SetVariantCallback(u32 objectId, s32 count) {
  struct ObjectRuntime *object;

  object = ObjectTable_Get(objectId);
  if (object != NULL && count > 0) {
    if (count > 3) {
      count = 3;
    }
    // Each count is 32 words of the script; start that many from its end.
    ObjectDispatch_InitializeFar(
        (struct DispatchObject *)object,
        (const s32 *)(gObjectMotion_VariantScripts + ((3 - count) << 7)));
  }
}

/// The event command "run repeated motion": as ObjectMotion_SetVariantCallback,
/// then waits for it to end.
void Motion_SetVarCbAndRefresh(u32 objectId, s32 count) {
  ObjectMotion_SetVariantCallback(objectId, count);
  Object_RefreshSelectorById(objectId);
}

/// Moves a dust puff of FieldFx_SpawnBurstParticle: it drifts with its x and
/// z velocity (kept in speed and acceleration), which drag slows by 1/18 and
/// 1/16 each frame, and rises 1/64 pixel a frame.
void FieldFx_UpdateParticleLinearMotion(union FieldObject *fieldObject) {
  struct FieldActor *particle = (struct FieldActor *)fieldObject;
  s32 velocityX;
  s32 x;
  s32 velocityZ;
  s32 z;
  s32 y;
  s32 roundedZ;
  velocityX = particle->speed;
  x = particle->x.fixed + velocityX;
  particle->x.fixed = x;
  particle->targetX = x;
  velocityZ = particle->acceleration;
  z = particle->z.fixed + velocityZ;
  particle->z.fixed = z;
  particle->targetZ = z;
  y = particle->y.fixed + 0x400;
  particle->y.fixed = y;
  particle->targetY = y;
  particle->speed = (velocityX - velocityX / 0x12);

  roundedZ = velocityZ;
  if (velocityZ < 0) {
    roundedZ += 0xF;
  }
  particle->acceleration = (velocityZ - (roundedZ >> 4));
}

/// Kicks up a dust puff (object 222) where source stands: it flies off toward
/// the back at 0.5 to 1.4 pixels a frame and sideways at up to 1.4, with one
/// of two animations. A nonzero param is given to its sprite parts
/// (ObjectGroup_SetChildValue).
void FieldFx_SpawnBurstParticle(struct FieldActor *source, s32 param) {
  struct FieldActor *object;
  struct AnimationObject *child;
  s32 speed;

  object = (struct FieldActor *)Object_CreateFar(
      222, source->x.fixed, source->y.fixed, source->z.fixed);
  if (object != 0) {
    child = object->sprite;
    switch (Random16() & 1) {
    case 1:
      ObjectDispatch_SelectAnimationFar(object, 2);
      ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                   (const s32 *)gBattleFx_BurstParticleScriptA);
      break;
    default:
      ObjectDispatch_SelectAnimationFar(object, 1);
      ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                   (const s32 *)gBattleFx_BurstParticleScriptB);
      break;
    }

    if (param != 0)
      ObjectGroup_SetChildValue(object, param);

    // speed and acceleration hold the puff's x and z velocity.
    object->motionFlags = 0;
    speed = Random16() % 10 + 5;
    object->acceleration = -0x1999 * speed;
    speed = Random16() % 15 - 7;
    speed <<= 1;
    object->speed = 0x1999 * speed;
    object->timer = 0;
    object->update = FieldFx_UpdateParticleLinearMotion;
    child->flags = 0;
    child->image.priority = source->sprite->image.priority;
  }
}

/// Makes an actor jump down off a ledge in front of it (the field runs it for
/// the leader when the player presses down at a ledge): it shows pose for 10
/// frames, hops, spins round while sinking 28 pixels with a dust puff every
/// other frame (none when param is -1), then jumps 16 pixels forward and lands.
/// The landing plays the queued sound (BattleFx_PlayQueuedSound).
void Actor_JumpDownLedge(s32 actor, s32 pose, s32 param) {
  s32 nextY;
  s32 startZ;
  struct ObjectRuntime *object;
  u8 *objectFlags;

  object = (struct ObjectRuntime *)Object_GetById(actor);
  startZ = object->z;
  if (object != 0) {
    AudioCommand_PlayFar(SE_289);
    ObjectDispatch_SelectAnimationFar(object, pose);
    WaitFrames(10);

    // Hop up and forward 12 pixels.
    objectFlags = &object->flags;
    ObjectDispatch_SelectAnimationFar(object, 1);
    {
      u8 flags = 2;
      flags |= *objectFlags;
      *objectFlags = flags;
    }
    object->velocityY = 0x40000;
    Object_SetMoveTargetFar(object, object->x, object->y, startZ + 0xC0000);
    WaitFrames(6);

    // Spin and sink 2 pixels a frame for 14 frames, raising dust.
    AudioCommand_PlayFar(SE_217);
    // From here pose counts frames.
    pose = 0;
    ObjectMotion_ArmCallback(actor, 0x5000, 0);
    *objectFlags = 0;
    do {
      nextY = object->y + 0xFFFE0000;
      object->y = nextY;
      object->targetY = nextY;
      WaitFrames(1);
      if ((param != -1) && (pose & 1)) {
        FieldFx_SpawnBurstParticle((struct FieldActor *)object, param);
      }
      pose++;
    } while ((u32)pose <= 0xD);

    // Jump 16 pixels forward and wait (at most 180 frames) to land.
    *objectFlags = 3;
    object->velocityY = 0x30000;
    Object_SetMoveTargetFar(object, object->x, object->y, startZ + 0x100000);
    FieldObject_WaitForMoveFar(object);
    for (pose = 0; (u32)pose <= 0xB3 && object->y > object->terrainHeight;
         pose++) {
      WaitFrames(1);
    }
    WaitFrames(2);
    BattleFx_PlayQueuedSound();
  }
}

/// The event command "actor face actor": turns the first actor at once to face
/// the second, then waits wait frames.
void ObjectMotion_SetAngleToward(s32 firstId, s32 secondId, s32 wait) {
  struct ObjectRuntime *first;
  struct ObjectRuntime *second;

  first = ObjectTable_Get(firstId);
  second = ObjectTable_Get(secondId);
  if (first != 0 && second != 0) {
    first->angle = ArcTan2(second->z - first->z, second->x - first->x);
    FieldEvent_WaitFrames(wait);
  }
}

/// The event command "actors face each other": turns two actors toward each
/// other, then waits wait frames.
void Object_LinkPair(s32 firstId, s32 secondId, s32 wait) {
  void *first = ObjectTable_Get(firstId);
  void *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    FacingObject_TurnPairToFaceEachOther(first, second);
    FieldEvent_WaitFrames(wait);
  }
}

/// Turns two actors to face each other, at most 1/16 turn a frame each, until
/// both do (at most 60 frames).
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *firstObject,
                                          struct FacingObject *secondObject) {
  struct ObjectRuntime *first = (struct ObjectRuntime *)firstObject;
  struct ObjectRuntime *second = (struct ObjectRuntime *)secondObject;
  s32 firstTargetAngle;
  s32 secondTargetAngle;
  s32 frame;
  s32 actorsTurning;
  s32 angleDifference;

  if (first == NULL || second == NULL)
    return;
  firstTargetAngle = (u16)ArcTan2(second->z - first->z, second->x - first->x);
  secondTargetAngle = firstTargetAngle + 0x8000;
  for (frame = 0; frame < 60; frame++) {
    actorsTurning = 2;
    angleDifference = (s16)(firstTargetAngle - first->angle);
    if (angleDifference != 0) {
      if (angleDifference > 0x1000)
        angleDifference = 0x1000;
      if (angleDifference < -0x1000)
        angleDifference = -0x1000;
      first->angle += angleDifference;
    } else {
      actorsTurning = 1;
    }
    angleDifference = (s16)(secondTargetAngle - second->angle);
    if (angleDifference != 0) {
      if (angleDifference > 0x1000)
        angleDifference = 0x1000;
      if (angleDifference < -0x1000)
        angleDifference = -0x1000;
      second->angle += angleDifference;
    } else {
      actorsTurning--;
    }
    if (actorsTurning == 0)
      break;
    WaitFrames(1);
  }
}

#ifndef UBFIX
struct ObjectSlotTable {
  u8 unk00[0x14];
  struct ObjectRuntime *slots[0];
};

STATIC_ASSERT(offsetof(struct ObjectSlotTable, slots) == 0x14);
#endif

/// The event command "destroy actor": releases the actor and frees its slot.
void ObjectTable_DestroyById(s32 index) {
  void *object = ObjectTable_Get(index);
#ifdef UBFIX
  struct FieldWork *table = (struct FieldWork *)gEventWork;
  struct ObjectRuntime *empty = NULL;
#else
  struct ObjectSlotTable *table = (struct ObjectSlotTable *)gEventWork;
#endif

  if (object != 0) {
    ObjectDispatch_ReleaseFar(object);
#ifdef UBFIX
    // The getter accepts slots through 191, including nonactor storage.
    // Copy the pointer representation without extending the actor array.
    memcpy((u8 *)table + 0x14 + index * sizeof(void *), &empty, sizeof(empty));
#else
    table->slots[index] = NULL;
#endif
  }
}

/// Does nothing.
UNUSED void ObjectTable_DestroyNoOp(void) {}

/// The event command "set actor sprite param": gives every sprite part of the
/// actor param (ObjectGroup_SetChildValue). With bit 0x100 set the actor
/// blinks instead (ObjectGroup_ApplyIndexedChildValue, every frame).
void ObjectGroup_ConfigureChildValue(s32 objectId, s32 param) {
  s32 blink;
  struct DispatchObject *object;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    blink = 0x100 & param;
    if (blink != 0) {
      ((struct ScriptMotionObject *)object)->hook =
          ObjectGroup_ApplyIndexedChildValue;
      return;
    }
    ((struct ScriptMotionObject *)object)->hook = NULL;
    ObjectGroup_SetChildValue((struct FieldActor *)object, param);
  }
}

/// Per-frame hook of a blinking actor: sets its sprite parts' param from
/// gObjectGroup_BlinkChildValues (7 for two frames out of eight, otherwise 0).
void ObjectGroup_ApplyIndexedChildValue(union FieldObject *fieldObject) {
  struct DispatchObject *object = (struct DispatchObject *)fieldObject;

  if ((object->kind & 0xf) == 1) {
    u8 param;
    struct AnimationObject *container;
    u8 partCount;

    param = gObjectGroup_BlinkChildValues[(gFrameCount >> 1) & 3];
    container = object->target.child;
    partCount = container->count;
    if (partCount != 0) {
      struct AnimationEntry **entries = container->entries;
      s32 remaining = partCount;
      do {
        struct AnimationEntry *entry = *entries++;
        if (entry != 0 && entry->script != 0) {
          entry->param = param;
        }
        remaining--;
      } while (remaining != 0);
    }
    container->dirty = 1;
  }
}

/// Sets the param byte of every animated part of an actor's sprite. Scenes set
/// 0 or 15, and alternate the two to make an actor flash.
void ObjectGroup_SetChildValue(struct FieldActor *actor, s32 param) {
  struct DispatchObject *object = (struct DispatchObject *)actor;

  if ((object->kind & 0xf) == 1) {
    struct AnimationObject *container = object->target.child;
    u8 partCount = container->count;

    if (partCount != 0) {
      struct AnimationEntry **entry = container->entries;
      u32 count = partCount;
      do {
        struct AnimationEntry *part = *entry++;
        if (part != NULL && part->script != 0) {
          part->param = param;
        }
        count--;
      } while (count != 0);
    }
    container->dirty = 1;
  }
}

/// Links an actor to the actor in the low 8 bits of targetId and starts script
/// on it. Unless ACTOR_LINK_KEEP_MOTION is set it takes action 40 and the
/// target's top speed with twice its acceleration, to keep up with it.
void Object_SetTargetAndCallback(u32 objectId, s32 targetId,
                                 const void *script) {
  struct ObjectRuntime *follower = ObjectTable_Get(objectId);
  struct ObjectRuntime *target = ObjectTable_Get(targetId & 0xff);

  if (follower != NULL && target != NULL) {
    follower->linkedObject = target;
    if (!(targetId & ACTOR_LINK_KEEP_MOTION)) {
      follower->action = 40;
      follower->acceleration = target->acceleration * 2;
      follower->speedLimit = target->speedLimit;
      follower->unk59 = 0;
    }
    ObjectDispatch_InitializeFar((struct DispatchObject *)follower, script);
  }
}

/// Script step that turns an actor toward the angle kept in its action: the
/// whole way when within 1/16 turn, otherwise 1/32 turn. Returns the turn
/// made, 0 once the actor faces that way.
s32 ObjectMotion_StepAngle(struct ObjectRuntime *object) {
  s32 delta = 0;

  if (object != NULL) {
    s32 targetAngle = (u16)object->action;
    s32 currentAngle = object->angle;
    delta = (s16)(targetAngle - currentAngle);
    if (delta != 0) {
      if (delta > 4096)
        delta = 2048;
      if (delta < -4096)
        delta = -2048;
      object->angle = currentAngle + delta;
    }
  }
  return delta;
}

/// Clears an actor's destination, puts it under event control and stands it
/// still.
void Object_ResetTargetAndSetMode1(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->targetX = ACTOR_NO_TARGET;
    object->targetY = ACTOR_NO_TARGET;
    object->targetZ = ACTOR_NO_TARGET;
    ObjectDispatch_StartIdleFar(object);
    ObjectDispatch_SelectAnimationFar(object, 1);
  }
}

/// The event command "actor face direction": the actor turns toward angle
/// (0x10000 a full turn; ObjectMotion_StepAngle), then the event waits wait
/// frames.
void ObjectMotion_ArmCallback(s32 objectId, s32 angle, s32 wait) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->action = angle;
    ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                 (const s32 *)gObjectMotion_StepAngleScript);
    FieldEvent_WaitFrames(wait);
  }
}

/// The event command "set actor sprite priority": fixes the priority of the
/// actor's sprite, which then no longer follows its depth.
void ObjectMotion_SetActionVariant(s32 objectId, s32 priority) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL && (object->animationKind & 0xF) == 1) {
    struct AnimationObject *sprite = object->animation;

    sprite->image.priority = priority;
    sprite->shadow.priority = priority;
    object->unk23 &= ~ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// Makes the target actor's sprite show the source actor's graphics (its VRAM
/// block and tile).
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId) {
  struct ObjectRuntime *object;
  struct AnimationObject *sprite;
  u8 vramBlock;
  u32 tile;

  object = Object_GetById(sourceId);
  sprite = object->animation;
  vramBlock = sprite->slot;
  tile = sprite->image.tile;

  object = Object_GetById(targetId);
  sprite = object->animation;
  sprite->slot = vramBlock;
  sprite->image.tile = tile;
}

/// Does nothing.
UNUSED void ObjectVisual_ReservedNoOp(void) {}

/// The event command "set message": the number of the event's next message.
void Event_SetMessageId(s32 message) {
  ((struct FieldWork *)gEventWork)->message = message;
}

/// Returns the sprite (the first animation's id) of the actor in the low 12
/// bits of actor, or -1 if there is no such animated actor.
s32 ObjectTable_ReadActiveValue(s32 actor) {
#ifdef UBFIX
  struct FieldWork *state = (struct FieldWork *)gEventWork;
  s32 sprite = -1;
  struct ObjectRuntime *entry;

  actor &= 0x0fff;
  // BUG: the -1 sentinel becomes index 4095, beyond the event work. All
  // indices from 66 to 4095 lie outside the actor table.
  if (actor >= 66)
    return -1;
  entry = (struct ObjectRuntime *)state->actors[actor];
#else
  struct FieldWork *state = (struct FieldWork *)gEventWork;
  s32 sprite = -1;
  struct ObjectRuntime *entry =
      (struct ObjectRuntime *)state->actors[actor & 0x0fff];
#endif

  if (entry != 0 && entry->animationKind == 1)
    sprite = ((struct AnimationObject *)entry->animation)->entries[0]->animId;
  return sprite;
}

/// Returns the first scene actor (8 to 65) showing sprite, or -1.
s32 ObjectTable_FindActiveByValue(s32 sprite) {
  struct FieldWork *state = (struct FieldWork *)gEventWork;
  s32 result = -1;
  s32 index;
  struct ObjectRuntime *object;

  for (index = 8; index <= 65; index++) {
    object = (struct ObjectRuntime *)state->actors[index];
    if (object != 0 && object->animationKind == 1 &&
        ((struct AnimationObject *)object->animation)->entries[0]->animId ==
            sprite) {
      result = index;
      break;
    }
  }
  return result;
}
