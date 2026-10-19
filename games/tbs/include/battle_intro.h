#ifndef ALCHEMY_BATTLE_INTRO_H
#define ALCHEMY_BATTLE_INTRO_H

/// @file
/// Who strikes first as a battle opens.

#include "types.h"

enum BattleEncounterAdvantage {
  BATTLE_ENCOUNTER_NORMAL = 0,
  BATTLE_ENCOUNTER_PARTY_FIRST = 1,
  BATTLE_ENCOUNTER_ENEMIES_FIRST = 2
};

#endif
