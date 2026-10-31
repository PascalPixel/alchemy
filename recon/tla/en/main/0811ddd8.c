// Near miss, differ 4 of 78 (tla-en, 2026-10-31): scheduler order only. The
// native issues sub sp right after the variant's store, then the actor's ldrb,
// then mov sl, r1 before the actor's store; here the ldrb comes first, sub sp
// after it and mov sl, r1 after the targetCount load. sched2's dump gives the
// native order exactly if the ldrb waits two cycles after the variant's store,
// which may alias it; -mthumb-slow-loads charges that wait only for a plain
// load, not an extending one, so the ldrb issues at once (080d3c88 records the
// same for an ldrsh). No device: this is the Broken Seal's form with the added
// test. A one-pass block around the kind's store scores 2 by making sub sp a
// barrier's predecessor, so it issues after the flags' load, ten slots ahead of
// the native; not kept. The earlier draft's one-pass block before the count's
// read no longer changes anything.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, each local pinned to each register,
// every order of the opening's seven statements (none under 4), five minutes
// of permute (26 October) and four more (31 October, 12,903 candidates: best
// the one-pass block above); none reaches 0.
// Func_0811ddd8 is ☀️'s BattleStaging_BuildTargetList
// (games/tbs/src/battle/battle/staging.c) with ⚓️'s added test that the
// target is in the battle (BattleParty_IsUnitListed); void, as its callers
// load r0 before r1 for it.
#include "types.h"
#include "battle_type.h"
#include "battle_display.h"

struct BattleUnit *Unit_Get(s32 unitId);
u32 BattleParty_IsUnitListed(u32 id);

void BattleStaging_BuildTargetList(struct BattlePlan *plan,
                                struct BattlePresentationWork *effect) {
  s16 *outputTarget;
  u32 flags;
  s32 livingTargets;
  s32 targetIndex;
  u8 *inputTarget;

  livingTargets = 0;
  effect->flags = 0;
  flags = plan->presentationFlags;
  effect->kind = flags & 0xfff;
  effect->variant = (flags & 0x3000) >> 12;
  effect->actor = plan->actorId;
  targetIndex = 0;
  if (targetIndex < plan->targetCount) {
    inputTarget = plan->targetIds;
    outputTarget = effect->actors;
    do {
      if (BattleParty_IsUnitListed(*inputTarget) &&
          (Unit_Get(*inputTarget)->hp != 0 ||
           (plan->presentationFlags & 0x10000))) {
        *outputTarget++ = *inputTarget;
        livingTargets++;
      }
      targetIndex++;
      inputTarget++;
    } while (targetIndex < plan->targetCount);
  }
  if (livingTargets == 0) {
    effect->actors[0] = plan->targetIds[0];
    livingTargets = 1;
  }
  effect->firstTarget = plan->targetIds[0];
  effect->count = livingTargets;
  effect->initialValue = 1;
}
