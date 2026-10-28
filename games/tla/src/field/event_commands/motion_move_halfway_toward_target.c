/// @file
/// Objects: follow a watched object.

#include "actor.h"

/// Halves the distance to the watched object along x, y and z.
/// obj: object to move.
void Actor_MoveHalfwayTowardTarget(struct Actor *obj) {
  struct Actor *target = obj->watchedObject;

  obj->x += (target->x - obj->x) / 2;
  /* FAKEMATCH: separate CSE blocks keep each axis's own-position load and
   * destructive add; as direct updates (obj->y += ...), 4 of 26 instructions
   * differ in tla-en. */
  do {
    s32 position = obj->y;

    position += (target->y - position) / 2;
    obj->y = position;
  } while (0);
  do {
    s32 position = obj->z;

    position += (target->z - position) / 2;
    obj->z = position;
  } while (0);
}
