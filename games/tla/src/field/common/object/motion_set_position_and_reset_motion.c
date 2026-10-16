//--------------------------------------------------------------------------
// motion_set_position_and_reset_motion.c
// Objects: set position and cancel motion.
//--------------------------------------------------------------------------

#include "object_runtime.h"

//--------------------------------------------------------------------------
// Set position, clear the destination, and zero velocity.
// obj: object.
// x, y, z: 16.16 fixed-point position.
//--------------------------------------------------------------------------
void Object_SetPositionAndResetMotion(struct ObjectRuntime *obj, s32 x, s32 y,
                                      s32 z) {
  obj->z = z;
  obj->targetX = 0x80000000; // No destination.
  obj->targetY = 0x80000000;
  obj->targetZ = 0x80000000;
  obj->x = x;
  obj->y = y;
  obj->velocityX = 0;
  obj->velocityY = 0;
  obj->velocityZ = 0;
}
