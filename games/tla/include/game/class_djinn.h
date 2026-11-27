#ifndef ALCHEMY_CLASS_DJINN_H
#define ALCHEMY_CLASS_DJINN_H

/* Classes and Djinn as a unit holds them: the class definitions, the caps on a
   unit's stats, and the functions that set, give and recover Djinn. */

#include "types.h"
#include "battle/battle_unit.h"
#include "battle/battle_side.h"
#include "static_assert.h"

/* An ability a class teaches, and the level it is learned at; The Broken Seal's
   holds the ability in one byte. */
typedef struct CLASS_ABILITY {
	Uint16 ability; /* the action taught, 0 for none */
	Uint8  level;	/* the level it is taught at */
	Uint8  padding; /* padding */
} CLASS_ABILITY;

/* CLASS_DEFINITION.series, each named by its first class. The ten The Broken
   Seal has are the series a character's elements choose
   (ClassSeriesByElements); 11 is in no row, 12 holds class 0, "NPC", an
   enemy's (CLASS_NONE), and the five after are The Lost Age's own, which
   gmClassDetermine gives for a character or an item. */
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
	/* Class 0, "NPC", an enemy's (CLASS_NONE). */
	CLASS_SERIES_NPC = 12,
	CLASS_SERIES_FLAME_USER, /* Jenna's, where the others are Guards */
	CLASS_SERIES_MARINER,	 /* Piers's, where the others are Water Seers */
	CLASS_SERIES_PIERROT,	 /* with the Mysterious Card equipped */
	CLASS_SERIES_TAMER,		 /* with the Trainer's Whip equipped */
	CLASS_SERIES_DARK_MAGE	 /* with the Tomegathericon equipped */
} CLASS_SERIES;

/* The caps on a unit's level and stats (gmUnitCalc). */
#define UNIT_LEVEL_MAX		  99
#define UNIT_ENEMY_HP_MAX	  16383 /* maximum HP, an enemy's */
#define UNIT_ENEMY_PP_MAX	  9999	/* maximum PP, an enemy's */
#define UNIT_CHARACTER_HP_MAX 1999	/* maximum HP and PP, a character's */
#define UNIT_STAT_MAX		  999	/* Attack, Defense and Agility */
#define UNIT_LUCK_MAX		  99
#define UNIT_ELEMENT_MAX	  200 /* elemental power and resistance */
#define UNIT_HP_REGEN_MAX	  10000
#define UNIT_PP_REGEN_MAX	  200

/* How many actions a class can teach (CLASS_DEFINITION.abilities). */
#define CLASS_ABILITY_SLOTS 16

/* How many classes ClassDefinitionTable holds. */
#define CLASS_COUNT 244

/* A class: its series, the element levels it needs, its stat multipliers and
   the abilities it teaches. */
typedef struct CLASS_DEFINITION {
	long  series;	   /* the series gmClassDetermine picks from */
	Uint8 required[4]; /* the whole level needed in each element */
	Uint8 hp;		   /* tenths of the base stat */
	Uint8 pp;		   /* tenths of the base stat */
	Uint8 attack;	   /* tenths of the base stat */
	Uint8 defense;	   /* tenths of the base stat */
	Uint8 agility;	   /* tenths of the base stat */
	Uint8 luck;		   /* tenths of the base stat */
	Uint8 padding[2];  /* padding */
	/* The Psynergy the class teaches. */
	CLASS_ABILITY abilities[CLASS_ABILITY_SLOTS];
	Uint8		  weakEffs[4]; /* the ability effects the class is weak to */
} CLASS_DEFINITION;

extern const CLASS_DEFINITION ClassDefinitionTable[];

STATIC_ASSERT(sizeof(CLASS_DEFINITION) == 84);
STATIC_ASSERT(offsetof(DJINN_RECOVERY_TABLE, list) == 8);
STATIC_ASSERT(offsetof(DJINN_RECOVERY_TABLE, list.cnt) == 296);

CLASS_DEFINITION *gmClassGetDefinition(Sint32 classId);

const struct DJINN_DEFINITION *gmDjinnGetDefinition(
	Uint32 element, Uint32 djinn);

#endif
