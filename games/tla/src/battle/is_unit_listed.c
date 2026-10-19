/// @file
/// Battle: test whether a unit is in a battle formation.

#include "types.h"
#include "battle_party.h"

/// Returns 1 if the unit is in its side's formation.
/// id: unit ID (0-7 allies, all others enemies).
u32 BattleParty_IsUnitListed(u32 id) {
  u16 list[8];
  s32 side;
  s32 cnt;
  s32 i;

  side = BATTLE_SIDE_PARTY;
  if (id > 7)
    side = BATTLE_SIDE_ENEMIES;
  cnt = BattleParty_ListActorIds(side, list);

  for (i = 0; i < cnt; i++) {
    if (list[i] == id)
      break;
  }

  return i != cnt;
}
