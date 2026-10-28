/// @file
/// Actor scripts: moving, turning and the built-in movement types.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// The SC_CALL routine of movement type 7: clears an actor's sprite flags and
/// its collision flags, so nothing collides with it. Returns 0, so SC_CALL
/// moves on at once (gMovementType_ResetActionScript then ends).
s32 Actor_ClearChildAndCollisionFlags(void *obj) {
  Actor_SetSpriteFlagsFar(obj, 0);
  ((struct Actor *)obj)->collisionFlags = 0;
  return 0;
}

/// The SC_CALL routine of movement type 5: follows the linked actor. While it
/// is at least timer pixels away the actor walks (animation 2) toward a point
/// 16 / timer of the way there, so with timer 16 it heads for the linked actor
/// itself; closer, it stands (animation 1). Returns 1, so SC_CALL runs it again
/// every frame until the actor's script is replaced.
s32 Actor_MoveTowardTarget(struct Actor *actor) {
  s32 step;
  struct Actor *target;
  s32 deltaX;
  s32 deltaZ;
  s32 pixelsX;
  s32 pixelsZ;
  s32 newX;
  s32 newZ;
  s32 distance;

  target = actor->linkedActor;
  if (target != 0) {
    // Whole pixels, rounded toward zero as a division by 0x10000 would.
    deltaX = target->x.fixed - actor->x.fixed;
    if (deltaX < 0)
      deltaX += 0xffff;
    pixelsX = deltaX >> 16;
    deltaZ = target->z.fixed - actor->z.fixed;
    if (deltaZ < 0)
      deltaZ += 0xffff;
    pixelsZ = deltaZ >> 16;
    distance = Iwram_Sqrt(pixelsX * pixelsX + pixelsZ * pixelsZ);
    step = actor->timer;
    if (distance >= step) {
      // << 20 is 16 pixels in 16.16 per pixel of distance. Offsets can be
      // negative: shift their word representation before reading it as a
      // signed displacement, rather than shifting a negative.
      newX = actor->x.fixed + (s32)((u32)pixelsX << 20) / step;
      newZ = actor->z.fixed + (s32)((u32)pixelsZ << 20) / step;
      Actor_SetMoveTargetFar(actor, newX, actor->y.fixed, newZ);
      Actor_SelectAnimationFar(actor, 2);
    } else {
      Actor_SelectAnimationFar(actor, 1);
    }
  }
  return 1;
}

/// The SC_CALL routine of movement type 6: turns the actor toward its linked
/// actor, at most 1/16 turn a frame. Returns 1, so SC_CALL runs it again every
/// frame until the actor's script is replaced.
s32 Actor_TurnTowardLinkedActor(struct Actor *actor) {
  s16 turnStep;
  u16 currentAngle;
  s32 targetAngle;
  struct Actor *target;

  target = actor->linkedActor;
  if (target != NULL) {
    // Clear ACTOR_FACES_MOTION, so moving does not turn it away again.
    actor->steeringFlags &= 0xfe;
    targetAngle = (u16)ArcTan2(target->z.fixed - actor->z.fixed,
                               target->x.fixed - actor->x.fixed);
    currentAngle = actor->facing;
    turnStep = targetAngle - currentAngle;
    if (turnStep != 0) {
      if (turnStep > 0x1000)
        turnStep = 0x1000;
      if (turnStep < -0x1000)
        turnStep = -0x1000;
      actor->facing = (currentAngle + turnStep);
    }
  }
  return 1;
}

/// Starts an actor's movement type: an enum MovementType, or any other
/// value as the actor script to run itself. The built-in scripts are in
/// scripts.s (field.h). Reached from an actor template's behavior column
/// (overworld/actors.c), from Scene_TalkToActor after a talk
/// (overworld/triggers.c), from the actor commands and from Lalivero's
/// interiors.
void Actor_SetMovementType(struct Actor *actor, s32 action) {
  switch (action) {
  case MOVEMENT_TYPE_STAND:
    action = (s32)gMovementType_StandScript;
    break;
  case MOVEMENT_TYPE_WANDER:
    action = (s32)gMovementType_WanderScript;
    break;
  case MOVEMENT_TYPE_WANDER_NEAR:
    action = (s32)gMovementType_WanderNearScript;
    break;
  case MOVEMENT_TYPE_WANDER_CLOSE:
    action = (s32)gMovementType_WanderCloseScript;
    break;
  case MOVEMENT_TYPE_FOLLOW_LINKED:
    action = (s32)gMovementType_FollowLinkedScript;
    break;
  case MOVEMENT_TYPE_FACE_LEADER:
    actor->linkedActor = Actor_GetById(gAdventure.leaderActorId);
    action = (s32)gMovementType_FaceLeaderScript;
    break;
  case MOVEMENT_TYPE_RESET_ACTION:
    action = (s32)gMovementType_ResetActionScript;
    break;
  default:
    break;
  }
  Actor_StartScriptFar(actor, (const s32 *)action);
}

/// Finds the nearest animated actor to self within 40 pixels and 48 pixels of
/// height, ignoring actors more than 3/16 turn to either side once they are
/// over 23 pixels away. Returns it if it shows sprite, otherwise NULL: the
/// sprite is tested only on the nearest actor, so a nearer actor of
/// another sprite hides a matching one. Field_TryJumpForward (climbing.c)
/// asks it for a log, SPRITE_LOG_WEST_EAST or SPRITE_LOG_NORTH_SOUTH, where the
/// leader lands.
struct Actor *Actor_FindNearestFacingTarget(struct Actor *self, s32 sprite) {
  struct Actor *entry;
  struct Actor *found;
  s32 slotIndex;
  s32 nearestDistance;
  s32 heightDifference;
  s32 pixelX;
  s32 pixelZ;
  s32 distance;
  s32 angle;
  s32 angleDifference;

  found = NULL;
  nearestDistance = 40;
  entry = gActors;
  for (slotIndex = 0; slotIndex < 64; slotIndex++, entry++) {
    if (entry->script == NULL)
      continue;
    if (entry == self)
      continue;
    if (entry->spriteStorageKind != ACTOR_SPRITE_ONE)
      continue;
    // 0x2fffff: under 48 pixels apart in height, either way.
    heightDifference = entry->y.fixed - self->y.fixed;
    if (heightDifference >= 0) {
      if (heightDifference > 0x2fffff)
        continue;
    } else {
      if (self->y.fixed - entry->y.fixed > 0x2fffff)
        continue;
    }
    pixelX = (entry->x.fixed - self->x.fixed) / Q16_ONE;
    pixelZ = (entry->z.fixed - self->z.fixed) / Q16_ONE;
    distance = Iwram_Sqrt(pixelX * pixelX + pixelZ * pixelZ);
    if (distance >= nearestDistance)
      continue;
    angle = (u16)ArcTan2(entry->z.fixed - self->z.fixed,
                         entry->x.fixed - self->x.fixed);
    if (distance > 23) {
      angleDifference = (s16)(angle - self->facing);
      if (angleDifference < -0x2fff)
        continue;
      if (angleDifference > 0x2fff)
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
