/// @file
/// Goma Cave (SceneId_GomaCavePond), the room below the room of two pillars
/// and a stump, in the cave in the Goma Range that the mayor of Vault tells the
/// party to pass through on the way to Bilibin.
///
/// The room is reached only from the room above (goma_cave_pillar.c): its
/// three warps lead back up to that room's
/// GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_WEST, _SOUTH and _EAST. It has no
/// map events of its own. Its one placed object event is the stump the party
/// pushes over the edge up there with Move: once it has fallen
/// (FLAG_GOMA_CAVE_STUMP_FELL), entering finds it sunk in the pond until only
/// its top shows (STUMP_ANIMATION_SINK), drawn underfoot with no collision
/// flags. The five by five cells around it, open water in the room's render
/// (places/goma-cave-pond/map-1.png), are replaced by a copy that adds
/// MB_JUMP_FORWARD cells, so the leader can jump across the water by
/// way of it.
/// The flag is a map-section temp flag, so the stump is back up in the room
/// above once the party leaves Goma Cave's four maps.
#include "unit/scene/goma_cave_pond.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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

#include "heap_slots.h"
#include "map_tables.h"
#include "object_event.h"

#include "script_layer.h"
#include "flags.h"

// Map-section temp flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when
// the party enters another map section, and other map sections use the numbers
// for their own.

/// The stump has been pushed over into the room below (goma_cave_pillar.c).
/// ★ Named from what the player sees: a tree stump standing on a ledge in the
/// room above (its render)
#define FLAG_GOMA_CAVE_STUMP_FELL 0x300

/// The room's one placed object event.
enum GomaCavePondLocalId {
  /// The stump the party pushes over in the room above, lying in this room.
  /// ★ Named from the game: "It's no use. Our Psynergy isn't strong enough to
  /// free that stump." (MsgGomaNoUsePsynergy)
  LOCALID_FALLEN_STUMP = LOCALID_FIRST_PLACED
};

const struct MapWarpDest *Scene_GetWarpDests(void) {
  return gGomaCavePondWarpDests;
}

const struct MapWarpRegion *Scene_GetWarpRegions(void) { return NULL; }

const u32 *Scene_GetWarps(void) { return gGomaCavePondWarps; }

const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void) {
  return gGomaCavePondObjectEventTemplates;
}

const struct MapEvent *Scene_GetEvents(void) { return gGomaCavePondEvents; }

/// Sets up the room as the party enters. Once the stump in the room above has
/// been pushed over, it lies here underfoot, and the cells it covers change.
s32 Scene_Initialize(void) {
  struct FieldWork *work;

  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  work->transitionFrames = 24;
  if (GameFlag_Get(FLAG_GOMA_CAVE_STUMP_FELL) != 0) {
    ObjectEvent_SetPosition(LOCALID_FALLEN_STUMP, PIXELS(216), PIXELS(136));
    Import_ObjectEventSetAnimation(LOCALID_FALLEN_STUMP, STUMP_ANIMATION_SINK);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_FALLEN_STUMP), 0);
    ObjectEvent_GetById(LOCALID_FALLEN_STUMP)->priorityFlags =
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    ObjectEvent_GetById(LOCALID_FALLEN_STUMP)->collisionFlags = 0;
    MapGrid_CopyCellAttributes(11, 36, 5, 5, 11, 6);
  }
  return 0;
}

struct MapWarpDest gGomaCavePondWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_WEST, CONDITION_ALWAYS, 120, 0, 88,
     FACING_SOUTH, 0, 24, 8, 504, 424, 0},
    {GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_SOUTH, CONDITION_ALWAYS, 424, 0, 344,
     FACING_SOUTH, 0, 24, 8, 504, 424, 0},
    {GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_EAST, CONDITION_ALWAYS, 408, 0, 136,
     FACING_SOUTH, 0, 24, 8, 504, 424, 0},
    {MAP_TABLE_END},
};

u32 gGomaCavePondWarps[] = {
    MAP_WARPS(GomaCavePond),
    MAP_WARP(GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_WEST, GomaCavePillar,
             GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_WEST),
    MAP_WARP(GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_SOUTH, GomaCavePillar,
             GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_SOUTH),
    MAP_WARP(GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_EAST, GomaCavePillar,
             GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gGomaCavePondObjectEventTemplates[] = {
    { SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};
// clang-format on

struct MapEvent gGomaCavePondEvents[] = {
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS,
     GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_WEST},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS,
     GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_SOUTH},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS,
     GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_EAST},
    {MAP_EVENTS_END},
};
