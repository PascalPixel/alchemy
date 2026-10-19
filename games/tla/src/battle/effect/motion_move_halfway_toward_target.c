/// @file
/// Objects: follow a linked object.

#include "object_runtime.h"

/// Halve the distance to the linked object along x, y, and z.
/// obj: object to move.
void ObjectMotion_MoveHalfwayTowardTarget(struct ObjectRuntime *obj) {
  struct ObjectRuntime *target = obj->linkedObject;

  obj->x += (target->x - obj->x) / 2;
  /* FAKEMATCH: separate CSE blocks retain each axis's own-position load and
     destructive add. Direct updates reverse the y/z loads under TLA's flags;
     unscoped temporaries change four add/store registers in TBS.
     2026-10-13: the three direct updates (+= or = obj->y + ...), the y and
     z temporaries in plain braces, and one function-scope temporary
     reused for y and z all match TBS and differ 4 of 26 in TLA, its
     scheduler loading the target's y and z first; a temporary for all
     three axes differs 32 in both games, a delta temporary 44. */
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
