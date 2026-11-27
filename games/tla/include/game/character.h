#ifndef ALCHEMY_CHARACTER_H
#define ALCHEMY_CHARACTER_H

// The definitions of characters, Djinn and enemies, as The Broken Seal's character.h has them; an
// enemy's lacks the ten bytes The Broken Seal's keeps before its items.

#include "types.h"
#include "static_assert.h"
#include "battle/battle_unit.h"

// Levels between two points of a growth curve.
#define GROWTH_BAND_LEVELS 20
// Points on a growth curve: levels 0 to 100, GROWTH_BAND_LEVELS apart.
#define GROWTH_POINTS      6

// A character's definition: the growth curves of its stats, its element bonus, starting level and
// starting equipment.
typedef struct CHARACTER_DEFINITION
{
    Uint8  reserved[80]; // twenty words, 0 in every row
    // The growth curves: each stat at levels 0, 20, 40, 60, 80 and 100.
    Sint16 hp[GROWTH_POINTS];
    Sint16 pp[GROWTH_POINTS];
    Uint16 attack[GROWTH_POINTS];
    Uint16 defense[GROWTH_POINTS];
    Uint16 agility[GROWTH_POINTS];
    Uint8  luck[GROWTH_POINTS];
    // The character's own level in each element before any Djinn, in tenths of a level (54 is
    // level 5.4); each Djinni set adds a whole level.
    Uint8  elementBonus[4];
    Uint8  startingLevel;      // the level a new game raises the character to
    Uint8  paddingBeforeItems; // padding
    // The starting items, each added and equipped at a new game; the low nine bits are the item.
    Uint16 items[13];
    Uint8  paddingAfterItems[2]; // padding
} CHARACTER_DEFINITION;

STATIC_ASSERT(sizeof(CHARACTER_DEFINITION) == 180);
STATIC_ASSERT(offsetof(CHARACTER_DEFINITION, hp) == 80);
STATIC_ASSERT(offsetof(CHARACTER_DEFINITION, elementBonus) == 146);
STATIC_ASSERT(offsetof(CHARACTER_DEFINITION, items) == 152);

// A Djinni: its ability and the stat bonuses it gives while set.
typedef struct DJINN_DEFINITION
{
    Uint16 abilityId;               // the action the Djinni unleashes
    Uint8  reservedAfterAbility[2]; // 0 in every row
    // Added to the base stats while the Djinni is set, before the class multiplies them.
    Sint8  hp;
    Sint8  pp;
    Sint8  attack;
    Sint8  defense;
    Sint8  agility;
    Sint8  luck;
    Uint8  reservedAfterStats[2]; // 0 in every row
} DJINN_DEFINITION;

STATIC_ASSERT(sizeof(DJINN_DEFINITION) == 12);

// ENEMY_DEFINITION.targetStrategy: a single-target attacker favours the target with the most HP
// left, the most maximum HP, or no one. The same column steers the action choice: with any strategy
// but TARGET_STRATEGY_MOST_HP an enemy skips Psynergy it lacks the PP for, and with
// TARGET_STRATEGY_RANDOM any Psynergy while sealed.
typedef enum ENEMY_TARGET_STRATEGY
{
    TARGET_STRATEGY_MOST_HP     = 0,
    TARGET_STRATEGY_MOST_MAX_HP = 1,
    TARGET_STRATEGY_RANDOM      = 2
} ENEMY_TARGET_STRATEGY;

// ENEMY_DEFINITION.actionStrategy: how btAIChooseAction picks a place in the enemy's eight actions.
// The first three roll against BattleAIActionWeights; the next two remember the place in the unit's
// AI state.
typedef enum ENEMY_ACTION_STRATEGY
{
    ACTION_STRATEGY_EVEN,           // 32 in 256 each
    ACTION_STRATEGY_FALLING,        // from 53 in 256 down to 11
    ACTION_STRATEGY_FIRST_FAVOURED, // the first 113 in 256, down to 5
    // A random place the first time, then each in turn at the enemy's turn.
    ACTION_STRATEGY_RANDOM_START,
    ACTION_STRATEGY_IN_ORDER,  // each in turn from the remembered place
    ACTION_STRATEGY_NEXT_PICK, // the next place at each new pick, from 0
    ACTION_STRATEGY_SAME       // the place last picked
} ENEMY_ACTION_STRATEGY;

// An enemy's definition: its level and stats, items, element preset, the actions it chooses from
// and its rewards.
typedef struct ENEMY_DEFINITION
{
    // blank in every row; the name comes from the messages
    Uint8  name[UNIT_NAME_SIZE];
    Uint8  level;
    // The base stats copied into the unit.
    Uint16 hp;
    Uint16 pp;
    Uint16 attack;
    Uint16 defense;
    Uint16 agility;
    Uint8  luck;
    Uint8  actionCount;    // actions a turn
    Uint8  hpRegen;        // HP restored at each turn's end
    Uint8  ppRegen;        // PP restored at each turn's end
    Uint16 items[4];       // items the enemy carries, 0 for none
    Uint8  counts[4];      // how many of each item it carries
    Uint8  elementPreset;  // row of EnemyElementPresetTable
    Sint8  targetStrategy; // whom a single-target attack favours
    Sint8  actionStrategy; // how the action is chosen from abilities
    // One bit per place in `abilities` (ITEM_ACTION): use an item held instead.
    Uint8  itemActions;
    Uint16 abilities[8];   // the abilities the enemy chooses from
    Uint8  weakEffects[4]; // the ability effects the enemy is weak to
    Uint16 coins;          // coins won
    Sint16 item;           // the item it may leave, 0 for none
    // The chance of the item, as a power of two: 1 in 2 to the power n - 1 (1 always, 6 one in 32);
    // 0 for none.
    Sint16 itemChance;
    Uint16 experience; // experience won
    Uint8  padding[2]; // padding
} ENEMY_DEFINITION;

STATIC_ASSERT(sizeof(ENEMY_DEFINITION) == 76);
STATIC_ASSERT(offsetof(ENEMY_DEFINITION, items) == 30);
STATIC_ASSERT(offsetof(ENEMY_DEFINITION, abilities) == 46);
STATIC_ASSERT(offsetof(ENEMY_DEFINITION, coins) == 66);
STATIC_ASSERT(offsetof(ENEMY_DEFINITION, experience) == 72);
// ENEMY_DEFINITION.itemActions' bit for `place` in its abilities.
#define ITEM_ACTION(place) (1 << (place))

// An enemy's element preset in EnemyElementPresetTable: the element it attacks with, its level in
// each element, and its base power and resistance in each.
typedef struct ENEMY_ELEMENT_PRESET
{
    // the element it attacks with (gmEnemyGetAttackElement)
    Sint32               attackElement;
    // Its level in each element; gmUnitGetElementLevels returns ten times each.
    Uint8                elementLevels[4];
    // Its base power and resistance in each element, as gmUnitCalculateBaseElements gives them to
    // the unit.
    BATTLE_ELEMENT_STATS stats[4];
} ENEMY_ELEMENT_PRESET;

// The rows of EnemyElementPresetTable (ENEMY_DEFINITION.elementPreset), each named by the first
// enemy that has it, as The Broken Seal names its own. Rows 10, 22, 28 and 35 are spares no enemy
// has.
typedef enum ENEMY_ELEMENT_PRESET_ID
{
    // The row an out-of-range preset reads as, and every enemy's that names none.
    ELEMENT_PRESET_DEFAULT        = 0,
    ELEMENT_PRESET_BANDIT         = 1,
    ELEMENT_PRESET_AZART          = 2,
    ELEMENT_PRESET_CHESTBEATER    = 3,
    ELEMENT_PRESET_GRISLY         = 4,
    ELEMENT_PRESET_PUNCH_ANT      = 5,
    ELEMENT_PRESET_KING_SCORPION  = 6,
    ELEMENT_PRESET_MAD_PLANT      = 7,
    ELEMENT_PRESET_WOOD_WALKER    = 8,
    ELEMENT_PRESET_DRONE_BEE      = 9,
    ELEMENT_PRESET_MOMONGA        = 11,
    ELEMENT_PRESET_PIXIE          = 12,
    ELEMENT_PRESET_MUMMY          = 13,
    ELEMENT_PRESET_SKELETON       = 14,
    ELEMENT_PRESET_GHOST          = 15,
    ELEMENT_PRESET_LIVING_ARMOR   = 16,
    ELEMENT_PRESET_GARGOYLE       = 17,
    ELEMENT_PRESET_SERPENT        = 18,
    ELEMENT_PRESET_AQUA_HYDRA     = 19,
    ELEMENT_PRESET_AVIMANDER      = 20,
    ELEMENT_PRESET_TORNADO_LIZARD = 21,
    ELEMENT_PRESET_VENUS_DJINNI   = 23,
    ELEMENT_PRESET_CUTTLE         = 24,
    ELEMENT_PRESET_MERCURY_DJINNI = 25,
    ELEMENT_PRESET_STONE_SOLDIER  = 26,
    ELEMENT_PRESET_MARS_DJINNI    = 27,
    ELEMENT_PRESET_JUPITER_DJINNI = 29,
    ELEMENT_PRESET_MINI_GOBLIN    = 30,
    ELEMENT_PRESET_ASSASSIN       = 31,
    ELEMENT_PRESET_WOLFKIN_CUB    = 32,
    ELEMENT_PRESET_MERMAN         = 33,
    ELEMENT_PRESET_GNOME          = 34,
    ELEMENT_PRESET_GNOME_MAGE     = 36,
    ELEMENT_PRESET_NIGHTMARE      = 37,
    ELEMENT_PRESET_MIMIC          = 38,
    ELEMENT_PRESET_AGATIO         = 39,
    ELEMENT_PRESET_KARST          = 40,
    ELEMENT_PRESET_FUSION_DRAGON  = 41,
    ELEMENT_PRESET_SENTINEL       = 42,
    ELEMENT_PRESET_GRYPHON        = 43,
    ELEMENT_PRESET_STAR_MAGICIAN  = 44,
    ELEMENT_PRESET_VALUKAR        = 45,
    ELEMENT_PRESET_DULLAHAN       = 46,
    ELEMENT_PRESET_DOOM_DRAGON    = 47
} ENEMY_ELEMENT_PRESET_ID;

// How many element presets EnemyElementPresetTable holds; an enemy naming one past the last reads
// the first.
#define ENEMY_ELEMENT_PRESET_COUNT 48

STATIC_ASSERT(sizeof(ENEMY_ELEMENT_PRESET) == 24);

// A level up's result: the level reached and the gain in each stat. The caller provides it;
// gmUnitLevelUp fills it, and btAwardSpoils reports each gain that is not zero.
typedef struct LEVEL_UP_RESULT
{
    Sint16 level;
    // Set to -1 and read by nothing: the battle finds an ability learned by comparing the unit's
    // Psynergy before and after.
    Uint16 ability;
    Uint16 hp;
    Uint16 pp;
    Uint16 attack;
    Uint16 defense;
    Uint16 agility;
    Uint16 luck;
} LEVEL_UP_RESULT;

#endif
