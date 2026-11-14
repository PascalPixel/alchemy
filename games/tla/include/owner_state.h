#ifndef ALCHEMY_OWNER_STATE_H
#define ALCHEMY_OWNER_STATE_H

#include "types.h"
#include "battle/battle_unit.h"
#include "static_assert.h"

struct OwnerInventoryState {
  u8 unk000[0xd8];
  u16 inventory[15];
  u8 unk0f6[0x54];
  u16 classId;
};

struct OwnerActionState {
  u8 unk000[0x58];
  struct OwnerActionSlot actionSlots[32];
};

union OwnerDjinnFlags {
  struct {
    u32 owned[4];
    u32 active[4];
  } banks;
  u32 slots[8];
};

union OwnerDjinnCounts {
  struct {
    u8 owned[4];
    u8 active[4];
  } banks;
  u8 slots[8];
};

struct OwnerDjinnState {
  u8 unk000[0xf8];
  union OwnerDjinnFlags flags;
  union OwnerDjinnCounts counts;
};

enum { TRADE_OFFER_PENDING = -1 };

/// A Djinni on standby or recovering, as ☀️'s DjinnRecoveryEntry: its element
/// and index, its owner and its status.
struct TradeOffer {
  u8 element;
  u8 djinn; ///< the Djinni within its element
  u8 owner;
  s8 status; ///< TRADE_OFFER_PENDING (-1) on standby; -2 is tested apart
};

/// One side's Djinn recovery table, as Djinn_GetRecoveryTable returns it: the
/// party's inside gMainRoutineWork from byte 0xc, the enemies' in unit 0x83.
struct TradeOfferState {
  /// The summons the side can call, bit n for summon n (summon_table.c).
  u32 summonMask;
  u8 unusedAfterSummons[4]; ///< the party's runs over its coins
  struct TradeOffer offers[72];
  u32 count; ///< offers in use
};

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
  CLASS_SERIES_FLAME_USER = 13, ///< Jenna's, where the others are Guards
  CLASS_SERIES_MARINER,         ///< Piers's, where the others are Water Seers
  CLASS_SERIES_PIERROT,         ///< with the Mysterious Card equipped
  CLASS_SERIES_TAMER,           ///< with the Trainer's Whip equipped
  CLASS_SERIES_DARK_MAGE        ///< with the Tomegathericon equipped
};

// The caps on a unit's level and stats (Unit_Calculate).
#define UNIT_LEVEL_MAX 99
#define UNIT_ENEMY_HP_MAX 16383    ///< maximum HP, an enemy's
#define UNIT_HP_MAX 9999           ///< maximum PP, an enemy's
#define UNIT_CHARACTER_HP_MAX 1999 ///< maximum HP and PP, a character's
#define UNIT_STAT_MAX 999          ///< Attack, Defense and Agility
#define UNIT_LUCK_MAX 99
#define UNIT_ELEMENT_MAX 200 ///< elemental power and resistance
#define UNIT_HP_REGEN_MAX 10000
#define UNIT_PP_REGEN_MAX 200

/// A full HP or PP gauge (BattleUnit.hpGauge, ppGauge): the gauge is the
/// current value over the maximum in 2.14 fixed point.
#define UNIT_GAUGE_SHIFT 14
#define UNIT_GAUGE_FULL (1 << UNIT_GAUGE_SHIFT)

/// How many actions a unit's list holds (BattleUnit.psynergy), and how
/// many a class teaches (ClassDefinition.abilities).
#define PSYNERGY_SLOTS 32
#define CLASS_ABILITY_SLOTS 16

/// Unit ids from 128 are the enemies', as The Broken Seal's.
#define FIRST_ENEMY_UNIT 128

/// How many classes gClass_DefinitionTable holds.
#define CLASS_COUNT 244

/// A class: its series, the element levels it needs, its stat multipliers and
/// the abilities it teaches.
struct ClassDefinition {
  s32 series;                        ///< the series Class_Determine picks from
  u8 required[4];                    ///< each element's level needed, in tens
  u8 hp;                             ///< tenths of the base stat
  u8 pp;                             ///< tenths of the base stat
  u8 atk;                            ///< tenths of the base stat
  u8 def;                            ///< tenths of the base stat
  u8 agi;                            ///< tenths of the base stat
  u8 luck;                           ///< tenths of the base stat
  u8 padding[2];                     ///< padding
  struct ClassAbility abilities[16]; ///< the Psynergy the class teaches
  u8 weakEffects[4]; ///< the ability effects the class is weak to
};

extern const struct ClassDefinition gClass_DefinitionTable[];

STATIC_ASSERT(sizeof(struct ClassDefinition) == 0x54);
STATIC_ASSERT(offsetof(struct OwnerDjinnState, flags.banks.owned) == 0xf8);
STATIC_ASSERT(offsetof(struct OwnerDjinnState, flags.banks.active) == 0x108);
STATIC_ASSERT(offsetof(struct OwnerDjinnState, counts.banks.owned) == 0x118);
STATIC_ASSERT(offsetof(struct OwnerDjinnState, counts.banks.active) == 0x11c);
STATIC_ASSERT(sizeof(struct TradeOffer) == 4);
STATIC_ASSERT(offsetof(struct TradeOfferState, offers) == 8);
STATIC_ASSERT(offsetof(struct TradeOfferState, count) == 0x128);

void *Unit_Get(s32 owner);
void Unit_UpdateClass(s32 owner);
struct ClassDefinition *Class_GetDefinition(s32 classId);
s32 Owner_SumDjinnCounts(s32 owner, s32 element);

const u16 *Djinn_GetDefinition(u32 element, u32 djinn);
u16 Djinn_GetDefinitionHeader(u32 element, u32 djinn);
s32 Djinn_Give(s32 owner, s32 element, s32 djinn);
s32 Djinn_GiveToParty(s32 element, s32 djinn);
u32 Djinn_IsSet(s32 owner, s32 element, s32 djinn);
s32 Djinn_Set(s32 owner, s32 element, s32 djinn);
u32 Djinn_PutOnStandby(s32 owner, s32 element, s32 djinn);
s32 Djinn_Transfer(s32 source, s32 element, s32 djinn, s32 target);

struct TradeOfferState *Djinn_GetRecoveryTable(s32 which);
u32 *DjinnRecovery_AddStandby(u32 owner, u32 element, u32 djinn);
s32 DjinnRecovery_Remove(s32 owner, s32 element, s32 djinn);
s32 DjinnRecovery_CountStandby(u8 *counts);

#endif
