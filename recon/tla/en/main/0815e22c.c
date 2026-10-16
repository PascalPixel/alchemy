// Near miss, differ 4 of 43 (two swapped pairs, all else identical): the native
// loads the effect work (ldr r5, [r3, #92]) before it starts the 0x78f0 offset
// (movs r1, #240), and reads the effect (ldr r3, [r5]) before the frame is
// opened (sub sp, #24); here both come one step later. Measured: a one-pass
// block around the work load fixes the first pair but pulls sub sp and mov r8,
// r0 to the top (differ 6); a one-pass block around the effect load is differ
// 10; the work read through a macro at each use is differ 34; a pointer to the
// effect slot is this same differ 4. The same one-pass block matches the two
// neighbours without a frame (BattleFx_ProbeKeysForExtendedActor,
// BattleFx_DispatchByIdRange in fxdisp.c).
#include "types.h"
#include "ram_buffer.h"
#include "battle_effect_work.h"

struct BattleFxWorkTail {
  u8 unk0000[0x78e4];
  s32 active;
  s32 unk78e8;
  s32 unk78ec;
  struct BattleEffectArgument *effect;
};

void EffectPosition_ApplyAlternateStepAndYOffset(
    s32 id, struct EffectPosition *position);

// Midpoint between the first and last targets.
void Func_0815e22c(struct EffectPosition *position) {
  struct BattleFxWorkTail *work;
  struct EffectPosition first;
  struct EffectPosition last;

  work = (struct BattleFxWorkTail *)Ram_HeapSlots->battleFxWork[0];
  EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &first);
  EffectPosition_ApplyAlternateStepAndYOffset(
      work->effect->actors[work->effect->count - 1], &last);
  position->x = first.x + (last.x - first.x) / 2;
  position->y = first.y;
  position->depth = first.depth;
}
