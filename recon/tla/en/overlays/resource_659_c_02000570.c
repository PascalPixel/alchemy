// Differ 24 of 72 (tla-en, 2026-11-08, -mtune=gba): besides the pooled
// 0x2000000, actor 9's angle (224 << 8) is built before x here and after it
// natively. The listing object relocates that pool word as gEwramHeap; written
// so (not kept: it misstates the coordinate), 23.
// 6 November: ActorPlaceFacing as a forwarded inline to Func_02001dc0 (as
// ActorPlace) orders actor 10's arguments as the listing does, 28 to 24; Call4
// gives 24 too. Left: the pooled 0x2000000. Permuter hits at 18-20 are bogus
// (actor id 0x3490000, x truncated to short).
// Under -Os, 5 November: differ 22 of 72 (tla-en); older figures below predate it.
// Near miss, differ 24 of 72: the listing loads x = 0x02000000 for actor 9 from
// the literal pool in both branches, where this draft builds it as 128 << 18
// (the split constant; 0x1d80000 for actor 10 is built in both). No compiled ⚓️
// code pools 0x2000000. Measured: x and z in locals, 512 << 16, a direct call,
// a pixel-coordinate inline that shifts (all 24 to 26). The 0xd4c store matches
// through the one-pass block that Func_02000624 also takes.
#include "../../../../games/tla/src/scene/madra_interiors/madra_interiors.h"

static inline void ActorPlaceFacing(s32 actor, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH(wrapper): forwarded inline, as scene_call.h describes; as direct
   * calls to Func_02001dc0, 28 of 72 instructions differ in tla-en. */
  Func_02001dc0(actor, x, z, angle);
}

// Left: the listing pools 0x2000000 in both arms where a plain constant splits to
// movs and lsls; as (s32)gEwramHeap it pools but gcse hoists it, differ 21.
void Func_02000570(void) {
  void *event;

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
    ActorPlaceFacing(9, 0x2000000, 0x3490000, 0xe000);
    ActorPlaceFacing(10, 0x1d80000, 0x36d0000, 0xc000);
    ActorPlace(8, 0, 0);
  } else {
    ActorPlaceFacing(9, 0x2000000, 0x3490000, 0xa000);
    ActorPlaceFacing(10, 0x1d80000, 0x36d0000, 0xc000);
    ActorPlace(8, 0, 0);
  }
}
