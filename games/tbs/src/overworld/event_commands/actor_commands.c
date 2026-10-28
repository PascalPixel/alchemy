/// @file
/// Event commands on actors: placing, moving and animating them.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// Returns an actor, or NULL if it does not exist. Events reach it
/// through Actor_Get and ACTOR_GET; it is the far-callable form of
/// Actor_Lookup that the other banks use.
struct Actor *Actor_GetById(s32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor == NULL)
    return NULL;
  return actor;
}

/// The event command "set actor speed": its top speed and
/// acceleration, 16.16 pixels per frame. Writes the actor's speedLimit
/// and acceleration. Events reach it
/// (ACTOR_SPEED, Actor_SetSpeed).
void Actor_SetSpeedParameters(u32 actorId, s32 speedLimit, s32 acceleration) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->acceleration = acceleration;
    actor->speedLimit = speedLimit;
  }
}

/// Gives an actor a movement type or actor script (Actor_SetMovementType) and
/// makes it face where it moves: sets ACTOR_FACES_MOTION, bit 0 of its
/// actionFlags. Events reach it (Actor_SetScript and
/// Actor_SetMovementTypeById).
void Actor_SetMovementTypeById(u32 actorId, s32 action) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    s32 value = 1;
    value |= actor->steeringFlags;
    actor->steeringFlags = value;
    Actor_SetMovementType(actor, action);
  }
}

/// The event command "stop actor": stops its movement type and puts it back
/// under script control, on the idle script (a lone SC_END), and sets
/// ACTOR_FACES_MOTION, bit 0 of its actionFlags. Events reach it
/// (Actor_Stop), about 22 of them.
void Actor_Stop(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);
  u8 enabled;

  if (actor != NULL) {
    enabled = 1;
    actor->steeringFlags = enabled | actor->steeringFlags;
    Actor_StartIdleFar(actor);
  }
}

/// Makes an actor follow another (the linked actor script): writes its
/// linkedActor. Called by the Colosso log's event, and by Altmiller
/// Cave's.
void Actor_LinkAndSetScript(u32 actorId, u32 linkedActorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->linkedActor = Actor_GetById(linkedActorId);
    Actor_SetMovementType(actor, (s32)gActor_LinkedScript);
  }
}

/// Waits, at most 300 frames, for an actor's script to reach its end
/// (command 0x10). Called by Actor_RunRepeatedMotion, the climbing
/// events and about 28 others.
void Actor_RefreshSelectorById(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL)
    Actor_WaitForScriptEndFar(actor);
}

/// Starts a movement type or supplied actor script, as
/// Actor_SetMovementTypeById does, then waits up to 300 frames
/// for its script to end. Event calls use
/// Actor_RunScriptAndWait.
void Actor_SetMovementTypeAndWaitById(u32 actorId, s32 action) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    s32 value = 1;
    value |= actor->steeringFlags;
    actor->steeringFlags = value;
    Actor_SetMovementType(actor, action);
    Actor_WaitForScriptEndFar(actor);
  }
}

/// The event command "set actor destination": the actor heads
/// for (x, z), in pixels, keeping its current animation. Clears its
/// movementState, so a paused actor moves again, as the other move
/// commands here do. Events reach it
/// (Actor_SetDestination).
void Actor_ResetAndSetPosition(u32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SetMoveTargetFar(actor, x << 16, actor->y.fixed, z << 16);
  }
}

/// The event command "move actor and wait": as
/// Actor_ResetAndSetPosition, then waits (at most 600 frames) for the
/// actor to arrive. Events reach it (Actor_MoveToAndWait);
/// the climbing events call it too.
void Actor_SetPositionAndCommit(u32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SetMoveTargetFar(actor, x << 16, actor->y.fixed, z << 16);
    Actor_WaitForMoveFar(actor);
  }
}

/// The event command "walk actor to": the actor walks (animation
/// 2) toward (x, z), in pixels. Events reach it (ACTOR_WALK,
/// Actor_WalkTo); the Battle Arena calls it too.
void Actor_ResetAndSetPositionInMode2(u32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SelectAnimationFar(actor, 2);
    Actor_SetMoveTargetFar(actor, x << 16, actor->y.fixed, z << 16);
  }
}

/// The event command "walk actor to and wait": walks the actor
/// to (x, z), in pixels, waits for it to arrive and stands it still (animation
/// 1). Events reach it
/// (Actor_WalkToAndWait).
void Actor_SetPositionAndReset(u32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SelectAnimationFar(actor, 2);
    Actor_SetMoveTargetFar(actor, x << 16, actor->y.fixed, z << 16);
    Actor_WaitForMoveFar(actor);
    Actor_SelectAnimationFar(actor, 1);
  }
}

/// The event command "center actor and walk": walks the actor
/// sideways to the middle of its 16-pixel column, sets its sprite priority to
/// priority (Actor_SetSpritePriority), then sends it zOffset pixels along
/// z. Kalay's interiors reach it.
void Actor_CenterOnColumn(u32 actorId, s32 priority, s32 zOffset) {
  struct Actor *actor;
  s16 pixelX;
  s32 columnX;
  s16 offsetInColumn;

  actor = Actor_Lookup(actorId);
  if (actor != NULL) {
    // The column's left edge, from a division by 16 that rounds toward zero
    // (left of x = 0 it finds the edge to the right); its middle is 8 pixels
    // in.
    pixelX = ((union FieldCoordinate *)&actor->x.fixed)->part.pixel;
    columnX = pixelX;
    if (pixelX < 0) {
      columnX += 15;
    }
    columnX >>= 4;
    columnX *= 16;
    offsetInColumn = pixelX - columnX;
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SelectAnimationFar(actor, 2);
    Actor_SetMoveTargetFar(actor, actor->x.fixed + ((8 - offsetInColumn) << 16),
                           actor->y.fixed, actor->z.fixed);
    Actor_WaitForMoveFar(actor);
    Actor_SetSpritePriority(actorId, priority);
    Actor_SetMoveTargetFar(actor, actor->x.fixed, actor->y.fixed,
                           actor->z.fixed + (zOffset << 16));
  }
}

/// The event command "set actor destination offset": the actor
/// heads (xOffset, zOffset) pixels from where it stands. Events reach it
/// (Actor_SetDestinationOffset); Actor_LaunchFromLeader
/// calls it too.
void Actor_OffsetPositionAndResetMotion(u32 actorId, s32 xOffset, s32 zOffset) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SetMoveTargetFar(actor, actor->x.fixed + (xOffset << 16),
                           actor->y.fixed, actor->z.fixed + (zOffset << 16));
  }
}

/// Walks the actor (animation 2) (xOffset, zOffset) pixels from where it
/// stands. Events reach it
/// (Actor_WalkBy); Actor_CommitPositionAndActivate calls it too.
void Actor_OffsetPositionAndReset(u32 actorId, s32 xOffset, s32 zOffset) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SelectAnimationFar(actor, 2);
    Actor_SetMoveTargetFar(actor, actor->x.fixed + (xOffset << 16),
                           actor->y.fixed, actor->z.fixed + (zOffset << 16));
  }
}

/// Walks the actor (xOffset, zOffset) pixels, waits for it to arrive and
/// stands it still. Events reach it (Actor_WalkByAndWait); the
/// Lemurian ship calls it by name.
void Actor_CommitPositionAndActivate(u32 actorId, s32 xOffset, s32 zOffset) {
  struct Actor *actor = Actor_Lookup(actorId);

  Actor_OffsetPositionAndReset(actorId, xOffset, zOffset);
  if (actor != NULL) {
    Actor_WaitForMoveFar(actor);
    Actor_SelectAnimationFar(actor, 1);
  }
}

/// Places an actor on the leader and sends it walking (xOffset, zOffset)
/// pixels away at speed 0.6 and acceleration 0.3, through the launch script;
/// the actor then takes movement type action. Called by the Lemurian ship, Babi
/// Lighthouse and Lalivero events.
void Actor_LaunchFromLeader(u32 actorId, s32 xOffset, s32 zOffset, s32 action) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    struct Actor *leader;

    Actor_SetSpeedParameters(actorId, 0x9999, 0x4CCC);
    leader = Actor_GetById(gAdventure.leaderActorId);
    if (leader != NULL)
      Actor_SetHorizontalPositionWithTerrain(actorId, leader->x.fixed,
                                             leader->z.fixed);
    actor->paused = 0;
    Actor_SelectAnimationFar(actor, 2);
    Actor_OffsetPositionAndResetMotion(actorId, xOffset, zOffset);
    Actor_StartScriptFar(actor, (const s32 *)&gActor_LaunchScript);
    actor->timer = action;
  }
}

/// The event command "wait for actor": waits (at most 600 frames) for
/// the actor to arrive and stands it still. Events reach it
/// (Actor_WaitForMoveById); the Battle Arena and the climbing events call it
/// too.
void Actor_WaitForMoveById(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    Actor_WaitForMoveFar(actor);
    Actor_SelectAnimationFar(actor, 1);
  }
}

/// The event command "set actor position": puts the actor at (x,
/// z), 16.16 pixels, at rest. An actor that follows the ground
/// (ACTOR_FOLLOWS_TERRAIN) keeps its height above the ground at the new
/// place. Clears its velocity and targets. Events reach it as
/// Import_ActorSetPosition (ACTOR_POSITION_SET,
/// Actor_SetPosition); Actor_LaunchFromLeader calls it too.
void Actor_SetHorizontalPositionWithTerrain(u32 actorId, s32 x, s32 z) {
  struct Actor *object;
  s32 groundHeight;
  s32 tileX;
  s32 tileZ;
  s32 terrainId;

  object = Actor_Lookup(actorId);
  if (object != NULL) {
    Actor_StartIdleFar(object);
    SET_VELOCITY(object, 0, 0, 0);
    object->targetY = ACTOR_NO_TARGET;
    object->targetX = ACTOR_NO_TARGET;
    object->x.fixed = x;
    object->z.fixed = z;
    // Swap the old ground's height for the new one, so the actor's
    // height above the ground is kept. The coordinates are whole pixels,
    // rounded toward zero. MapGrid_GetTerrainHeight takes 16.16 coordinates and
    // returns 16.16 pixels, so these pixel coordinates read the layer's first
    // cell, and the << 16 keeps only the result's low 16 bits (0 on a flat
    // cell). The actor is left at its height above the ground, counted
    // from 0, for the rest of this frame; before it is drawn,
    // Actor_UpdateAllThumb's ground following adds the new ground's height,
    // so one standing on the ground lands on it there.
    if (1 & object->motionFlags) {
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
      object->y.fixed = (object->y.fixed - object->groundHeight) + groundHeight;
      object->groundHeight = groundHeight;
    }
  }
}

/// Puts the actor at (x, y, z), 16.16 pixels, at rest; an actor
/// that follows the ground (ACTOR_FOLLOWS_TERRAIN) is raised by the ground
/// height there, read as Actor_SetHorizontalPositionWithTerrain reads
/// it. Nothing in ☀️ calls it.
void Actor_SetPositionWithTerrain(u32 actorId, s32 x, s32 y, s32 z) {
  struct Actor *actor;
  s32 groundHeight;
  s32 tileX;
  s32 tileZ;

  actor = Actor_Lookup(actorId);
  if (actor != NULL) {
    Actor_StartIdleFar(actor);
    SET_VELOCITY(actor, 0, 0, 0);
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetX = ACTOR_NO_TARGET;
    SET_ACTOR_POSITION(actor, x, y, z);
    if (1 & actor->motionFlags) {
      s32 terrainId = actor->layer;
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
      actor->y.fixed = (actor->y.fixed - actor->groundHeight) + groundHeight;
      actor->groundHeight = groundHeight;
    }
  }
}

/// The event command "set actor animation" (1 stands, 2 walks, 3 nods,
/// 4 shakes the head and so on). Events reach it as
/// Import_ActorSetAnimation (ACTOR_ANIMATION, Actor_SetAnimation);
/// Event_AskYesNoWithActorAnimation and
/// Actor_SetAnimationAndWait call it too.
void Actor_SetModeById(u32 actorId, s32 animation) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL)
    Actor_SelectAnimationFar(actor, animation);
}

/// Sets the animation timer steps of each of an actor's sprites
/// (Sprite_SetAnimSpeed). Called by the Tolbi ship and
/// Lamakan Desert events.
void Actor_SetAnimationSpeedById(u32 actorId, s32 steps) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL)
    Actor_SetAnimationSpeedFar(actor, steps);
}

/// Waits, at most 90 frames, for an actor's animation to change. Only an
/// actor drawn as a sprite (ACTOR_SPRITE_ONE) is waited for.
/// Called by Actor_SetAnimationAndWait, a few events and the debug
/// level screen.
void Actor_WaitForAnimationChange(u32 actorId) {
  struct Actor *actor;
  struct Sprite *sprite;
  /* FAKEMATCH(volatile): saved lives in a 4-byte stack frame, stored once and
   * loaded again for each compare, as the bytes have it; as a plain s32, 29
   * of 31 instructions differ in tbs-en. */
  volatile s32 saved;
  s32 elapsedFrames;

  actor = Actor_Lookup(actorId);
  if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    sprite = actor->sprite;
    saved = sprite->lastNo;
    for (elapsedFrames = 0; elapsedFrames <= 89; elapsedFrames++) {
      WaitFrames(1);
      if (saved != sprite->lastNo) {
        break;
      }
    }
  }
}

/// The event command "set actor animation and wait": plays the
/// animation and waits for it to end. Events reach it
/// (Actor_SetAnimationAndWait).
void Actor_SetAnimationAndWait(u32 actorId, s32 animation) {
  Actor_SetModeById(actorId, animation);
  Actor_WaitForAnimationChange(actorId);
}

/// An empty function, reached only through an unnamed veneer in the object
/// bank's far-call table; nothing in ☀️ names it.
void Actor_NoOp(void) {}

/// The event command "actor jump": throws the actor upward at
/// speed pixels per frame with a jump sound (a higher one above 5), then waits
/// frames frames. Sets bit 1 of the actor's motion flags, so gravity
/// brings it down, and its velocityY. Events reach it (Actor_Jump),
/// about 40 of them; the events of Djinn joining the party (djinni.c and the
/// four element parts) call it too.
void Actor_Jump(u32 actorId, s32 speed, s32 frames) {
  struct Actor *actor;
  u8 *objectFlags;
  s32 updatedFlags;
  s32 verticalVelocity;

  actor = Actor_Lookup(actorId);
  if (actor != NULL) {
    objectFlags = &actor->motionFlags;
    updatedFlags = *objectFlags | ACTOR_FALLS;
    verticalVelocity = speed << 16;
    *objectFlags = updatedFlags;
    actor->velocityY = verticalVelocity;
    if (speed > 5) {
      Sound_PlaySongFar(SE_HIGH_JUMP);
    } else {
      Sound_PlaySongFar(SE_JUMP);
    }
    Event_WaitFrames(frames);
  }
}

/// The event command "start repeated motion": the actor squashes and
/// springs back count times (1 to 3), through gActor_VariantScripts. Events
/// reach it
/// (Actor_StartRepeatedMotion).
void Actor_StartRepeatedMotion(u32 actorId, s32 count) {
  struct Actor *actor;

  actor = Actor_Lookup(actorId);
  if (actor != NULL && count > 0) {
    if (count > 3) {
      count = 3;
    }
    // Each count is 32 words of the script; start that many from its end.
    Actor_StartScriptFar(
        actor, (const s32 *)(gActor_VariantScripts + ((3 - count) << 7)));
  }
}

/// The event command "run repeated motion": as
/// Actor_StartRepeatedMotion, then waits for it to end. Events reach
/// it (Actor_RunRepeatedMotion);
/// the events of Djinn joining the party call it too.
void Actor_RunRepeatedMotion(u32 actorId, s32 count) {
  Actor_StartRepeatedMotion(actorId, count);
  Actor_RefreshSelectorById(actorId);
}

/// Moves a dust puff of FieldEffect_SpawnBurstParticle: it drifts with its x
/// and z velocity (kept in speed and acceleration), which drag slows by 1/18
/// and 1/16 each frame, and rises 1/64 pixel a frame. Runs each frame as the
/// puff's update routine.
void FieldEffect_UpdateParticleLinearMotion(struct Actor *actor) {
  struct Actor *particle = (struct Actor *)actor;
  s32 velocityX;
  s32 x;
  s32 velocityZ;
  s32 z;
  s32 y;
  s32 roundedZ;
  velocityX = particle->speedLimit;
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
  particle->speedLimit = (velocityX - velocityX / 0x12);

  roundedZ = velocityZ;
  if (velocityZ < 0) {
    roundedZ += 0xF;
  }
  particle->acceleration = (velocityZ - (roundedZ >> 4));
}

/// Kicks up a dust puff (SPRITE_DUST_PUFF) where source stands: it flies off
/// toward the back at 0.5 to 1.4 pixels a frame and sideways at up to 1.4, with
/// one of two animations. A nonzero param is given to its sprite parts
/// (Actor_SetSpriteColor). Only Actor_JumpDownLedge calls it.
void FieldEffect_SpawnBurstParticle(struct Actor *source, s32 param) {
  struct Actor *actor;
  struct Sprite *child;
  s32 speed;

  actor = Actor_CreateFar(SPRITE_DUST_PUFF, source->x.fixed, source->y.fixed,
                          source->z.fixed);
  if (actor != 0) {
    child = actor->sprite;
    switch (Random16() & 1) {
    case 1:
      Actor_SelectAnimationFar(actor, 2);
      Actor_StartScriptFar(actor,
                           (const s32 *)gFieldEffect_BurstParticleScriptA);
      break;
    default:
      Actor_SelectAnimationFar(actor, 1);
      Actor_StartScriptFar(actor,
                           (const s32 *)gFieldEffect_BurstParticleScriptB);
      break;
    }

    if (param != 0)
      Actor_SetSpriteColor(actor, param);

    // speed and acceleration hold the puff's x and z velocity, in steps of
    // 0x1999 (0.1 pixel): z is -0.5 to -1.4, x -1.4 to 1.4 in steps of 0.2.
    actor->motionFlags = 0;
    speed = Random16() % 10 + 5;
    actor->acceleration = -0x1999 * speed;
    speed = Random16() % 15 - 7;
    speed <<= 1;
    actor->speedLimit = 0x1999 * speed;
    actor->timer = 0;
    actor->update = FieldEffect_UpdateParticleLinearMotion;
    child->flags = 0;
    child->image.priority = source->sprite->image.priority;
  }
}

/// Makes an actor jump down off a ledge in front of it (the field runs
/// it for the leader when the player presses down at a ledge): it shows pose
/// for 10 frames, hops 12 pixels south, turns toward angle 0x5000 (1/16 turn
/// past FACING_SOUTH) while sinking 28 pixels with a dust puff every other
/// frame (none when param is -1), then jumps on to 16 pixels south of where it
/// started and lands. The landing plays the queued sound
/// (Scene_PlayQueuedMusic). Called by Field_RunPushCell for a ledge
/// (MB_LEDGE) with pose 6 and param 0, and by seven scenes through
/// FieldActor_JumpDownLedge; Mercury Lighthouse passes -1.
void Actor_JumpDownLedge(s32 actorId, s32 pose, s32 param) {
  s32 nextY;
  s32 startZ;
  struct Actor *actor;
  u8 *objectFlags;

  actor = Actor_GetById(actorId);
  startZ = actor->z.fixed;
  if (actor != 0) {
    Sound_PlaySongFar(SONG_FADE_OUT_ACTION_SE);
    Actor_SelectAnimationFar(actor, pose);
    WaitFrames(10);

    // Hop up and forward 12 pixels.
    objectFlags = &actor->motionFlags;
    Actor_SelectAnimationFar(actor, 1);
    {
      u8 flags = 2;
      flags |= *objectFlags;
      *objectFlags = flags;
    }
    actor->velocityY = 0x40000;
    Actor_SetMoveTargetFar(actor, actor->x.fixed, actor->y.fixed,
                           startZ + 0xC0000);
    WaitFrames(6);

    // Turn and sink 2 pixels a frame for 14 frames, raising dust, in free
    // flight: no motion flags, so it neither falls nor follows the ground.
    Sound_PlaySongFar(SE_FLOOR_SWITCH);
    // From here pose counts frames.
    pose = 0;
    Actor_ArmCallback(actorId, FACING_SOUTH + FACING_STEP, 0);
    *objectFlags = 0;
    do {
      nextY = actor->y.fixed + 0xFFFE0000;
      actor->y.fixed = nextY;
      actor->targetY = nextY;
      WaitFrames(1);
      if ((param != -1) && (pose & 1)) {
        FieldEffect_SpawnBurstParticle(actor, param);
      }
      pose++;
    } while ((u32)pose <= 0xD);

    // Jump 16 pixels forward and wait (at most 180 frames) to land, following
    // the ground and falling again.
    *objectFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->velocityY = 0x30000;
    Actor_SetMoveTargetFar(actor, actor->x.fixed, actor->y.fixed,
                           startZ + 0x100000);
    Actor_WaitForMoveFar(actor);
    for (pose = 0; (u32)pose <= 0xB3 && actor->y.fixed > actor->groundHeight;
         pose++) {
      WaitFrames(1);
    }
    WaitFrames(2);
    Scene_PlayQueuedMusic();
  }
}

/// The event command "actor face actor": turns the first
/// actor at once to face the second, then waits wait frames. Writes the first
/// actor's angle. Events reach it (Actor_FaceActor); the Battle Arena calls it
/// too.
void Actor_SetAngleToward(s32 firstId, s32 secondId, s32 wait) {
  struct Actor *first;
  struct Actor *second;

  first = Actor_Lookup(firstId);
  second = Actor_Lookup(secondId);
  if (first != 0 && second != 0) {
    first->facing = ArcTan2(second->z.fixed - first->z.fixed,
                            second->x.fixed - first->x.fixed);
    Event_WaitFrames(wait);
  }
}

/// The event command "actors face each other": turns two actors
/// toward each other, then waits wait frames. Events reach it
/// (Actor_FaceEachOther); Scene_TalkToActor turns the leader and
/// the actor spoken to with it.
void Actor_LinkPair(s32 firstId, s32 secondId, s32 wait) {
  void *first = Actor_Lookup(firstId);
  void *second = Actor_Lookup(secondId);

  if (first != NULL && second != NULL) {
    Actor_TurnPairToFaceEachOther(first, second);
    Event_WaitFrames(wait);
  }
}

/// Turns two actors to face each other, at most 1/16 turn a frame each,
/// until both do (at most 60 frames). Only Actor_LinkPair calls it.
void Actor_TurnPairToFaceEachOther(struct Actor *first, struct Actor *second) {
  s32 firstTargetAngle;
  s32 secondTargetAngle;
  s32 frame;
  s32 actorsTurning;
  s32 angleDifference;

  if (first == NULL || second == NULL)
    return;
  firstTargetAngle = (u16)ArcTan2(second->z.fixed - first->z.fixed,
                                  second->x.fixed - first->x.fixed);
  // The second actor faces the opposite way, half a turn round.
  secondTargetAngle = firstTargetAngle + 0x8000;
  for (frame = 0; frame < 60; frame++) {
    // Each actor that already faces its way takes one off the count;
    // both facing ends the turn early.
    actorsTurning = 2;
    angleDifference = (s16)(firstTargetAngle - first->facing);
    if (angleDifference != 0) {
      if (angleDifference > 0x1000)
        angleDifference = 0x1000;
      if (angleDifference < -0x1000)
        angleDifference = -0x1000;
      first->facing += angleDifference;
    } else {
      actorsTurning = 1;
    }
    angleDifference = (s16)(secondTargetAngle - second->facing);
    if (angleDifference != 0) {
      if (angleDifference > 0x1000)
        angleDifference = 0x1000;
      if (angleDifference < -0x1000)
        angleDifference = -0x1000;
      second->facing += angleDifference;
    } else {
      actorsTurning--;
    }
    if (actorsTurning == 0)
      break;
    WaitFrames(1);
  }
}

/// The event command "release actor": releases the actor and
/// empties its slot, writing NULL to its place in the field work's actor
/// table. Events reach it (about 14 of them).
void Actor_ReleaseById(s32 index) {
  void *object = Actor_Lookup(index);
  struct FieldWork *table = gFieldWork;
#ifdef UBFIX
  struct Actor *empty = NULL;
#endif

  if (object != 0) {
    Actor_ReleaseFar(object);
#ifdef UBFIX
    // The getter accepts slots through 191, including non-actor storage.
    // Copy the pointer representation without extending the actor array.
    memcpy((u8 *)table + 0x14 + index * sizeof(void *), &empty, sizeof(empty));
#else
    // UB: the getter accepts slots through 191, past the 66 actors.
    table->actors[index] = NULL;
#endif
  }
}

UNUSED void Actor_ReleaseNoOp(void) {}

/// The event command "set actor sprite param": gives every sprite part
/// of the actor param (Actor_SetSpriteColor). With bit 0x100 set
/// the actor blinks instead (Actor_UpdateBlink, every
/// frame); any other param also ends a blink, clearing the actor's update
/// routine. Events reach it (Actor_SetSpriteColorById).
void Actor_ConfigureSpriteColor(s32 actorId, s32 param) {
  s32 blink;
  struct Actor *actor;

  actor = Actor_Lookup(actorId);
  if (actor != NULL) {
    blink = 0x100 & param;
    if (blink != 0) {
      (actor)->update = Actor_UpdateBlink;
      return;
    }
    (actor)->update = NULL;
    Actor_SetSpriteColor(actor, param);
  }
}

/// Per-frame update routine of a blinking actor: sets its sprite parts' param
/// from gActor_BlinkSpriteColors (7 for two frames out of eight, otherwise 0).
/// Actor_ConfigureSpriteColor installs it.
void Actor_UpdateBlink(struct Actor *actor) {
  struct Actor *object = (struct Actor *)actor;

  if ((object->spriteStorageKind & 0xf) == 1) {
    u8 param;
    struct Sprite *container;
    u8 partCount;

    // The table's four entries each last two frames.
    param = gActor_BlinkSpriteColors[(gFrameCount >> 1) & 3];
    container = object->sprite;
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

/// Sets the param byte of every animated part of an actor's sprite. Events
/// set 0 or 15, and alternate the two to make an actor flash.
/// Marks the sprite dirty. Called by Actor_ConfigureSpriteColor,
/// FieldEffect_SpawnBurstParticle and about 40 event sites.
void Actor_SetSpriteColor(struct Actor *actor, s32 param) {
  struct Actor *object = actor;

  if ((object->spriteStorageKind & 0xf) == 1) {
    struct Sprite *container = object->sprite;
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

/// Links an actor to the actor in the low 8 bits of targetId and
/// starts script on it. Unless ACTOR_LINK_KEEP_MOTION is set it takes a
/// follow distance of 40 pixels (timer 40, read by
/// Actor_MoveTowardTarget), the target's top speed with twice its
/// acceleration, to keep up with it, and no collision flags, so it cannot block
/// the target. Called by Vale's, Bilibin's, Imil's, Kolima's and McCoy's palace
/// events, among others.
void Actor_SetTargetAndCallback(u32 actorId, s32 targetId, const void *script) {
  struct Actor *follower = Actor_Lookup(actorId);
  struct Actor *target = Actor_Lookup(targetId & 0xff);

  if (follower != NULL && target != NULL) {
    follower->linkedActor = target;
    if (!(targetId & ACTOR_LINK_KEEP_MOTION)) {
      follower->timer = 40;
      follower->acceleration = target->acceleration * 2;
      follower->speedLimit = target->speedLimit;
      follower->collisionFlags = 0;
    }
    Actor_StartScriptFar(follower, script);
  }
}

/// SC_CALL routine that turns an actor toward the angle kept in its timer:
/// the whole way when within 1/16 turn, otherwise 1/32 turn. Returns the turn
/// made, 0 once the actor faces that way: SC_CALL runs it again each
/// frame while it is nonzero, so the script ends once the turn is done.
/// gActor_StepAngleScript and gActor_LaunchScript run it.
s32 Actor_StepAngle(struct Actor *actor) {
  s32 delta = 0;

  if (actor != NULL) {
    s32 targetAngle = (u16)actor->timer;
    s32 currentAngle = actor->facing;
    delta = (s16)(targetAngle - currentAngle);
    if (delta != 0) {
      if (delta > 4096)
        delta = 2048;
      if (delta < -4096)
        delta = -2048;
      actor->facing = currentAngle + delta;
    }
  }
  return delta;
}

/// Clears an actor's destination, puts it under script control and
/// stands it still. Reached only through an unnamed veneer in the overworld
/// bank's far-call table; nothing in ☀️ names it.
void Actor_ResetTargetAndSetMode1(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    Actor_StartIdleFar(actor);
    Actor_SelectAnimationFar(actor, 1);
  }
}

/// The event command "actor face direction": the actor turns
/// toward angle (0x10000 a full turn; Actor_StepAngle), then the event
/// waits wait frames. Writes the angle to the actor's timer. Events
/// reach it (ACTOR_DIR,
/// Actor_FaceDirection); the Djinni, Retreat and climbing events and
/// Actor_JumpDownLedge call it too.
void Actor_ArmCallback(s32 actorId, s32 angle, s32 wait) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->timer = angle;
    Actor_StartScriptFar(actor, (const s32 *)gActor_StepAngleScript);
    Event_WaitFrames(wait);
  }
}

/// The event command "set actor sprite priority": fixes the priority of
/// the actor's sprite, which then no longer follows its depth: clears
/// ACTOR_PRIORITY_AUTOMATIC. Events reach it
/// (Actor_SetSpritePriority); Actor_CenterOnColumn calls it too.
void Actor_SetSpritePriority(s32 actorId, s32 priority) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL && (actor->spriteStorageKind & 0xF) == ACTOR_SPRITE_ONE) {
    struct Sprite *sprite = actor->sprite;

    sprite->image.priority = priority;
    sprite->shadow.priority = priority;
    actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// Makes the target actor's sprite show the source actor's
/// graphics (its VRAM block and tile). Neither actor is checked for
/// NULL. Called by the Tolbi ship deck's event.
void Actor_CopyGraphics(u32 targetId, u32 sourceId) {
  struct Actor *actor;
  struct Sprite *sprite;
  u8 vramBlock;
  u32 tile;

  actor = Actor_GetById(sourceId);
  sprite = actor->sprite;
  vramBlock = sprite->slot;
  tile = sprite->image.tile;

  actor = Actor_GetById(targetId);
  sprite = actor->sprite;
  sprite->slot = vramBlock;
  sprite->image.tile = tile;
}

UNUSED void Actor_ReservedNoOp(void) {}

/// The event command "set message": the number of the event's next message,
/// written to work->message. Events reach it (MESSAGE_SET); the field's
/// talk, step trigger, item and Psynergy events call it too.
void Event_SetMessageId(s32 message) { gFieldWork->message = message; }

/// Returns the sprite (the first animation's id) of the actor in the low
/// 12 bits of actorId, or -1 if there is no such animated actor. The
/// message commands (messages.c) call it to find the speaker's portrait and
/// voice.
s32 Actor_ReadActiveValue(s32 actorId) {
#ifdef UBFIX
  struct FieldWork *state = gFieldWork;
  s32 sprite = -1;
  struct Actor *entry;

  actorId &= 0x0fff;
  // BUG: the -1 sentinel becomes index 4095, beyond the field work. All
  // indices from 66 to 4095 lie outside the actor table.
  if (actorId >= 66)
    return -1;
  entry = state->actors[actorId];
#else
  struct FieldWork *state = gFieldWork;
  s32 sprite = -1;
  struct Actor *entry = state->actors[actorId & 0x0fff];
#endif

  if (entry != 0 && entry->spriteStorageKind == ACTOR_SPRITE_ONE)
    sprite = entry->sprite->entries[0]->animationId;
  return sprite;
}

/// Returns the first of the scene's actors (8 to 65) showing sprite, or
/// -1. Reached only through an unnamed veneer in the overworld bank's far-call
/// table; nothing in ☀️ names it.
s32 Actor_FindActiveByValue(s32 sprite) {
  struct FieldWork *state = gFieldWork;
  s32 result = -1;
  s32 index;
  struct Actor *actor;

  for (index = 8; index <= 65; index++) {
    actor = state->actors[index];
    if (actor != 0 && actor->spriteStorageKind == ACTOR_SPRITE_ONE &&
        actor->sprite->entries[0]->animationId == sprite) {
      result = index;
      break;
    }
  }
  return result;
}
