#ifndef ALCHEMY_ENEMY_ELEMENT_PRESET_H
#define ALCHEMY_ENEMY_ELEMENT_PRESET_H

/* The enemies' element presets: the element an enemy attacks with, and its
   level, power and resistance in each element. */

#include "types.h"
#include "battle/battle_unit.h"

/* An enemy's element preset in EnemyElementPresetTable: the element it attacks
   with, its level in each element, and its base power and resistance in each
   element. */
typedef struct ENEMY_ELEMENT_PRESET {
	/* The element of its attacks, ELEMENT_NONE for most
	   (gmEnemyGetAttackElement). */
	Sint32 attackElement;
	/* Its level in each element; gmUnitGetElementLevels returns ten times
	   each. */
	Uint8 elementLevels[4];
	/* Its base power and resistance in each element, as
	   gmUnitCalculateBaseElements gives them to the unit. */
	BATTLE_ELEMENT_STATS stats[4];
} ENEMY_ELEMENT_PRESET;

/* The rows of EnemyElementPresetTable (ENEMY_DEFINITION.elementPreset), each
   named by the first enemy that has it. Rows 6, 10, 18, 22, 28 and 35 are
   spares no enemy has. */
typedef enum ENEMY_ELEMENT_PRESET_ID {
	/* The row an out-of-range preset reads as; only the unused "???" has it. */
	ELEMENT_PRESET_DEFAULT = 0,
	ELEMENT_PRESET_BANDIT = 1,
	ELEMENT_PRESET_AZART = 2,
	ELEMENT_PRESET_VERMIN = 3,
	ELEMENT_PRESET_GRISLY = 4,
	ELEMENT_PRESET_SPIDER = 5,
	ELEMENT_PRESET_WILD_MUSHROOM = 7,
	ELEMENT_PRESET_TRET = 8,
	ELEMENT_PRESET_DRONE_BEE = 9,
	ELEMENT_PRESET_BAT = 11,
	ELEMENT_PRESET_HARRIDAN = 12,
	ELEMENT_PRESET_ZOMBIE = 13,
	ELEMENT_PRESET_SKELETON = 14,
	ELEMENT_PRESET_GHOST = 15,
	ELEMENT_PRESET_LICH = 16,
	ELEMENT_PRESET_GARGOYLE = 17,
	ELEMENT_PRESET_CEREBUS = 19,
	ELEMENT_PRESET_MANTICORE = 20,
	ELEMENT_PRESET_TORNADO_LIZARD = 21,
	ELEMENT_PRESET_EARTH_LIZARD = 23,
	ELEMENT_PRESET_SLIME = 24,
	ELEMENT_PRESET_LIVING_STATUE = 25,
	ELEMENT_PRESET_STONE_SOLDIER = 26,
	ELEMENT_PRESET_SALAMANDER = 27,
	ELEMENT_PRESET_THUNDER_LIZARD = 29,
	ELEMENT_PRESET_GNOME_WIZARD = 30,
	ELEMENT_PRESET_ORC_LORD = 31,
	ELEMENT_PRESET_RAT_SOLDIER = 32,
	ELEMENT_PRESET_SIREN = 33,
	ELEMENT_PRESET_GNOME = 34,
	ELEMENT_PRESET_GNOME_MAGE = 36,
	ELEMENT_PRESET_NIGHTMARE = 37,
	ELEMENT_PRESET_MIMIC = 38,
	ELEMENT_PRESET_SATUROS = 39,
	ELEMENT_PRESET_MENARDI = 40,
	ELEMENT_PRESET_FUSION_DRAGON = 41,
	ELEMENT_PRESET_DEADBEARD = 42,
	ELEMENT_PRESET_GRYPHON = 43
} ENEMY_ELEMENT_PRESET_ID;

/* How many rows EnemyElementPresetTable has; an enemy's preset past them reads
   as row 0. */
#define ENEMY_ELEMENT_PRESET_COUNT 44

#endif
