/// @file
/// Sol Sanctum (MapId_SolSanctumCorridor), a passage that joins two rooms of
/// MapId_SolSanctumReliefs (sol_sanctum_entrance.c).
///
/// It places nobody and has no events but its two warps, one at each end:
/// SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST leads into the room
/// with the chest and SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST into
/// the room with the stepping stones. Its setup, SolSanctumRooms_Initialize
/// (common/sol_sanctum_init_zoom.h), is the one MapId_SolSanctumMaze shares:
/// once Mt. Aleph has erupted (FLAG_SOL_SANCTUM_ERUPTED), the passage rumbles
/// and shakes.

#include "unit/map/sol_sanctum_corridor.h"
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

#include "resource_37de_map.h"
#include "map_tables.h"
#include "object_event.h"

#include "script_layer.h"

const struct MapWarpDest *Map_GetWarpDests(void) {
  return gSolSanctumCorridorWarpDests;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gSolSanctumCorridorWarps; }

const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  return gSolSanctumCorridorObjectEventTemplates;
}

/// Returns the passage's two warps, its only events.
const struct MapEvent *Map_GetEvents(void) { return gSolSanctumCorridorEvents; }

#include "common/sol_sanctum_init_zoom.h"

struct MapWarpDest gSolSanctumCorridorWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_EAST, CONDITION_ALWAYS, 727,
     0, 92, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_WEST, CONDITION_ALWAYS, 56,
     0, 616, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gSolSanctumCorridorWarps[] = {
    MAP_WARPS(SolSanctumCorridor),
    MAP_WARP(SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST,
             SolSanctumReliefs,
             SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_EAST),
    MAP_WARP(SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST,
             SolSanctumReliefs,
             SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gSolSanctumCorridorObjectEventTemplates[] = {
    {MAP_TABLE_END},
};

struct MapEvent gSolSanctumCorridorEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_EAST},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     SOL_SANCTUM_CORRIDOR_TO_SOL_SANCTUM_RELIEFS_WEST},
    {MAP_EVENTS_END},
};
