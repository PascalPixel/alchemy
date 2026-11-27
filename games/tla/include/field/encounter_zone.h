#ifndef ALCHEMY_ENCOUNTER_ZONE_H
#define ALCHEMY_ENCOUNTER_ZONE_H

// A row of the encounter zone table, EncounterZones (encounter_zones.c).

#include "types.h"

// An encounter zone, a row of EncounterZones: its rate, the level it is made for, and the
// formations that can appear with their chances.
typedef struct ENCOUNTER_ZONE
{
    // The base encounter rate; 0 for a zone with no random encounters.
    Uint16 rate;
    Uint16 level;         // the party level the zone is made for
    Sint16 formations[8]; // the formations that can appear, by formation id
    Uint8  weights[8];    // each formation's relative chance
} ENCOUNTER_ZONE;

#endif
