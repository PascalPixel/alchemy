/// @file
/// The debug rooms Test 0 to Test 7, all one room, where talking to Flint,
/// Fizz, Forge or Gust gives that Djinni.

#include "djinn_ids.h"
#include "flags.h"
#include "types.h"
#include "battle/battle.h"
#include "field/event.h"
#include "field/scene_tables.h"
#include "system/system.h"

u8 *Scene_GetEntrances(void);
u8 *Scene_GetExits(void);
u8 *Scene_GetActorPlacements(void);
u8 *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gTest_Entrances[];
extern u32 gTest_Exits[];
extern struct ActorPlacement gTest_ActorPlacements[];
extern struct Trigger gTest_Triggers[];

/// The placed actors: the first Djinni of each element, each there while its
/// flag is clear.
enum TestActorId {
  ACTOR_FLINT = ACTOR_FIRST_PLACED,
  ACTOR_FIZZ,
  ACTOR_FORGE,
  ACTOR_GUST
};

u8 *Scene_GetEntrances(void) { return (u8 *)gTest_Entrances; }

/// The scene has no exit regions: its regions entry returns none.
s32 Scene_GetExitRegions(void) { return 0; }

u8 *Scene_GetExits(void) { return (u8 *)gTest_Exits; }

u8 *Scene_GetActorPlacements(void) { return (u8 *)gTest_ActorPlacements; }

/// Gives the first Djinni of an element, then clears its flag so it stays.
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

u8 *Scene_GetTriggers(void) { return (u8 *)gTest_Triggers; }

/// Nothing to set up.
s32 Scene_Initialize(void) { return 0; }

#include "debug/test_rows.h"

struct SceneEntrance gTest_Entrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gTest_Exits[] = {
    SCENE_EXITS(Test0), SCENE_EXITS(Test1), SCENE_EXITS(Test2),
    SCENE_EXITS(Test3), SCENE_EXITS_END,
};

struct ActorPlacement gTest_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_FLINT),
                    MOVEMENT_TYPE_STAND, 496, 0, 176, FACING_EAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_FIZZ),
                    MOVEMENT_TYPE_STAND, 496, 0, 200, FACING_EAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FORGE),
                    MOVEMENT_TYPE_STAND, 528, 0, 176, FACING_WEST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_GUST),
                    MOVEMENT_TYPE_STAND, 528, 0, 200, FACING_WEST,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gTest_Triggers[] = {
    {TRIGGER_TALK, ACTOR_FLINT, CONDITION_ALWAYS, EVENT(Test_ReceiveFlint)},
    {TRIGGER_TALK, ACTOR_FIZZ, CONDITION_ALWAYS, EVENT(Test_ReceiveFizz)},
    {TRIGGER_TALK, ACTOR_FORGE, CONDITION_ALWAYS, EVENT(Test_ReceiveForge)},
    {TRIGGER_TALK, ACTOR_GUST, CONDITION_ALWAYS, EVENT(Test_ReceiveGust)},
    {TRIGGER_TABLE_END},
};
