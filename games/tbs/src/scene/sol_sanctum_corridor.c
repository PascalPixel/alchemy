/* @file */
/* Sol Sanctum (SceneId_SolSanctumCorridor), a passage that joins two rooms of
 */
/* SceneId_SolSanctumReliefs (sol_sanctum_entrance.c). */
/* */
/* It places nobody and has no events but its two exits, one at each end. */
/* SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST leads into the room with */
/* the chest, and SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST into the */
/* room with the stepping stones. */
/* */
/* Its setup, scInit (shared/sol_sanctum_rumble.c), is */
/* the one SceneId_SolSanctumMaze shares. Once Mt. Aleph has erupted */
/* (FLAG_SOL_SANCTUM_ERUPTED), the passage rumbles and shakes. */

#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "graphics/graphics.h"

extern SCENE_ENTRANCE SolSanctumCorridorEntrances[];
extern u32 SolSanctumCorridorExits[];
extern ACTOR_PLACE SolSanctumCorridorActorPlacements[];
extern TRIGGER SolSanctumCorridorTriggers[];

const SCENE_ENTRANCE *scGetEntrances(void) {
	return SolSanctumCorridorEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

const u32 *scGetExits(void) { return SolSanctumCorridorExits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	return SolSanctumCorridorActorPlacements;
}

/* Returns the passage's two exits, its only events. */
const TRIGGER *scGetTriggers(void) { return SolSanctumCorridorTriggers; }

#include "shared/sol_sanctum_rumble.c"

SCENE_ENTRANCE SolSanctumCorridorEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_EAST, CONDITION_ALWAYS, 727,
	 0, 92, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_WEST, CONDITION_ALWAYS, 56,
	 0, 616, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 SolSanctumCorridorExits[] = {
	SCENE_EXITS(SolSanctumCorridor),
	SCENE_EXIT(SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST,
			   SolSanctumReliefs,
			   SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_EAST),
	SCENE_EXIT(SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST,
			   SolSanctumReliefs,
			   SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST),
	SCENE_EXITS_END,
};

ACTOR_PLACE SolSanctumCorridorActorPlacements[] = {
	{SCENE_TABLE_END},
};

TRIGGER SolSanctumCorridorTriggers[] = {
	{TRIGGER_EXIT, 1, CONDITION_ALWAYS,
	 SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS,
	 SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST},
	{TRIGGER_TABLE_END},
};
