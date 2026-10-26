/// @file
/// Actor scripts: moving, turning and the built-in actions.
// Part of field_services.c.

#include "../field_services_internal.h"

/// Script step of action 7: clears an object's sprite flags and its collision
/// flags, so nothing collides with it. Returns 0, so SC_CALL moves on at
/// once (gMovementType_ResetActionScript then ends).
s32 Object_ClearChildAndCollisionFlags(void *obj) {
  Object_SetSpriteFlagsFar(obj, 0);
  ((struct Object *)obj)->collisionFlags = 0;
  return 0;
}

/// Script step of action 5: follows the linked object. While it is at least
/// action pixels away the object walks (animation 2) toward a point 16 /
/// action of the way there, so with action 16 it heads for the linked
/// object itself; closer, it stands (animation 1). Returns 1, so SC_CALL
/// runs it again every frame until the actor's script is replaced.
s32 ObjectMotion_MoveTowardTarget(struct Object *object) {
  s32 step;
  struct Object *target;
  s32 deltaX;
  s32 deltaZ;
  s32 pixelsX;
  s32 pixelsZ;
  s32 newX;
  s32 newZ;
  s32 distance;

  target = object->linkedObject;
  if (target != 0) {
    // Whole pixels, rounded toward zero as a division by 0x10000 would.
    deltaX = target->x - object->x;
    if (deltaX < 0)
      deltaX += 0xffff;
    pixelsX = deltaX >> 16;
    deltaZ = target->z - object->z;
    if (deltaZ < 0)
      deltaZ += 0xffff;
    pixelsZ = deltaZ >> 16;
    distance = Iwram_Sqrt(pixelsX * pixelsX + pixelsZ * pixelsZ);
    step = object->action;
    if (distance >= step) {
      // << 20 is 16 pixels in 16.16 per pixel of distance. Offsets can be
      // negative: shift their word representation before reading it as a
      // signed displacement, rather than shifting a negative.
      newX = object->x + (s32)((u32)pixelsX << 20) / step;
      newZ = object->z + (s32)((u32)pixelsZ << 20) / step;
      Object_SetMoveTargetFar(object, newX, object->y, newZ);
      Object_SelectAnimationFar(object, 2);
    } else {
      Object_SelectAnimationFar(object, 1);
    }
  }
  return 1;
}

/// Script step of action 6: turns the object toward its linked object, at most
/// 1/16 turn a frame. Returns 1, so SC_CALL runs it again every frame until
/// the actor's script is replaced.
s32 ObjectMotion_TurnTowardLinkedTarget(struct Object *object) {
  s16 turnStep;
  u16 currentAngle;
  s32 targetAngle;
  struct Object *target;

  target = object->linkedObject;
  if (target != NULL) {
    // Clear ACTOR_FACES_MOTION, so moving does not turn it away again.
    object->steeringFlags &= 0xfe;
    targetAngle = (u16)ArcTan2(target->z - object->z, target->x - object->x);
    currentAngle = object->facing;
    turnStep = targetAngle - currentAngle;
    if (turnStep != 0) {
      if (turnStep > 0x1000)
        turnStep = 0x1000;
      if (turnStep < -0x1000)
        turnStep = -0x1000;
      object->facing = (currentAngle + turnStep);
    }
  }
  return 1;
}

/// Starts an actor's action script: an enum MovementType, or any other
/// value as the script to run itself. The built-in scripts are in scripts.s
/// (field.h). Reached from an actor template's behavior column
/// (overworld/actors.c), from Scene_TalkToActor after a talk
/// (overworld/events.c), from the actor commands and from Lalivero's
/// interiors.
void Actor_SetMovementType(struct Object *object, s32 action) {
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
    object->linkedObject =
        (struct Object *)Actor_GetById(gSaveBlock.leaderActorId);
    action = (s32)gMovementType_FaceLeaderScript;
    break;
  case MOVEMENT_TYPE_RESET_ACTION:
    action = (s32)gMovementType_ResetActionScript;
    break;
  default:
    break;
  }
  Object_InitializeFar((struct Object *)object, (const s32 *)action);
}

/// Finds the nearest animated object to self within 40 pixels and 48 pixels of
/// height, ignoring objects more than 3/16 turn to either side once they are
/// over 23 pixels away. Returns it if it shows sprite, otherwise NULL: the
/// sprite is tested only on the nearest object, so a nearer object of
/// another sprite hides a matching one. Field_TryJumpForward (climbing.c)
/// asks it for a log, SPRITE_LOG_WEST_EAST or SPRITE_LOG_NORTH_SOUTH, where the
/// leader lands.
struct Object *Object_FindNearestFacingTarget(struct Object *self, s32 sprite) {
  struct Object *entry;
  struct Object *found;
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
  entry = gObjects;
  for (slotIndex = 0; slotIndex < 64; slotIndex++, entry++) {
    if (entry->script == NULL)
      continue;
    if (entry == self)
      continue;
    if (entry->spriteStorageKind != OBJECT_SPRITE_ONE)
      continue;
    // 0x2fffff: under 48 pixels apart in height, either way.
    heightDifference = entry->y - self->y;
    if (heightDifference >= 0) {
      if (heightDifference > 0x2fffff)
        continue;
    } else {
      if (self->y - entry->y > 0x2fffff)
        continue;
    }
    pixelX = (entry->x - self->x) / Q16_ONE;
    pixelZ = (entry->z - self->z) / Q16_ONE;
    distance = Iwram_Sqrt(pixelX * pixelX + pixelZ * pixelZ);
    if (distance >= nearestDistance)
      continue;
    angle = (u16)ArcTan2(entry->z - self->z, entry->x - self->x);
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
