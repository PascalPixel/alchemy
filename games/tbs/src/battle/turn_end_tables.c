/// @file
/// The order the two sides take their turn-end effects in, and two tables
/// after it that no code names.

#include "battle/battle.h"
#include "battle/battle_side.h"
#include "fixed_math.h"
#include "system/matrix.h"

const struct BattleSideOrder gBattle_TurnEndSideOrder = {
    {BATTLE_SIDE_MASK_PARTY, BATTLE_SIDE_MASK_ENEMIES},
};

/// Unused: the numbers 0 to 15 in order.
const u8 gBattle_UnusedCounting[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};

/// Unused: a unit matrix.
// clang-format off
const MATRIX gBattle_UnusedUnitMatrix = {
    FIXED_ONE, 0,         0,
    0,         FIXED_ONE, 0,
    0,         0,         FIXED_ONE,
    0,         0,         0,
};
// clang-format on
