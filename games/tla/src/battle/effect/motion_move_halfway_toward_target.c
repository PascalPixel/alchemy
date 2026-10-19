/// @file
/// Objects: follow a linked object.

#include "object_runtime.h"

/// Halves the distance to the linked object along x, y and z.
/// obj: object to move.
void ObjectMotion_MoveHalfwayTowardTarget(struct ObjectRuntime *obj) {
  struct ObjectRuntime *target = obj->linkedObject;

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
