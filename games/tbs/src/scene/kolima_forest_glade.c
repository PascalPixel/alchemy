/// @file
/// Kolima Forest (SceneId_KolimaForestGlade), the second of the forest's
/// five maps, on the party's way to Tret between SceneId_KolimaForestEntrance
/// and SceneId_KolimaForestCliff. Three logs lie in the grass to be rolled by
/// walking into them, and a Nut floats out of reach until Catch pulls it in
/// (FLAG_KOLIMA_FOREST_NUT_CAUGHT). Until Tret lifts the curse
/// (FLAG_KOLIMA_CURSE_LIFTED), KolimaPalette_Curse gives the forest redder,
/// darker colours.
///
/// Until the third patch the Nut's placement row tested Vale's
/// FLAG_VALE_NUT_CAUGHT rather than FLAG_KOLIMA_FOREST_NUT_CAUGHT, so once
/// Vale's Nut had been caught this one was never placed.
///
/// Open questions:
/// - XXX: what the map's name, Glade, rests on; a render of
///   SceneId_KolimaForestGlade would show whether a clearing sets it apart.

#include "battle_effect_runtime.h"
#include "flags.h"
#include "version.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/kolima_forest_glade.h"
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

/// Kolima Forest's actors, numbering the rows of gKolimaForestGlade_Placements.
enum KolimaForestGladeActor {
  /// The three logs.
  /// ★ Named from what the player sees: logs lying on the forest grass among
  /// the stumps (its render)
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  /// The Nut that floats until Catch pulls it in.
  /// ★ Named from the game: "Nut" (MsgItemName)
  ACTOR_NUT = ACTOR_FIRST_LOG + 3
};

#include "common/object_staged_actor.h"

/// Where the party enters, from SceneId_KolimaForestEntrance and
/// SceneId_KolimaForestCliff.
const struct SceneEntrance *KolimaForestGlade_GetEntrances(void) {
  return gKolimaForestGlade_Entrances;
}

const struct SceneRegion *KolimaForestGlade_GetRegions(void) { return 0; }

/// The ways on into the forest (SceneId_KolimaForestCliff) and back
/// (SceneId_KolimaForestEntrance).
const u32 *KolimaForestGlade_GetExits(void) { return gKolimaForestGlade_Exits; }

/// The three logs and the Nut.
const struct ScenePlacement *KolimaForestGlade_GetPlacements(void) {
  return gKolimaForestGlade_Placements;
}

/// Kolima Forest (SceneId_KolimaForestGlade), walking into a log: it rolls
/// ahead until something stops it.
static void KolimaForestGlade_PushLog(void) {
  struct StagedActorProbe result;
  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&result))
    SceneActor_MoveAndRedraw(result);
  Engine_EventEnd();
}

/// Kolima Forest (SceneId_KolimaForestGlade), Catch on the floating Nut (until
/// FLAG_KOLIMA_FOREST_NUT_CAUGHT): it vanishes and the party finds a Nut.
static void KolimaForestGlade_CatchNut(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(ACTOR_NUT, 0, 0);
  Engine_GameFlagSet(FLAG_KOLIMA_FOREST_NUT_CAUGHT);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

/// The logs, the Nut and the two ways out.
const struct SceneEvent *KolimaForestGlade_GetEvents(void) {
  return gKolimaForestGlade_Events;
}

/// Sets up the floating Nut and the logs on entry. The Nut floats until it is
/// caught, the logs mark the cells they cover, and the forest keeps its cursed
/// colours until the curse is lifted.
s32 KolimaForestGlade_Initialize(void) {

  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_FOREST_NUT_CAUGHT) == 0) {
    Scene_FloatNut(ACTOR_NUT);
  }
  FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG);
  FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG + 1);
  FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG + 2);
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    KolimaPalette_Curse(11);
  }
  return 0;
}

#include "common/kolima_palette.h"
#include "common/object_orbit.h"

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

struct SceneEntrance gKolimaForestGlade_Entrances[] = {
    { 0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_ENTRANCE, CONDITION_ALWAYS, 480, 0, 376, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_CLIFF, CONDITION_ALWAYS, 312, 0, 40, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

/// KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_CLIFF on into the forest, and
/// KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_ENTRANCE back.
u32 gKolimaForestGlade_Exits[] = {
    SCENE_EXITS(KolimaForestGlade),
    SCENE_EXIT(KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_CLIFF, KolimaForestCliff, KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_GLADE),
    SCENE_EXIT(KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_ENTRANCE, KolimaForestEntrance, KOLIMA_FOREST_ENTRANCE_FROM_KOLIMA_FOREST_GLADE),
    SCENE_EXITS_END,
};

/// Until the third patch the Nut's placement tested Vale's FLAG_VALE_NUT_CAUGHT, not the flag
/// catching it sets.
struct ScenePlacement gKolimaForestGlade_Placements[] = {
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(360), 0, PIXELS(368), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(160), 0, PIXELS(392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(136), 0, PIXELS(336), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#if !TBS_PATCH_3
    { SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, ACTOR_RESET_ACTION,
        PIXELS(280), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#else
    { SPRITE_BLANK, FLAG_KOLIMA_FOREST_NUT_CAUGHT, ACTOR_RESET_ACTION,
        PIXELS(280), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#endif
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gKolimaForestGlade_Events[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS,
     KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_CLIFF},
    {EVENT_EXIT, 2, CONDITION_ALWAYS,
     KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_ENTRANCE},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestGlade_PushLog)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_KOLIMA_FOREST_NUT_CAUGHT,
     EVENT_SCRIPT(KolimaForestGlade_CatchNut)},
    {SCENE_EVENTS_END},
};

// The .bss order follows the first declarations (externs in field.h).
u8 gKolimaPalette_First[1792] __attribute__((section(".bss")));
u8 gKolimaPalette_Second[896] __attribute__((section(".bss")));
