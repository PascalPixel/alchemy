#ifndef ALCHEMY_WORLD_MAP_ENCOUNTER_RULES_H
#define ALCHEMY_WORLD_MAP_ENCOUNTER_RULES_H

/* Which encounter zone each part of the World Map takes. */

#include "types.h"

/* The World Map's encounter zone for an encounter region and battle
   environment. Read by fdEncounterResolveWorldMapZoneAndBackdrop, first match
   first. */
typedef struct WORLD_MAP_ENCOUNTER_RULE {
	/* The World Map metatile's encounter region (WORLD_MAP_ENCOUNTER_REGION);
	   -1 ends the table. */
	Sint16 region;
	/* The battle environment (BATTLE_ENVIRONMENT), which also picks the battle
	   background, or -1 for any. */
	Sint16 environment;
	Sint16 flags; /* -1, or a flag the rule needs set */
	Sint16 zone;  /* a row of EncounterZones */
} WORLD_MAP_ENCOUNTER_RULE;

extern const WORLD_MAP_ENCOUNTER_RULE WorldMapEncounterRules[];
/* The same aboard the ship (LEADER_MOVEMENT_SHIP). */
extern const WORLD_MAP_ENCOUNTER_RULE WorldMapShipEncounterRules[];

#endif
