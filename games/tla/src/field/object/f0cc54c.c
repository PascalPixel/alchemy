#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

union PolarOffsetPosition {
  struct EffectVector vector;
  s32 coordinates[3];
};

struct Object {
  u8 pad0[6];
  u16 angle;
  struct EffectVector pos;
  u8 pad1[14];
  u8 kind;
};

struct Object *Func_080cb8e8(void);
void Vector_AddPolarOffset(s32 distance, u32 angle, s32 *position);
s32 Map_GetTerrainHeightFar(s32 kind, s32 x, s32 z);
s32 Func_080ccd78(s32 kind, s32 code);
s32 GetWorldMapCollisionFar(s32 *position);

s32 Func_080cc54c(void) {
  u8 *workBytes;
  u8 *mapBytes;
  struct Object *leader;
  union PolarOffsetPosition probePosition;
  u8 *cell;
  s32 collisionCode;
  s32 terrainHeight;
  s32 result;

  result = 0;
  leader = Func_080cb8e8();
  workBytes = Ram_HeapSlots->eventWork;
  mapBytes = (u8 *)Ram_HeapSlots->mapWork;
  if (leader != 0) {
    /* FAKEMATCH(temporary): the probe distance in a local set before the
     * copies; as the constant in the call, 36 of 138 instructions differ in
     * tla-en. */
    s32 distance = 0x100000;

    probePosition.vector.x = leader->pos.x;
    probePosition.vector.y = leader->pos.y;
    probePosition.vector.z = leader->pos.z;
    {
      s32 *position = probePosition.coordinates;

      /* FAKEMATCH(barrier): the probe position passed through an empty asm;
       * as probePosition.coordinates, 2 of 138 instructions differ in
       * tla-en. */
      asm("" : "+r"(position));
      Vector_AddPolarOffset(distance, leader->angle, position);
    }
    if ((s8)workBytes[0x18a] == 3) {
      s32 x, z;

      x = probePosition.vector.x;
      z = probePosition.vector.z;
      cell = Ram_MapBlocks +
             ((((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)) << 2);
    } else {
      s32 x, z;

      cell = *(u8 **)(mapBytes + 0x138);
      x = probePosition.vector.x;
      z = probePosition.vector.z;
      cell += (((x / 0x100000) + ((z / 0x100000) << 7)) << 2);
    }
    collisionCode = cell[2];
    if (collisionCode >= 242 && collisionCode <= 247) {
      terrainHeight = Map_GetTerrainHeightFar(
          leader->kind, probePosition.vector.x, probePosition.vector.z);
      if (terrainHeight >= leader->pos.y &&
          terrainHeight <= leader->pos.y + 0x100000) {
        result = collisionCode;
      }
    } else if (Func_080ccd78(3, collisionCode) != 0) {
      result = collisionCode;
    }
    if (result == 0 && (s8)workBytes[0x18a] == 3) {
      if (gMainRoutineWork.ownerOverride != 0 &&
          GetWorldMapCollisionFar(probePosition.coordinates) == 3)
        result = 99;
    }
  }
  return result;
}
