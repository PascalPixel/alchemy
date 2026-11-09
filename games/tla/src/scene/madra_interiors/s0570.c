#include "madra_interiors.h"
#include "scene_tables.h"

static inline void ActorPlaceFacing(s32 actor, s32 x, s32 z, u16 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actor, x, z, angle);
}

void Func_02000570(void) {
  /* FAKEMATCH(wrapper): Actor_SetHorizontalPositionAndAngleWithTerrain
   * forwarded through ActorPlaceFacing; as direct calls to
   * Actor_SetHorizontalPositionAndAngleWithTerrain, 18 of 72 instructions
   * differ in tla-en. */
  if (!GameFlag_Test(0x8ab)) {
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 0xd4c) = 3;
    if (!GameFlag_Test(0x911)) {
      Func_020007dc();
    } else {
      Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    }
  } else if (GameFlag_Test(0x8ff)) {
    ActorPlaceFacing(9, PIXELS(515), PIXELS(841), FACING_NORTHEAST);
    ActorPlaceFacing(10, PIXELS(472), PIXELS(877), FACING_NORTH);
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  } else {
    ActorPlaceFacing(9, PIXELS(515), PIXELS(841), FACING_NORTHWEST);
    ActorPlaceFacing(10, PIXELS(472), PIXELS(877), FACING_NORTH);
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  }
}
