#ifndef ALCHEMY_CLASS_DJINN_H
#define ALCHEMY_CLASS_DJINN_H

/// @file
/// Classes and Djinn as a unit holds them: the class definitions, the caps on
/// a unit's stats, and the routines that set, give and recover Djinn.

#include "types.h"
#include "battle/battle_unit.h"
#include "battle/battle_side.h"
#include "static_assert.h"

/// An ability a class teaches, and the level it is learned at; The Broken
/// Seal's holds the ability in one byte.
struct ClassAbility {
  u16 ability; ///< the action taught, 0 for none
  u8 level;    ///< the level it is taught at
  u8 padding;  ///< padding
};

/// ClassDefinition.series, each named by its first class. The ten The Broken
/// Seal has are the series a character's elements choose
/// (gClass_SeriesByElements); 11 is in no row, 12 holds class 0, "NPC", an
/// enemy's (CLASS_NONE), and the five after are The Lost Age's own, which
/// Class_Determine gives for a character or an item.
enum ClassSeries {
  CLASS_SERIES_SQUIRE = 1,
  CLASS_SERIES_GUARD,
  CLASS_SERIES_SWORDSMAN,
  CLASS_SERIES_BRUTE,
  CLASS_SERIES_APPRENTICE,
  CLASS_SERIES_WATER_SEER,
  CLASS_SERIES_WIND_SEER,
  CLASS_SERIES_SEER,
  CLASS_SERIES_PILGRIM,
  CLASS_SERIES_HERMIT,
  /// Class 0, "NPC", an enemy's (CLASS_NONE).
  CLASS_SERIES_NPC = 12,
  CLASS_SERIES_FLAME_USER, ///< Jenna's, where the others are Guards
  CLASS_SERIES_MARINER,    ///< Piers's, where the others are Water Seers
  CLASS_SERIES_PIERROT,    ///< with the Mysterious Card equipped
  CLASS_SERIES_TAMER,      ///< with the Trainer's Whip equipped
  CLASS_SERIES_DARK_MAGE   ///< with the Tomegathericon equipped
};

/// The caps on a unit's level and stats (Unit_Calculate).
#define UNIT_LEVEL_MAX 99
#define UNIT_ENEMY_HP_MAX 16383    ///< maximum HP, an enemy's
#define UNIT_ENEMY_PP_MAX 9999     ///< maximum PP, an enemy's
#define UNIT_CHARACTER_HP_MAX 1999 ///< maximum HP and PP, a character's
#define UNIT_STAT_MAX 999          ///< Attack, Defense and Agility
#define UNIT_LUCK_MAX 99
#define UNIT_ELEMENT_MAX 200 ///< elemental power and resistance
#define UNIT_HP_REGEN_MAX 10000
#define UNIT_PP_REGEN_MAX 200

/// How many actions a class can teach (ClassDefinition.abilities).
#define CLASS_ABILITY_SLOTS 16

/// How many classes gClass_DefinitionTable holds.
#define CLASS_COUNT 244

/// A class: its series, the element levels it needs, its stat multipliers and
/// the abilities it teaches.
struct ClassDefinition {
  long series;    ///< the series Class_Determine picks from
  u8 required[4]; ///< the whole level needed in each element
  u8 hp;          ///< tenths of the base stat
  u8 pp;          ///< tenths of the base stat
  u8 attack;      ///< tenths of the base stat
  u8 defense;     ///< tenths of the base stat
  u8 agility;     ///< tenths of the base stat
  u8 luck;        ///< tenths of the base stat
  u8 padding[2];  ///< padding
  /// The Psynergy the class teaches.
  struct ClassAbility abilities[CLASS_ABILITY_SLOTS];
  u8 weakEffects[4]; ///< the ability effects the class is weak to
};

extern const struct ClassDefinition gClass_DefinitionTable[];

STATIC_ASSERT(sizeof(struct ClassDefinition) == 84);
STATIC_ASSERT(offsetof(struct DjinnRecoveryTable, list) == 8);
STATIC_ASSERT(offsetof(struct DjinnRecoveryTable, list.count) == 296);

struct Unit *Unit_Get(u32 unitId);
void Unit_UpdateClass(s32 owner);
struct ClassDefinition *Class_GetDefinition(s32 classId);
s32 Owner_SumDjinnCounts(s32 owner, s32 element);

const struct DjinnDefinition *Djinn_GetDefinition(u32 element, u32 djinn);
u16 Djinn_GetAbilityId(u32 element, u32 djinn);
s32 Djinn_Give(s32 owner, s32 element, s32 djinn);
s32 Djinn_GiveToParty(s32 element, s32 djinn);
u32 Djinn_IsSet(s32 owner, s32 element, s32 djinn);
s32 Djinn_Set(s32 owner, s32 element, s32 djinn);
u32 Djinn_PutOnStandby(s32 owner, s32 element, s32 djinn);
s32 Djinn_Transfer(s32 source, s32 element, s32 djinn, s32 target);

struct DjinnRecoveryTable *Djinn_GetRecoveryTable(s32 enemySide);
u32 *DjinnRecovery_AddStandby(u32 owner, u32 element, u32 djinn);
s32 DjinnRecovery_Remove(s32 owner, s32 element, s32 djinn);
s32 DjinnRecovery_CountStandby(u8 *counts);

#endif
