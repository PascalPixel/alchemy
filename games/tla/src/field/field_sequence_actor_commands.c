/// @file
/// Event commands on actors: placing, moving and animating them.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "system/frame.h"
#include "field/event_commands.h"
#include "field/object.h"
#include "system/field_effect.h"

/// Returns an actor, or NULL if it does not exist.
struct Actor *Actor_GetById(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor == NULL)
    return NULL;
  return actor;
}

/// The event command "set actor speed": its top speed and acceleration, 16.16
/// pixels per frame. Writes the actor's speedLimit and acceleration.
void Actor_SetSpeedParameters(s32 actorId, FIXED speedLimit,
                              FIXED acceleration) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->acceleration = acceleration;
    actor->speedLimit = speedLimit;
  }
}

/// Gives an actor a movement type or actor script (Actor_SetMovementType)
/// and makes it face where it moves (ACTOR_FACES_MOTION).
void Actor_SetMovementTypeById(s32 actorId, const void *script) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_SetMovementType(actor, (s32)script);
  }
}

/// The event command "stop actor": stops its movement type and puts it back
/// under script control, on the idle script (a lone SC_END), and makes it face
/// where it moves.
void Actor_Stop(s32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);
  if (actor != NULL) {
    actor->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_StartIdleFar(actor);
  }
}

/// As Actor_Stop, and also clears the actor's target and velocity, so it
/// stands where it is.
void Actor_ResetTargetsAndVelocity(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_StartIdleFar(actor);
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    SET_VELOCITY(actor, 0, 0, 0);
    actor->arrivalAxis = 0;
  }
}

/// Makes an actor follow another (the watched actor script): writes its
/// watchedActor.
void Actor_WatchAndSetScript(s32 actorId, s32 watchedActorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->watchedActor = (union FieldObject *)Actor_GetById(watchedActorId);
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
void Actor_SetMovementTypeAndWaitById(s32 actorId, const void *script) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_SetMovementType(actor, (s32)script);
    Actor_WaitForScriptEndFar(actor);
  }
}

/// The event command "set actor destination": the actor heads for (x, z), in
/// pixels, keeping its current animation.
void Actor_SetDestinationById(u32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SetMoveTargetFar(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
  }
}

/// The event command "move actor and wait": as Actor_SetDestinationById, then
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

/// The event command "walk actor to": the actor walks toward (x, z), in pixels.
void Actor_WalkToById(s32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SelectAnimationFar(actor, ANIM_WALK);
    Actor_SetMoveTargetFar(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
  }
}

/// The event command "walk actor to and wait": walks the actor to (x, z), in
/// pixels, waits for it to arrive and stands it still.
void Actor_WalkToAndWaitById(s32 actorId, s32 x, s32 z) {
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

/// The event command "center actor and walk": walks the actor sideways to the
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
    columnX = pixelX / 16 * 16;
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

/// The event command "set actor destination offset": the actor heads (xOffset,
/// zOffset) pixels from where it stands.
void Actor_SetDestinationOffsetById(s32 actorId, s32 xOffset, s32 zOffset) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartIdleFar(actor);
    Actor_SetMoveTargetFar(actor, actor->x.fixed + toFIXED(xOffset),
                           actor->y.fixed, actor->z.fixed + toFIXED(zOffset));
  }
}

/// Walks the actor (xOffset, zOffset) pixels from where it stands, through the
/// offset script.
void Actor_WalkByById(u32 actorId, s32 xOffset, s32 zOffset) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    actor->paused = 0;
    Actor_StartScriptFar(actor, (u32)gActor_OffsetScript);
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

/// Places an actor on the leader, a pixel nearer the back, and sends it walking
/// (xOffset, zOffset) pixels away at speed 0.8 and acceleration 0.4, through
/// the launch script; the actor then takes movement type action.
void Actor_LaunchFromLeader(s32 actorId, s32 xOffset, s32 zOffset, s32 action) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    Actor_SetSpeedParameters(actorId, toFIXED(4) / 5, toFIXED(2) / 5);
    Actor_PlaceOnActor(actorId, gMainRoutineState.leaderActorId);
    Actor_SetPositionAndResetMotionFar(actor, actor->x.fixed, actor->y.fixed,
                                       actor->z.fixed - toFIXED(1));
    actor->paused = 0;
    Actor_SelectAnimationFar(actor, ANIM_WALK);
    Actor_SetDestinationOffsetById(actorId, xOffset, zOffset);
    Actor_StartScriptFar(actor, gActor_LaunchScript);
    actor->timer = action;
  }
}

/// The event command "wait for actor": waits (at most 600 frames) for the actor
/// to arrive and stands it still.
void Actor_WaitForMoveById(s32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    Actor_WaitForMoveFar(actor);
    Actor_SelectAnimationFar(actor, ANIM_STAND);
  }
}

/// The event command "set actor position": puts the actor at (x, z), 16.16
/// pixels, at rest; an actor that follows the ground keeps its height above
/// it.
void Actor_SetHorizontalPositionWithTerrain(u32 actorId, FIXED x, FIXED z) {
  struct Actor *actor = Actor_Lookup(actorId);
  s32 terrainHeight;

  if (actor != NULL) {
    Actor_StartIdleFar(actor);
    Actor_SetPositionAndResetMotionFar(actor, x, actor->y.fixed, z);
    // Swap the old ground's height for the new one, so the actor's height above
    // the ground is kept.
    if (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN) {
      terrainHeight = MapGrid_GetTerrainHeightFar(actor->layer, actor->x.fixed,
                                                  actor->z.fixed);
      actor->y.fixed = actor->y.fixed - actor->groundHeight + terrainHeight;
      actor->groundHeight = terrainHeight;
    }
  }
}

// clang-format off
/// As Actor_SetHorizontalPositionWithTerrain, and turns the actor to angle.
/// Written in the old style: the angle arrives as a word and is taken as a
/// halfword.
void Actor_SetHorizontalPositionAndAngleWithTerrain(actorId, x, z, angle)
  s32 actorId;
  s32 x;
  s32 z;
  s16 angle;
{
  // clang-format on
  struct Actor *actor = Actor_Lookup(actorId);
  s32 terrainHeight;

  if (actor != NULL) {
    Actor_StartIdleFar(actor);
    Actor_SetPositionAndResetMotionFar(actor, x, actor->y.fixed, z);
    actor->facing = angle;
    if (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN) {
      terrainHeight = MapGrid_GetTerrainHeightFar(actor->layer, actor->x.fixed,
                                                  actor->z.fixed);
      actor->y.fixed = actor->y.fixed - actor->groundHeight + terrainHeight;
      actor->groundHeight = terrainHeight;
    }
  }
}

/// Puts the actor at (x, y, z), 16.16 pixels, at rest; an actor that follows
/// the ground is raised by the ground height there, as
/// Actor_SetHorizontalPositionWithTerrain raises it.
void Actor_SetPositionWithTerrain(u32 actorId, FIXED x, FIXED y, FIXED z) {
  struct Actor *actor = Actor_Lookup(actorId);
  s32 terrainHeight;

  if (actor != NULL) {
    Actor_StartIdleFar(actor);
    Actor_SetPositionAndResetMotionFar(actor, x, y, z);
    if (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN) {
      terrainHeight = MapGrid_GetTerrainHeightFar(actor->layer, actor->x.fixed,
                                                  actor->z.fixed);
      actor->y.fixed = actor->y.fixed - actor->groundHeight + terrainHeight;
      actor->groundHeight = terrainHeight;
    }
  }
}

/// Puts the first actor where the second stands, at rest, on the same ground
/// height.
void Actor_PlaceOnActor(u32 actorId, u32 otherId) {
  struct Actor *actor = Actor_Lookup(actorId);
  struct Actor *other = Actor_Lookup(otherId);

  if (actor != NULL && other != NULL) {
    Actor_SetPositionAndResetMotionFar(actor, other->x.fixed, other->y.fixed,
                                       other->z.fixed);
    actor->groundHeight = other->groundHeight;
  }
}

/// The event command "set actor animation" (an enum ActorAnimation: ANIM_STAND,
/// ANIM_WALK, ANIM_NOD, ANIM_SHAKE_HEAD and so on).
void Actor_SetAnimationById(u32 actorId, s32 animation) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL)
    Actor_SelectAnimationFar(actor, animation);
}

/// Sets the animation timer steps of each of an actor's sprites.
void Actor_SetAnimationSpeedById(s32 actorId, s32 steps) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL)
    Actor_SetAnimationSpeedFar(actor, steps);
}

/// Waits, at most 90 frames, for an actor's animation to change or for its
/// first sprite animation's script to reach its hold command (ANIMCMD_HOLD).
/// Only an actor drawn as a sprite (ACTOR_SPRITE_ONE) is waited for.
void Actor_WaitForAnimationChange(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);
  struct Sprite *sprite;
  // DEAD: saved lives in a word on the stack, which nothing here needs; the
  // ROM keeps its str and the ldr of each compare.
  volatile u32 saved;
  s32 elapsedFrames;

  if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    sprite = actor->sprite;
    saved = sprite->lastNo;
    for (elapsedFrames = 0; elapsedFrames < 90; elapsedFrames++) {
      wait(1);
      if (saved != sprite->lastNo)
        break;
      if (sprite->entries[0] != NULL && sprite->entries[0]->script != NULL &&
          sprite->entries[0]->script[sprite->entries[0]->scriptOffset] ==
              ANIMCMD_HOLD)
        break;
    }
  }
}

/// The event command "set actor animation and wait": plays the animation and
/// waits for it to end.
void Actor_SetAnimationAndWait(s32 actorId, s32 animation) {
  Actor_SetAnimationById(actorId, animation);
  Actor_WaitForAnimationChange(actorId);
}

/// The event command "actor jump": throws the actor upward at speed pixels per
/// frame with a jump sound (a higher one above 5), then waits frames frames.
void Actor_Jump(s32 actorId, s32 speed, s32 frames) {
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

/// The most squash-and-spring motions gActor_VariantScripts holds: it plays the
/// motion that many times over, REPEATED_MOTION_SCRIPT_BYTES (32 words) each.
#define REPEATED_MOTION_MAX 3
#define REPEATED_MOTION_SCRIPT_BYTES (32 * sizeof(s32))

/// The event command "start repeated motion": the actor squashes and springs
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

/// The event command "run repeated motion": as Actor_StartRepeatedMotion, then
/// waits for it to end.
void Actor_RunRepeatedMotion(u32 actorId, s32 count) {
  Actor_StartRepeatedMotion(actorId, count);
  Actor_WaitForScriptEndById(actorId);
}

/// Moves a dust puff of FieldEffect_SpawnDustPuff: it drifts with its x and z
/// velocity (kept in speed and acceleration), which drag slows each frame, and
/// rises 1/64 pixel a frame.
void FieldEffect_UpdateDustPuff(struct Actor *particle) {
  FIXED velocityX = particle->speedLimit;
  FIXED velocityZ = particle->acceleration;

  particle->x.fixed += velocityX;
  particle->z.fixed += velocityZ;
  particle->y.fixed += toFIXED(1) / 64;
  particle->speedLimit = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
  particle->acceleration = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;
}

/// Kicks up a dust puff (SPRITE_DUST_CLOUD) where source stands: it flies off
/// toward the back at 0.5 to 1.4 pixels a frame and sideways at up to 1.4, with
/// one of two animations, drawn at source's priority, in colorMode unless 0.
void FieldEffect_SpawnDustPuff(struct Actor *source, s32 colorMode) {
  struct Actor *actor;
  struct Sprite *child;
  s32 speed;
  FIXED backStep = -(toFIXED(1) / 10); // 0.1 pixel a frame toward the back

  actor = Actor_CreateFar(SPRITE_DUST_CLOUD, source->x.fixed, source->y.fixed,
                          source->z.fixed);
  if (actor != NULL) {
    child = actor->sprite;
    switch (Random16() & 1) {
    case 1:
      Actor_SelectAnimationFar(actor, 2);
      Actor_StartScriptFar(actor, gDustPuff_LongScript);
      break;
    default:
      Actor_SelectAnimationFar(actor, 1);
      Actor_StartScriptFar(actor, gDustPuff_ShortScript);
      break;
    }

    if (colorMode != 0)
      Actor_SetSpriteColor(actor, colorMode);

    // speed and acceleration hold the puff's x and z velocity, in steps of
    // toFIXED(1) / 10 (0.1 pixel): z is -0.5 to -1.4, x -1.4 to 1.4 in steps of
    // 0.2.
    actor->motionFlags = 0;
    speed = Random16() % 10 + 5;
    actor->acceleration = backStep * speed;
    speed = Random16() % 15 - 7;
    speed <<= 1;
    actor->speedLimit = toFIXED(1) / 10 * speed;
    actor->timer = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->frozen = 1;
    actor->update = (ObjectUpdate)FieldEffect_UpdateDustPuff;
    child->flags = 0;
    child->image.priority = ((struct Sprite *)source->sprite)->image.priority;
  }
}

/// Makes an actor jump down off a ledge in front of it: it shows pose for 10
/// frames, hops 12 pixels south from the middle of its 16-pixel column, turns
/// toward FACING_SOUTH_SOUTHWEST while sinking 28 pixels with a dust puff every
/// other frame in the colour mode dust (none when dust is -1), then jumps on to
/// 16 pixels south of where it started and lands, waiting at most 180 frames
/// for it unless dust has LEDGE_JUMP_NO_LANDING_WAIT.
void Actor_JumpDownLedge(s32 actorId, s32 pose, s32 dust) {
  struct Actor *actor = Actor_GetById(actorId);
  FIXED startZ = actor->z.fixed;
  FIXED x = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  s32 spriteFlags;
  u32 frame;

  if (actor == NULL)
    return;
  spriteFlags = ((struct Sprite *)actor->sprite)->flags;
  Actor_SelectAnimationFar(actor, pose);
  wait(10);

  // Hop up and forward 12 pixels.
  Actor_SelectAnimationFar(actor, ANIM_STAND);
  Actor_SetSpriteFlagsFar(actor, 0);
  actor->motionFlags |= ACTOR_FALLS;
  actor->velocityY = toFIXED(4);
  Actor_SetMoveTargetFar(actor, x, actor->y.fixed, startZ + toFIXED(12));
  wait(6);

  // Turn and sink 2 pixels a frame for 14 frames, raising dust, in free
  // flight: no motion flags, so it neither falls nor follows the ground.
  Sound_PlayFar(SE_FLOOR_SWITCH);
  Actor_FaceDirectionById(actorId, FACING_SOUTH_SOUTHWEST, 0);
  actor->motionFlags = 0;
  for (frame = 0; frame < 14; frame++) {
    actor->y.fixed -= toFIXED(2);
    actor->targetY = actor->y.fixed;
    wait(1);
    if (dust != -1 && (frame & 1))
      FieldEffect_SpawnDustPuff(actor, dust);
  }

  // Jump 16 pixels forward, following the ground and falling again.
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  actor->velocityY = toFIXED(3);
  Actor_SetMoveTargetFar(actor, x, actor->y.fixed, startZ + toFIXED(16));
  Actor_WaitForMoveFar(actor);
  if (dust & LEDGE_JUMP_NO_LANDING_WAIT) {
    wait(4);
    return;
  }
  frame = 0;
  if (actor->y.fixed > actor->groundHeight) {
    do {
      wait(1);
      if (++frame >= 180)
        break;
    } while (actor->y.fixed > actor->groundHeight);
  }
  Actor_SetSpriteFlagsFar(actor, spriteFlags);
  wait(2);
  Event_NoOp();
}

/// The event command "actor face actor": turns the first actor at once to face
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

/// The event command "actors face each other": turns two actors toward each
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
/// until both do (at most 60 frames).
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

/// The event command "release actor": releases the actor and empties its slot
/// in the field state's actor table.
void Actor_ReleaseById(s32 actorId) {
  void *released = Actor_Lookup(actorId);
  struct FieldState *state = Ram_HeapSlots->fieldState;

  if (released != NULL) {
    Actor_ReleaseFar(released);
    state->actors[actorId] = NULL;
  }
}

/// The event command "set actor sprite colour": gives every sprite animation of
/// the actor the colour mode (Actor_SetSpriteColor); with SPRITE_COLOR_BLINK
/// the actor blinks instead (Actor_UpdateBlink).
void Actor_ConfigureSpriteColor(s32 actorId, s32 colorMode) {
  struct Actor *actor;

  actor = Actor_Lookup(actorId);
  if (actor != NULL) {
    if (colorMode & SPRITE_COLOR_BLINK) {
      actor->update = Actor_UpdateBlink;
      return;
    }
    actor->update = NULL;
    Actor_SetSpriteColor(actor, colorMode);
  }
}

/// Per-frame update routine of a blinking actor: sets its sprite animations'
/// colour mode from gActor_BlinkSpriteColors, each entry lasting two frames.
void Actor_UpdateBlink(struct Actor *actor) {
  struct Sprite *sprite;
  struct SpriteAnimation *animation;
  s32 colorMode;
  s32 i;

  if ((actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
    sprite = actor->sprite;
    colorMode = gActor_BlinkSpriteColors[(gFrameCount >> 1) & 3];
    for (i = 0; i < sprite->count; i++) {
      animation = sprite->entries[i];
      if (animation != 0 && animation->script != 0) {
        animation->colorMode = colorMode;
      }
    }
    sprite->dirty = 1;
  }
}

/// Sets the colour mode (SpriteColorMode) of every running sprite animation of
/// an actor's sprite. Events set SPRITE_COLOR_NORMAL or SPRITE_COLOR_HIDDEN,
/// and alternate the two to make an actor flash.
void Actor_SetSpriteColor(struct Actor *actor, s32 colorMode) {
  struct Sprite *sprite;
  struct SpriteAnimation *animation;
  s32 i;

  if ((actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
    sprite = actor->sprite;
    for (i = 0; i < sprite->count; i++) {
      animation = sprite->entries[i];
      if (animation != 0 && animation->script != 0) {
        animation->colorMode = colorMode;
      }
    }
    sprite->dirty = 1;
  }
}

/// Actor_WatchAndStartScript's targetId: the actor to watch, and a flag to keep
/// the watcher's own motion.
enum ActorWatchFlag { ACTOR_WATCH_KEEP_MOTION = 0x10000 };
#define ACTOR_WATCH_ACTOR_MASK 0xff ///< the actor's bits
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
    follower->watchedActor = (union FieldObject *)target;
    if (!(targetId & ACTOR_WATCH_KEEP_MOTION)) {
      follower->timer = FOLLOW_DISTANCE;
      follower->acceleration = target->acceleration * 2;
      follower->speedLimit = target->speedLimit;
      follower->collisionFlags = 0;
    }
    Actor_StartScriptFar(follower, script);
  }
}
