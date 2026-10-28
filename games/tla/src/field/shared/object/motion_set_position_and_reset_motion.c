/// @file
/// Objects: set position and cancel motion.

#include "object_runtime.h"
#include "macros.h"

/// Sets the position, clears the destination and zeroes the velocity.
/// obj: object.
/// x, y, z: 16.16 fixed-point position.
void Object_SetPositionAndResetMotion(struct ObjectRuntime *obj, s32 x, s32 y,
                                      s32 z) {
  obj->z = z;
  obj->targetX = 0x80000000; // No destination.
  obj->targetY = 0x80000000;
  obj->targetZ = 0x80000000;
  obj->x = x;
  obj->y = y;
  SET_VELOCITY(obj, 0, 0, 0);
}
