/* @file */
/* The eight test scenes that share one room, whose talk events would give */
/* Flint, Fizz, Forge or Gust; the room places no actors to talk to. */

#include "field/field.h"
#include "game/game.h"
#include "field/scene_tables.h"
#include "field/event_inline.h"
#include "macros.h"

/* The actors the talk events answer: the first Djinni of each element. The */
/* room's actor list is empty, so none of them is ever there. */
typedef enum TEST_ACTOR_ID {
	ACTOR_FLINT = ACTOR_FIRST_PLACED,
	ACTOR_FIZZ,
	ACTOR_FORGE,
	ACTOR_GUST
} TEST_ACTOR_ID;

void dbTestReceiveFizz(void);

void dbTestReceiveForge(void);

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* Gives the first Djinni of an element, then clears its found flag again, so */
/* talking to it can give it once more. */
static __inline__ void dbTestGiveDjinni(s32 actorId, s32 element) {
	fdDjinnJoinParty(actorId, element, 0);
	syGameFlagClear(FLAG_DJINN_FIRST + element * DJINN_PER_ELEMENT);
}

void dbTestReceiveFlint(void) { dbTestGiveDjinni(ACTOR_FLINT, ELEMENT_VENUS); }

void dbTestReceiveFizz(void) { dbTestGiveDjinni(ACTOR_FIZZ, ELEMENT_MERCURY); }

void dbTestReceiveForge(void) { dbTestGiveDjinni(ACTOR_FORGE, ELEMENT_MARS); }

void dbTestReceiveGust(void) { dbTestGiveDjinni(ACTOR_GUST, ELEMENT_JUPITER); }

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

/* Nothing to set up. */
s32 scInit(void) { return 0; }

s32 scPrepareMap(void) { return 0; }

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 SceneExits[] = {
	SCENE_EXITS(Test0), SCENE_EXITS(Test1), SCENE_EXITS(Test2),
	SCENE_EXITS(Test3), SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	{SCENE_TABLE_END},
};

TRIGGER SceneEvents[] = {
	{TRIGGER_TALK, ACTOR_FLINT, CONDITION_ALWAYS, EVENT(dbTestReceiveFlint)},
	{TRIGGER_TALK, ACTOR_FIZZ, CONDITION_ALWAYS, EVENT(dbTestReceiveFizz)},
	{TRIGGER_TALK, ACTOR_FORGE, CONDITION_ALWAYS, EVENT(dbTestReceiveForge)},
	{TRIGGER_TALK, ACTOR_GUST, CONDITION_ALWAYS, EVENT(dbTestReceiveGust)},
	{TRIGGER_TABLE_END},
};

/* Unused: four halfwords and an end of -1s after the room's tables; nothing */
/* reads them. */
s16 TestScenes_UnusedRow[] ALIGNED(4) = {
	512, 512, 1024, 864, -1, -1, -1, -1,
};
