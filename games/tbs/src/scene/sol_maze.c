#include "unit/scene/sol_maze.h"
#include "unit/scene/common/sol_initzoom.h"
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

// A maze of Sol Sanctum, scene 14: two exits to scene 16 and one chest.

// Where the party can enter the maze.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSolMazeEntrances;
}

// This maze has no exit regions.
const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

// Where each exit leads.
const u32 *Scene_GetExits(void) { return gSolMazeExits; }

// Returns the empty placement table; this maze has no placed actors.
const struct ScenePlacement *Scene_GetPlacements(void) {
  return gSolMazePlacements;
}

// The exits, and the maze's chest.
const struct SceneEvent *Scene_GetEvents(void) { return gSolMazeEvents; }

// Shared scene code, included where the ROM places it.
#include "common/sol_initzoom.h"

struct SceneEntrance gSolMazeEntrances[] = {
    {0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 424, 0, 615, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 376, 0, 53, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSolMazeExits[] = {
    SCENE_EXITS(14),
    SCENE_EXIT(1, 16, 7),
    SCENE_EXIT(2, 16, 14),
    SCENE_EXITS_END,
};

struct ScenePlacement gSolMazePlacements[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gSolMazeEvents[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {SEARCH(SEARCH_CHEST), 100, 0xf02, FIND_ITEM(0xe1)},
    {SCENE_EVENTS_END},
};
