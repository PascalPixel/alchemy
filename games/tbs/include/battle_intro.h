#ifndef ALCHEMY_BATTLE_INTRO_H
#define ALCHEMY_BATTLE_INTRO_H

/// @file
/// Who strikes first as a battle opens.

#include "types.h"

/// BattleWork.encounterMode: rolled as a battle opens when nothing set its
/// start (gGameState.battleStart 0), and set back to normal after the first
/// round (Battle_RunEncounter).
enum BattleEncounterAdvantage {
  BATTLE_ENCOUNTER_NORMAL = 0,
  /// 1 in 16, and always in the Djinn tutorial: no enemy acts in the first
  /// round, and running away cannot fail.
  BATTLE_ENCOUNTER_PARTY_FIRST = 1,
  /// 1 in 32 of the rest: in the first round the party chooses no commands,
  /// and at most 2 to 5 enemies, drawn at random, act once each
  /// (BattlePres_BuildOpponentEntries). A failed escape sets it for the rest
  /// of its round.
  BATTLE_ENCOUNTER_ENEMIES_FIRST = 2
};

#endif
