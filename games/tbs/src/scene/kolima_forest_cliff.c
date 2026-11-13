/// @file
/// Kolima Forest (SceneId_KolimaForestCliff), the third of the forest's five
/// scenes, between SceneId_KolimaForestGlade and SceneId_KolimaForestFloodgate.
///
/// It has three logs, rolled by walking into them. The one pushed west to the
/// water's edge tips into the river and floats there, cells the party can cross
/// to the far side. From the bank the leader can hop east (step trigger 11,
/// KolimaForest_HopEast).
///
/// The forest keeps its cursed colours until Tret lifts the curse
/// (FLAG_KOLIMA_CURSE_LIFTED).
///
/// FLAG_KOLIMA_FOREST_CLIFF_LOG_IN_RIVER: the log is in the river, until the
/// party next comes in.
///
/// The scene's name, Cliff, is ours, for what sets the scene apart. A rock
/// cliff runs across its north side above the river and the logs
/// (places/kolima-forest-cliff_cursed/map-0.png).

#include "speaker.h"
#include "flags.h"
#include "heap_slots.h"
#include "module/import.h"
#include "module/scene/kolima_forest_cliff.h"
#include "module/scene/shared/kolima_palette.h"
#include "module/field/event_commands.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "macros.h"

#include "pushed_actor.h"
#include "main_routine_work.h"
#include "event.h"

/// The scene's actors, numbering the rows of
/// gKolimaForestCliffActorPlacements.
enum {
  /// The three logs. The third is the one that can reach the river.
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_RIVER_LOG = ACTOR_FIRST_LOG + 2
};

#include "shared/staged_actor.c"

/// The river log has tipped into the river and floats there as ground the party
/// can cross.
#define FLAG_KOLIMA_FOREST_CLIFF_LOG_IN_RIVER FLAG_SCENE_TEMP_01

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gKolimaForestCliffEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gKolimaForestCliffExits; }

/// The three logs.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gKolimaForestCliffActorPlacements;
}

/// In Kolima Forest (SceneId_KolimaForestCliff), walking into a log rolls it
/// ahead until something stops it.
static void KolimaForestCliff_PushLog(void) {
  struct PushedActorProbe result;

  Event_Start();
  if (PushedActor_FindClearPosition(&result)) {
    Actor_MoveAndRedraw(result);
    if (result.actorSlot == ACTOR_RIVER_LOG &&
        (FIELD_CELL(result.position.x)) == 12) {
      Actor_SetAnimation(ACTOR_RIVER_LOG, LOG_ANIMATION_ROLL_WEST);
      Actor_SetDestinationOffset(ACTOR_RIVER_LOG, -18, 6);
      Event_Wait(30);
      Sound_Play(SE_LOG_FALL);
      Actor_SetAnimation(ACTOR_RIVER_LOG, LOG_ANIMATION_DROP_IN);
      Actor_Get(ACTOR_RIVER_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(32, 20, 2, 4, 11, 16);
      PushedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4,
                                             CELL_BEHAVIOR_NORMAL);
      GameFlag_Set(FLAG_KOLIMA_FOREST_CLIFF_LOG_IN_RIVER);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_RIVER_LOG), 0);
    }
  }
  Event_End();
}

#include "shared/kolima_jump.c"

/// The logs, the two ways out and the hop east from the bank.
const struct Trigger *Scene_GetTriggers(void) {
  return gKolimaForestCliffTriggers;
}

/// Sets up the logs and the forest's colours on entry.
s32 Scene_Initialize(void) {
  struct FieldWork *work = gFieldWork;

  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Scene_RedrawActorFootprint(ACTOR_RIVER_LOG);

  if (GameFlag_Get(FLAG_KOLIMA_FOREST_CLIFF_LOG_IN_RIVER) != 0) {
    Actor_Get(ACTOR_RIVER_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(32, 20, 2, 4, 11, 16);
    PushedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4,
                                           CELL_BEHAVIOR_NORMAL);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_RIVER_LOG), 0);
  }

  Scene_RedrawActorFootprint(ACTOR_FIRST_LOG);
  Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 1);

  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    KolimaPalette_Curse(6);
  }
  return 0;
}

#include "shared/kolima_palette.c"

#include "shared/pushed_actor_tables.c"
// clang-format off

struct SceneEntrance gKolimaForestCliffEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_GLADE, CONDITION_ALWAYS, 320, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_FLOODGATE, CONDITION_ALWAYS, 480, 0, 104, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

/// On into the forest (SceneId_KolimaForestFloodgate) and back
/// (SceneId_KolimaForestGlade).
u32 gKolimaForestCliffExits[] = {
    SCENE_EXITS(KolimaForestCliff),
    SCENE_EXIT(KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_FLOODGATE, KolimaForestFloodgate, KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_CLIFF),
    SCENE_EXIT(KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_GLADE, KolimaForestGlade, KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_CLIFF),
    SCENE_EXITS_END,
};

struct ActorPlacement gKolimaForestCliffActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        256, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        224, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        312, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gKolimaForestCliffTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_FLOODGATE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_GLADE},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 10, CONDITION_ALWAYS,
     EVENT(KolimaForestCliff_PushLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS,
     EVENT(KolimaForest_HopEast)},
    {TRIGGER_TABLE_END},
};

// The .bss order follows the first declarations (externs in field.h).
u8 gKolimaPalette_Healthy[1792] __attribute__((section(".bss"))) ALIGNED(16);
u8 gKolimaPalette_Cursed[896] __attribute__((section(".bss")));
