/// @file
/// Sol Sanctum (SolSanctumCorridor, scene 15), a passage between two parts of
/// the sanctum's rooms inside its door, scene 16 (sol_sanctum_entrance.c),
/// which Isaac, Garet, Jenna and Kraden explore together.
///
/// It places nobody and holds no event of its own: its only rows are its two
/// exits, the passage's two ends, which both lead back to scene 16
/// (SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_ENTRANCE_1_EAST and
/// SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_ENTRANCE_1_WEST), and the party comes
/// back in through the same doors. Its setup is the one it shares with scene 14
/// (sol_sanctum_maze.c), SolSanctumRooms_Initialize: once Mt. Aleph has erupted
/// (FLAG_SOL_SANCTUM_ERUPTED), the passage rumbles and shakes.

#include "unit/scene/sol_sanctum_corridor.h"
#include "unit/scene/common/sol_sanctum_init_zoom.h"
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
#include "scene.h"

#include "resource_37de_scene.h"
#include "field_scene.h"
#include "field_obj.h"

#include "field_event.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSolSanctumCorridorEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSolSanctumCorridorExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gSolSanctumCorridorPlacements;
}

/// Returns the passage's two exits, its only events.
const struct SceneEvent *Scene_GetEvents(void) {
  return gSolSanctumCorridorEvents;
}

#include "common/sol_sanctum_init_zoom.h"

struct SceneEntrance gSolSanctumCorridorEntrances[] = {
    {0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_ENTRANCE_1_EAST, CONDITION_ALWAYS,
     727, 0, 92, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_ENTRANCE_1_WEST, CONDITION_ALWAYS,
     56, 0, 616, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumCorridorExits[] = {
    SCENE_EXITS(SolSanctumCorridor),
    SCENE_EXIT(SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_ENTRANCE_1_EAST,
               SolSanctumEntrance1,
               SOL_SANCTUM_ENTRANCE_1_FROM_SOL_SANCTUM_CORRIDOR_EAST),
    SCENE_EXIT(SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_ENTRANCE_1_WEST,
               SolSanctumEntrance1,
               SOL_SANCTUM_ENTRANCE_1_FROM_SOL_SANCTUM_CORRIDOR_WEST),
    SCENE_EXITS_END,
};

struct ScenePlacement gSolSanctumCorridorPlacements[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gSolSanctumCorridorEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS,
     SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_ENTRANCE_1_EAST},
    {EVENT_EXIT, 2, CONDITION_ALWAYS,
     SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_ENTRANCE_1_WEST},
    {SCENE_EVENTS_END},
};
