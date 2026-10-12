#include "game_state.h"
#include "field_event.h"
#include "unit/import.h"
#include "unit/field/suhara_heya/heya.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field2.h"
#include "unit/field/common/script/field3.h"
#include "unit/field/common/script/field5.h"
#include "unit/field/common/script/field6.h"
#include "unit/field/common/script/field7.h"
#include "unit/field/common/script/field8.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "suhara.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"

u8 *SuharaHeya_GetEntrances(void)
{
	return gSuharaHeyaEntrances;
}

u8 *SuharaHeya_GetRegions(void)
{
	return gSuharaHeyaRegions;
}

u8 *SuharaHeya_GetExits(void)
{
	return gSuharaHeyaExits;
}

s32 SuharaHeya_SelectPlacements(void)
{
	if (Engine_GameFlagIsSet(0x96f) != 0)
	{
		return (s32)gSuharaHeyaPlacements96f;
	}
	return (s32)gSuharaHeyaPlacements;
}

static void EV12_TALK(s32 no)
{
	u16 facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
	if (facing == 0xc000)
	{
		Engine_ShopOpen(31, no);
	}
	else if (Engine_GameFlagIsSet(0x96f))
	{
		s32 msg = (s32)MsgSuharaArentSurprisedFind;
		Engine_EventSetMessage(msg);
		Engine_EventOpenMessage(no, 0);
		if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0)
		{
			Engine_EventWait(10);
			Engine_EventSetMessage(msg + 1);
		}
		else
		{
			Engine_EventSetMessage(msg + 2);
		}
		Engine_EventShowMessage(no, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgSuharaOursOnlyStore);
		Engine_EventShowMessage(no, 0);
	}
}

static void EV13_TALK(s32 no)
{
	u16 facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
	if (facing == 0xc000)
	{
		Engine_InnCheckIn(10, no);
	}
	else if (Engine_GameFlagIsSet(0x96f))
	{
		Engine_EventSetMessage((s32)MsgSuharaBusyEverSince);
		Engine_EventShowMessage(no, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgSuharaSighNothinMaybe);
		Engine_EventShowMessage(no, 0);
	}
}

static void EV18_TALK(s32 no)
{
	u16 facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
	if (facing == 0xc000)
	{
		Engine_ShopConfirmAct(no);
	}
	else if (Engine_GameFlagIsSet(0x96f))
	{
		Engine_EventSetMessage((s32)MsgSuharaWonderWhySandstorms);
		Engine_EventShowMessage(no, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgSuharaGetSickThinkingAboutLalivero);
		Engine_EventShowMessage(no, 0);
	}
}

static void EV17_TALK(s32 obj)
{
	s32 msg = (s32)MsgSuharaSupposeFolkBlown;
	Engine_EventSetMessage(msg);
	Engine_EventOpenMessage(obj, 0);
	if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0)
	{
		Engine_EventWait(10);
		Engine_EventSetMessage(msg + 1);
	}
	else
	{
		Engine_EventSetMessage(msg + 2);
	}
	Engine_EventShowMessage(obj, 0);
}

static void SuharaHeya_PlayCueAndRequestTouchedExit(void)
{
	struct EventWork *state = gEventWork;
	s16				 *cnt = (s16 *)&state->touched_trigger;

	Engine_EventRequestExit(*cnt);
	Engine_AudioPlayCue(0x7B);
}

static void EV50_TOUCH(void)
{
	struct FieldActor *record;
	GameFlag_Set(2480);
	if (GameFlag_IsSet(2442) == 0)
	{
		Event_PlayCue(30);
		Event_Begin();
		Camera_MoveTo(24117248, -1, 6815744, 1);
		Actor_WalkToAndWait(0, 368, 160);
		Actor_FaceDirection(0, 49152, 0);
		FieldMotion_LaunchFromFocusedObject(19, 0, -16, 49152);
		Actor_WaitForMove(19);
		Camera_WaitForMove();
		Event_SetMessage((s32)MsgSuharaIodem);
		Event_Wait(10);
		Actor_RunRepeatedMotion(20, 2);
		Event_Wait(20);
		Event_ShowMessage(20, 0);
		Event_Wait(10);
		Actor_RunRepeatedMotion(19, 2);
		Event_Wait(20);
		Actor_SetSpeed(19, 78643, 39321);
		Actor_WalkByAndWait(19, 0, -16);
		Actor_FaceDirection(19, 0, 0);
		Event_Wait(30);
		Actor_FaceDirection(19, 57344, 0);
		Event_Wait(30);
		Actor_FaceDirection(19, 0, 0);
		Event_Wait(30);
		Actor_ShowEmote(19, 256, 40);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_RunRepeatedMotion(21, 2);
		Event_Wait(20);
		Event_ShowMessage(21, 0);
		Event_Wait(10);
		Actor_SetSpeed(19, 78643, 39321);
		Actor_WalkByAndWait(19, 0, -24);
		Actor_WalkByAndWait(19, 48, 0);
		Actor_FaceDirection(19, 49152, 0);
		Event_Wait(30);
		Event_ShowMessage(19, 0);
		Event_Wait(20);
		Actor_ShowEmote(19, 256, 40);
		Event_ShowMessage(19, 0);
		Event_Wait(20);
		Actor_ShowEmote(20, 258, 40);
		Event_ShowMessage(20, 0);
		Event_Wait(10);
		Actor_FaceDirection(19, 8192, 0);
		Event_Wait(20);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_RunRepeatedMotion(21, 2);
		Event_Wait(20);
		Actor_SetAnimationAndWait(21, 4);
		Event_Wait(20);
		Event_ShowMessage(21, 0);
		Event_Wait(10);
		Actor_FaceDirection(19, 49152, 0);
		Event_Wait(30);
		Actor_ShowEmote(19, 263, 40);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_StartRepeatedMotion(20, 2);
		Actor_RunRepeatedMotion(21, 2);
		Event_Wait(20);
		Actor_SetSpeed(0, 78643, 39321);
		Actor_WalkToAndWait(0, 368, 104);
		Actor_WalkByAndWait(0, 16, 0);
		Actor_FaceDirection(0, 0, 0);
		Event_Wait(20);
		Event_Wait(10);
		Actor_SetAnimationAndWait(19, 4);
		Event_Wait(20);
		Event_ShowMessage(19, 0);
		Event_Wait(20);
		Actor_ShowEmote(19, 258, 50);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_RunRepeatedMotion(20, 2);
		Event_Wait(20);
		Event_ShowMessage(20, 0);
		Event_Wait(10);
		Actor_FaceActor(0, 20, 0);
		Actor_FaceDirection(19, 8192, 0);
		Event_Wait(20);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_RunRepeatedMotion(21, 2);
		Event_Wait(20);
		Event_ShowMessage(21, 0);
		Event_Wait(10);
		Actor_FaceActor(0, 21, 0);
		Actor_FaceDirection(19, 49152, 0);
		Event_Wait(40);
		Actor_SetAnimationAndWait(19, 3);
		Event_Wait(30);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_RunRepeatedMotion(21, 2);
		Event_Wait(20);
		Actor_SetAnimationAndWait(21, 4);
		Event_Wait(20);
		Event_ShowMessage(21, 0);
		Event_Wait(10);
		Actor_FaceEachOther(19, 0, 30);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_RunRepeatedMotion(20, 2);
		Event_Wait(20);
		Event_ShowMessage(20, 0);
		Event_Wait(10);
		Actor_FaceActor(0, 20, 0);
		Actor_FaceDirection(19, 8192, 0);
		Event_Wait(40);
		Event_Wait(10);
		Actor_RunRepeatedMotion(21, 2);
		Event_Wait(20);
		Event_ShowMessage(21, 0);
		Event_Wait(10);
		Actor_FaceActor(0, 21, 0);
		Actor_FaceDirection(19, 49152, 0);
		Event_Wait(40);
		Actor_SetAnimation(0, 3);
		Actor_SetAnimationAndWait(19, 3);
		Event_Wait(30);
		Event_Wait(10);
		Actor_SetAnimationAndWait(20, 4);
		Event_Wait(20);
		Event_ShowMessage(20, 0);
		Event_Wait(10);
		Actor_ShowEmote(0, 256, 0);
		Actor_ShowEmote(19, 256, 40);
		Event_Wait(10);
		Actor_FaceActor(0, 20, 0);
		Actor_FaceDirection(19, 8192, 0);
		Event_Wait(40);
		Event_Wait(10);
		Actor_RunRepeatedMotion(21, 2);
		Event_Wait(20);
		Event_ShowMessage(21, 0);
		Event_Wait(10);
		Actor_ShowEmote(0, 256, 50);
		Actor_FaceActor(0, 21, 0);
		Event_Wait(30);
		Actor_RunRepeatedMotion(21, 2);
		Event_Wait(30);
		Event_Wait(10);
		Actor_FaceDirection(19, 49152, 0);
		Event_Wait(30);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_RunRepeatedMotion(21, 2);
		Event_Wait(20);
		Actor_SetAnimationAndWait(21, 3);
		Event_Wait(30);
		Event_Wait(10);
		Actor_ShowEmote(19, 258, 40);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_SetAnimationAndWait(20, 4);
		Event_Wait(20);
		Event_ShowMessage(20, 0);
		Event_Wait(10);
		Actor_ShowEmote(19, 257, 50);
		Event_Wait(10);
		Actor_FaceActor(0, 20, 0);
		Actor_FaceDirection(19, 8192, 0);
		Event_Wait(20);
		Event_Wait(10);
		Actor_RunRepeatedMotion(20, 2);
		Event_Wait(20);
		Event_ShowMessage(20, 0);
		Event_Wait(10);
		Actor_ShowEmote(19, 256, 40);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_RunRepeatedMotion(21, 2);
		Event_Wait(20);
		Event_ShowMessage(21, 0);
		Actor_RunRepeatedMotion(21, 2);
		Event_Wait(20);
		Event_Wait(10);
		Actor_FaceEachOther(19, 0, 0);
		Event_Wait(30);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_SetAnimationAndWait(20, 4);
		Event_Wait(20);
		Event_ShowMessage(20, 0);
		Event_Wait(10);
		Actor_ShowEmote(0, 258, 0);
		Actor_ShowEmote(19, 258, 80);
		Actor_ShowEmote(21, 258, 50);
		Event_ShowMessage(21, 0);
		Event_Wait(10);
		Actor_RunRepeatedMotion(21, 2);
		Event_Wait(20);
		Event_ShowMessage(21, 0);
		Actor_RunRepeatedMotion(21, 3);
		Event_Wait(20);
		Event_Wait(10);
		Actor_FaceDirection(19, 49152, 0);
		Event_Wait(30);
		Actor_RunRepeatedMotion(19, 2);
		Event_Wait(10);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_RunRepeatedMotion(20, 2);
		Event_Wait(20);
		Event_ShowMessage(20, 0);
		Event_Wait(10);
		Actor_FaceActor(0, 20, 0);
		Actor_FaceDirection(19, 8192, 0);
		Event_Wait(30);
		Actor_SetAnimationAndWait(19, 3);
		Event_Wait(30);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_StartRepeatedMotion(20, 2);
		Actor_RunRepeatedMotion(21, 2);
		Event_Wait(30);
		Event_Wait(10);
		Actor_FaceEachOther(19, 0, 20);
		Actor_WalkByAndWait(19, -12, 0);
		Event_Wait(20);
		Event_OpenMessage(19, 0);
		if (Event_ChooseYesNo(0, 0) == 0)
		{
			Event_Wait(20);
			Event_ShowMessage(19, 0);
			gEventWork->message++;
		}
		else
		{
			Event_Wait(10);
			gEventWork->message++;
			Event_ShowMessage(19, 0);
		}
		Event_Wait(10);
		Actor_RunRepeatedMotion(19, 2);
		Event_Wait(20);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_ShowEmote(19, 258, 50);
		Event_ShowMessage(19, 0);
		Event_Wait(10);
		Actor_SetAnimationAndWait(0, 3);
		Event_Wait(20);
		Actor_SetAnimationAndWait(19, 3);
		Event_Wait(30);
		Event_PlayCue(30);
		Actor_SetSpeed(19, 78643, 39321);
		Actor_SetAnimation(19, 2);
		record = Actor_Get(0);
		if (record != 0)
		{
			Actor_SetDestination(19, record->x.part.pixel, record->z.part.pixel);
		}
		Actor_WaitForMove(19);
		Actor_SetPosition(19, 0, 0);
		Event_Wait(10);
		Audio_PlayCueFromEventWork();
		Event_End();
	}
}

s32 SuharaHeya_SelectEvents(void)
{
	if (Engine_GameFlagIsSet(0x96F) != 0)
	{
		return (s32)gSuharaHeyaEvents96f;
	}
	return (s32)gSuharaHeyaEvents;
}

s32 Scene_InitActorRecords(void)
{
	union SceneActor *work;
	if (gGameState.entrance == 90)
		Engine_GameFlagSet(0x96f);
	gEventWork->start_transition = 521;
	gEventWork->transition_frames = 24;
	((union SceneActor *)Object_GetById(12))->bytes[89] |= 4;
	((union SceneActor *)Object_GetById(13))->bytes[89] |= 4;
	work = (union SceneActor *)Object_GetById(20);
	work->fields.record->field_26 = 0;
	work->fields.record->angle = 0x4000;
	{
		struct SceneRecord *record = work->fields.record;
		s32					flags = ~12;

		flags &= record->flags;
		record->flags = flags | 4;
	}
	work = (union SceneActor *)Object_GetById(21);
	work->fields.record->field_26 = 0;
	work->fields.record->angle = 0x4000;
	work->bytes[85] = 2;
	work->fields.y = 0;
	return 0;
}

struct SceneEntrance gSuharaHeyaEntrances[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 256, 0, 456, FACING_NORTH, 0, 0, 280, 336, 480, 0 },
	{ 2, CONDITION_ALWAYS, 432, 0, 728, FACING_NORTH, 0, 312, 528, 552, 752, 0 },
	{ 3, CONDITION_ALWAYS, 640, 0, 168, FACING_NORTH, 0, 496, 16, 736, 192, 0 },
	{ 4, CONDITION_ALWAYS, 112, 0, 168, FACING_NORTH, 0, 0, 16, 240, 208, 0 },
	{ 5, CONDITION_ALWAYS, 352, 0, 216, FACING_NORTH, 0, 256, 16, 496, 232, 0 },
	{ 6, CONDITION_ALWAYS, 432, 0, 630, FACING_NORTH, 0, 312, 528, 552, 752, 0 },
	{ 7, CONDITION_ALWAYS, 680, 0, 120, FACING_WEST, 0, 496, 16, 736, 192, 0 },
	{ 10, CONDITION_ALWAYS, 104, 0, 360, FACING_NORTH, 0, 0, 280, 336, 480, 0 },
	{ 11, CONDITION_ALWAYS, 152, 0, 360, FACING_NORTH, 0, 0, 280, 336, 480, 0 },
	{ 12, CONDITION_ALWAYS, 472, 0, 344, FACING_SOUTH, 0, 344, 280, 584, 440, 0 },
	{ 13, CONDITION_ALWAYS, 712, 0, 344, FACING_SOUTH, 0, 600, 280, 840, 440, 0 },
	{ 14, CONDITION_ALWAYS, 88, 0, 352, FACING_SOUTH, 0, 0, 280, 336, 480, 0 },
	{ 15, CONDITION_ALWAYS, 88, 0, 584, FACING_NORTH, 0, 16, 528, 256, 688, 0 },
	{ SCENE_TABLE_END },
};

s16 gSuharaHeyaRegions[] = {
	83, 28, 356, 91, 36, 364, -1, 14,
	83, -20, 596, 91, -12, 604, -1, 15,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSuharaHeyaExits[] = {
	SCENE_EXITS(168),
	SCENE_EXIT(1, 167, 1),
	SCENE_EXIT(2, 167, 2),
	SCENE_EXIT(3, 167, 3),
	SCENE_EXIT(4, 167, 4),
	SCENE_EXIT(5, 167, 5),
	SCENE_EXIT(10, 168, 12),
	SCENE_EXIT(11, 168, 13),
	SCENE_EXIT(12, 168, 10),
	SCENE_EXIT(13, 168, 11),
	SCENE_EXIT(14, 168, 15),
	SCENE_EXIT(15, 168, 14),
	SCENE_EXITS_END,
};

struct ScenePlacement gSuharaHeyaPlacements[] = {
	{ 0x66, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(368), 0, PIXELS(192), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(80), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(56), 0, PIXELS(120), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(160), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x76, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(568), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(272), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x80, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(224), 0, PIXELS(344), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(432), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSuharaHeyaPlacements96f[] = {
	{ 0x66, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(400), 0, PIXELS(184), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(416), 0, PIXELS(184), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(56), 0, PIXELS(120), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(160), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x76, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(568), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(272), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x80, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(224), 0, PIXELS(344), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(688), 0, PIXELS(384), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(712), 0, PIXELS(408), FACING_NORTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6d, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(136), 0, PIXELS(616), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(432), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x39, 0x9b0, ACTOR_STAND,
		PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(416), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(416), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gSuharaHeyaEvents[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaMyFatherLovesPeopleSoMuch) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaLetMeKnowIfYouHave) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaWhenISeePeopleInTrouble) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaIWantToHelpPeopleSo) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaTolbiProbablyIgnoresSmallVillagesLike) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaIfYouHeadEastAcrossSuhalla) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaSuhallaIsADesertVillageI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaGoodThingItsNotEasyTo) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaAVillageThisSmallCantSupport) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaIWonderIfAnyCustomersCan) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaOnceTheFestivalEndsTheTravelers) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaForSomeReasonAnyoneWhoGets) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaTheHolyChildActorWillSave) },
	{ SEARCH(SEARCH_OVEN), 100, 0xfb0, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ SEARCH(SEARCH_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuharaHeyaItsACactusSteakIWonder)) },
	{ SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuharaHeyaHesMakingMulukhiyaSoupItTastes)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gSuharaHeyaEvents96f[] = {
	{ EVENT_TOUCH, 50, 0x9b0, EVENT_SCRIPT(EV50_TOUCH) },
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaHeya_PlayCueAndRequestTouchedExit) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaTheWoundedAreSleepingSoDont) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaIAmAmazedTheseSoldiersSurvived) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaWhyWereTheyHeadedTowardEastern) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaWhereIsActorWhatHasHappened) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaTheseOldSoldiersMustRestUntil) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaWhoCouldHaveDoneThisTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaAColumnOfWaterShotUp) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaActorWasNotFarBehindUs) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaDoNotApproachTheLargeCyclones) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaDeathStormsHuhThereWasA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaOnceYouGetSweptUpIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaYeWhoSeekSomethingAboutEyes) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaWeveHadMoreCustomersLatelyBecause) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaItWillTakeTimeForThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaTheSandstormsAreSendingCustomersOur) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaPleaseGreatSandstormsSendUsMore) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaWeWentIntoTheSuhallaTo) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaTheSandstormsAreSoBadWeve) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaEvenIfWeCantGoIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaSoShallWeGiveUpOn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaWhenThatCycloneThrewUsInto) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgSuharaHeyaIShudderToThinkSandstormsWill) },
	{ SEARCH(SEARCH_OVEN), 100, 0xfb0, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ SEARCH(SEARCH_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuharaHeyaItsACactusSteakIWonder)) },
	{ SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuharaHeyaHesMakingMulukhiyaSoupItTastes)) },
	{ SCENE_EVENTS_END },
};
