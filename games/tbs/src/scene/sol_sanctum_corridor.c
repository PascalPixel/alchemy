/* Sol Sanctum (SceneIdSolSanctumCorridor), a passage that joins two rooms of
   SceneIdSolSanctumReliefs (sol_sanctum_entrance.c).

   It places nobody and has no scripts but its two exits, one at each end.
   SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST leads into the room with the
   chest, and SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST into the room
   with the stepping stones.

   Its setup, scInit (shared/sol_sanctum_rumble.c), is the one
   SceneIdSolSanctumMaze shares. Once Mt. Aleph has erupted
   (FLAG_SOL_SANCTUM_ERUPTED), the passage rumbles and shakes. */

#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script.h"
#include "graphics/graphics.h"

extern SCENE_ENTRANCE SolSanctumCorridorEntrances[];
extern Uint32		  SolSanctumCorridorExits[];
extern ACTOR_PLACE	  SolSanctumCorridorActorPlacements[];
extern CUE			  SolSanctumCorridorCues[];

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SolSanctumCorridorEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return SolSanctumCorridorExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return SolSanctumCorridorActorPlacements;
}

/* Returns the passage's two exits, its only scripts. */
const CUE *scGetCues(void)
{
	return SolSanctumCorridorCues;
}

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

Uint32 SolSanctumCorridorExits[] = {
	SCENE_EXITS(SolSanctumCorridor),
	SCENE_EXIT(SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST,
		SolSanctumReliefs, SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_EAST),
	SCENE_EXIT(SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST,
		SolSanctumReliefs, SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST),
	SCENE_EXITS_END,
};

ACTOR_PLACE SolSanctumCorridorActorPlacements[] = {
	{SCENE_TABLE_END},
};

CUE SolSanctumCorridorCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS,
		SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST},
	{CUE_EXIT, 2, CONDITION_ALWAYS,
		SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST},
	{CUE_TABLE_END},
};
