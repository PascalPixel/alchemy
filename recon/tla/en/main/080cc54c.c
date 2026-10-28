// Under -mthumb-slow-loads (2026-10-26): differ 8 of 138 in tla-en, instruction
// order and register choice. No empty asm before any one statement lowers it.
// Near miss, differ 8 of 138: ☀️'s Actor_GetTriggerCellAheadOfCurrent
// (far stub Actor_GetTriggerTileAheadOfCurrentFar in table_26), with ⚓️'s
// leader lookup Func_080cb8e8, a 16-pixel height window, the search event
// test Func_080ccd78(3, code) and, on the world map with ownerOverride set,
// 99 where the world map collision is 3. The one-pass block (FAKEMATCH on
// adoption) loads gSequenceParentWork before ownerOverride's offset (differ 10
// without it). Left: the native sets the probe argument (adds r2, r5)
// before loading the angle (ldrh r1), where sched2 ties them at priority 7
// and takes RTL order; and outside the world map it builds the cells
// offset in r0 where agscc takes r2 (z and cells read first, or z kept in
// a function-wide local, differ 24 or 8; a struct copy of the position 79).
#include "types.h"
#include "sequence_parent_work.h"
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

extern u8 gMapBlocks[];

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
  u8 *cells;
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
      cell = gMapBlocks +
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
      struct SequenceParentWork *party = &gSequenceParentWork;

      do {
      } while (0);
      if (party->ownerOverride != 0 &&
          GetWorldMapCollisionFar(probePosition.coordinates) == 3)
        result = 99;
    }
  }
  return result;
}
