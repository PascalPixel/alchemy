/// @file
/// Sol Sanctum (SceneId_SolSanctumPsynergyStone), a room that holds a Psynergy
/// Stone.
///
/// It opens off the room of the statue puzzle (SceneId_SolSanctumStatue), whose
/// SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE leads here
/// (SOL_SANCTUM_PSYNERGY_STONE_FROM_SOL_SANCTUM_STATUE); this room's only exit,
/// SOL_SANCTUM_PSYNERGY_STONE_TO_SOL_SANCTUM_STATUE, leads back
/// (SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_PSYNERGY_STONE). Searching in it finds
/// the Psynergy Stone, which restores all PP. The stone is kept by a place flag
/// (FLAG_SOL_SANCTUM_PSYNERGY_STONE_TAKEN, 0x350) rather than a story flag:
/// Scene_ResetFlagsOnEnter clears it whenever the party enters another place
/// group, so the stone is back on every new visit to the sanctum. The room
/// places no actors. Retreat does not work here, and once Mt. Aleph has erupted
/// the room rumbles and shakes gently, as the rest of the sanctum does.

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

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSolSanctumPsynergyStoneEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSolSanctumPsynergyStoneExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gSolSanctumPsynergyStonePlacements;
}

/// The exit and the Psynergy Stone.
const struct SceneEvent *Scene_GetEvents(void) {
  return gSolSanctumPsynergyStoneEvents;
}

/// Opens the scene with the window transition and blocks Retreat. Once Mt.
/// Aleph has erupted (FLAG_SOL_SANCTUM_ERUPTED), the room rumbles (SE_141)
/// and shakes gently, as the rest of the sanctum does.
s32 Scene_Initialize(void) {
  s32 shakeX = PIXELS(1);
  s32 shakeY = PIXELS(1);
  s32 decay = Q16_ONE;

  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);

  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Resource37c_QueueSoundCue(SE_141);
    Engine_WorkSetValuesIfNonNegative(shakeX, shakeY, decay);
    Resource37c_FinalizeSetup();
  }
  return 0;
}

struct SceneEntrance gSolSanctumPsynergyStoneEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 87, 0, 142, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {SOL_SANCTUM_PSYNERGY_STONE_FROM_SOL_SANCTUM_STATUE, CONDITION_ALWAYS, 87,
     0, 142, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumPsynergyStoneExits[] = {
    SCENE_EXITS(SolSanctumPsynergyStone),
    SCENE_EXIT(SOL_SANCTUM_PSYNERGY_STONE_TO_SOL_SANCTUM_STATUE,
               SolSanctumStatue,
               SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_PSYNERGY_STONE),
    SCENE_EXITS_END,
};

struct ScenePlacement gSolSanctumPsynergyStonePlacements[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gSolSanctumPsynergyStoneEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS,
     SOL_SANCTUM_PSYNERGY_STONE_TO_SOL_SANCTUM_STATUE},
    {SEARCH(SEARCH_UNNAMED), 100, FLAG_SOL_SANCTUM_PSYNERGY_STONE_TAKEN,
     FIND_PSYNERGY_STONE},
    {SCENE_EVENTS_END},
};
