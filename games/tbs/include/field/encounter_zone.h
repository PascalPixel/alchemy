#ifndef ALCHEMY_ENCOUNTER_ZONE_H
#define ALCHEMY_ENCOUNTER_ZONE_H

// The encounter zones.

#include "types.h"

// The number of formations an encounter zone can hold.
enum
{
	ENCOUNTER_GROUP_COUNT = 8
};

// An encounter zone, a row of EncounterZones: its rate, the level it is made for, and the
// formations that can appear with their chances. 28 bytes, the 14 halfwords
// fdEncounterGetFormationAtSlot counts a zone as.
typedef struct ENCOUNTER_ZONE
{
	// The base encounter rate; 0 for zone 0 (no encounters) and for a zone only scripted battles,
	// mimics and Djinn use.
	Uint16 rate;
	Uint16 level; // the party level the zone is made for
	// The formations that can appear, by formation id (battle/formations.c).
	Uint16 formations[ENCOUNTER_GROUP_COUNT];
	Uint8  weights[ENCOUNTER_GROUP_COUNT]; // each formation's relative chance
} ENCOUNTER_ZONE;

#endif
