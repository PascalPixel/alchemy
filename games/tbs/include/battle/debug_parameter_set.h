#ifndef ALCHEMY_DEBUG_PARAMETER_SET_H
#define ALCHEMY_DEBUG_PARAMETER_SET_H

/* The debug parameter sets. */

#include "types.h"
#include "battle/battle_unit.h"

/* One row of the debug parameter sets: a party member, the level to raise them
   to, the Djinn of each element to give them, the items to equip and up to two
   abilities to teach. A row whose party member is DEBUG_PARAMETER_SET_END ends
   a parameter set. */
typedef struct DEBUG_PARM_SET_ROW {
	Sint8  unitId; /* the party member, or the end of a set */
	Uint8  level;  /* the level the party member is raised to */
	Sint8  djinnCnts[ELEMENT_COUNT]; /* Djinn of each element to give and Set */
	Uint16 items[4];				 /* items to add and equip; 0 for none */
	Uint16 abilities[2];			 /* abilities to teach; 0 for none */
	Uint16 unused;					 /* 0 in every row */
} DEBUG_PARM_SET_ROW;

/* The party member of the row that ends a parameter set. */
#define DEBUG_PARAMETER_SET_END -1

/* The debug menu's parameter sets (debug_parameter_sets.c). */
extern DEBUG_PARM_SET_ROW Debug_ParameterSets[];

#endif
