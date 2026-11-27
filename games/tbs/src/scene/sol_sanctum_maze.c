/* @file */
/* Sol Sanctum (SceneId_SolSanctumMaze), the room that holds the second Small */
/* Jewel. */
/* */
/* It is reached from two rooms of SceneId_SolSanctumReliefs */
/* (sol_sanctum_entrance.c) and leads back to them. */
/* SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_SOUTH goes to the room of steps off
 */
/* the statue hall, and SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_NORTH to */
/* the second relief room. */
/* */
/* It places nobody. Its one event besides the exits is the chest */
/* (FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND), whose jewel opens the second */
/* minotaur relief. */
/* */
/* Its setup, scInit (shared/sol_sanctum_rumble.c), is */
/* the one SceneId_SolSanctumCorridor shares. Once Mt. Aleph has erupted, the */
/* room rumbles and shakes. */
/* */
/* The south exit leaves down stairs (EXIT_STYLE_STAIRS_DOWN). Walking north */
/* into its arch, the leader steps in and sinks out of sight as if down steps */
/* behind it, to SE_STAIRS_DOWN. He comes out walking down from a doorway in */
/* the room of steps. */

#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "graphics/graphics.h"

extern SCENE_ENTRANCE SolSanctumMazeEntrances[];
extern u32 SolSanctumMazeExits[];
extern ACTOR_PLACE SolSanctumMazeActorPlacements[];
extern TRIGGER SolSanctumMazeTriggers[];

const SCENE_ENTRANCE *scGetEntrances(void) { return SolSanctumMazeEntrances; }

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

const u32 *scGetExits(void) { return SolSanctumMazeExits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	return SolSanctumMazeActorPlacements;
}

/* The exits, and the Small Jewel's chest. */
const TRIGGER *scGetTriggers(void) { return SolSanctumMazeTriggers; }

#include "shared/sol_sanctum_rumble.c"

SCENE_ENTRANCE SolSanctumMazeEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_SOUTH, CONDITION_ALWAYS, 424, 0,
	 615, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_NORTH, CONDITION_ALWAYS, 376, 0,
	 53, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 SolSanctumMazeExits[] = {
	SCENE_EXITS(SolSanctumMaze),
	SCENE_EXIT(SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_SOUTH, SolSanctumReliefs,
			   SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_WEST),
	SCENE_EXIT(SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_NORTH, SolSanctumReliefs,
			   SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST),
	SCENE_EXITS_END,
};

ACTOR_PLACE SolSanctumMazeActorPlacements[] = {
	{SCENE_TABLE_END},
};

TRIGGER SolSanctumMazeTriggers[] = {
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_SOUTH, CONDITION_ALWAYS,
	 SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_SOUTH},
	{TRIGGER_EXIT, SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_NORTH,
	 CONDITION_ALWAYS, SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_NORTH},
	{EXAMINE_TRIGGER(EXAMINE_CHEST), 100,
	 FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND, FIND_ITEM(ITEM_SMALL_JEWEL)},
	{TRIGGER_TABLE_END},
};
