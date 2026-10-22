#ifndef ALCHEMY_CHARACTER_H
#define ALCHEMY_CHARACTER_H

/// @file
/// The definitions of characters, classes, Djinn and enemies, and a level up's
/// result.

#include "unit/game/game.h"
#include "types.h"
#include "owner_value.h"
#include "static_assert.h"

struct BattleUnit;

/// A character's definition: the growth curves of its stats, its element
/// bonus, starting level and starting equipment.
struct CharacterDefinition {
  u8 reserved[0x50]; ///< twenty words, 0 in every row; read by no code
  /// The growth curves: each stat at levels 0, 20, 40, 60, 80 and 100. A
  /// level up adds a twentieth of the step around the new level, with a
  /// random part; level 1 adds the first point (Unit_LevelUp).
  ///@{
  s16 hp[6];
  s16 pp[6];
  u16 atk[6];
  u16 def[6];
  u16 agi[6];
  u8 luck[6];
  ///@}
  /// Added to ten times the Djinn set in each element to give the
  /// character's element levels (Unit_GetElementLevels).
  u8 elementBonus[4];
  u8 startingLevel; ///< the level a new game raises the character to
  u8 unk097;        ///< Padding in chars.tsv.
  /// The starting items, each added and equipped at a new game; the low nine
  /// bits are the item.
  u16 items[13];
  u8 unk0b2[2]; ///< Padding in chars.tsv.
};

/// An ability a class teaches, and the level it is learned at.
struct ClassAbility {
  u8 ability;  ///< the action taught, 0 for none
  u8 level;    ///< the level it is taught at
  u8 unk02[2]; ///< Padding in classes.tsv.
};

/// A class: its series, the element levels it needs, its stat multipliers and
/// the abilities it teaches.
struct ClassDefinition {
  /// The series the class belongs to; Class_Determine picks the series from
  /// the character's two strongest elements.
  long series;
  /// The element levels the class needs, in tens: the last class of the
  /// series the character meets in all four is its class.
  u8 required[4];
  u8 hp;                             ///< tenths of the base stat
  u8 pp;                             ///< tenths of the base stat
  u8 atk;                            ///< tenths of the base stat
  u8 def;                            ///< tenths of the base stat
  u8 agi;                            ///< tenths of the base stat
  u8 luck;                           ///< tenths of the base stat
  u8 unk0e[2];                       ///< Padding in classes.tsv.
  struct ClassAbility abilities[16]; ///< the Psynergy the class teaches
  /// [0] to [2]: the ability effects the class is weak to
  /// (BattleTarget_IsWeakToEffect); [3] is 0 in every row and not read.
  u8 weakEffects[4];
};

/// A Djinni: its ability and the stat bonuses it gives while set.
struct DjinnDefinition {
  u16 abilityId; ///< the action the Djinni unleashes
  u8 unk02[2];   ///< Reserved in djinn.tsv; not named by any TBS code.
  /// Added to the base stats while the Djinni is set (UnitCalc), before the
  /// class multiplies them.
  ///@{
  s8 hp;
  s8 pp;
  s8 atk;
  s8 def;
  s8 agi;
  s8 luck;
  ///@}
  u8 unk0a[2]; ///< Reserved in djinn.tsv.
};

/// EnemyDefinition.targetStrategy: a single-target attacker favours the
/// target with the most HP left, the most maximum HP, or no one
/// (BattleTarget_SelectForAction). The same column steers
/// BattleCommand_SelectAutomatic: with any strategy but STRATEGY_MOST_HP an
/// enemy skips Psynergy it lacks the PP for, and with STRATEGY_RANDOM any
/// Psynergy while sealed; with STRATEGY_MOST_HP, an enemy that picks an item
/// place with no items left still commands COMMAND_ITEM.
enum EnemyTargetStrategy {
  STRATEGY_MOST_HP = 0,
  STRATEGY_MOST_MAX_HP = 1,
  STRATEGY_RANDOM = 2
};

/// EnemyDefinition.actionStrategy: how BattleCommand_SelectAutomatic picks a
/// place in the enemy's eight actions. The first three roll against
/// gBattleCommand_SelectionWeights; the next two remember the place in
/// BattleUnit.rewards. No row of enemies.tsv has a strategy past
/// ACTION_STRATEGY_RANDOM_START.
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
  /// The enemy's name: enemies.tsv's name column, 15 bytes, blank in every
  /// row. BattleUnitSet takes the name from the messages instead.
  u8 unk00[15];
  u8 level;
  /// The base stats BattleUnitSet copies into the unit.
  ///@{
  u16 hp;
  u16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 turns;   ///< actions a round (BattleUnit.baseActionCount)
  u8 hpRegen; ///< HP restored at each round's end (BattleUnit.baseHpRegen)
  u8 ppRegen; ///< PP restored at each round's end (BattleUnit.basePpRegen)
  ///@}
  u8 unk1e[2];      ///< Reserved in enemies.tsv: 0 in every row.
  u32 rewards;      ///< 0 in every row; copied to BattleUnit.rewards
  u8 unk24[4];      ///< Reserved in enemies.tsv: 0 in every row.
  u16 items[4];     ///< items the enemy carries, 0 for none
  u8 counts[4];     ///< how many of each item it carries
  u8 elementPreset; ///< row of gEnemy_ElementPresetTable, 0 to 43
  /// Whom a single-target attack favours: an enum EnemyTargetStrategy.
  s8 targetStrategy;
  /// How the action is chosen from `actions` (BattleCommand_SelectAutomatic):
  /// an enum EnemyActionStrategy.
  s8 actionStrategy;
  u8 itemActions; ///< one bit per place in `actions`: use an item held instead
  u16 actions[8]; ///< the abilities the enemy chooses from
  /// [0] to [2]: the ability effects the enemy is weak to
  /// (BattleTarget_IsWeakToEffect); [3] is 0 in every row.
  u8 weakEffects[4];
  u16 coins; ///< coins won
  s16 item;  ///< the item it may leave, 0 for none
  /// The chance of the item: 1 in 2 to the power itemChance - 1, two powers
  /// better with BattleEnemy_RecordDefeat's Djinn bonus; 0 for none.
  s16 itemChance;
  u16 experience; ///< experience won
};

/// A level up's result: the level reached and the gain in each stat. The
/// caller provides it; Unit_LevelUp fills it, and
/// Battle_AwardSpoils reports each gain that is not zero.
struct LevelUpResult {
  s16 level; ///< the level reached; the old level when it was already 99
  /// Always 0xffff: Unit_LevelUp stores no ability here, and the spoils find
  /// a new one by comparing the unit's actions with a copy taken before.
  u16 ability;
  /// The gain in each base stat; the spoils read them as signed.
  ///@{
  u16 hp;
  u16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u16 luck;
  ///@}
};

STATIC_ASSERT(sizeof(struct CharacterDefinition) == 0xb4);
STATIC_ASSERT(offsetof(struct CharacterDefinition, hp) == 0x50);
STATIC_ASSERT(offsetof(struct CharacterDefinition, startingLevel) == 0x96);
STATIC_ASSERT(offsetof(struct CharacterDefinition, items) == 0x98);
STATIC_ASSERT(sizeof(struct ClassDefinition) == 0x54);
STATIC_ASSERT(offsetof(struct ClassDefinition, hp) == 8);
STATIC_ASSERT(sizeof(struct ClassAbility) == 4);
STATIC_ASSERT(offsetof(struct ClassDefinition, abilities) == 0x10);
STATIC_ASSERT(sizeof(struct DjinnDefinition) == 0xc);
STATIC_ASSERT(offsetof(struct DjinnDefinition, abilityId) == 0);
STATIC_ASSERT(offsetof(struct DjinnDefinition, unk02) == 2);
STATIC_ASSERT(offsetof(struct DjinnDefinition, hp) == 4);
STATIC_ASSERT(sizeof(struct EnemyDefinition) == 0x54);
STATIC_ASSERT(offsetof(struct EnemyDefinition, rewards) == 0x20);
STATIC_ASSERT(offsetof(struct EnemyDefinition, actionStrategy) == 0x36);
STATIC_ASSERT(offsetof(struct EnemyDefinition, itemActions) == 0x37);
STATIC_ASSERT(offsetof(struct EnemyDefinition, actions) == 0x38);
STATIC_ASSERT(offsetof(struct EnemyDefinition, coins) == 0x4c);
STATIC_ASSERT(offsetof(struct EnemyDefinition, experience) == 0x52);
STATIC_ASSERT(sizeof(struct LevelUpResult) == 0x10);

/// The eight characters' definitions, by character id (game/chars.tsv).
extern struct CharacterDefinition gCharacter_DefinitionTable[];
/// The classes, by class index (game/classes.tsv, 203 rows).
extern struct ClassDefinition gClass_DefinitionTable[];

#endif
