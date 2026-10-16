//--------------------------------------------------------------------------
// actor_commit_placement.c
// Battle: place objects for all units.
//--------------------------------------------------------------------------

#include "types.h"
#include "battle.h"
#include "battle_party.h"
#include "battle_display.h"

//--------------------------------------------------------------------------
// Prepare all units' battle graphics, releasing those of departed units,
// and reset everyone's display mode and motion.
//--------------------------------------------------------------------------
void BattleObjPut(void) {
  s16 list[14];

  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  BattleActor_SpawnObjectsForList(list, 1);
}

//--------------------------------------------------------------------------

// list_all_units_and_submit.c

// Battle: place objects for all units.

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Prepare battle graphics for all units, releasing those of departed units.
//--------------------------------------------------------------------------
void BattleParty_ListAllUnitsAndSubmit(void) {
  s16 list[14];

  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  BattleActor_SpawnObjectsForList(list, 0);
}
