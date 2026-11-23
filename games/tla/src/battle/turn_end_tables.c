/// @file
/// The order the two sides take their turn-end effects in, and two tables
/// after it that no code names.

#include "fixed_math.h"
#include "battle/battle_side.h"
#include "battle/presentation.h"
#include "system/matrix.h"

/// The party takes its turn-end effects first, then the enemies; the second
/// console of a link battle swaps them (Battle_ProcessTurnEnd).
const struct BattleSideOrder gBattleParty_TurnEndSideOrder = {
    {BATTLE_SIDE_MASK_PARTY, BATTLE_SIDE_MASK_ENEMIES},
};

/// Unused: the numbers 0 to 15 in order.
const u8 gBattle_UnusedCounting[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};

// clang-format off
/// Unused: a unit matrix.
const MATRIX gBattle_UnusedUnitMatrix = {
    FIXED_ONE, 0,         0,
    0,         FIXED_ONE, 0,
    0,         0,         FIXED_ONE,
    0,         0,         0,
};
// clang-format on
