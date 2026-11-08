// Differ 20 of 72 (tla-en, 2026-11-08, -mtune=gba, -Os): only the pooled
// 0x2000000 (actor 9's x) is left; here it splits to movs and lsls in both
// arms. 2026-11-08: the angle set in a local just before each call orders it
// first, as the listing does (24 to 20); without the wrapper it is 24.
// The listing object relocates that pool word as gEwramHeap: written so it
// pools, but -Os code hoisting lifts the load above the 0x8ff test (15; not
// kept: it misstates the coordinate). Also measured without gain: x in a
// local per arm, x in pixels shifted (in the inline or by a local), angle as
// the inline's second parameter, a once-only block before an arm, the 0x8ff
// test nested in an else. Permuter hits at 18-20 (6 November) were bogus
// (actor id 0x3490000, x truncated to short). No compiled ⚓️ code pools
// 0x2000000. The 0xd4c store matches through the one-pass block that
// Also 8 November: 0x2000000 as a constant local of the whole function, or of
// the arm, still splits (20).
// Func_02000624 also takes.
#include "../../../../games/tla/src/scene/madra_interiors/madra_interiors.h"

static inline void ActorPlaceFacing(s32 actor, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH(wrapper): forwarded inline, as scene_call.h describes; as direct
   * calls to Func_02001dc0, 24 of 72 instructions differ in tla-en. */
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
    ActorPlaceFacing(9, 0x2000000, 0x3490000, angle);
    angle = 0xc000;
    ActorPlaceFacing(10, 0x1d80000, 0x36d0000, angle);
    ActorPlace(8, 0, 0);
  } else {
    angle = 0xa000;
    ActorPlaceFacing(9, 0x2000000, 0x3490000, angle);
    angle = 0xc000;
    ActorPlaceFacing(10, 0x1d80000, 0x36d0000, angle);
    ActorPlace(8, 0, 0);
  }
}
