/// @file
/// The cave of the Apojii Islands: four patches of overgrowth on its walls,
/// each blown away for good by Whirlwind. The third hides the passage to the
/// Djinni Haze, exit 1, which opens once that patch is gone.
///
/// XXX: what the four sprite 307 actors look like, which Telago's guide calls
/// "overgrowth foliage"; a render of the cave would settle it.

#include "shared/tables.h"
#include "event_work.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "flags.h"
#include "module/scene/apojii_islands_cave.h"

/// The four patches of overgrowth, numbering the rows of gActorPlacements.
enum ApojiiIslandsCaveActorId {
  ACTOR_FIRST_FOLIAGE = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_FOLIAGE,
  ACTOR_PASSAGE_FOLIAGE, ///< over the passage to Haze, exit 1
  ACTOR_FOURTH_FOLIAGE
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#include "event_inline.h"

/// Whirlwind on a patch of overgrowth blows it away for good.
void ApojiiIslandsCave_BlowAwayFoliage(s32 unused, s32 actor) {
  GameFlag_SetBit(FLAG_APOJII_CAVE_FOLIAGE_BLOWN(actor));
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x201;
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// clang-format off
u32 gApojiiIslandsCave_FoliageScript[] = {
    22, 26, 0, 22, 22, 0x40000,
    22, 6, 0x30000, 22, 10, 0x18000,
    17,
};
// clang-format on

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_SCENE_EXITS(157, 157, 2, 157, 1, 155, 3, 4, 155, 5)

struct ActorPlacement gActorPlacements[] = {
    {307, FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_FIRST_FOLIAGE),
     (s32)gApojiiIslandsCave_FoliageScript, PIXELS(120), 0, PIXELS(608),
     FACING_NORTH, 2, 0},
    {307, FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_SECOND_FOLIAGE),
     (s32)gApojiiIslandsCave_FoliageScript, PIXELS(184), 0, PIXELS(592),
     FACING_NORTH, 2, 0},
    {307, FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_PASSAGE_FOLIAGE),
     (s32)gApojiiIslandsCave_FoliageScript, PIXELS(216), 0, PIXELS(592),
     FACING_NORTH, 2, 0},
    {307, FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_FOURTH_FOLIAGE),
     (s32)gApojiiIslandsCave_FoliageScript, PIXELS(280), 0, PIXELS(640),
     FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1,
     IF_FLAG_SET(FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_PASSAGE_FOLIAGE)), 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_FOLIAGE,
     FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_FIRST_FOLIAGE),
     EVENT(ApojiiIslandsCave_BlowAwayFoliage)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SECOND_FOLIAGE,
     FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_SECOND_FOLIAGE),
     EVENT(ApojiiIslandsCave_BlowAwayFoliage)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_PASSAGE_FOLIAGE,
     FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_PASSAGE_FOLIAGE),
     EVENT(ApojiiIslandsCave_BlowAwayFoliage)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FOURTH_FOLIAGE,
     FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_FOURTH_FOLIAGE),
     EVENT(ApojiiIslandsCave_BlowAwayFoliage)},
    {TRIGGER_TABLE_END},
};
