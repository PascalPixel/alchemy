#include "game_state.h"
#include "unit/field/suhara_mura/mura.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/objtable.h"
#include "unit/battle/effect/evbasic.h"
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
#include "field_event.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"

u8 *SceneData_GetScriptTable(void)
{
	return (u8 *)SuharaMura_Scripts;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return (u8 *)SuharaMura_Messages;
}

s32 SceneData_SelectActorTableByFlag96f(void)
{
	if (Engine_GameFlagIsSet(0x96f) != 0)
	{
		return (s32)SuharaMura_ActorsFlag96f;
	}
	return (s32)SuharaMura_Actors;
}

static void EV09_TALK(s32 obj)
{
	s32 cue = (s32)MsgSuharaTryingGetLalivero;
	Engine_EventSetMessage(cue);
	Engine_EventOpenMessage(obj, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(10);
		Engine_EventSetMessage(cue + 1);
	}
	else
	{
		Engine_EventSetMessage(cue + 2);
	}
	Engine_EventShowMessage(obj, 0);
}

static void EV13_TALK(s32 obj)
{
	s32 cue = (s32)MsgSuharaBroughtSuhallaSandstorm;
	Engine_EventSetMessage(cue);
	Engine_EventOpenMessage(obj, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(10);
		Engine_EventSetMessage(cue + 1);
	}
	else
	{
		Engine_EventSetMessage(cue + 2);
	}
	Engine_EventShowMessage(obj, 0);
}

static void EV16_PSYNERGY(void)
{
	s32 width = 4;
	s32 height = 9;

	Engine_MapCopyCellAttributes(25, 9, 1, 1, width, height);
	Engine_GameFlagSet(0x201);
}

static void SuharaMura_AnimateCells0(void)
{
	struct EventWork *work = gEventWork;
	s32				  no = work->touched_trigger;
	u16				  x = SuharaMura_CellAnimationOrigins[no][0];
	u16				  y = SuharaMura_CellAnimationOrigins[no][1];

	Event_PlayCue(158);
	Map_AnimateCells(SuharaMura_CellSteps0, x, y);
	Actor_WalkBy(0, 0, -16);
	gEventWork->transition_frames = 16;
	Engine_EventRequestExit(no);
}

static void EV02_TOUCH(void)
{
	struct EventWork *work = gEventWork;
	s32				  no = work->touched_trigger;
	u16				  x = SuharaMura_CellAnimationOrigins[no][0];
	u16				  y = SuharaMura_CellAnimationOrigins[no][1];

	Event_PlayCue(158);
	Map_AnimateCells(SuharaMura_CellSteps1, x, y);
	Actor_WalkBy(0, 0, -16);
	gEventWork->transition_frames = 16;
	Engine_EventRequestExit(no);
}

s32 SceneData_SelectExtraTableByFlag96f(void)
{
	if (Engine_GameFlagIsSet(0x96F) != 0)
	{
		return (s32)SuharaMura_ExtrasFlag96f;
	}
	return (s32)SuharaMura_Extras;
}

s32 SceneState_InitEntryWorkspaceAndFlag96f(void)
{
	struct EventWork *work;

	if (gGameState.entrance == 90)
	{
		Engine_GameFlagSet(0x96f);
	}

	work = gEventWork;
	work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	work->transition_frames = 24;

	if (Engine_GameFlagIsSet(0x201) != 0)
	{
		EV16_PSYNERGY();
		Engine_ActorSetAnimation(16, 4);
	}
	return 0;
}

struct SceneEntrance SuharaMura_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 136, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 256, 0, 120, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 88, 0, 296, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 216, 0, 280, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 296, 0, 296, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 120, 0, 272, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 40, 0, 176, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 SuharaMura_Messages[] = {
	SCENE_EXITS(167),
	SCENE_EXIT(1, 168, 1),
	SCENE_EXIT(2, 168, 2),
	SCENE_EXIT(3, 168, 3),
	SCENE_EXIT(4, 168, 4),
	SCENE_EXIT(5, 168, 5),
	SCENE_EXIT(10, 2, 27),
	SCENE_EXIT(11, 2, 56),
	SCENE_EXIT(12, 2, 57),
	SCENE_EXITS_END,
};

struct ScenePlacement SuharaMura_Actors[] = {
	{ 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(104), 0, PIXELS(184), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(184), 0, PIXELS(304), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x8f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(296), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(248), 0, PIXELS(168), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(320), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(112), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x66, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(168), 0, PIXELS(208), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement SuharaMura_ActorsFlag96f[] = {
	{ 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(104), 0, PIXELS(184), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(184), 0, PIXELS(320), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x8f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(296), 0, PIXELS(328), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(336), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(248), 0, PIXELS(168), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(320), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(72), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x66, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(168), 0, PIXELS(208), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

// セルアニメの原点 (x, y)
s16 SuharaMura_CellAnimationOrigins[][2] = {
	{ 0, 0 }, { 8, 7 }, { 15, 5 }, { 5, 16 }, { 13, 15 }, { 18, 16 },
};

u16 SuharaMura_CellSteps0[] = {
	MAP_CELL_STEP(25, 0, 1, 2, 8), MAP_CELL_STEP(26, 0, 1, 2, 1), MAP_CELL_STEPS_END,
};

u16 SuharaMura_CellSteps1[] = {
	MAP_CELL_STEP(26, 2, 2, 2, 8), MAP_CELL_STEP(28, 2, 2, 2, 1), MAP_CELL_STEPS_END,
};

struct SceneEvent SuharaMura_Extras[] = {
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaMura_AnimateCells0) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaMura_AnimateCells0) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaMura_AnimateCells0) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaMura_AnimateCells0) },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraThisIsSuhallaDontTryCrossing) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraWeGondowansWillNeverTrustAnyone) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraItHasBeenSaidForAges) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraYouCantGetToSouthernGondowan) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraTheWorkersSayTherereHugeFestivals) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraTheSandstormsSuddenlyIncreasedIHope) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraTheSuhallaHasLongBeenKnown) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraThePeopleOfLaliveroHaveBeen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraTheRulerOfTolbiTookA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraIKnowTheDesertSpiritsWill) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraSouthernGondowanIsGuardedByThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraIveHeardThatTolbisColossoIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraNoneOfTheGrownUpsIn) },
	{ SEARCH(SEARCH_CHEST), 100, 0xfad, FIND_ITEM(0xc2) },
	{ SEARCH(SEARCH_JAR), 101, 0xfae, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_JAR), 102, 0xfaf, FIND_COINS(5) },
	{ PSYNERGY_ON_ACTOR(0xc), 16, 0x201, EVENT_SCRIPT(EV16_PSYNERGY) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent SuharaMura_ExtrasFlag96f[] = {
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaMura_AnimateCells0) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaMura_AnimateCells0) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaMura_AnimateCells0) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaMura_AnimateCells0) },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraWelcomeToSuhallaIBetThe) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraTheDesertIsEnvelopedInSandstorms) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraSoldiersFromTolbiWereCaughtIn) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraWeWereBroughtHereByThe) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraThisSandstormIsTheFaultOf) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraWhyAreTheSandstormsAttackingHumans) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraThoseSoldiersKeptMumblingAboutMonsters) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraIBetTheseWarriorsWereBrought) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraIWonderWhyThereAreSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraTheSoldiersFromTolbiWereCollapsing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraAfterYouGetSwallowedByA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraLatelyAllTheSandstormsHaveBeen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraIfYouFightTheStormsYoull) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraGrownUpsSayTheSandstormsAttack) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgSuharaMuraTheySayTheSoldiersWereRavaged) },
	{ SEARCH(SEARCH_CHEST), 100, 0xfad, FIND_ITEM(0xc2) },
	{ SEARCH(SEARCH_JAR), 101, 0xfae, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_JAR), 102, 0xfaf, FIND_COINS(5) },
	{ PSYNERGY_ON_ACTOR(0xc), 16, 0x201, EVENT_SCRIPT(EV16_PSYNERGY) },
	{ SCENE_EVENTS_END },
};
