#ifndef ALCHEMY_BATTLE_UNIT_H
#define ALCHEMY_BATTLE_UNIT_H

/// @file
/// Unit, a party member's or enemy's battle state, with the elements,
/// abilities and actions it uses.

#include "edition.h"
#include "static_assert.h"

/// The sizes the STATIC_ASSERTs below hold the structs to, and the
/// action id's bits in an action list entry.
enum {
  ABILITY_DEFINITION_SIZE = 0x10, ///< bytes in a struct AbilityDefinition
  /// Bytes in a struct Unit, which the code clears and copies whole.
  BATTLE_UNIT_SIZE = 0x14c,
  PSYNERGY_ABILITY_MASK = 0x3fff ///< PsynergyEntry.encodedAction's action id
};

/// An entry in a unit's action list: the action's id (PSYNERGY_ABILITY_MASK)
/// and marks for an action its class or an equipped item gave.
struct PsynergyEntry {
  /// bits: 15 from the class | 14 from an equipped item | 13..0 action id.
  /// Entries with neither mark were taught for good; 0 is empty.
  u16 encodedAction;
  u16 unused; ///< no code names it
};

/// A unit's elemental power and resistance in one element.
struct BattleElementStats {
  s16 power;      ///< Elemental power in the element.
  s16 resistance; ///< Elemental resistance to it.
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
  ELEMENT_COUNT = ELEMENT_NONE
};

/// AbilityDefinition.range of an ability that reaches every unit on the side,
/// each as if aimed at.
#define ABILITY_RANGE_ALL 255

/// The low four bits of AbilityDefinition.flags: the ability kind (enum
/// AbilityFormula).
#define ABILITY_FORMULA_MASK 15

/// An ability kind picks what the ability's power does to each target: the
/// formula BattleAbility_ApplyToTarget puts it through. ABILITY_FORMULA_BLANK,
/// ABILITY_FORMULA_EFFECT_ONLY and ABILITY_FORMULA_NO_POWER apply no power. No
/// row has a kind above ABILITY_FORMULA_HEAL_PP, which would apply none either.
enum AbilityFormula {
  ABILITY_FORMULA_BLANK = 0,             ///< only the blank rows no table uses
  ABILITY_FORMULA_HEAL_HP = 1,           ///< Cure, Fizz, healing items
  ABILITY_FORMULA_EFFECT = 2,            ///< damage only if the effect lands
  ABILITY_FORMULA_ATTACK = 3,            ///< Attack vs Defense, plus the power
  ABILITY_FORMULA_ATTACK_MULTIPLIED = 4, ///< that attack times power / 10
  ABILITY_FORMULA_DAMAGE = 5, ///< HP damage from the power (Psynergy)
  /// HP damage from enemies' moves such as Fire Blessing and Deadly Gas,
  /// falling off faster than Psynergy's (gRangeFalloff_EnemyDamage). Unless the
  /// move has wording of its own, it's announced as `<unit> unleashes
  /// <ability>!`. A weapon's unleash is something else: ABILITY_FORMULA_ATTACK,
  /// or for Rapid Smash ABILITY_FORMULA_ATTACK_MULTIPLIED.
  ABILITY_FORMULA_ENEMY_DAMAGE = 6,
  /// Granite, Flash and Kite, whose effect is all they do.
  ABILITY_FORMULA_EFFECT_ONLY = 7,
  /// Summon damage falls off faster than Psynergy's
  /// (gRangeFalloff_SummonDamage). Called with the Summon command, a summon
  /// also adds 3% of the target's maximum HP to its power for each Djinni it
  /// spends; the Fusion Dragon's Outer Space and Dragon Driver go without that
  /// bonus.
  ABILITY_FORMULA_SUMMON = 8,
  /// The field Psynergy (Move, Catch...), the field-only stat items (Apple,
  /// Power Bread...), Defend, Flee and the enemies' commands (Threaten, Ally
  /// Search...). The enemies' targeting check (CHECK_EFFECT_TARGET) treats it
  /// as a summon: worth it on a living unit only when the power is nonzero.
  ABILITY_FORMULA_NO_POWER = 9,
  ABILITY_FORMULA_PP_DAMAGE = 10, ///< Crazy Voice, and Psy Drain when it lands
  ABILITY_FORMULA_HEAL_PP = 11    ///< Ember, Psy Crystal, Adept Ring
};

/// AbilityDefinition.effect: what an ability does besides its damage or
/// healing, applied by BattleAbility_ApplyToTarget unless noted. Each is named
/// for the abilities that have it (abilities.tsv, with their English names and
/// descriptions). ABILITY_EFFECT_SIDESTEP and ABILITY_EFFECT_CRAZY_VOICE are
/// read by no code here, and no row has ABILITY_EFFECT_RESTORE_HP or
/// ABILITY_EFFECT_RESTORE_PP. A stat change whose name ends in `_2` moves its
/// stat two stages, one without moves it one.
enum AbilityEffect {
  ABILITY_EFFECT_NONE = 0,
  ABILITY_EFFECT_RESTORE_HP = 1,         ///< restores HP (the item menu's use)
  ABILITY_EFFECT_RESTORE_PP = 2,         ///< restores PP (the item menu's use)
  ABILITY_EFFECT_CURE_POISON = 3,        ///< Cure Poison, Antidote
  ABILITY_EFFECT_RESTORE = 4,            ///< Restore, Elixir
  ABILITY_EFFECT_REVIVE = 5,             ///< Revive, Water of Life: full HP
  ABILITY_EFFECT_ATTACK_UP_2 = 6,        ///< Impact
  ABILITY_EFFECT_ATTACK_UP = 7,          ///< High Impact
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
  ABILITY_EFFECT_VENOM = 19,             ///< Poison
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
  /// BattleTurn_Build. Only an attack (ABILITY_FORMULA_ATTACK or
  /// ABILITY_FORMULA_ATTACK_MULTIPLIED) applies it, and no ability has one.
  ABILITY_EFFECT_CRITICAL_1_IN_64 = 36,
  ABILITY_EFFECT_CRITICAL_1_IN_32 = 37,
  ABILITY_EFFECT_CRITICAL_1_IN_16 = 38,
  ABILITY_EFFECT_CRITICAL_1_IN_8 = 39,
  ABILITY_EFFECT_CRITICAL_1_IN_4 = 40,
  /// A chance of a double or triple blow, rolled by BattleTurn_Build.
  ABILITY_EFFECT_DOUBLE_BLOW_1_IN_8 = 41,
  ABILITY_EFFECT_DOUBLE_BLOW_1_IN_4 = 42, ///< Double Fang, Twin Beaks
  ABILITY_EFFECT_TRIPLE_BLOW_1_IN_8 = 43,
  ABILITY_EFFECT_TRIPLE_BLOW_1_IN_4 = 44, ///< Swift Strike
  ABILITY_EFFECT_THREATEN = 45,           ///< Contain, Threaten, Tremble
  ABILITY_EFFECT_DEFEND = 46,             ///< Defend, Granite
  ABILITY_EFFECT_TOTAL_DEFENSE = 47,      ///< Total Defense, Flash
  ABILITY_EFFECT_SIDESTEP = 48,           ///< Sidestep
  ABILITY_EFFECT_FLEE = 49,           ///< Flee: an enemy's becomes COMMAND_FLEE
  ABILITY_EFFECT_CALL_ALLY = 50,      ///< Ally Search, Speed Surge
  ABILITY_EFFECT_CALL_HELP = 51,      ///< Search for Help
  ABILITY_EFFECT_CRAZY_VOICE = 52,    ///< Crazy Voice
  ABILITY_EFFECT_GROUND = 53,         ///< Ground, War Cry, Wicked Howl
  ABILITY_EFFECT_CHALLENGE = 54,      ///< no ability has it
  ABILITY_EFFECT_SELF_DESTRUCT = 55,  ///< Dynamite, Mortal Blow
  ABILITY_EFFECT_REVIVE_HALF = 56,    ///< Quartz
  ABILITY_EFFECT_REVIVE_80 = 57,      ///< Dew: 80% of max HP
  ABILITY_EFFECT_AGILITY_DOWN = 58,   ///< Vine
  ABILITY_EFFECT_AGILITY_UP = 59,     ///< Zephyr
  ABILITY_EFFECT_DRAIN_HALF_HP = 60,  ///< Sap
  ABILITY_EFFECT_HEAL_HP_60 = 61,     ///< Fizz: 60% of max HP
  ABILITY_EFFECT_HEAL_HP_30 = 62,     ///< Spritz: 30%
  ABILITY_EFFECT_HEAL_PP_7 = 63,      ///< Ember: 7% of max PP
  ABILITY_EFFECT_CURE_ALL = 64,       ///< Tonic
  ABILITY_EFFECT_DOUBLE_BLOW_60 = 65, ///< Gust, Rapid Smash: 153 in 256
  ABILITY_EFFECT_ACT_TWICE = 66,      ///< Kite
  ABILITY_EFFECT_SEAL_2_TURNS = 67,   ///< Luff
  ABILITY_EFFECT_TRIPLE_BLOW_60 = 68, ///< Sonic Smash
  ABILITY_EFFECT_PSY_LEECH = 69       ///< Psy Leech
};

/// AbilityDefinition.targetMode: whom an ability may target, from its user's
/// view. The party's cursor (BattleTarget_RunSelection) shows the enemies for
/// every mode but ABILITY_TARGET_ALLIES and ABILITY_TARGET_SELF. No row of
/// abilities.tsv has a mode between ABILITY_TARGET_ALLIES and
/// ABILITY_TARGET_SELF or above it; BattleTurn_SetTargets would walk such a
/// mode's range as it does ABILITY_TARGET_FOES'.
enum AbilityTarget {
  /// The ability takes one target, the aimed unit, whatever its range
  /// (BattleTurn_SetTargets), and an enemy aims it at itself. The field
  /// Psynergy and enemies' commands such as Threaten, Ally Search and Flee
  /// have it.
  ABILITY_TARGET_ONE = 0,
  ABILITY_TARGET_FOES = 1,
  ABILITY_TARGET_ALLIES = 2, ///< the user's side, the user included
  ABILITY_TARGET_SELF = 4    ///< the user alone, whatever the range
};

/// Unit.curse bits, the curse.
enum CurseFlag {
  CURSE_BOUND = 0x01,      ///< the unit is bound
  CURSE_ITEM = 0x02,       ///< a cursed item is equipped
  CURSE_KEEP_BOUND = 0x04, ///< keeps CURSE_BOUND through Unit_Calculate
  /// EQUIP_EFFECT_CURSE_REMOVABLE: Unit_Calculate clears it again with
  /// CURSE_BOUND.
  CURSE_REMOVABLE = 0x08
};

/// An ability's definition, a row of gAbilityDefinitions: its
/// targeting, ability kind, element, effect, range, PP cost and power.
struct AbilityDefinition {
  u8 targetMode; ///< enum AbilityTarget
  /// bits: 7 usable in battle | 6 usable in the field | 5..4 clear in every
  /// row | 3..0 ability kind (ABILITY_FORMULA_MASK).
  u8 flags;
  u8 element;              ///< enum Element
  u8 effect;               ///< enum AbilityEffect
  u8 icon;                 ///< the icon the menus draw
  u8 reservedAfterIcon[3]; ///< 0 in every row
  u8 range;  ///< the aimed unit and range - 1 either side, or ABILITY_RANGE_ALL
  u8 ppCost; ///< PP the ability costs
  /// The ability's power, as its ability kind reads it: for damage, about what
  /// the aimed target loses when the actor's Power in the element equals the
  /// target's Resist (a summon adds to it, see ABILITY_FORMULA_SUMMON); for
  /// healing, about what the aimed target regains from a healer with 100
  /// Power in the element; for an attack, what it adds to the blow at an even
  /// match, and for a multiplied one the blow's multiple in tenths. The battle
  /// README's "How an ability resolves" gives the formulas.
  u16 power;
  /// The field Psynergy the ability works as, 1 (Move) to 16 (Avoid); 0 for
  /// none. The field effects branch on it, and the summary screen counts it as
  /// usable in the field.
  u8 fieldPsynergy;
  u8 reservedAfterType[3]; ///< 0 in every row
};

/// AbilityDefinition.flags: where an ability can be used.
enum AbilityUse {
  ABILITY_USABLE_IN_FIELD = 0x40, ///< can be used outside battle
  ABILITY_USABLE_IN_BATTLE = 0x80 ///< can be used in battle; else greyed there
};

/// Unit.classId of an enemy, which has no class of its own.
#define CLASS_NONE 0

/// Unit.poison: the poison takes a tenth of the maximum HP at the end of
/// each turn, deadly poison a fifth.
#define POISON_NORMAL 1
#define POISON_VENOM 2

/// Unit.guardLevel, 0 for no guard: Defend and Granite halve the damage,
/// Total Defense and Flash cut it to a tenth (REDUCE_BY_GUARD, target_exec.c).
/// It lasts the round: BattleUnit_ResetGuardLevels clears it as the next
/// one's commands are gathered.
#define GUARD_DEFEND 1
#define GUARD_TOTAL_DEFENSE 2

/// A unit in battle: a party member's or an enemy's stats, Psynergy, items,
/// Djinn and statuses.
struct Unit {
  /// The name, one byte a character, ending in 0. An enemy's is its message
  /// name cut to 14 characters, with the suffix letter BattleUnit_Set is given
  /// (a digit in Japanese); name[14] is always 0.
  u8 name[0x0f];
  u8 level;        ///< 1 to 99 for a character
  s16 baseHp;      ///< before equipment, Djinn and class (Unit_Calculate)
  s16 basePp;      ///< before equipment, Djinn and class (Unit_Calculate)
  s16 hpGauge;     ///< hp / hpMax in 0..0x4000; 1 while any HP remains
  s16 ppGauge;     ///< pp / ppMax in 0..0x4000; 1 while any PP remains
  u16 baseAttack;  ///< before equipment, Djinn, class and stages
  u16 baseDefense; ///< before equipment, Djinn, class and stages
  u16 baseAgility; ///< before equipment, Djinn, class and stages
  u8 baseLuck;     ///< before equipment, Djinn and class
  /// Actions a round before equipment: the low four bits are read. An enemy's
  /// comes from its definition's turns; a level up sets a character's to 1.
  u8 baseActionCount;
  u8 baseHpRegen; ///< HP regained each turn before equipment; 0 for a character
  u8 basePpRegen; ///< PP regained each turn before equipment and lighthouses
  u8 baseUnused[2]; ///< no code names it
  /// Power and resistance before equipment and stages: an enemy's from its
  /// element preset, a character's from its element levels
  /// (Unit_CalculateBaseElements).
  struct BattleElementStats baseElement[ELEMENT_COUNT];
  s16 hpMax; ///< 0 to 1999 for a character, 9999 for an enemy (Unit_Calculate)
  s16 ppMax; ///< 0 to 1999 for a character, 9999 for an enemy (Unit_Calculate)
  s16 hp;    ///< current HP, 0 when downed
  s16 pp;    ///< current PP
  u16 atk;   ///< Attack, 0 to 999 (Unit_Calculate)
  u16 def;   ///< Defense, 0 to 999 (Unit_Calculate)
  u16 agi;   ///< Agility, 0 to 999: the order of the round's actions
  u8 luck;   ///< 0 to 99 (Unit_Calculate)
  /// Actions each round, 0 to 2: baseActionCount, one more while ready, and
  /// the equipment's extra turns (Unit_Calculate). Each gives the unit a place
  /// in the round's queue.
  u8 actionsPerRound;
  u8 hpRegen; ///< HP regained at each turn's end (Unit_Calculate)
  /// PP regained at each turn's end, 0 to 200 (Unit_Calculate): 4 more once the
  /// lighthouse of the character's element is lit.
  u8 ppRegen;
  u8 unused[2]; ///< no code names it, as baseUnused
  /// Power and resistance in each element, 0 to 200, after equipment, a
  /// summon's boost and Resistance stages (Unit_Calculate).
  struct BattleElementStats element[ELEMENT_COUNT];
  /// The action list: Psynergy taught for good, then the class's and the
  /// equipment's (Unit_RefreshPsynergy). An empty entry is 0.
  struct PsynergyEntry psynergy[32];
  /// Items held, one entry each: bits 15..11 quantity | 10 broken |
  /// 9 equipped | 8..0 item id (INVENTORY_*). An empty entry is 0.
  u16 inventory[15];
  u8 paddingBeforeDjinn[2]; ///< up to the word djinnHeld starts on
  /// The Djinn held, one bit per Djinni (by its index) in each element's word.
  u32 djinnHeld[ELEMENT_COUNT];
  /// The Djinn set, one bit per Djinni; only these add their stat bonuses.
  u32 djinnSet[ELEMENT_COUNT];
  u8 djinnHeldCounts[ELEMENT_COUNT]; ///< the Djinn held in each element, at
                                     ///< most 10
  u8 djinnSetCounts[ELEMENT_COUNT];  ///< the Djinn set in each element; the
                                     ///< class's element levels are read from
                                     ///< these
  /// Copied from the enemy's definition, where it is 0 in every row. During
  /// a battle the enemy's action choice keeps its place in its action list
  /// here (BattleAI_ChooseAction).
  u32 aiActionIndex;
  u32 experience; ///< a character's experience points
  /// Who the unit is: a character's id (ISAAC to JENNA, below 8) or an
  /// enemy's id (BattleUnit_Set). The class tables are indexed by classIndex.
  u8 id;
  /// The character's class, worked out from its Djinn (Unit_UpdateClass);
  /// CLASS_NONE marks an enemy, and code throughout tests it to tell
  /// the two apart.
  u8 classId;
  /// enum BattleSide. The Djinn the party fights are enemies put on
  /// BATTLE_SIDE_PARTY (BattleUnit_Set).
  u8 side;
  /// GUARD_DEFEND or GUARD_TOTAL_DEFENSE while the unit guards, else 0.
  s8 guardLevel;
  /// A summon's boost to its summoner's power in each element: at the end of
  /// the summoner's turn, the element the summon spent the most Djinn of
  /// rises to that many (BattleUnit_ProcessTurnEnd), and n adds
  /// (n * n + n) * 5 power (Unit_Calculate). Cleared at the battle's start, on
  /// going down and by Break.
  s8 summonBoost[ELEMENT_COUNT];
  /// The curse: CurseFlag bits. Unit_Calculate rebuilds CURSE_BOUND and
  /// CURSE_ITEM from the equipment. A bound unit loses one action in
  /// four, except Defend (BattleTurn_Build); any bit set shows the curse
  /// in the summary screens and lets the sanctum remove it.
  s8 curse;
  s8 poison; ///< POISON_NORMAL or POISON_VENOM, 0 for none
  /// Each stat change: the turns it has left, and its stage (Unit_Calculate: an
  /// eighth of Attack, Defense or Agility, or 20 Resistance, a stage).
  /// Stages run -4 to 4 and last 7 turns; a lowered one may recover sooner at
  /// the end of a turn.
  ///@{
  u8 attackTurns;
  s8 attackStage;
  u8 defenseTurns;
  s8 defenseStage;
  u8 resistanceTurns;
  s8 resistanceStage;
  ///@}
  /// Delusion to sleep: the turns each status has left, 7 when it lands; each
  /// may end sooner at the end of a turn. Sleep, confusion, charm and stun
  /// cost the unit its actions.
  ///@{
  u8 delusion;
  s8 confusion;
  u8 charm;
  u8 stun;
  u8 sleep;
  ///@}
  /// bits: 7..3 Luff (16 when cast, minus 8 a turn) | 2..0 Bind's turns
  /// (7 when cast). Psynergy is sealed while any bit is set.
  u8 psySeal;
  u8 regenerate; ///< Regenerate's turns left, 7 when cast
  s8 reflect;    ///< Reflect's turns left, 7 when cast
  u8 haunt;      ///< set while an evil spirit grips the unit
  /// The Grim Reaper's countdown: 7 when cursed, down one at each turn's
  /// end; at 0 the unit falls.
  u8 grimReaper;
  /// The totals of EQUIP_EFFECT_ADD_UNLEASH_CHANCE and EQUIP_EFFECT_24 that
  /// Unit_Calculate sums; TBS never reads them back
  u8 equipBonus[2];
  /// Kite's extra action: 2 when cast, down one at each turn's end; while
  /// nonzero Unit_Calculate gives the unit one more action a round.
  u8 ready;
  u8 immobile;     ///< set by ABILITY_EFFECT_GROUND: the next action is lost
  u8 agilityTurns; ///< the Agility change's turns left, 5 when cast
  /// The Agility stage: -4 (Vine) to 8 (Zephyr, twice the Agility).
  s8 agilityStage;
  /// Set by the challenge effect: an enemy with it never chooses to flee.
  s8 challenge;
  u8 paddingBeforeEnemyId; ///< up to the halfword enemyId starts on
  u16 enemyId; ///< Not named by any TBS code; TLA reads it as the unit's id.
};

STATIC_ASSERT(sizeof(struct AbilityDefinition) == ABILITY_DEFINITION_SIZE);
STATIC_ASSERT(offsetof(struct AbilityDefinition, effect) == 0x03);
STATIC_ASSERT(offsetof(struct AbilityDefinition, power) == 0x0a);
STATIC_ASSERT(offsetof(struct AbilityDefinition, fieldPsynergy) == 0x0c);

STATIC_ASSERT(sizeof(struct PsynergyEntry) == 4);
STATIC_ASSERT(sizeof(struct BattleElementStats) == 4);
STATIC_ASSERT(sizeof(struct Unit) == BATTLE_UNIT_SIZE);
STATIC_ASSERT(offsetof(struct Unit, baseHp) == 0x10);
STATIC_ASSERT(offsetof(struct Unit, baseAttack) == 0x18);
STATIC_ASSERT(offsetof(struct Unit, psynergy) == 0x58);
STATIC_ASSERT(offsetof(struct Unit, djinnHeld) == 0xf8);
STATIC_ASSERT(offsetof(struct Unit, djinnSet) == 0x108);
STATIC_ASSERT(offsetof(struct Unit, aiActionIndex) == 0x120);
STATIC_ASSERT(offsetof(struct Unit, hp) == 0x038);
STATIC_ASSERT(offsetof(struct Unit, baseElement) == 0x024);
STATIC_ASSERT(offsetof(struct Unit, level) == 0x00f);
STATIC_ASSERT(offsetof(struct Unit, element) == 0x048);
STATIC_ASSERT(offsetof(struct Unit, ready) == 0x144);
STATIC_ASSERT(offsetof(struct Unit, inventory) == 0x0d8);
STATIC_ASSERT(offsetof(struct Unit, djinnHeldCounts) == 0x118);
STATIC_ASSERT(offsetof(struct Unit, djinnSetCounts) == 0x11c);
STATIC_ASSERT(offsetof(struct Unit, guardLevel) == 0x12b);
STATIC_ASSERT(offsetof(struct Unit, charm) == 0x13a);
STATIC_ASSERT(offsetof(struct Unit, haunt) == 0x140);

STATIC_ASSERT(offsetof(struct Unit, enemyId) == 0x14a);

/// Clamps a gauge ratio to 0..0x4000, where 0x4000 means full.
static __inline__ s32 Gauge_Clamp(s32 ratio) {
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
// A statement macro, written do { } while (0): every site matches through that
// shape. Written out at each site, or as an inline function, the gauge loads
// are scheduled across the store before them.
#define Unit_UpdateGauges(unit)                                                \
  do {                                                                         \
    (unit)->hpGauge = Gauge_Clamp(((unit)->hp << 14) / (unit)->hpMax);         \
    if ((unit)->hpGauge == 0 && (unit)->hp != 0)                               \
      (unit)->hpGauge = 1;                                                     \
    (unit)->ppGauge = Gauge_Clamp(((unit)->pp << 14) / (unit)->ppMax);         \
    if ((unit)->ppGauge == 0 && (unit)->pp != 0)                               \
      (unit)->ppGauge = 1;                                                     \
  } while (0)

#endif
