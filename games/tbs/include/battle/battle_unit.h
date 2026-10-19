#ifndef ALCHEMY_BATTLE_UNIT_H
#define ALCHEMY_BATTLE_UNIT_H

/// @file
/// BattleUnit, a party member's or enemy's battle state, with the elements,
/// abilities and actions it uses.

#include "edition.h"
#include "static_assert.h"

enum {
  BATTLE_ACTION_SIZE = 0x10,
  BATTLE_UNIT_SIZE = 0x14c,
  OWNER_ACTION_ID_MASK = 0x3fff
};

/// An entry in a unit's action list: the action's id (OWNER_ACTION_ID_MASK)
/// and marks for an action its class or an equipped item gave.
struct OwnerActionSlot {
  /// bits: 15 from the class | 14 from an equipped item | 13..0 action id.
  /// Entries with neither mark were taught for good; 0 is empty.
  u16 encodedAction;
  u16 unk02; ///< Not named by any TBS code.
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

/// The kinds of ability, by the low four bits of BattleAct.flags. Three kinds
/// have no enumerator, because BattleTargetExec applies no power for them:
/// 0 is no ability; 7 is Granite, Flash and Kite, whose effect is all they
/// do; 9 is the field Psynergy (Move, Catch...), the field-only stat items
/// (Apple, Power Bread...), Defend, Flee and the enemies' commands (Threaten,
/// Ally Search...). The targeting check treats kind 9 as a summon: worth it
/// on a living unit only when the power is nonzero.
enum AbilityKind {
  ABILITY_KIND_HEAL_HP = 1,           ///< Cure, Fizz, healing items
  ABILITY_KIND_EFFECT = 2,            ///< damage only if the effect lands
  ABILITY_KIND_ATTACK = 3,            ///< Attack vs Defense, plus the power
  ABILITY_KIND_ATTACK_MULTIPLIED = 4, ///< that attack times power / 10
  ABILITY_KIND_DAMAGE = 5,            ///< HP damage from the power (Psynergy)
  ABILITY_KIND_UNLEASHED_DAMAGE = 6,  ///< an enemy's. Shown as `X unleashes Y!`
  ABILITY_KIND_SUMMON = 8,     ///< the summons: damage scaled by distance
  ABILITY_KIND_PP_DAMAGE = 10, ///< Crazy Voice: the target loses PP
  ABILITY_KIND_HEAL_PP = 11    ///< Ember, Psy Crystal, Adept Ring
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

/// An ability's definition, a row of gBattleAction_DefinitionTable: its
/// targeting, kind, element, effect, range, PP cost and power.
struct BattleAct {
  u8 targetMode; ///< TARGET_MODE_NONE, _FOES, _ALLIES or _SELF
  /// bits: 7 usable in battle | 6 usable in the field | 5..4 clear in every
  /// row | 3..0 kind (ABILITY_KIND_MASK, enum AbilityKind).
  u8 flags;
  u8 element; ///< enum Element
  u8 effect;  ///< enum AbilityEffect
  /// [0] the icon the menus draw; [1] to [3] are 0 in every row.
  u8 unk04[4];
  u8 range;  ///< the aimed unit and range - 1 either side, or ABILITY_RANGE_ALL
  u8 ppCost; ///< PP the ability costs
  u16 power; ///< what the kind applies, 0 for none
  /// The field Psynergy the ability works as, 1 (Move) to 16 (Avoid); 0 for
  /// none. The field effects branch on it, and the status screen counts it as
  /// usable in the field.
  u8 type0c;
  u8 unk0d[3]; ///< 0 in every row.
};

/// BattleAct.flags: where an ability can be used.
enum BattleActionUse {
  ACTION_USABLE_IN_FIELD = 0x40, ///< can be used outside battle
  ACTION_USABLE_IN_BATTLE = 0x80 ///< can be used in battle; else greyed there
};

/// A unit in battle: a party member's or an enemy's stats, Psynergy, items,
/// Djinn and ailments.
struct BattleUnit {
  /// The name, one byte a character, ending in 0. An enemy's is its message
  /// name cut to 14 characters, with the suffix letter BattleUnitSet is given
  /// (a digit in Japanese); name[14] is always 0.
  u8 name[0x0f];
  u8 level;        ///< 1 to 99 for a character
  s16 baseHp;      ///< before equipment, Djinn and class (UnitCalc)
  s16 basePp;      ///< before equipment, Djinn and class (UnitCalc)
  s16 hpGauge;     ///< hp / maxHp in 0..0x4000; 1 while any HP remains
  s16 ppGauge;     ///< pp / maxPp in 0..0x4000; 1 while any PP remains
  u16 baseAttack;  ///< before equipment, Djinn, class and stages
  u16 baseDefense; ///< before equipment, Djinn, class and stages
  u16 baseAgility; ///< before equipment, Djinn, class and stages
  u8 baseLuck;     ///< before equipment, Djinn and class
  /// Actions a round before equipment: the low four bits are read. An enemy's
  /// comes from its definition's turns; a level up sets a character's to 1.
  u8 baseActionCount;
  u8 baseHpRegen; ///< HP regained each turn before equipment; 0 for a character
  u8 basePpRegen; ///< PP regained each turn before equipment and lighthouses
  u8 unk022[2];   ///< Not named by any TBS code.
  /// Power and resistance before equipment and stages: an enemy's from its
  /// element preset, a character's from its element levels
  /// (Unit_CalcBaseElements).
  struct BattleElementStats baseElement[4];
  s16 maxHp; ///< 0 to 1999 for a character, 9999 for an enemy (UnitCalc)
  s16 maxPp; ///< 0 to 1999 for a character, 9999 for an enemy (UnitCalc)
  s16 hp;    ///< current HP, 0 when downed
  s16 pp;    ///< current PP
  u16 atk;   ///< Attack, 0 to 999 (UnitCalc)
  u16 def;   ///< Defense, 0 to 999 (UnitCalc)
  u16 agi;   ///< Agility, 0 to 999: the order of the round's actions
  u8 luck;   ///< 0 to 99 (UnitCalc)
  /// Actions each round, 0 to 2: baseActionCount, one more while ready, and
  /// the equipment's extra turns (UnitCalc). Each gives the unit a place in
  /// the round's queue.
  u8 actionEntryCount;
  u8 hpRegen; ///< HP regained at each turn's end (UnitCalc)
  /// PP regained at each turn's end, 0 to 200 (UnitCalc): 4 more once the
  /// lighthouse of the character's element is lit.
  u8 ppRegen;
  u8 unk046[2]; ///< Not named by any TBS code.
  /// Power and resistance in each element, 0 to 200, after equipment,
  /// element levels and Resistance stages (UnitCalc).
  struct BattleElementStats element[4];
  /// The action list: Psynergy taught for good, then the class's and the
  /// equipment's (Unit_RefreshPsynergy). An empty entry is 0.
  struct OwnerActionSlot actionSlots[32];
  /// Items held, one entry each: bits 15..11 quantity | 10 broken |
  /// 9 equipped | 8..0 item id (INVENTORY_*). An empty entry is 0.
  u16 inventory[15];
  u8 unk0f6[2]; ///< Not named by any TBS code.
  /// The Djinn held, one bit per Djinni (by its index) in each element's word.
  u32 djinnAvailable[4];
  /// The Djinn set, one bit per Djinni; only these add their stat bonuses.
  u32 djinnActive[4];
  u8 djinnOwnedCounts[4];  ///< the Djinn held in each element, at most 10
  u8 djinnActiveCounts[4]; ///< the Djinn set in each element; the class's
                           ///< element levels are read from these
  /// Copied from the enemy's definition, where it is 0 in every row. During
  /// a battle the enemy's action choice keeps its place in its action list
  /// here (BattleCommand_SelectAutomatic).
  u32 rewards;
  u32 experience; ///< a character's experience points
  /// Who the unit is: a character's id (ISAAC to JENNA, below 8) or an
  /// enemy's id (BattleUnitSet). The class tables are indexed by classIndex.
  u8 classId;
  /// The character's class, worked out from its Djinn (Unit_UpdateClass); 0
  /// marks an enemy, and code throughout tests it to tell the two apart.
  u8 classIndex;
  /// BATTLE_UNIT_ABSENT (0), BATTLE_UNIT_ENEMY (1) or BATTLE_UNIT_PARTY (2).
  /// Enemies 158 to 171 are put on side 2 (BattleUnitSet).
  u8 side;
  /// 0, GUARD_DEFEND (1, half the damage) or GUARD_TOTAL_DEFENSE (2, a
  /// tenth); set by Defend and Granite, cleared each round.
  s8 guardLevel;
  /// A summon's boost to each element: the Djinn of that element the summon
  /// spent, adding (n * n + n) * 5 power (UnitCalc). Cleared at the battle's
  /// start and by Break.
  s8 elementLevels[4];
  /// The curse. bits: 3 item effect 25 (UnitCalc clears it again with bit 0)
  /// | 2 keeps bit 0 | 1 cursed item equipped | 0 bound. UnitCalc rebuilds
  /// bits 1 and 0 from the equipment. A bound unit loses one action in four,
  /// except Defend (BattleCommand_BuildPlan); any bit set shows the curse in
  /// the status screens and lets the sanctum remove it.
  s8 restraint;
  s8 poison; ///< 1 poisoned, 2 deadly poison
  /// Each stat change: the turns it has left, and its stage (UnitCalc: an
  /// eighth of Attack, Defense or Agility, or 20 Resistance, a stage).
  /// Stages run -4 to 4 and last 7 turns.
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
  u8 refrain; ///< Regenerate's turns left, 7 when cast
  s8 reflect; ///< Reflect's turns left, 7 when cast
  u8 haunt;   ///< set while an evil spirit grips the unit
  /// The Grim Reaper's countdown: 7 when cursed, down one at each turn's
  /// end; at 0 the unit falls.
  u8 grimReaper;
  /// The totals of equipment effects 23 (unleash chance) and 24 that UnitCalc
  /// sums; TBS never reads them back
  u8 equipBonus[2];
  /// Kite's extra action: 2 when cast, down one at each turn's end; while
  /// nonzero UnitCalc gives the unit one more action a round.
  u8 ready;
  u8 immobile;     ///< set by Ground: the next action is lost
  u8 agilityTurns; ///< the Agility change's turns left, 5 when cast
  /// The Agility stage: -4 (Vine) to 8 (Zephyr, twice the Agility).
  s8 agilityStage;
  /// Set by the challenge effect: an enemy with it never chooses to flee.
  s8 challenge;
  u8 unk149;   ///< Not named by any TBS code.
  u16 enemyId; ///< Not named by any TBS code; TLA reads it as the unit's id.
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
// A statement macro, written do { } while (0): every site matches through that
// shape. Written out at each site, or as an inline function, the gauge loads
// are scheduled across the store before them.
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
