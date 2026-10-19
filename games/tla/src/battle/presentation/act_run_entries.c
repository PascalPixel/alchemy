/// @file
/// Battle presentation: apply the action's effects to each target.

#include "types.h"
#include "battle.h"
#include "system.h"
#include "battle_event.h"
#include "battle_display.h"
#include "battle_type.h"

void BattlePresentation_WaitForAdvance(void);

/// Clear highlights, apply each target's effect, and run the events.
/// With no targets, only wait for message advancement.
/// plan: action and target information.
s32 BattlePres_RunActorEntries(struct BattlePlan *plan, s32 unused) {
  s32 i;

  BattlePres_SetActorModes(0, 0);
  if (plan->targetCount == 0) {
    BattlePresentation_WaitForAdvance();
  } else {
    for (i = 0; i < plan->targetCount; i++) {
      BattleTargetExec(plan, i);
      BattleEv_DispatchQueued();
    }
  }
  WaitFrames(1);
}
