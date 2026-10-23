/// @file
/// Kolima Forest (MapId_KolimaForestCliff), the third of the forest's five
/// maps, between MapId_KolimaForestGlade and MapId_KolimaForestFloodgate.
/// Of its three logs, rolled by walking into them, the one pushed west to the
/// water's edge tips into the river and floats there, cells the party can cross
/// to the far side, and from the bank the leader can hop east (coord event 11,
/// KolimaForest_HopEast). The forest keeps its cursed colours until Tret lifts
/// the curse (FLAG_KOLIMA_CURSE_LIFTED).
///
/// FLAG_MAP_TEMP_01: the log is in the river, until the party next comes in.
///
/// The map's name, Cliff, is ours, for what sets the map apart: a rock cliff
/// runs across its north side above the river and the logs
/// (places/kolima-forest-cliff_cursed/map-0.png).
///
/// Open questions: none.

#include "speaker.h"
#include "flags.h"
#include "heap_slots.h"
#include "unit/import.h"
#include "unit/map/kolima_forest_cliff.h"
#include "unit/map/common/kolima_palette.h"
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
#include "map_grid_copy.h"
#include "types.h"
#include "tbs_edition.h"

#include "pushed_object.h"
#include "object_event.h"
#include "game_state.h"
#include "script_layer.h"
#include "map_tables.h"
#include "songs.h"

/// The map's object events, numbering the rows of
/// gKolimaForestCliffObjectEventTemplates.
enum {
  /// The three logs; the third is the one that can reach the river.
  /// ★ Named from what the player sees: a log lying west to east (its render)
  LOCALID_FIRST_LOG = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: a log lying north to south (its
  /// render)
  LOCALID_RIVER_LOG = LOCALID_FIRST_LOG + 2
};

#include "common/object_staged_object_event.h"

const struct MapWarpDest *KolimaForestCliff_GetWarpDests(void) {
  return gKolimaForestCliffWarpDests;
}

const struct MapWarpRegion *KolimaForestCliff_GetRegions(void) { return 0; }

const u32 *KolimaForestCliff_GetWarps(void) { return gKolimaForestCliffWarps; }

/// The three logs.
const struct ObjectEventTemplate *
KolimaForestCliff_GetObjectEventTemplates(void) {
  return gKolimaForestCliffObjectEventTemplates;
}

/// Kolima Forest (MapId_KolimaForestCliff), walking into a log: it rolls
/// ahead until something stops it. The river log, stopped at the bank (cell
/// column 12), tips over with a splash and floats in the water; its cells
/// become ground the party can cross (sets FLAG_MAP_TEMP_01).
static void KolimaForestCliff_PushLog(void) {
  struct PushedObjectProbe result;
  Engine_ScriptStart();
  if (PushedObject_FindClearPosition(&result)) {
    ObjectEvent_MoveAndRedraw(result);
    if (result.objectEventSlot == LOCALID_RIVER_LOG &&
        (result.position.x >> 20) == 12) {
      struct ObjectEvent *objectEvent;
      Engine_ObjectEventSetAnimation(LOCALID_RIVER_LOG,
                                     LOG_ANIMATION_ROLL_WEST);
      Engine_ObjectEventSetDestinationOffset(LOCALID_RIVER_LOG, -18, 6);
      Engine_ScriptWait(30);
      Engine_AudioPlayCue(SE_240);
      Engine_ObjectEventSetAnimation(LOCALID_RIVER_LOG, LOG_ANIMATION_DROP_IN);
      ObjectEvent_GetById(LOCALID_RIVER_LOG)->priorityFlags =
          OBJ_EVENT_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(32, 20, 2, 4, 11, 16);
      PushedObject_FillGridAttributeRectangle(2, 12, 16, 1, 4, 0);
      Engine_GameFlagSet(FLAG_MAP_TEMP_01);
      objectEvent = ObjectEvent_GetById(LOCALID_RIVER_LOG);
      Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
    }
  }
  Engine_ScriptEnd();
}

#include "common/kolima_jump.h"

/// The logs, the two ways out and the hop east from the bank.
const struct MapEvent *KolimaForestCliff_GetEvents(void) {
  return gKolimaForestCliffEvents;
}

/// Sets up the logs and the forest's colours on entry. A log already in the
/// river floats there again, the logs mark the cells they cover, and the forest
/// keeps its cursed colours until the curse is lifted.
s32 KolimaForestCliff_Initialize(void) {
  struct EventWork *work = gFieldWork;

  work->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Map_RedrawObjectEventFootprint(LOCALID_RIVER_LOG);

  if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_01) != 0) {
    ObjectEvent_GetById(LOCALID_RIVER_LOG)->priorityFlags =
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(32, 20, 2, 4, 11, 16);
    PushedObject_FillGridAttributeRectangle(2, 12, 16, 1, 4, 0);
    Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_RIVER_LOG), 0);
  }

  Map_RedrawObjectEventFootprint(LOCALID_FIRST_LOG);
  Map_RedrawObjectEventFootprint(LOCALID_FIRST_LOG + 1);

  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    KolimaPalette_Curse(6);
  }
  return 0;
}

#include "common/kolima_palette.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gPushedObject_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedObject_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedObject_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

struct MapWarpDest gKolimaForestCliffWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_GLADE, CONDITION_ALWAYS, 320, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_FLOODGATE, CONDITION_ALWAYS, 480, 0, 104, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

/// On into the forest (MapId_KolimaForestFloodgate) and back
/// (MapId_KolimaForestGlade).
u32 gKolimaForestCliffWarps[] = {
    MAP_WARPS(KolimaForestCliff),
    MAP_WARP(KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_FLOODGATE, KolimaForestFloodgate, KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_CLIFF),
    MAP_WARP(KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_GLADE, KolimaForestGlade, KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_CLIFF),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gKolimaForestCliffObjectEventTemplates[] = {
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(256), 0, PIXELS(344), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(224), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(312), 0, PIXELS(288), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};
// clang-format on

struct MapEvent gKolimaForestCliffEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_FLOODGATE},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_GLADE},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestCliff_PushLog)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForest_HopEast)},
    {MAP_EVENTS_END},
};

// The .bss order follows the first declarations (externs in field.h).
u8 gKolimaPalette_First[1792] __attribute__((section(".bss")));
u8 gKolimaPalette_Second[896] __attribute__((section(".bss")));
