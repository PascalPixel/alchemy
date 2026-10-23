/// @file
/// Sol Sanctum (SceneId_SolSanctumMaze), the room that holds the second Small
/// Jewel.
///
/// It is reached from two rooms of SceneId_SolSanctumReliefs
/// (sol_sanctum_entrance.c) and leads back to them:
/// SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_SOUTH to the room of steps off the
/// first relief room, and SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_NORTH to the
/// second relief room. It places nobody; its one event besides the exits is
/// the chest (FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND), whose jewel opens the
/// second minotaur relief. Its setup, SolSanctumRooms_Initialize
/// (common/sol_sanctum_init_zoom.h), is the one SceneId_SolSanctumCorridor
/// shares: once Mt. Aleph has erupted, the room rumbles and shakes.
///
/// The south exit leaves down stairs (EXIT_STYLE_ANIMATED_ALTERNATE): walking
/// north into its arch, the leader steps in and sinks out of sight as if down
/// steps behind it, to SE_STAIRS_DOWN, and comes out walking down from a
/// doorway in the room of steps (render probe maze-exit).
///
/// Open questions: none.

#include "unit/map/sol_sanctum_maze.h"
#include "unit/map/common/sol_sanctum_init_zoom.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_offset.h"

#include "resource_37de_map.h"
#include "map_tables.h"
#include "object_event.h"

#include "script_layer.h"
#include "flags.h"
#include "item_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSolSanctumMazeEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSolSanctumMazeExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gSolSanctumMazePlacements;
}

/// The exits, and the Small Jewel's chest.
const struct SceneEvent *Scene_GetEvents(void) { return gSolSanctumMazeEvents; }

#include "common/sol_sanctum_init_zoom.h"

struct SceneEntrance gSolSanctumMazeEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_SOUTH, CONDITION_ALWAYS, 424, 0,
     615, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_NORTH, CONDITION_ALWAYS, 376, 0,
     53, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumMazeExits[] = {
    SCENE_EXITS(SolSanctumMaze),
    SCENE_EXIT(SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_SOUTH, SolSanctumReliefs,
               SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_WEST),
    SCENE_EXIT(SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_NORTH, SolSanctumReliefs,
               SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST),
    SCENE_EXITS_END,
};

struct ScenePlacement gSolSanctumMazePlacements[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gSolSanctumMazeEvents[] = {
    {EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 1, CONDITION_ALWAYS,
     SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_SOUTH},
    {EVENT_EXIT, 2, CONDITION_ALWAYS,
     SOL_SANCTUM_MAZE_TO_SOL_SANCTUM_RELIEFS_NORTH},
    {SEARCH(SEARCH_CHEST), 100, FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND,
     FIND_ITEM(ITEM_SMALL_JEWEL)},
    {SCENE_EVENTS_END},
};
