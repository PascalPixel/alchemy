#ifndef ALCHEMY_WORLD_MAP_ENCOUNTER_RULES_H
#define ALCHEMY_WORLD_MAP_ENCOUNTER_RULES_H

/* @file */
/* Which encounter zone each part of the World Map takes. */

#include "types.h"

/* The World Map's encounter zone for an encounter region and battle */
/* environment. Read by fdEncounterResolveWorldMapZoneAndBackdrop, first match
 */
/* first. */
typedef struct WORLD_MAP_ENCOUNTER_RULE {
	/* The World Map cell's encounter region (WORLD_MAP_ENCOUNTER_REGION); -1
	 */
	/* ends the table. */
	s16 region;
	/* The battle environment (BATTLE_ENVIRONMENT), which also picks the */
	/* battle background, or -1 for any. */
	s16 environment;
	s16 flg;  /* -1, or a flag the rule needs set */
	s16 zone; /* a row of EncounterZones */
} WORLD_MAP_ENCOUNTER_RULE;

extern const WORLD_MAP_ENCOUNTER_RULE WorldMap_EncounterRules[];
/* The same aboard the ship (LEADER_MOVEMENT_SHIP). */
extern const WORLD_MAP_ENCOUNTER_RULE WorldMap_ShipEncounterRules[];

#endif
