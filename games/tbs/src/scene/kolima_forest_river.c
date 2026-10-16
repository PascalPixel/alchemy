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

// Shared source is included here in ROM order.
#include "common/object_staged_actor.h"

u8 *MapStagedScene_SelectPrimaryData(void) {
  return gKolimaForestRiverEntrances;
}

s32 MapStagedScene_GetEmptyData(void) { return 0; }

u8 *MapStagedScene_SelectSecondaryData(void) { return gKolimaForestRiverExits; }

u8 *MapStagedScene_SelectTertiaryData(void) {
  return gKolimaForestRiverPlacements;
}

static void EV10_SCRIPT(void) {
  struct StagedActorProbe result;
  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&result)) {
    SceneActor_MoveAndRedraw(result);
    if (result.actorSlot == 10 && (result.positionX >> 20) == 12) {
      struct FieldActor *actor;
      Engine_ActorSetAnimation(10, 3);
      Engine_ActorSetDestinationOffset(10, -18, 6);
      Engine_EventWait(30);
      Engine_AudioPlayCue(240);
      Engine_ActorSetAnimation(10, 8);
      Object_GetById(10)->priorityFlags = 2;
      Map_CopyCellAttributes(32, 20, 2, 4, 11, 16);
      StagedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4, 0);
      Engine_GameFlagSet(0x201);
      actor = Object_GetById(10);
      Engine_ActorSetSpriteFlags(actor, 0);
    }
  }
  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/kolima_jump.h"

u8 *KolimaForestRiver_GetEvents(void) { return gKolimaForestRiverEvents; }

s32 KolimaForestRiver_PrepareWindowTransition(void) {
  struct EventWork *work = gEventWork;

  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  FieldScene_RedrawActorFootprint(10);

  if (Engine_GameFlagIsSet(0x201) != 0) {
    Object_GetById(10)->priorityFlags = 2;
    Map_CopyCellAttributes(32, 20, 2, 4, 11, 16);
    StagedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
  }

  FieldScene_RedrawActorFootprint(8);
  FieldScene_RedrawActorFootprint(9);

  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    SceneEffect_AdjustPaletteColors(6);
  }
  return 0;
}

// Shared source is included here in ROM order.
#include "common/kolima_palette.h"

// Pixel step by direction: X in the upper halfword, Z in the lower.
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
    {0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SCRIPT)},
    {TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(EV11_TOUCH)},
    {SCENE_EVENTS_END},
};

// The .bss order follows the first declarations (externs in field.h).
u8 gKolimaPalette_First[1792] __attribute__((section(".bss")));
u8 gKolimaPalette_Second[896] __attribute__((section(".bss")));
