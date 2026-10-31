// Near miss, differ 2 of 138 (tla-en, 2026-10-31): the native sets the probe
// argument (adds r2, r5) before loading the angle (ldrh r1); sched2 ties them
// (same priority, both independent of the last store, two dependents each)
// and takes RTL order, where the call's arguments load r0, r1, r2 in turn.
// No u16, s32 or void * prototype, angle local or position pointer moves it.
// ☀️'s Actor_GetTriggerCellAheadOfCurrent (far stub
// Actor_GetTriggerTileAheadOfCurrentFar in table_26), with ⚓️'s leader lookup
// Func_080cb8e8, a 16-pixel height window, the search event test
// Func_080ccd78(3, code) and, on the world map with ownerOverride set, 99
// where the world map collision is 3. The world map reads its cell table at
// its fixed address (Ram_MapBlocks): the constant's reload turns reload's
// register rotation so the map cells offset takes r0 as in the native (as the
// symbol gMapBlocks, r2 and differ 8). With it the one-pass block before
// ownerOverride is no longer needed. The twin's cells local, or the
// positions read inline, differ 29 and 21.
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

s32 Actor_GetTriggerCellAheadOfCurrent(void) {
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
    probePosition.vector.x = leader->pos.x;
    probePosition.vector.y = leader->pos.y;
    probePosition.vector.z = leader->pos.z;
    Vector_AddPolarOffset(0x100000, leader->angle, probePosition.coordinates);
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
