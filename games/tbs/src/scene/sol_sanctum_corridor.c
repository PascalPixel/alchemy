/// @file
/// Sol Sanctum (SceneId_SolSanctumCorridor), a passage that joins two rooms of
/// SceneId_SolSanctumReliefs (sol_sanctum_entrance.c).
///
/// It places nobody and has no events but its two exits, one at each end:
/// SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST leads into the room
/// with the chest and SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST into
/// the room with the stepping stones. Its setup, SolSanctumRooms_Initialize
/// (common/sol_sanctum_init_zoom.h), is the one SceneId_SolSanctumMaze shares:
/// once Mt. Aleph has erupted (FLAG_SOL_SANCTUM_ERUPTED), the passage rumbles
/// and shakes.
///
/// Open questions:
/// - None.

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
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_EAST, CONDITION_ALWAYS, 727,
     0, 92, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_WEST, CONDITION_ALWAYS, 56,
     0, 616, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumCorridorExits[] = {
    SCENE_EXITS(SolSanctumCorridor),
    SCENE_EXIT(SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST,
               SolSanctumReliefs,
               SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_EAST),
    SCENE_EXIT(SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST,
               SolSanctumReliefs,
               SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST),
    SCENE_EXITS_END,
};

struct ScenePlacement gSolSanctumCorridorPlacements[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gSolSanctumCorridorEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS,
     SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST},
    {EVENT_EXIT, 2, CONDITION_ALWAYS,
     SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST},
    {SCENE_EVENTS_END},
};
