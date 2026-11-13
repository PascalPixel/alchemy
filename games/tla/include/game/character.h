#ifndef ALCHEMY_CHARACTER_H
#define ALCHEMY_CHARACTER_H

/// @file
/// The definitions of characters, Djinn and enemies, as The Broken Seal's
/// character.h has them; an enemy's lacks the ten bytes The Broken Seal's
/// keeps before its items.

#include "types.h"
#include "static_assert.h"

/// A character's definition: the growth curves of its stats, its element
/// bonus, starting level and starting equipment.
struct CharacterDefinition {
  u8 reserved[80]; ///< twenty words, 0 in every row
  /// The growth curves: each stat at levels 0, 20, 40, 60, 80 and 100.
  ///@{
  s16 hp[6];
  s16 pp[6];
  u16 atk[6];
  u16 def[6];
  u16 agi[6];
  u8 luck[6];
  ///@}
  /// Added to ten times the Djinn set in each element to give the
  /// character's element levels.
  u8 elementBonus[4];
  u8 startingLevel;      ///< the level a new game raises the character to
  u8 paddingBeforeItems; ///< padding
  /// The starting items, each added and equipped at a new game; the low nine
  /// bits are the item.
  u16 items[13];
  u8 paddingAfterItems[2]; ///< padding
};

STATIC_ASSERT(sizeof(struct CharacterDefinition) == 0xb4);
STATIC_ASSERT(offsetof(struct CharacterDefinition, hp) == 0x50);
STATIC_ASSERT(offsetof(struct CharacterDefinition, elementBonus) == 0x92);
STATIC_ASSERT(offsetof(struct CharacterDefinition, items) == 0x98);

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

/// An enemy's definition: its level and stats, items, element preset, the
/// actions it chooses from and its rewards.
struct EnemyDefinition {
  u8 name[15]; ///< blank in every row; the name comes from the messages
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
  u8 itemActions;    ///< one bit per place in abilities: use an item instead
  u16 abilities[8];  ///< the abilities the enemy chooses from
  u8 weakEffects[4]; ///< the ability effects the enemy is weak to
  u16 coins;         ///< coins won
  s16 item;          ///< the item it may leave, 0 for none
  s16 itemChance;    ///< the chance of the item, 0 for none
  u16 experience;    ///< experience won
  u8 padding[2];     ///< padding
};

STATIC_ASSERT(sizeof(struct EnemyDefinition) == 76);
STATIC_ASSERT(offsetof(struct EnemyDefinition, items) == 0x1e);
STATIC_ASSERT(offsetof(struct EnemyDefinition, abilities) == 0x2e);
STATIC_ASSERT(offsetof(struct EnemyDefinition, coins) == 0x42);
STATIC_ASSERT(offsetof(struct EnemyDefinition, experience) == 0x48);

/// An enemy's element preset in gEnemy_ElementPresetTable: the element it
/// attacks with, its level in each element, and its base power and resistance
/// in each.
struct EnemyElementPreset {
  s32 first;    ///< the element it attacks with (enum Element)
  u8 digits[4]; ///< its level in each element, in tens
  struct {
    s16 power;
    s16 resistance;
  } values[4]; ///< each element's base power and resistance
};

STATIC_ASSERT(sizeof(struct EnemyElementPreset) == 24);

struct LevelUpResult {
  s16 level;
  u16 ability;
  u16 hp;
  u16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u16 luck;
};

#endif
