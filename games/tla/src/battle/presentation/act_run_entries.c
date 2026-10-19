/// @file
/// Battle presentation: apply the action's effects to each target.

#include "types.h"
#include "battle.h"
#include "system.h"
#include "battle_event.h"
#include "battle_display.h"
#include "battle_type.h"

void BattlePresentation_WaitForAdvance(void);

/// Clears the highlights, applies each target's effect and runs the events.
/// With no targets, only waits for the message to advance.
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
