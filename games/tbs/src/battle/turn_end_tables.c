/* The order the two sides take their turn-end effects in, and two tables after
   it that no code names. */

#include "battle/battle_side.h"
#include "system.h"

const BATTLE_SIDE_ORDER Battle_TurnEndSideOrder = {
	{BATTLE_SIDE_MASK_PARTY, BATTLE_SIDE_MASK_ENEMIES},
};

/* Unused: the numbers 0 to 15 in order. */
const Uint8 Battle_UnusedCounting[] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
};

/* Unused: a unit matrix. */
/* clang-format off */
const MATRIX Battle_UnusedUnitMatrix = {
	toFIXED(1), 0,         0,
	0,         toFIXED(1), 0,
	0,         0,         toFIXED(1),
	0,         0,         0,
};
/* clang-format on */
