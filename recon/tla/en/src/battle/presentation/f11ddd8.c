#include "types.h"
#include "call.h"
#include "battle_type.h"
#include "battle_display.h"

struct BattleUnit *Unit_Get(s32 unitId);
u32 BattleParty_IsUnitListed(u32 id);

void Func_0811ddd8(struct BattlePlan *plan,
                   struct BattlePresentationWork *effect) {
  s16 *outputTarget;
  u32 flags;
  s32 livingTargets;
  s32 targetIndex;
  u8 *inputTarget;

  livingTargets = 0;
  effect->flags = 0;
  flags = plan->presentationFlags;
  effect->kind = flags & SPLIT_CONSTANT(0xfff);
  effect->variant = (flags & 0x3000) >> 12;
  /* FAKEMATCH(volatile): the actor read through a volatile pointer, a plain
   * byte load that waits after the variant's store; as plan->actorId, 4 of
   * 78 instructions differ in tla-en. */
  effect->actor = *(volatile u8 *)&plan->actorId;
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
