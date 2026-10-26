/// @file
/// Kolima Forest (SceneId_KolimaForestGlade), the second of the forest's
/// five maps, on the party's way to Tret between SceneId_KolimaForestEntrance
/// and SceneId_KolimaForestCliff. Three logs lie in the grass to be rolled by
/// walking into them, and a Nut floats out of reach until Catch pulls it in
/// (FLAG_KOLIMA_FOREST_NUT_CAUGHT). Until Tret lifts the curse
/// (FLAG_KOLIMA_CURSE_LIFTED), KolimaPalette_Curse gives the forest redder,
/// darker colours.
///
/// Until the third patch the Nut's object event template row tested Vale's
/// FLAG_VALE_NUT_CAUGHT rather than FLAG_KOLIMA_FOREST_NUT_CAUGHT, so once
/// Vale's Nut had been caught this one was never placed.
///
/// The map's name, Glade, is ours, for what sets the map apart: open grass
/// and stumps ringed by trees, with no cliff or river
/// (places/kolima-forest-glade_cursed/map-0.png).

#include "speaker.h"
#include "flags.h"
#include "version.h"
#include "heap_slots.h"
#include "unit/import.h"
#include "unit/scene/kolima_forest_glade.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/scene/common/kolima_palette.h"
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
#include "script_layer.h"
#include "pushed_object.h"
#include "io_reg.h"
#include "ability_ids.h"
#include "map_tables.h"
#include "item_ids.h"
#include "heap.h"

/// Kolima Forest's object events, numbering the rows of
/// gKolimaForestGlade_ObjectEventTemplates.
enum KolimaForestGladeLocalId {
  /// The three logs.
  /// ★ Named from what the player sees: logs lying on the forest grass among
  /// the stumps (its render)
  LOCALID_FIRST_LOG = LOCALID_FIRST_PLACED,
  /// The Nut that floats until Catch pulls it in.
  /// ★ Named from the game: "Nut" (MsgItemName)
  LOCALID_NUT = LOCALID_FIRST_LOG + 3
};

#include "common/object_staged_object_event.h"

/// Where the party enters, from SceneId_KolimaForestEntrance and
/// SceneId_KolimaForestCliff.
const struct MapWarpDest *KolimaForestGlade_GetWarpDests(void) {
  return gKolimaForestGlade_WarpDests;
}

const struct MapWarpRegion *KolimaForestGlade_GetRegions(void) { return 0; }

/// The ways on into the forest (SceneId_KolimaForestCliff) and back
/// (SceneId_KolimaForestEntrance).
const u32 *KolimaForestGlade_GetWarps(void) { return gKolimaForestGlade_Warps; }

/// The three logs and the Nut.
const struct ObjectEventTemplate *
KolimaForestGlade_GetObjectEventTemplates(void) {
  return gKolimaForestGlade_ObjectEventTemplates;
}

/// Kolima Forest (SceneId_KolimaForestGlade), walking into a log: it rolls
/// ahead until something stops it.
static void KolimaForestGlade_PushLog(void) {
  struct PushedObjectProbe result;
  FieldScript_Start();
  if (PushedObject_FindClearPosition(&result))
    ObjectEvent_MoveAndRedraw(result);
  FieldScript_End();
}

/// Kolima Forest (SceneId_KolimaForestGlade), Catch on the floating Nut (until
/// FLAG_KOLIMA_FOREST_NUT_CAUGHT): it vanishes and the party finds a Nut.
static void KolimaForestGlade_CatchNut(void) {
  FieldScript_Start();
  Import_ObjectEventSetPosition(LOCALID_NUT, 0, 0);
  GameFlag_Set(FLAG_KOLIMA_FOREST_NUT_CAUGHT);
  ItemDisplay_ShowItem(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
  FieldScript_End();
}

/// The logs, the Nut and the two ways out.
const struct MapEvent *KolimaForestGlade_GetEvents(void) {
  return gKolimaForestGlade_Events;
}

/// Sets up the floating Nut and the logs on entry. The Nut floats until it is
/// caught, the logs mark the cells they cover, and the forest keeps its cursed
/// colours until the curse is lifted.
s32 KolimaForestGlade_Initialize(void) {

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (GameFlag_Get(FLAG_KOLIMA_FOREST_NUT_CAUGHT) == 0) {
    Scene_FloatNut(LOCALID_NUT);
  }
  Scene_RedrawObjectEventFootprint(LOCALID_FIRST_LOG);
  Scene_RedrawObjectEventFootprint(LOCALID_FIRST_LOG + 1);
  Scene_RedrawObjectEventFootprint(LOCALID_FIRST_LOG + 2);
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    KolimaPalette_Curse(11);
  }
  return 0;
}

#include "common/kolima_palette.h"
#include "common/object_orbit.h"

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

struct MapWarpDest gKolimaForestGlade_WarpDests[] = {
    { 0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_ENTRANCE, CONDITION_ALWAYS, 480, 0, 376, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_CLIFF, CONDITION_ALWAYS, 312, 0, 40, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

/// KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_CLIFF on into the forest, and
/// KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_ENTRANCE back.
u32 gKolimaForestGlade_Warps[] = {
    MAP_WARPS(KolimaForestGlade),
    MAP_WARP(KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_CLIFF, KolimaForestCliff, KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_GLADE),
    MAP_WARP(KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_ENTRANCE, KolimaForestEntrance, KOLIMA_FOREST_ENTRANCE_FROM_KOLIMA_FOREST_GLADE),
    MAP_WARPS_END,
};

/// Until the third patch the Nut's object event template tested Vale's FLAG_VALE_NUT_CAUGHT, not the flag
/// catching it sets.
struct ObjectEventTemplate gKolimaForestGlade_ObjectEventTemplates[] = {
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(360), 0, PIXELS(368), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(160), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(136), 0, PIXELS(336), FACING_SOUTH, TALK_FACING_KEEP, 0 },
#if !TBS_PATCH_3
    { SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(280), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
#else
    { SPRITE_BLANK, FLAG_KOLIMA_FOREST_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(280), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
#endif
    { MAP_TABLE_END },
};
// clang-format on

struct MapEvent gKolimaForestGlade_Events[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_CLIFF},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_ENTRANCE},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestGlade_PushLog)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_NUT,
     FLAG_KOLIMA_FOREST_NUT_CAUGHT, EVENT_SCRIPT(KolimaForestGlade_CatchNut)},
    {MAP_EVENTS_END},
};

// The .bss order follows the first declarations (externs in field.h).
u8 gKolimaPalette_Healthy[1792] __attribute__((section(".bss")));
u8 gKolimaPalette_Cursed[896] __attribute__((section(".bss")));
