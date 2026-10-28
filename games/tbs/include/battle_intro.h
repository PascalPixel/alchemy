#ifndef ALCHEMY_BATTLE_INTRO_H
#define ALCHEMY_BATTLE_INTRO_H

/// @file
/// Who strikes first as a battle opens.

#include "types.h"

/// BattleWork.firstStrike: rolled as a battle opens when nothing set its
/// start (gAdventure.battleType 0), and set back to normal after the
/// first round (Battle_Run).
enum BattleFirstStrike {
  FIRST_STRIKE_NONE = 0,
  /// 1 in 16, and always in the Djinn tutorial: no enemy acts in the first
  /// round, and running away cannot fail.
  FIRST_STRIKE_PARTY = 1,
  /// 1 in 32 of the rest: in the first round the party chooses no commands,
  /// and at most 2 to 5 enemies, drawn at random, act once each
  /// (Battle_BuildOpponentActions). A failed flee sets it for the
  /// rest of its round.
  FIRST_STRIKE_ENEMIES = 2
};

#endif
