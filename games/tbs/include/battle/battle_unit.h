#ifndef ALCHEMY_BATTLE_UNIT_H
#define ALCHEMY_BATTLE_UNIT_H

#include "edition.h"
#include "static_assert.h"

enum {
  BATTLE_ACTION_SIZE = 0x10,
  BATTLE_UNIT_SIZE = 0x14c,
  OWNER_ACTION_ID_MASK = 0x3fff
};

struct OwnerActionSlot {
  u16 encodedAction;
  u16 unk02;
};

struct BattleElementStats {
  s16 power;
  s16 resistance;
};

/// The elements, in the order of every table kept per element: a unit's
/// element stats, the Djinn and the summons. ELEMENT_NONE marks an ability or
/// a weapon that has no element.
enum Element {
  ELEMENT_VENUS,
  ELEMENT_MERCURY,
  ELEMENT_MARS,
  ELEMENT_JUPITER,
  ELEMENT_NONE,
  NUM_ELEMENTS = ELEMENT_NONE
};

/// BattleAct.range of an ability that reaches every unit on the side, each
/// as if aimed at.
#define ABILITY_RANGE_ALL 255

/// The low four bits of BattleAct.flags: how BattleTargetExec applies
/// the ability's power. Kinds 0, 7 and 9 apply none: 0 is no ability, 7 is
/// Granite, Flash and Kite, 9 the field Psynergy, Defend, Flee and enemies'
/// commands.
#define ABILITY_KIND_MASK 15
enum AbilityKind {
  ABILITY_KIND_HEAL_HP = 1,           ///< Cure, Fizz, healing items
  ABILITY_KIND_EFFECT = 2,            ///< damage only if the effect lands
  ABILITY_KIND_ATTACK = 3,            ///< Attack vs Defense, plus the power
  ABILITY_KIND_ATTACK_MULTIPLIED = 4, ///< that attack times power / 10
  ABILITY_KIND_DAMAGE = 5,            ///< HP damage from the power (Psynergy)
  ABILITY_KIND_UNLEASHED_DAMAGE = 6,  ///< an enemy's. Shown as `X unleashes Y!`
  ABILITY_KIND_SUMMON = 8,
  ABILITY_KIND_PP_DAMAGE = 10,
  ABILITY_KIND_HEAL_PP = 11
};

/// BattleAct.effect: what an ability does besides its damage or healing,
/// applied by BattleTargetExec unless noted. Each is named for the abilities
/// that have it (actions.tsv, with their English names and descriptions).
/// Sidestep (48) and Crazy Voice (52) have effects no code here reads.
enum AbilityEffect {
  ABILITY_EFFECT_NONE = 0,
  ABILITY_EFFECT_CURE_POISON = 3,        ///< Cure Poison, Antidote
  ABILITY_EFFECT_RESTORE = 4,            ///< Restore, Elixir
  ABILITY_EFFECT_REVIVE = 5,             ///< Revive, Water of Life: full HP
  ABILITY_EFFECT_ATTACK_UP_2 = 6,        ///< Impact: two stages
  ABILITY_EFFECT_ATTACK_UP = 7,          ///< High Impact: one stage
  ABILITY_EFFECT_ATTACK_DOWN_2 = 8,      ///< Dull
  ABILITY_EFFECT_ATTACK_DOWN = 9,        ///< Blunt
  ABILITY_EFFECT_DEFENSE_UP_2 = 10,      ///< Guard
  ABILITY_EFFECT_DEFENSE_UP = 11,        ///< Protect
  ABILITY_EFFECT_DEFENSE_DOWN_2 = 12,    ///< Impair
  ABILITY_EFFECT_DEFENSE_DOWN = 13,      ///< Debilitate
  ABILITY_EFFECT_RESISTANCE_UP_2 = 14,   ///< Ward
  ABILITY_EFFECT_RESISTANCE_UP = 15,     ///< Resist
  ABILITY_EFFECT_RESISTANCE_DOWN_2 = 16, ///< Weaken
  ABILITY_EFFECT_RESISTANCE_DOWN = 17,   ///< Enfeeble
  ABILITY_EFFECT_POISON = 18,            ///< Taint
  ABILITY_EFFECT_DEADLY_POISON = 19,     ///< Poison
  ABILITY_EFFECT_DELUSION = 20,          ///< Delude
  ABILITY_EFFECT_CONFUSION = 21,         ///< Confuse
  ABILITY_EFFECT_CHARM = 22,             ///< Charm
  ABILITY_EFFECT_STUN = 23,              ///< Paralyze
  ABILITY_EFFECT_SLEEP = 24,             ///< Sleep
  ABILITY_EFFECT_SEAL = 25,              ///< Bind: Psynergy sealed
  ABILITY_EFFECT_HAUNT = 26,             ///< Haunt
  ABILITY_EFFECT_INSTANT_DEATH = 27,     ///< Condemn
  ABILITY_EFFECT_CURSE = 28,             ///< Curse: the Grim Reaper's countdown
  ABILITY_EFFECT_REGENERATE = 29,        ///< Regenerate
  ABILITY_EFFECT_REFLECT = 30,           ///< Reflect
  ABILITY_EFFECT_DRAIN_HP = 31,          ///< Drain
  ABILITY_EFFECT_DRAIN_PP = 32,          ///< Psy Drain
  ABILITY_EFFECT_BREAK = 33,             ///< Break: raised stats undone
  ABILITY_EFFECT_LEAVE_ONE_HP = 34,      ///< Severe Blow, Thrash
  ABILITY_EFFECT_HALVE_DEFENSE = 35,     ///< Torch
  /// A 1 in 64 to 1 in 4 chance of an ability's critical hit, rolled by
  /// BattleCommand_BuildPlan.
  ABILITY_EFFECT_CRITICAL_1_IN_64 = 36,
  ABILITY_EFFECT_CRITICAL_1_IN_32 = 37,
  ABILITY_EFFECT_CRITICAL_1_IN_16 = 38,
  ABILITY_EFFECT_CRITICAL_1_IN_8 = 39,
  ABILITY_EFFECT_CRITICAL_1_IN_4 = 40,
  /// A chance of a double or triple blow, rolled by BattleCommand_BuildPlan.
  ABILITY_EFFECT_DOUBLE_BLOW_1_IN_8 = 41,
  ABILITY_EFFECT_DOUBLE_BLOW_1_IN_4 = 42, ///< Double Fang, Twin Beaks
  ABILITY_EFFECT_TRIPLE_BLOW_1_IN_8 = 43,
  ABILITY_EFFECT_TRIPLE_BLOW_1_IN_4 = 44, ///< Swift Strike
  ABILITY_EFFECT_THREATEN = 45,           ///< Contain, Threaten, Tremble
  ABILITY_EFFECT_DEFEND = 46,             ///< Defend, Granite
  ABILITY_EFFECT_TOTAL_DEFENSE = 47,      ///< Total Defense, Flash
  ABILITY_EFFECT_FLEE = 49,               ///< Flee
  ABILITY_EFFECT_CALL_ALLY = 50,          ///< Ally Search, Speed Surge
  ABILITY_EFFECT_CALL_HELP = 51,          ///< Search for Help
  ABILITY_EFFECT_GROUND = 53,             ///< Ground: the target's turn lost
  ABILITY_EFFECT_CHALLENGE = 54,          ///< no ability has it
  ABILITY_EFFECT_SELF_DESTRUCT = 55,      ///< Dynamite, Mortal Blow
  ABILITY_EFFECT_REVIVE_HALF = 56,        ///< Quartz
  ABILITY_EFFECT_REVIVE_80 = 57,          ///< Dew: 80% of max HP
  ABILITY_EFFECT_AGILITY_DOWN = 58,       ///< Vine
  ABILITY_EFFECT_AGILITY_UP = 59,         ///< Zephyr
  ABILITY_EFFECT_DRAIN_HALF_HP = 60,      ///< Sap
  ABILITY_EFFECT_HEAL_HP_60 = 61,         ///< Fizz: 60% of max HP
  ABILITY_EFFECT_HEAL_HP_30 = 62,         ///< Spritz: 30%
  ABILITY_EFFECT_HEAL_PP_7 = 63,          ///< Ember: 7% of max PP
  ABILITY_EFFECT_CURE_ALL = 64,           ///< Tonic
  ABILITY_EFFECT_DOUBLE_BLOW_60 = 65,     ///< Gust, Rapid Smash: 153 in 256
  ABILITY_EFFECT_ACT_TWICE = 66,          ///< Kite
  ABILITY_EFFECT_SEAL_2_TURNS = 67,       ///< Luff
  ABILITY_EFFECT_TRIPLE_BLOW_60 = 68,     ///< Sonic Smash
  ABILITY_EFFECT_PSY_LEECH = 69           ///< Psy Leech
};

struct BattleAct {
  u8 targetMode;
  u8 flags;   ///< the kind (ABILITY_KIND_MASK) and where it can be used
  u8 element; ///< enum Element
  u8 effect;  ///< enum AbilityEffect
  u8 unk04[4];
  u8 range; ///< the aimed unit and range - 1 either side, or ABILITY_RANGE_ALL
  u8 ppCost;
  u16 power;
  u8 type0c;
  u8 unk0d[3];
};

enum BattleActionUse {
  ACTION_USABLE_IN_FIELD = 0x40,
  ACTION_USABLE_IN_BATTLE = 0x80
};

struct BattleUnit {
  u8 name[0x0f];
  u8 level;
  s16 baseHp;
  s16 basePp;
  s16 hpGauge;
  s16 ppGauge;
  u16 baseAttack;
  u16 baseDefense;
  u16 baseAgility;
  u8 baseLuck;
  u8 baseActionCount;
  u8 baseHpRegen;
  u8 basePpRegen;
  u8 unk022[2];
  struct BattleElementStats baseElement[4];
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 actionEntryCount;
  u8 hpRegen;
  u8 ppRegen;
  u8 unk046[2];
  struct BattleElementStats element[4];
  struct OwnerActionSlot actionSlots[32];
  u16 inventory[15];
  u8 unk0f6[2];
  u32 djinnAvailable[4];
  u32 djinnActive[4];
  u8 djinnOwnedCounts[4];
  u8 djinnActiveCounts[4];
  u32 rewards;
  u32 experience;
  u8 classId;
  u8 classIndex;
  u8 side;
  s8 guardLevel;
  s8 elementLevels[4];
  s8 restraint;
  s8 poison; ///< 1 poisoned, 2 deadly poison
  /// Each stat change: the turns it has left, and its stage (UnitCalc: an
  /// eighth of Attack, Defense or Agility, or 20 Resistance, a stage).
  u8 attackTurns;
  s8 attackStage;
  u8 defenseTurns;
  s8 defenseStage;
  u8 resistanceTurns;
  s8 resistanceStage;
  /// Delusion to sleep: the turns each status has left.
  u8 delusion;
  s8 confusion;
  u8 charm;
  u8 stun;
  u8 sleep;
  u8 psySeal; ///< Bind's turns in the low three bits, Luff's above
  u8 refrain;
  s8 reflect;
  u8 haunt;      ///< set while an evil spirit grips the unit
  u8 grimReaper; ///< the Grim Reaper's countdown
  /// The totals of equipment effects 23 (unleash chance) and 24 that UnitCalc
  /// sums; TBS never reads them back
  u8 equipBonus[2];
  u8 ready;
  u8 immobile;
  u8 agilityTurns;
  s8 agilityStage;
  s8 challenge;
  u8 unk149;
  u16 enemyId;
};

STATIC_ASSERT(sizeof(struct BattleAct) == BATTLE_ACTION_SIZE);
STATIC_ASSERT(offsetof(struct BattleAct, effect) == 0x03);
STATIC_ASSERT(offsetof(struct BattleAct, power) == 0x0a);
STATIC_ASSERT(offsetof(struct BattleAct, type0c) == 0x0c);

STATIC_ASSERT(sizeof(struct OwnerActionSlot) == 4);
STATIC_ASSERT(sizeof(struct BattleElementStats) == 4);
STATIC_ASSERT(sizeof(struct BattleUnit) == BATTLE_UNIT_SIZE);
STATIC_ASSERT(offsetof(struct BattleUnit, baseHp) == 0x10);
STATIC_ASSERT(offsetof(struct BattleUnit, baseAttack) == 0x18);
STATIC_ASSERT(offsetof(struct BattleUnit, actionSlots) == 0x58);
STATIC_ASSERT(offsetof(struct BattleUnit, djinnAvailable) == 0xf8);
STATIC_ASSERT(offsetof(struct BattleUnit, djinnActive) == 0x108);
STATIC_ASSERT(offsetof(struct BattleUnit, rewards) == 0x120);
STATIC_ASSERT(offsetof(struct BattleUnit, hp) == 0x038);
STATIC_ASSERT(offsetof(struct BattleUnit, baseElement) == 0x024);
STATIC_ASSERT(offsetof(struct BattleUnit, level) == 0x00f);
STATIC_ASSERT(offsetof(struct BattleUnit, element) == 0x048);
STATIC_ASSERT(offsetof(struct BattleUnit, ready) == 0x144);
STATIC_ASSERT(offsetof(struct BattleUnit, inventory) == 0x0d8);
STATIC_ASSERT(offsetof(struct BattleUnit, djinnOwnedCounts) == 0x118);
STATIC_ASSERT(offsetof(struct BattleUnit, djinnActiveCounts) == 0x11c);
STATIC_ASSERT(offsetof(struct BattleUnit, guardLevel) == 0x12b);
STATIC_ASSERT(offsetof(struct BattleUnit, charm) == 0x13a);
STATIC_ASSERT(offsetof(struct BattleUnit, haunt) == 0x140);

STATIC_ASSERT(offsetof(struct BattleUnit, enemyId) == 0x14a);

/// Clamps a gauge ratio to 0..0x4000, where 0x4000 means full.
static __inline__ s32 Vitals_Clamp(s32 ratio) {
  s32 clamped;

  if (ratio > 0x4000)
    clamped = 0x4000;
  else if (ratio < 0)
    clamped = 0;
  else
    clamped = ratio;
  return clamped;
}

/// Recalculates HP and PP gauges, keeping at least one when any points remain.
/// A statement macro, written do { } while (0): every site matches through
/// that shape. Written out at each site, or as an inline function, the
/// gauge loads are scheduled across the store before them.
#define Unit_UpdateGauges(unit)                                                \
  do {                                                                         \
    (unit)->hpGauge = Vitals_Clamp(((unit)->hp << 14) / (unit)->maxHp);        \
    if ((unit)->hpGauge == 0 && (unit)->hp != 0)                               \
      (unit)->hpGauge = 1;                                                     \
    (unit)->ppGauge = Vitals_Clamp(((unit)->pp << 14) / (unit)->maxPp);        \
    if ((unit)->ppGauge == 0 && (unit)->pp != 0)                               \
      (unit)->ppGauge = 1;                                                     \
  } while (0)

#endif
