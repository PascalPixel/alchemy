#include "game_state.h"
#include "unit/field/suhara_gate/gate.h"
#include "unit/battle/effect/object.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "gate.h"
#include "field_scene.h"
#include "types.h"
#include "field_event.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"

static void Scene_SetActor13Value1A(void)
{
	BattleFx_SetPhaseRequest(0xD, 0x1A);
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_SuharaGate2)
	{
		return gSuharaGateEntrances2;
	}
	if (scene == (s32)&SceneId_SuharaGate3)
	{
		return gSuharaGateEntrances3;
	}
	return gSuharaGateEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetSceneTable(void)
{
	return (u8 *)SuharaGate_SceneTable;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_SuharaGate2)
	{
		return gSuharaGatePlacements2;
	}
	if (scene == (s32)&SceneId_SuharaGate1)
	{
		if (Engine_GameFlagIsSet(0x96f) != 0)
		{
			return gSuharaGatePlacements1Flagged;
		}
		return gSuharaGatePlacements1;
	}
	return gSuharaGatePlacementsOther;
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_SuharaGate2)
	{
		return gSuharaGateEvents2;
	}
	if (scene == (s32)&SceneId_SuharaGate3)
	{
		return gSuharaGateEvents3;
	}
	return gSuharaGateEventsOther;
}

s32 SuharaGate_EnterScene(void)
{
	s32 scene;

	if (Engine_GameFlagIsSet(0x89f))
	{
		gGameState.saved_scene = (s32)&SceneId_RunpaSuhara;
		gGameState.saved_entrance = 10;
	}
	scene = gGameState.scene;
	if (scene == (s32)&SceneId_SuharaGate1)
	{
		if (Engine_GameFlagIsSet(0x897))
			Engine_ActorSetPosition(10, 0, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
		else if (Engine_GameFlagIsSet(0x96f))
			Map_CopyCellAttributes(0, 0, 2, 1, 29, 18);
		if (Engine_GameFlagIsSet(0x96f))
		{
			Map_CopyCellAttributes(0, 0, 2, 1, 8, 25);
			Map_CopyCellAttributes(0, 0, 2, 1, 9, 26);
		}
#endif
		if (gGameState.entrance == 3)
		{
			if (Engine_GameFlagIsSet(0x8fb))
			{
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
				gGameState.retreat_scene = (s32)&SceneId_SuharaGate1;
#else
				gGameState.retreat_scene = scene;
#endif
				gGameState.retreat_entrance = 1;
			}
			if (Engine_GameFlagIsSet(0x8fc))
			{
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
				gGameState.retreat_scene = (s32)&SceneId_SuharaGate1;
#else
				gGameState.retreat_scene = scene;
#endif
				gGameState.retreat_entrance = 5;
			}
			Engine_GameFlagClear(0x12f);
		}
		if (gGameState.entrance == 1)
		{
			Engine_GameFlagSet(0x8fb);
			if (!Engine_GameFlagIsSet(0x96f))
				Map_CopyCellAttributes(6, 0, 2, 1, 8, 27);
		}
		if (gGameState.entrance == 5)
			Engine_GameFlagSet(0x8fc);
	}
	else if (scene == (s32)&SceneId_SuharaGate2)
	{
		Engine_ActorSetAnimation(8, 4);
		Engine_ActorSetAnimation(9, 4);
		Engine_ActorSetAnimation(10, 3);
		Engine_ActorSetAnimation(11, 4);
		Engine_ActorSetAnimation(12, 3);
		Object_GetById(15)->scale_y = 0x19999;
		Map_CopyCellAttributes(108, 38, 1, 1, 102, 56);
	}
	return 0;
}

static void Scene_RunScene3c3SequenceC(void)
{
	s32 arg;
	s32 v5;

	arg = gEventWork->touched_trigger;
	Actor_Get(0)->motion_flags = 0;
	v5 = 2;
	Event_PlayCue(158);
	Map_CopyCellsTo(66, 36, 71, 8, v5, v5);
	Engine_TaskWait(4);
	Map_CopyCellsTo(68, 36, 71, 8, v5, v5);
	Engine_TaskWait(4);
	Actor_CenterAndWalk(0, 3, -16);
	Engine_EventRequestExit(arg);
}

static void Dialogue_ShowMessages8fbAnd8fc(void)
{
	s16 token = gEventWork->touched_trigger;

	Engine_AudioPlayCue(123);
	Engine_GameFlagClear(0x8FB);
	Engine_GameFlagClear(0x8FC);
	Engine_EventRequestExit(token);
}

static void Scene_RunPrimarySequence(void)
{
	Engine_EventBegin();
	Actor_SetSpeed(8, 65536, 32768);
	Actor_SetSpeed(9, 65536, 32768);
	Actor_WalkTo(8, 136, 384);
	Actor_WalkToAndWait(9, 152, 384);
	Actor_FaceDirection(8, 16384, 0);
	Actor_FaceDirection(9, 16384, 0);
	Engine_ActorSetAnimation(8, 1);
	Map_CopyCellAttributes(6, 27, 1, 1, 7, 27);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	Map_CopyCellAttributes(11, 26, 1, 1, 7, 26);
	Map_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
	Map_CopyCellAttributes(9, 26, 2, 1, 7, 26);
#endif
	Engine_EventEnd();
}

static void Scene_RunScene3c3SequenceA(void)
{
	struct FieldActor *record;
	s32				   v5;

	Engine_EventBegin();
	Actor_SetSpeed(0, 0x19999, 0xcccc);
	Actor_WalkToAndWait(0, 120, 0x1b6);
	Actor_FaceDirection(0, 0xc000, 0);
	record = Actor_Get(0);
	if (record != 0)
	{
		Actor_SetPosition(11, record->x.fixed, record->z.fixed);
	}
	Engine_TaskWait(1);
	Actor_SetSpeed(11, 0x19999, 0xcccc);
	Actor_WalkToAndWait(11, 108, 0x1af);
	Actor_FaceDirection(11, 0xd000, 10);
	Actor_ShowEmote(11, 0x100, 20);
	Actor_FaceDirection(11, 0xd000, 20);
	Actor_FaceDirection(11, 0, 40);
	Actor_FaceDirection(11, 0xd000, 40);
	Actor_FaceDirection(11, 0, 20);
	Engine_ActorStartRepeatedMotion(11, 2);
	Engine_EventSetMessage((s32)MsgSuharaMeaning);
	Event_ShowMessageAndWait(11, 0, 40);
	Actor_ShowEmote(8, 0x100, 0);
	Engine_ActorRunRepeatedMotion(8, 2);
	Event_ShowMessageAndWait(8, 0, 10);
	Actor_WalkToAndWait(11, 132, 0x1a4);
	Actor_FaceDirection(11, 0xd000, 0);
	Actor_FaceDirection(0, 0xe000, 0);
	Actor_WalkToAndWait(11, 138, 0x1a0);
	Actor_FaceDirection(11, 0xb000, 10);
	Engine_ActorStartRepeatedMotion(11, 2);
	Event_ShowMessageAndWait(11, 0, 40);
	Engine_ActorRunRepeatedMotion(8, 2);
	Event_ShowMessageAndWait(8, 0, 40);
	Actor_ShowEmote(9, 0x100, 20);
	Engine_ActorRunRepeatedMotion(9, 2);
	Event_ShowMessageAndWait(9, 0, 20);
	Actor_FaceDirection(0, 0xc000, 0);
	Actor_WalkToAndWait(11, 144, 0x1a4);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(9, 2);
	Event_ShowMessageAndWait(9, 0, 20);
	Actor_SetAttachedEffect(9, 0x102);
	Engine_ActorStartRepeatedMotion(9, 3);
	Event_ShowMessageAndWait(9, 0, 20);
	Actor_FaceDirection(11, 0x5000, 20);
	Event_AskYesNo(11, 0);
	if (GameFlag_IsSet(0x9b0) != 0)
	{
		Actor_FaceDirection(11, 0xd000, 40);
		Actor_SetAttachedEffect(11, 0x102);
		Engine_EventWait(40);
		Event_ShowMessageAndWait(11, 0, 10);
	}
	else
	{
		gEventWork->message += 1;
	}
	Actor_FaceDirection(11, 0x5000, 10);
	Event_ShowMessageAndWait(11, 0, 40);
	Actor_ShowEmote(11, 0x100, 40);
	Actor_FaceDirection(11, 0xb000, 10);
	Event_ShowMessageAndWait(11, 0, 10);
	Actor_WalkToAndWait(11, 138, 0x1a0);
	Actor_FaceDirection(11, 0xb000, 20);
	Engine_ActorStartRepeatedMotion(8, 2);
	Event_ShowMessageAndWait(8, 0, 10);
	Actor_SetAttachedEffect(11, 0x102);
	Engine_ActorRunRepeatedMotion(11, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(11, 0, 20);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(0, 0xe000, 10);
	Engine_ActorRunRepeatedMotion(9, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(9, 0, 10);
	Engine_ActorRunRepeatedMotion(8, 2);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorRunRepeatedMotion(11, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(11, 0, 20);
	Actor_FaceDirection(11, 0x5000, 10);
	Event_ShowMessageAndWait(11, 0, 10);
	Engine_ActorSetAnimation(11, 2);
	record = Actor_Get(0);
	if (record != 0)
	{
		Actor_SetDestination(11, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(11);
	Actor_SetPosition(11, 0, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	Map_CopyCellAttributes(6, 27, 1, 1, 7, 27);
	Map_CopyCellAttributes(11, 26, 1, 1, 7, 26);
	Map_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
	v5 = 7;
	Map_CopyCellAttributes(6, 27, 1, 1, v5, 27);
	Map_CopyCellAttributes(9, 26, 2, 1, v5, 26);
#endif
	GameFlag_Set(0x89f);
	Engine_EventEnd();
}

static void Scene_RunActorNinePromptDialogue(void)
{
	u8 *work;

	Engine_EventBegin();

	if (Engine_GameFlagIsSet(0x89f) != 0)
	{
		Engine_EventSetMessage((s32)MsgSuharaPityColossoVictor);
		goto close;
	}

	Engine_EventSetMessage((s32)MsgSuharaWantGoBabi);
	{
		s32 mode = 0;
		s32 no = 9;

		Engine_EventOpenMessage(no, mode);
	}

	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		goto skip;
	}

	Engine_EventShowMessage(9, 0);
	Engine_ActorSetAnimationAndWait(9, 4);

close:
	Engine_EventShowMessage(9, 0);
	goto done;

skip:
	work = (u8 *)gEventWork;
	*(u16 *)(work + 472) += 2;
	Engine_EventShowMessage(9, 0);

done:
	Engine_EventEnd();
}

static void Scene_RunActorTenRepeatedMotion(void)
{
	unsigned int beat;

	Engine_EventBegin();

	Engine_EventSetMessage((s32)MsgFieldVenusLighthouseWasAttackedBy);
	Event_ShowMessageAndWait(10, 0, 10);

	beat = 0;
	do
	{
		Actor_SetChildValue(10, 0);
		Engine_ActorSetSpriteFlags(Actor_Get(10), 1);
		Engine_TaskWait(4);

		Actor_SetChildValue(10, 15);
		Engine_ActorSetSpriteFlags(Actor_Get(10), 0);
		beat++;
		Engine_TaskWait(4);
	} while (beat <= 5);

	beat = 0;
	do
	{
		Actor_SetChildValue(10, 0);
		Engine_ActorSetSpriteFlags(Actor_Get(10), 1);
		Engine_TaskWait(2);

		Actor_SetChildValue(10, 15);
		Engine_ActorSetSpriteFlags(Actor_Get(10), 0);
		beat++;
		Engine_TaskWait(2);
	} while (beat <= 11);

	Actor_SetPosition(10, 0, 0);

	GameFlag_Set(0x897);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	Map_CopyCellAttributes(29, 19, 2, 1, 29, 18);
#endif
	Engine_EventEnd();
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
s32 SuharaGate_PatrolStartup[] = {
	SC_SET(SW_COLLISION, 0),
};
#endif

s32 gSuharaGatePlacements1Flagged1[] = {
	SC_ARG(5),
	SC_END,
};

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
s32 SuharaGate_FirstGuardStartup[] = {
	SC_SET(SW_COLLISION, 0),
};
#endif

s32 gSuharaGatePlacements1Flagged0[] = {
	SC_SET(SW_SCALE_X, -0x10000),
	SC_ARG(5),
	SC_END,
};

struct SceneEntrance gSuharaGateEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 152, 0, 504, FACING_SOUTH, 0, 32, 32, 272, 544, 0 },
	{ 1, CONDITION_ALWAYS, 152, 0, 520, FACING_NORTH, 0, 32, 32, 272, 544, 0 },
	{ 2, CONDITION_ALWAYS, 248, 0, 280, FACING_WEST, 0, 32, 32, 272, 544, 0 },
	{ 3, CONDITION_ALWAYS, 136, 0, 304, FACING_SOUTH, 0, 32, 32, 272, 544, 0 },
	{ 4, CONDITION_ALWAYS, 360, 0, 296, FACING_EAST, 0, 336, 32, 736, 496, 0 },
	{ 5, CONDITION_ALWAYS, 664, 0, 424, FACING_WEST, 0, 336, 32, 736, 496, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gSuharaGateEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 40, 0, 304, FACING_EAST, 0, 16, 32, 416, 480, 0 },
	{ 2, CONDITION_ALWAYS, 392, 0, 368, FACING_WEST, 0, 16, 32, 416, 480, 0 },
	{ 3, CONDITION_ALWAYS, 40, 0, 912, FACING_EAST, 0, 16, 608, 720, 1008, 0 },
	{ 4, CONDITION_ALWAYS, 680, 0, 904, FACING_WEST, 0, 16, 608, 720, 1008, 0 },
	{ 5, CONDITION_ALWAYS, 616, 0, 920, FACING_SOUTH, 0, 16, 608, 720, 1008, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gSuharaGateEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 72, 0, 264, FACING_NORTH, 0, 24, 24, 280, 256, 0 },
	{ 2, CONDITION_ALWAYS, 184, 0, 168, FACING_SOUTH, 0, 24, 24, 280, 256, 0 },
	{ 3, CONDITION_ALWAYS, 280, 0, 344, FACING_SOUTH, 0, 208, 288, 704, 640, 0 },
	{ 4, CONDITION_ALWAYS, 664, 0, 616, FACING_NORTH, 0, 208, 288, 704, 640, 0 },
	{ SCENE_TABLE_END },
};

u32 SuharaGate_SceneTable[] = {
	SCENE_EXITS(169),
	SCENE_EXIT(1, 2, 29),
	SCENE_EXIT(2, 170, 1),
	SCENE_EXIT(3, 105, 10),
	SCENE_EXIT(4, 170, 4),
	SCENE_EXIT(5, 2, 42),
	SCENE_EXIT(0, 170, 0),
	SCENE_EXIT(1, 169, 2),
	SCENE_EXIT(2, 170, 3),
	SCENE_EXIT(3, 170, 2),
	SCENE_EXIT(4, 169, 4),
	SCENE_EXIT(5, 171, 1),
	SCENE_EXIT(0, 171, 0),
	SCENE_EXIT(1, 170, 5),
	SCENE_EXIT(2, 171, 3),
	SCENE_EXIT(3, 171, 2),
	SCENE_EXIT(4, 2, 33),
	SCENE_EXITS_END,
};

struct ScenePlacement gSuharaGatePlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSuharaGatePlacements2[] = {
	{ 0xee, CONDITION_ALWAYS, 7,
		PIXELS(264), 0, PIXELS(400), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, 7,
		PIXELS(280), 0, PIXELS(368), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, 7,
		PIXELS(472), 0, PIXELS(912), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, 7,
		PIXELS(536), 0, PIXELS(912), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, 7,
		PIXELS(552), 0, PIXELS(880), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x5b, 0x4a, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(888), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSuharaGatePlacements1[] = {
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSuharaGatePlacements1Flagged[] = {
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
	{ 0x98, CONDITION_ALWAYS, (s32)gSuharaGatePlacements1Flagged0,
		PIXELS(136), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x98, CONDITION_ALWAYS, (s32)gSuharaGatePlacements1Flagged1,
		PIXELS(168), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x98, CONDITION_ALWAYS, (s32)gSuharaGatePlacements1Flagged1,
		PIXELS(472), 0, PIXELS(304), FACING_SOUTH, TALK_FACE_PARTY, 0 },
#else
	{ 0x98, CONDITION_ALWAYS, (s32)SuharaGate_FirstGuardStartup,
		PIXELS(140), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x98, CONDITION_ALWAYS, (s32)SuharaGate_PatrolStartup,
		PIXELS(164), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x98, CONDITION_ALWAYS, (s32)SuharaGate_PatrolStartup,
		PIXELS(476), 0, PIXELS(300), FACING_SOUTH, TALK_FACE_PARTY, 0 },
#endif
	{ 0x39, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gSuharaGateEventsOther[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Dialogue_ShowMessages8fbAnd8fc) },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Dialogue_ShowMessages8fbAnd8fc) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunScene3c3SequenceC) },
	{ EVENT_TOUCH, 10, 0x96f, EVENT_SCRIPT(Scene_RunPrimarySequence) },
	{ EVENT_TOUCH, 10, 0x89f, EVENT_SCRIPT(Scene_RunScene3c3SequenceA) },
	{ EVENT_TALK, 8, 0x89f, MSGNO(MsgSuharaTravelPapersRequired) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuharaSendHelpFromTower) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunActorNinePromptDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x89f, MSGNO(MsgSuharaObeyBabiOrders) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSuharaAshamedBeforeYodemu) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x89f, MSGNO(MsgSuharaSeeKorosseoNextYear) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSuharaSurvivedAttack) },
	{ EVENT_TALK, 10, 0x897, EVENT_SCRIPT(Scene_RunActorTenRepeatedMotion) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40a, 0x897, EVENT_SCRIPT(Scene_RunActorTenRepeatedMotion) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gSuharaGateEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_SetActor13Value1A) },
	{ 0xc413, 100, 0xfb4, FIND_ITEM(0xc3) },
	{ 0x413, 100, 0xfb4, FIND_ITEM(0xc3) },
	{ 0x8413, 100, 0xfb4, FIND_ITEM(0xc3) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gSuharaGateEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ 0x31, 2, CONDITION_ALWAYS, 2 },
	{ 0x21, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ SEARCH(SEARCH_UNNAMED), 100, 0x350, FIND_PSYNERGY_STONE },
	{ SCENE_EVENTS_END },
};
