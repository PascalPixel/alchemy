#ifndef ALCHEMY_DEBUG_PARAMETER_SET_H
#define ALCHEMY_DEBUG_PARAMETER_SET_H

/* The debug parameter sets. */

#include "types.h"

/* A row of the debug parameter sets: a party member, its level, its Djinn of
   each element, its equipped items and two Psynergy it learns. A row whose
   party member is DEBUG_PARAMETER_SET_END ends a parameter set. */
typedef struct DEBUG_PARTY_ENTRY {
	Sint8  owner;
	Uint8  level;
	Sint8  djinnCounts[4];
	Uint16 items[6];
	Uint16 actions[2];
	Uint16 unused; /* 0 in every row; nothing reads it */
} DEBUG_PARTY_ENTRY;

/* The owner that ends the debug parameter sets. */
#define DEBUG_PARAMETER_SET_END -1

/* The debug parameter sets btDebugParamSetLoad loads, a party member a row
   (debug_parameter_sets.c). */
extern DEBUG_PARTY_ENTRY DebugParameterSets[];

#endif
