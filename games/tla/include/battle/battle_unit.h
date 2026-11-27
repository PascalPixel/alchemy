#ifndef ALCHEMY_BATTLE_UNIT_H
#define ALCHEMY_BATTLE_UNIT_H

// Unit, a party member's or enemy's battle state, with the elements, abilities and actions it uses.

#include "edition.h"
#include "static_assert.h"
#include "game/item.h"

// The letters A to Z, which the Japanese edition gives enemies as name suffixes.
#define NAME_LETTER_ALPHABET 26
// The name suffixes that tell enemies of one kind apart, one more character after the name
// (gmBattleUnitSet): the alphabet in Japanese, the digits 1 to 9 elsewhere, as
// btStringFindFirstDigit reads them back.
#define NAME_SUFFIX_COUNT    JAPANESE_OR(NAME_LETTER_ALPHABET, 9)

// Bytes in a UNIT, which the code clears and copies whole: the size the STATIC_ASSERTs below hold
// it to.
enum
{
    BATTLE_UNIT_SIZE = 332
};

// The action id's bits in an action list entry.
enum
{
    // PSYNERGY_ENTRY.encodedAction's action id
    PSYNERGY_ABILITY_MASK     = 0x3fff,
    // PSYNERGY_ENTRY.encodedAction's marks: the action comes from an equipped item, or from the
    // class (its top bit, PSYNERGY_FROM_CLASS_SHIFT).
    PSYNERGY_FROM_ITEM        = 0x4000,
    PSYNERGY_FROM_CLASS       = 0x8000,
    PSYNERGY_FROM_CLASS_SHIFT = 15
};

// An entry in a unit's action list: the action's id (PSYNERGY_ABILITY_MASK) and marks for an action
// its class or an equipped item gave.
typedef struct PSYNERGY_ENTRY
{
    // bits: 15 from the class | 14 from an equipped item | 13..0 action id. Entries with neither
    // mark were taught for good; 0 is empty.
    Uint16 encodedAction;
    Uint16 unused; // no code names it
} PSYNERGY_ENTRY;

// A unit's elemental power and resistance in one element.
typedef struct BATTLE_ELEMENT_STATS
{
    Sint16 power;      // Elemental power in the element.
    Sint16 resistance; // Elemental resistance to it.
} BATTLE_ELEMENT_STATS;

// The elements, in the order of every table kept per element: a unit's element stats, the Djinn and
// the summons. ELEMENT_NONE marks an ability or a weapon that has no element.
typedef enum ELEMENT
{
    ELEMENT_VENUS,
    ELEMENT_MERCURY,
    ELEMENT_MARS,
    ELEMENT_JUPITER,
    ELEMENT_NONE,
    ELEMENT_COUNT = ELEMENT_NONE
} ELEMENT;

// ABILITY_DEFINITION.range of an ability that reaches every unit on the side, each as if aimed at.
#define ABILITY_RANGE_ALL    255

// The low four bits of ABILITY_DEFINITION.flags: the ability kind (enum ABILITY_FORMULA).
#define ABILITY_FORMULA_MASK 15

// An ability kind picks what the ability's power does to each target; ABILITY_FORMULA_BLANK,
// ABILITY_FORMULA_EFFECT_ONLY and ABILITY_FORMULA_NO_POWER apply none.
typedef enum ABILITY_FORMULA
{
    ABILITY_FORMULA_BLANK             = 0, // no ability
    ABILITY_FORMULA_HEAL_HP           = 1, // Cure, Fizz, healing items
    ABILITY_FORMULA_EFFECT            = 2, // damage only if the effect lands
    ABILITY_FORMULA_ATTACK            = 3, // Attack vs Defense, plus the power
    ABILITY_FORMULA_ATTACK_MULTIPLIED = 4, // that attack times power / 10
    ABILITY_FORMULA_DAMAGE            = 5, // HP damage from the power (Psynergy)
    // HP damage from enemies' moves, announced as `<unit> unleashes <ability>!`.
    ABILITY_FORMULA_ENEMY_DAMAGE      = 6,
    // Granite, Flash and Kite, whose effect is all they do.
    ABILITY_FORMULA_EFFECT_ONLY       = 7,
    ABILITY_FORMULA_SUMMON            = 8,
    // The field Psynergy, Defend, Flee and enemies' commands.
    ABILITY_FORMULA_NO_POWER          = 9,
    ABILITY_FORMULA_PP_DAMAGE         = 10,
    ABILITY_FORMULA_HEAL_PP           = 11
} ABILITY_FORMULA;

// ABILITY_DEFINITION.flags: where an ability can be used.
typedef enum ABILITY_USE
{
    ABILITY_USABLE_IN_FIELD  = 0x40, // can be used outside battle
    // can be used in battle; else greyed there
    ABILITY_USABLE_IN_BATTLE = 0x80
} ABILITY_USE;

// ABILITY_DEFINITION.targetMode: whom an ability may target, from its user's view.
typedef enum ABILITY_TARGET
{
    ABILITY_TARGET_ONE    = 0, // the aimed unit alone, whatever its range
    ABILITY_TARGET_FOES   = 1, // the other side
    ABILITY_TARGET_ALLIES = 2, // the user's side, the user included
    ABILITY_TARGET_SELF   = 4  // the user alone, whatever the range
} ABILITY_TARGET;

// ABILITY_DEFINITION.effect: what an ability does besides its damage or healing, applied by
// btAbilityApplyToTarget unless noted. Each is named for the abilities that have it (actions.tsv,
// with their English names and descriptions). Sidestep (48), Crazy Voice (52) and Vengeance (89)
// have effects no code here reads.
typedef enum ABILITY_EFFECT
{
    ABILITY_EFFECT_NONE                    = 0,
    ABILITY_EFFECT_RESTORE_HP              = 1,  // restores HP (the item menu's use)
    ABILITY_EFFECT_RESTORE_PP              = 2,  // restores PP (the item menu's use)
    ABILITY_EFFECT_CURE_POISON             = 3,  // Cure Poison, Antidote
    ABILITY_EFFECT_RESTORE                 = 4,  // Restore, Elixir
    ABILITY_EFFECT_REVIVE                  = 5,  // Revive, Water of Life: full HP
    ABILITY_EFFECT_ATTACK_UP_SHARPLY       = 6,  // Impact: two stages
    ABILITY_EFFECT_ATTACK_UP               = 7,  // High Impact: one stage
    ABILITY_EFFECT_ATTACK_DOWN_SHARPLY     = 8,  // Dull
    ABILITY_EFFECT_ATTACK_DOWN             = 9,  // Blunt
    ABILITY_EFFECT_DEFENSE_UP_SHARPLY      = 10, // Guard
    ABILITY_EFFECT_DEFENSE_UP              = 11, // Protect
    ABILITY_EFFECT_DEFENSE_DOWN_SHARPLY    = 12, // Impair
    ABILITY_EFFECT_DEFENSE_DOWN            = 13, // Debilitate
    ABILITY_EFFECT_RESISTANCE_UP_SHARPLY   = 14, // Ward
    ABILITY_EFFECT_RESISTANCE_UP           = 15, // Resist
    ABILITY_EFFECT_RESISTANCE_DOWN_SHARPLY = 16, // Weaken
    ABILITY_EFFECT_RESISTANCE_DOWN         = 17, // Enfeeble
    ABILITY_EFFECT_POISON                  = 18, // Taint
    ABILITY_EFFECT_VENOM                   = 19, // Poison
    ABILITY_EFFECT_DELUSION                = 20, // Delude
    ABILITY_EFFECT_CONFUSION               = 21, // Confuse
    ABILITY_EFFECT_CHARM                   = 22, // Charm
    ABILITY_EFFECT_STUN                    = 23, // Paralyze
    ABILITY_EFFECT_SLEEP                   = 24, // Sleep
    ABILITY_EFFECT_SEAL                    = 25, // Bind: Psynergy sealed
    ABILITY_EFFECT_HAUNT                   = 26, // Haunt
    ABILITY_EFFECT_INSTANT_DEATH           = 27, // Condemn
    ABILITY_EFFECT_CURSE                   = 28, // Curse: the Grim Reaper's countdown
    ABILITY_EFFECT_REGENERATE              = 29, // Regenerate
    ABILITY_EFFECT_REFLECT                 = 30, // Reflect
    ABILITY_EFFECT_DRAIN_HP                = 31, // Drain
    ABILITY_EFFECT_DRAIN_PP                = 32, // Psy Drain
    ABILITY_EFFECT_BREAK                   = 33, // Break: raised stats undone
    ABILITY_EFFECT_LEAVE_ONE_HP            = 34, // Severe Blow, Thrash
    ABILITY_EFFECT_HALVE_DEFENSE           = 35, // Torch
    // A 1 in 64 to 1 in 4 chance of an ability's critical hit, rolled by BattleCommandBuildPlan.
    ABILITY_EFFECT_CRITICAL_A              = 36,
    ABILITY_EFFECT_CRITICAL_B              = 37,
    ABILITY_EFFECT_CRITICAL_C              = 38,
    ABILITY_EFFECT_CRITICAL_D              = 39,
    ABILITY_EFFECT_CRITICAL_E              = 40,
    // A chance of a double or triple blow, rolled by BattleCommandBuildPlan.
    ABILITY_EFFECT_DOUBLE_BLOW_RARE        = 41,
    ABILITY_EFFECT_DOUBLE_BLOW             = 42, // Double Fang, Twin Beaks
    ABILITY_EFFECT_TRIPLE_BLOW_RARE        = 43,
    ABILITY_EFFECT_TRIPLE_BLOW             = 44, // Swift Strike
    ABILITY_EFFECT_THREATEN                = 45, // Contain, Threaten, Tremble
    ABILITY_EFFECT_DEFEND                  = 46, // Defend, Granite
    ABILITY_EFFECT_TOTAL_DEFENSE           = 47, // Total Defense, Flash
    ABILITY_EFFECT_SIDESTEP                = 48, // Sidestep
    ABILITY_EFFECT_FLEE                    = 49, // Flee
    ABILITY_EFFECT_CALL_ALLY               = 50, // Ally Search, Speed Surge
    ABILITY_EFFECT_CALL_HELP               = 51, // Search for Help
    ABILITY_EFFECT_CRAZY_VOICE             = 52, // Crazy Voice
    ABILITY_EFFECT_GROUND                  = 53, // Ground: the target's turn lost
    ABILITY_EFFECT_CHALLENGE               = 54, // no ability has it
    ABILITY_EFFECT_SELF_DESTRUCT           = 55, // Dynamite, Mortal Blow
    ABILITY_EFFECT_REVIVE_HALF             = 56, // Quartz
    ABILITY_EFFECT_DEW                     = 57, // Dew: 80% of max HP
    ABILITY_EFFECT_AGILITY_DOWN            = 58, // Vine
    ABILITY_EFFECT_AGILITY_UP              = 59, // Zephyr
    ABILITY_EFFECT_DRAIN_HALF_HP           = 60, // Sap
    ABILITY_EFFECT_FIZZ                    = 61, // Fizz: 60% of max HP
    ABILITY_EFFECT_SPRITZ                  = 62, // Spritz: 30%
    ABILITY_EFFECT_EMBER                   = 63, // Ember: 7% of max PP
    ABILITY_EFFECT_CURE_ALL                = 64, // Tonic
    ABILITY_EFFECT_DOUBLE_BLOW_LIKELY      = 65, // Gust, Rapid Smash: 153 in 256
    ABILITY_EFFECT_ACT_TWICE               = 66, // Kite
    ABILITY_EFFECT_SEAL_2_TURNS            = 67, // Luff
    ABILITY_EFFECT_TRIPLE_BLOW_LIKELY      = 68, // Sonic Smash
    ABILITY_EFFECT_PSY_LEECH               = 69, // Psy Leech
    ABILITY_EFFECT_CRYSTAL                 = 70, // Crystal: half of max HP
    ABILITY_EFFECT_SPRING                  = 71, // Spring
    ABILITY_EFFECT_SHIELD                  = 72, // Shade: DEFEND_SHIELD
    ABILITY_EFFECT_BALM                    = 73, // Balm, Spark: 60% of max HP
    ABILITY_EFFECT_COUNTER                 = 74, // Reflux, Counter-rush
    // Shine: the first target takes the damage, the others are deluded.
    ABILITY_EFFECT_DAZZLE                  = 75,
    ABILITY_EFFECT_BREATH                  = 76, // Breath
    ABILITY_EFFECT_AROMA                   = 77, // Aroma
    ABILITY_EFFECT_ETHER                   = 78, // Ether
    ABILITY_EFFECT_VANISH                  = 79, // Haze: DEFEND_VANISHED
    // Gasp: Curse, rolled three times (gmBattleRollEffectSuccess).
    ABILITY_EFFECT_CURSE_THREE_ROLLS       = 80,
    // Lull: "Everybody is resting!" (BATTLE_STATE.ceaseFire).
    ABILITY_EFFECT_CEASE_FIRE              = 81,
    // Gale: an enemy blown out of the battle, as one that flees.
    ABILITY_EFFECT_BLOW_AWAY               = 82,
    ABILITY_EFFECT_ULYSSES                 = 83, // Ulysses: as Ground
    // Catastrophe: the target also loses a tenth of its max PP.
    ABILITY_EFFECT_CATASTROPHE             = 84,
    // Fire Puppet: Paralyze, rolled three times (gmBattleRollEffectSuccess).
    ABILITY_EFFECT_STUN_THREE_ROLLS        = 85,
    // Clarion Cry, Mystic Call: a Sea Fighter called in, or a Star Magician's ball.
    ABILITY_EFFECT_CALL_REINFORCEMENT      = 86,
    // Regen Dance: the next enemy of BATTLE_STATE.queuedSummons called in.
    ABILITY_EFFECT_CALL_QUEUED             = 87,
    ABILITY_EFFECT_GUARD_AURA              = 88, // Guard Aura: as Total Defense
    ABILITY_EFFECT_VENGEANCE               = 89, // Vengeance
    // Raiden's Wrath: lands only on a dragon (EnemyDragonIds), and ignores its Defense.
    ABILITY_EFFECT_SLAY_DRAGON             = 90,
    // Trident: lands only on a sea creature (EnemySeaCreatureIds), and ignores its Defense.
    ABILITY_EFFECT_SLAY_SEA_CREATURE       = 91
} ABILITY_EFFECT;

// An ability's definition, a row of AbilityDefinitions: its targeting, kind, element, effect,
// range, PP cost and power.
typedef struct ABILITY_DEFINITION
{
    Uint8  targetMode; // ABILITY_TARGET
    // bits: 7 usable in battle | 6 usable in the field | 5..4 clear in every row | 3..0 kind
    // (ABILITY_FORMULA_MASK, ABILITY_FORMULA).
    Uint8  flags;
    Uint8  element;                    // ELEMENT
    Uint8  effect;                     // ABILITY_EFFECT
    Uint16 icon;                       // the ability's icon
    Uint8  fieldPsynergy;              // FIELD_PSY, or 0 for none
    Uint8  reservedAfterFieldPsynergy; // 0 in every row
    Uint8
        // the aimed unit and range - 1 either side, or ABILITY_RANGE_ALL
           range;
    Uint8  ppCost; // PP the ability costs
    Uint16 power;  // what the kind applies, 0 for none
} ABILITY_DEFINITION;

// The curse bits of UNIT.curse.
typedef enum CURSE_FLAG
{
    CURSE_BOUND      = 0x01, // the unit is bound: one action in four is lost
    CURSE_ITEM       = 0x02, // a cursed item is equipped
    CURSE_KEEP_BOUND = 0x04, // keeps CURSE_BOUND through gmUnitCalculate
    // EQUIP_EFFECT_CURSE_REMOVABLE: gmUnitCalculate clears it again with CURSE_BOUND.
    CURSE_REMOVABLE  = 0x08
} CURSE_FLAG;

// UNIT.classId of an enemy, which has no class of its own.
#define CLASS_NONE        0

// The last party unit id: ids 0 to 7, one for each character, ISAAC to PIERS.
#define ALLY_MAX          7

// Party unit ids: one for each character, ISAAC to PIERS.
#define PARTY_UNIT_COUNT  (ALLY_MAX + 1)

// The party fights four at a time, three in a link battle; six enemies at most, three in a link
// battle.
#define PARTY_BATTLE_SIZE 4
#define ENEMY_BATTLE_SIZE 6

// The most units in a battle, every party unit and the most enemies: the length of a unit list such
// as btUnitListIds fills.
#define UNIT_MAX          (PARTY_UNIT_COUNT + ENEMY_BATTLE_SIZE)

// UNIT.poison: poisoned, or deadly poisoned.
#define POISON_NORMAL     1
#define POISON_VENOM      2

// UNIT.defendLevel, 0 for no guard: each guard effect raises it to its own level and no lower
// (btAbilityApplyToTarget). Defend and Granite halve the damage, Shade cuts it to two fifths, Total
// Defense, Flash and Guard Aura to a tenth (REDUCE_BY_GUARD), and Haze leaves the unit untouched.
#define DEFEND_NORMAL     1
#define DEFEND_SHIELD     2
#define DEFEND_TOTAL      3
#define DEFEND_VANISHED   4

// Unit ids from 128 are the enemies'.
#define FIRST_ENEMY_UNIT  128
// The bit set in every enemy's unit id (FIRST_ENEMY_UNIT and up), and so the side a unit fights on.
#define UNIT_ID_ENEMY_BIT FIRST_ENEMY_UNIT

// How many actions a unit's list holds (UNIT.psynergy).
#define PSYNERGY_SLOTS    32

// An enemy's memory of its action choice (btAIChooseAction).
typedef struct BATTLE_AI_STATE
{
    // ACTION_STRATEGY_RANDOM_START has drawn its start.
    unsigned initialized : 1;
    unsigned selection   : 3; // the place in its action list, 0 to 7
    // the Dullahan's form, 0 to 2 (BattleAIDullahanForms)
    unsigned form        : 2;
    unsigned remaining   : 26; // no code names it
} BATTLE_AI_STATE;

// A full HP or PP gauge (UNIT.hpGauge, UNIT.ppGauge): the gauge is the current value over the
// maximum in 2.14 fixed point, and the cap gmUnitClampGauge puts on hp / hpMax shifted up
// UNIT_GAUGE_SHIFT bits.
#define UNIT_GAUGE_SHIFT       14
#define UNIT_GAUGE_FULL        (1 << UNIT_GAUGE_SHIFT)

// The bits of UNIT.baseActionCount that count the unit's actions a turn.
#define UNIT_ACTION_COUNT_MASK 0xf

// A unit's name buffer: 14 characters and the 0 that ends them.
#define UNIT_NAME_SIZE         15
// The Resistance each of a unit's battle Resistance stages (UNIT.resistanceStage) adds to every
// element's.
#define RESISTANCE_PER_STAGE   20

// UNIT.psySeal's layout: its low three bits Bind's turns left, the seal Bind casts (seven turns,
// which may end sooner); above them Luff's, cast as PSY_SEAL_LUFF and losing PSY_SEAL_LUFF_TURN a
// turn, so it lasts two and never ends early.
#define PSY_SEAL_BIND_MASK     7
#define PSY_SEAL_BIND          7
#define PSY_SEAL_LUFF          16
#define PSY_SEAL_LUFF_TURN     8

// A unit in battle: a party member's or an enemy's stats, Psynergy, items, Djinn and statuses.
typedef struct UNIT
{
    Uint8                name[UNIT_NAME_SIZE]; // one byte a character, ending in 0
    Uint8                level;                // 1 to 99 for a character
    Sint16               baseHp; // before equipment, Djinn and class (gmUnitCalculate)
    Sint16               basePp; // before equipment, Djinn and class (gmUnitCalculate)
    // hp / hpMax in 0..UNIT_GAUGE_FULL; 1 while any HP remains
    Sint16               hpGauge;
    // pp / ppMax in 0..UNIT_GAUGE_FULL; 1 while any PP remains
    Sint16               ppGauge;
    Uint16               baseAttack;      // before equipment, Djinn, class and stages
    Uint16               baseDefense;     // before equipment, Djinn, class and stages
    Uint16               baseAgility;     // before equipment, Djinn, class and stages
    Uint8                baseLuck;        // before equipment, Djinn and class
    Uint8                baseActionCount; // actions a turn before equipment
    Uint8                baseHpRegen;     // HP regained each turn before equipment
    Uint8                basePpRegen;     // PP regained each turn before equipment
    Uint8                baseUnused[2];   // no code names it
    // Power and resistance before equipment and stages: an enemy's from its element preset, a
    // character's from its element levels (gmUnitCalculateBaseElements).
    BATTLE_ELEMENT_STATS baseElement[ELEMENT_COUNT];
    Sint16               hpMax;          // maximum HP (gmUnitCalculate)
    Sint16               ppMax;          // maximum PP (gmUnitCalculate)
    Sint16               hp;             // current HP, 0 when downed
    Sint16               pp;             // current PP
    Uint16               attack;         // Attack (gmUnitCalculate)
    Uint16               defense;        // Defense (gmUnitCalculate)
    Uint16               agility;        // Agility (gmUnitCalculate)
    Uint8                luck;           // Luck (gmUnitCalculate)
    Uint8                actionsPerTurn; // actions each turn (gmUnitCalculate)
    Uint8                hpRegen;        // HP regained at each turn's end
    Uint8                ppRegen;        // PP regained at each turn's end
    Uint8                unused[2];      // no code names it, as baseUnused
    // Power and resistance in each element after equipment, element levels and Resistance stages
    // (gmUnitCalculate).
    BATTLE_ELEMENT_STATS element[ELEMENT_COUNT];
    // The action list: Psynergy taught for good, then the class's and the equipment's. An empty
    // entry is 0.
    PSYNERGY_ENTRY       psynergy[PSYNERGY_SLOTS];
    // Items held, one entry each: bits 15..11 quantity | 10 broken | 9 equipped | 8..0 item id
    // (INVENTORY_*). An empty entry is 0.
    Uint16               inventory[INVENTORY_SLOTS];
    Uint8                paddingBeforeDjinn[2]; // up to the word djinnHeld starts on
    // The Djinn held, one bit per Djinni (by its index) in each element's word.
    Uint32               djinnHeld[ELEMENT_COUNT];
    // The Djinn set, one bit per Djinni.
    Uint32               djinnSet[ELEMENT_COUNT];
    Uint8                djinnHeldCounts[ELEMENT_COUNT]; // the Djinn held in each element
    Uint8                djinnSetCounts[ELEMENT_COUNT];  // the Djinn set in each element
    // Cleared when an enemy is set up. During a battle the enemy's action choice keeps its place in
    // its action list here (btAIChooseAction).
    BATTLE_AI_STATE      aiState;
    Uint32               experience; // a character's experience points
    // A character's id (ISAAC to PIERS, below 8), written at a new game: the equipment masks and
    // the battle presentation read it. Who any unit is, character or enemy, is enemyId.
    Uint8                id;
    // The character's class, worked out from its Djinn (gmUnitUpdateClass); CLASS_NONE marks an
    // enemy, and code throughout tests it to tell the two apart.
    Uint8                classId;
    // An BATTLE_SIDE. gmBattleUnitSet puts the Djinn the party fights (enemies 9 to 66 and 369 to
    // 377) on the party's side.
    Uint8                side;
    // 0 to 4: each guard effect raises it to its own level and no lower; Defend's is 1.
    Uint8                defendLevel;
    // cleared by btPartyClearBattleStatuses
    Sint8                summonBoost[ELEMENT_COUNT];
    // The curse: nonzero shows it in the menus and lets the sanctum remove it.
    Sint8                curse;
    Sint8                poison; // 1 poisoned, 2 deadly poison
    // Each stat change: the turns it has left, and its stage. Stages run -4 to 4 and last 7 turns.
    Uint8                attackTurns;
    Sint8                attackStage;
    Uint8                defenseTurns;
    Sint8                defenseStage;
    Uint8                resistanceTurns;
    Sint8                resistanceStage;
    // Delusion to sleep: the turns each status has left, 7 when it lands.
    Uint8                delusion;
    Sint8                confusion;
    Uint8                charm;
    Uint8                stun;
    Uint8                sleep;
    // Psynergy is sealed while any bit is set: Luff's seal above PSY_SEAL_BIND_MASK, Bind's turns
    // within it.
    Uint8                psySeal;
    Uint8                regenerate; // HP regeneration's turns left, 5 when cast
    Sint8                reflect;    // Reflect's turns left, 7 when cast
    Uint8                haunt;      // set while an evil spirit grips the unit
    Uint8                grimReaper; // the Grim Reaper's countdown, 7 when cursed
    // [0] the EQUIP_EFFECT_ADD_UNLEASH_CHANCE total gmUnitCalculate sums, which nothing reads
    // (gmEquipmentGetUnleashRateBonus sums it afresh); [1] set by ABILITY_EFFECT_COUNTER while the
    // unit is ready to counterattack, for the rest of the turn.
    Uint8                equipBonus[2];
    // The extra action: 2 when cast, down one at each turn's end.
    Uint8                ready;
    Uint8                immobile;     // set by ABILITY_EFFECT_GROUND
    Uint8                agilityTurns; // the Agility change's turns left, 5 when cast
    Sint8                agilityStage; // the Agility stage, -4 to 8 (twice the Agility)
    // Set by ABILITY_EFFECT_CHALLENGE, which no ability has: the unit will not flee.
    Sint8                challenge;
    Uint8                paddingBeforeEnemyId; // up to the halfword enemyId starts on
    // Who the unit is: an enemy's id (gmBattleUnitSet), or a character's id (copied from id at a
    // new game) for gmUnitUpdateClass and the experience table.
    Uint16               enemyId;
} UNIT;

STATIC_ASSERT(offsetof(ABILITY_DEFINITION, effect) == 3);
STATIC_ASSERT(offsetof(ABILITY_DEFINITION, power) == 10);
STATIC_ASSERT(sizeof(ABILITY_DEFINITION) == 12);

STATIC_ASSERT(sizeof(PSYNERGY_ENTRY) == 4);
STATIC_ASSERT(sizeof(BATTLE_ELEMENT_STATS) == 4);
STATIC_ASSERT(sizeof(UNIT) == BATTLE_UNIT_SIZE);
STATIC_ASSERT(offsetof(UNIT, baseHp) == 16);
STATIC_ASSERT(offsetof(UNIT, baseAttack) == 24);
STATIC_ASSERT(offsetof(UNIT, psynergy) == 88);
STATIC_ASSERT(offsetof(UNIT, djinnHeld) == 248);
STATIC_ASSERT(offsetof(UNIT, djinnSet) == 264);
STATIC_ASSERT(offsetof(UNIT, aiState) == 288);
STATIC_ASSERT(offsetof(UNIT, hp) == 56);
STATIC_ASSERT(offsetof(UNIT, baseElement) == 36);
STATIC_ASSERT(offsetof(UNIT, level) == 15);
STATIC_ASSERT(offsetof(UNIT, element) == 72);
STATIC_ASSERT(offsetof(UNIT, ready) == 324);
STATIC_ASSERT(offsetof(UNIT, inventory) == 216);
STATIC_ASSERT(offsetof(UNIT, djinnHeldCounts) == 280);
STATIC_ASSERT(offsetof(UNIT, djinnSetCounts) == 284);
STATIC_ASSERT(offsetof(UNIT, defendLevel) == 299);
STATIC_ASSERT(offsetof(UNIT, charm) == 314);
STATIC_ASSERT(offsetof(UNIT, haunt) == 320);

STATIC_ASSERT(offsetof(UNIT, enemyId) == 330);

// Clamps a gauge ratio to 0..UNIT_GAUGE_FULL.
static __inline__ Sint32 gmUnitClampGauge(Sint32 ratio)
{
    Sint32 clamped;

    if (ratio > UNIT_GAUGE_FULL)
        clamped = UNIT_GAUGE_FULL;
    else if (ratio < 0)
        clamped = 0;
    else
        clamped = ratio;
    return clamped;
}

// Recalculates HP and PP gauges, keeping at least one when any points remain.
#define UNIT_UPDATE_GAUGES(unit)                                                                   \
    do                                                                                             \
    {                                                                                              \
        (unit)->hpGauge = gmUnitClampGauge(((unit)->hp << UNIT_GAUGE_SHIFT) / (unit)->hpMax);      \
        if ((unit)->hpGauge == 0 && (unit)->hp != 0)                                               \
            (unit)->hpGauge = 1;                                                                   \
        (unit)->ppGauge = gmUnitClampGauge(((unit)->pp << UNIT_GAUGE_SHIFT) / (unit)->ppMax);      \
        if ((unit)->ppGauge == 0 && (unit)->pp != 0)                                               \
            (unit)->ppGauge = 1;                                                                   \
    } while (0)

// The eight characters' units, ISAAC to PIERS, kept in the save.
extern UNIT CharacterUnits[PARTY_UNIT_COUNT];

// A unit's base stats from UNIT.baseHp on: the formations copy the block before a level or strength
// change, and the Clear Data writes the imported and password members' stats through it.
typedef struct BATTLE_BASE_STATS
{
    Sint16 baseHp;
    Sint16 basePp;
    Sint16 hpGauge; // copied, not read
    Sint16 ppGauge; // copied, not read
    Uint16 baseAttack;
    Uint16 baseDefense;
    Uint16 baseAgility;
    Uint8  baseLuck;        // copied, not read
    Uint8  actionCount;     // copied, not read
    Uint8  hpRegen;         // copied, not read
    Uint8  ppRegen;         // copied, not read
    Uint8  unused[2];       // Unit's baseUnused
    BATTLE_ELEMENT_STATS
    element[ELEMENT_COUNT]; // the base power is read
} BATTLE_BASE_STATS;

#endif
