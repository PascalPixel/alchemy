// Matches: differ 0 (tla-en, 2026-11-08)
// Actor 9's x is PIXELS(515), not PIXELS(512): the linked overlay's pool word
// reads 0x02030000. The scaffold writes it as the relocation gEwramHeap only
// because that symbol sits at the same address, so the word shows as 0 in the
// unlinked listing object. 0x203 is not a shifted byte and has a zero low byte,
// so the constant pools as the listing does. One device: the angle call
// forwarded through an inline with a u16 angle (the game's angle type); as
// direct calls, 18. The flag tests and the plain placements are direct calls.
// Measured without gain on 8 November: x, z or the actor in locals per call,
// the angle declared in each arm's block, both angles set before the first call
// (36), a u16 angle local without the wrapper (18).
#include "../../../../games/tla/src/scene/madra_interiors/madra_interiors.h"
#include "scene_tables.h"

static inline void ActorPlaceFacing(s32 actor, s32 x, s32 z, u16 angle) {
  Func_02001dc0(actor, x, z, angle);
}

void Func_02000570(void) {
  /* FAKEMATCH(wrapper): Func_02001dc0 forwarded through ActorPlaceFacing; as
   * direct calls to Func_02001dc0, 18 of 72 instructions differ in tla-en. */
  if (!GameFlag_Test(0x8ab)) {
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 0xd4c) = 3;
    if (!GameFlag_Test(0x911)) {
      Func_020007dc();
    } else {
      Func_02001db8(9, 0, 0);
      Func_02001db8(10, 0, 0);
    }
  } else if (GameFlag_Test(0x8ff)) {
    ActorPlaceFacing(9, PIXELS(515), PIXELS(841), FACING_NORTHEAST);
    ActorPlaceFacing(10, PIXELS(472), PIXELS(877), FACING_NORTH);
    Func_02001db8(8, 0, 0);
  } else {
    ActorPlaceFacing(9, PIXELS(515), PIXELS(841), FACING_NORTHWEST);
    ActorPlaceFacing(10, PIXELS(472), PIXELS(877), FACING_NORTH);
    Func_02001db8(8, 0, 0);
  }
}
