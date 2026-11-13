#ifndef ALCHEMY_ENEMY_ELEMENT_PRESET_H
#define ALCHEMY_ENEMY_ELEMENT_PRESET_H

/// @file
/// EnemyElementPreset, a row of an enemy's element presets.

#include "types.h"
#include "battle_unit.h"

/// An enemy's element preset in gEnemy_ElementPresetTable: the element it
/// attacks with, its level in each element, and its base power and resistance
/// in each element.
struct EnemyElementPreset {
  s32 first; ///< the element it attacks with (Enemy_GetAttackElement)
  /// Its level in each element; Unit_GetElementLevels returns ten times each.
  u8 elementLevels[4];
  /// Its base power and resistance in each element, as
  /// Unit_CalculateBaseElements gives them to the unit.
  struct BattleElementStats stats[4];
};

#endif
