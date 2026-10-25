// Near miss, differ 4 of 228 (two reordered pairs): ☀️'s
// Camera_FollowLeaderInBounds (games/tbs/src/field/field.c) on The Lost Age's
// map, whose vertical band does not follow the leader's height: SC_CMD_2F.
// Left: the native loads the leader's z before it copies x to r11, and the
// camera's z and the spilled z before it builds 0x400000, each one row
// earlier than sched2 here.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "map.h"
#include "lib.h"

#define TARGET_UNSET ((s32)0x80000000)

s32 Func_08025428(struct ObjectRuntime *cameraObject) {
  struct ObjectRuntime *leader;
  struct MapWork *bounds;
  s32 minX;
  s32 minZ;
  s32 maxX;
  s32 maxZ;
  s32 x;
  s32 y;
  s32 z;
  s32 dz;
  s32 dy;
  s32 dx;
  s32 distance;
  s32 moveStep;

  bounds = Ram_HeapSlots->mapWork;
  leader = cameraObject->linkedObject;
  minX = bounds->minX + 0x780000;
  minZ = bounds->minZ + 0x600000;
  maxX = bounds->maxX - 0x780000;
  maxZ = bounds->maxZ - 0x400000;
  cameraObject->motionFlags = 0;
  if (leader != NULL && leader->script != NULL) {
    x = leader->x;
    y = leader->y;
    z = leader->z;
    cameraObject->targetX = TARGET_UNSET;
    cameraObject->targetY = TARGET_UNSET;
    cameraObject->targetZ = TARGET_UNSET;
    if (x < minX)
      x = minX;
    if (z < minZ)
      z = minZ;
    if (x > maxX)
      x = maxX;
    if (z > maxZ)
      z = maxZ;
    if (cameraObject->action != 0) {
      cameraObject->x = x;
      cameraObject->y = y;
      cameraObject->z = z;
    } else {
      dx = (x - cameraObject->x) / 0x10000;
      /* FAKEMATCH(dead): this dy, overwritten below, ranks y above the bounds
       * in global allocation and keeps it in r5 as native; without it, 54 of
       * 228 instructions differ in tla-en. */
      dy = (y - cameraObject->y) / 0x10000;
      dz = (z - cameraObject->z) / 0x10000;
      distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
      dx = x - cameraObject->x;
      dy = y - cameraObject->y;
      dz = z - cameraObject->z;
      if (distance < 0x400000)
        distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
      if ((moveStep = distance / 8) > cameraObject->speedLimit)
        moveStep = cameraObject->speedLimit;
      if (distance < 0x4000) {
        cameraObject->x = x;
        cameraObject->z = z;
      } else {
        if (distance > moveStep) {
          dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), moveStep);
          dz = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dz), moveStep);
        }
        cameraObject->x += dx;
        cameraObject->z += dz;
      }
      if ((dy < 0 ? -dy : dy) > 0x8000)
        dy /= 4;
      cameraObject->y += dy;
    }
  }
  cameraObject->step++;
  return 1;
}
