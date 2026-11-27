#ifndef ALCHEMY_WORLD_MAP_ENCOUNTER_RULES_H
#define ALCHEMY_WORLD_MAP_ENCOUNTER_RULES_H

/* @file */
/* Which encounter zone each part of the World Map takes, and the Djinn */
/* battles' formations. */

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
	/* battle background (fdBattleBackgroundSelectForEnvironment), or -1 for
	 * any. */
	s16 environment;
	s16 flg;  /* -1, or a flag that turns the rule off once set */
	s16 zone; /* a row of encounter_zones.c */
} WORLD_MAP_ENCOUNTER_RULE;

/* A Djinni battle: the formation in slot `slot` of encounter zone `zone`. */
typedef struct DJINNI_BATTLE {
	u16 zone;
	u16 slot;
} DJINNI_BATTLE;

extern const WORLD_MAP_ENCOUNTER_RULE WorldMap_EncounterRules[];
extern const DJINNI_BATTLE Encounter_DjinnBattles[];

#endif
