/// @file
/// The cave of the Apojii Islands: four clumps of weeds on the walls of its
/// south chamber, each scattered for good by Whirlwind. The third hides the
/// passage north, which leads out to the top of the falls where the Djinni
/// Haze waits.

#include "shared/tables.h"
#include "field/field_sequence_event.h"
#include "battle/battle.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "system/ram_buffer.h"
#include "flags.h"
#include "field/event_inline.h"
#include "field/actor_script.h"
#include "fixed_math.h"

/// The ways between the cave's south chamber and the passage north, by the
/// passage the third clump of weeds hides.
#define APOJII_ISLANDS_CAVE_SOUTH_TO_NORTH 1
#define APOJII_ISLANDS_CAVE_NORTH_TO_SOUTH 2
#define APOJII_ISLANDS_CAVE_SOUTH_FROM_NORTH 1
#define APOJII_ISLANDS_CAVE_NORTH_FROM_SOUTH 2

extern u32 gApojiiIslandsCave_WeedsScript[];

/// The four clumps of weeds on the south chamber's walls, from west to east,
/// numbering the rows of gActorPlacements.
enum ApojiiIslandsCaveActorId {
  ACTOR_FIRST_WEEDS = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_WEEDS,
  ACTOR_PASSAGE_WEEDS, ///< over the passage north
  ACTOR_FOURTH_WEEDS
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Whirlwind on a clump of weeds scatters it for good.
void ApojiiIslandsCave_ScatterWeeds(s32 unused, s32 weeds) {
  GameFlag_Set(FLAG_APOJII_CAVE_FOLIAGE_BLOWN(weeds));
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// clang-format off
/// The weeds' script: each clump stands still three pixels up, on ground four
/// pixels up, drawn half as tall again.
u32 gApojiiIslandsCave_WeedsScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_GROUND, toFIXED(4)),
    SC_SET(SW_Y, toFIXED(3)),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 3 / 2),
    SC_END,
};
// clang-format on

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(ApojiiIslandsCave),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_CAVE_SOUTH_TO_NORTH, ApojiiIslandsCave,
                      APOJII_ISLANDS_CAVE_NORTH_FROM_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_CAVE_NORTH_TO_SOUTH, ApojiiIslandsCave,
                      APOJII_ISLANDS_CAVE_SOUTH_FROM_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        APOJII_ISLANDS_CAVE_TO_APOJII_ISLANDS_HAZE_SOUTHWEST, ApojiiIslandsHaze,
        APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_CAVE_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        APOJII_ISLANDS_CAVE_TO_APOJII_ISLANDS_HAZE_SOUTHEAST, ApojiiIslandsHaze,
        APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_CAVE_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        APOJII_ISLANDS_CAVE_TO_APOJII_ISLANDS_HAZE_NORTH, ApojiiIslandsHaze,
        APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_CAVE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WEEDS,
                    FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_FIRST_WEEDS),
                    (s32)gApojiiIslandsCave_WeedsScript, 120, 0, 608,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS,
                    FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_SECOND_WEEDS),
                    (s32)gApojiiIslandsCave_WeedsScript, 184, 0, 592,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS,
                    FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_PASSAGE_WEEDS),
                    (s32)gApojiiIslandsCave_WeedsScript, 216, 0, 592,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS,
                    FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_FOURTH_WEEDS),
                    (s32)gApojiiIslandsCave_WeedsScript, 280, 0, 640,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1,
     IF_FLAG_SET(FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_PASSAGE_WEEDS)),
     APOJII_ISLANDS_CAVE_SOUTH_TO_NORTH},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, APOJII_ISLANDS_CAVE_NORTH_TO_SOUTH},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS,
     APOJII_ISLANDS_CAVE_TO_APOJII_ISLANDS_HAZE_SOUTHWEST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     APOJII_ISLANDS_CAVE_TO_APOJII_ISLANDS_HAZE_SOUTHEAST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS,
     APOJII_ISLANDS_CAVE_TO_APOJII_ISLANDS_HAZE_NORTH},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_WEEDS,
     FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_FIRST_WEEDS),
     EVENT(ApojiiIslandsCave_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SECOND_WEEDS,
     FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_SECOND_WEEDS),
     EVENT(ApojiiIslandsCave_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_PASSAGE_WEEDS,
     FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_PASSAGE_WEEDS),
     EVENT(ApojiiIslandsCave_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FOURTH_WEEDS,
     FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_FOURTH_WEEDS),
     EVENT(ApojiiIslandsCave_ScatterWeeds)},
    {TRIGGER_TABLE_END},
};
