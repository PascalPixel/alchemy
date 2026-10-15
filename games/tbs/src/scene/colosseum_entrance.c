#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/colosseum_entrance.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
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
#include "field_scene.h"
#include "text/msg_ids.h"
#include "field_obj.h"
#include "ability_ids.h"
#include "objcmd.h"

// 前方宣言
static void FieldScene_DispatchBySelector(void);
static void FieldScene_CallPairWith10(s32 no);
static void SceneState_ForwardMaskedHalfwordWith10(s32 arg0, s32 arg1);
static void SceneState_ApplyRectsByFlags2384And962(void);
static void FieldScene_BuildActorPresentationSequence(void);
static void ColosseumEntrance_EscortToEastExit(void);
static void ColosseumEntrance_AnnounceFinals(void);
static void ColosseumEntrance_WalkToEastExit(void);
static void ColosseumEntrance_CelebrateAdvancement(void);
static void ColosseumEntrance_EscortToWestExit(void);
static void ColosseumEntrance_WalkToWestExit(void);
static void ColosseumEntrance_CelebrateBothWins(void);
static void ActorPresentation_SetActorsTwelveToEighteen(void);

// 向きを変える (10 フレーム): SceneState_ForwardMaskedHalfwordWith10 をインライン関数を通して呼ぶ
static inline void ColosseumEntrance_Turn(s32 actor, s32 facing)
{
	/* FAKEMATCH: forwarding inline for EV10_TOUCH's one call (call.h's Call2 did the same); called directly EV10_TOUCH's later message skip swaps r2 and r3 (7 halfwords differ in tbs-en, 2026-10-13). */
	SceneState_ForwardMaskedHalfwordWith10(actor, facing);
}

#define RECORD_17 17

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_ColosseumEntrance1)
	{
		return gColosseumEntranceEntrances1;
	}
	if (scene == (s32)&SceneId_ColosseumEntrance3)
	{
		return gColosseumEntranceEntrances3;
	}
	return gColosseumEntranceEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return gColosseumEntranceExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	const struct ScenePlacement *table;

	if (gGameState.scene == (s32)&SceneId_ColosseumEntrance1)
	{
		switch (gGameState.entrance)
		{
		case 5:
		case 69:
			table = gColosseumEntrancePlacements1Entrance5;
			break;
		case 7:
		case 70:
			table = gColosseumEntrancePlacements1Entrance7;
			break;
		case 8:
		case 21:
		case 31:
		case 64:
		case 65:
		case 67:
			table = gColosseumEntrancePlacements1Entrance8;
			break;
		case 12:
			table = gColosseumEntrancePlacements1Entrance12;
			break;
		case 66:
		case 68:
			table = gColosseumEntrancePlacements1Entrance66;
			break;
		default:
			table = gColosseumEntrancePlacements1;
			break;
		}
	}
	else if (gGameState.scene == (s32)&SceneId_ColosseumEntrance3)
	{
		if (Engine_GameFlagIsSet(0x950) != 0)
		{
			table = gColosseumEntrancePlacements3Flag950;
		}
		else if (Engine_GameFlagIsSet(0x962) != 0)
		{
			table = gColosseumEntrancePlacements3Flag962;
		}
		else
		{
			table = gColosseumEntrancePlacements3;
		}
	}
	else
	{
		table = gColosseumEntrancePlacementsOther;
	}
	return table;
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_ColosseumEntrance2)
	{
		return gColosseumEntranceEvents2;
	}
	if (scene == (s32)&SceneId_ColosseumEntrance1)
	{
		if (gGameState.entrance == 12)
		{
			return gColosseumEntranceEvents1Entrance12;
		}
		return gColosseumEntranceEvents1;
	}
	if (scene == (s32)&SceneId_ColosseumEntrance3)
	{
		return gColosseumEntranceEvents3;
	}
	return gColosseumEntranceEventsOther;
}

static void EV10_TALK(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x962))
	{
		Engine_EventSetMessage(MsgColosseumColosseumAlreadyFullIfReally);
		Engine_EventShowMessage(10, 0);
	}
	else
	{
		Engine_EventSetMessage(MsgColosseumHopingGetInSeeColosso);
		Engine_EventAskYesNo(10, 0);
	}
	Engine_EventEnd();
}

static void EV13_TALK_3(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x962))
	{
		Engine_ActorShowEmote(13, 258, 40);
		Engine_EventSetMessage(MsgColosseumWantWatchFinalsFromGood);
		Engine_EventShowMessage(13, 0);
	}
	else
	{
		Engine_EventSetMessage(MsgColosseumColossosLuckyGuessMakesBattles);
		Engine_EventShowMessage(13, 0);
	}
	Engine_EventEnd();
}

static void EV14_TALK_3(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x962) != 0)
	{
		Engine_ActorRunRepeatedMotion(14, 2);
		Engine_EventSetMessage(MsgColosseumHoldOnSecond);
		FieldScene_CallPairWith10(14);
		Engine_ActorFaceEachOther(14, ACTOR_PARTY_LEADER, 0);
		Engine_EventWait(20);
		Engine_EventAskYesNo(14, 0);
		SceneState_ForwardMaskedHalfwordWith10(14, 0);
	}
	else
	{
		Engine_EventSetMessage(MsgColosseumWantWinBigPutOn);
		Engine_EventShowMessage(14, 0);
	}
	Engine_EventEnd();
}

static void EV16_TALK_3(void)
{
	Engine_EventBegin();
	if (GameFlag_IsSet(0x962) != 0)
	{
		if (GameFlag_IsSet(0x3c0) != 0)
		{
			Engine_EventSetMessage(MsgColosseumLookImSorryButWe);
			Event_ShowMessage(16, 0);
		}
		else
		{
			Engine_EventSetMessage(MsgColosseumWantTryOutLuckyGuess);
			Event_OpenMessage(16, 0);
			if (Engine_EventChooseYesNo(0, 0) == 0)
			{
				gEventWork->message++;
				Actor_ShowEmote(16, 0x100, 40);
				Event_OpenMessage(16, 0);
				if (Engine_EventChooseYesNo(0, 0) == 0)
				{
					gEventWork->message++;
				}
				Engine_EventWait(40);
				Event_ShowMessage(16, 0);
				GameFlag_Set(0x3c0);
			}
			else
			{
				Event_ShowMessage(16, 0);
			}
		}
	}
	else
	{
		Engine_EventSetMessage(MsgColosseumDoKnowLuckyGuess);
		Event_AskYesNo(16, 0);
	}
	Engine_EventEnd();
}

static void EV13_TALK(void)
{
	struct FieldActor *rec7;

	rec7 = Object_GetById(13);
	Engine_EventBegin();
	Engine_ActorStop(13);
	Engine_ActorFaceEachOther(13, ACTOR_PARTY_LEADER, 20);
	Engine_EventSetMessage(MsgColosseumImRatedAsSecondBest);
	FieldScene_CallPairWith10(13);
	Engine_ActorRunRepeatedMotion(13, 1);
	Engine_EventShowMessage(13, 0);
	{
		u16 *target = (u16 *)&rec7->timer;
		u16	 shown = 0x2d0;

		*target = shown;
	}
	{
		u16 *target = (u16 *)&rec7->work;
		u16	 shown = 112;

		*target = shown;
	}
	Engine_ActorEnableActionCallback(13, 2);
	Engine_EventEnd();
}

static void EV14_TALK(void)
{
	Engine_EventBegin();
	Actor_SetAttachedEffect(14, 0x102);
	Engine_ActorRunRepeatedMotion(14, 2);
	Engine_EventSetMessage(MsgColosseumWaahDontFrightenMe);
	FieldScene_CallPairWith10(14);
	Actor_ShowEmote(14, 0x102, 40);
	Event_ShowMessage(14, 0);
	Engine_EventEnd();
}

static void EV15_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage(MsgColosseumWhoHaveComeQuestionMe);
	FieldScene_CallPairWith10(15);
	Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 20);
	FieldScene_CallPairWith10(15);
	Engine_ActorSetAnimationAndWait(15, 3);
	Engine_ActorSetAnimation(15, 0);
	FieldScene_CallPairWith10(15);
	SceneState_ForwardMaskedHalfwordWith10(15, 20480);
	Engine_EventEnd();
}

static void EV16_TALK(void)
{
	Engine_EventBegin();
	Engine_ActorRunRepeatedMotion(16, 2);
	Engine_EventSetMessage(MsgColosseumDekkaMustWinFinalsDekka);
	Event_ShowMessageAndWait(16, 0, 20);
	if (GameFlag_IsSet(0x3c1) != 0)
	{
		Engine_EventWait(20);
	}
	else
	{
		SceneState_ForwardMaskedHalfwordWith10(17, 0);
		Engine_ActorRunRepeatedMotion(17, 1);
		FieldScene_CallPairWith10(17);
		Engine_ActorFaceEachOther(17, ACTOR_PARTY_LEADER, 20);
		Engine_ActorSetAnimation(17, 4);
		FieldScene_CallPairWith10(17);
		Actor_ShowEmote(17, 0x105, 40);
		FieldScene_CallPairWith10(17);
		SceneState_ForwardMaskedHalfwordWith10(17, 0x5000);
		GameFlag_Set(0x3c1);
	}
	Engine_EventEnd();
}

static void EV17_TALK(void)
{
	Engine_EventBegin();
	Engine_ActorFaceEachOther(17, ACTOR_PARTY_LEADER, 20);
	Engine_EventSetMessage(MsgColosseumAmNavampaGondowanSixthRanked);
	FieldScene_CallPairWith10(17);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(17, 3);
	FieldScene_CallPairWith10(17);
	Engine_ActorRunRepeatedMotion(17, 1);
	FieldScene_CallPairWith10(17);
	SceneState_ForwardMaskedHalfwordWith10(17, 20480);
	Engine_EventEnd();
}

static void EV18_TALK(void)
{
	Engine_EventBegin();
	Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 20);
	Engine_EventSetMessage(MsgColosseumImBufordSeventhSeed);
	FieldScene_CallPairWith10(18);
	Actor_FaceDirection(18, 0xd000, 20);
	Actor_FaceDirection(18, 0xb000, 20);
	Actor_FaceDirection(18, FACING_WEST, 40);
	Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 20);
	FieldScene_CallPairWith10(18);
	Engine_ActorSetAnimationAndWait(18, 3);
	FieldScene_CallPairWith10(18);
	SceneState_ForwardMaskedHalfwordWith10(18, 0x5000);
	Engine_EventEnd();
}

static void EV08_TALK(void)
{
	Engine_EventBegin();
	Engine_ActorFaceEachOther(8, ACTOR_PARTY_LEADER, 20);
	Engine_EventSetMessage(MsgColosseumYouReadyForFinals);
	Engine_EventOpenMessage(8, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
		gEventWork->message++;
	Engine_EventShowMessage(8, 0);
	Engine_EventEnd();
}

static void EV10_TOUCH(void)
{
	s32 record;

	if (Engine_GameFlagIsSet(0x962) == 0)
	{
		return;
	}

	Engine_EventBegin();
	Engine_ActorRunRepeatedMotion(17, 1);
	Actor_FaceDirection(17, 0x3000, 20);
	Engine_ActorFaceDirection(17, FACING_EAST, 60);
	Actor_ShowEmote(17, 0x100, 40);
	SceneState_ForwardMaskedHalfwordWith10(17, 0x3000);
	Engine_EventSetMessage(MsgColosseumHeyYouRe);
	Engine_ActorStartRepeatedMotion(17, 2);
	FieldScene_CallPairWith10(17);
	Engine_ActorFaceDirection(18, 0x3000, 0);
	Engine_ActorFaceDirection(19, 0x3000, 0);
	Engine_ActorFaceDirection(20, 0x3000, 0);
	Engine_EventGetViewCenter()->motionFlags = 0;
	Engine_CameraSetSpeed(0x19999, 0x3333);
	Camera_MoveTo(0x1000000, -1, 0xac0000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Actor_ShowEmote(18, 0x102, 40);
	FieldScene_CallPairWith10(18);
	Engine_ActorSetAnimationAndWait(17, 3);
	FieldScene_CallPairWith10(17);
	Actor_SetAttachedEffect(19, 0x102);
	Engine_EventWait(40);
	SceneState_ForwardMaskedHalfwordWith10(19, 0);
	FieldScene_CallPairWith10(19);
	Actor_ShowEmote(20, 0x103, 40);
	SceneState_ForwardMaskedHalfwordWith10(20, 0);
	Engine_ActorStartRepeatedMotion(20, 2);
	Engine_EventShowMessageAndWait(20, 0, 20);
	Engine_ActorRunRepeatedMotion(17, 1);
	Engine_EventWait(10);
	Actor_FaceDirection(17, 0xb000, 20);
	Engine_EventShowMessageAndWait(17, 0, 20);
	Engine_ActorRunRepeatedMotion(18, 1);
	Engine_ActorSetAnimationAndWait(18, 4);
	Engine_EventWait(20);
	SceneState_ForwardMaskedHalfwordWith10(17, 0x8000);
	Engine_EventShowMessageAndWait(17, 0, 20);
	Engine_ActorRunRepeatedMotion(19, 1);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(19, 4);
	Engine_EventShowMessageAndWait(17, 0, 20);
	Engine_ActorRunRepeatedMotion(20, 2);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(20, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(17, 3);
	Engine_ActorSetAnimation(18, 3);
	Engine_ActorSetAnimation(19, 3);
	Engine_ActorSetAnimationAndWait(20, 3);
	Engine_ActorFaceDirection(17, 0x3000, 0);
	Engine_ActorFaceDirection(18, 0x3000, 0);
	Engine_ActorFaceDirection(19, 0x3000, 0);
	SceneState_ForwardMaskedHalfwordWith10(20, 0x3000);
	Actor_SetSpeed(17, 0x9999, 0x4ccc);
	Actor_WalkToAndWait(17, 0x102, 172);
	Actor_SetSpeed(0, 0xcccc, 0x6666);
	Actor_WalkToAndWait(0, 0x106, 188);
	Actor_FaceDirection(0, FACING_NORTH, 0);
	record = Object_GetById(0);
	if (record != 0)
	{
		Engine_ActorSetPosition(
			1, ((struct FieldActor *)record)->x.fixed, ((struct FieldActor *)record)->z.fixed);
	}
	record = Object_GetById(0);
	if (record != 0)
	{
		Engine_ActorSetPosition(
			2, ((struct FieldActor *)record)->x.fixed, ((struct FieldActor *)record)->z.fixed);
	}
	record = Object_GetById(0);
	if (record != 0)
	{
		Engine_ActorSetPosition(
			3, ((struct FieldActor *)record)->x.fixed, ((struct FieldActor *)record)->z.fixed);
	}
	Actor_SetSpeed(1, 0xcccc, 0x6666);
	Actor_SetSpeed(2, 0xcccc, 0x6666);
	Actor_SetSpeed(3, 0xcccc, 0x6666);
	Engine_ActorWalkTo(1, 246, 200);
	Actor_WalkTo(2, 0x106, 200);
	Actor_WalkToAndWait(3, 0x116, 200);
	Engine_ActorSetAnimation(2, 1);
	Engine_ActorSetAnimation(1, 1);
	Actor_FaceDirection(1, FACING_NORTH, 0);
	Actor_FaceDirection(2, FACING_NORTH, 0);
	Actor_FaceDirection(3, FACING_NORTH, 40);
	Engine_ActorRunRepeatedMotion(17, 1);
	FieldScene_CallPairWith10(17);
	Engine_ActorStartRepeatedMotion(0, 2);
	Engine_ActorStartRepeatedMotion(1, 2);
	Engine_ActorStartRepeatedMotion(2, 2);
	Engine_ActorRunRepeatedMotion(3, 2);
	Engine_ActorSetAnimationAndWait(17, 3);
	Event_OpenMessage(17, 0);
	Actor_FaceDirection(1, FACING_NORTHEAST, 0);
	Actor_FaceDirection(3, FACING_NORTHWEST, 0);

	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_ActorSetAnimationAndWait(17, 3);
		FieldScene_CallPairWith10(17);
		Engine_ActorEnableActionCallback(1, ColosseumEntrance_ActionTable1);
		Engine_ActorEnableActionCallback(2, ColosseumEntrance_ActionTable1);
		Object_SetActionCallbackAndRefreshById(3, (s32)ColosseumEntrance_ActionTable1);
		Engine_CameraSetSpeed(0x6666, 0xccc);
		Engine_CameraMoveTo(0x1000000, -1, 0x640000, 1);
		Actor_SetSpeed(17, 0x10000, 0x8000);
		Engine_ActorEnableActionCallback(17, ColosseumEntrance_ActionTable4);
		Engine_EventWait(10);
		Actor_SetSpeed(0, 0x10000, 0x8000);
		Engine_ActorEnableActionCallback(0, ColosseumEntrance_ActionTable4);
		Engine_EventWait(80);
		gEventWork->transitionFrames = 40;
		Engine_EventCloseScreen();
		Engine_EventWaitForScreen();
	}
	else
	{
		Event_SkipMessages(1);
		Actor_ShowEmote(17, 0x102, 40);
		FieldScene_CallPairWith10(17);
		Actor_ShowEmote(18, 0x102, 40);
		FieldScene_CallPairWith10(18);
		Actor_ShowEmote(19, 0x102, 40);
		FieldScene_CallPairWith10(19);
		Actor_ShowEmote(20, 0x102, 40);
		FieldScene_CallPairWith10(19);
		Engine_ActorSetAnimationAndWait(17, 4);
		FieldScene_CallPairWith10(17);
		Engine_ActorRunRepeatedMotion(2, 1);
		Engine_EventWait(20);
		FieldScene_CallPairWith10(2);
		Actor_ShowEmote(3, 0x102, 40);
		FieldScene_CallPairWith10(3);
		Actor_FaceDirection(17, 0xb000, 0);
		Engine_ActorFaceDirection(19, FACING_WEST, 0);
		Engine_ActorFaceDirection(20, FACING_EAST, 60);
		Engine_ActorFaceDirection(17, 0x3000, 0);
		Engine_ActorFaceDirection(19, 0x3000, 0);
		Engine_ActorFaceDirection(20, 0x3000, 20);
		Engine_ActorSetAnimation(17, 3);
		Engine_ActorSetAnimation(18, 3);
		Engine_ActorSetAnimation(19, 3);
		Engine_ActorSetAnimationAndWait(20, 3);
		Actor_ShowEmote(3, 0x100, 60);
		ColosseumEntrance_Turn(3, 0xa000);
		FieldScene_CallPairWith10(3);
		Engine_ActorSetAnimation(1, 4);
		Engine_EventWait(20);
		Event_OpenMessage(1, 0);

		record = Engine_EventChooseYesNo(0, 0);
		if (record == 0)
		{
			Engine_ActorSetAnimationAndWait(17, 3);
			FieldScene_CallPairWith10(17);
			Engine_ActorEnableActionCallback(1, ColosseumEntrance_ActionTable1);
			Engine_ActorEnableActionCallback(2, ColosseumEntrance_ActionTable1);
			Object_SetActionCallbackAndRefreshById(3, (s32)ColosseumEntrance_ActionTable1);
			Engine_CameraSetSpeed(0x6666, 0xccc);
			Engine_CameraMoveTo(0x1000000, -1, 0x640000, 1);
			Actor_SetSpeed(17, 0x10000, 0x8000);
			Engine_ActorEnableActionCallback(17, ColosseumEntrance_ActionTable4);
			Engine_EventWait(10);
			Actor_SetSpeed(0, 0x10000, 0x8000);
			Engine_ActorEnableActionCallback(0, ColosseumEntrance_ActionTable4);
			Engine_EventWait(80);
			gEventWork->transitionFrames = 40;
			Engine_EventCloseScreen();
			Engine_EventWaitForScreen();
		}
		else
		{
			Event_SkipMessages(1);
			Engine_EventWait(20);
			Engine_ActorStartRepeatedMotion(1, 2);
			Engine_EventShowMessageAndWait(1, 0, 20);
			Engine_ActorShowEmote(2, 0x102, 60);
			FieldScene_CallPairWith10(2);
			SceneState_ForwardMaskedHalfwordWith10(3, 0x8000);
			Engine_ActorSetAnimation(3, 3);
			FieldScene_CallPairWith10(3);
			Engine_ActorEnableActionCallback(2, ColosseumEntrance_ActionTable2);
			Object_SetActionCallbackAndRefreshById(3, (s32)ColosseumEntrance_ActionTable2);
			Engine_EventWait(20);
			Object_SetActionCallbackAndRefreshById(0, (s32)ColosseumEntrance_ActionTable2);
			Actor_WalkToAndWait(1, 0x106, 188);
			SceneState_ForwardMaskedHalfwordWith10(1, 0xc000);
			Engine_ActorSetAnimationAndWait(1, 3);
			FieldScene_CallPairWith10(1);
			Engine_CameraSetSpeed(0x6666, 0xccc);
			Engine_CameraMoveTo(0x1000000, -1, 0x640000, 1);
			Actor_SetSpeed(17, 0x10000, 0x8000);
			Engine_ActorEnableActionCallback(17, ColosseumEntrance_ActionTable4);
			Engine_EventWait(10);
			Actor_SetSpeed(1, 0x10000, 0x8000);
			Engine_ActorEnableActionCallback(1, ColosseumEntrance_ActionTable4);
			Engine_EventWait(80);
			gEventWork->transitionFrames = 40;
			Engine_EventCloseScreen();
			Engine_EventWaitForScreen();
		}
	}

	Engine_EventRequestExit(2);
	Engine_GameFlagSet(0x93f);
	Engine_EventEnd();
}

static void EV12_TOUCH(void)
{
	struct FieldActor *actor;
	s32				   script;

	if (Engine_GameFlagIsSet(0x234) != 0)
	{
		Engine_GameFlagSet(0x235);
		Engine_EventBegin();
		Actor_SetSpeed(0, 0x10000, 0x8000);
		Actor_WalkToAndWait(0, 0x368, 120);
		SceneState_ForwardMaskedHalfwordWith10(0, 0xa000);
		Camera_SetSpeed(0x19999, 0x3333);
		Camera_MoveTo(0x3300000, -1, 0x700000, 1);
		Engine_CameraWaitForMove();
		Actor_FaceDirection(8, 0x3000, 20);
		Engine_ActorRunRepeatedMotion(8, 1);
		Engine_EventSetMessage(MsgColosseumYouReadyForFinals);
		Event_OpenMessage(0x8008, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_EventWait(20);
			SceneState_ForwardMaskedHalfwordWith10(12, 0xd000);
			Engine_ActorRunRepeatedMotion(12, 1);
			Engine_EventSetMessage(MsgColosseumDayTiredWaiting);
			FieldScene_CallPairWith10(0x400c);
			SceneState_ForwardMaskedHalfwordWith10(17, 0);
			SceneState_ForwardMaskedHalfwordWith10(0, 0x8000);
			Engine_ActorSetAnimationAndWait(17, 3);
			Engine_ActorSetAnimationAndWait(0, 3);
			Engine_ActorSetAnimation(15, 1);
			Engine_ActorFaceDirection(15, 0xd000, 20);
			Engine_ActorRunRepeatedMotion(15, 1);
			FieldScene_CallPairWith10(15);
			SceneState_ForwardMaskedHalfwordWith10(16, 0x8000);
			Engine_ActorSetAnimationAndWait(16, 3);
			Engine_ActorRunRepeatedMotion(17, 2);
			SceneState_ForwardMaskedHalfwordWith10(17, 0xa000);
			FieldScene_CallPairWith10(0x4011);
			SceneState_ForwardMaskedHalfwordWith10(18, 0xb000);
			Engine_ActorStartRepeatedMotion(18, 2);
			FieldScene_CallPairWith10(0x4012);
			SceneState_ForwardMaskedHalfwordWith10(11, 0x3000);
			Engine_ActorSetAttachedEffect(11, 0x102);
			Engine_EventWait(60);
			Engine_ActorStartRepeatedMotion(11, 2);
			FieldScene_CallPairWith10(0x800b);
			Engine_ActorStop(13);
			Engine_TaskWait(1);
			Engine_ActorStartRepeatedMotion(13, 2);
			Engine_ActorStartRepeatedMotion(14, 2);
			Engine_ActorRunRepeatedMotion(16, 2);
			Engine_EventWait(20);
			Engine_ActorFaceDirection(13, FACING_EAST, 0);
			Engine_ActorFaceDirection(14, FACING_WEST, 0);
			Engine_ActorFaceDirection(16, 0xb000, 40);
			Engine_ActorSetAnimation(13, 3);
			Engine_ActorSetAnimation(14, 3);
			Engine_ActorSetAnimationAndWait(16, 3);
			Actor_SetSpeed(13, 0x19999, 0xcccc);
			Actor_SetSpeed(14, 0x19999, 0xcccc);
			Actor_SetSpeed(16, 0x19999, 0xcccc);
			Engine_ActorEnableActionCallback(13, ColosseumEntrance_ActionTable5);
			Engine_ActorEnableActionCallback(16, ColosseumEntrance_ActionTable7);
			Engine_EventWait(20);
			Engine_ActorFaceDirection(15, 0xd000, 0);
			Engine_ActorFaceDirection(17, 0xb000, 0);
			Engine_ActorFaceDirection(0, FACING_NORTHWEST, 0);
			Engine_ActorFaceDirection(12, 0xd000, 0);
			Engine_ActorFaceDirection(18, 0xb000, 0);
			Object_SetActionCallbackAndRefreshById(14, (s32)ColosseumEntrance_ActionTable6);
			Engine_EventWait(20);
			Engine_ActorFaceDirection(8, FACING_EAST, 0);
			Engine_ActorFaceDirection(11, FACING_WEST, 40);
			Engine_ActorSetAnimation(8, 3);
			Engine_ActorSetAnimationAndWait(11, 3);
			Engine_CameraMoveTo(0x3280000, -1, 0x560000, 1);
			Engine_CameraWaitForMove();
			Engine_ActorRunRepeatedMotion(8, 2);
			FieldScene_CallPairWith10(8);
			Engine_ActorSetSpeed(8, 0x10000, 0x8000);
			Object_GetById(8)->steeringFlags &= ~ACTOR_FACES_MOTION;
			Engine_ActorWalkToAndWait(8, 0x318, 72);
			Engine_EventWait(1);
			Object_GetById(8)->steeringFlags |= ACTOR_FACES_MOTION;
			SceneState_ForwardMaskedHalfwordWith10(8, 0);
			Engine_ActorStop(13);
			actor = Object_GetById(13);
			actor->update = NULL;
			actor->timer = 0;
			actor->work = 0;
			actor->velocityX = 0;
			actor->velocityY = 0;
			actor->velocityZ = 0;
			actor->targetX = ACTOR_NO_TARGET;
			actor->targetY = ACTOR_NO_TARGET;
			actor->targetZ = ACTOR_NO_TARGET;
			Engine_TaskWait(1);
			script = (s32)ColosseumEntrance_ActionTable8;
			Engine_ActorEnableActionCallback(15, script);
			Engine_EventWait(20);
			Engine_ActorEnableActionCallback(13, script);
			Engine_EventWait(20);
			Engine_ActorEnableActionCallback(17, script);
			Engine_EventWait(20);
			Engine_ActorEnableActionCallback(14, script);
			Engine_EventWait(20);
			Engine_ActorEnableActionCallback(16, script);
			Engine_EventWait(20);
			Engine_ActorEnableActionCallback(12, script);
			Engine_EventWait(20);
			Engine_ActorEnableActionCallback(18, script);
			Engine_EventWait(60);
			Engine_ActorEnableActionCallback(0, script);
			Engine_EventWait(80);
			Engine_EventRequestExit(66);
		}
		else
		{
			gEventWork->message++;
			Engine_EventShowMessage(0x8008, 0);
		}
		Engine_EventEnd();
	}
}

static void EV13_TOUCH(void)
{
	Engine_GameFlagClear(0x235);
	Engine_GameFlagSet(564);
}

static void EV17_TALK_1(void)
{
	s32 flag_8a4;

	Engine_EventBegin();
	flag_8a4 = GameFlag_IsSet(0x8a4);
	if (flag_8a4 != 0)
	{
		Engine_ActorFaceEachOther(RECORD_17, ACTOR_PARTY_LEADER, 40);
		Engine_EventSetMessage(MsgColosseumHavePerfectTechniquesLikeThose);
		FieldScene_CallPairWith10(RECORD_17);
		Actor_FaceDirection(RECORD_17, 0x3000, 20);
	}
	else
	{
		Engine_ActorStartRepeatedMotion(RECORD_17, 2);
		Engine_EventSetMessage(MsgColosseumSilence);
		Event_ShowMessage(RECORD_17, 0);
		*(((u8 *)Engine_EventGetViewCenter()) + 85) = flag_8a4;
		Engine_TaskWait(1);
		Camera_SetSpeed(0x66666, 0xcccc);
		Camera_MoveTo(0x21c0000, -1, 0xd00000, 1);
		Engine_CameraWaitForMove();
		gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
		gEventWork->transitionFrames = 32;
		Engine_EventCloseScreen();
		Engine_EventWaitForScreen();
		if (GameFlag_IsSet(0x8a3) != 0)
		{
			Engine_EventRequestExit(70);
		}
		else
		{
			Engine_EventRequestExit(7);
		}
	}
	Engine_EventEnd();
}

static void FieldScene_BuildActorPresentationGroup(void)
{
	struct FieldActor *rec2;
	s32				   shift;

	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Event_PlayCue(247);
	Engine_ActorSetAnimation(8, 2);
	Engine_ActorSetAnimation(9, 2);
	Engine_ActorSetAnimation(10, 2);
	Engine_ActorSetAnimation(11, 2);
	Engine_ActorSetAnimation(12, 2);
	Engine_ActorSetAnimation(13, 2);
	Engine_ActorSetAnimation(14, 0);
	Engine_ActorSetAnimation(15, 0);
	Engine_ActorSetAnimation(16, 0);
	Engine_ActorSetAnimation(17, 0);
	Engine_ActorSetAnimation(18, 0);
	rec2 = Object_GetById(21);
	Engine_ActorSetSpriteFlags(rec2, 0);
	rec2 = Object_GetById(19);
	rec2->scaleX = -0x10000;
	rec2 = Object_GetById(20);
	rec2->scaleX = -0x10000;
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Engine_TaskWait(1);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	gEventWork->transitionFrames = 32;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(16, 1);
	Engine_EventWait(20);
	Actor_SetSpeed(16, 0xcccc, 0x6666);
	Actor_WalkToAndWait(16, 164, 0x388);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(16, 9);
	Engine_EventWait(40);
	Engine_ActorSetAnimation(16, 10);
	Engine_EventWait(60);
	Engine_ActorSetAnimation(16, 1);
	Engine_EventWait(20);
	Actor_WalkToAndWait(16, 164, 0x398);
	Actor_WalkToAndWait(16, 185, 0x398);
	Actor_FaceDirection(16, FACING_NORTH, 20);
	Actor_WalkToAndWait(16, 185, 0x394);
	Engine_ActorSetAnimation(16, 11);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(16, 1);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(16, 3);
	Engine_EventWait(40);
	Engine_ActorEnableActionCallback(16, (s32)ColosseumEntrance_ActionTable9);
	Engine_EventWait(80);
	Actor_SetAttachedEffect(16, 0x102);
	Engine_EventWait(60);
	Actor_FaceDirection(14, 0xd000, 0);
	Actor_FaceDirection(15, 0x5000, 0);
	Actor_FaceDirection(17, FACING_EAST, 0);
	Actor_FaceDirection(18, FACING_WEST, 20);
	Actor_SetAttachedEffect(14, 0x102);
	Actor_SetAttachedEffect(15, 0x102);
	Actor_SetAttachedEffect(17, 0x102);
	Actor_SetAttachedEffect(18, 0x102);
	Engine_EventWait(60);
	Actor_FaceDirection(14, 0x3000, 0);
	Actor_FaceDirection(15, 0x3000, 0);
	Actor_FaceDirection(17, 0x3000, 0);
	shift = 0x3000;
	SceneState_ForwardMaskedHalfwordWith10(18, shift);
	Engine_ActorStop(16);
	rec2 = Object_GetById(16);
	rec2->facing = 0xd000;
	rec2->scaleX = 0x10000;
	rec2->scaleY = 0x10000;
	Engine_EventWait(20);
	Engine_ActorSetAnimation(16, 0);
	Engine_EventWait(40);
	Engine_ActorSetAnimation(19, 5);
	Engine_ActorSetAnimation(20, 5);
	Engine_EventWait(60);
	Actor_FaceDirection(16, shift, 20);
	Engine_ActorSetAnimation(16, 8);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(14, 4);
	Engine_ActorSetAnimation(15, 4);
	Engine_ActorSetAnimation(17, 4);
	Engine_ActorSetAnimationAndWait(18, 4);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(16, 4);
	Engine_EventWait(10);
	Actor_SetSpeed(16, 0x20000, 0x10000);
	Actor_WalkToAndWait(16, 162, 0x394);
	Actor_WalkToAndWait(16, 162, 0x37a);
	Engine_ActorSetAnimation(19, 1);
	Engine_ActorSetAnimation(20, 1);
	Actor_WalkToAndWait(16, 184, 0x35f);
	Actor_WalkToAndWait(16, 184, 0x31c);
	Actor_SetPosition(16, 0, 0);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	gEventWork->transitionFrames = 16;
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_EventRequestExit(69);
	Engine_EventEnd();
}

static void ColosseumEntrance_ShowQualifyingTrialMessage(void)
{
	Engine_EventBegin();
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);

	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();

	Engine_EventWait(20);
	SceneState_ForwardMaskedHalfwordWith10(17, 160 << 7);
	Engine_EventSetMessage(MsgColosseumHaveMakeThroughCountlessMatches);

	if (Engine_GameFlagIsSet(0x8a4) != 0)
	{
		gEventWork->message = (gEventWork->message + 1);
	}

	FieldScene_CallPairWith10(17);
	SceneState_ForwardMaskedHalfwordWith10(17, 192 << 6);
	Engine_GameFlagSet(0x8a3);

	Engine_EventEnd();
}

static void FieldScene_RunMiddleSequence(void)
{
	u32				   counter;
	struct FieldActor *rec;
	struct FieldActor *rec2;

	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Event_PlayCue(247);
	Engine_ActorSetAnimation(8, 2);
	Engine_ActorSetAnimation(9, 2);
	Engine_ActorSetAnimation(10, 2);
	Engine_ActorSetAnimation(11, 2);
	Engine_ActorSetAnimation(12, 2);
	Engine_ActorSetAnimation(13, 2);
	Engine_ActorSetAnimation(14, 0);
	Engine_ActorSetAnimation(15, 0);
	Actor_SetPosition(16, 0, 0);
	Engine_ActorSetAnimation(17, 0);
	Engine_ActorSetAnimation(18, 0);
	rec2 = Object_GetById(21);
	Engine_ActorSetSpriteFlags(rec2, 0);
	rec2 = Object_GetById(19);
	rec2->scaleX = -0x10000;
	rec2 = Object_GetById(20);
	rec2->scaleX = -0x10000;
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Engine_TaskWait(1);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	gEventWork->transitionFrames = 32;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(17, 1);
	Engine_EventWait(20);
	Actor_SetSpeed(17, 0xcccc, 0x6666);
	Actor_WalkToAndWait(17, 164, 0x388);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(17, 9);
	Engine_EventWait(40);
	Engine_ActorSetAnimation(17, 10);
	Engine_EventWait(60);
	Engine_ActorSetAnimation(17, 1);
	Engine_EventWait(20);
	Actor_WalkToAndWait(17, 164, 0x398);
	Actor_WalkToAndWait(17, 185, 0x398);
	Actor_FaceDirection(17, FACING_NORTH, 20);
	Actor_WalkToAndWait(17, 185, 0x394);
	Engine_ActorSetAnimation(17, 11);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(17, 1);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(17, 3);
	Engine_EventWait(40);
	Engine_ActorEnableActionCallback(17, (s32)ColosseumEntrance_ActionTable9);
	Engine_EventWait(80);
	Actor_SetAttachedEffect(17, 0x102);
	Engine_EventWait(60);
	Actor_FaceDirection(14, 0xd000, 0);
	Actor_FaceDirection(15, 0x5000, 0);
	Actor_FaceDirection(18, FACING_WEST, 20);
	Actor_SetAttachedEffect(14, 0x102);
	Actor_SetAttachedEffect(15, 0x102);
	Actor_SetAttachedEffect(17, 0x102);
	Actor_SetAttachedEffect(18, 0x102);
	Engine_EventWait(60);
	Actor_FaceDirection(14, 0x3000, 0);
	Actor_FaceDirection(15, 0x3000, 0);
	SceneState_ForwardMaskedHalfwordWith10(18, 0x3000);
	Actor_SetAttachedEffect(17, 0x101);
	rec = Object_GetById(21);
	rec->motionFlags = 0;
	for (counter = 0; counter < 20; counter++)
	{
		rec->y.fixed += 0x9999;
		Engine_TaskWait(4);
		rec->y.fixed += -0x4ccc;
		Engine_TaskWait(4);
	}
	Engine_ActorSetAnimation(19, 6);
	Engine_ActorSetAnimation(20, 6);
	Engine_EventWait(60);
	Actor_SetAttachedEffect(17, 0x100);
	Engine_ActorStop(17);
	Engine_ActorSetAnimation(17, 1);
	rec2 = Object_GetById(17);
	rec2->facing = 0xd000;
	rec->motionFlags = 3;
	rec->scaleX = 0x10000;
	rec->scaleY = 0x10000;
	Engine_EventWait(10);
	Event_PlayCue(107);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Engine_EventWait(10);
	Event_PlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	Actor_SetSpeed(17, 0x19999, 0xcccc);
	Actor_WalkToAndWait(17, 208, 0x3a0);
	Event_PlayCue(92);
	Actor_FaceDirection(17, 0x3000, 20);
	Engine_ActorSetAnimation(17, 9);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(17, 10);
	Engine_EventWait(40);
	Engine_ActorSetAnimation(17, 9);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(17, 10);
	Engine_EventWait(80);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	gEventWork->transitionFrames = 16;
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	GameFlag_Set(0x8a4);
	Engine_EventRequestExit(69);
	Engine_EventEnd();
}

s32 Scene_Initialize(void)
{
	s32 scene = gGameState.scene;

	if (scene == (s32)&SceneId_ColosseumEntrance1)
	{
		FieldScene_DispatchBySelector();
	}
	else if (scene == (s32)&SceneId_ColosseumEntrance3)
	{
		SceneState_ApplyRectsByFlags2384And962();
	}
	return 0;
}

static void FieldScene_DispatchBySelector(void)
{
	s32 no;

	Engine_TaskWait(1);
	no = gGameState.entrance;
	switch (no)
	{
	case 5:
		Engine_ActorSetAnimation(8, 2);
		Engine_ActorSetAnimation(9, 2);
		break;
	case 69:
		Engine_ActorSetAnimation(8, 2);
		Engine_ActorSetAnimation(9, 2);
		if (Engine_GameFlagIsSet(0x109) != 0)
			break;
		ColosseumEntrance_ShowQualifyingTrialMessage();
		break;
	case 7:
		FieldScene_BuildActorPresentationGroup();
		break;
	case 70:
		FieldScene_RunMiddleSequence();
		break;
	case 64:
		FieldScene_BuildActorPresentationSequence();
		InventorySnapshot_Take();
		break;
	case 65:
		ColosseumEntrance_EscortToEastExit();
		break;
	case 66:
		ColosseumEntrance_WalkToEastExit();
		break;
	case 12:
		Engine_GameFlagSet(324);
		ActorPresentation_SetActorsTwelveToEighteen();
		if (Engine_GameFlagIsSet(0x109) != 0)
			break;
		ColosseumEntrance_AnnounceFinals();
		break;
	case 21:
		Party_AddActiveOwner(1);
		Party_AddActiveOwner(2);
		Party_AddActiveOwner(3);
		Engine_GameFlagSet(0x90e);
		ColosseumEntrance_CelebrateAdvancement();
		break;
	case 67:
		ColosseumEntrance_EscortToWestExit();
		break;
	case 68:
		ColosseumEntrance_WalkToWestExit();
		break;
	case 31:
		Party_AddActiveOwner(1);
		Party_AddActiveOwner(2);
		Party_AddActiveOwner(3);
		Engine_GameFlagSet(0x90f);
		ColosseumEntrance_CelebrateBothWins();
		break;
	default:
		break;
	}
}

static void FieldScene_CallPairWith10(s32 no)
{
	Engine_EventShowMessage(no, 0);
	Engine_EventWait(10);
}

static void SceneState_ForwardMaskedHalfwordWith10(s32 arg0, s32 arg1)
{
	Engine_ActorFaceDirection(arg0, (u16)arg1, 10);
}

static void SceneState_ApplyRectsByFlags2384And962(void)
{
	s32 a, b;

	if (Engine_GameFlagIsSet(2384) != 0)
	{
		Map_CopyCellsTo(64, 0, 48, 5, 2, 2);
		a = 16;
		b = 8;
		Map_CopyCellAttributes(14, 8, 2, 1, a, b);
	}
	else
	{
		Engine_ActorSetChildValue(16, 2);
		if (Engine_GameFlagIsSet(0x962) != 0)
		{
			a = 14;
			b = 11;
			Map_CopyCellAttributes(30, 22, 1, 2, a, b);
		}
	}
}

static void FieldScene_BuildActorPresentationSequence(void)
{
	s32 flag;

	Engine_EventBegin();
	Actor_SetPosition(GARET, 0x3180000, 0x880000);
	Actor_SetPosition(IVAN, 0x3380000, 0x880000);
	Actor_SetPosition(MIA, 0x3280000, 0x980000);
	Engine_TaskWait(1);
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_ColorBufferApplySource(0, 0);
	ColorBuffer_ApplyTarget(0, 0);
	Engine_ColorBufferInterpolate(1);
	Engine_TaskWait(1);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
	gEventWork->transitionFrames = 1;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_ColorBufferApplySource(0, 0);
	ColorBuffer_ApplyTarget(0x10002, 0);
	Engine_ColorBufferInterpolate(40);
	Engine_EventWait(80);
	Engine_ActorRunRepeatedMotion(8, 1);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(IVAN, 2);
	Engine_EventWait(40);
	SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	ColorBuffer_ApplyTarget(0x10000, 0);
	Engine_ColorBufferInterpolate(40);
	Engine_EventWait(80);
	Actor_ShowEmote(IVAN, 0x100, 0);
	Engine_ActorRunRepeatedMotion(IVAN, 1);
	Engine_EventWait(20);
	Engine_EventSetMessage(MsgColosseumIsaacOnlyOneEnteringFinals);
	FieldScene_CallPairWith10(2);
	Engine_ActorSetAnimationAndWait(8, 3);
	FieldScene_CallPairWith10(8);
	Actor_ShowEmote(MIA, 0x102, 40);
	FieldScene_CallPairWith10(3);
	Actor_FaceDirection(8, FACING_WEST, 0);
	Actor_FaceDirection(9, FACING_EAST, 40);
	Actor_FaceDirection(8, 0x5000, 0);
	SceneState_ForwardMaskedHalfwordWith10(9, 0x3000);
	Actor_ShowEmote(9, 0x105, 20);
	FieldScene_CallPairWith10(9);
	Actor_ShowEmote(GARET, 0x103, 20);
	Engine_ActorStartRepeatedMotion(GARET, 2);
	FieldScene_CallPairWith10(1);
	Actor_ShowEmote(10, 0x102, 40);
	FieldScene_CallPairWith10(10);
	Engine_ActorSetAnimationAndWait(11, 3);
	FieldScene_CallPairWith10(11);
	SceneState_ForwardMaskedHalfwordWith10(2, 0xa000);
	Engine_ActorSetAnimationAndWait(IVAN, 4);
	FieldScene_CallPairWith10(2);
	Engine_ActorRunRepeatedMotion(MIA, 1);
	FieldScene_CallPairWith10(3);
	Actor_ShowEmote(GARET, 0x103, 20);
	SceneState_ForwardMaskedHalfwordWith10(1, 0);
	Engine_ActorStartRepeatedMotion(GARET, 2);
	FieldScene_CallPairWith10(1);
	SceneState_ForwardMaskedHalfwordWith10(0, 0x6000);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
	Engine_ActorRunRepeatedMotion(IVAN, 1);
	SceneState_ForwardMaskedHalfwordWith10(2, 0x8000);
	FieldScene_CallPairWith10(2);
	Engine_ActorSetAnimation(MIA, 4);
	Engine_EventWait(20);
	FieldScene_CallPairWith10(3);
	Engine_ActorRunRepeatedMotion(GARET, 1);
	SceneState_ForwardMaskedHalfwordWith10(1, 0x2000);
	FieldScene_CallPairWith10(1);
	Actor_ShowEmote(IVAN, 0x105, 60);
	Actor_FaceDirection(IVAN, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(GARET, FACING_EAST, 0);
	Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(GARET, 0x102, 0);
	Actor_ShowEmote(MIA, 0x102, 80);
	Actor_ShowEmote(IVAN, 0x106, 0);
	Engine_ActorRunRepeatedMotion(IVAN, 1);
	SceneState_ForwardMaskedHalfwordWith10(2, 0xc000);
	FieldScene_CallPairWith10(2);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GARET, FACING_NORTH, 0);
	Actor_FaceDirection(MIA, FACING_NORTH, 20);
	SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
	Engine_ActorSetAnimationAndWait(8, 3);
	FieldScene_CallPairWith10(8);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GARET, 0x100, 0);
	Actor_ShowEmote(MIA, 0x100, 40);
	Engine_ActorRunRepeatedMotion(9, 1);
	FieldScene_CallPairWith10(9);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
	Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
	Engine_ActorRunRepeatedMotion(10, 1);
	FieldScene_CallPairWith10(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GARET, FACING_NORTH, 0);
	Actor_FaceDirection(IVAN, FACING_NORTH, 0);
	SceneState_ForwardMaskedHalfwordWith10(3, 0xc000);
	Engine_ActorSetAnimationAndWait(11, 3);
	FieldScene_CallPairWith10(11);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
	Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
	Actor_ShowEmote(GARET, 0x105, 0);
	Actor_ShowEmote(IVAN, 0x105, 0);
	Actor_ShowEmote(MIA, 0x105, 80);
	Actor_ShowEmote(IVAN, 0x106, 0);
	Engine_ActorRunRepeatedMotion(IVAN, 1);
	SceneState_ForwardMaskedHalfwordWith10(2, 0xe000);
	FieldScene_CallPairWith10(2);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	Actor_FaceDirection(GARET, FACING_EAST, 20);
	Actor_ShowEmote(11, 0x101, 60);
	Event_OpenMessage(IVAN, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
	Actor_FaceDirection(MIA, FACING_NORTH, 0);
	flag = 0;
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		FieldScene_CallPairWith10(2);
		flag = 1;
	}
	else
	{
		gEventWork->message++;
		Engine_ActorSetAnimationAndWait(IVAN, 3);
		SceneState_ForwardMaskedHalfwordWith10(2, 0xc000);
		FieldScene_CallPairWith10(2);
	}
	if (flag != 0)
	{
		gEventWork->message++;
	}
	Actor_FaceDirection(8, FACING_WEST, 0);
	Actor_FaceDirection(9, FACING_EAST, 0);
	Actor_FaceDirection(10, 0x3000, 0);
	Actor_FaceDirection(11, 0xb000, 20);
	Actor_ShowEmote(8, 0x105, 0);
	Actor_ShowEmote(9, 0x105, 0);
	Actor_ShowEmote(10, 0x105, 0);
	Actor_ShowEmote(11, 0x105, 60);
	Engine_ActorRunRepeatedMotion(8, 1);
	SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
	FieldScene_CallPairWith10(8);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GARET, FACING_NORTH, 0);
	Actor_FaceDirection(IVAN, FACING_NORTH, 0);
	Actor_FaceDirection(MIA, FACING_NORTH, 20);
	Engine_ActorRunRepeatedMotion(9, 1);
	SceneState_ForwardMaskedHalfwordWith10(9, 0x3000);
	FieldScene_CallPairWith10(9);
	Engine_ActorRunRepeatedMotion(10, 1);
	SceneState_ForwardMaskedHalfwordWith10(10, 0x5000);
	FieldScene_CallPairWith10(10);
	SceneState_ForwardMaskedHalfwordWith10(11, 0x8000);
	Engine_ActorSetAnimationAndWait(11, 3);
	FieldScene_CallPairWith10(11);
	Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
	Engine_ActorSetAnimation(GARET, 3);
	Engine_ActorSetAnimation(IVAN, 3);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Actor_SetSpeed(GARET, 0x10000, 0x8000);
	Actor_SetSpeed(IVAN, 0x10000, 0x8000);
	Actor_SetSpeed(MIA, 0x10000, 0x8000);
	Engine_ActorEnableActionCallback(GARET, (s32)ColosseumEntrance_ActionTable3);
	Engine_ActorEnableActionCallback(IVAN, (s32)ColosseumEntrance_ActionTable3);
	Object_SetActionCallbackAndRefreshById(3, (s32)ColosseumEntrance_ActionTable3);
	Engine_EventWait(20);
	SceneState_ForwardMaskedHalfwordWith10(0, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(11, 3);
	Actor_SetSpeed(11, 0x10000, 0x8000);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Engine_ActorSetAnimation(11, 2);
	Actor_MoveToAndWait(11, 830, 152);
	Actor_MoveToAndWait(11, 808, 164);
	Actor_SetDestination(11, 808, 312);
	Engine_EventWait(20);
	Camera_SetSpeed(0x6666, 0xccc);
	Camera_MoveTo(0x3280000, -1, 0x1380000, 1);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 312);
	Engine_EventWait(60);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	gEventWork->transitionFrames = 40;
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(64);
}

static void ColosseumEntrance_EscortToEastExit(void)
{
	struct FieldActor *record;

	if (GameFlag_IsSet(5) != 0)
	{
		GameFlag_Set(0x16d);
		Party_RemoveActiveOwner(5);
		Party_AddActiveOwner(3);
	}
	Engine_EventBegin();
	Actor_SetPosition(11, 0x2c80000, 0x24c0000);
	Engine_TaskWait(1);
	Engine_CameraFollowActor(11, 1);
	Actor_SetSpeed(11, 0x19999, 0xcccc);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
	record = Object_GetById(11);
	record->facing = 0;
	Engine_EventOpenScreen();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Engine_ActorSetAnimation(11, 2);
	Actor_SetDestination(ACTOR_PARTY_LEADER, 0x30c, 0x24c);
	Actor_MoveToAndWait(11, 0x32c, 0x24c);
	Actor_SetDestination(ACTOR_PARTY_LEADER, 0x370, 0x24c);
	Actor_MoveToAndWait(11, 0x390, 0x24c);
	Actor_SetDestination(ACTOR_PARTY_LEADER, 0x3d4, 0x24c);
	Actor_SetDestination(11, 0x3f4, 0x24c);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	if (GameFlag_IsSet(0x90f) != 0)
	{
		Engine_EventRequestExit(31);
	}
	else
	{
		Engine_EventRequestExit(65);
	}
}

static void ColosseumEntrance_AnnounceFinals(void)
{
	struct FieldActor *rec7;

	rec7 = Object_GetById(13);
	Engine_EventBegin();
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_ActorStop(13);
	Engine_TaskWait(1);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	Engine_ActorSetAnimation(13, 1);
	Actor_FaceDirection(12, 0xd000, 0);
	Actor_FaceDirection(13, FACING_EAST, 0);
	Actor_FaceDirection(14, FACING_WEST, 0);
	Actor_FaceDirection(15, 0xd000, 0);
	Actor_FaceDirection(16, FACING_WEST, 0);
	Actor_FaceDirection(17, 0xb000, 0);
	Actor_FaceDirection(18, 0xb000, 0);
	Engine_EventSetMessage(MsgColosseumPrepareYourselvesContestantsFinalsWill);
	FieldScene_CallPairWith10(8);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	{
		u16 *target = (u16 *)&rec7->timer;
		u16	 shown = 0x2d0;

		*target = shown;
	}
	{
		u16 *target = (u16 *)&rec7->work;
		u16	 shown = 112;

		*target = shown;
	}
	Engine_ActorEnableActionCallback(13, 2);
	Actor_FaceDirection(12, 0x3000, 0);
	Actor_FaceDirection(14, 0xb000, 0);
	Actor_FaceDirection(15, 0x5000, 0);
	Actor_FaceDirection(16, FACING_EAST, 0);
	Actor_FaceDirection(17, 0x5000, 0);
	Actor_FaceDirection(18, 0x5000, 0);
	Engine_EventEnd();
}

static void ColosseumEntrance_WalkToEastExit(void)
{
	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
	Engine_EventOpenScreen();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x30c, 0x1ac);
	Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x370, 0x1ac);
	Actor_SetDestination(ACTOR_PARTY_LEADER, 0x3d4, 0x1ac);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	if (GameFlag_IsSet(0x90f) != 0)
	{
		Engine_EventRequestExit(32);
	}
	else
	{
		Engine_EventRequestExit(12);
	}
}

static void ColosseumEntrance_CelebrateAdvancement(void)
{
	s32 actions;

	Engine_EventBegin();
	Actor_SetPosition(GARET, 0x3180000, 0x880000);
	Actor_SetPosition(IVAN, 0x3380000, 0x880000);
	Actor_SetPosition(MIA, 0x3280000, 0x980000);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(8, 1);
	Engine_ActorSetAnimation(8, 3);
	Engine_EventSetMessage(MsgColosseumSeeYouveMadeThroughYour);
	FieldScene_CallPairWith10(8);
	Engine_ActorRunRepeatedMotion(9, 1);
	FieldScene_CallPairWith10(9);
	Engine_ActorRunRepeatedMotion(10, 1);
	FieldScene_CallPairWith10(10);
	Engine_ActorRunRepeatedMotion(11, 1);
	Engine_ActorSetAnimation(11, 3);
	FieldScene_CallPairWith10(11);
	Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
	Actor_SetSpeed(GARET, 0x10000, 0x8000);
	Actor_SetSpeed(IVAN, 0x10000, 0x8000);
	Actor_SetSpeed(MIA, 0x10000, 0x8000);
	actions = (s32)ColosseumEntrance_ActionTable3;
	Engine_ActorEnableActionCallback(GARET, actions);
	Engine_ActorEnableActionCallback(2, actions);
	Object_SetActionCallbackAndRefreshById(3, actions);
	Engine_EventWait(20);
	SceneState_ForwardMaskedHalfwordWith10(0, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(11, 3);
	Actor_SetSpeed(11, 0x10000, 0x8000);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Engine_ActorSetAnimation(11, 2);
	Actor_MoveToAndWait(11, 0x33e, 152);
	Actor_MoveToAndWait(11, 0x328, 164);
	Actor_SetDestination(11, 0x328, 0x138);
	Engine_EventWait(20);
	Camera_SetSpeed(0x6666, 0xccc);
	Camera_MoveTo(0x3280000, -1, 0x1380000, 1);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x328, 164);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x328, 0x138);
	Engine_EventWait(60);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(67);
}

static void ColosseumEntrance_EscortToWestExit(void)
{
	struct FieldActor *record;

	if (GameFlag_IsSet(5) != 0)
	{
		GameFlag_Set(0x16d);
		Party_RemoveActiveOwner(5);
		Party_AddActiveOwner(3);
	}
	Engine_EventBegin();
	Actor_SetPosition(11, 0x3640000, 0x24c0000);
	Engine_TaskWait(1);
	Engine_CameraFollowActor(11, 1);
	Actor_SetSpeed(11, 0x19999, 0xcccc);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
	record = Object_GetById(11);
	record->facing = 0x8000;
	Engine_EventOpenScreen();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Engine_ActorSetAnimation(11, 2);
	Actor_SetDestination(ACTOR_PARTY_LEADER, 0x320, 0x24c);
	Actor_MoveToAndWait(11, 0x300, 0x24c);
	Actor_SetDestination(ACTOR_PARTY_LEADER, 0x2bc, 0x24c);
	Actor_MoveToAndWait(11, 0x29c, 0x24c);
	Actor_SetDestination(ACTOR_PARTY_LEADER, 0x258, 0x24c);
	Actor_SetDestination(11, 0x238, 0x24c);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(21);
}

static void ColosseumEntrance_WalkToWestExit(void)
{
	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
	Engine_EventOpenScreen();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x320, 0x1ac);
	Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x2bc, 0x1ac);
	Actor_SetDestination(ACTOR_PARTY_LEADER, 0x258, 0x1ac);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(22);
}

static void ColosseumEntrance_CelebrateBothWins(void)
{
	s32 actions;

	Engine_EventBegin();
	Actor_SetPosition(GARET, 0x3180000, 0x880000);
	Actor_SetPosition(IVAN, 0x3380000, 0x880000);
	Actor_SetPosition(MIA, 0x3280000, 0x980000);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(8, 1);
	Engine_ActorSetAnimation(8, 3);
	Engine_EventSetMessage(MsgColosseumWonBothMatches);
	FieldScene_CallPairWith10(8);
	Engine_ActorRunRepeatedMotion(9, 1);
	FieldScene_CallPairWith10(9);
	Engine_ActorRunRepeatedMotion(10, 1);
	FieldScene_CallPairWith10(10);
	Engine_ActorRunRepeatedMotion(11, 1);
	Engine_ActorSetAnimation(11, 3);
	FieldScene_CallPairWith10(11);
	Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
	Actor_SetSpeed(GARET, 0x10000, 0x8000);
	Actor_SetSpeed(IVAN, 0x10000, 0x8000);
	Actor_SetSpeed(MIA, 0x10000, 0x8000);
	actions = (s32)ColosseumEntrance_ActionTable3;
	Engine_ActorEnableActionCallback(GARET, actions);
	Engine_ActorEnableActionCallback(2, actions);
	Object_SetActionCallbackAndRefreshById(3, actions);
	Engine_EventWait(20);
	SceneState_ForwardMaskedHalfwordWith10(0, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(11, 3);
	Actor_SetSpeed(11, 0x10000, 0x8000);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Engine_ActorSetAnimation(11, 2);
	Actor_MoveToAndWait(11, 0x33e, 152);
	Actor_MoveToAndWait(11, 0x328, 164);
	Actor_SetDestination(11, 0x328, 0x138);
	Engine_EventWait(20);
	Camera_SetSpeed(0x6666, 0xccc);
	Camera_MoveTo(0x3280000, -1, 0x1380000, 1);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x328, 164);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x328, 0x138);
	Engine_EventWait(60);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(64);
}

static void ActorPresentation_SetActorsTwelveToEighteen(void)
{
	Engine_ActorSetChildValue(12, 3);
	Engine_ActorSetChildValue(13, 0);
	Engine_ActorSetChildValue(14, 4);
	Engine_ActorSetChildValue(15, 1);
	Engine_ActorSetChildValue(16, 5);
	Engine_ActorSetChildValue(17, 2);
	Engine_ActorSetChildValue(18, 6);
	Object_SetActionById(13, 10);
	Object_SetActionById(14, 20);
	Engine_ActorSetAnimation(15, 0);
	Object_SetActionById(16, 40);
	Object_SetActionById(17, 50);
	Object_SetActionById(18, 60);
}

s32 ColosseumEntrance_ActionTable1[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(262), 0, PIXELS(188)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_END,
};

s32 ColosseumEntrance_ActionTable2[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(246), 0, PIXELS(200)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_END,
};

s32 ColosseumEntrance_ActionTable3[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(808), 0, PIXELS(120)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_END,
};

s32 ColosseumEntrance_ActionTable4[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(268), 0, PIXELS(146)),
	SC_SYNC,
	SC_MOVE(PIXELS(262), 0, PIXELS(24)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 ColosseumEntrance_ActionTable5[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(776), 0, PIXELS(120)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0xd000),
	SC_WAIT(20),
	SC_END,
};

s32 ColosseumEntrance_ActionTable6[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(894), 0, PIXELS(106)),
	SC_SYNC,
	SC_MOVE(PIXELS(882), 0, PIXELS(136)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(136)),
	SC_SYNC,
	SC_MOVE(PIXELS(792), 0, PIXELS(152)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0xd000),
	SC_WAIT(20),
	SC_END,
};

s32 ColosseumEntrance_ActionTable7[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(840), 0, PIXELS(136)),
	SC_SYNC,
	SC_MOVE(PIXELS(824), 0, PIXELS(152)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0xb000),
	SC_WAIT(20),
	SC_END,
};

s32 ColosseumEntrance_ActionTable8[] = {
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x6666),
	SC_ARG(2),
	SC_MOVE(PIXELS(808), 0, PIXELS(88)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(58)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_END,
};

s32 ColosseumEntrance_ActionTable9[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, -0x2000),
	SC_ADD(SW_SCALE_Y, 0x2000),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 2048),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 2048),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gColosseumEntranceEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 72, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 72, 0, 72, FACING_SOUTH, 0, 16, 16, 528, 256, 0 },
	{ 2, CONDITION_ALWAYS, 488, 0, 88, FACING_SOUTH, 0, 16, 16, 528, 256, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gColosseumEntranceEntrances1[] = {
	{ 0, CONDITION_ALWAYS, 56, 0, 296, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 184, 0, 72, FACING_SOUTH, 0, 0, 0, 240, 384, 0 },
	{ 2, CONDITION_ALWAYS, 56, 0, 120, FACING_SOUTH, 0, 0, 0, 240, 384, 0 },
	{ 3, CONDITION_ALWAYS, 120, 0, 176, FACING_SOUTH, 0, 0, 0, 240, 384, 0 },
	{ 4, CONDITION_ALWAYS, 56, 0, 296, FACING_SOUTH, 0, 0, 0, 240, 384, 0 },
	{ 5, CONDITION_ALWAYS, 392, 0, 120, FACING_SOUTH, 0, 272, 0, 656, 304, 0 },
	{ 6, CONDITION_ALWAYS, 520, 0, 72, FACING_SOUTH, 0, 272, 0, 656, 304, 0 },
	{ 7, CONDITION_ALWAYS, 184, 0, 912, FACING_SOUTH, 0, 0, 736, 368, 1024, 0 },
	{ 8, CONDITION_ALWAYS, 808, 0, 296, FACING_NORTH, 0, 656, 0, 960, 320, 0 },
	{ 12, CONDITION_ALWAYS, 784, 0, 108, FACING_SOUTHWEST, 0, 656, 0, 960, 320, 0 },
	{ 21, CONDITION_ALWAYS, 808, 0, 120, FACING_NORTH, 0, 656, 0, 960, 320, 0 },
	{ 31, CONDITION_ALWAYS, 808, 0, 120, FACING_NORTH, 0, 656, 0, 960, 320, 0 },
	{ 64, CONDITION_ALWAYS, 808, 0, 120, FACING_NORTH, 0, 656, 0, 960, 320, 0 },
	{ 65, CONDITION_ALWAYS, 680, 0, 588, FACING_EAST, 0, 520, 490, 1020, 640, 0 },
	{ 66, CONDITION_ALWAYS, 680, 0, 428, FACING_EAST, 0, 520, 330, 1020, 480, 0 },
	{ 67, CONDITION_ALWAYS, 900, 0, 588, FACING_WEST, 0, 520, 490, 1020, 640, 0 },
	{ 68, CONDITION_ALWAYS, 900, 0, 428, FACING_WEST, 0, 520, 330, 1020, 480, 0 },
	{ 69, CONDITION_ALWAYS, 440, 0, 120, FACING_SOUTHEAST, 0, 272, 0, 656, 304, 0 },
	{ 70, CONDITION_ALWAYS, 184, 0, 912, FACING_SOUTH, 0, 0, 736, 368, 1024, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gColosseumEntranceEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 96, 0, 288, FACING_NORTH, 0, 0, 0, 512, 320, 0 },
	{ 2, CONDITION_ALWAYS, 272, 0, 88, FACING_SOUTH, 0, 0, 0, 512, 320, 0 },
	{ SCENE_TABLE_END },
};

u32 gColosseumEntranceExits[] = {
	SCENE_EXITS(141),
	SCENE_EXIT(1, 138, 13),
	SCENE_EXIT(2, 140, 4),
	SCENE_EXIT(0, 140, 0),
	SCENE_EXIT(1, 140, 6),
	SCENE_EXIT(2, 140, 5),
	SCENE_EXIT(3, 140, 1),
	SCENE_EXIT(4, 141, 2),
	SCENE_EXIT(5, 140, 2),
	SCENE_EXIT(6, 140, 1),
	SCENE_EXIT(7, 140, 7),
	SCENE_EXIT(69, 140, 69),
	SCENE_EXIT(70, 140, 70),
	SCENE_EXIT(64, 140, 65),
	SCENE_EXIT(65, 143, 3),
	SCENE_EXIT(66, 140, 66),
	SCENE_EXIT(67, 140, 67),
	SCENE_EXIT(12, 143, 2),
	SCENE_EXIT(21, 144, 3),
	SCENE_EXIT(22, 144, 2),
	SCENE_EXIT(31, 145, 3),
	SCENE_EXIT(32, 145, 2),
	SCENE_EXIT(0, 142, 0),
	SCENE_EXIT(1, 135, 12),
	SCENE_EXIT(2, 140, 64),
	SCENE_EXITS_END,
};

struct ScenePlacement gColosseumEntrancePlacementsOther[] = {
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(408), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements1Entrance5[] = {
	{ 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(496), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(592), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(408), 0, PIXELS(88), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x39, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(528), 0, PIXELS(88), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(184), FACING_SOUTHEAST, TALK_KEEP_FACING, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(432), 0, PIXELS(96), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(476), 0, PIXELS(96), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(168), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x72, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(454), 0, PIXELS(104), FACING_SOUTHEAST, TALK_KEEP_FACING, 0 },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(208), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(444), 0, PIXELS(72), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(224), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(200), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(224), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(168), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(216), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(428), 0, PIXELS(72), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(240), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(192), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(296), 0, PIXELS(200), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(608), 0, PIXELS(104), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements1Entrance7[] = {
	{ 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(250), 0, PIXELS(846), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(298), 0, PIXELS(912), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(250), 0, PIXELS(974), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(121), 0, PIXELS(974), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(912), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(121), 0, PIXELS(846), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(96), 0, PIXELS(900), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(114), 0, PIXELS(884), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(872), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(148), 0, PIXELS(864), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(164), 0, PIXELS(860), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(212), 0, PIXELS(872), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(888), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xfd, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(184), 0, PIXELS(902), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements1[] = {
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(72), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(72), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(128), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements1Entrance8[] = {
	{ GARET, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ IVAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ MIA, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(808), 0, PIXELS(88), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(782), 0, PIXELS(96), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(832), 0, PIXELS(96), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(856), 0, PIXELS(120), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements1Entrance12[] = {
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(808), 0, PIXELS(72), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(808), 0, PIXELS(234), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(778), 0, PIXELS(72), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(840), 0, PIXELS(72), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(760), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(720), 0, PIXELS(112), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(872), 0, PIXELS(88), FACING_NORTHWEST + FACING_STEP, 3, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(808), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(920), 0, PIXELS(134), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(840), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(856), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements1Entrance66[] = {
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(536), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(632), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(728), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(824), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(920), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1016), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements3[] = {
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(168), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(198), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(266), 0, PIXELS(144), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(278), 0, PIXELS(152), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(74), 0, PIXELS(196), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(80), 0, PIXELS(238), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(198), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(180), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(180), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(48), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(88), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x72, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(80), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x72, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(56), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x9f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(140), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x9f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(64), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xa9, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(72), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements3Flag962[] = {
	{ GARET, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ IVAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ MIA, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(182), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(294), 0, PIXELS(178), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(228), 0, PIXELS(192), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(198), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(74), 0, PIXELS(196), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(438), 0, PIXELS(186), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(424), 0, PIXELS(186), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(180), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(180), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(258), 0, PIXELS(161), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(244), 0, PIXELS(150), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(161), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(168), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(278), 0, PIXELS(52), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(278), 0, PIXELS(70), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x72, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(278), 0, PIXELS(88), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x72, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(278), 0, PIXELS(106), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x9f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(278), 0, PIXELS(124), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x9f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(278), 0, PIXELS(142), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xa9, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(160), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements3Flag950[] = {
	{ 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(80), 0, PIXELS(238), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(286), 0, PIXELS(164), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gColosseumEntranceEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gColosseumEntranceEvents2[] = {
	{ 0x21, 1, CONDITION_ALWAYS, 1 },
	{ 0x21, 2, CONDITION_ALWAYS, 2 },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gColosseumEntranceEvents1[] = {
	{ 0x21, 1, CONDITION_ALWAYS, 1 },
	{ 0x21, 2, CONDITION_ALWAYS, 2 },
	{ 0x31, 4, CONDITION_ALWAYS, 4 },
	{ 0x31, 5, CONDITION_ALWAYS, 5 },
	{ 0x31, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceGoForItThatsItIm) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWatchingTheTrialsMakesMeReally) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWatchingSuchProudSkilledWarriorsIn) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceIveWaitedAWholeYearTo) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_TALK_1) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceEvenTheSmallerWarriorsCanWin) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheseTrialsAreBasedOnPersonal) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhatFantasticPhysiquesWhyIfI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgColossoDidntThinkBattles) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceNoMatterHowGoodAWarrior) },
	{ SEARCH(SEARCH_UNNAMED), 50, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianItIsLocked)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gColosseumEntranceEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TALK, 8, 0x962, MSGNO(MsgColosseumEntranceTheyreHoldingThePreliminaryColossoHeats) },
	{ EVENT_TALK, 8, 0x950, MSGNO(MsgColosseumEntranceTheFinalsAreFinallyHereI) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceColossoIsOverAndTheColosseum) },
	{ EVENT_TALK, 9, 0x962, MSGNO(MsgColosseumEntranceSighTheColosseumIsPackedWith) },
	{ EVENT_TALK, 9, 0x950, MSGNO(MsgColosseumEntranceIHeardBabiHasPickedSome) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheColosseumIsClosedUntilNext) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK) },
	{ EVENT_TALK, 11, 0x962, MSGNO(MsgColosseumEntranceIfYouHitItBigIn) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceAWarriorNamedIsaacWasSelected) },
	{ EVENT_TALK, 12, 0x962, MSGNO(MsgColosseumEntranceIveBeenBettingOnTheLucky) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceHavingAWarriorWhoWasntIn) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK_3) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK_3) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TALK_3) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x962, MSGNO(MsgColosseumEntranceIWonderWhichWarriorWillBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x950, MSGNO(MsgColosseumEntranceTheRealBattlesAreAboutTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhenTheColosseumGatesFinallyShut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x962, MSGNO(MsgColosseumEntranceHavingToArriveSoEarlyJust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x950, MSGNO(MsgColosseumEntranceTheWarriorNamedIsaacIsSure) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheFightingAtThisYearsColosso) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x962, MSGNO(MsgColosseumEntranceThereAreSoManyPeopleWho) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceOfCourseItsTheRichWho) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x962, MSGNO(MsgColosseumEntranceImGoingToPickTheBiggest) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceThereCanBeUpsetsInThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x962, MSGNO(MsgColosseumEntranceImFeelingConfidentThisTimeIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhatKindOfWarriorIsThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x962, MSGNO(MsgColosseumEntranceEveryoneKeepsRiskingMoreAndMore) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceMaybeIWillBuyASeat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x962, MSGNO(MsgColosseumEntranceEveryYearIGoAfterA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceImGoingToBeInTrouble) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x962, MSGNO(MsgColosseumEntranceOnceTheTrialsAreOverIts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceAllOfTheProfitsFromThe) },
	{ EVENT_TOUCH, 10, 0x950, EVENT_SCRIPT(EV10_TOUCH) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gColosseumEntranceEvents1Entrance12[] = {
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceIAmMorganIAmThe) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TALK) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_TALK) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceYouCannotLeaveUntilTheFinals) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceIFeelWeveBeenWaitingForever) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhenTheGongSoundsTheFinals) },
	{ EVENT_TOUCH, 12, 0x235, EVENT_SCRIPT(EV12_TOUCH) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TOUCH) },
	{ SCENE_EVENTS_END },
};
