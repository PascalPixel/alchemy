/// @file
/// Goma Cave: the room the stump in the room above falls into.
#include "unit/scene/goma_cave_pond.h"
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
#include "scene.h"

#include "resource_388_runtime.h"
#include "field_scene.h"
#include "field_obj.h"

#include "field_event.h"
#include "flags.h"
#include "event.h"

/// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
/// enters another place group, and other places use the numbers for their own.
/// The stump has been pushed over into the room below (goma_cave_pillar.c).
#define FLAG_GOMA_CAVE_STUMP_FELL 0x300

/// The stump the party pushes over in the room above, lying in this room.
#define FALLEN_STUMP 8

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gGomaCavePondEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gGomaCavePondExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gGomaCavePondPlacements;
}

const struct SceneEvent *Scene_GetEvents(void) { return gGomaCavePondEvents; }

/// Once the stump in the room above has been pushed over, it lies fallen
/// here: put it in place and copy the map cells it covers.
s32 Scene_Initialize(void) {
  struct EventWork *work;

  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  work->transitionFrames = 24;
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_STUMP_FELL) != 0) {
    OBJ_POS_SET(FALLEN_STUMP, 216, 136);
    OBJ_MODE(FALLEN_STUMP, 2);
    Engine_ActorSetSpriteFlags(Object_GetById(FALLEN_STUMP), 0);
    Object_GetById(FALLEN_STUMP)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(FALLEN_STUMP)->collisionFlags = 0;
    Map_CopyCellAttributes(11, 36, 5, 5, 11, 6);
  }
  return 0;
}

struct SceneEntrance gGomaCavePondEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 120, 0, 88, FACING_SOUTH, 0, 24, 8, 504, 424, 0},
    {2, CONDITION_ALWAYS, 424, 0, 344, FACING_SOUTH, 0, 24, 8, 504, 424, 0},
    {3, CONDITION_ALWAYS, 408, 0, 136, FACING_SOUTH, 0, 24, 8, 504, 424, 0},
    {SCENE_TABLE_END},
};

u32 gGomaCavePondExits[] = {
    SCENE_EXITS(26),      SCENE_EXIT(1, 27, 4), SCENE_EXIT(2, 27, 5),
    SCENE_EXIT(3, 27, 3), SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gGomaCavePondPlacements[] = {
    { 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gGomaCavePondEvents[] = {
    {0x21, 5, CONDITION_ALWAYS, 1},
    {0x21, 6, CONDITION_ALWAYS, 2},
    {0x21, 7, CONDITION_ALWAYS, 3},
    {SCENE_EVENTS_END},
};
