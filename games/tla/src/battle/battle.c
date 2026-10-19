//--------------------------------------------------------------------------
// actor_commit_placement.c
// Battle: place objects for all units.
//--------------------------------------------------------------------------

#include "types.h"
#include "battle.h"
#include "battle_party.h"
#include "battle_display.h"

/// Readies every unit's battle graphics, releasing those of departed units,
/// and resets everyone's display mode and motion.
void BattleObjPut(void) {
  s16 list[14];

  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  BattleActor_SpawnObjectsForList(list, 1);
}

//--------------------------------------------------------------------------

// list_all_units_and_submit.c

// Battle: place objects for all units.

//--------------------------------------------------------------------------

/// Readies every unit's battle graphics, releasing those of departed units,
/// and keeps their display modes and motions.
void BattleParty_ListAllUnitsAndSubmit(void) {
  s16 list[14];

  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  BattleActor_SpawnObjectsForList(list, 0);
}
