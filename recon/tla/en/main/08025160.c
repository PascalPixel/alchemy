// Draft, differ 332 (378 before statements reordered 2026-10-26) of 345: logic in place (the region push decoded from
// the listing), allocation far: the native spills the camera object to sp+4
// and keeps x, y and z in r10, r11 and r9 with an 8-byte frame; here x and z
// take r8 and r10 and the frame is 12 bytes.
// SC_CAMERA, ☀️'s Camera_FollowLeaderInBounds (games/tbs/src/field/
// field.c) with The Lost Age's keep-out regions: after the map bounds, a
// target inside one of the map's rectangles (a list of s16 x0, z0, x1, z1 in
// pixels, ended by x0 == -1) is pushed out to its nearest edge.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "map.h"
#include "lib.h"

#define TARGET_UNSET ((s32)0x80000000)
#define DISTANCE(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

s32 Func_08025160(struct ObjectRuntime *cameraObject) {
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
  s16 *region;
  s32 nearX;
  s32 nearZ;

  bounds = Ram_HeapSlots->mapWork;
  leader = cameraObject->linkedObject;
  minX = bounds->minX + 0x780000;
  minZ = bounds->minZ + leader->y + 0x600000;
  maxX = bounds->maxX - 0x780000;
  maxZ = bounds->maxZ + leader->y - 0x400000;
  cameraObject->motionFlags = 0;
  if (leader != NULL && leader->script != NULL) {
    x = leader->x;
    z = leader->z;
    cameraObject->targetX = TARGET_UNSET;
    cameraObject->targetY = TARGET_UNSET;
    cameraObject->targetZ = TARGET_UNSET;
    y = leader->y;
    if (x < minX)
      x = minX;
    if (z < minZ)
      z = minZ;
    if (x > maxX)
      x = maxX;
    if (z > maxZ)
      z = maxZ;
    region = *(s16 **)bounds->unk0fc;
    if (region != NULL) {
      while (region[0] != -1) {
        minX = (region[0] << 16) - 0x780000;
        minZ = (region[1] << 16) + leader->y - 0x400000;
        maxZ = (region[3] << 16) + leader->y + 0x600000;
        maxX = (region[2] << 16) + 0x780000;
        if (x >= minX && z >= minZ && x <= maxX && z <= maxZ) {
          nearX = minX;
          if (DISTANCE(minX, x) > DISTANCE(maxX, x))
            nearX = maxX;
          nearZ = minZ;
          if (DISTANCE(minZ, z) > DISTANCE(maxZ, z))
            nearZ = maxZ;
          if (DISTANCE(nearX, x) <= DISTANCE(nearZ, z))
            x = nearX;
          else
            z = nearZ;
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
      /* FAKEMATCH: dead code, as in ☀️'s twin. */
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
