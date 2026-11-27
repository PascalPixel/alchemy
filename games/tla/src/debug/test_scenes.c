// The eight test scenes that share one room, whose talk scripts would give Flint, Fizz, Forge or
// Gust; the room places no actors to talk to.

#include "field/field.h"
#include "game/game.h"
#include "field/scene_tables.h"
#include "field/script_inline.h"
#include "macros.h"

// The actors the talk scripts answer: the first Djinni of each element. The room's actor list is
// empty, so none of them is ever there.
typedef enum TEST_ACTOR_ID
{
	ACTOR_FLINT = ACTOR_FIRST_PLACED,
	ACTOR_FIZZ,
	ACTOR_FORGE,
	ACTOR_GUST
} TEST_ACTOR_ID;

void                  dbTestReceiveFizz(void);

void                  dbTestReceiveForge(void);

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

// Gives the first Djinni of an element, then clears its found flag again, so talking to it can give
// it once more.
static __inline__ void dbTestGiveDjinni(Sint32 actorId, Sint32 element)
{
	fdDjinnJoinParty(actorId, element, 0);
	syGameFlagClear(FLAG_DJINN_FIRST + element * DJINN_PER_ELEMENT);
}

void dbTestReceiveFlint(void)
{
	dbTestGiveDjinni(ACTOR_FLINT, ELEMENT_VENUS);
}

void dbTestReceiveFizz(void)
{
	dbTestGiveDjinni(ACTOR_FIZZ, ELEMENT_MERCURY);
}

void dbTestReceiveForge(void)
{
	dbTestGiveDjinni(ACTOR_FORGE, ELEMENT_MARS);
}

void dbTestReceiveGust(void)
{
	dbTestGiveDjinni(ACTOR_GUST, ELEMENT_JUPITER);
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

// Nothing to set up.
Sint32 scInit(void)
{
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(Test0),
	SCENE_EXITS(Test1),
	SCENE_EXITS(Test2),
	SCENE_EXITS(Test3),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_TALK, ACTOR_FLINT, CONDITION_ALWAYS, SCRIPT_REF(dbTestReceiveFlint)},
	{CUE_TALK, ACTOR_FIZZ, CONDITION_ALWAYS, SCRIPT_REF(dbTestReceiveFizz)},
	{CUE_TALK, ACTOR_FORGE, CONDITION_ALWAYS, SCRIPT_REF(dbTestReceiveForge)},
	{CUE_TALK, ACTOR_GUST, CONDITION_ALWAYS, SCRIPT_REF(dbTestReceiveGust)},
	{CUE_TABLE_END},
};

// Unused: four halfwords and an end of -1s after the room's tables; nothing reads them.
Sint16 TestScenesUnusedRow[] ALIGNED(4) = {
	512,
	512,
	1024,
	864,
	-1,
	-1,
	-1,
	-1,
};
