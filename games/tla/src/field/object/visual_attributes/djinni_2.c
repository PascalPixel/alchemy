/// @file
/// A second copy of the step that halves an actor's distance to its target.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

void Actor_MoveHalfwayTowardTargetCopy(struct Actor *object) {
  struct Actor *target = object->watchedObject;

  object->x += (target->x - object->x) / 2;
  object->y += (target->y - object->y) / 2;
  object->z += (target->z - object->z) / 2;
}
