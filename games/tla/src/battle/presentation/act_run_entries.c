/// @file
/// Battle presentation: apply the action's effects to each target.

#include "types.h"
#include "battle.h"
#include "system.h"
#include "battle_event.h"
#include "battle_display.h"
#include "battle_type.h"

void BattleMessage_WaitForAdvance(void);

/// Clears the highlights, applies each target's effect and runs the events.
/// With no targets, only waits for the message to advance.
/// plan: action and target information.
s32 BattleStaging_RunActorEntries(struct BattlePlan *plan, s32 unused) {
  s32 i;

  BattleHighlight_SetModes(0, 0);
  if (plan->targetCount == 0) {
    BattleMessage_WaitForAdvance();
  } else {
    for (i = 0; i < plan->targetCount; i++) {
      BattleTarget_Execute(plan, i);
      BattleEvent_DispatchQueued();
    }
  }
  wait(1);
}
