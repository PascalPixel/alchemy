#ifndef ALCHEMY_CHARACTER_H
#define ALCHEMY_CHARACTER_H

/* The definitions of characters, classes, Djinn and enemies, and a level up's
   result. */

#include "edition.h"
#include "static_assert.h"
#include "types.h"
#include "battle/battle_action_run.h"
#include "battle/battle_unit.h"
#include "battle/playback_queue.h"
#include "stubs.h"

struct UNIT;

/* Levels between two points of a growth curve (CHARACTER_DEFINITION.hp and its
   siblings). */
#define GROWTH_BAND_LEVELS 20
/* Points on a growth curve: levels 0 to 100, GROWTH_BAND_LEVELS apart. */
#define GROWTH_POINTS	   6

/* A character's definition: the growth curves of its stats, its element bonus,
   starting level and starting equipment. */
typedef struct CHARACTER_DEFINITION {
	Uint8 reserved[80]; /* twenty words, 0 in every row; read by no code */
	/* The growth curves: each stat at levels 0, 20, 40, 60, 80 and 100. A level
	   up adds a twentieth of the step around the new level, with a random part;
	   level 1 adds the first point (gmUnitLevelUp). */
	Sint16 hp[GROWTH_POINTS];
	Sint16 pp[GROWTH_POINTS];
	Uint16 attack[GROWTH_POINTS];
	Uint16 defense[GROWTH_POINTS];
	Uint16 agility[GROWTH_POINTS];
	Uint8  luck[GROWTH_POINTS];
	/* Added to ten times the Djinn set in each element to give the character's
	   element levels (gmUnitGetElementLevels). */
	Uint8 elementBonus[4];
	Uint8 startingLevel; /* the level a new game raises the character to */
	Uint8 paddingBeforeItems;
	/* The starting items, each added and equipped at a new game; the low nine
	   bits are the item. */
	Uint16 items[13];
	Uint8  paddingAfterItems[2];
} CHARACTER_DEFINITION;

/* An ability a class teaches, and the level it is learned at. */
typedef struct CLASS_ABILITY {
	Uint8 ability; /* the action taught, 0 for none */
	Uint8 level;   /* the level it is taught at */
	Uint8 padding[2];
} CLASS_ABILITY;

/* CLASS_DEFINITION.series for the series a character's elements can choose
   (ClassSeriesByElements), each named by its first class, and two that no pair
   of elements chooses. */
typedef enum CLASS_SERIES {
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
	/* The classes gmClassDetermine gives outright, whatever the elements. */
	CLASS_SERIES_FIXED,
	/* Class 0, "NPC", an enemy's (CLASS_NONE). */
	CLASS_SERIES_NPC
} CLASS_SERIES;

/* The classes of CLASS_SERIES_FIXED, the last rows of ClassDefinitionTable. */
typedef enum FIXED_CLASS {
	/* Isaac's while FLAG_CHILDHOOD is set; the text names it "?". */
	CLASS_ISAAC_CHILD = 200,
	/* Garet's while FLAG_CHILDHOOD is set; the text names it "?". */
	CLASS_GARET_CHILD,
	/* Jenna's, "Flame User", whatever her elements. */
	CLASS_FLAME_USER,
	CLASS_COUNT
} FIXED_CLASS;

/* How many actions a class can teach (CLASS_DEFINITION.abilities). */
#define CLASS_ABILITY_SLOTS 16

/* A class: its series, the element levels it needs, its stat multipliers and
   the abilities it teaches. */
typedef struct CLASS_DEFINITION {
	/* The series the class belongs to; gmClassDetermine picks the series from
	   the character's two strongest elements. */
	long series;
	/* The element levels the class needs, in tens: the last class of the series
	   the character meets in all four is its class. */
	Uint8 required[4];
	Uint8 hp;	   /* tenths of the base stat */
	Uint8 pp;	   /* tenths of the base stat */
	Uint8 attack;  /* tenths of the base stat */
	Uint8 defense; /* tenths of the base stat */
	Uint8 agility; /* tenths of the base stat */
	Uint8 luck;	   /* tenths of the base stat */
	Uint8 padding[2];
	CLASS_ABILITY
	abilities[CLASS_ABILITY_SLOTS]; /* the Psynergy the class teaches */
	/* [0] to [2]: the ability effects the class is weak to
	   (gmBattleAbilityIsWeakToEffect); [3] is 0 in every row and not read. */
	Uint8 weakEffects[4];
} CLASS_DEFINITION;

/* A Djinni: its ability and the stat bonuses it gives while set. */
typedef struct DJINN_DEFINITION {
	Uint16 abilityId;				/* the action the Djinni unleashes */
	Uint8  reservedAfterAbility[2]; /* reserved; no code names it */
	/* Added to the base stats while the Djinni is set (gmUnitCalculate), before
	   the class multiplies them. */
	Sint8 hp;
	Sint8 pp;
	Sint8 attack;
	Sint8 defense;
	Sint8 agility;
	Sint8 luck;
	Uint8 reservedAfterStats[2];
} DJINN_DEFINITION;

/* ENEMY_DEFINITION.targetStrategy: a single-target attacker favours the target
   with the most HP left, the most maximum HP, or no one (btAIChooseTarget). The
   same column steers btAIChooseAction: with any strategy but
   TARGET_STRATEGY_MOST_HP an enemy skips Psynergy it lacks the PP for, and with
   TARGET_STRATEGY_RANDOM any Psynergy while sealed; with
   TARGET_STRATEGY_MOST_HP, an enemy that picks an item place with no items left
   still commands COMMAND_ITEM. */
typedef enum ENEMY_TARGET_STRATEGY {
	TARGET_STRATEGY_MOST_HP = 0,
	TARGET_STRATEGY_MOST_MAX_HP = 1,
	TARGET_STRATEGY_RANDOM = 2
} ENEMY_TARGET_STRATEGY;

/* ENEMY_DEFINITION.actionStrategy: how btAIChooseAction picks a place in the
   enemy's eight actions. The first three roll against BattleAIActionWeights;
   the next two remember the place in UNIT.aiState. No row of enemy_table.c has
   a strategy past ACTION_STRATEGY_RANDOM_START. */
typedef enum ENEMY_ACTION_STRATEGY {
	ACTION_STRATEGY_EVEN,			/* 32 in 256 each */
	ACTION_STRATEGY_FALLING,		/* from 53 in 256 down to 11 */
	ACTION_STRATEGY_FIRST_FAVOURED, /* the first 113 in 256, down to 5 */
	/* A random place the first time, then each in turn at the enemy's turn. */
	ACTION_STRATEGY_RANDOM_START,
	ACTION_STRATEGY_IN_ORDER,  /* each in turn from the remembered place */
	ACTION_STRATEGY_NEXT_PICK, /* the next place at each new pick, from 0 */
	ACTION_STRATEGY_SAME	   /* the place last picked */
} ENEMY_ACTION_STRATEGY;

/* An enemy's definition: its level and stats, items, element preset, the
   actions it chooses from and its rewards. */
typedef struct ENEMY_DEFINITION {
	/* Blank in every row: gmBattleUnitSet takes the enemy's name from the
	   messages instead. */
	Uint8 name[15];
	Uint8 level;
	/* The base stats gmBattleUnitSet copies into the unit. */
	Uint16 hp;
	Uint16 pp;
	Uint16 attack;
	Uint16 defense;
	Uint16 agility;
	Uint8  luck;
	Uint8  actionCount; /* actions a turn (UNIT.baseActionCount) */
	Uint8  hpRegen;		/* HP restored at each turn's end (UNIT.baseHpRegen) */
	Uint8  ppRegen;		/* PP restored at each turn's end (UNIT.basePpRegen) */
	Uint8  reservedBeforeAiState[2]; /* 0 in every row */
	/* 0 in every row; the unit's first UNIT.aiState. */
	BATTLE_AI_STATE aiState;
	Uint8			reservedAfterAiState[4]; /* 0 in every row */
	Uint16			items[4];	   /* items the enemy carries, 0 for none */
	Uint8			counts[4];	   /* how many of each item it carries */
	Uint8			elementPreset; /* row of EnemyElementPresetTable, 0 to 43 */
	/* Whom a single-target attack favours: an ENEMY_TARGET_STRATEGY. */
	Sint8 targetStrategy;
	/* How the action is chosen from `abilities` (btAIChooseAction):
	   an ENEMY_ACTION_STRATEGY. */
	Sint8 actionStrategy;
	/* One bit per place in `abilities` (ITEM_ACTION): the enemy uses an item it
	   carries there instead. */
	Uint8  itemActions;
	Uint16 abilities[8]; /* the abilities the enemy chooses from */
	/* [0] to [2]: the ability effects the enemy is weak to
	   (gmBattleAbilityIsWeakToEffect); [3] is 0 in every row. */
	Uint8  weakEffects[4];
	Uint16 coins; /* coins won */
	Sint16 item;  /* the item it may leave, 0 for none */
	/* The chance of the item: 1 in 2 to the power itemChance - 1, two powers
	   better with btEnemyRecordDefeat's Djinn bonus; 0 for none. */
	Sint16 itemChance;
	Uint16 experience; /* experience won */
} ENEMY_DEFINITION;

/* ENEMY_DEFINITION.itemActions' bit for `place` in its abilities. */
#define ITEM_ACTION(place) (1 << (place))

/* A level up's result: the level reached and the gain in each stat. The caller
   provides it; gmUnitLevelUp fills it, and btAwardSpoils reports each gain that
   is not zero. */
typedef struct LEVEL_UP_RESULT {
	Sint16 level; /* the level reached; the old level when it was already 99 */
	/* Always 0xffff: gmUnitLevelUp stores no ability here, and the spoils find
	   a new one by comparing the unit's actions with a copy taken before. */
	Uint16 ability;
	/* The gain in each base stat; the spoils read them as signed. */
	Uint16 hp;
	Uint16 pp;
	Uint16 attack;
	Uint16 defense;
	Uint16 agility;
	Uint16 luck;
} LEVEL_UP_RESULT;

STATIC_ASSERT(sizeof(CHARACTER_DEFINITION) == 180);
STATIC_ASSERT(offsetof(CHARACTER_DEFINITION, hp) == 80);
STATIC_ASSERT(offsetof(CHARACTER_DEFINITION, startingLevel) == 150);
STATIC_ASSERT(offsetof(CHARACTER_DEFINITION, items) == 152);
STATIC_ASSERT(sizeof(CLASS_DEFINITION) == 84);
STATIC_ASSERT(offsetof(CLASS_DEFINITION, hp) == 8);
STATIC_ASSERT(sizeof(CLASS_ABILITY) == 4);
STATIC_ASSERT(offsetof(CLASS_DEFINITION, abilities) == 16);
STATIC_ASSERT(sizeof(DJINN_DEFINITION) == 12);
STATIC_ASSERT(offsetof(DJINN_DEFINITION, abilityId) == 0);
STATIC_ASSERT(offsetof(DJINN_DEFINITION, reservedAfterAbility) == 2);
STATIC_ASSERT(offsetof(DJINN_DEFINITION, hp) == 4);
STATIC_ASSERT(sizeof(ENEMY_DEFINITION) == 84);
STATIC_ASSERT(offsetof(ENEMY_DEFINITION, aiState) == 32);
STATIC_ASSERT(offsetof(ENEMY_DEFINITION, actionStrategy) == 54);
STATIC_ASSERT(offsetof(ENEMY_DEFINITION, itemActions) == 55);
STATIC_ASSERT(offsetof(ENEMY_DEFINITION, abilities) == 56);
STATIC_ASSERT(offsetof(ENEMY_DEFINITION, coins) == 76);
STATIC_ASSERT(offsetof(ENEMY_DEFINITION, experience) == 82);
STATIC_ASSERT(sizeof(LEVEL_UP_RESULT) == 16);

/* The eight characters' definitions, by character id
   (game/character_table.c). */
extern const CHARACTER_DEFINITION CharacterDefinitionTable[];
/* The classes, by class index (game/class_table.c, 203 entries). */
extern const CLASS_DEFINITION ClassDefinitionTable[];

#endif
