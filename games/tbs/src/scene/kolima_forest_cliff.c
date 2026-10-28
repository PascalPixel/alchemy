/// @file
/// Kolima Forest (SceneId_KolimaForestCliff), the third of the forest's five
/// scenes, between SceneId_KolimaForestGlade and SceneId_KolimaForestFloodgate.
/// Of its three logs, rolled by walking into them, the one pushed west to the
/// water's edge tips into the river and floats there, cells the party can cross
/// to the far side, and from the bank the leader can hop east (step trigger 11,
/// KolimaForest_HopEast). The forest keeps its cursed colours until Tret lifts
/// the curse (FLAG_KOLIMA_CURSE_LIFTED).
///
/// FLAG_KOLIMA_FOREST_CLIFF_LOG_IN_RIVER: the log is in the river, until the
/// party next comes in.
///
/// The scene's name, Cliff, is ours, for what sets the scene apart: a rock
/// cliff runs across its north side above the river and the logs
/// (places/kolima-forest-cliff_cursed/map-0.png).

#include "speaker.h"
#include "flags.h"
#include "heap_slots.h"
#include "module/import.h"
#include "module/scene/kolima_forest_cliff.h"
#include "module/scene/shared/kolima_palette.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "map_grid_copy.h"
#include "types.h"
#include "tbs_edition.h"

#include "pushed_actor.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "event.h"
#include "scene_tables.h"
#include "sounds.h"

/// The scene's actors, numbering the rows of
/// gKolimaForestCliffActorPlacements.
enum {
  /// The three logs; the third is the one that can reach the river.
  /// ★ Named from what the player sees: a log lying west to east (its render)
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: a log lying north to south (its
  /// render)
  ACTOR_RIVER_LOG = ACTOR_FIRST_LOG + 2
};

#include "shared/staged_actor.h"

/// The river log has tipped into the river and floats there as ground the party
/// can cross.
/// ★ Named from what the player sees: a log lying north to south beside the
/// river that winds through the clearing (its render)
#define FLAG_KOLIMA_FOREST_CLIFF_LOG_IN_RIVER FLAG_SCENE_TEMP_01

const struct SceneEntrance *KolimaForestCliff_GetEntrances(void) {
  return gKolimaForestCliffEntrances;
}

const struct SceneExitRegion *KolimaForestCliff_GetRegions(void) { return 0; }

const u32 *KolimaForestCliff_GetExits(void) { return gKolimaForestCliffExits; }

/// The three logs.
const struct ActorPlacement *KolimaForestCliff_GetActorPlacements(void) {
  return gKolimaForestCliffActorPlacements;
}

/// Kolima Forest (SceneId_KolimaForestCliff), walking into a log: it rolls
/// ahead until something stops it. The river log, stopped at the bank (cell
/// column 12), tips over with a splash and floats in the water; its cells
/// become ground the party can cross (sets
/// FLAG_KOLIMA_FOREST_CLIFF_LOG_IN_RIVER).
static void KolimaForestCliff_PushLog(void) {
  struct PushedActorProbe result;
  Event_Start();
  if (PushedActor_FindClearPosition(&result)) {
    Actor_MoveAndRedraw(result);
    if (result.actorSlot == ACTOR_RIVER_LOG &&
        (result.position.x >> 20) == 12) {
      struct Actor *actor;
      Import_ActorSetAnimation(ACTOR_RIVER_LOG, LOG_ANIMATION_ROLL_WEST);
      Actor_OffsetPositionAndResetMotion(ACTOR_RIVER_LOG, -18, 6);
      Event_Wait(30);
      Sound_Play(SE_LOG_FALL);
      Import_ActorSetAnimation(ACTOR_RIVER_LOG, LOG_ANIMATION_DROP_IN);
      Actor_GetById(ACTOR_RIVER_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(32, 20, 2, 4, 11, 16);
      PushedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4,
                                             CELL_BEHAVIOR_NORMAL);
      GameFlag_Set(FLAG_KOLIMA_FOREST_CLIFF_LOG_IN_RIVER);
      actor = Actor_GetById(ACTOR_RIVER_LOG);
      Actor_SetSpriteFlags(actor, 0);
    }
  }
  Event_End();
}

#include "shared/kolima_jump.h"

/// The logs, the two ways out and the hop east from the bank.
const struct Trigger *KolimaForestCliff_GetTriggers(void) {
  return gKolimaForestCliffTriggers;
}

/// Sets up the logs and the forest's colours on entry. A log already in the
/// river floats there again, the logs mark the cells they cover, and the forest
/// keeps its cursed colours until the curse is lifted.
s32 KolimaForestCliff_Initialize(void) {
  struct FieldWork *work = gFieldWork;

  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Scene_RedrawActorFootprint(ACTOR_RIVER_LOG);

  if (GameFlag_Get(FLAG_KOLIMA_FOREST_CLIFF_LOG_IN_RIVER) != 0) {
    Actor_GetById(ACTOR_RIVER_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(32, 20, 2, 4, 11, 16);
    PushedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4,
                                           CELL_BEHAVIOR_NORMAL);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_RIVER_LOG), 0);
  }

  Scene_RedrawActorFootprint(ACTOR_FIRST_LOG);
  Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 1);

  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    KolimaPalette_Curse(6);
  }
  return 0;
}

#include "shared/kolima_palette.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gPushedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

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
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(256), 0, PIXELS(344), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(224), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(312), 0, PIXELS(288), FACING_SOUTH, TALK_FACING_KEEP, 0 },
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
    {TRIGGERS_END},
};

// The .bss order follows the first declarations (externs in field.h).
u8 gKolimaPalette_Healthy[1792] __attribute__((section(".bss")));
u8 gKolimaPalette_Cursed[896] __attribute__((section(".bss")));
