#ifndef ALCHEMY_FAR_CALL_LUCKY_DICE_H
#define ALCHEMY_FAR_CALL_LUCKY_DICE_H

/// @file
/// The far calls into the lucky dice bank, as the other banks' code calls them.

#include "types.h"

/// Plays Lucky Dice until the player quits, and returns the entrance the field
/// takes up again by, always 0; the entrance it is given is not read.
s32 LuckyDice_StartFar(s32 entrance);

#endif
