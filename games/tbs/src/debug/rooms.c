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
#include "field_offset.h"
#include "script_layer.h"
#include "map_tables.h"
#include "battle_unit.h"
#include "flags.h"
#include "djinn_ids.h"

/// The placed object events, numbered in the order of
/// gTestRooms_ObjectEventTemplates:
/// the first Djinni of each element, which TestRooms_GiveDjinniForTesting
/// gives, placed while its flag (FLAG_DJINN_FIRST + element *
/// DJINN_PER_ELEMENT) is clear.
enum TestRoomsLocalId {
  /// ★ Named from the game: "Flint" (MsgDjinnName)
  LOCALID_FLINT = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "Fizz" (MsgDjinnName)
  LOCALID_FIZZ,
  /// ★ Named from the game: "Forge" (MsgDjinnName)
  LOCALID_FORGE,
  /// ★ Named from the game: "Gust" (MsgDjinnName)
  LOCALID_GUST
};

u8 *TestRooms_GetWarpDests(void) { return (u8 *)gTestRooms_WarpDests; }

/// The room has no warp regions: its regions entry returns none.
s32 MapData_ReturnZero(void) { return 0; }

u8 *TestRooms_GetWarps(void) { return (u8 *)gTestRooms_Warps; }

/// Places one Djinni of each element in the test room, Flint, Fizz, Forge and
/// Gust, each placed while its caught flag (the template's condition)
/// is clear.
u8 *TestRooms_GetObjectEventTemplates(void) {
  return (u8 *)gTestRooms_ObjectEventTemplates;
}

/// Grants the first Djinni of an element, then clears its found flag for
/// testing. Djinn_JoinParty plays the element's capture effect on the object
/// event, frees its slot and gives the element's first Djinni to the member
/// with the fewest Djinn; clearing the flag the template tests lets the room
/// place the Djinni again on the next entry.
static __inline__ void TestRooms_GiveDjinniForTesting(s32 localId,
                                                      s32 element) {
  Djinn_JoinParty(localId, element, 0);
  Engine_GameFlagClear(FLAG_DJINN_FIRST + element * DJINN_PER_ELEMENT);
}

static void TestRooms_ReceiveFlint(void) {
  TestRooms_GiveDjinniForTesting(LOCALID_FLINT, ELEMENT_VENUS);
}

static void TestRooms_ReceiveFizz(void) {
  TestRooms_GiveDjinniForTesting(LOCALID_FIZZ, ELEMENT_MERCURY);
}

static void TestRooms_ReceiveForge(void) {
  TestRooms_GiveDjinniForTesting(LOCALID_FORGE, ELEMENT_MARS);
}

static void TestRooms_ReceiveGust(void) {
  TestRooms_GiveDjinniForTesting(LOCALID_GUST, ELEMENT_JUPITER);
}

/// The room's events: talking to Flint, Fizz, Forge or Gust gives that
/// Djinni.
u8 *TestRooms_GetEvents(void) { return (u8 *)gTestRooms_Events; }

/// The room's initialize entry, which Field_RunMap runs on entry: nothing
/// to set up.
s32 Map_RunEmpty(void) { return 0; }

/// Eight rows of seven words that nothing reads; the menu test room keeps the
/// same 56 words (gDebugItemPromptHead, test.c), and no other data in the ROM
/// has their layout. Overlay data is reached only through the room's entries,
/// and none returns them. XXX: what they hold is unknown, since no code reads
/// them.
u32 gTestRooms_WarpDestsHead[] = {
    0x500050, 0, 80,  0x500000, 0x10003, 2, 0,
    0x780078, 0, 120, 0x780000, 0x10003, 2, 0,
    0x28003c, 0, 128, 0x800000, 0x10002, 1, 0,
    0x28003c, 0, 128, 0x800000, 0x10002, 3, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
    0x200020, 0, 0,   0,        0,       0, 0,
    0x800080, 0, 128, 0x800000, 0x20003, 1, 0,
};

struct MapWarpDest gTestRooms_WarpDests[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gTestRooms_Warps[] = {
    MAP_WARPS(TestRoom0), MAP_WARPS(TestRoom1), MAP_WARPS(TestRoom2),
    MAP_WARPS(TestRoom3), MAP_WARPS_END,
};

struct ObjectEventTemplate gTestRooms_ObjectEventTemplates[] = {
    {0x5a, 0x30, MOVEMENT_TYPE_STAND, PIXELS(496), 0, PIXELS(176), FACING_EAST,
     TALK_FACE_PARTY, 0},
    {0x5b, 0x44, MOVEMENT_TYPE_STAND, PIXELS(496), 0, PIXELS(200), FACING_EAST,
     TALK_FACE_PARTY, 0},
    {0x5c, 0x58, MOVEMENT_TYPE_STAND, PIXELS(528), 0, PIXELS(176), FACING_WEST,
     TALK_FACE_PARTY, 0},
    {0x5d, 0x6c, MOVEMENT_TYPE_STAND, PIXELS(528), 0, PIXELS(200), FACING_WEST,
     TALK_FACE_PARTY, 0},
    {MAP_TABLE_END},
};

struct MapEvent gTestRooms_Events[] = {
    {MAP_EVENT_TALK, LOCALID_FLINT, CONDITION_ALWAYS,
     EVENT_SCRIPT(TestRooms_ReceiveFlint)},
    {MAP_EVENT_TALK, LOCALID_FIZZ, CONDITION_ALWAYS,
     EVENT_SCRIPT(TestRooms_ReceiveFizz)},
    {MAP_EVENT_TALK, LOCALID_FORGE, CONDITION_ALWAYS,
     EVENT_SCRIPT(TestRooms_ReceiveForge)},
    {MAP_EVENT_TALK, LOCALID_GUST, CONDITION_ALWAYS,
     EVENT_SCRIPT(TestRooms_ReceiveGust)},
    {MAP_EVENTS_END},
};
