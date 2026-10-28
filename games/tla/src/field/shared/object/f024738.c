#include "types.h"
#include "object_runtime.h"
#include "iwram_call.h"
#include "lib.h"

#define TARGET_UNSET ((s32)0x80000000)

void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z) {
  s32 dx;
  s32 distance;
  s32 dz;
  s32 squaredZ;
  s32 dy;
  u8 *arrivalAxis;

  dx = (x - object->x) / 0x10000;
  dy = (y - object->y) / 0x10000;
  dz = (z - object->z) / 0x10000;
  squaredZ = dz * dz;
  distance = Iwram_Sqrt(dx * dx + dy * dy + squaredZ) << 16;
  if (distance < 0x100000) {
    dx = x - object->x;
    dy = y - object->y;
    dz = z - object->z;
    distance =
        FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                  Iwram_MulQ16(dz, dz));
  }
  if (distance < 0x10000) {
    object->x = x;
    object->y = y;
    object->z = z;
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    return;
  }
  if (object->snapToTarget == 0) {
    s32 brakingDistance;
    s32 ratio;

    brakingDistance =
        Iwram_RatioMulQ14(object->acceleration,
                          Iwram_MulQ16(object->speedLimit, object->speedLimit));
    if (distance > brakingDistance)
      brakingDistance = distance - brakingDistance / 2;
    else
      brakingDistance = distance / 2;
    ratio = Iwram_RatioMulQ14(distance, brakingDistance);
    x = object->x + Iwram_MulQ16(x - object->x, ratio);
    y = object->y + Iwram_MulQ16(y - object->y, ratio);
    z = object->z + Iwram_MulQ16(z - object->z, ratio);
  }
  object->targetX = x;
  object->targetY = y;
  object->targetZ = z;
  dx = x - object->x;
  dy = y - object->y;
  dz = z - object->z;
  arrivalAxis = &object->unk56;
  *arrivalAxis = 16;
  if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
    *arrivalAxis = 18;
    dx = dz;
  }
  if (object->motionFlags == 0) {
    if (dx < 0)
      dx = -dx;
    if (dx < (dy < 0 ? -dy : dy))
      *arrivalAxis = 17;
  }
}
