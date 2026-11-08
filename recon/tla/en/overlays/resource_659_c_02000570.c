// Matches: differ 0 (tla-en, 2026-11-08)
// Actor 9's x is 0x2030000 (515 pixels), not 0x2000000: the linked overlay's
// pool word reads 0x02030000. The scaffold writes it as the relocation
// gEwramHeap only because that symbol sits at the same address, so the word
// shows as 0 in the unlinked listing object. 0x203 is not a shifted byte and
// has a zero low byte, so the constant pools as the listing does. Both devices
// are needed: the forwarded inline, and the angle set in a local just before
// each call (wrapper without the locals 12, locals without the wrapper 14).
// Also measured without gain on 8 November: x, z or the actor in locals per
// call, the angle declared in each arm's block, both angles set before the
// first call (36), the angle as u16 (18). Earlier rounds' notes on 0x2000000
// (pooling it through a symbol, splitting it) are moot. The 0xd4c store matches
// through the one-pass block that Func_02000624 also takes.
#include "../../../../games/tla/src/scene/madra_interiors/madra_interiors.h"

static inline void ActorPlaceFacing(s32 actor, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH(wrapper): forwarded inline, as scene_call.h describes; as direct
   * calls to Func_02001dc0, 14 of 72 instructions differ in tla-en. */
  Func_02001dc0(actor, x, z, angle);
}

void Func_02000570(void) {
  void *event;
  // The angle is set before each call; the native builds it first.
  s32 angle;

  if (!FlagCheck(0x8ab)) {
    event = Ram_HeapSlots->eventWork;
    FIELD_AT(event, u16, 0xd4c) = 3;
    if (!FlagCheck(0x911)) {
      Func_020007dc();
    } else {
      ActorPlace(9, 0, 0);
      ActorPlace(10, 0, 0);
    }
  } else if (FlagCheck(0x8ff)) {
    angle = 0xe000;
    ActorPlaceFacing(9, 0x2030000, 0x3490000, angle);
    angle = 0xc000;
    ActorPlaceFacing(10, 0x1d80000, 0x36d0000, angle);
    ActorPlace(8, 0, 0);
  } else {
    angle = 0xa000;
    ActorPlaceFacing(9, 0x2030000, 0x3490000, angle);
    angle = 0xc000;
    ActorPlaceFacing(10, 0x1d80000, 0x36d0000, angle);
    ActorPlace(8, 0, 0);
  }
}
