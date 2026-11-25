/// @file
/// Actor scripts: moving, turning and the built-in movement types.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// A following actor (MOVEMENT_TYPE_FOLLOW_WATCHED) stays its timer's
/// distance in pixels from the watched actor: from as far as that it walks
/// toward a point this many pixels along the way for each timer's distance it
/// is off, and nearer it stands.
#define FOLLOW_STRIDE 16
/// Actor_FindNearestFacingTarget's reach in pixels, across and in height, and
/// the distance beyond which an actor must be within FACING_TARGET_ANGLE of
/// where it faces.
#define FACING_TARGET_REACH 40
#define FACING_TARGET_HEIGHT 48
#define FACING_TARGET_NEAR 23
#define FACING_TARGET_ANGLE (TRIG_TURN * 3 / 16)

/// The SC_CALL routine of MOVEMENT_TYPE_RESET_ACTION: clears an actor's sprite
/// flags, so it has no shadow, and its collision flags, so nothing collides
/// with it.
s32 Actor_ClearSpriteAndCollisionFlags(struct Actor *actor) {
  Actor_SetSpriteFlagsFar(actor, 0);
  actor->collisionFlags = 0;
  return 0;
}

/// The SC_CALL routine of MOVEMENT_TYPE_FOLLOW_WATCHED: follows the watched
/// actor.
s32 Actor_MoveTowardTarget(struct Actor *actor) {
  s32 step;
  struct Actor *target;
  FIXED deltaX;
  FIXED deltaZ;
  s32 pixelsX;
  s32 pixelsZ;
  FIXED newX;
  FIXED newZ;
  s32 distance;

  target = (struct Actor *)actor->watchedActor;
  if (target != NULL) {
    // Whole pixels, rounded toward zero.
    deltaX = target->x.fixed - actor->x.fixed;
    pixelsX = deltaX / FIXED_ONE;
    deltaZ = target->z.fixed - actor->z.fixed;
    pixelsZ = deltaZ / FIXED_ONE;
    distance = Iwram_Sqrt(pixelsX * pixelsX + pixelsZ * pixelsZ);
    step = actor->timer;
    if (distance >= step) {
      newX =
          actor->x.fixed + (s32)((u32)pixelsX * toFIXED(FOLLOW_STRIDE)) / step;
      newZ =
          actor->z.fixed + (s32)((u32)pixelsZ * toFIXED(FOLLOW_STRIDE)) / step;
      Actor_SetMoveTargetFar(actor, newX, actor->y.fixed, newZ);
      Actor_SelectAnimationFar(actor, ANIM_WALK);
    } else {
      Actor_SelectAnimationFar(actor, ANIM_STAND);
    }
  }
  return 1;
}

/// The SC_CALL routine of MOVEMENT_TYPE_FACE_LEADER: turns the actor toward
/// its watched actor, at most FACING_STEP a frame.
s32 Actor_TurnTowardWatchedActor(struct Actor *actor) {
  s16 turnStep;
  u16 currentAngle;
  s32 targetAngle;
  struct Actor *target;

  target = (struct Actor *)actor->watchedActor;
  if (target != NULL) {
    // Clear ACTOR_FACES_MOTION, so moving does not turn it away again.
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
    targetAngle = (u16)Trig_Atan(target->z.fixed - actor->z.fixed,
                                 target->x.fixed - actor->x.fixed);
    currentAngle = actor->facing;
    turnStep = targetAngle - currentAngle;
    if (turnStep != 0) {
      if (turnStep > FACING_STEP)
        turnStep = FACING_STEP;
      if (turnStep < -FACING_STEP)
        turnStep = -FACING_STEP;
      actor->facing = (currentAngle + turnStep);
    }
  }
  return 1;
}

/// Starts an actor's movement type: an enum MovementType, or any other value as
/// the actor script to run itself.
void Actor_SetMovementType(struct Actor *actor, s32 movement) {
  switch (movement) {
  case MOVEMENT_TYPE_STAND:
    movement = (s32)gMovementType_StandScript;
    break;
  case MOVEMENT_TYPE_WANDER:
    movement = (s32)gMovementType_WanderScript;
    break;
  case MOVEMENT_TYPE_WANDER_NEAR:
    movement = (s32)gMovementType_WanderNearScript;
    break;
  case MOVEMENT_TYPE_WANDER_CLOSE:
    movement = (s32)gMovementType_WanderCloseScript;
    break;
  case MOVEMENT_TYPE_FOLLOW_WATCHED:
    movement = (s32)gMovementType_FollowWatchedScript;
    break;
  case MOVEMENT_TYPE_FACE_LEADER:
    actor->watchedActor =
        (union ActorOrEffect *)Actor_GetById(gMainRoutineState.leaderActorId);
    movement = (s32)gMovementType_FaceLeaderScript;
    break;
  case MOVEMENT_TYPE_RESET_ACTION:
    movement = (s32)gMovementType_ResetActionScript;
    break;
  default:
    break;
  }
  Actor_StartScriptFar(actor, (const s32 *)movement);
}

/// Finds the nearest animated actor to self within FACING_TARGET_REACH pixels
/// and under FACING_TARGET_HEIGHT of height, ignoring actors more than
/// FACING_TARGET_ANGLE to either side once they are over FACING_TARGET_NEAR
/// pixels away; returns it if its sprite is `sprite` (an enum SpriteId), else
/// NULL.
struct Actor *Actor_FindNearestFacingTarget(struct Actor *self, s32 sprite) {
  struct Actor *entry;
  struct Actor *found;
  s32 slotIndex;
  s32 nearestDistance;
  FIXED heightDifference;
  s32 pixelX;
  s32 pixelZ;
  s32 distance;
  s32 angle;
  s32 angleDifference;

  found = NULL;
  nearestDistance = FACING_TARGET_REACH;
  entry = gActors;
  for (slotIndex = 0; slotIndex < ACTOR_COUNT; slotIndex++, entry++) {
    if (entry->script == NULL)
      continue;
    if (entry == self)
      continue;
    if (entry->spriteStorageKind != ACTOR_SPRITE_ONE)
      continue;
    heightDifference = entry->y.fixed - self->y.fixed;
    if (heightDifference >= 0) {
      if (heightDifference > toFIXED(FACING_TARGET_HEIGHT) - 1)
        continue;
    } else {
      if (self->y.fixed - entry->y.fixed > toFIXED(FACING_TARGET_HEIGHT) - 1)
        continue;
    }
    pixelX = (entry->x.fixed - self->x.fixed) / FIXED_ONE;
    pixelZ = (entry->z.fixed - self->z.fixed) / FIXED_ONE;
    distance = Iwram_Sqrt(pixelX * pixelX + pixelZ * pixelZ);
    if (distance >= nearestDistance)
      continue;
    angle = (u16)Trig_Atan(entry->z.fixed - self->z.fixed,
                           entry->x.fixed - self->x.fixed);
    if (distance > FACING_TARGET_NEAR) {
      angleDifference = (s16)(angle - self->facing);
      if (angleDifference < -(FACING_TARGET_ANGLE - 1))
        continue;
      if (angleDifference > FACING_TARGET_ANGLE - 1)
        continue;
    }
    found = entry;
    nearestDistance = distance;
  }
  if (found == NULL)
    return NULL;
  if (found->sprite->entries[0]->animationId != sprite)
    return NULL;
  return found;
}

#undef FOLLOW_STRIDE
#undef FACING_TARGET_REACH
#undef FACING_TARGET_HEIGHT
#undef FACING_TARGET_NEAR
#undef FACING_TARGET_ANGLE
