#ifndef ALCHEMY_ENEMY_ELEMENT_PRESET_H
#define ALCHEMY_ENEMY_ELEMENT_PRESET_H

/// @file
/// EnemyElementPreset, a row of an enemy's element presets.

#include "types.h"

/// An enemy's element preset in gEnemy_ElementPresetTable: the element it
/// attacks with, its level in each element, and its base power and resistance
/// in each element as packed halfword pairs.
struct EnemyElementPreset {
  s32 first; ///< the element it attacks with (Enemy_GetAttackElement)
  /// Its level in each element; Unit_GetElementLevels returns ten times each.
  u8 digits[4];
  /// Each element's struct BattleElementStats as one word: power in the low
  /// halfword, resistance in the high (Unit_CalculateBaseElements).
  s32 values[4];
};

/// One of EnemyElementPreset.values: an element's base power and resistance.
#define PRESET_STATS(power, resist) (((resist) << 16) | (power))

#endif
