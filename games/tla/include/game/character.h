#ifndef ALCHEMY_CHARACTER_H
#define ALCHEMY_CHARACTER_H

/// @file
/// The definitions of characters, Djinn and enemies, as The Broken Seal's
/// character.h has them; an enemy's lacks the ten bytes The Broken Seal's
/// keeps before its items.

#include "types.h"
#include "static_assert.h"
#include "battle/battle_unit.h"

/// Levels between two points of a growth curve.
#define GROWTH_BAND_LEVELS 20
/// Points on a growth curve: levels 0 to 100, GROWTH_BAND_LEVELS apart.
#define GROWTH_POINTS 6

/// A character's definition: the growth curves of its stats, its element
/// bonus, starting level and starting equipment.
struct CharacterDefinition {
  u8 reserved[80]; ///< twenty words, 0 in every row
  /// The growth curves: each stat at levels 0, 20, 40, 60, 80 and 100.
  ///@{
  s16 hp[GROWTH_POINTS];
  s16 pp[GROWTH_POINTS];
  u16 atk[GROWTH_POINTS];
  u16 def[GROWTH_POINTS];
  u16 agi[GROWTH_POINTS];
  u8 luck[GROWTH_POINTS];
  ///@}
  /// The character's own level in each element before any Djinn, in tenths
  /// of a level (54 is level 5.4); each Djinni set adds a whole level.
  u8 elementBonus[4];
  u8 startingLevel;      ///< the level a new game raises the character to
  u8 paddingBeforeItems; ///< padding
  /// The starting items, each added and equipped at a new game; the low nine
  /// bits are the item.
  u16 items[13];
  u8 paddingAfterItems[2]; ///< padding
};

STATIC_ASSERT(sizeof(struct CharacterDefinition) == 180);
STATIC_ASSERT(offsetof(struct CharacterDefinition, hp) == 80);
STATIC_ASSERT(offsetof(struct CharacterDefinition, elementBonus) == 146);
STATIC_ASSERT(offsetof(struct CharacterDefinition, items) == 152);

/// A Djinni: its ability and the stat bonuses it gives while set.
struct DjinnDefinition {
  u16 abilityId;              ///< the action the Djinni unleashes
  u8 reservedAfterAbility[2]; ///< 0 in every row
  /// Added to the base stats while the Djinni is set, before the class
  /// multiplies them.
  ///@{
  s8 hp;
  s8 pp;
  s8 atk;
  s8 def;
  s8 agi;
  s8 luck;
  ///@}
  u8 reservedAfterStats[2]; ///< 0 in every row
};

STATIC_ASSERT(sizeof(struct DjinnDefinition) == 12);

/// EnemyDefinition.targetStrategy: a single-target attacker favours the
/// target with the most HP left, the most maximum HP, or no one. The same
/// column steers the action choice: with any strategy but
/// TARGET_STRATEGY_MOST_HP an enemy skips Psynergy it lacks the PP for, and
/// with TARGET_STRATEGY_RANDOM any Psynergy while sealed.
enum EnemyTargetStrategy {
  TARGET_STRATEGY_MOST_HP = 0,
  TARGET_STRATEGY_MOST_MAX_HP = 1,
  TARGET_STRATEGY_RANDOM = 2
};

/// EnemyDefinition.actionStrategy: how BattleAI_ChooseAction picks a
/// place in the enemy's eight actions. The first three roll against
/// gBattleAI_ActionWeights; the next two remember the place in the unit's
/// AI state.
enum EnemyActionStrategy {
  ACTION_STRATEGY_EVEN,           ///< 32 in 256 each
  ACTION_STRATEGY_FALLING,        ///< from 53 in 256 down to 11
  ACTION_STRATEGY_FIRST_FAVOURED, ///< the first 113 in 256, down to 5
  /// A random place the first time, then each in turn at the enemy's turn.
  ACTION_STRATEGY_RANDOM_START,
  ACTION_STRATEGY_IN_ORDER,  ///< each in turn from the remembered place
  ACTION_STRATEGY_NEXT_PICK, ///< the next place at each new pick, from 0
  ACTION_STRATEGY_SAME       ///< the place last picked
};

/// An enemy's definition: its level and stats, items, element preset, the
/// actions it chooses from and its rewards.
struct EnemyDefinition {
  u8 name[UNIT_NAME_SIZE]; ///< blank in every row; the name comes from the
                           ///< messages
  u8 level;
  /// The base stats copied into the unit.
  ///@{
  u16 hp;
  u16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 actionCount; ///< actions a turn
  u8 hpRegen;     ///< HP restored at each turn's end
  u8 ppRegen;     ///< PP restored at each turn's end
  ///@}
  u16 items[4];      ///< items the enemy carries, 0 for none
  u8 counts[4];      ///< how many of each item it carries
  u8 elementPreset;  ///< row of gEnemy_ElementPresetTable
  s8 targetStrategy; ///< whom a single-target attack favours
  s8 actionStrategy; ///< how the action is chosen from abilities
  /// One bit per place in `abilities` (ITEM_ACTION): use an item held instead.
  u8 itemActions;
  u16 abilities[8];  ///< the abilities the enemy chooses from
  u8 weakEffects[4]; ///< the ability effects the enemy is weak to
  u16 coins;         ///< coins won
  s16 item;          ///< the item it may leave, 0 for none
  /// The chance of the item, as a power of two: 1 in 2 to the power n - 1 (1
  /// always, 6 one in 32); 0 for none.
  s16 itemChance;
  u16 experience; ///< experience won
  u8 padding[2];  ///< padding
};

STATIC_ASSERT(sizeof(struct EnemyDefinition) == 76);
STATIC_ASSERT(offsetof(struct EnemyDefinition, items) == 30);
STATIC_ASSERT(offsetof(struct EnemyDefinition, abilities) == 46);
STATIC_ASSERT(offsetof(struct EnemyDefinition, coins) == 66);
STATIC_ASSERT(offsetof(struct EnemyDefinition, experience) == 72);
/// EnemyDefinition.itemActions' bit for `place` in its abilities.
#define ITEM_ACTION(place) (1 << (place))

/// An enemy's element preset in gEnemy_ElementPresetTable: the element it
/// attacks with, its level in each element, and its base power and resistance
/// in each.
struct EnemyElementPreset {
  s32 first; ///< the element it attacks with (Enemy_GetAttackElement)
  /// Its level in each element; Unit_GetElementLevels returns ten times each.
  u8 elementLevels[4];
  /// Its base power and resistance in each element, as
  /// Unit_CalculateBaseElements gives them to the unit.
  struct BattleElementStats stats[4];
};

/// How many element presets gEnemy_ElementPresetTable holds; an enemy naming
/// one past the last reads the first.
#define ENEMY_ELEMENT_PRESET_COUNT 48

STATIC_ASSERT(sizeof(struct EnemyElementPreset) == 24);

/// A level up's result: the level reached and the gain in each stat. The
/// caller provides it; Unit_LevelUp fills it, and
/// Battle_AwardSpoils reports each gain that is not zero.
struct LevelUpResult {
  s16 level;
  /// Set to -1 and read by nothing: the battle finds an ability learned by
  /// comparing the unit's Psynergy before and after.
  u16 ability;
  u16 hp;
  u16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u16 luck;
};

#endif
