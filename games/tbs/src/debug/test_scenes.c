/* @file */
/* The debug rooms Test 0 to Test 7, all one room, where talking to Flint, */
/* Fizz, Forge or Gust gives that Djinni. */

#include "djinn_ids.h"
#include "flags.h"
#include "battle/battle.h"
#include "field/event.h"

const SCENE_ENTRANCE *scGetEntrances(void);
const u32 *scGetExits(void);
const ACTOR_PLACE *scGetActorPlacements(void);
const TRIGGER *scGetTriggers(void);
s32 scInit(void);
extern SCENE_ENTRANCE Test_Entrances[];
extern u32 Test_Exits[];
extern ACTOR_PLACE Test_ActorPlacements[];
extern TRIGGER Test_Triggers[];

/* The placed actors: the first Djinni of each element, each there while its */
/* flag is clear. */
typedef enum TEST_ACTOR_ID {
	ACTOR_FLINT = ACTOR_FIRST_PLACED,
	ACTOR_FIZZ,
	ACTOR_FORGE,
	ACTOR_GUST
} TEST_ACTOR_ID;

const SCENE_ENTRANCE *scGetEntrances(void) { return Test_Entrances; }

/* The scene has no exit regions: its regions entry returns none. */
const SCENE_EXIT_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return Test_Exits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return Test_ActorPlacements; }

/* Gives the first Djinni of an element, then clears its flag so it stays. */
static __inline__ void dbTestGiveDjinni(s32 actorId, s32 element) {
	fdDjinnJoinParty(actorId, element, 0);
	gmFlagClear(FLAG_DJINN_FIRST + element * DJINN_PER_ELEMENT);
}

/* Talking to Flint, Fizz, Forge or Gust gives that Djinni to the party. */
static void dbTestReceiveFlint(void) {
	dbTestGiveDjinni(ACTOR_FLINT, ELEMENT_VENUS);
}

static void dbTestReceiveFizz(void) {
	dbTestGiveDjinni(ACTOR_FIZZ, ELEMENT_MERCURY);
}

static void dbTestReceiveForge(void) {
	dbTestGiveDjinni(ACTOR_FORGE, ELEMENT_MARS);
}

static void dbTestReceiveGust(void) {
	dbTestGiveDjinni(ACTOR_GUST, ELEMENT_JUPITER);
}

const TRIGGER *scGetTriggers(void) { return Test_Triggers; }

/* Nothing to set up. */
s32 scInit(void) { return 0; }

#include "test_rows.c"

SCENE_ENTRANCE Test_Entrances[] = {
	{0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

/* No exits: the headers of Test 0 to Test 3's tables, with none under them. */
u32 Test_Exits[] = {
	SCENE_EXITS(Test0), SCENE_EXITS(Test1), SCENE_EXITS(Test2),
	SCENE_EXITS(Test3), SCENE_EXITS_END,
};

ACTOR_PLACE Test_ActorPlacements[] = {
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

TRIGGER Test_Triggers[] = {
	{TRIGGER_TALK, ACTOR_FLINT, CONDITION_ALWAYS, EVENT(dbTestReceiveFlint)},
	{TRIGGER_TALK, ACTOR_FIZZ, CONDITION_ALWAYS, EVENT(dbTestReceiveFizz)},
	{TRIGGER_TALK, ACTOR_FORGE, CONDITION_ALWAYS, EVENT(dbTestReceiveForge)},
	{TRIGGER_TALK, ACTOR_GUST, CONDITION_ALWAYS, EVENT(dbTestReceiveGust)},
	{TRIGGER_TABLE_END},
};
