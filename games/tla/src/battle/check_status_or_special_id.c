/// @file
/// Battle: test whether an ability uses PP.

#include "battle_system.h"

/// Returns 1 for abilities that cost PP, and for Psy Drain whatever its cost.
/// abilityId: ability ID.
u32 Ability_IsPsynergy(s32 abilityId) {
  u8 pp;

  if (abilityId == 126) // Psy Drain.
    return 1;
  pp = BattleAction_Get(abilityId)->ppCost;
  return (u32)((0 - pp) | pp) >> 31; // 1 if pp is nonzero.
}
