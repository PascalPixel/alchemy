/// @file
/// Event commands on actors: placing, moving and animating them.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Returns an actor, or NULL if it does not exist.
struct Actor *Actor_GetById(s32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor == NULL)
    return NULL;
  return actor;
}

/// Sets an actor's top speed and acceleration, in 16.16 pixels per frame.
void Actor_SetSpeedParameters(u32 actorId, FIXED speedLimit,
                              FIXED acceleration) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->acceleration = acceleration;
    actor->speedLimit = speedLimit;
  }
}

/// Gives an actor a movement type or actor script (Actor_SetMovementType) and
/// makes it face where it moves (ACTOR_FACES_MOTION).
void Actor_SetMovementTypeById(u32 actorId, s32 action) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_SetMovementType(actor, action);
  }
}

/// Stops an actor's movement type and puts it back under script control, on
/// the idle script (a lone SC_END), and sets ACTOR_FACES_MOTION.
void Actor_Stop(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_StartIdleFar(actor);
  }
}

/// Makes an actor follow another (the watched actor script): writes its
/// watchedActor.
void Actor_WatchAndSetScript(u32 actorId, u32 watchedActorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->watchedActor = Actor_GetById(watchedActorId);
    Actor_SetMovementType(actor, (s32)gActor_FaceWatchedScript);
  }
}

/// Waits, at most 300 frames, for an actor's script to reach its end (SC_END).
void Actor_WaitForScriptEndById(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL)
    Actor_WaitForScriptEndFar(actor);
}

/// Starts a movement type or supplied actor script, as
/// Actor_SetMovementTypeById does, then waits up to 300 frames for its script
/// to end.
void Actor_SetMovementTypeAndWaitById(u32 actorId, s32 action) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_SetMovementType(actor, action);
    Actor_WaitForScriptEndFar(actor);
  }
}

/// Sends an actor toward (x, z), in pixels, keeping its current animation.
void Actor_SetDestinationById(u32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SetMoveTargetFar(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
  }
}

/// As Actor_SetDestinationById, then
/// waits (at most 600 frames) for the actor to arrive.
void Actor_MoveToAndWaitById(u32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SetMoveTargetFar(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
    Actor_WaitForMoveFar(actor);
  }
}

/// Walks an actor toward (x, z), in pixels.
void Actor_WalkToById(u32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SelectAnimationFar(actor, ANIM_WALK);
    Actor_SetMoveTargetFar(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
  }
}

/// Walks the actor to (x, z), in
/// pixels, waits for it to arrive and stands it still.
void Actor_WalkToAndWaitById(u32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SelectAnimationFar(actor, ANIM_WALK);
    Actor_SetMoveTargetFar(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
    Actor_WaitForMoveFar(actor);
    Actor_SelectAnimationFar(actor, ANIM_STAND);
  }
}

/// Walks the actor sideways to the
/// middle of its 16-pixel column, sets its sprite priority to priority
/// (Actor_SetSpritePriority), then sends it zOffset pixels along z.
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
    pixelX = actor->x.part[0].integer;
    columnX = pixelX;
    if (pixelX < 0) {
      columnX += 15;
    }
    columnX >>= 4;
    columnX *= 16;
    offsetInColumn = pixelX - columnX;
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SelectAnimationFar(actor, ANIM_WALK);
    Actor_SetMoveTargetFar(actor, actor->x.fixed + toFIXED(8 - offsetInColumn),
                           actor->y.fixed, actor->z.fixed);
    Actor_WaitForMoveFar(actor);
    Actor_SetSpritePriority(actorId, priority);
    Actor_SetMoveTargetFar(actor, actor->x.fixed, actor->y.fixed,
                           actor->z.fixed + toFIXED(zOffset));
  }
}

/// Sends an actor (xOffset, zOffset) pixels from where it stands, keeping its
/// current animation.
void Actor_SetDestinationOffsetById(u32 actorId, s32 xOffset, s32 zOffset) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SetMoveTargetFar(actor, actor->x.fixed + toFIXED(xOffset),
                           actor->y.fixed, actor->z.fixed + toFIXED(zOffset));
  }
}

/// Walks the actor (xOffset, zOffset) pixels from where it stands. Events
/// reach it (Actor_WalkBy); Actor_WalkByAndWaitById calls it too.
void Actor_WalkByById(u32 actorId, s32 xOffset, s32 zOffset) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SelectAnimationFar(actor, ANIM_WALK);
    Actor_SetMoveTargetFar(actor, actor->x.fixed + toFIXED(xOffset),
                           actor->y.fixed, actor->z.fixed + toFIXED(zOffset));
  }
}

/// Walks the actor (xOffset, zOffset) pixels, waits for it to arrive and stands
/// it still.
void Actor_WalkByAndWaitById(u32 actorId, s32 xOffset, s32 zOffset) {
  struct Actor *actor = Actor_Lookup(actorId);

  Actor_WalkByById(actorId, xOffset, zOffset);
  if (actor != NULL) {
    Actor_WaitForMoveFar(actor);
    Actor_SelectAnimationFar(actor, ANIM_STAND);
  }
}

/// Places an actor on the leader and sends it walking (xOffset, zOffset) pixels
/// away at speed 0.6 and acceleration 0.3; once there, the launch script turns
/// it to face facing.
void Actor_LaunchFromLeader(u32 actorId, s32 xOffset, s32 zOffset, s32 facing) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    struct Actor *leader;

    Actor_SetSpeedParameters(actorId, toFIXED(3) / 5, toFIXED(3) / 10);
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (leader != NULL)
      Actor_SetHorizontalPositionWithTerrain(actorId, leader->x.fixed,
                                             leader->z.fixed);
    actor->paused = 0;
    Actor_SelectAnimationFar(actor, ANIM_WALK);
    Actor_SetDestinationOffsetById(actorId, xOffset, zOffset);
    Actor_StartScriptFar(actor, (const s32 *)&gActor_LaunchScript);
    actor->timer = facing;
  }
}

/// Waits (at most 600 frames) for the actor
/// to arrive and stands it still.
void Actor_WaitForMoveById(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    Actor_WaitForMoveFar(actor);
    Actor_SelectAnimationFar(actor, ANIM_STAND);
  }
}

/// Puts the actor at (x, z), 16.16
/// pixels, at rest.
void Actor_SetHorizontalPositionWithTerrain(u32 actorId, FIXED x, FIXED z) {
  struct Actor *actor;
  FIXED groundHeight;
  s32 tileX;
  s32 tileZ;
  s32 terrainId;

  actor = Actor_Lookup(actorId);
  if (actor != NULL) {
    Actor_StartIdleFar(actor);
    SET_VELOCITY(actor, 0, 0, 0);
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetX = ACTOR_NO_TARGET;
    actor->x.fixed = x;
    actor->z.fixed = z;
    // Swap the old ground's height for the new one, so the actor's height above
    // the ground is kept.
    if (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN) {
      terrainId = actor->layer;
      tileX = x;
      if (tileX < 0) {
        tileX += FIXED_ONE - 1;
      }
      tileX = FIXEDtoINT(tileX);
      tileZ = z;
      if (tileZ < 0) {
        tileZ += FIXED_ONE - 1;
      }
      tileZ = FIXEDtoINT(tileZ);
      groundHeight =
          toFIXED(MapGrid_GetTerrainHeightFar(terrainId, tileX, tileZ));
      actor->y.fixed = (actor->y.fixed - actor->groundHeight) + groundHeight;
      actor->groundHeight = groundHeight;
    }
  }
}

/// Puts the actor at (x, y, z), 16.16 pixels, at rest; an actor that follows
/// the ground (ACTOR_FOLLOWS_TERRAIN) is raised by the ground height there,
/// read as Actor_SetHorizontalPositionWithTerrain reads it.
void Actor_SetPositionWithTerrain(u32 actorId, FIXED x, FIXED y, FIXED z) {
  struct Actor *actor;
  FIXED groundHeight;
  s32 tileX;
  s32 tileZ;

  actor = Actor_Lookup(actorId);
  if (actor != NULL) {
    Actor_StartIdleFar(actor);
    SET_VELOCITY(actor, 0, 0, 0);
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetX = ACTOR_NO_TARGET;
    SET_ACTOR_POSITION(actor, x, y, z);
    if (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN) {
      s32 terrainId = actor->layer;
      tileX = x;
      if (tileX < 0) {
        tileX += FIXED_ONE - 1;
      }
      tileX = FIXEDtoINT(tileX);
      tileZ = z;
      if (tileZ < 0) {
        tileZ += FIXED_ONE - 1;
      }
      groundHeight = toFIXED(
          MapGrid_GetTerrainHeightFar(terrainId, tileX, FIXEDtoINT(tileZ)));
      actor->y.fixed = (actor->y.fixed - actor->groundHeight) + groundHeight;
      actor->groundHeight = groundHeight;
    }
  }
}

/// Switches an actor to an animation (an enum ActorAnimation: ANIM_STAND,
/// ANIM_WALK, ANIM_NOD, ANIM_SHAKE_HEAD and so on).
void Actor_SetAnimationById(u32 actorId, s32 animation) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL)
    Actor_SelectAnimationFar(actor, animation);
}

/// Sets the animation timer steps of each of an actor's sprites
/// (Sprite_SetAnimationSpeed).
void Actor_SetAnimationSpeedById(u32 actorId, s32 steps) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL)
    Actor_SetAnimationSpeedFar(actor, steps);
}

/// Waits, at most 90 frames, for an actor's animation to change. Only an actor
/// drawn as a sprite (ACTOR_SPRITE_ONE) is waited for.
void Actor_WaitForAnimationChange(u32 actorId) {
  struct Actor *actor;
  struct Sprite *sprite;
  // DEAD: saved lives in a word on the stack, which nothing here needs; the
  // ROM keeps its str and the ldr of each compare.
  volatile s32 saved;
  s32 elapsedFrames;

  actor = Actor_Lookup(actorId);
  if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    sprite = actor->sprite;
    saved = sprite->lastNo;
    for (elapsedFrames = 0; elapsedFrames < 90; elapsedFrames++) {
      wait(1);
      if (saved != sprite->lastNo) {
        break;
      }
    }
  }
}

/// Plays an animation and waits for it to end. Events reach it
/// (Actor_SetAnimationAndWait).
void Actor_SetAnimationAndWait(u32 actorId, s32 animation) {
  Actor_SetAnimationById(actorId, animation);
  Actor_WaitForAnimationChange(actorId);
}

/// An empty function, reached only through an unnamed veneer in the object
/// bank's far-call table.
void Actor_NoOp(void) {}

/// Throws the actor upward at speed pixels per
/// frame with a jump sound (a higher one above 5), then waits frames frames.
void Actor_Jump(u32 actorId, s32 speed, s32 frames) {
  struct Actor *actor;

  actor = Actor_Lookup(actorId);
  if (actor != NULL) {
    actor->motionFlags |= ACTOR_FALLS;
    actor->velocityY = toFIXED(speed);
    if (speed > 5) {
      Sound_PlayFar(SE_HIGH_JUMP);
    } else {
      Sound_PlayFar(SE_JUMP);
    }
    Event_Wait(frames);
  }
}

/// The most times a repeated motion repeats: gActor_VariantScripts holds the
/// motion that many times over, REPEATED_MOTION_SCRIPT_BYTES (32 words) each.
#define REPEATED_MOTION_MAX 3
#define REPEATED_MOTION_SCRIPT_BYTES (32 * sizeof(s32))

/// Makes an actor squash and spring
/// back count times (1 to REPEATED_MOTION_MAX), through gActor_VariantScripts.
void Actor_StartRepeatedMotion(u32 actorId, s32 count) {
  struct Actor *actor;

  actor = Actor_Lookup(actorId);
  if (actor != NULL && count > 0) {
    if (count > REPEATED_MOTION_MAX) {
      count = REPEATED_MOTION_MAX;
    }
    // Start count motions from the script's end.
    Actor_StartScriptFar(actor,
                         (const s32 *)(gActor_VariantScripts +
                                       (REPEATED_MOTION_MAX - count) *
                                           REPEATED_MOTION_SCRIPT_BYTES));
  }
}

/// As Actor_StartRepeatedMotion, then
/// waits for it to end.
void Actor_RunRepeatedMotion(u32 actorId, s32 count) {
  Actor_StartRepeatedMotion(actorId, count);
  Actor_WaitForScriptEndById(actorId);
}

/// The share of a dust puff's x and z velocity that drag takes each frame, as
/// one part in this many.
#define DUST_PUFF_DRAG_X 18
#define DUST_PUFF_DRAG_Z 16

/// Moves a dust puff of FieldEffect_SpawnDustPuff: it drifts with its x and z
/// velocity (kept in speed and acceleration), which drag slows each frame, and
/// rises 1/64 pixel a frame.
void FieldEffect_UpdateDustPuff(struct Actor *particle) {
  s32 velocityX;
  FIXED x;
  s32 velocityZ;
  FIXED z;
  FIXED y;

  velocityX = particle->speedLimit;
  x = particle->x.fixed + velocityX;
  particle->x.fixed = x;
  particle->targetX = x;
  velocityZ = particle->acceleration;
  z = particle->z.fixed + velocityZ;
  particle->z.fixed = z;
  particle->targetZ = z;
  y = particle->y.fixed + toFIXED(1) / 64;
  particle->y.fixed = y;
  particle->targetY = y;
  particle->speedLimit = velocityX - velocityX / DUST_PUFF_DRAG_X;
  particle->acceleration = velocityZ - velocityZ / DUST_PUFF_DRAG_Z;
}

/// Kicks up a dust puff (SPRITE_DUST_PUFF) where source stands: it flies off
/// toward the back at 0.5 to 1.4 pixels a frame and sideways at up to 1.4, with
/// one of two animations, in the colour mode color.
void FieldEffect_SpawnDustPuff(struct Actor *source, s32 color) {
  struct Actor *actor;
  struct Sprite *sprite;
  s32 speed;

  actor = Actor_CreateFar(SPRITE_DUST_PUFF, source->x.fixed, source->y.fixed,
                          source->z.fixed);
  if (actor != 0) {
    sprite = actor->sprite;
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

    if (color != SPRITE_COLOR_NORMAL)
      Actor_SetSpriteColor(actor, color);

    // speed and acceleration hold the puff's x and z velocity, in steps of
    // toFIXED(1) / 10 (0.1 pixel): z is -0.5 to -1.4, x -1.4 to 1.4 in steps of
    // 0.2.
    actor->motionFlags = 0;
    speed = Random16() % 10 + 5;
    actor->acceleration = -(toFIXED(1) / 10) * speed;
    speed = Random16() % 15 - 7;
    speed <<= 1;
    actor->speedLimit = toFIXED(1) / 10 * speed;
    actor->timer = 0;
    actor->update = FieldEffect_UpdateDustPuff;
    sprite->flags = 0;
    sprite->image.priority = source->sprite->image.priority;
  }
}

/// The frames a ledge jump sinks for, 2 pixels a frame, and the most it waits
/// to land.
#define LEDGE_JUMP_SINK_FRAMES 14
#define LEDGE_JUMP_LANDING_FRAMES 180

/// Makes an actor jump down off a ledge in front of it (the field runs it for
/// the leader when the player presses down at a ledge): it shows pose for 10
/// frames, hops 12 pixels south, turns toward FACING_SOUTH_SOUTHWEST (1/16 turn
/// past FACING_SOUTH) while sinking 28 pixels with a dust puff every other
/// frame in the colour mode dustColor (none when it is -1), then jumps on to 16
/// pixels south of where it started and lands.
void Actor_JumpDownLedge(s32 actorId, u32 pose, s32 dustColor) {
  FIXED nextY;
  FIXED startZ;
  struct Actor *actor;

  actor = Actor_GetById(actorId);
  startZ = actor->z.fixed;
  if (actor != 0) {
    Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
    Actor_SelectAnimationFar(actor, pose);
    wait(10);

    // Hop up and forward 12 pixels.
    Actor_SelectAnimationFar(actor, ANIM_STAND);
    actor->motionFlags |= ACTOR_FALLS;
    actor->velocityY = toFIXED(4);
    Actor_SetMoveTargetFar(actor, actor->x.fixed, actor->y.fixed,
                           startZ + toFIXED(12));
    wait(6);

    // Turn and sink, raising dust, in free flight: no motion flags, so it
    // neither falls nor follows the ground.
    Sound_PlayFar(SE_FLOOR_SWITCH);
    // From here pose counts frames.
    Actor_FaceDirectionById(actorId, FACING_SOUTH_SOUTHWEST, 0);
    actor->motionFlags = 0;
    for (pose = 0; pose < LEDGE_JUMP_SINK_FRAMES; pose++) {
      nextY = actor->y.fixed - toFIXED(2);
      actor->y.fixed = nextY;
      actor->targetY = nextY;
      wait(1);
      if ((dustColor != -1) && (pose & 1)) {
        FieldEffect_SpawnDustPuff(actor, dustColor);
      }
    }

    // Jump 16 pixels forward and wait to land, following the ground and
    // falling again.
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->velocityY = toFIXED(3);
    Actor_SetMoveTargetFar(actor, actor->x.fixed, actor->y.fixed,
                           startZ + toFIXED(16));
    Actor_WaitForMoveFar(actor);
    for (pose = 0; pose < LEDGE_JUMP_LANDING_FRAMES &&
                   actor->y.fixed > actor->groundHeight;
         pose++) {
      wait(1);
    }
    wait(2);
    Scene_PlayQueuedMusic();
  }
}

/// Turns the first actor at once to face
/// the second, then waits the given number of frames.
void Actor_FaceActorById(s32 firstId, s32 secondId, s32 frames) {
  struct Actor *first;
  struct Actor *second;

  first = Actor_Lookup(firstId);
  second = Actor_Lookup(secondId);
  if (first != 0 && second != 0) {
    first->facing = Trig_Atan(second->z.fixed - first->z.fixed,
                              second->x.fixed - first->x.fixed);
    Event_Wait(frames);
  }
}

/// Turns two actors toward each
/// other, then waits the given number of frames.
void Actor_TurnToFaceEachOther(s32 firstId, s32 secondId, s32 frames) {
  void *first = Actor_Lookup(firstId);
  void *second = Actor_Lookup(secondId);

  if (first != NULL && second != NULL) {
    Actor_TurnPairToFaceEachOther(first, second);
    Event_Wait(frames);
  }
}

/// Turns two actors to face each other, at most 1/16 turn a frame each,
/// until both do (at most 60 frames). Only Actor_TurnToFaceEachOther calls it.
void Actor_TurnPairToFaceEachOther(struct Actor *first, struct Actor *second) {
  s32 firstTargetAngle;
  s32 secondTargetAngle;
  s32 frame;
  s32 actorsTurning;
  s32 angleDifference;

  if (first == NULL || second == NULL)
    return;
  firstTargetAngle = (u16)Trig_Atan(second->z.fixed - first->z.fixed,
                                    second->x.fixed - first->x.fixed);
  // The second actor faces the opposite way, half a turn round.
  secondTargetAngle = firstTargetAngle + DEGtoANG(180);
  for (frame = 0; frame < 60; frame++) {
    // Each actor that already faces its way takes one off the count;
    // both facing ends the turn early.
    actorsTurning = 2;
    angleDifference = (s16)(firstTargetAngle - first->facing);
    if (angleDifference != 0) {
      if (angleDifference > FACING_STEP)
        angleDifference = FACING_STEP;
      if (angleDifference < -FACING_STEP)
        angleDifference = -FACING_STEP;
      first->facing += angleDifference;
    } else {
      actorsTurning = 1;
    }
    angleDifference = (s16)(secondTargetAngle - second->facing);
    if (angleDifference != 0) {
      if (angleDifference > FACING_STEP)
        angleDifference = FACING_STEP;
      if (angleDifference < -FACING_STEP)
        angleDifference = -FACING_STEP;
      second->facing += angleDifference;
    } else {
      actorsTurning--;
    }
    if (actorsTurning == 0)
      break;
    wait(1);
  }
}

/// Releases the actor and empties its slot,
/// writing NULL to its place in the field work's actor table.
void Actor_ReleaseById(s32 actorId) {
  void *released = Actor_Lookup(actorId);
  struct FieldWork *table = gFieldWork;
#ifdef UBFIX
  struct Actor *empty = NULL;
#endif

  if (released != 0) {
    Actor_ReleaseFar(released);
#ifdef UBFIX
    // The getter accepts slots through 191, including non-actor storage.
    memcpy((u8 *)table + offsetof(struct FieldWork, actors) +
               // Copy the pointer representation without extending the actor
               // array.
               actorId * sizeof(void *),
           &empty, sizeof(empty));
#else
    // UB: the getter accepts slots through 191, past the 66 actors.
    table->actors[actorId] = NULL;
#endif
  }
}

UNUSED void Actor_ReleaseNoOp(void) {}

/// Gives every sprite part of an actor the colour mode color
/// (Actor_SetSpriteColor), or with SPRITE_COLOR_BLINK makes it blink.
void Actor_ConfigureSpriteColor(s32 actorId, s32 color) {
  s32 blink;
  struct Actor *actor;

  actor = Actor_Lookup(actorId);
  if (actor != NULL) {
    blink = SPRITE_COLOR_BLINK & color;
    if (blink != 0) {
      actor->update = Actor_UpdateBlink;
      return;
    }
    actor->update = NULL;
    Actor_SetSpriteColor(actor, color);
  }
}

/// Per-frame update routine of a blinking actor: sets its sprite parts' colour
/// mode from gActor_BlinkSpriteColors (SPRITE_COLOR_WHITE for two frames out of
/// eight, otherwise SPRITE_COLOR_NORMAL).
void Actor_UpdateBlink(struct Actor *actor) {
  if ((actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
    u8 color;
    struct Sprite *container;
    u8 partCount;

    // The table's four entries each last two frames.
    color = gActor_BlinkSpriteColors[(gFrameCount >> 1) & 3];
    container = actor->sprite;
    partCount = container->count;
    if (partCount != 0) {
      struct SpriteAnimation **entries = container->entries;
      s32 remaining = partCount;
      do {
        struct SpriteAnimation *entry = *entries++;
        if (entry != 0 && entry->script != 0) {
          entry->colorMode = color;
        }
        remaining--;
      } while (remaining != 0);
    }
    container->dirty = 1;
  }
}

/// Sets the colour mode of every animated part of an actor's sprite. Events set
/// SPRITE_COLOR_NORMAL or SPRITE_COLOR_HIDDEN, and alternate the two to make an
/// actor flash.
void Actor_SetSpriteColor(struct Actor *actor, s32 color) {
  if ((actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
    struct Sprite *container = actor->sprite;
    u8 partCount = container->count;

    if (partCount != 0) {
      struct SpriteAnimation **entry = container->entries;
      u32 count = partCount;
      do {
        struct SpriteAnimation *part = *entry++;
        if (part != NULL && part->script != 0) {
          part->colorMode = color;
        }
        count--;
      } while (count != 0);
    }
    container->dirty = 1;
  }
}

/// The actor in Actor_WatchAndStartScript's targetId, below its enum
/// ActorWatchFlag bits.
#define ACTOR_WATCH_ACTOR_MASK 0xff
/// The distance, in pixels, a follower stays behind the actor it watches
/// (Actor_MoveTowardTarget reads it from the follower's timer).
#define FOLLOW_DISTANCE 40

/// Makes an actor follow the actor in targetId's ACTOR_WATCH_ACTOR_MASK bits
/// and starts script on it. Unless targetId has ACTOR_WATCH_KEEP_MOTION it
/// keeps FOLLOW_DISTANCE, takes the target's top speed with twice its
/// acceleration, to keep up with it, and no collision flags, so it cannot block
/// the target.
void Actor_WatchAndStartScript(u32 actorId, s32 targetId, const void *script) {
  struct Actor *follower = Actor_Lookup(actorId);
  struct Actor *target = Actor_Lookup(targetId & ACTOR_WATCH_ACTOR_MASK);

  if (follower != NULL && target != NULL) {
    follower->watchedActor = target;
    if (!(targetId & ACTOR_WATCH_KEEP_MOTION)) {
      follower->timer = FOLLOW_DISTANCE;
      follower->acceleration = target->acceleration * 2;
      follower->speedLimit = target->speedLimit;
      follower->collisionFlags = 0;
    }
    Actor_StartScriptFar(follower, script);
  }
}

/// SC_CALL routine that turns an actor toward the angle kept in its timer: the
/// whole way when within 1/16 turn, otherwise 1/32 turn.
s32 Actor_StepAngle(struct Actor *actor) {
  s32 delta = 0;

  if (actor != NULL) {
    s32 targetAngle = (u16)actor->timer;
    s32 currentAngle = actor->facing;
    delta = (s16)(targetAngle - currentAngle);
    if (delta != 0) {
      if (delta > TRIG_TURN / 16)
        delta = TRIG_TURN / 32;
      if (delta < -(TRIG_TURN / 16))
        delta = -(TRIG_TURN / 32);
      actor->facing = currentAngle + delta;
    }
  }
  return delta;
}

/// Clears an actor's destination, puts it under script control and
/// stands it still.
void Actor_ClearDestinationAndStand(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    Actor_StartIdleFar(actor);
    Actor_SelectAnimationFar(actor, ANIM_STAND);
  }
}

/// Turns an actor toward angle
/// (TRIG_TURN a full turn; Actor_StepAngle), then the event waits the given
/// number of frames.
void Actor_FaceDirectionById(s32 actorId, s32 angle, s32 frames) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->timer = angle;
    Actor_StartScriptFar(actor, (const s32 *)gActor_StepAngleScript);
    Event_Wait(frames);
  }
}

/// Fixes the priority of the
/// actor's sprite, which then no longer follows its depth: clears
/// ACTOR_PRIORITY_AUTOMATIC.
void Actor_SetSpritePriority(s32 actorId, s32 priority) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL &&
      (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
    struct Sprite *sprite = actor->sprite;

    sprite->image.priority = priority;
    sprite->shadow.priority = priority;
    actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// Makes the target actor's sprite show the source actor's graphics (its VRAM
/// block and tile). Neither actor is checked for NULL.
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

/// Sets the number of the event's next message.
void Event_SetMessageId(s32 message) { gFieldWork->message = message; }

/// Returns the sprite (the first animation's id) of the actor in the low 12
/// bits of actorId, or -1 if there is no such animated actor.
s32 Actor_GetSpriteById(s32 actorId) {
#ifdef UBFIX
  struct FieldWork *state = gFieldWork;
  s32 sprite = -1;
  struct Actor *entry;

  actorId &= SPEAKER_ACTOR_MASK;
  // BUG: the -1 sentinel becomes index 4095, beyond the field work. All
  // indices from ACTOR_ID_COUNT to 4095 lie outside the actor table.
  if (actorId >= ACTOR_ID_COUNT)
    return -1;
  entry = state->actors[actorId];
#else
  struct FieldWork *state = gFieldWork;
  s32 sprite = -1;
  struct Actor *entry = state->actors[actorId & SPEAKER_ACTOR_MASK];
#endif

  if (entry != 0 && entry->spriteStorageKind == ACTOR_SPRITE_ONE)
    sprite = entry->sprite->entries[0]->animationId;
  return sprite;
}

/// Returns the first of the scene's actors (ACTOR_FIRST_PLACED to
/// ACTOR_LAST_ID) showing sprite, or -1.
s32 Actor_FindBySprite(s32 sprite) {
  struct FieldWork *state = gFieldWork;
  s32 result = -1;
  s32 index;
  struct Actor *actor;

  for (index = ACTOR_FIRST_PLACED; index <= ACTOR_LAST_ID; index++) {
    actor = state->actors[index];
    if (actor != 0 && actor->spriteStorageKind == ACTOR_SPRITE_ONE &&
        actor->sprite->entries[0]->animationId == sprite) {
      result = index;
      break;
    }
  }
  return result;
}

#undef DUST_PUFF_DRAG_X
#undef DUST_PUFF_DRAG_Z
#undef ACTOR_WATCH_ACTOR_MASK
#undef FOLLOW_DISTANCE
#undef REPEATED_MOTION_MAX
#undef REPEATED_MOTION_SCRIPT_BYTES
#undef LEDGE_JUMP_SINK_FRAMES
#undef LEDGE_JUMP_LANDING_FRAMES
