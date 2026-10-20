#include "unit/debug/rooms.h"
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
#include "field_event.h"
#include "field_scene.h"
#include "battle_unit.h"
#include "flags.h"

u8 *TestRooms_GetEntrances(void) { return (u8 *)gTestRooms_Entrances; }

/// The room has no exit regions: its regions entry returns none.
s32 SceneData_ReturnZero(void) { return 0; }

u8 *TestRooms_GetExits(void) { return (u8 *)gTestRooms_Exits; }

/// Places one Djinni of each element in the test room: actors 8 to 11,
/// Flint, Fizz, Forge and Gust, each placed while its caught flag (the
/// placement's condition) is clear.
u8 *TestRooms_GetPlacements(void) { return (u8 *)gTestRooms_Placements; }

/// Grants the first Djinni of an element, then clears its found flag for
/// testing. Djinn_JoinParty plays the element's capture effect on the actor,
/// frees its slot and gives Djinni 0 of the element to the member with the
/// fewest Djinn; clearing the flag the placement tests lets the room place
/// the Djinni again on the next entry.
static __inline__ void TestRooms_GiveDjinniForTesting(s32 actorId,
                                                      s32 element) {
  Djinn_JoinParty(actorId, element, 0);
  Engine_GameFlagClear(FLAG_DJINNI_0 + element * 20);
}

static void TestRooms_ReceiveFlint(void) {
  TestRooms_GiveDjinniForTesting(8, ELEMENT_VENUS);
}

static void TestRooms_ReceiveFizz(void) {
  TestRooms_GiveDjinniForTesting(9, ELEMENT_MERCURY);
}

static void TestRooms_ReceiveForge(void) {
  TestRooms_GiveDjinniForTesting(10, ELEMENT_MARS);
}

static void TestRooms_ReceiveGust(void) {
  TestRooms_GiveDjinniForTesting(11, ELEMENT_JUPITER);
}

/// The room's events: talking to Djinni actor 8, 9, 10 or 11 gives that
/// Djinni.
u8 *TestRooms_GetEvents(void) { return (u8 *)gTestRooms_Events; }

/// The room's initialize entry, which Field_RunScene runs on entry: nothing
/// to set up.
s32 Scene_RunEmptyScene(void) { return 0; }

/// Eight rows of seven words that nothing reads by name; the menu test room
/// keeps the same 56 words (gDebugItemPromptHead, test.c). XXX: what they
/// hold is not known; a reader of them, or their source, would say.
u32 gTestRooms_EntrancesHead[] = {
    0x500050, 0, 80,  0x500000, 0x10003, 2, 0,
    0x780078, 0, 120, 0x780000, 0x10003, 2, 0,
    0x28003c, 0, 128, 0x800000, 0x10002, 1, 0,
    0x28003c, 0, 128, 0x800000, 0x10002, 3, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
    0x200020, 0, 0,   0,        0,       0, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
};

struct SceneEntrance gTestRooms_Entrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gTestRooms_Exits[] = {
    SCENE_EXITS(191),      SCENE_EXIT(0, 192, 0), SCENE_EXIT(0, 193, 0),
    SCENE_EXIT(0, 194, 0), SCENE_EXITS_END,
};

struct ScenePlacement gTestRooms_Placements[] = {
    {0x5a, 0x30, ACTOR_STAND, PIXELS(496), 0, PIXELS(176), FACING_EAST,
     TALK_FACE_PARTY, 0},
    {0x5b, 0x44, ACTOR_STAND, PIXELS(496), 0, PIXELS(200), FACING_EAST,
     TALK_FACE_PARTY, 0},
    {0x5c, 0x58, ACTOR_STAND, PIXELS(528), 0, PIXELS(176), FACING_WEST,
     TALK_FACE_PARTY, 0},
    {0x5d, 0x6c, ACTOR_STAND, PIXELS(528), 0, PIXELS(200), FACING_WEST,
     TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

struct SceneEvent gTestRooms_Events[] = {
    {EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(TestRooms_ReceiveFlint)},
    {EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(TestRooms_ReceiveFizz)},
    {EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(TestRooms_ReceiveForge)},
    {EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(TestRooms_ReceiveGust)},
    {SCENE_EVENTS_END},
};
