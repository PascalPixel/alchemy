/// @file
/// Sol Sanctum (SceneId_SolSanctumPsynergyStone), a room off the statue room
/// (SceneId_SolSanctumStatue) that holds a Psynergy Stone.
///
/// Its one exit, SOL_SANCTUM_PSYNERGY_STONE_TO_SOL_SANCTUM_STATUE, leads back
/// to the statue room, and it places nobody.
///
/// Searching in it finds the Psynergy Stone, which restores all PP. The stone
/// is kept by an area flag (FLAG_SOL_SANCTUM_PSYNERGY_STONE_TAKEN) rather than
/// a story flag, so Scene_Arrive puts it back whenever the party enters another
/// area.
///
/// Retreat does not work here. Once Mt. Aleph has erupted, the room rumbles and
/// shakes, as the rest of the sanctum does.

#include "types.h"
#include "field/scene_tables.h"

extern struct SceneEntrance gSolSanctumPsynergyStoneEntrances[];
extern u32 gSolSanctumPsynergyStoneExits[];
extern struct ActorPlacement gSolSanctumPsynergyStoneActorPlacements[];
extern struct Trigger gSolSanctumPsynergyStoneTriggers[];

#include "battle/battle.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"

#include "field/event.h"
#include "flags.h"

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.
#define FLAG_SOL_SANCTUM_PSYNERGY_STONE_TAKEN FLAG_AREA_TEMP_50

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

/// Opens the map with the window transition and blocks Retreat.
s32 Scene_Initialize(void) {
  s32 shakeX = px(1);
  s32 shakeY = px(1);

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);

  Import_GameFlagSet(FLAG_RETREAT_BLOCKED);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Scene_QueueMusic(SE_RUMBLE);
    MapGrid_SetScreenShake(shakeX, shakeY, Q16_ONE);
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
    {TRIGGER_TABLE_END},
};
