// Under -Os, 5 November: differ 24 of 69 (tla-en); older figures below predate it.
// Draft, differ 50 of 69, skeleton gap 6/8 (tla-en, 2026-11-01)
// The twin Func_02000be8 opens with this same fade. The native rebuilds its
// constants every pass and compares with 352 (blt), which is a label-and-goto
// loop: a for or while hoists them and compares with 351 (differ 80).
// Left: BLDALPHA's right side narrows to HImode, so 16 is pooled, and the
// address is built before the right side. Reaching for that with a local
// `s32 value = BLEND_ALPHA(alpha, fade); REG_BLDALPHA = value;` gives skeleton 0
// and differ 30, but it is a forced temporary, so it is not kept; a form that
// keeps SImode and evaluates the right side first without one would land there.
// At that point only registers differ, and the cause is measured:
// - i prefers STACK_REG (i - 17 fits thumb_addsi3's !k,!O alternative), so it
//   falls to ip or r0, where the native has r4; it needs one more LO_REGS use.
// - The native's fade is a global pseudo (r0, with temp r3, next r1; the sink
//   loop has r1, r2, r3), while here it is local to its block and ties to the
//   division temp. Block scope or function scope makes no difference.
// - blend (.368) ranks below back (.370), so r5 and r6 swap. One more insn in
//   both live ranges would reverse that, and may be the same missing copy.
// Tried with no gain: a u32 offset local or copy for i - 17, ++i, i + 1, the
// range spelled three ways, blend set after the call, a local for 352 or for
// 0x10000, swapped layer order, i moved up, one variable reused for the value.
#include "../../../../games/tla/src/scene/gabomba_statue_chamber/gabomba_statue_chamber.h"
#include "../../../../games/tla/include/system/io_reg.h"

struct GabombaStatueChamberLayer {
  u8 unk00[12];
  s32 y;
  u8 unk10[0x28];
};

struct GabombaStatueChamberMapWork {
  u8 unk000[0x108];
  struct GabombaStatueChamberLayer layers[3];
};

void Func_02001d40(void);
void Func_02001d60(void);
void Func_02000be8(void);

void Func_02001ba8(void) {
  if (GameFlag_Test(265)) {
    if (GameFlag_Test(519)) {
      struct GabombaStatueChamberMapWork *map =
          (struct GabombaStatueChamberMapWork *)Ram_HeapSlots->mapWork;
      struct GabombaStatueChamberLayer *front = &map->layers[0];
      struct GabombaStatueChamberLayer *back = &map->layers[2];
      s32 blend = 1;
      s32 i;

      Func_02001d60();
      i = 0;
    loop:
      if (i >= 17 && i <= 79) {
        s32 fade = (blend + 1) / 4;
        s32 alpha = 16 - fade;
        REG_BLDALPHA = BLEND_ALPHA(alpha, fade);
        blend++;
      }
      front->y += 0x10000;
      back->y += 0x10000;
      i++;
      if (i < 352)
        goto loop;
      Func_02001d40();
      wait(1);
    }
  } else if (gMainRoutineWork.entrance == 2) {
    Func_02000be8();
  }
}
