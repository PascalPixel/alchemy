
#include "unit/scene/sol_sanctum_psynergy_stone.h"
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

#include "resource_37c_runtime.h"
#include "field_scene.h"

#include "field_event.h"
#include "flags.h"
#include "songs.h"
#include "fixed_math.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.
#define FLAG_SOL_SANCTUM_PSYNERGY_STONE_TAKEN 0x350

// The Sol Sanctum room that holds a Psynergy Stone, scene 13.

/// Where the party can enter the room.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSolSanctumPsynergyStoneEntrances;
}

/// The room has no regions.
const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

/// Where its one exit leads.
const u32 *Scene_GetExits(void) { return gSolSanctumPsynergyStoneExits; }

/// Nobody is placed here.
const struct ScenePlacement *Scene_GetPlacements(void) {
  return gSolSanctumPsynergyStonePlacements;
}

/// The exit and the psynergy stone.
const struct SceneEvent *Scene_GetEvents(void) {
  return gSolSanctumPsynergyStoneEvents;
}

/// The room opens through a window. After the eruption it rumbles and shakes
/// gently, as the rest of the sanctum does.
s32 Scene_Initialize(void) {
  s32 shakeX = PIXELS(1);
  s32 shakeY = PIXELS(1);
  s32 decay = Q16_ONE;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  // Retreat does not work here (flag 0x144).
  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Resource37c_QueueSoundCue(SE_141);
    Engine_WorkSetValuesIfNonNegative(shakeX, shakeY, decay);
    Resource37c_FinalizeSetup();
  }
  return 0;
}

struct SceneEntrance gSolSanctumPsynergyStoneEntrances[] = {
    {0, CONDITION_ALWAYS, 87, 0, 142, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 87, 0, 142, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumPsynergyStoneExits[] = {
    SCENE_EXITS(13),
    SCENE_EXIT(1, 12, 2),
    SCENE_EXITS_END,
};

struct ScenePlacement gSolSanctumPsynergyStonePlacements[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gSolSanctumPsynergyStoneEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {SEARCH(SEARCH_UNNAMED), 100, FLAG_SOL_SANCTUM_PSYNERGY_STONE_TAKEN,
     FIND_PSYNERGY_STONE},
    {SCENE_EVENTS_END},
};
