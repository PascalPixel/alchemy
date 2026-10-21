/// @file
/// Actor scripts: moving, turning and the built-in actions.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// Script step of action 7: clears an object's sprite flags and its collision
/// flags, so nothing collides with it. Returns 0, so SC_CALL moves on at
/// once (gObjectMotion_ResetActionScript then ends).
s32 Object_ClearChildAndCollisionFlags(void *obj) {
  ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
  ((struct ScriptObjectRuntime *)obj)->flags59 = 0;
  return 0;
}

/// Script step of action 5: follows the linked object. While it is at least
/// action pixels away the object walks (animation 2) toward a point 16 /
/// action of the way there, so with action 16 it heads for the linked
/// object itself; closer, it stands (animation 1). Returns 1, so SC_CALL
/// runs it again every frame until the actor's script is replaced.
s32 ObjectMotion_MoveTowardTarget(struct ObjectRuntime *object) {
  s32 step;
  struct ObjectRuntime *target;
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
      ObjectDispatch_SelectAnimationFar(object, 2);
    } else {
      ObjectDispatch_SelectAnimationFar(object, 1);
    }
  }
  return 1;
}

/// Script step of action 6: turns the object toward its linked object, at most
/// 1/16 turn a frame. Returns 1, so SC_CALL runs it again every frame until
/// the actor's script is replaced.
s32 ObjectMotion_TurnTowardLinkedTarget(struct ObjectRuntime *object) {
  s16 turnStep;
  u16 currentAngle;
  s32 targetAngle;
  struct ObjectRuntime *target;

  target = object->linkedObject;
  if (target != NULL) {
    // Clear ACTOR_FACES_MOTION, so moving does not turn it away again.
    object->actionFlags &= 0xfe;
    targetAngle = (u16)ArcTan2(target->z - object->z, target->x - object->x);
    currentAngle = object->angle;
    turnStep = targetAngle - currentAngle;
    if (turnStep != 0) {
      if (turnStep > 0x1000)
        turnStep = 0x1000;
      if (turnStep < -0x1000)
        turnStep = -0x1000;
      object->angle = (currentAngle + turnStep);
    }
  }
  return 1;
}

/// Starts an actor's action script: an enum ActorBehavior, or any other value
/// as the script to run itself. The built-in scripts are in scripts.s
/// (field.h). Reached from a placement's behavior column (object/actors.c),
/// from Scene_TalkToActor after a talk (object/events.c), from the actor
/// commands and from Lalivero's interiors (Engine_ActorSetActionCallback).
void ObjectMotion_SetActionCallback(struct ObjectRuntime *object, s32 action) {
  switch (action) {
  case ACTOR_STAND:
    action = (s32)gObjectMotion_ActionKind1Script;
    break;
  case ACTOR_WANDER:
    action = (s32)gObjectMotion_ActionKind2Script;
    break;
  case ACTOR_WANDER_NEAR:
    action = (s32)gObjectMotion_ActionKind3Script;
    break;
  case ACTOR_WANDER_CLOSE:
    action = (s32)gObjectMotion_ActionKind4Script;
    break;
  case ACTOR_FOLLOW_LINKED:
    action = (s32)gObjectMotion_MoveTowardTargetScript;
    break;
  case ACTOR_FACE_LEADER:
    object->linkedObject =
        (struct ObjectRuntime *)Object_GetById(gGameState.selectedActor);
    action = (s32)gObjectMotion_TurnTowardLinkedScript;
    break;
  case ACTOR_RESET_ACTION:
    action = (s32)gObjectMotion_ResetActionScript;
    break;
  default:
    break;
  }
  ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                               (const s32 *)action);
}

/// Finds the nearest animated object to self within 40 pixels and 48 pixels of
/// height, ignoring objects more than 3/16 turn to either side once they are
/// over 23 pixels away. Returns it if it shows sprite, otherwise NULL: the
/// sprite is tested only on the nearest object, so a nearer object of
/// another sprite hides a matching one. Field_TryJumpForward (climbing.c)
/// asks it for a platform, sprite 207 or 205, where the leader lands.
struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *self,
                                                     s32 sprite) {
  struct ObjectRuntime *entry;
  struct ObjectRuntime *found;
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
  entry = gObjectSlots;
  for (slotIndex = 0; slotIndex < 64; slotIndex++, entry++) {
    if (entry->script == NULL)
      continue;
    if (entry == self)
      continue;
    if (entry->animationKind != OBJECT_SPRITE_ONE)
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
    pixelX = (entry->x - self->x) / 0x10000;
    pixelZ = (entry->z - self->z) / 0x10000;
    distance = Iwram_Sqrt(pixelX * pixelX + pixelZ * pixelZ);
    if (distance >= nearestDistance)
      continue;
    angle = (u16)ArcTan2(entry->z - self->z, entry->x - self->x);
    if (distance > 23) {
      angleDifference = (s16)(angle - self->angle);
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
  if (((struct AnimationObject *)found->animation)->entries[0]->animId !=
      sprite)
    return NULL;
  return found;
}
