#include "battle_effect_runtime.h"
#include "flags.h"
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
#include "item_ids.h"

// Kolima Forest's clearing, between the bridge and the river. Three logs
// lie in the grass for the party to push, and a Nut circles out of reach
// until Catch pulls it in.

// Actors 8 to 10 are the logs; 11 is the Nut.
enum { NUT = 11 };

// Shared source is included here in ROM order.
#include "common/object_staged_actor.h"

// Where the party enters from the bridge and the river.
const struct SceneEntrance *KolimaForestClearing_GetEntrances(void) {
  return gKolimaForestClearing_Entrances;
}

// The clearing has no regions.
const struct SceneRegion *KolimaForestClearing_GetRegions(void) { return 0; }

// The paths to the river and back to the bridge.
const u32 *KolimaForestClearing_GetExits(void) {
  return gKolimaForestClearing_Exits;
}

// The three logs and the Nut.
const struct ScenePlacement *KolimaForestClearing_GetPlacements(void) {
  return gKolimaForestClearing_Placements;
}

// Walking into a log: it rolls ahead until something stops it.
static void KolimaForestClearing_PushLog(void) {
  struct StagedActorProbe result;
  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&result))
    SceneActor_MoveAndRedraw(result);
  Engine_EventEnd();
}

// Catch on the circling Nut: it vanishes and the party finds a Nut.
static void KolimaForestClearing_CatchNut(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(NUT, 0, 0);
  Engine_GameFlagSet(FLAG_KOLIMA_FOREST_NUT_CAUGHT);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

// The logs, the Nut and the two paths.
const struct SceneEvent *KolimaForestClearing_GetEvents(void) {
  return gKolimaForestClearing_Events;
}

// On entry the Nut circles until it is caught, the logs mark the cells they
// cover, and the forest keeps its cursed colours until the curse is lifted.
s32 KolimaForestClearing_Initialize(void) {

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_FOREST_NUT_CAUGHT) == 0) {
    Scene_FloatNut(NUT);
  }
  FieldScene_RedrawActorFootprint(8);
  FieldScene_RedrawActorFootprint(9);
  FieldScene_RedrawActorFootprint(10);
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    KolimaPalette_Curse(11);
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

struct SceneEntrance gKolimaForestClearing_Entrances[] = {
    { 0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 480, 0, 376, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 312, 0, 40, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

// Exit 1 to the river (scene 42), exit 2 to the bridge (scene 40).
u32 gKolimaForestClearing_Exits[] = {
    SCENE_EXITS(41),
    SCENE_EXIT(1, 42, 1),
    SCENE_EXIT(2, 40, 2),
    SCENE_EXITS_END,
};

// Until the third patch the Nut's placement tested flag 0xfd0, not the flag
// catching it sets.
struct ScenePlacement gKolimaForestClearing_Placements[] = {
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
    { 0x16, FLAG_KOLIMA_FOREST_NUT_CAUGHT, 7,
        PIXELS(280), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#endif
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gKolimaForestClearing_Events[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestClearing_PushLog)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), NUT, FLAG_KOLIMA_FOREST_NUT_CAUGHT,
     EVENT_SCRIPT(KolimaForestClearing_CatchNut)},
    {SCENE_EVENTS_END},
};

// The .bss order follows the first declarations (externs in field.h).
u8 gKolimaPalette_First[1792] __attribute__((section(".bss")));
u8 gKolimaPalette_Second[896] __attribute__((section(".bss")));
