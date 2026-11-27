#ifndef ALCHEMY_FIRST_STRIKE_H
#define ALCHEMY_FIRST_STRIKE_H

// Who strikes first as a battle opens.

#include "types.h"

// BATTLE_STATE.firstStrike: who strikes first as a battle opens.
typedef enum BATTLE_FIRST_STRIKE
{
	FIRST_STRIKE_NONE    = 0,
	// No enemy acts in the first turn, and running away cannot fail.
	FIRST_STRIKE_PARTY   = 1,
	// In the first turn the party chooses no commands. A failed flee sets it for the rest of its
	// turn.
	FIRST_STRIKE_ENEMIES = 2
} BATTLE_FIRST_STRIKE;

#endif
