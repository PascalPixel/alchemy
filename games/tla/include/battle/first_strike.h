#ifndef ALCHEMY_FIRST_STRIKE_H
#define ALCHEMY_FIRST_STRIKE_H

/// @file
/// Who strikes first as a battle opens.

#include "types.h"

/// BattleState.firstStrike: who strikes first as a battle opens.
enum BattleFirstStrike {
  FIRST_STRIKE_NONE = 0,
  /// No enemy acts in the first turn, and running away cannot fail.
  FIRST_STRIKE_PARTY = 1,
  /// In the first turn the party chooses no commands. A failed flee sets it
  /// for the rest of its turn.
  FIRST_STRIKE_ENEMIES = 2
};

#endif
