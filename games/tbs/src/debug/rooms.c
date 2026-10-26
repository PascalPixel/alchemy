#include "unit/debug/rooms.h"
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
#include "map_tables.h"
#include "battle_unit.h"
#include "flags.h"
#include "djinn_ids.h"

/// The placed actors, numbered in the order of
/// gTestRoom_ActorTemplates:
/// the first Djinni of each element, which TestRoom_GiveDjinniForTesting
/// gives, placed while its flag (FLAG_DJINN_FIRST + element *
/// DJINN_PER_ELEMENT) is clear.
enum TestRoomsActorId {
  /// ★ Named from the game: "Flint" (MsgDjinnName)
  ACTOR_FLINT = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Fizz" (MsgDjinnName)
  ACTOR_FIZZ,
  /// ★ Named from the game: "Forge" (MsgDjinnName)
  ACTOR_FORGE,
  /// ★ Named from the game: "Gust" (MsgDjinnName)
  ACTOR_GUST
};

u8 *TestRoom_GetEntrances(void) { return (u8 *)gTestRoom_Entrances; }

/// The room has no exit regions: its regions entry returns none.
s32 Scene_GetExitRegions(void) { return 0; }

u8 *TestRoom_GetExits(void) { return (u8 *)gTestRoom_Exits; }

/// Places one Djinni of each element in the test room, Flint, Fizz, Forge and
/// Gust, each placed while its caught flag (the template's condition)
/// is clear.
u8 *TestRoom_GetActorTemplates(void) { return (u8 *)gTestRoom_ActorTemplates; }

/// Grants the first Djinni of an element, then clears its found flag for
/// testing. Djinn_JoinParty plays the element's capture effect on the
/// actor, frees its slot and gives the element's first Djinni to the member
/// with the fewest Djinn; clearing the flag the template tests lets the room
/// place the Djinni again on the next entry.
static __inline__ void TestRoom_GiveDjinniForTesting(s32 actorId, s32 element) {
  Djinn_JoinParty(actorId, element, 0);
  GameFlag_Clear(FLAG_DJINN_FIRST + element * DJINN_PER_ELEMENT);
}

static void TestRoom_ReceiveFlint(void) {
  TestRoom_GiveDjinniForTesting(ACTOR_FLINT, ELEMENT_VENUS);
}

static void TestRoom_ReceiveFizz(void) {
  TestRoom_GiveDjinniForTesting(ACTOR_FIZZ, ELEMENT_MERCURY);
}

static void TestRoom_ReceiveForge(void) {
  TestRoom_GiveDjinniForTesting(ACTOR_FORGE, ELEMENT_MARS);
}

static void TestRoom_ReceiveGust(void) {
  TestRoom_GiveDjinniForTesting(ACTOR_GUST, ELEMENT_JUPITER);
}

/// The room's events: talking to Flint, Fizz, Forge or Gust gives that
/// Djinni.
u8 *TestRoom_GetEvents(void) { return (u8 *)gTestRoom_Events; }

/// The room's initialize entry, which SceneManager_Run runs on entry: nothing
/// to set up.
s32 TestRoom_Initialize(void) { return 0; }

// XXX (no evidence): what they hold; no code reads them.
/// Eight rows of seven words that nothing reads; the menu test room keeps the
/// same 56 words (gDebugItemPromptHead, test.c), and no other data in the ROM
/// has their layout. Overlay data is reached only through the room's entries,
/// and none returns them.
u32 gTestRoom_EntrancesHead[] = {
    0x500050, 0, 80,  0x500000, 0x10003, 2, 0,
    0x780078, 0, 120, 0x780000, 0x10003, 2, 0,
    0x28003c, 0, 128, 0x800000, 0x10002, 1, 0,
    0x28003c, 0, 128, 0x800000, 0x10002, 3, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
    0x200020, 0, 0,   0,        0,       0, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
};

struct SceneEntrance gTestRoom_Entrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gTestRoom_Exits[] = {
    SCENE_EXITS(TestRoom0), SCENE_EXITS(TestRoom1), SCENE_EXITS(TestRoom2),
    SCENE_EXITS(TestRoom3), SCENE_EXITS_END,
};

struct ActorTemplate gTestRoom_ActorTemplates[] = {
    {0x5a, 0x30, MOVEMENT_TYPE_STAND, PIXELS(496), 0, PIXELS(176), FACING_EAST,
     TALK_FACING_PARTY, 0},
    {0x5b, 0x44, MOVEMENT_TYPE_STAND, PIXELS(496), 0, PIXELS(200), FACING_EAST,
     TALK_FACING_PARTY, 0},
    {0x5c, 0x58, MOVEMENT_TYPE_STAND, PIXELS(528), 0, PIXELS(176), FACING_WEST,
     TALK_FACING_PARTY, 0},
    {0x5d, 0x6c, MOVEMENT_TYPE_STAND, PIXELS(528), 0, PIXELS(200), FACING_WEST,
     TALK_FACING_PARTY, 0},
    {MAP_TABLE_END},
};

struct MapEvent gTestRoom_Events[] = {
    {MAP_EVENT_TALK, ACTOR_FLINT, CONDITION_ALWAYS,
     EVENT_SCRIPT(TestRoom_ReceiveFlint)},
    {MAP_EVENT_TALK, ACTOR_FIZZ, CONDITION_ALWAYS,
     EVENT_SCRIPT(TestRoom_ReceiveFizz)},
    {MAP_EVENT_TALK, ACTOR_FORGE, CONDITION_ALWAYS,
     EVENT_SCRIPT(TestRoom_ReceiveForge)},
    {MAP_EVENT_TALK, ACTOR_GUST, CONDITION_ALWAYS,
     EVENT_SCRIPT(TestRoom_ReceiveGust)},
    {MAP_EVENTS_END},
};
