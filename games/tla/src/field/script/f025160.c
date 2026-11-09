#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "map.h"
#include "lib.h"

#define TARGET_UNSET ((s32)0x80000000)
#define DISTANCE(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

s32 Func_08025160(struct Actor *interpreter) {
  struct Actor *cameraObject = interpreter;
  struct Actor *leader;
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
  s16 *region;

  bounds = Ram_HeapSlots->mapWork;
  leader = cameraObject->watchedObject;
  minX = bounds->minX + 0x780000;
  minZ = bounds->minZ + leader->y + 0x600000;
  maxX = bounds->maxX - 0x780000;
  maxZ = bounds->maxZ + leader->y - 0x400000;
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
    if (*(s16 **)bounds->unk0fc != NULL) {
      region = *(s16 **)bounds->unk0fc;
      while (region[0] != -1) {
        minX = (region[0] << 16) - 0x780000;
        minZ = (region[1] << 16) + leader->y - 0x400000;
        maxX = (region[2] << 16) + 0x780000;
        maxZ = (region[3] << 16) + leader->y + 0x600000;
        if (x >= minX && z >= minZ && x <= maxX && z <= maxZ) {
          dx = minX;
          if (DISTANCE(minX, x) > DISTANCE(maxX, x))
            dx = maxX;
          dz = minZ;
          if (DISTANCE(minZ, z) > DISTANCE(maxZ, z))
            dz = maxZ;
          if (DISTANCE(dx, x) <= DISTANCE(dz, z))
            x = dx;
          else
            z = dz;
          break;
        }
        region += 4;
      }
    }
    if (cameraObject->action != 0) {
      cameraObject->x = x;
      cameraObject->y = y;
      cameraObject->z = z;
    } else {
      dx = (x - cameraObject->x) / 0x10000;
      /* FAKEMATCH(dead): this dy, overwritten below, as in The Broken Seal's
       * FieldCamera_FollowLeaderInBounds, which steers register allocation;
       * without it, 129 of 345 instructions differ in tla-en. */
      dy = (y - cameraObject->y) / 0x10000;
      dz = (z - cameraObject->z) / 0x10000;
      distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
      dx = x - cameraObject->x;
      dy = y - cameraObject->y;
      dz = z - cameraObject->z;
      if (distance < 0x400000)
        distance = FixedSqrt(IwramMulQ16(dx, dx) + IwramMulQ16(dz, dz));
      if ((moveStep = distance / 8) > cameraObject->speedLimit)
        moveStep = cameraObject->speedLimit;
      if (distance < 0x4000) {
        cameraObject->x = x;
        cameraObject->z = z;
      } else {
        if (distance > moveStep) {
          s32 ratio = Iwram_RatioMulQ14(distance, dx);
          s32 (*mulQ16)(s32, s32) = IwramMulQ16;
          dx = mulQ16(ratio, moveStep);
          dz = mulQ16(Iwram_RatioMulQ14(distance, dz), moveStep);
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
