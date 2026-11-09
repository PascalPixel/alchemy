/// @file
/// Objects: follow a watched object.

#include "actor.h"

/// Halves the distance to the watched object along x, y and z.
/// obj: object to move.
void Actor_MoveHalfwayTowardTarget(struct Actor *obj) {
  struct Actor *target = obj->watchedObject;

  obj->x += (target->x - obj->x) / 2;
  {
    s32 position = obj->y;

    position += (target->y - position) / 2;
    obj->y = position;
  }
  {
    s32 position = obj->z;

    position += (target->z - position) / 2;
    obj->z = position;
  }
}
