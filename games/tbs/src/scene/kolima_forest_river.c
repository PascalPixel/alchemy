/// @file
/// Kolima Forest's river, between the clearing and the bend. Of its three logs,
/// the one pushed west to the bank tips into the river and floats there, a
/// bridge to the far side.
///
/// FLAG_SCENE_TEMP_01: the log is in the river.

#include "battle_effect_runtime.h"
#include "flags.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/kolima_forest_river.h"
#include "unit/scene/common/kolima_palette.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "types.h"
#include "tbs_edition.h"

#include "staged_actor.h"
#include "staged_actor_effect.h"
#include "field_obj.h"
#include "game_state.h"
#include "field_event.h"
#include "field_scene.h"
#include "songs.h"

/// Actors 8 to 10 are the logs; 10 is the one that can reach the river.
enum { RIVER_LOG = 10 };

#include "common/object_staged_actor.h"

/// Where the party enters from the clearing and the bend.
const struct SceneEntrance *KolimaForestRiver_GetEntrances(void) {
  return gKolimaForestRiverEntrances;
}

/// The river has no regions.
const struct SceneRegion *KolimaForestRiver_GetRegions(void) { return 0; }

/// The paths to the bend and back to the clearing.
const u32 *KolimaForestRiver_GetExits(void) { return gKolimaForestRiverExits; }

/// The three logs.
const struct ScenePlacement *KolimaForestRiver_GetPlacements(void) {
  return gKolimaForestRiverPlacements;
}

/// Walking into a log: it rolls ahead until something stops it. The river
/// log, stopped at the bank (cell column 12), tips over with a splash and
/// floats in the water; its cells become ground the party can cross.
static void KolimaForestRiver_PushLog(void) {
  struct StagedActorProbe result;
  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&result)) {
    SceneActor_MoveAndRedraw(result);
    if (result.actorSlot == RIVER_LOG && (result.positionX >> 20) == 12) {
      struct FieldActor *actor;
      Engine_ActorSetAnimation(RIVER_LOG, 3);
      Engine_ActorSetDestinationOffset(RIVER_LOG, -18, 6);
      Engine_EventWait(30);
      Engine_AudioPlayCue(SE_240);
      Engine_ActorSetAnimation(RIVER_LOG, 8);
      Object_GetById(RIVER_LOG)->priorityFlags = 2;
      Map_CopyCellAttributes(32, 20, 2, 4, 11, 16);
      StagedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4, 0);
      Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
      actor = Object_GetById(RIVER_LOG);
      Engine_ActorSetSpriteFlags(actor, 0);
    }
  }
  Engine_EventEnd();
}

#include "common/kolima_jump.h"

/// The logs, the two paths and the jump east from the bank.
const struct SceneEvent *KolimaForestRiver_GetEvents(void) {
  return gKolimaForestRiverEvents;
}

/// On entry a log already in the river floats there again, the logs mark the
/// cells they cover, and the forest keeps its cursed colours until the curse
/// is lifted.
s32 KolimaForestRiver_Initialize(void) {
  struct EventWork *work = gEventWork;

  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  FieldScene_RedrawActorFootprint(RIVER_LOG);

  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01) != 0) {
    Object_GetById(RIVER_LOG)->priorityFlags = 2;
    Map_CopyCellAttributes(32, 20, 2, 4, 11, 16);
    StagedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(RIVER_LOG), 0);
  }

  FieldScene_RedrawActorFootprint(8);
  FieldScene_RedrawActorFootprint(9);

  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    KolimaPalette_Curse(6);
  }
  return 0;
}

#include "common/kolima_palette.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

struct SceneEntrance gKolimaForestRiverEntrances[] = {
    { 0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 320, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 480, 0, 104, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

/// Exit 1 to the bend (scene 43), exit 2 to the clearing (scene 41).
u32 gKolimaForestRiverExits[] = {
    SCENE_EXITS(42),
    SCENE_EXIT(1, 43, 1),
    SCENE_EXIT(2, 41, 2),
    SCENE_EXITS_END,
};

struct ScenePlacement gKolimaForestRiverPlacements[] = {
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(256), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(224), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(312), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gKolimaForestRiverEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestRiver_PushLog)},
    {TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForest_HopEast)},
    {SCENE_EVENTS_END},
};

// The .bss order follows the first declarations (externs in field.h).
u8 gKolimaPalette_First[1792] __attribute__((section(".bss")));
u8 gKolimaPalette_Second[896] __attribute__((section(".bss")));
