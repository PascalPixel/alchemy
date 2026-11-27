#ifndef ALCHEMY_LUCKY_DICE_STUBS_H
#define ALCHEMY_LUCKY_DICE_STUBS_H

/* @file */
/* The stub calls into the lucky dice module, as the other modules' code calls
 */
/* them. */

#include "types.h"

/* Plays Lucky Dice until the player quits, and returns the entrance the field
 */
/* takes up again by, always 0; the entrance it is given is not read. */
s32 fdLuckyDiceStartStub(s32 entrance);

#endif
