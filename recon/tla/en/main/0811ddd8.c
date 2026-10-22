// Draft, differ 4 of 78: Func_0811ddd8 is ☀️'s BattlePresentation_BuildTargetList
// (games/tbs/src/battle/battle/staging.c) with ⚓️'s added test that the
// target is in the battle (BattleParty_IsUnitListed); void, as its callers
// load r0 before r1 for it. Left: the opening, the plan's copy into r6 one
// slot ahead of the flags' store, and sub sp four slots early (the one-pass
// block that places mov sl, r1 moves it). Devices: one-pass blocks (tagged);
// without them 12 differ (10 for the old for-loop draft).
#include "types.h"
#include "battle_type.h"
#include "battle_display.h"

struct BattleUnit *Unit_Get(s32 unitId);
u32 BattleParty_IsUnitListed(u32 id);

void BattlePres_BuildTargetList(struct BattlePlan *plan,
                                struct BattlePresentationWork *effect) {
  s16 *outputTarget;
  u32 flags;
  s32 livingTargets;
  s32 targetIndex;
  s32 count;
  u8 *inputTarget;

  livingTargets = 0;
  effect->flags = 0;
  flags = plan->presentationFlags;
  /* FAKEMATCH(one-pass): the kind's store ends a CSE block, so
   * targetIndex's zero is copied before the actor's store; as a plain
   * statement, 8 of 78 instructions differ in tla-en. */
  do {
    effect->kind = flags & 0xfff;
  } while (0);
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
      count = plan->targetCount;
      /* FAKEMATCH(one-pass): the count is read before targetIndex
       * steps; as while (targetIndex < plan->targetCount), 10 of 78
       * instructions differ in tla-en. */
      do {
      } while (0);
      targetIndex++;
      inputTarget++;
    } while (targetIndex < count);
  }
  if (livingTargets == 0) {
    effect->actors[0] = plan->targetIds[0];
    livingTargets = 1;
  }
  effect->firstTarget = plan->targetIds[0];
  effect->count = livingTargets;
  effect->initialValue = 1;
}
