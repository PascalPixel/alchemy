#ifndef ALCHEMY_LUCKY_DICE_STUBS_H
#define ALCHEMY_LUCKY_DICE_STUBS_H

// The stub calls into the lucky dice module, as the other modules' code calls them.

#include "types.h"

// Plays Lucky Dice until the player quits, and returns the entrance the field takes up again by,
// always 0; the entrance it is given is not read.
Sint32 fdLuckyDiceStartStub(Sint32 entrance);

#endif
