

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

// A corridor of Sol Sanctum, scene 15: two exits to scene 16 and nothing
// else.

// Where the party can enter the corridor.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSolSanctumCorridorEntrances;
}

// No regions.
const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

// Where each exit leads.
const u32 *Scene_GetExits(void) { return gSolSanctumCorridorExits; }

// Nobody is placed here.
const struct ScenePlacement *Scene_GetPlacements(void) {
  return gSolSanctumCorridorPlacements;
}

// The two exits.
const struct SceneEvent *Scene_GetEvents(void) { return gSolSanctumCorridorEvents; }

// Shared scene code, included where the ROM places it.
#include "common/sol_sanctum_init_zoom.h"

struct SceneEntrance gSolSanctumCorridorEntrances[] = {
    {0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 727, 0, 92, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 56, 0, 616, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumCorridorExits[] = {
    SCENE_EXITS(15),
    SCENE_EXIT(1, 16, 5),
    SCENE_EXIT(2, 16, 1),
    SCENE_EXITS_END,
};

struct ScenePlacement gSolSanctumCorridorPlacements[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gSolSanctumCorridorEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {SCENE_EVENTS_END},
};
