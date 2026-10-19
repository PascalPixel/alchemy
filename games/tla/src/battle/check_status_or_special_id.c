/// @file
/// Battle: test whether an ability uses PP.

#include "battle_system.h"

/// Return 1 for PP-consuming abilities; Psy Drain returns 1 regardless of PP.
/// abilityId: ability ID.
u32 Ability_IsPsynergy(s32 abilityId) {
  u8 pp;

  if (abilityId == 126) // Psy Drain.
    return 1;
  pp = BattleActGet(abilityId)->ppCost;
  return (u32)((0 - pp) | pp) >> 31; // 1 if pp is nonzero.
}
