/// @file
/// Event commands on actors: placing, moving and animating them.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// Returns an actor, or NULL if it does not exist. Scene scripts reach it
/// through Actor_Get and OBJ_GET; it is the far-callable form of
/// ObjectTable_Get that the other banks use.
struct FieldActor *Object_GetById(s32 actor) {
  struct FieldActor *object = ObjectTable_Get(actor);

  if (object == NULL)
    return NULL;
  return object;
}

/// The event command "set actor speed": its top speed and acceleration, 16.16
/// pixels per frame. Writes the actor's speedLimit and acceleration. Scene
/// scripts reach it as Engine_ActorSetSpeed (OBJ_SPEED, Actor_SetSpeed).
void ObjectMotion_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->acceleration = acceleration;
    object->speedLimit = speedLimit;
  }
}

/// Gives an actor an action script (ObjectMotion_SetActionCallback) and makes
/// it face where it moves: sets ACTOR_FACES_MOTION, bit 0 of its
/// actionFlags. Scene scripts reach it as Engine_ActorEnableActionCallback
/// (Actor_SetActionScript and Actor_SetBuiltinAction).
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
/// event control, on the idle script (a lone SC_END), and sets
/// ACTOR_FACES_MOTION, bit 0 of its actionFlags. Scene scripts reach it as
/// Engine_ActorStop (Actor_Stop), about 22 of them.
void ObjectMotion_EnableActionAndResetMotion(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);
  u8 enabled;

  if (object != NULL) {
    enabled = 1;
    object->actionFlags = enabled | object->actionFlags;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
  }
}

/// Makes an actor follow another (the linked action script): writes its
/// linkedObject. Called by the Colosso log scene script, and by Altmiller
/// Cave's as Engine_ActorFollow.
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
/// (command 0x10). Called by Motion_SetVarCbAndRefresh, the climbing events
/// and about 28 scene scripts.
void Object_RefreshSelectorById(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    ObjectDispatch_WaitForValue16Far(object);
}

/// Starts a built-in action or supplied script, as
/// ObjectMotion_EnableActionAndSetCallback does, then waits up to 300 frames
/// for its script to end. Scene script calls use
/// Actor_RunActionScriptAndWait.
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
/// pixels, keeping its current animation. Clears its movementState, so a
/// paused actor moves again, as the other move commands here do. Scene
/// scripts reach it as Engine_ActorSetDestination (Actor_SetDestination).
void ObjectMotion_ResetAndSetPosition(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
  }
}

/// The event command "move actor and wait": as
/// ObjectMotion_ResetAndSetPosition, then waits (at most 600 frames) for the
/// actor to arrive. Scene scripts reach it as Engine_ActorMoveToAndWait
/// (Actor_MoveToAndWait); the climbing events call it too.
void ObjectMotion_SetPositionAndCommit(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
    FieldObject_WaitForMoveFar(object);
  }
}

/// The event command "walk actor to": the actor walks (animation 2) toward
/// (x, z), in pixels. Scene scripts reach it as Engine_ActorWalkTo (OBJ_WALK,
/// Actor_WalkTo); the link lobby calls it too.
void ObjectMotion_ResetAndSetPositionInMode2(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    ObjectDispatch_SelectAnimationFar(object, 2);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
  }
}

/// The event command "walk actor to and wait": walks the actor to (x, z), in
/// pixels, waits for it to arrive and stands it still (animation 1). Scene
/// scripts reach it as Engine_ActorWalkToAndWait (Actor_WalkToAndWait).
void ObjectMotion_SetPositionAndReset(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    ObjectDispatch_SelectAnimationFar(object, 2);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
    FieldObject_WaitForMoveFar(object);
    ObjectDispatch_SelectAnimationFar(object, 1);
  }
}

/// The event command "center actor and walk": walks the actor sideways to the
/// middle of its 16-pixel column, sets its sprite priority to priority
/// (ObjectMotion_SetActionVariant), then sends it zOffset pixels along z.
/// Kalay's interiors reach it as Engine_ActorCenterAndWalk.
void ObjectMotion_CenterOnColumn(u32 objectId, s32 priority, s32 zOffset) {
  struct ObjectRuntime *object;
  s16 pixelX;
  s32 columnX;
  s16 offsetInColumn;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    // The column's left edge, from a division by 16 that rounds toward zero
    // (left of x = 0 it finds the edge to the right); its middle is 8 pixels
    // in.
    pixelX = ((union FieldCoordinate *)&object->x)->part.pixel;
    columnX = pixelX;
    if (pixelX < 0) {
      columnX += 15;
    }
    columnX >>= 4;
    columnX *= 16;
    offsetInColumn = pixelX - columnX;
    object->movementState = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
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
/// (xOffset, zOffset) pixels from where it stands. Scene scripts reach it as
/// Engine_ActorSetDestinationOffset (Actor_SetDestinationOffset);
/// Motion_LaunchFromFocusedObject calls it too.
void ObjectMotion_OffsetPositionAndResetMotion(u32 objectId, s32 xOffset,
                                               s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    Object_SetMoveTargetFar(object, object->x + (xOffset << 16), object->y,
                            object->z + (zOffset << 16));
  }
}

/// Walks the actor (animation 2) (xOffset, zOffset) pixels from where it
/// stands. Scene scripts reach it as Engine_ActorWalkBy (Actor_WalkBy);
/// ObjectMotion_CommitPositionAndActivate calls it too.
void ObjectMotion_OffsetPositionAndReset(u32 objectId, s32 xOffset,
                                         s32 zOffset) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    ObjectDispatch_SelectAnimationFar(object, 2);
    Object_SetMoveTargetFar(object, object->x + (xOffset << 16), object->y,
                            object->z + (zOffset << 16));
  }
}

/// Walks the actor (xOffset, zOffset) pixels, waits for it to arrive and
/// stands it still. Scene scripts reach it as Engine_ActorWalkByAndWait
/// (Actor_WalkByAndWait); the Lemurian ship calls it by name.
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
/// the actor then takes action action. Called by the Lemurian ship, Babi
/// Lighthouse and Lalivero scene scripts.
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
/// to arrive and stands it still. Scene scripts reach it as
/// Engine_ActorWaitForMove (Actor_WaitForMove); the link lobby and the
/// climbing events call it too.
void ObjectMotion_CommitCurrentPositionAndActivate(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    FieldObject_WaitForMoveFar(object);
    ObjectDispatch_SelectAnimationFar(object, 1);
  }
}

/// The event command "set actor position": puts the actor at (x, z), 16.16
/// pixels, at rest. An actor that follows the ground (ACTOR_FOLLOWS_TERRAIN)
/// keeps its height above the terrain at the new place. Clears its velocity and
/// targets. Scene scripts reach it as Engine_ActorSetPosition (OBJ_POS_SET,
/// Actor_SetPosition); Motion_LaunchFromFocusedObject calls it too.
void ObjectMotion_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z) {
  struct ObjectRuntime *object;
  s32 terrainHeight;
  s32 tileX;
  s32 tileZ;
  s32 terrainId;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    SET_VELOCITY(object, 0, 0, 0);
    object->targetY = ACTOR_NO_TARGET;
    object->targetX = ACTOR_NO_TARGET;
    object->x = x;
    object->z = z;
    // Swap the old ground's height for the new one, so the actor's height
    // above the ground is kept. The coordinates are whole pixels, rounded
    // toward zero.
    // Map_GetTerrainHeight takes 16.16 coordinates and returns 16.16 pixels,
    // so these pixel coordinates read the layer's first cell, and the << 16
    // keeps only the result's low 16 bits (0 on a flat cell).
    // XXX: likely written for an older interface in pixels, because weather.c
    // makes the same call; a scene placing an actor this way on raised
    // ground would show what the game does.
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
/// the ground (ACTOR_FOLLOWS_TERRAIN) is raised by the terrain height there,
/// read as ObjectMotion_SetHorizontalPositionWithTerrain reads it (see the XXX
/// there). Nothing in ☀️ calls it.
void ObjectMotion_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z) {
  struct ObjectRuntime *object;
  s32 terrainHeight;
  s32 tileX;
  s32 tileZ;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
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
/// the head and so on). Scene scripts reach it as Engine_ActorSetAnimation
/// (OBJ_MODE, Actor_SetAnimation); Event_AskYesNoWithActorAnimation and
/// Motion_SetModeAndWaitAnimation call it too.
void Object_SetModeById(u32 objectId, s32 animation) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    ObjectDispatch_SelectAnimationFar(object, animation);
}

/// Sets the animation timer steps of each of an actor's sprites
/// (AnimationObjects_SetActiveTimerSteps). Called by the Tolbi ship and
/// Lamakan Desert scene scripts.
void Object_SetActionById(u32 objectId, s32 steps) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL)
    ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)object, steps);
}

/// Waits, at most 90 frames, for an actor's animation to change. Only an actor
/// drawn as an animation object (OBJECT_SPRITE_ONE) is waited for. Called by
/// Motion_SetModeAndWaitAnimation, a few scene scripts and the debug level
/// screen.
void ObjectMotion_WaitForAnimationChange(u32 objectId) {
  struct ObjectRuntime *object;
  struct AnimationObject *sprite;
  /* FAKEMATCH(volatile): saved lives in a 4-byte frame, stored once and
   * loaded again for each compare, as the bytes have it; as a plain s32, 29
   * of 31 instructions differ in tbs-en. */
  volatile s32 saved;
  s32 elapsedFrames;

  object = ObjectTable_Get(objectId);
  if (object != NULL && object->animationKind == OBJECT_SPRITE_ONE) {
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
/// waits for it to end. Scene scripts reach it as
/// Engine_ActorSetAnimationAndWait (Actor_SetAnimationAndWait).
void Motion_SetModeAndWaitAnimation(u32 objectId, s32 animation) {
  Object_SetModeById(objectId, animation);
  ObjectMotion_WaitForAnimationChange(objectId);
}

/// An empty function, reached only through an unnamed veneer in the object
/// bank's far-call table; nothing in ☀️ names it.
void ObjectMotion_NoOp(void) {}

/// The event command "actor jump": throws the actor upward at speed pixels per
/// frame with a jump sound (a higher one above 5), then waits frames frames.
/// Sets bit 1 of the actor's motion flags, so gravity brings it down, and its
/// velocityY. Scene scripts reach it as Engine_ActorJump (Actor_Jump), about
/// 40 of them; the scenes of Djinn joining the party (djinni.c and the four
/// element parts) call it too.
void ObjectMotion_Launch(u32 objectId, s32 speed, s32 frames) {
  struct ObjectRuntime *object;
  u8 *objectFlags;
  s32 updatedFlags;
  s32 verticalVelocity;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    objectFlags = &object->flags;
    updatedFlags = *objectFlags | ACTOR_FALLS;
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
/// back count times (1 to 3), through gObjectMotion_VariantScripts. Scene
/// scripts reach it as Engine_ActorStartRepeatedMotion
/// (Actor_StartRepeatedMotion).
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
/// then waits for it to end. Scene scripts reach it as
/// Engine_ActorRunRepeatedMotion (Actor_RunRepeatedMotion); the scenes of
/// Djinn joining the party call it too.
void Motion_SetVarCbAndRefresh(u32 objectId, s32 count) {
  ObjectMotion_SetVariantCallback(objectId, count);
  Object_RefreshSelectorById(objectId);
}

/// Moves a dust puff of FieldEffect_SpawnBurstParticle: it drifts with its x
/// and z velocity (kept in speed and acceleration), which drag slows by 1/18
/// and 1/16 each frame, and rises 1/64 pixel a frame. Runs each frame as the
/// puff's update hook.
void FieldEffect_UpdateParticleLinearMotion(union FieldObject *fieldObject) {
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

/// Kicks up a dust puff (SPRITE_DUST_PUFF) where source stands: it flies off
/// toward the back at 0.5 to 1.4 pixels a frame and sideways at up to 1.4, with
/// one of two animations. A nonzero param is given to its sprite parts
/// (ObjectGroup_SetChildValue). Only Actor_JumpDownLedge calls it.
void FieldEffect_SpawnBurstParticle(struct FieldActor *source, s32 param) {
  struct FieldActor *object;
  struct AnimationObject *child;
  s32 speed;

  object = (struct FieldActor *)Object_CreateFar(
      SPRITE_DUST_PUFF, source->x.fixed, source->y.fixed, source->z.fixed);
  if (object != 0) {
    child = object->sprite;
    switch (Random16() & 1) {
    case 1:
      ObjectDispatch_SelectAnimationFar(object, 2);
      ObjectDispatch_InitializeFar(
          (struct DispatchObject *)object,
          (const s32 *)gFieldEffect_BurstParticleScriptA);
      break;
    default:
      ObjectDispatch_SelectAnimationFar(object, 1);
      ObjectDispatch_InitializeFar(
          (struct DispatchObject *)object,
          (const s32 *)gFieldEffect_BurstParticleScriptB);
      break;
    }

    if (param != 0)
      ObjectGroup_SetChildValue(object, param);

    // speed and acceleration hold the puff's x and z velocity, in steps of
    // 0x1999 (0.1 pixel): z is -0.5 to -1.4, x -1.4 to 1.4 in steps of 0.2.
    object->motionFlags = 0;
    speed = Random16() % 10 + 5;
    object->acceleration = -0x1999 * speed;
    speed = Random16() % 15 - 7;
    speed <<= 1;
    object->speed = 0x1999 * speed;
    object->timer = 0;
    object->update = FieldEffect_UpdateParticleLinearMotion;
    child->flags = 0;
    child->image.priority = source->sprite->image.priority;
  }
}

/// Makes an actor jump down off a ledge in front of it (the field runs it for
/// the leader when the player presses down at a ledge): it shows pose for 10
/// frames, hops 12 pixels south, turns toward angle 0x5000 (1/16 turn past
/// FACING_SOUTH) while sinking 28 pixels with a dust puff every other frame
/// (none when param is -1), then jumps on to 16 pixels south of where it
/// started and lands. The landing plays the queued sound
/// (FieldEffect_PlayQueuedSound). Called by Field_RunPushTileEvent for a ledge
/// (MAP_CELL_LEDGE) with pose 6 and param 0, and by seven scenes through
/// FieldActor_JumpDownLedge; Mercury Lighthouse passes -1.
void Actor_JumpDownLedge(s32 actor, s32 pose, s32 param) {
  s32 nextY;
  s32 startZ;
  struct ObjectRuntime *object;
  u8 *objectFlags;

  object = (struct ObjectRuntime *)Object_GetById(actor);
  startZ = object->z;
  if (object != 0) {
    AudioCommand_PlayFar(AUDIO_CUE_FADE_SLOT3);
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

    // Turn and sink 2 pixels a frame for 14 frames, raising dust, in free
    // flight: no motion flags, so no gravity and no terrain.
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
        FieldEffect_SpawnBurstParticle((struct FieldActor *)object, param);
      }
      pose++;
    } while ((u32)pose <= 0xD);

    // Jump 16 pixels forward and wait (at most 180 frames) to land, following
    // the terrain and falling again.
    *objectFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    object->velocityY = 0x30000;
    Object_SetMoveTargetFar(object, object->x, object->y, startZ + 0x100000);
    FieldObject_WaitForMoveFar(object);
    for (pose = 0; (u32)pose <= 0xB3 && object->y > object->terrainHeight;
         pose++) {
      WaitFrames(1);
    }
    WaitFrames(2);
    FieldEffect_PlayQueuedSound();
  }
}

/// The event command "actor face actor": turns the first actor at once to face
/// the second, then waits wait frames. Writes the first actor's angle. Scene
/// scripts reach it as Engine_ActorFaceActor (OBJ_FACE, Actor_FaceActor); the
/// link lobby calls it too.
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
/// other, then waits wait frames. Scene scripts reach it as
/// Engine_ActorFaceEachOther (Actor_FaceEachOther); Scene_TalkToActor turns
/// the leader and the actor spoken to with it.
void Object_LinkPair(s32 firstId, s32 secondId, s32 wait) {
  void *first = ObjectTable_Get(firstId);
  void *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    FacingObject_TurnPairToFaceEachOther(first, second);
    FieldEvent_WaitFrames(wait);
  }
}

/// Turns two actors to face each other, at most 1/16 turn a frame each, until
/// both do (at most 60 frames). Only Object_LinkPair calls it.
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
  // The second actor faces the opposite way, half a turn round.
  secondTargetAngle = firstTargetAngle + 0x8000;
  for (frame = 0; frame < 60; frame++) {
    // Each actor that already faces its way takes one off the count; both
    // facing ends the turn early.
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
/// The event work seen as its actor slots, which the actor commands index by
/// actor number.
struct ObjectSlotTable {
  u8 unk00[0x14];
  struct ObjectRuntime *slots[0];
};

STATIC_ASSERT(offsetof(struct ObjectSlotTable, slots) == 0x14);
#endif

/// The event command "destroy actor": releases the actor and frees its slot,
/// writing NULL to its place in the field work's actor table. Scene scripts
/// reach it as Engine_ActorDestroy (about 14 of them).
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

/// Unused: an empty function nothing calls.
UNUSED void ObjectTable_DestroyNoOp(void) {}

/// The event command "set actor sprite param": gives every sprite part of the
/// actor param (ObjectGroup_SetChildValue). With bit 0x100 set the actor
/// blinks instead (ObjectGroup_ApplyIndexedChildValue, every frame); any other
/// param also ends a blink, clearing the actor's hook. Scene scripts reach it
/// as Engine_ActorSetChildValue (Actor_SetChildValue).
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
/// ObjectGroup_ConfigureChildValue installs it.
void ObjectGroup_ApplyIndexedChildValue(union FieldObject *fieldObject) {
  struct DispatchObject *object = (struct DispatchObject *)fieldObject;

  if ((object->kind & 0xf) == 1) {
    u8 param;
    struct AnimationObject *container;
    u8 partCount;

    // The table's four entries each last two frames.
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
/// 0 or 15, and alternate the two to make an actor flash. Marks the sprite
/// dirty. Called by ObjectGroup_ConfigureChildValue,
/// FieldEffect_SpawnBurstParticle and about 40 scene script sites.
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
/// on it. Unless ACTOR_LINK_KEEP_MOTION is set it takes a follow distance of
/// 40 pixels (action 40, read by ObjectMotion_MoveTowardTarget), the target's
/// top speed with twice its acceleration, to keep up with it, and no collision
/// flags, so it cannot block the target. Called by Vale's, Bilibin's, Imil's,
/// Kolima's and McCoy's palace scene scripts, among others.
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
      follower->collisionFlags = 0;
    }
    ObjectDispatch_InitializeFar((struct DispatchObject *)follower, script);
  }
}

/// Script step that turns an actor toward the angle kept in its action: the
/// whole way when within 1/16 turn, otherwise 1/32 turn. Returns the turn
/// made, 0 once the actor faces that way: SC_CALL runs it again each frame
/// while it is nonzero, so the script ends once the turn is done.
/// gObjectMotion_StepAngleScript and gObjectMotion_LaunchScript run it.
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
/// still. Reached only through an unnamed veneer in the object bank's
/// far-call table; nothing in ☀️ names it.
void Object_ResetTargetAndSetMode1(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->targetX = ACTOR_NO_TARGET;
    object->targetY = ACTOR_NO_TARGET;
    object->targetZ = ACTOR_NO_TARGET;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    ObjectDispatch_SelectAnimationFar(object, 1);
  }
}

/// The event command "actor face direction": the actor turns toward angle
/// (0x10000 a full turn; ObjectMotion_StepAngle), then the event waits wait
/// frames. Writes the angle to the actor's action. Scene scripts reach it as
/// Engine_ActorFaceDirection (OBJ_DIR, Actor_FaceDirection); the Djinni,
/// Retreat and climbing events and Actor_JumpDownLedge call it too.
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
/// actor's sprite, which then no longer follows its depth: clears
/// ACTOR_PRIORITY_AUTOMATIC. Scene scripts reach it as
/// Engine_ActorSetSpritePriority (Actor_SetSpritePriority);
/// ObjectMotion_CenterOnColumn calls it too.
void ObjectMotion_SetActionVariant(s32 objectId, s32 priority) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL && (object->animationKind & 0xF) == OBJECT_SPRITE_ONE) {
    struct AnimationObject *sprite = object->animation;

    sprite->image.priority = priority;
    sprite->shadow.priority = priority;
    object->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// Makes the target actor's sprite show the source actor's graphics (its VRAM
/// block and tile). Neither actor is checked for NULL. Called by the Tolbi
/// ship deck's scene script.
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId) {
  struct ObjectRuntime *object;
  struct AnimationObject *sprite;
  u8 vramBlock;
  u32 tile;

  object = (struct ObjectRuntime *)Object_GetById(sourceId);
  sprite = object->animation;
  vramBlock = sprite->slot;
  tile = sprite->image.tile;

  object = (struct ObjectRuntime *)Object_GetById(targetId);
  sprite = object->animation;
  sprite->slot = vramBlock;
  sprite->image.tile = tile;
}

/// Unused: an empty function nothing calls.
UNUSED void ObjectVisual_ReservedNoOp(void) {}

/// The event command "set message": the number of the event's next message,
/// written to work->message. Scene scripts reach it as
/// Engine_EventSetMessage (MESSAGE_SET); the field's talk, touch, item and
/// Psynergy events call it too.
void Event_SetMessageId(s32 message) {
  ((struct FieldWork *)gEventWork)->message = message;
}

/// Returns the sprite (the first animation's id) of the actor in the low 12
/// bits of actor, or -1 if there is no such animated actor. The message
/// commands (messages.c) call it to find the speaker's portrait and voice.
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

  if (entry != 0 && entry->animationKind == OBJECT_SPRITE_ONE)
    sprite = ((struct AnimationObject *)entry->animation)->entries[0]->animId;
  return sprite;
}

/// Returns the first scene actor (8 to 65) showing sprite, or -1. Reached
/// only through an unnamed veneer in the object bank's far-call table;
/// nothing in ☀️ names it.
s32 ObjectTable_FindActiveByValue(s32 sprite) {
  struct FieldWork *state = (struct FieldWork *)gEventWork;
  s32 result = -1;
  s32 index;
  struct ObjectRuntime *object;

  for (index = 8; index <= 65; index++) {
    object = (struct ObjectRuntime *)state->actors[index];
    if (object != 0 && object->animationKind == OBJECT_SPRITE_ONE &&
        ((struct AnimationObject *)object->animation)->entries[0]->animId ==
            sprite) {
      result = index;
      break;
    }
  }
  return result;
}
