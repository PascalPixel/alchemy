/// @file
/// A maze of Sol Sanctum, scene 14: two exits to scene 16 and the chest that
/// holds the second Small Jewel (item 0xe1).

#include "unit/scene/sol_sanctum_maze.h"
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
    {0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 424, 0, 615, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 376, 0, 53, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumMazeExits[] = {
    SCENE_EXITS(14),
    SCENE_EXIT(1, 16, 7),
    SCENE_EXIT(2, 16, 14),
    SCENE_EXITS_END,
};

struct ScenePlacement gSolSanctumMazePlacements[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gSolSanctumMazeEvents[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {SEARCH(SEARCH_CHEST), 100, FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND,
     FIND_ITEM(ITEM_SMALL_JEWEL)},
    {SCENE_EVENTS_END},
};
