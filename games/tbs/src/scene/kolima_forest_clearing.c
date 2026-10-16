#include "battle_effect_runtime.h"
#include "version.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/kolima_forest_clearing.h"
#include "unit/scene/common/object_orbit.h"
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
#include "types.h"
#include "field_event.h"
#include "staged_actor.h"
#include "io_reg.h"
#include "ability_ids.h"
#include "field_scene.h"

#ifdef MODERN
// called before its definition below, which modern GCC refuses for an
// implicit int declaration of a u16 function
#endif

enum { ITEM_NUT = 181 };

typedef struct {
  s32 lo, hi;
} Pair;

// Shared source is included here in ROM order.
#include "common/object_staged_actor.h"

u8 *SceneData_GetScriptTable(void) { return gKolimaForestClearing_Scripts; }

s32 SceneData_ReturnZero(void) { return 0; }

u8 *SceneData_GetMessageTable(void) { return gKolimaForestClearing_Messages; }

u8 *SceneData_GetActorTable(void) { return gKolimaForestClearing_Actors; }

static void EV10_SCRIPT(void) {
  struct StagedActorProbe result;
  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&result))
    SceneActor_MoveAndRedraw(result);
  Engine_EventEnd();
}

static void EV11_PSYNERGY(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(11, 0, 0);
  Engine_GameFlagSet(0xfd3);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

u8 *SceneData_GetExtraTable(void) { return gKolimaForestClearing_Extras; }

s32 FieldScene_SetupEntryActors8To11(void) {

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (Engine_GameFlagIsSet(0xfd3) == 0) {
    InitializeStagedActorSceneOrbitingEffect(11);
  }
  FieldScene_RedrawActorFootprint(8);
  FieldScene_RedrawActorFootprint(9);
  FieldScene_RedrawActorFootprint(10);
  if (Engine_GameFlagIsSet(0x845) == 0) {
    SceneEffect_AdjustPaletteColors(11);
  }
  return 0;
}

// Shared source is included here in ROM order.
#include "common/kolima_palette.h"
#include "common/object_orbit.h"

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

struct SceneEntrance gKolimaForestClearing_Scripts[] = {
    { 0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 480, 0, 376, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 312, 0, 40, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gKolimaForestClearing_Messages[] = {
    SCENE_EXITS(41),
    SCENE_EXIT(1, 42, 1),
    SCENE_EXIT(2, 40, 2),
    SCENE_EXITS_END,
};

struct ScenePlacement gKolimaForestClearing_Actors[] = {
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(360), 0, PIXELS(368), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(160), 0, PIXELS(392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(336), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#if !TBS_PATCH_3
    { 0x16, 0xfd0, 7,
        PIXELS(280), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#else
    { 0x16, 0xfd3, 7,
        PIXELS(280), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#endif
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gKolimaForestClearing_Extras[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SCRIPT)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), 11, 0xfd3, EVENT_SCRIPT(EV11_PSYNERGY)},
    {SCENE_EVENTS_END},
};

// The .bss order follows the first declarations (externs in field.h).
u8 gKolimaPalette_First[1792] __attribute__((section(".bss")));
u8 gKolimaPalette_Second[896] __attribute__((section(".bss")));
