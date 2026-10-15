#ifndef ALCHEMY_BATTLE_UNIT_H
#define ALCHEMY_BATTLE_UNIT_H

#include "edition.h"
#include "layout_guard.h"

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
  s16 res;
};

// The elements, in the order of every table kept per element: a unit's
// element stats, the Djinn and the summons. ELEMENT_NONE marks an ability or
// a weapon that has no element.
enum Element {
  ELEMENT_VENUS,
  ELEMENT_MERCURY,
  ELEMENT_MARS,
  ELEMENT_JUPITER,
  ELEMENT_NONE,
  NUM_ELEMENTS = ELEMENT_NONE
};

// BattleAct.range of an ability that reaches every unit on the side, each
// as if aimed at.
#define ABILITY_RANGE_ALL 255

// The low four bits of BattleAct.targetFlag: how BattleTargetExec applies
// the ability's power. Kinds 0, 7 and 9 apply none: 0 is no ability, 7 is
// Granite, Flash and Kite, 9 the field Psynergy, Defend, Flee and enemies'
// commands.
#define ABILITY_KIND_MASK 15
enum AbilityKind {
  ABILITY_KIND_HEAL_HP = 1,           // Cure, Fizz, healing items
  ABILITY_KIND_EFFECT = 2,            // damage only if the effect lands
  ABILITY_KIND_ATTACK = 3,            // Attack vs Defense, plus the power
  ABILITY_KIND_ATTACK_MULTIPLIED = 4, // that attack times power / 10
  ABILITY_KIND_DAMAGE = 5,            // HP damage from the power (Psynergy)
  ABILITY_KIND_UNLEASHED_DAMAGE = 6,  // an enemy's: "X unleashes Y!"
  ABILITY_KIND_SUMMON = 8,
  ABILITY_KIND_PP_DAMAGE = 10,
  ABILITY_KIND_HEAL_PP = 11
};

// BattleAct.effect: what an ability does besides its damage or healing,
// applied by BattleTargetExec unless noted. Each is named for the abilities
// that have it (actions.tsv, with their English names and descriptions).
// Sidestep (48) and Crazy Voice (52) have effects no code here reads.
enum AbilityEffect {
  ABILITY_EFFECT_NONE = 0,
  ABILITY_EFFECT_CURE_POISON = 3,        // Cure Poison, Antidote
  ABILITY_EFFECT_RESTORE = 4,            // Restore, Elixir
  ABILITY_EFFECT_REVIVE = 5,             // Revive, Water of Life: full HP
  ABILITY_EFFECT_ATTACK_UP_2 = 6,        // Impact: two stages
  ABILITY_EFFECT_ATTACK_UP = 7,          // High Impact: one stage
  ABILITY_EFFECT_ATTACK_DOWN_2 = 8,      // Dull
  ABILITY_EFFECT_ATTACK_DOWN = 9,        // Blunt
  ABILITY_EFFECT_DEFENSE_UP_2 = 10,      // Guard
  ABILITY_EFFECT_DEFENSE_UP = 11,        // Protect
  ABILITY_EFFECT_DEFENSE_DOWN_2 = 12,    // Impair
  ABILITY_EFFECT_DEFENSE_DOWN = 13,      // Debilitate
  ABILITY_EFFECT_RESISTANCE_UP_2 = 14,   // Ward
  ABILITY_EFFECT_RESISTANCE_UP = 15,     // Resist
  ABILITY_EFFECT_RESISTANCE_DOWN_2 = 16, // Weaken
  ABILITY_EFFECT_RESISTANCE_DOWN = 17,   // Enfeeble
  ABILITY_EFFECT_POISON = 18,            // Taint
  ABILITY_EFFECT_DEADLY_POISON = 19,     // Poison
  ABILITY_EFFECT_DELUSION = 20,          // Delude
  ABILITY_EFFECT_CONFUSION = 21,         // Confuse
  ABILITY_EFFECT_CHARM = 22,             // Charm
  ABILITY_EFFECT_STUN = 23,              // Paralyze
  ABILITY_EFFECT_SLEEP = 24,             // Sleep
  ABILITY_EFFECT_SEAL = 25,              // Bind: Psynergy sealed
  ABILITY_EFFECT_HAUNT = 26,             // Haunt
  ABILITY_EFFECT_INSTANT_DEATH = 27,     // Condemn
  ABILITY_EFFECT_CURSE = 28,             // Curse: the Grim Reaper's countdown
  ABILITY_EFFECT_REGENERATE = 29,        // Regenerate
  ABILITY_EFFECT_REFLECT = 30,           // Reflect
  ABILITY_EFFECT_DRAIN_HP = 31,          // Drain
  ABILITY_EFFECT_DRAIN_PP = 32,          // Psy Drain
  ABILITY_EFFECT_BREAK = 33,             // Break: raised stats undone
  ABILITY_EFFECT_LEAVE_ONE_HP = 34,      // Severe Blow, Thrash
  ABILITY_EFFECT_HALVE_DEFENSE = 35,     // Torch
  // A 1 in 64 to 1 in 4 chance of an ability's critical hit, rolled by
  // BattleCommand_BuildPlan.
  ABILITY_EFFECT_CRITICAL_1_IN_64 = 36,
  ABILITY_EFFECT_CRITICAL_1_IN_32 = 37,
  ABILITY_EFFECT_CRITICAL_1_IN_16 = 38,
  ABILITY_EFFECT_CRITICAL_1_IN_8 = 39,
  ABILITY_EFFECT_CRITICAL_1_IN_4 = 40,
  // A chance of a double or triple blow, rolled by BattleCommand_BuildPlan.
  ABILITY_EFFECT_DOUBLE_BLOW_1_IN_8 = 41,
  ABILITY_EFFECT_DOUBLE_BLOW_1_IN_4 = 42, // Double Fang, Twin Beaks
  ABILITY_EFFECT_TRIPLE_BLOW_1_IN_8 = 43,
  ABILITY_EFFECT_TRIPLE_BLOW_1_IN_4 = 44, // Swift Strike
  ABILITY_EFFECT_THREATEN = 45,           // Contain, Threaten, Tremble
  ABILITY_EFFECT_DEFEND = 46,             // Defend, Granite
  ABILITY_EFFECT_TOTAL_DEFENSE = 47,      // Total Defense, Flash
  ABILITY_EFFECT_FLEE = 49,               // Flee
  ABILITY_EFFECT_CALL_ALLY = 50,          // Ally Search, Speed Surge
  ABILITY_EFFECT_CALL_HELP = 51,          // Search for Help
  ABILITY_EFFECT_GROUND = 53,             // Ground: the target's turn lost
  ABILITY_EFFECT_CHALLENGE = 54,          // no ability has it
  ABILITY_EFFECT_SELF_DESTRUCT = 55,      // Dynamite, Mortal Blow
  ABILITY_EFFECT_REVIVE_HALF = 56,        // Quartz
  ABILITY_EFFECT_REVIVE_80 = 57,          // Dew: 80% of max HP
  ABILITY_EFFECT_AGILITY_DOWN = 58,       // Vine
  ABILITY_EFFECT_AGILITY_UP = 59,         // Zephyr
  ABILITY_EFFECT_DRAIN_HALF_HP = 60,      // Sap
  ABILITY_EFFECT_HEAL_HP_60 = 61,         // Fizz: 60% of max HP
  ABILITY_EFFECT_HEAL_HP_30 = 62,         // Spritz: 30%
  ABILITY_EFFECT_HEAL_PP_7 = 63,          // Ember: 7% of max PP
  ABILITY_EFFECT_CURE_ALL = 64,           // Tonic
  ABILITY_EFFECT_DOUBLE_BLOW_60 = 65,     // Gust, Rapid Smash: 153 in 256
  ABILITY_EFFECT_ACT_TWICE = 66,          // Kite
  ABILITY_EFFECT_SEAL_2_TURNS = 67,       // Luff
  ABILITY_EFFECT_TRIPLE_BLOW_60 = 68,     // Sonic Smash
  ABILITY_EFFECT_PSY_LEECH = 69           // Psy Leech
};

struct BattleAct {
  u8 targetMode;
  u8 targetFlag;
  u8 damageClass;
  u8 effect;
  u8 unk04[4];
  u8 range;
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
#if GAME_TLA
  u8 guardLv;
#else
  s8 guardLv;
#endif
  s8 elementLevels[4];
  s8 restraint;
  s8 poison;
  u8 atkCnt;
  s8 atkLv;
  u8 defCnt;
  s8 defLv;
  u8 resCnt;
  s8 resLv;
  u8 delusion;
  s8 confusion;
  u8 charm;
  u8 stun;
  u8 sleep;
  u8 psySeal;
  u8 refrain;
  s8 reflect;
  u8 haunt;
  u8 grimReaper;
  u8 unk142[2];
  u8 ready;
  u8 immobile;
  u8 agiCnt;
  s8 agiLv;
  s8 challenge;
  u8 unk149;
  u16 enemyId;
};

LAYOUT_SIZE_GUARD(BattleAction_Size, struct BattleAct, BATTLE_ACTION_SIZE);
LAYOUT_OFFSET_GUARD(BattleAction_Effect, struct BattleAct, effect, 0x03);
LAYOUT_OFFSET_GUARD(BattleAction_Power, struct BattleAct, power, 0x0a);
LAYOUT_OFFSET_GUARD(BattleAction_Type, struct BattleAct, type0c, 0x0c);

LAYOUT_SIZE_GUARD(OwnerActionSlot_Size, struct OwnerActionSlot, 4);
LAYOUT_SIZE_GUARD(BattleElementStats_Size, struct BattleElementStats, 4);
LAYOUT_SIZE_GUARD(BattleUnit_Size, struct BattleUnit, BATTLE_UNIT_SIZE);
LAYOUT_OFFSET_GUARD(BattleUnit_BaseHp, struct BattleUnit, baseHp, 0x10);
LAYOUT_OFFSET_GUARD(BattleUnit_BaseAttack, struct BattleUnit, baseAttack, 0x18);
LAYOUT_OFFSET_GUARD(BattleUnit_Actions, struct BattleUnit, actionSlots, 0x58);
LAYOUT_OFFSET_GUARD(BattleUnit_AvailableDjinn, struct BattleUnit,
                    djinnAvailable, 0xf8);
LAYOUT_OFFSET_GUARD(BattleUnit_ActiveDjinn, struct BattleUnit, djinnActive,
                    0x108);
LAYOUT_OFFSET_GUARD(BattleUnit_Rewards, struct BattleUnit, rewards, 0x120);
LAYOUT_OFFSET_GUARD(BattleUnit_Hp, struct BattleUnit, hp, 0x038);
LAYOUT_OFFSET_GUARD(BattleUnit_BaseElements, struct BattleUnit, baseElement,
                    0x024);
LAYOUT_OFFSET_GUARD(BattleUnit_Level, struct BattleUnit, level, 0x00f);
LAYOUT_OFFSET_GUARD(BattleUnit_Elements, struct BattleUnit, element, 0x048);
LAYOUT_OFFSET_GUARD(BattleUnit_ReadyPose, struct BattleUnit, ready, 0x144);
LAYOUT_OFFSET_GUARD(BattleUnit_Inventory, struct BattleUnit, inventory, 0x0d8);
LAYOUT_OFFSET_GUARD(BattleUnit_DjinnOwnedCounts, struct BattleUnit,
                    djinnOwnedCounts, 0x118);
LAYOUT_OFFSET_GUARD(BattleUnit_DjinnActiveCounts, struct BattleUnit,
                    djinnActiveCounts, 0x11c);
LAYOUT_OFFSET_GUARD(BattleUnit_GuardLevel, struct BattleUnit, guardLv, 0x12b);
LAYOUT_OFFSET_GUARD(BattleUnit_Status13a, struct BattleUnit, charm, 0x13a);
LAYOUT_OFFSET_GUARD(BattleUnit_Status140, struct BattleUnit, haunt, 0x140);

LAYOUT_OFFSET_GUARD(BattleUnit_EnemyId, struct BattleUnit, enemyId, 0x14a);

// ゲージの割合 (0x4000 で満タン) を 0〜0x4000 に収める
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

// HP と PP のゲージを計算し直す（残っていれば最低 1）
// A statement macro, written do { } while (0): every site matches through
// that shape. Written out at each site, or as an inline function, the
// gauge loads are scheduled across the store before them.
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
