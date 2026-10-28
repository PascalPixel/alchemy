#include "module/debug/test_scenes.h"
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
#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "battle_unit.h"
#include "flags.h"
#include "djinn_ids.h"

/// The placed actors, numbered in the order of
/// gTest_ActorPlacements:
/// the first Djinni of each element, which Test_GiveDjinniForTesting
/// gives, placed while its flag (FLAG_DJINN_FIRST + element *
/// DJINN_PER_ELEMENT) is clear.
enum TestActorId {
  /// ★ Named from the game: "Flint" (MsgDjinnName)
  ACTOR_FLINT = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Fizz" (MsgDjinnName)
  ACTOR_FIZZ,
  /// ★ Named from the game: "Forge" (MsgDjinnName)
  ACTOR_FORGE,
  /// ★ Named from the game: "Gust" (MsgDjinnName)
  ACTOR_GUST
};

u8 *Test_GetEntrances(void) { return (u8 *)gTest_Entrances; }

/// The scene has no exit regions: its regions entry returns none.
s32 Scene_GetExitRegions(void) { return 0; }

u8 *Test_GetExits(void) { return (u8 *)gTest_Exits; }

/// Places one Djinni of each element in the test scenes, Flint, Fizz, Forge and
/// Gust, each placed while its caught flag (the placement's condition)
/// is clear.
u8 *Test_GetActorPlacements(void) { return (u8 *)gTest_ActorPlacements; }

/// Grants the first Djinni of an element, then clears its found flag for
/// testing. Djinn_JoinParty plays the element's capture effect on the
/// actor, releases its slot and gives the element's first Djinni to the party
/// member with the fewest Djinn; clearing the flag the placement tests lets the
/// scene place the Djinni again on the next entry.
static __inline__ void Test_GiveDjinniForTesting(s32 actorId, s32 element) {
  Djinn_JoinParty(actorId, element, 0);
  GameFlag_Clear(FLAG_DJINN_FIRST + element * DJINN_PER_ELEMENT);
}

static void Test_ReceiveFlint(void) {
  Test_GiveDjinniForTesting(ACTOR_FLINT, ELEMENT_VENUS);
}

static void Test_ReceiveFizz(void) {
  Test_GiveDjinniForTesting(ACTOR_FIZZ, ELEMENT_MERCURY);
}

static void Test_ReceiveForge(void) {
  Test_GiveDjinniForTesting(ACTOR_FORGE, ELEMENT_MARS);
}

static void Test_ReceiveGust(void) {
  Test_GiveDjinniForTesting(ACTOR_GUST, ELEMENT_JUPITER);
}

/// The scene's events: talking to Flint, Fizz, Forge or Gust gives that
/// Djinni.
u8 *Test_GetTriggers(void) { return (u8 *)gTest_Triggers; }

/// The scene's initialize entry, which FieldSequence_Run runs on entry: nothing
/// to set up.
s32 Test_Initialize(void) { return 0; }

// XXX (no evidence): what they hold; no code reads them.
/// Eight rows of seven words that nothing reads; the Menu test scene keeps the
/// same 56 words (gDebugItemPromptHead, test.c), and no other data in the ROM
/// has their layout. Overlay data is reached only through the scene's entries,
/// and none returns them.
u32 gTest_EntrancesHead[] = {
    0x500050, 0, 80,  0x500000, 0x10003, 2, 0,
    0x780078, 0, 120, 0x780000, 0x10003, 2, 0,
    0x28003c, 0, 128, 0x800000, 0x10002, 1, 0,
    0x28003c, 0, 128, 0x800000, 0x10002, 3, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
    0x200020, 0, 0,   0,        0,       0, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
};

struct SceneEntrance gTest_Entrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gTest_Exits[] = {
    SCENE_EXITS(Test0), SCENE_EXITS(Test1), SCENE_EXITS(Test2),
    SCENE_EXITS(Test3), SCENE_EXITS_END,
};

struct ActorPlacement gTest_ActorPlacements[] = {
    {0x5a, 0x30, MOVEMENT_TYPE_STAND, PIXELS(496), 0, PIXELS(176), FACING_EAST,
     TALK_FACING_PARTY, 0},
    {0x5b, 0x44, MOVEMENT_TYPE_STAND, PIXELS(496), 0, PIXELS(200), FACING_EAST,
     TALK_FACING_PARTY, 0},
    {0x5c, 0x58, MOVEMENT_TYPE_STAND, PIXELS(528), 0, PIXELS(176), FACING_WEST,
     TALK_FACING_PARTY, 0},
    {0x5d, 0x6c, MOVEMENT_TYPE_STAND, PIXELS(528), 0, PIXELS(200), FACING_WEST,
     TALK_FACING_PARTY, 0},
    {SCENE_TABLE_END},
};

struct Trigger gTest_Triggers[] = {
    {TRIGGER_TALK, ACTOR_FLINT, CONDITION_ALWAYS, EVENT(Test_ReceiveFlint)},
    {TRIGGER_TALK, ACTOR_FIZZ, CONDITION_ALWAYS, EVENT(Test_ReceiveFizz)},
    {TRIGGER_TALK, ACTOR_FORGE, CONDITION_ALWAYS, EVENT(Test_ReceiveForge)},
    {TRIGGER_TALK, ACTOR_GUST, CONDITION_ALWAYS, EVENT(Test_ReceiveGust)},
    {TRIGGERS_END},
};
