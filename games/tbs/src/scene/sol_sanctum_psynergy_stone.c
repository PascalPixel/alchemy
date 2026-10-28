/// @file
/// Sol Sanctum (SceneId_SolSanctumPsynergyStone), a room off the statue room
/// (SceneId_SolSanctumStatue) that holds a Psynergy Stone.
///
/// Its one exit, SOL_SANCTUM_PSYNERGY_STONE_TO_SOL_SANCTUM_STATUE, leads back
/// to the statue room, and it places nobody. Searching in it finds the
/// Psynergy Stone, which restores all PP. The stone is kept by an area
/// flag (FLAG_SOL_SANCTUM_PSYNERGY_STONE_TAKEN) rather than a story flag, so
/// Scene_Arrive puts it back whenever the party enters another
/// area. Retreat does not work here, and once Mt. Aleph has erupted the
/// room rumbles and shakes, as the rest of the sanctum does.

#include "module/scene/sol_sanctum_psynergy_stone.h"
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

#include "resource_37c_runtime.h"
#include "scene_tables.h"

#include "event.h"
#include "flags.h"
#include "sounds.h"
#include "fixed_math.h"

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.
/// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
#define FLAG_SOL_SANCTUM_PSYNERGY_STONE_TAKEN 0x350

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSolSanctumPsynergyStoneEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSolSanctumPsynergyStoneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gSolSanctumPsynergyStoneActorPlacements;
}

/// The exit and the Psynergy Stone.
const struct Trigger *Scene_GetTriggers(void) {
  return gSolSanctumPsynergyStoneTriggers;
}

/// Opens the map with the window transition and blocks Retreat. Once Mt.
/// Aleph has erupted (FLAG_SOL_SANCTUM_ERUPTED), the room rumbles (SE_RUMBLE)
/// and shakes gently, as the rest of the sanctum does.
s32 Scene_Initialize(void) {
  s32 shakeX = PIXELS(1);
  s32 shakeY = PIXELS(1);
  s32 decay = Q16_ONE;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);

  Import_GameFlagSet(FLAG_RETREAT_BLOCKED);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Scene_QueueMusic(SE_RUMBLE);
    MapGrid_SetScreenShake(shakeX, shakeY, decay);
    FieldEffect_InitializeSparkles();
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

struct ActorPlacement gSolSanctumPsynergyStoneActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct Trigger gSolSanctumPsynergyStoneTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     SOL_SANCTUM_PSYNERGY_STONE_TO_SOL_SANCTUM_STATUE},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 100,
     FLAG_SOL_SANCTUM_PSYNERGY_STONE_TAKEN, FIND_PSYNERGY_STONE},
    {TRIGGERS_END},
};
