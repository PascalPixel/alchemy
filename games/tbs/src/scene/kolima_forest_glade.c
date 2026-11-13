/// @file
/// Kolima Forest (SceneId_KolimaForestGlade), the second of the forest's five
/// scenes, on the party's way to Tret between SceneId_KolimaForestEntrance and
/// SceneId_KolimaForestCliff.
///
/// Three logs lie in the grass to be rolled by walking into them. A Nut floats
/// out of reach until Catch pulls it in (FLAG_KOLIMA_FOREST_NUT_CAUGHT).
///
/// Until Tret lifts the curse (FLAG_KOLIMA_CURSE_LIFTED), KolimaPalette_Curse
/// gives the forest redder, darker colours.
///
/// Until the third patch the Nut's actor placement row tested Vale's
/// FLAG_VALE_NUT_CAUGHT rather than FLAG_KOLIMA_FOREST_NUT_CAUGHT, so once
/// Vale's Nut had been caught this one was never placed.
///
/// The scene's name, Glade, is ours, for what sets the scene apart. It is open
/// grass and stumps ringed by trees, with no cliff or river
/// (places/kolima-forest-glade_cursed/map-0.png).

#include "field/speaker.h"
#include "flags.h"
#include "system/heap_slots.h"
#include "scene/import.h"
#include "types.h"
#include "field/scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gKolimaForestGlade_Entrances[];
extern u32 gKolimaForestGlade_Exits[];
extern struct ActorPlacement gKolimaForestGlade_ActorPlacements[];
extern struct Trigger gKolimaForestGlade_Triggers[];

#include "scene/actor_orbit.h"
#include "scene/kolima_palette.h"
#include "field/event_commands.h"
#include "battle/battle.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/event.h"
#include "field/pushed_actor.h"
#include "system/io_reg.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "macros.h"
#include "scene/catch_nut.h"

/// Kolima Forest's actors, numbering the rows of
/// gKolimaForestGlade_ActorPlacements.
enum KolimaForestGladeActorId {
  /// The three logs.
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  /// The Nut that floats until Catch pulls it in.
  ACTOR_NUT = ACTOR_FIRST_LOG + 3
};

#include "shared/staged_actor.c"

/// Where the party enters, from SceneId_KolimaForestEntrance and
/// SceneId_KolimaForestCliff.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gKolimaForestGlade_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

/// The ways on into the forest (SceneId_KolimaForestCliff) and back
/// (SceneId_KolimaForestEntrance).
const u32 *Scene_GetExits(void) { return gKolimaForestGlade_Exits; }

/// The three logs and the Nut.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gKolimaForestGlade_ActorPlacements;
}

/// In Kolima Forest (SceneId_KolimaForestGlade), walking into a log rolls it
/// ahead until something stops it.
static void KolimaForestGlade_PushLog(void) {
  struct PushedActorProbe result;
  Event_Start();
  if (PushedActor_FindClearPosition(&result))
    Actor_MoveAndRedraw(result);
  Event_End();
}

/// In Kolima Forest (SceneId_KolimaForestGlade), Catch on the floating Nut
/// makes it vanish, and the party finds a Nut.
static void KolimaForestGlade_CatchNut(void) {
  Scene_CatchNut(ACTOR_NUT, FLAG_KOLIMA_FOREST_NUT_CAUGHT);
}

/// The logs, the Nut and the two ways out.
const struct Trigger *Scene_GetTriggers(void) {
  return gKolimaForestGlade_Triggers;
}

/// Sets up the floating Nut and the logs on entry.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (GameFlag_Get(FLAG_KOLIMA_FOREST_NUT_CAUGHT) == 0) {
    Scene_FloatNut(ACTOR_NUT);
  }
  Scene_RedrawActorFootprint(ACTOR_FIRST_LOG);
  Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 1);
  Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 2);
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    KolimaPalette_Curse(11);
  }
  return 0;
}

#include "shared/kolima_palette.c"
#include "shared/actor_orbit.c"

#include "shared/pushed_actor_tables.c"
// clang-format off

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

/// Until the third patch the Nut's actor placement tested Vale's
/// FLAG_VALE_NUT_CAUGHT, not the flag catching it sets.
struct ActorPlacement gKolimaForestGlade_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        360, 0, 368, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        160, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        136, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
#if !TBS_PATCH_3
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        280, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
#else
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_KOLIMA_FOREST_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        280, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
#endif
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gKolimaForestGlade_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_CLIFF},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_ENTRANCE},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 10, CONDITION_ALWAYS,
     EVENT(KolimaForestGlade_PushLog)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_KOLIMA_FOREST_NUT_CAUGHT,
     EVENT(KolimaForestGlade_CatchNut)},
    {TRIGGER_TABLE_END},
};

// The .bss order follows the first declarations (externs in field.h).
u8 gKolimaPalette_Healthy[1792] __attribute__((section(".bss"))) ALIGNED(16);
u8 gKolimaPalette_Cursed[896] __attribute__((section(".bss")));
