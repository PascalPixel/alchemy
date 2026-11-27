#ifndef ALCHEMY_FIRST_STRIKE_H
#define ALCHEMY_FIRST_STRIKE_H

// Who strikes first as a battle opens.

#include "types.h"

// BATTLE_STATE.firstStrike: rolled as a battle opens when nothing set its start
// (MainRoutineStateWork.battleType 0), and set back to normal after the first turn (btRun).
typedef enum BATTLE_FIRST_STRIKE
{
    FIRST_STRIKE_NONE    = 0,
    // 1 in 16, and always in the Djinn tutorial: no enemy acts in the first turn, and running away
    // cannot fail.
    FIRST_STRIKE_PARTY   = 1,
    // 1 in 32 of the rest: in the first turn the party chooses no commands, and at most 2 to 5
    // enemies, drawn at random, act once each (btBuildEnemyActions). A failed flee sets it for the
    // rest of its turn.
    FIRST_STRIKE_ENEMIES = 2
} BATTLE_FIRST_STRIKE;

#endif
