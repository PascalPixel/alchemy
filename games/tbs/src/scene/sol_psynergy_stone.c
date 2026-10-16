
#include "unit/scene/sol_psynergy_stone.h"
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

// The Sol Sanctum room that holds a psynergy stone, scene 13.

enum {
  FLAG_144 = 0x144, // set on every entry; what reads it is not known
  FLAG_814 = 0x814  // when set, the room hums and sparkles on entry
};

// Where the party can enter the room.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSolPsynergyStoneEntrances;
}

// The room has no regions.
const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

// Where its one exit leads.
const u32 *Scene_GetExits(void) { return gSolPsynergyStoneExits; }

// Nobody is placed here.
const struct ScenePlacement *Scene_GetPlacements(void) {
  return gSolPsynergyStonePlacements;
}

// The exit and the psynergy stone.
const struct SceneEvent *Scene_GetEvents(void) {
  return gSolPsynergyStoneEvents;
}

// Open with a window transition and restore the stone's sparkle effect.
s32 Scene_Initialize(void) {
  s32 scaleX = 0x10000;
  s32 scaleY = 0x10000;
  s32 scaleZ = 0x10000;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Engine_GameFlagSet(FLAG_144);
  // The persistent flag enables both the sound and the initialized sparkles.
  if (Engine_GameFlagIsSet(FLAG_814) != 0) {
    Resource37c_QueueSoundCue(141);
    Engine_WorkSetValuesIfNonNegative(scaleX, scaleY, scaleZ);
    Resource37c_FinalizeSetup();
  }
  return 0;
}

struct SceneEntrance gSolPsynergyStoneEntrances[] = {
    {0, CONDITION_ALWAYS, 87, 0, 142, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 87, 0, 142, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSolPsynergyStoneExits[] = {
    SCENE_EXITS(13),
    SCENE_EXIT(1, 12, 2),
    SCENE_EXITS_END,
};

struct ScenePlacement gSolPsynergyStonePlacements[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gSolPsynergyStoneEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {SEARCH(SEARCH_UNNAMED), 100, 0x350, FIND_PSYNERGY_STONE},
    {SCENE_EVENTS_END},
};
