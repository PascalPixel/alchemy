/// @file
/// Goma Cave (SceneId_GomaCavePond), the room below the room of two pillars
/// and a stump, in the cave in the Goma Range that the mayor of Vault tells the
/// party to pass through on the way to Bilibin.
///
/// The room is reached only from the room above (goma_cave_pillar.c): its
/// three exits lead back up to that room's
/// GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_WEST, _SOUTH and _EAST. It has no
/// events of its own. Its one placed actor is the stump the party pushes over
/// the edge up there with Move: once it has fallen (FLAG_GOMA_CAVE_STUMP_FELL),
/// entering finds it sunk where it landed until only its top shows
/// (STUMP_ANIM_SINK), drawn underfoot with no collision flags, and the map
/// cells it covers change to match. The flag is a place flag, so
/// the stump is back up in the room above once the party leaves Goma Cave's
/// four scenes.
///
/// Open questions:
/// - XXX: whether the stump comes down in the pond the scene is named for, as
///   STUMP_ANIM_SINK and the changed cells suggest; a render of the room with
///   FLAG_GOMA_CAVE_STUMP_FELL set would settle it.
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

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.

/// The stump has been pushed over into the room below (goma_cave_pillar.c).
/// ★ Named from what the player sees: a tree stump standing on a ledge in the
/// room above (its render)
#define FLAG_GOMA_CAVE_STUMP_FELL 0x300

/// The room's one placed actor.
enum GomaCavePondActor {
  /// The stump the party pushes over in the room above, lying in this room.
  /// ★ Named from the game: "It's no use. Our Psynergy isn't strong enough to
  /// free that stump." (MsgGomaNoUsePsynergy)
  ACTOR_FALLEN_STUMP = ACTOR_FIRST_PLACED
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gGomaCavePondEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gGomaCavePondExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gGomaCavePondPlacements;
}

const struct SceneEvent *Scene_GetEvents(void) { return gGomaCavePondEvents; }

/// Sets up the room as the party enters. Once the stump in the room above has
/// been pushed over, it lies here underfoot, and the cells it covers change.
s32 Scene_Initialize(void) {
  struct EventWork *work;

  work = gEventWork;
  work->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  work->transitionFrames = 24;
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_STUMP_FELL) != 0) {
    OBJ_POS_SET(ACTOR_FALLEN_STUMP, 216, 136);
    OBJ_MODE(ACTOR_FALLEN_STUMP, STUMP_ANIM_SINK);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FALLEN_STUMP), 0);
    Object_GetById(ACTOR_FALLEN_STUMP)->priorityFlags =
        ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(ACTOR_FALLEN_STUMP)->collisionFlags = 0;
    Map_CopyCellAttributes(11, 36, 5, 5, 11, 6);
  }
  return 0;
}

struct SceneEntrance gGomaCavePondEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_WEST, CONDITION_ALWAYS, 120, 0, 88,
     FACING_SOUTH, 0, 24, 8, 504, 424, 0},
    {GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_SOUTH, CONDITION_ALWAYS, 424, 0, 344,
     FACING_SOUTH, 0, 24, 8, 504, 424, 0},
    {GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_EAST, CONDITION_ALWAYS, 408, 0, 136,
     FACING_SOUTH, 0, 24, 8, 504, 424, 0},
    {SCENE_TABLE_END},
};

u32 gGomaCavePondExits[] = {
    SCENE_EXITS(GomaCavePond),
    SCENE_EXIT(GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_WEST, GomaCavePillar,
               GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_WEST),
    SCENE_EXIT(GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_SOUTH, GomaCavePillar,
               GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_SOUTH),
    SCENE_EXIT(GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_EAST, GomaCavePillar,
               GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gGomaCavePondPlacements[] = {
    { SPRITE_STUMP, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gGomaCavePondEvents[] = {
    {EVENT_EXIT | EXIT_STYLE_ANIMATED, 5, CONDITION_ALWAYS,
     GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_WEST},
    {EVENT_EXIT | EXIT_STYLE_ANIMATED, 6, CONDITION_ALWAYS,
     GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_SOUTH},
    {EVENT_EXIT | EXIT_STYLE_ANIMATED, 7, CONDITION_ALWAYS,
     GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_EAST},
    {SCENE_EVENTS_END},
};
