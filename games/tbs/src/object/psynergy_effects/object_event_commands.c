/// @file
/// Script commands on object events: placing, moving and animating them.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// Returns an object event, or NULL if it does not exist. Map scripts reach it
/// through ObjectEvent_Get and OBJECT_GET; it is the far-callable form of
/// ObjectEvent_GetByLocalId that the other banks use.
struct ObjectEvent *ObjectEvent_GetById(s32 localId) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetByLocalId(localId);

  if (objectEvent == NULL)
    return NULL;
  return objectEvent;
}

/// The script command "set object event speed": its top speed and
/// acceleration, 16.16 pixels per frame. Writes the object event's speedLimit
/// and acceleration. Map scripts reach it as Engine_ObjectEventSetSpeed
/// (OBJECT_SPEED, ObjectEvent_SetSpeed).
void ObjectMotion_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    object->acceleration = acceleration;
    object->speedLimit = speedLimit;
  }
}

/// Gives an object event an action script (ObjectEvent_SetMovementType) and
/// makes it face where it moves: sets OBJ_EVENT_FACES_MOTION, bit 0 of its
/// actionFlags. Map scripts reach it as
/// Engine_ObjectEventEnableActionCallback (ObjectEvent_SetActionScript and
/// ObjectEvent_SetBuiltinAction).
void ObjectMotion_EnableActionAndSetCallback(u32 objectId, s32 action) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    s32 value = 1;
    value |= object->actionFlags;
    object->actionFlags = value;
    ObjectEvent_SetMovementType(object, action);
  }
}

/// The script command "stop object event": stops its action and puts it back
/// under script control, on the idle script (a lone SC_END), and sets
/// OBJ_EVENT_FACES_MOTION, bit 0 of its actionFlags. Map scripts reach it as
/// Engine_ObjectEventStop (ObjectEvent_Stop), about 22 of them.
void ObjectMotion_EnableActionAndResetMotion(u32 objectId) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);
  u8 enabled;

  if (object != NULL) {
    enabled = 1;
    object->actionFlags = enabled | object->actionFlags;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
  }
}

/// Makes an object event follow another (the linked action script): writes its
/// linkedObject. Called by the Colosso log's script, and by Altmiller
/// Cave's as Engine_ObjectEventFollow.
void ObjectEvent_LinkObjectAndSetCallback(u32 objectId, u32 linkedObjectId) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    object->linkedObject = (struct Object *)ObjectEvent_GetById(linkedObjectId);
    ObjectEvent_SetMovementType(object, (s32)gObjectMotion_LinkedActionScript);
  }
}

/// Waits, at most 300 frames, for an object event's script to reach its end
/// (command 0x10). Called by ObjectMotion_SetVarCbAndRefresh, the climbing
/// scripts and about 28 map scripts.
void ObjectEvent_RefreshSelectorById(u32 objectId) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL)
    ObjectDispatch_WaitForValue16Far(object);
}

/// Starts a built-in action or supplied script, as
/// ObjectMotion_EnableActionAndSetCallback does, then waits up to 300 frames
/// for its script to end. Map script calls use
/// ObjectEvent_RunActionScriptAndWait.
void ObjectEvent_SetActionCallbackAndRefreshById(u32 objectId, s32 action) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    s32 value = 1;
    value |= object->actionFlags;
    object->actionFlags = value;
    ObjectEvent_SetMovementType(object, action);
    ObjectDispatch_WaitForValue16Far(object);
  }
}

/// The script command "set object event destination": the object event heads
/// for (x, z), in pixels, keeping its current animation. Clears its
/// movementState, so a paused object event moves again, as the other move
/// commands here do. Map scripts reach it as Engine_ObjectEventSetDestination
/// (ObjectEvent_SetDestination).
void ObjectMotion_ResetAndSetPosition(u32 objectId, s32 x, s32 z) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
  }
}

/// The script command "move object event and wait": as
/// ObjectMotion_ResetAndSetPosition, then waits (at most 600 frames) for the
/// object event to arrive. Map scripts reach it as
/// Engine_ObjectEventMoveToAndWait (ObjectEvent_MoveToAndWait); the climbing
/// scripts call it too.
void ObjectMotion_SetPositionAndCommit(u32 objectId, s32 x, s32 z) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
    Object_WaitForMoveFar(object);
  }
}

/// The script command "walk object event to": the object event walks (animation
/// 2) toward (x, z), in pixels. Map scripts reach it as
/// Engine_ObjectEventWalkTo (OBJECT_WALK, ObjectEvent_WalkTo); the Battle Arena
/// calls it too.
void ObjectMotion_ResetAndSetPositionInMode2(u32 objectId, s32 x, s32 z) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    ObjectDispatch_SelectAnimationFar(object, 2);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
  }
}

/// The script command "walk object event to and wait": walks the object event
/// to (x, z), in pixels, waits for it to arrive and stands it still (animation
/// 1). Map scripts reach it as Engine_ObjectEventWalkToAndWait
/// (ObjectEvent_WalkToAndWait).
void ObjectMotion_SetPositionAndReset(u32 objectId, s32 x, s32 z) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    ObjectDispatch_SelectAnimationFar(object, 2);
    Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
    Object_WaitForMoveFar(object);
    ObjectDispatch_SelectAnimationFar(object, 1);
  }
}

/// The script command "center object event and walk": walks the object event
/// sideways to the middle of its 16-pixel column, sets its sprite priority to
/// priority (ObjectMotion_SetActionVariant), then sends it zOffset pixels along
/// z. Kalay's interiors reach it as Engine_ObjectEventCenterAndWalk.
void ObjectMotion_CenterOnColumn(u32 objectId, s32 priority, s32 zOffset) {
  struct Object *object;
  s16 pixelX;
  s32 columnX;
  s16 offsetInColumn;

  object = ObjectEvent_GetByLocalId(objectId);
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
    Object_WaitForMoveFar(object);
    ObjectMotion_SetActionVariant(objectId, priority);
    Object_SetMoveTargetFar(object, object->x, object->y,
                            object->z + (zOffset << 16));
  }
}

/// The script command "set object event destination offset": the object event
/// heads (xOffset, zOffset) pixels from where it stands. Map scripts reach it
/// as Engine_ObjectEventSetDestinationOffset
/// (ObjectEvent_SetDestinationOffset); ObjectMotion_LaunchFromFocusedObject
/// calls it too.
void ObjectMotion_OffsetPositionAndResetMotion(u32 objectId, s32 xOffset,
                                               s32 zOffset) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    Object_SetMoveTargetFar(object, object->x + (xOffset << 16), object->y,
                            object->z + (zOffset << 16));
  }
}

/// Walks the object event (animation 2) (xOffset, zOffset) pixels from where it
/// stands. Map scripts reach it as Engine_ObjectEventWalkBy
/// (ObjectEvent_WalkBy); ObjectMotion_CommitPositionAndActivate calls it too.
void ObjectMotion_OffsetPositionAndReset(u32 objectId, s32 xOffset,
                                         s32 zOffset) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    object->movementState = 0;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    ObjectDispatch_SelectAnimationFar(object, 2);
    Object_SetMoveTargetFar(object, object->x + (xOffset << 16), object->y,
                            object->z + (zOffset << 16));
  }
}

/// Walks the object event (xOffset, zOffset) pixels, waits for it to arrive and
/// stands it still. Map scripts reach it as Engine_ObjectEventWalkByAndWait
/// (ObjectEvent_WalkByAndWait); the Lemurian ship calls it by name.
void ObjectMotion_CommitPositionAndActivate(u32 objectId, s32 xOffset,
                                            s32 zOffset) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  ObjectMotion_OffsetPositionAndReset(objectId, xOffset, zOffset);
  if (object != NULL) {
    Object_WaitForMoveFar(object);
    ObjectDispatch_SelectAnimationFar(object, 1);
  }
}

/// Places an object event on the leader and sends it walking (xOffset, zOffset)
/// pixels away at speed 0.6 and acceleration 0.3, through the launch script;
/// the object event then takes action action. Called by the Lemurian ship, Babi
/// Lighthouse and Lalivero map scripts.
void ObjectMotion_LaunchFromFocusedObject(u32 objectId, s32 xOffset,
                                          s32 zOffset, s32 action) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    struct Object *leader;

    ObjectMotion_SetSpeedParameters(objectId, 0x9999, 0x4CCC);
    leader = (struct Object *)ObjectEvent_GetById(gGameState.leaderLocalId);
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

/// The script command "wait for object event": waits (at most 600 frames) for
/// the object event to arrive and stands it still. Map scripts reach it as
/// Engine_ObjectEventWaitForMove (ObjectEvent_WaitForMove); the Battle Arena
/// and the climbing scripts call it too.
void ObjectMotion_CommitCurrentPositionAndActivate(u32 objectId) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    Object_WaitForMoveFar(object);
    ObjectDispatch_SelectAnimationFar(object, 1);
  }
}

/// The script command "set object event position": puts the object event at (x,
/// z), 16.16 pixels, at rest. An object event that follows the ground
/// (OBJ_EVENT_FOLLOWS_TERRAIN) keeps its height above the ground at the new
/// place. Clears its velocity and targets. Map scripts reach it as
/// Engine_ObjectEventSetPosition (OBJECT_POSITION_SET,
/// ObjectEvent_SetPosition); ObjectMotion_LaunchFromFocusedObject calls it too.
void ObjectMotion_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z) {
  struct Object *object;
  s32 groundHeight;
  s32 tileX;
  s32 tileZ;
  s32 terrainId;

  object = ObjectEvent_GetByLocalId(objectId);
  if (object != NULL) {
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    SET_VELOCITY(object, 0, 0, 0);
    object->targetY = OBJ_EVENT_NO_TARGET;
    object->targetX = OBJ_EVENT_NO_TARGET;
    object->x = x;
    object->z = z;
    // Swap the old ground's height for the new one, so the object event's
    // height above the ground is kept. The coordinates are whole pixels,
    // rounded toward zero. MapGrid_GetTerrainHeight takes 16.16 coordinates and
    // returns 16.16 pixels, so these pixel coordinates read the layer's first
    // cell, and the
    // << 16 keeps only the result's low 16 bits (0 on a flat cell).
    // XXX: likely written for an older interface in pixels, because weather.c
    // makes the same call; a map script placing an object event this way on
    // raised ground would show what the game does.
    if (1 & object->flags) {
      terrainId = object->layer;
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
      groundHeight = MapGrid_GetTerrainHeightFar(terrainId, tileX, tileZ)
                     << 0x10;
      object->y = (object->y - object->groundHeight) + groundHeight;
      object->groundHeight = groundHeight;
    }
  }
}

/// Puts the object event at (x, y, z), 16.16 pixels, at rest; an object event
/// that follows the ground (OBJ_EVENT_FOLLOWS_TERRAIN) is raised by the ground
/// height there, read as ObjectMotion_SetHorizontalPositionWithTerrain reads it
/// (see the XXX there). Nothing in ☀️ calls it.
void ObjectMotion_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z) {
  struct Object *object;
  s32 groundHeight;
  s32 tileX;
  s32 tileZ;

  object = ObjectEvent_GetByLocalId(objectId);
  if (object != NULL) {
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    SET_VELOCITY(object, 0, 0, 0);
    object->targetY = OBJ_EVENT_NO_TARGET;
    object->targetX = OBJ_EVENT_NO_TARGET;
    SET_POSITION(object, x, y, z);
    if (1 & object->flags) {
      s32 terrainId = object->layer;
      tileX = x;
      if (tileX < 0) {
        tileX += 0xFFFF;
      }
      tileX >>= 0x10;
      tileZ = z;
      if (tileZ < 0) {
        tileZ += 0xFFFF;
      }
      groundHeight =
          MapGrid_GetTerrainHeightFar(terrainId, tileX, tileZ >> 0x10) << 0x10;
      object->y = (object->y - object->groundHeight) + groundHeight;
      object->groundHeight = groundHeight;
    }
  }
}

/// The script command "set object event animation" (1 stands, 2 walks, 3 nods,
/// 4 shakes the head and so on). Map scripts reach it as
/// Engine_ObjectEventSetAnimation (OBJECT_ANIMATION, ObjectEvent_SetAnimation);
/// FieldScript_AskYesNoWithObjectEventAnimation and
/// ObjectMotion_SetAnimationAndWait call it too.
void ObjectEvent_SetModeById(u32 objectId, s32 animation) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL)
    ObjectDispatch_SelectAnimationFar(object, animation);
}

/// Sets the animation timer steps of each of an object event's sprites
/// (Sprite_SetAnimSpeed). Called by the Tolbi ship and
/// Lamakan Desert map scripts.
void ObjectEvent_SetActionById(u32 objectId, s32 steps) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL)
    ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)object, steps);
}

/// Waits, at most 90 frames, for an object event's animation to change. Only an
/// object event drawn as a sprite (OBJECT_SPRITE_ONE) is waited for.
/// Called by ObjectMotion_SetAnimationAndWait, a few map scripts and the debug
/// level screen.
void ObjectMotion_WaitForAnimationChange(u32 objectId) {
  struct Object *object;
  struct Sprite *sprite;
  /* FAKEMATCH(volatile): saved lives in a 4-byte frame, stored once and
   * loaded again for each compare, as the bytes have it; as a plain s32, 29
   * of 31 instructions differ in tbs-en. */
  volatile s32 saved;
  s32 elapsedFrames;

  object = ObjectEvent_GetByLocalId(objectId);
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

/// The script command "set object event animation and wait": plays the
/// animation and waits for it to end. Map scripts reach it as
/// Engine_ObjectEventSetAnimationAndWait (ObjectEvent_SetAnimationAndWait).
void ObjectMotion_SetAnimationAndWait(u32 objectId, s32 animation) {
  ObjectEvent_SetModeById(objectId, animation);
  ObjectMotion_WaitForAnimationChange(objectId);
}

/// An empty function, reached only through an unnamed veneer in the object
/// bank's far-call table; nothing in ☀️ names it.
void ObjectMotion_NoOp(void) {}

/// The script command "object event jump": throws the object event upward at
/// speed pixels per frame with a jump sound (a higher one above 5), then waits
/// frames frames. Sets bit 1 of the object event's motion flags, so gravity
/// brings it down, and its velocityY. Map scripts reach it as
/// Engine_ObjectEventJump (ObjectEvent_Jump), about 40 of them; the scenes of
/// Djinn joining the party (djinni.c and the four element parts) call it too.
void ObjectMotion_Launch(u32 objectId, s32 speed, s32 frames) {
  struct Object *object;
  u8 *objectFlags;
  s32 updatedFlags;
  s32 verticalVelocity;

  object = ObjectEvent_GetByLocalId(objectId);
  if (object != NULL) {
    objectFlags = &object->flags;
    updatedFlags = *objectFlags | OBJ_EVENT_FALLS;
    verticalVelocity = speed << 16;
    *objectFlags = updatedFlags;
    object->velocityY = verticalVelocity;
    if (speed > 5) {
      Sound_PlaySongFar(SE_HIGH_JUMP);
    } else {
      Sound_PlaySongFar(SE_JUMP);
    }
    FieldScript_WaitFrames(frames);
  }
}

/// The script command "start repeated motion": the object event squashes and
/// springs back count times (1 to 3), through gObjectMotion_VariantScripts. Map
/// scripts reach it as Engine_ObjectEventStartRepeatedMotion
/// (ObjectEvent_StartRepeatedMotion).
void ObjectMotion_SetVariantCallback(u32 objectId, s32 count) {
  struct Object *object;

  object = ObjectEvent_GetByLocalId(objectId);
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

/// The script command "run repeated motion": as
/// ObjectMotion_SetVariantCallback, then waits for it to end. Map scripts reach
/// it as Engine_ObjectEventRunRepeatedMotion (ObjectEvent_RunRepeatedMotion);
/// the scenes of Djinn joining the party call it too.
void ObjectMotion_SetVarCbAndRefresh(u32 objectId, s32 count) {
  ObjectMotion_SetVariantCallback(objectId, count);
  ObjectEvent_RefreshSelectorById(objectId);
}

/// Moves a dust puff of FieldEffect_SpawnBurstParticle: it drifts with its x
/// and z velocity (kept in speed and acceleration), which drag slows by 1/18
/// and 1/16 each frame, and rises 1/64 pixel a frame. Runs each frame as the
/// puff's update hook.
void FieldEffect_UpdateParticleLinearMotion(union FieldObject *fieldObject) {
  struct ObjectEvent *particle = (struct ObjectEvent *)fieldObject;
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
/// (ObjectGroup_SetChildValue). Only ObjectEvent_JumpDownLedge calls it.
void FieldEffect_SpawnBurstParticle(struct ObjectEvent *source, s32 param) {
  struct ObjectEvent *objectEvent;
  struct Sprite *child;
  s32 speed;

  objectEvent = (struct ObjectEvent *)Object_CreateFar(
      SPRITE_DUST_PUFF, source->x.fixed, source->y.fixed, source->z.fixed);
  if (objectEvent != 0) {
    child = objectEvent->sprite;
    switch (Random16() & 1) {
    case 1:
      ObjectDispatch_SelectAnimationFar(objectEvent, 2);
      ObjectDispatch_InitializeFar(
          (struct DispatchObject *)objectEvent,
          (const s32 *)gFieldEffect_BurstParticleScriptA);
      break;
    default:
      ObjectDispatch_SelectAnimationFar(objectEvent, 1);
      ObjectDispatch_InitializeFar(
          (struct DispatchObject *)objectEvent,
          (const s32 *)gFieldEffect_BurstParticleScriptB);
      break;
    }

    if (param != 0)
      ObjectGroup_SetChildValue(objectEvent, param);

    // speed and acceleration hold the puff's x and z velocity, in steps of
    // 0x1999 (0.1 pixel): z is -0.5 to -1.4, x -1.4 to 1.4 in steps of 0.2.
    objectEvent->motionFlags = 0;
    speed = Random16() % 10 + 5;
    objectEvent->acceleration = -0x1999 * speed;
    speed = Random16() % 15 - 7;
    speed <<= 1;
    objectEvent->speed = 0x1999 * speed;
    objectEvent->timer = 0;
    objectEvent->update = FieldEffect_UpdateParticleLinearMotion;
    child->flags = 0;
    child->image.priority = source->sprite->image.priority;
  }
}

/// Makes an object event jump down off a ledge in front of it (the field runs
/// it for the leader when the player presses down at a ledge): it shows pose
/// for 10 frames, hops 12 pixels south, turns toward angle 0x5000 (1/16 turn
/// past FACING_SOUTH) while sinking 28 pixels with a dust puff every other
/// frame (none when param is -1), then jumps on to 16 pixels south of where it
/// started and lands. The landing plays the queued sound
/// (Map_PlayQueuedMusic). Called by Field_RunPushCellEvent for a ledge
/// (MB_LEDGE) with pose 6 and param 0, and by seven maps through
/// FieldObjectEvent_JumpDownLedge; Mercury Lighthouse passes -1.
void ObjectEvent_JumpDownLedge(s32 localId, s32 pose, s32 param) {
  s32 nextY;
  s32 startZ;
  struct Object *object;
  u8 *objectFlags;

  object = (struct Object *)ObjectEvent_GetById(localId);
  startZ = object->z;
  if (object != 0) {
    Sound_PlaySongFar(SONG_FADE_OUT_ACTION_SE);
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
    // flight: no motion flags, so it neither falls nor follows the ground.
    Sound_PlaySongFar(SE_217);
    // From here pose counts frames.
    pose = 0;
    ObjectMotion_ArmCallback(localId, 0x5000, 0);
    *objectFlags = 0;
    do {
      nextY = object->y + 0xFFFE0000;
      object->y = nextY;
      object->targetY = nextY;
      WaitFrames(1);
      if ((param != -1) && (pose & 1)) {
        FieldEffect_SpawnBurstParticle((struct ObjectEvent *)object, param);
      }
      pose++;
    } while ((u32)pose <= 0xD);

    // Jump 16 pixels forward and wait (at most 180 frames) to land, following
    // the ground and falling again.
    *objectFlags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
    object->velocityY = 0x30000;
    Object_SetMoveTargetFar(object, object->x, object->y, startZ + 0x100000);
    Object_WaitForMoveFar(object);
    for (pose = 0; (u32)pose <= 0xB3 && object->y > object->groundHeight;
         pose++) {
      WaitFrames(1);
    }
    WaitFrames(2);
    Map_PlayQueuedMusic();
  }
}

/// The script command "object event face object event": turns the first object
/// event at once to face the second, then waits wait frames. Writes the first
/// object event's angle. Map scripts reach it as
/// Engine_ObjectEventFaceObjectEvent (OBJECT_FACE,
/// ObjectEvent_FaceObjectEvent); the Battle Arena calls it too.
void ObjectMotion_SetAngleToward(s32 firstId, s32 secondId, s32 wait) {
  struct Object *first;
  struct Object *second;

  first = ObjectEvent_GetByLocalId(firstId);
  second = ObjectEvent_GetByLocalId(secondId);
  if (first != 0 && second != 0) {
    first->facing = ArcTan2(second->z - first->z, second->x - first->x);
    FieldScript_WaitFrames(wait);
  }
}

/// The script command "object events face each other": turns two object events
/// toward each other, then waits wait frames. Map scripts reach it as
/// Engine_ObjectEventFaceEachOther (ObjectEvent_FaceEachOther);
/// Map_TalkToObjectEvent turns the leader and the object event spoken to with
/// it.
void ObjectEvent_LinkPair(s32 firstId, s32 secondId, s32 wait) {
  void *first = ObjectEvent_GetByLocalId(firstId);
  void *second = ObjectEvent_GetByLocalId(secondId);

  if (first != NULL && second != NULL) {
    FacingObject_TurnPairToFaceEachOther(first, second);
    FieldScript_WaitFrames(wait);
  }
}

/// Turns two object events to face each other, at most 1/16 turn a frame each,
/// until both do (at most 60 frames). Only ObjectEvent_LinkPair calls it.
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *firstObject,
                                          struct FacingObject *secondObject) {
  struct Object *first = (struct Object *)firstObject;
  struct Object *second = (struct Object *)secondObject;
  s32 firstTargetAngle;
  s32 secondTargetAngle;
  s32 frame;
  s32 objectEventsTurning;
  s32 angleDifference;

  if (first == NULL || second == NULL)
    return;
  firstTargetAngle = (u16)ArcTan2(second->z - first->z, second->x - first->x);
  // The second object event faces the opposite way, half a turn round.
  secondTargetAngle = firstTargetAngle + 0x8000;
  for (frame = 0; frame < 60; frame++) {
    // Each object event that already faces its way takes one off the count;
    // both facing ends the turn early.
    objectEventsTurning = 2;
    angleDifference = (s16)(firstTargetAngle - first->facing);
    if (angleDifference != 0) {
      if (angleDifference > 0x1000)
        angleDifference = 0x1000;
      if (angleDifference < -0x1000)
        angleDifference = -0x1000;
      first->facing += angleDifference;
    } else {
      objectEventsTurning = 1;
    }
    angleDifference = (s16)(secondTargetAngle - second->facing);
    if (angleDifference != 0) {
      if (angleDifference > 0x1000)
        angleDifference = 0x1000;
      if (angleDifference < -0x1000)
        angleDifference = -0x1000;
      second->facing += angleDifference;
    } else {
      objectEventsTurning--;
    }
    if (objectEventsTurning == 0)
      break;
    WaitFrames(1);
  }
}

#ifndef UBFIX
/// The event work seen as its object event slots, which the object event
/// commands index by local id.
struct ObjectSlotTable {
  u8 tablesAndEvents[0x14]; ///< FieldWork's tables and events
  struct Object *slots[0];
};

STATIC_ASSERT(offsetof(struct ObjectSlotTable, slots) == 0x14);
#endif

/// The script command "destroy object event": releases the object event and
/// frees its slot, writing NULL to its place in the field work's object event
/// table. Map scripts reach it as Engine_ObjectEventDestroy (about 14 of them).
void ObjectEvent_DestroyById(s32 index) {
  void *object = ObjectEvent_GetByLocalId(index);
#ifdef UBFIX
  struct FieldWork *table = (struct FieldWork *)gFieldWork;
  struct Object *empty = NULL;
#else
  struct ObjectSlotTable *table = (struct ObjectSlotTable *)gFieldWork;
#endif

  if (object != 0) {
    ObjectDispatch_ReleaseFar(object);
#ifdef UBFIX
    // The getter accepts slots through 191, including non-object-event storage.
    // Copy the pointer representation without extending the object event array.
    memcpy((u8 *)table + 0x14 + index * sizeof(void *), &empty, sizeof(empty));
#else
    table->slots[index] = NULL;
#endif
  }
}

/// Unused: an empty function nothing calls.
UNUSED void ObjectEvent_DestroyNoOp(void) {}

/// The script command "set object event sprite param": gives every sprite part
/// of the object event param (ObjectGroup_SetChildValue). With bit 0x100 set
/// the object event blinks instead (ObjectGroup_ApplyIndexedChildValue, every
/// frame); any other param also ends a blink, clearing the object event's hook.
/// Map scripts reach it as Engine_ObjectEventSetChildValue
/// (ObjectEvent_SetChildValue).
void ObjectGroup_ConfigureChildValue(s32 objectId, s32 param) {
  s32 blink;
  struct DispatchObject *object;

  object = ObjectEvent_GetByLocalId(objectId);
  if (object != NULL) {
    blink = 0x100 & param;
    if (blink != 0) {
      ((struct ScriptMotionObject *)object)->hook =
          ObjectGroup_ApplyIndexedChildValue;
      return;
    }
    ((struct ScriptMotionObject *)object)->hook = NULL;
    ObjectGroup_SetChildValue((struct ObjectEvent *)object, param);
  }
}

/// Per-frame hook of a blinking object event: sets its sprite parts' param from
/// gObjectGroup_BlinkChildValues (7 for two frames out of eight, otherwise 0).
/// ObjectGroup_ConfigureChildValue installs it.
void ObjectGroup_ApplyIndexedChildValue(union FieldObject *fieldObject) {
  struct DispatchObject *object = (struct DispatchObject *)fieldObject;

  if ((object->kind & 0xf) == 1) {
    u8 param;
    struct Sprite *container;
    u8 partCount;

    // The table's four entries each last two frames.
    param = gObjectGroup_BlinkChildValues[(gFrameCount >> 1) & 3];
    container = object->target.child;
    partCount = container->count;
    if (partCount != 0) {
      struct SpriteAnim **entries = container->entries;
      s32 remaining = partCount;
      do {
        struct SpriteAnim *entry = *entries++;
        if (entry != 0 && entry->script != 0) {
          entry->colorMode = param;
        }
        remaining--;
      } while (remaining != 0);
    }
    container->dirty = 1;
  }
}

/// Sets the param byte of every animated part of an object event's sprite. Map
/// scripts set 0 or 15, and alternate the two to make an object event flash.
/// Marks the sprite dirty. Called by ObjectGroup_ConfigureChildValue,
/// FieldEffect_SpawnBurstParticle and about 40 map script sites.
void ObjectGroup_SetChildValue(struct ObjectEvent *objectEvent, s32 param) {
  struct DispatchObject *object = (struct DispatchObject *)objectEvent;

  if ((object->kind & 0xf) == 1) {
    struct Sprite *container = object->target.child;
    u8 partCount = container->count;

    if (partCount != 0) {
      struct SpriteAnim **entry = container->entries;
      u32 count = partCount;
      do {
        struct SpriteAnim *part = *entry++;
        if (part != NULL && part->script != 0) {
          part->colorMode = param;
        }
        count--;
      } while (count != 0);
    }
    container->dirty = 1;
  }
}

/// Links an object event to the object event in the low 8 bits of targetId and
/// starts script on it. Unless OBJ_EVENT_LINK_KEEP_MOTION is set it takes a
/// follow distance of 40 pixels (action 40, read by
/// ObjectMotion_MoveTowardTarget), the target's top speed with twice its
/// acceleration, to keep up with it, and no collision flags, so it cannot block
/// the target. Called by Vale's, Bilibin's, Imil's, Kolima's and McCoy's palace
/// map scripts, among others.
void ObjectEvent_SetTargetAndCallback(u32 objectId, s32 targetId,
                                      const void *script) {
  struct Object *follower = ObjectEvent_GetByLocalId(objectId);
  struct Object *target = ObjectEvent_GetByLocalId(targetId & 0xff);

  if (follower != NULL && target != NULL) {
    follower->linkedObject = target;
    if (!(targetId & OBJ_EVENT_LINK_KEEP_MOTION)) {
      follower->action = 40;
      follower->acceleration = target->acceleration * 2;
      follower->speedLimit = target->speedLimit;
      follower->collisionFlags = 0;
    }
    ObjectDispatch_InitializeFar((struct DispatchObject *)follower, script);
  }
}

/// Script step that turns an object event toward the angle kept in its action:
/// the whole way when within 1/16 turn, otherwise 1/32 turn. Returns the turn
/// made, 0 once the object event faces that way: SC_CALL runs it again each
/// frame while it is nonzero, so the script ends once the turn is done.
/// gObjectMotion_StepAngleScript and gObjectMotion_LaunchScript run it.
s32 ObjectMotion_StepAngle(struct Object *object) {
  s32 delta = 0;

  if (object != NULL) {
    s32 targetAngle = (u16)object->action;
    s32 currentAngle = object->facing;
    delta = (s16)(targetAngle - currentAngle);
    if (delta != 0) {
      if (delta > 4096)
        delta = 2048;
      if (delta < -4096)
        delta = -2048;
      object->facing = currentAngle + delta;
    }
  }
  return delta;
}

/// Clears an object event's destination, puts it under script control and
/// stands it still. Reached only through an unnamed veneer in the object bank's
/// far-call table; nothing in ☀️ names it.
void ObjectEvent_ResetTargetAndSetMode1(u32 objectId) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    object->targetX = OBJ_EVENT_NO_TARGET;
    object->targetY = OBJ_EVENT_NO_TARGET;
    object->targetZ = OBJ_EVENT_NO_TARGET;
    ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
    ObjectDispatch_SelectAnimationFar(object, 1);
  }
}

/// The script command "object event face direction": the object event turns
/// toward angle (0x10000 a full turn; ObjectMotion_StepAngle), then the script
/// waits wait frames. Writes the angle to the object event's action. Map
/// scripts reach it as Engine_ObjectEventFaceDirection (OBJECT_DIR,
/// ObjectEvent_FaceDirection); the Djinni, Retreat and climbing scripts and
/// ObjectEvent_JumpDownLedge call it too.
void ObjectMotion_ArmCallback(s32 objectId, s32 angle, s32 wait) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL) {
    object->action = angle;
    ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                 (const s32 *)gObjectMotion_StepAngleScript);
    FieldScript_WaitFrames(wait);
  }
}

/// The script command "set object event sprite priority": fixes the priority of
/// the object event's sprite, which then no longer follows its depth: clears
/// OBJ_EVENT_PRIORITY_AUTOMATIC. Map scripts reach it as
/// Engine_ObjectEventSetSpritePriority (ObjectEvent_SetSpritePriority);
/// ObjectMotion_CenterOnColumn calls it too.
void ObjectMotion_SetActionVariant(s32 objectId, s32 priority) {
  struct Object *object = ObjectEvent_GetByLocalId(objectId);

  if (object != NULL && (object->animationKind & 0xF) == OBJECT_SPRITE_ONE) {
    struct Sprite *sprite = object->animation;

    sprite->image.priority = priority;
    sprite->shadow.priority = priority;
    object->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  }
}

/// Makes the target object event's sprite show the source object event's
/// graphics (its VRAM block and tile). Neither object event is checked for
/// NULL. Called by the Tolbi ship deck's map script.
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId) {
  struct Object *object;
  struct Sprite *sprite;
  u8 vramBlock;
  u32 tile;

  object = (struct Object *)ObjectEvent_GetById(sourceId);
  sprite = object->animation;
  vramBlock = sprite->slot;
  tile = sprite->image.tile;

  object = (struct Object *)ObjectEvent_GetById(targetId);
  sprite = object->animation;
  sprite->slot = vramBlock;
  sprite->image.tile = tile;
}

/// Unused: an empty function nothing calls.
UNUSED void ObjectVisual_ReservedNoOp(void) {}

/// The script command "set message": the number of the script's next message,
/// written to work->message. Map scripts reach it as
/// Engine_ScriptSetMessage (MESSAGE_SET); the field's talk, coord event, item
/// and Psynergy scripts call it too.
void FieldScript_SetMessageId(s32 message) {
  ((struct FieldWork *)gFieldWork)->message = message;
}

/// Returns the sprite (the first animation's id) of the object event in the low
/// 12 bits of localId, or -1 if there is no such animated object event. The
/// message commands (messages.c) call it to find the speaker's portrait and
/// voice.
s32 ObjectEvent_ReadActiveValue(s32 localId) {
#ifdef UBFIX
  struct FieldWork *state = (struct FieldWork *)gFieldWork;
  s32 sprite = -1;
  struct Object *entry;

  localId &= 0x0fff;
  // BUG: the -1 sentinel becomes index 4095, beyond the event work. All
  // indices from 66 to 4095 lie outside the object event table.
  if (localId >= 66)
    return -1;
  entry = (struct Object *)state->objectEvents[localId];
#else
  struct FieldWork *state = (struct FieldWork *)gFieldWork;
  s32 sprite = -1;
  struct Object *entry = (struct Object *)state->objectEvents[localId & 0x0fff];
#endif

  if (entry != 0 && entry->animationKind == OBJECT_SPRITE_ONE)
    sprite = ((struct Sprite *)entry->animation)->entries[0]->animationId;
  return sprite;
}

/// Returns the first of the map's object events (8 to 65) showing sprite, or
/// -1. Reached only through an unnamed veneer in the object bank's far-call
/// table; nothing in ☀️ names it.
s32 ObjectEvent_FindActiveByValue(s32 sprite) {
  struct FieldWork *state = (struct FieldWork *)gFieldWork;
  s32 result = -1;
  s32 index;
  struct Object *object;

  for (index = 8; index <= 65; index++) {
    object = (struct Object *)state->objectEvents[index];
    if (object != 0 && object->animationKind == OBJECT_SPRITE_ONE &&
        ((struct Sprite *)object->animation)->entries[0]->animationId ==
            sprite) {
      result = index;
      break;
    }
  }
  return result;
}
