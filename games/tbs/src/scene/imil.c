#include "game_state.h"
#include "keystate.h"
#include "scene_ids.h"
#include "unit/scene/imil.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "text/msg_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
#include "facing_object.h"
#include "battle_effect_runtime.h"
#include "object_lookup.h"
#include "field_service.h"
#include "iwram_call.h"
#include "frame.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

// 前方宣言
static void FieldScene_RunPrimaryScriptChoreography(void);
static void Imil_CastPsynergyAtActorNine(void);
static void FieldScene_RunScene399SequenceA(void);
static void SceneState_UpdateZoneFlagsFromActorZero(void);
static void EV11_PSYNERGY(void);
static void RunEventScript02(void);
static void FieldScene_RunThreeActorChoreography(void);
static void StagedActor_RunHeadingProbeStep(void);

#define ACTOR_FLAGS_OFFSET 90

typedef struct
{
	u8	filler0[6];
	u16 unk6;
	u8	filler8[82];
	u8	unk5A;
	u8	filler5B[9];
	u16 unk64;
} T;

typedef struct
{
	u8 filler0[23];
	u8 unk17;
} State;

struct SelectedSubject
{
	u8	pad000[500];
	s32 selectedSubject;
};

// 表を符号付き半語で読む見方
extern s32 gImil_ArcOrigin[];

static s32 OverlayObject_UpdateWobbleByCounter(struct FieldActor *obj)
{
	switch (obj->timer)
	{
	case 6:
		obj->scaleX += 0xffffc000;
		obj->scaleY += 0x2000;
		break;
	case 4:
		obj->scaleX += 0x2000;
		obj->scaleY -= 0x1000;
		break;
	case 2:
		obj->scaleX += 0x1000;
		obj->scaleY += 0xfffff800;
		break;
	case 0:
		obj->scaleX += 0x1000;
		obj->scaleY += 0xfffff800;
		if ((s16)obj->work != 0)
		{
			obj->timer = (u32)Engine_RandomNext() % 40 + 40;
		}
		else
		{
			obj->timer = (u32)Engine_RandomNext() % 20 + 20;
		}
		break;
	}
	obj->timer--;
	return 1;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/object_facing.h"

const struct SceneEntrance *Scene_GetEntrances(void)
{
	if (gGameState.scene == (s32)&SceneId_Imil2)
	{
		return gImilEntrances2;
	}
	return gImilEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *Imil_GetExits(void)
{
	return (u8 *)gImil_SceneTable;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	u8 *script;

	if (gGameState.scene == ((s32)&SceneId_Imil2))
	{
		script = gImilPlacements2;
		FieldScene_PrepareActors(script);
		if (Engine_GameFlagIsSet(0x881) != 0)
		{
			script[262] = 0;
			*(s32 *)(script + 0x50) = 182 << 16;
			*(s32 *)(script + 0x58) = 564 << 16;
			*(s32 *)(script + 0x4c) = 2;
		}
		return (const struct ScenePlacement *)script;
	}

	if (Engine_GameFlagIsSet(0x881) != 0)
	{
		return gImilPlacementsFlag881;
	}
	return gImilPlacements;
}

static void EV90_TOUCH(void)
{
	StagedActor_RunHeadingProbeStep();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	if (gGameState.scene == (s32)&SceneId_Imil2)
	{
		return gImilEvents2;
	}
	return gImilEventsOther;
}

static void EV08_TALK(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(3) != 0)
	{
		Engine_EventSetMessage(MSGNO(MsgImilEvenIfMiaHealsUs));
	}
	else
	{
		Engine_EventSetMessage(MSGNO(MsgImilBrrrrrChooImFreezingMia));
	}
	{
		s32 val = 0;
		s32 mode = 8;
		Engine_EventShowMessage(mode, val);
	}
	Engine_EventEnd();
}

static void EV08_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(3) != 0)
	{
		Engine_EventSetMessage(MSGNO(MsgImilCantUnderstandWhyAnyOne));
	}
	else
	{
		Engine_EventSetMessage(MSGNO(MsgImilMiaShouldHereByNow));
	}
	Engine_EventShowMessage(8, 0);
	Engine_EventEnd();
}

static void EV09_TALK(void)
{
	Engine_EventBegin();
	Engine_ActorFaceActor(9, ACTOR_PARTY_LEADER, 10);
	Engine_EventSetMessage(MSGNO(MsgImilHiNewInImil));
	Engine_EventAskYesNo(9, 0);
	Engine_EventEnd();
}

static void EV10_TALK(void)
{
	Engine_EventBegin();

	if (GameFlag_IsSet(0x881) != 0)
	{
		Engine_EventSetMessage(MSGNO(MsgImilCountingAfterDeparture));
		Event_ShowMessage(10, 0);
		Actor_SetAttachedEffect(10, 258);
		Engine_EventWait(40);
		Engine_ActorSetAnimation(10, 1);
		Engine_EventWait(20);
		Actor_FaceActor(10, ACTOR_PARTY_LEADER, 20);
		Event_AskYesNo(10, 0);
		Actor_FaceDirection(10, 0x3000, 10);
		Engine_ActorSetAnimation(10, 9);
	}
	else
	{
		Engine_EventSetMessage(MSGNO(MsgImilOneTwoThreeFour));
		Event_ShowMessage(10, 0);
		Actor_SetAttachedEffect(10, 258);
		Engine_EventWait(40);
		Engine_ActorSetAnimation(10, 1);
		Engine_EventWait(20);
		Actor_FaceActor(10, ACTOR_PARTY_LEADER, 20);
		Event_ShowMessage(10, 0);
		Actor_FaceDirection(10, 0x3000, 10);
		Engine_ActorSetAnimation(10, 9);
	}

	Engine_EventEnd();
}

static void FieldScene_RunSupplementalSequenceOne(void)
{

	struct FieldActor *actor9_record;
	struct FieldActor *actor8_record;

	if (GameFlag_IsSet(2177) != 0)
	{
		Engine_EventBegin();
		Actor_FaceActor(9, 0, 0);
		Engine_EventWait(10);
		Engine_EventSetMessage(MsgMercuryReallySayDie);
		Event_AskYesNo(9, 0);
		Engine_EventEnd();
	}
	else
	{
		if (GameFlag_IsSet(2091) != 0)
		{
			Engine_EventBegin();
			Engine_ActorSetAnimation(9, 7);
			Engine_MapAnimateCells((s32)gImil_CellStepsA, 10, 69);
			Engine_EventSetMessage(MsgMercuryFeelMuchBetter);
			Event_ShowMessage(9, 0);
			Engine_ActorSetAnimation(9, 8);
			Map_AnimateCells((s32)gImil_CellStepsB, 10, 69);
			Engine_EventEnd();
		}
		else
		{
			Engine_EventBegin();
			actor9_record = Object_GetById(9);
			actor9_record->timer = 10;
			Engine_ActorEnableActionCallback(9, gImil_ActorScriptA);
			Engine_EventSetMessage(MsgMercuryOoohHelpMe);
			Engine_EventShowMessage(9, 0);
			Engine_ActorStop(8);
			Actor_ShowEmote(8, 256, 40);
			Actor_FaceDirection(8, 53248, 10);
			Engine_ActorStartRepeatedMotion(8, 2);
			Event_ShowMessageAndWait(8, 0, 20);
			Engine_ActorEnableActionCallback(0, gImil_ActorScriptC);
			Actor_SetSpeed(8, 104857, 52428);
			Object_SetActionCallbackAndRefreshById(8, (s32)gImil_ActorScriptB);
			Engine_EventWait(40);
			Engine_ActorJump(8, 2, 0);
			Engine_ActorStartRepeatedMotion(8, 2);
			Engine_ActorSetAttachedEffect(8, 258);
			Engine_EventWait(60);
			Event_ShowMessageAndWait(8, 0, 10);
			Actor_FaceDirection(8, 12288, 20);
			Engine_ActorStartRepeatedMotion(8, 2);
			Engine_EventShowMessage(8, 0);
			actor8_record = Object_GetById(8);
			actor8_record->collisionFlags ^= 0x2;
			GameFlag_Set(0x82c);
			Engine_EventEnd();
		}
	}
}

static void EV12_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage(MSGNO(MsgImilDoWantWeapons));
	Engine_EventAskYesNo(12, 0);
	Engine_EventEnd();
}

static void EV18_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage(MSGNO(MsgImilDoWantSeeRestaurantMenu));
	Engine_EventAskYesNo(18, 0);
	Engine_EventEnd();
}

static void EV20_TALK(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(3) != 0)
	{
		Engine_EventSetMessage(MSGNO(MsgImilEveryoneCountsOnMiaThats));
		Engine_EventShowMessage(20, 0);
	}
	else
	{
		Engine_EventSetMessage(MsgImilHaveVisitedOldCoupleWho);
		Engine_EventAskYesNo(20, 0);
		Engine_GameFlagSet(0x82a);
		Engine_GameFlagSet(0x82c);
	}
	Engine_EventEnd();
}

static void EV20_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(3) != 0)
	{
		Engine_EventSetMessage(MSGNO(MsgImilWeHaveDoWhateverWe));
	}
	else
	{
		Engine_EventSetMessage(MSGNO(MsgImilMiaWasSayingSheHas));
	}
	Engine_EventShowMessage(20, 0);
	Engine_EventEnd();
}

static void EV08_TALK_2(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage(MSGNO(MsgImilHappenedInLighthouseNortheast));
	Event_ShowMessage(8, 0);
	Actor_FaceDirection(8, 0x3000, 10);
	Engine_EventEnd();
}

static void EV08_PSYNERGY_2(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x82b) != 0)
	{
		Engine_EventSetMessage(MSGNO(MsgImilMiaClanOnceLivedHere));
	}
	else if (Engine_GameFlagIsSet(0x82c) != 0)
	{
		Engine_EventSetMessage(MSGNO(MsgImilHesAlwaysExaggeratingThingsBut));
	}
	else
	{
		Engine_EventSetMessage(MSGNO(MsgImilMiaRunningAroundTownCaring));
	}
	Engine_EventShowMessage(8, 0);
	Engine_EventEnd();
}

static void EV10_TALK_2(void)
{
	FieldScene_RunSupplementalSequenceOne();
}

static void EV10_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage(MSGNO(MsgImilMiaIsAGoodGirlI));
	Engine_EventShowMessage(10, 0);
	Engine_EventEnd();
}

static void EV19_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(3) != 0)
	{
		Engine_EventSetMessage(MSGNO(MsgImilFeelLikeGrownUpWhen));
	}
	else
	{
		Engine_EventSetMessage(MSGNO(MsgImilTheseFolkOkayTheyDont));
	}
	Engine_EventShowMessage(19, 0);
	Engine_EventEnd();
}

static void Imil_RunExitDoor(void)
{
	struct EventWork  *event;
	struct FieldActor *actor;
	u32				   i;
	s32				   exit;

	event = gEventWork;
	Engine_EventBegin();
	for (i = 8; i <= 65; i++)
	{
		actor = Object_GetById(i);
		if (actor != NULL)
		{
			actor->motionFlags = 0;
		}
	}
	exit = (s16)(event->touchedTrigger - 50);
	if (exit == 6)
	{
		Engine_AudioPlayCue(188);
	}
	else
	{
		Engine_AudioPlayCue(158);
	}
	{
		s32 x = gImil_ExitCellPoints[exit - 1][0];
		s32 y = gImil_ExitCellPoints[exit - 1][1];

		Engine_MapAnimateCells(gImil_ExitCellSteps[exit - 1], x, y);
	}
	Actor_SetSpeed(0, 0x8000, 0x4000);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if (exit == 6)
	{
		Actor_SetSpeed(0, 0x3333, 0x1999);
		Engine_ActorSetAnimation(0, 2);
		Engine_ActorSetSpritePriority(0, 3);
		Actor_SetDestinationOffset(0, 0, -8);
	}
	else
	{
		Object_GetById(0)->motionFlags = 0;
		Actor_CenterAndWalk(0, 3, -16);
	}
	Engine_EventWait(16);
	Engine_EventRequestExit(exit);
	Engine_EventEnd();
}

static void Imil_RunWeaponShop(void)
{
	s32 dir;

	dir = *(s16 *)((s32)Object_GetById(0) + 6);
	if (Engine_GameFlagIsSet(0x881) != 0)
	{
		if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000)
		{
			Engine_ShopOpen(10, 12);
			return;
		}
		Engine_EventBegin();
		Engine_ActorFaceActor(12, 0, 0);
		Engine_EventWait(10);
		Engine_EventSetMessage(MsgMercuryCatchUpLostOpportunity);
		Engine_EventShowMessage(12, 0);
		Actor_FaceDirection(12, 0x4000, 10);
		Engine_EventEnd();
	}
	else
	{
		if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000)
		{
			Engine_EventBegin();
			Engine_CameraSetSpeed(0x60000, 0xc000);
			Camera_MoveTo(0x1aa0000, -1, 0x1ec0000, 1);
			Engine_CameraWaitForMove();
			Engine_EventWait(20);
			Object_SetActionCallbackAndRefreshById(12, (s32)gImil_TurnScript);
			Engine_EventSetMessage(MsgMercuryStoreClosedUntilWell);
			Engine_EventShowMessage(12, 0);
			Engine_CameraMoveTo(0x1aa0000, -1, 0x2680000, 1);
			Engine_CameraWaitForMove();
			Engine_EventEnd();
		}
	}
}

static void Imil_RunArmorShop(void)
{
	s32 dir;

	dir = *(s16 *)((s32)Object_GetById(0) + 6);
	if (Engine_GameFlagIsSet(0x881) != 0)
	{
		if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000)
		{
			Engine_ShopOpen(11, 13);
			return;
		}
		Engine_EventBegin();
		Engine_ActorFaceActor(13, 0, 0);
		Engine_EventWait(10);
		Engine_EventSetMessage(MsgMercuryShameStoreClosed);
		Engine_EventShowMessage(13, 0);
		Actor_FaceDirection(13, 0x4000, 10);
		Engine_EventEnd();
	}
	else
	{
		if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000)
		{
			Engine_EventBegin();
			Engine_CameraSetSpeed(0x60000, 0xc000);
			Camera_MoveTo(0x1aa0000, -1, 0x1ec0000, 1);
			Engine_CameraWaitForMove();
			Engine_EventWait(20);
			Object_SetActionCallbackAndRefreshById(13, (s32)gImil_TurnScript);
			Engine_EventSetMessage(MsgMercuryTooSickBusiness);
			Engine_EventShowMessage(13, 0);
			Engine_CameraMoveTo(0x1aa0000, -1, 0x2680000, 1);
			Engine_CameraWaitForMove();
			Engine_EventEnd();
		}
	}
}

static void Imil_RunItemShop(void)
{
	s32 dir;

	dir = *(s16 *)((s32)Object_GetById(0) + 6);
	if (Engine_GameFlagIsSet(0x881) != 0)
	{
		if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000)
		{
			Engine_ShopOpen(12, 15);
			return;
		}
		Engine_EventBegin();
		Engine_ActorFaceActor(15, 0, 0);
		Engine_EventSetMessage(MsgMercuryRecoveredThanksFountain);
		Engine_EventShowMessage(15, 0);
		Actor_FaceDirection(15, 0x4000, 0);
		Engine_EventEnd();
	}
	else
	{
		if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000)
		{
			Engine_EventBegin();
			Engine_EventSetMessage(MsgMercuryMomsSickGetWhatever);
			Engine_EventShowMessage(14, 0);
			/* FAKEMATCH: called through a cast to the value-returning call the bytes show; called by its void prototype, the two argument moves swap (differ 2). A prototyped s32 (*)(s32, s32) cast matches as well (2026-10-13). */
			((s32 (*)())Engine_ShopOpen)(12, 14);
			Engine_EventEnd();
		}
		else
		{
			Engine_ActorFaceActor(14, 0, 10);
			Engine_EventSetMessage(MsgMercuryDadLumberjack);
			Engine_EventShowMessage(14, 0);
			Actor_FaceDirection(14, 0x5000, 10);
		}
	}
}

static void EV16_TALK(void)
{
	struct FieldActor *leader;

	leader = Object_GetById(0);
	if ((u16)(leader->facing + 0x5fff) <= 0x3ffe)
	{
		Engine_InnOpen(4, 16);
	}
	else
	{
		Engine_EventBegin();
		Actor_FaceActor(16, ACTOR_PARTY_LEADER, 10);
		if (GameFlag_IsSet(0x881) != 0)
		{
			Engine_EventSetMessage(MSGNO(MsgImilItsAlmostTimeForLeave));
			Event_AskYesNo(16, 0);
		}
		else
		{
			Engine_EventSetMessage(MSGNO(MsgImilWhyHaveTwoGroupsTravelers));
			Event_ShowMessage(16, 0);
		}
		Actor_FaceDirection(16, 0x3000, 10);
		Engine_EventEnd();
	}
}

static void EV19_TALK(void)
{
	struct FieldActor *leader;

	leader = Object_GetById(0);
	if ((u16)(leader->facing + 0x5fff) <= 0x3ffe)
	{
		Engine_EventBegin();
		if (GameFlag_IsSet(0x82d) == 0)
		{
			Engine_EventSetMessage(MSGNO(MsgImilMayOnlyStudentButCan));
			Event_ShowMessage(19, 0);
			GameFlag_Set(0x82d);
		}
		Engine_EventEnd();
		Engine_SanctumOpen(19);
	}
	else
	{
		Engine_EventBegin();
		if (GameFlag_IsSet(0x881) != 0)
		{
			Engine_EventSetMessage(MSGNO(MsgImilMiaGoingOnJourneyWith));
			Event_ShowMessage(19, 0);
		}
		else if (GameFlag_IsSet(3) != 0)
		{
			Engine_EventSetMessage(MSGNO(MsgImilAmHealerWhileMiaOut));
			Event_ShowMessage(19, 0);
		}
		else
		{
			Engine_EventSetMessage(MSGNO(MsgImilLookingForMia));
			(void)Event_AskYesNo(19, 0);
			Actor_FaceDirection(19, 0x3000, 10);
		}
		Engine_EventEnd();
	}
}

s32 Imil_ApplyEntryState(void)
{
	struct FieldActor *leader;
	s32				   entrance;

	if (gGameState.scene == (s32)&SceneId_Imil1)
	{
		leader = Object_GetById(0);
		gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		Engine_ActorSetAnimation(10, 9);
		if (GameFlag_IsSet(0x109))
		{
			Engine_GameFlagClear(0x200);
			Engine_GameFlagClear(0x201);
		}
		leader->timer = 0;
		leader->work = 0;
		Task_AddCallback(FieldScene_RunScene399SequenceA, 0xc80);
		FieldScheduler_AddCallback(SceneState_UpdateZoneFlagsFromActorZero, 0xc80);
		Engine_ActorSetSpritePriority(11, 1);
		if (Engine_GameFlagIsSet(0x203))
		{
			EV11_PSYNERGY();
		}
		if (!Engine_GameFlagIsSet(0x109) && gGameState.entrance == 9)
		{
			RunEventScript02();
		}
	}
	else if (gGameState.scene == (s32)&SceneId_Imil2)
	{
		gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
		entrance = gGameState.entrance;
		if (entrance == 1)
		{
			Engine_ActorSetChildValue(21, 15);
			Object_GetById(21)->collisionFlags |= 8;
			Engine_ActorSetSpritePriority(21, 1);
			if (Engine_GameFlagIsSet(0x881))
			{
				Map_CopyCellAttributes(10, 7, 1, 1, 10, 8);
				Map_CopyCellsTo(3, 125, 9, 69, 3, 3);
				Engine_MapRedraw();
				Engine_TaskWait(1);
				Engine_ActorEnableActionCallback(8, (const s32 *)2);
				Engine_ActorSetPosition(10, 0, 0);
			}
			else if (Engine_GameFlagIsSet(0x82c) && Engine_GameFlagIsSet(0x82a))
			{
				Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
				Engine_ActorSetPosition(9, 0xae0000, 0xa40000);
				Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
				Engine_ActorSetAnimation(9, 5);
				Actor_SetPosition(8, 0xa80000, 0x980000);
				Object_GetById(8)->facing = 0x3000;
				if (!Engine_GameFlagIsSet(0x82b))
				{
					FieldScene_RunPrimaryScriptChoreography();
				}
			}
			else
			{
				Map_CopyCellAttributes(10, 7, 1, 1, 10, 8);
				Map_CopyCellsTo(3, 125, 9, 69, 3, 3);
				Engine_MapRedraw();
				Engine_TaskWait(1);
				if (Engine_GameFlagIsSet(0x82c))
				{
					Engine_ActorSetPosition(8, 0x950000, 0x740000);
					Object_GetById(8)->facing = 0;
					Object_GetById(9)->work = 0;
					Engine_ActorEnableActionCallback(9, gImil_ActorScriptA);
				}
				else
				{
					Engine_ActorEnableActionCallback(8, (const s32 *)2);
				}
			}
		}
		else if (entrance == 2)
		{
			if (!Engine_GameFlagIsSet(0x881))
			{
				Object_GetById(11)->work = 1;
				Engine_ActorEnableActionCallback(11, gImil_ActorScriptA);
			}
		}
		else if (entrance == 4)
		{
			if (Engine_GameFlagIsSet(0x881))
			{
				Actor_SetPosition(12, 0x16c0000, 0x2420000);
				Engine_ActorSetSpritePriority(12, 2);
				Object_GetById(12)->collisionFlags |= 4;
				Map_CopyCellsTo(6, 125, 22, 88, 3, 3);
				Actor_SetPosition(13, 0x1ec0000, 0x2420000);
				Engine_ActorSetSpritePriority(13, 2);
				Object_GetById(13)->collisionFlags |= 4;
				Map_CopyCellsTo(9, 125, 28, 88, 3, 3);
			}
			else
			{
				Object_GetById(12)->scaleX = -0x10000;
				Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
				Engine_ActorSetAnimation(12, 5);
				Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
				Engine_ActorSetAnimation(13, 5);
			}
		}
		else if (entrance == 3)
		{
			if (Engine_GameFlagIsSet(0x881))
			{
				Actor_SetPosition(15, 0x1cc0000, 0x1020000);
				Engine_ActorSetSpritePriority(15, 2);
				Object_GetById(15)->collisionFlags |= 4;
				Engine_ActorSetPosition(14, 0x1980000, 0x1080000);
				Object_GetById(14)->facing = 0x1000;
				Map_CopyCellsTo(12, 125, 26, 70, 3, 3);
			}
			else
			{
				Actor_SetPosition(14, 0x1cc0000, 0x1020000);
				Engine_ActorSetSpritePriority(14, 2);
				Object_GetById(14)->collisionFlags |= 4;
				Object_GetById(15)->scaleX = -0x10000;
				Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
				Engine_ActorSetAnimation(15, 5);
			}
		}
		else if (entrance == 7)
		{
			if (Engine_GameFlagIsSet(0x881))
			{
				Object_GetById(20)->facing = 0x3000;
				if (!Engine_GameFlagIsSet(0x82e))
				{
					Actor_SetPosition(20, 0x28a0000, 0xa10000);
					FieldScene_RunThreeActorChoreography();
				}
				else
				{
					Actor_SetPosition(20, 0x2840000, 0xa60000);
				}
			}
		}
	}
	return 0;
}

static void FieldScene_RunPrimaryScriptChoreography(void)
{
	struct FieldActor *leader;
	s32				   table;
	struct EventWork  *work;

	Engine_EventBegin();
	Actor_SetPosition(MIA, 0xb60000, 0x960000);
	Camera_MoveTo(0x8d0000, -1, 0xdd0000, 0);
	Engine_TaskWait(1);
	Camera_SetSpeed(0x4ccc, 0x999);
	Camera_MoveTo(0x8c0000, -1, 0xa40000, 1);
	work = gEventWork;
	work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	work->transitionFrames = 40;
	Engine_EventOpenScreen();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
	Actor_SetSpeed(GARET, 0x6666, 0x3333);
	Actor_SetSpeed(IVAN, 0x6666, 0x3333);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 142, 221);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
	leader = Object_GetById(0);
	if (leader != NULL)
	{
		Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
	}
	leader = Object_GetById(0);
	if (leader != NULL)
	{
		Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
	}
	Actor_WalkTo(GARET, 150, 234);
	Actor_WalkToAndWait(IVAN, 134, 234);
	Engine_ActorSetAnimation(GARET, 1);
	FieldObject_SetTargetAndCallback(0, 0x10003, gImil_PrimaryScript);
	FieldObject_SetTargetAndCallback(1, 0x10003, gImil_PrimaryScript);
	FieldObject_SetTargetAndCallback(2, 0x10003, gImil_PrimaryScript);
	Engine_CameraWaitForMove();
	table = (s32)gImil_TurnScript;
	Engine_ActorEnableActionCallback(9, table);
	Engine_EventWait(40);
	Actor_SetAttachedEffect(MIA, 0x102);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(MIA, 1);
	Engine_EventSetMessage(MSGNO(MsgImilHowFeeling));
	Event_ShowMessageAndWait(MIA, 0, 20);
	Engine_ActorEnableActionCallback(9, table);
	Event_ShowMessageAndWait(9, 0, 20);
	Actor_FaceDirection(MIA, 0x8000, 20);
	Actor_FaceDirection(8, 0, 10);
	Engine_ActorSetAnimationAndWait(8, 4);
	Event_ShowMessageAndWait(8, 0, 40);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_EventWait(10);
	Actor_FaceDirection(MIA, 0x4000, 0);
	Actor_FaceDirection(8, 0x3000, 20);
	Event_ShowMessageAndWait(MIA, 0, 10);
	Engine_ActorEnableActionCallback(9, table);

	Imil_CastPsynergyAtActorNine();
	Actor_ShowEmote(MIA, 0x101, 60);
	Event_ShowMessageAndWait(MIA, 0, 40);
	Event_ShowMessageAndWait(9, 0, 20);
	Actor_ShowEmote(8, 0x105, 60);
	Engine_ActorSetAnimation(9, 7);
	Map_AnimateCells((s32)gImil_CellStepsA, 10, 69);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Engine_ActorSetAnimationAndWait(MIA, 4);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Engine_ActorRunRepeatedMotion(9, 1);
	Engine_EventWait(40);
	Engine_ActorSetAnimation(9, 8);
	Map_AnimateCells((s32)gImil_CellStepsB, 10, 69);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(8, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Event_ShowMessageAndWait(8, 0, 10);
	Actor_ShowEmote(MIA, 0x101, 30);
	Actor_FaceDirection(MIA, 0x8000, 10);
	Engine_ActorSetAnimation(MIA, 4);
	Event_ShowMessageAndWait(MIA, 0, 10);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_EventWait(40);
	Actor_SetSpeed(MIA, 0x10000, 0x8000);
	Object_GetById(3)->timer = 0;
	Engine_ActorEnableActionCallback(MIA, gImil_MiaScriptA);
	while (Object_GetById(3)->timer == 0)
	{
		Engine_TaskWait(1);
	}
	Camera_MoveTo(0x8c0000, -1, 0xc60000, 1);
	Object_RefreshSelectorById(3);
	Actor_ShowEmote(MIA, 0x101, 80);
	Event_ShowMessageAndWait(MIA, 0, 40);
	Engine_ActorRunRepeatedMotion(MIA, 1);
	Engine_EventWait(10);
	Event_ShowMessage(MIA, 0);
	Event_PlayCue(131);
	ColorBuffer_ApplySource(0x10000, 0);
	ColorBuffer_ApplyTarget(0x207e9f, 0);
	Engine_ColorBufferInterpolate(10);
	Engine_TaskWait(1);
	Event_PlayCue(220);
	Engine_TaskWait(40);
	ColorBuffer_ApplyTarget(0x10000, 0);
	Engine_ColorBufferInterpolate(60);
	Engine_TaskWait(60);
	Actor_SetAttachedEffect(MIA, 0x102);
	Engine_EventWait(20);
	Actor_FaceDirection(MIA, 0, 10);
	Actor_SetSpeed(MIA, 0x20000, 0x10000);
	Actor_WalkToAndWait(MIA, 202, 198);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Event_ShowMessage(MIA, 0);
	Engine_ActorSetAnimationAndWait(MIA, 4);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Actor_SetAttachedEffect(MIA, 0x102);
	Engine_EventWait(40);
	Event_ShowMessageAndWait(MIA, 0, 40);
	Actor_ShowEmote(MIA, 0x100, 40);
	Event_ShowMessage(MIA, 0);
	Engine_ActorStop(ACTOR_PARTY_LEADER);
	Engine_ActorStop(GARET);
	Engine_ActorStop(IVAN);
	Actor_SetSpeed(MIA, 0x30000, 0x18000);
	Object_GetById(3)->timer = 0;
	Engine_ActorEnableActionCallback(MIA, gImil_MiaScriptB);
	while (Object_GetById(3)->timer == 0)
	{
		Engine_TaskWait(1);
	}
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
	Actor_FaceDirection(GARET, 0x4000, 0);
	Actor_FaceDirection(IVAN, 0x4000, 10);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x40000, 0x20000);
	Actor_SetSpeed(GARET, 0x40000, 0x20000);
	Actor_SetSpeed(IVAN, 0x40000, 0x20000);
	Event_PlayCue(152);
	Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
	Object_GetById(1)->steeringFlags &= ~ACTOR_FACES_MOTION;
	Object_GetById(2)->steeringFlags &= ~ACTOR_FACES_MOTION;
	Actor_SetDestination(ACTOR_PARTY_LEADER, 132, 206);
	Actor_SetDestination(GARET, 136, 221);
	Actor_SetDestination(IVAN, 122, 238);
	Object_RefreshSelectorById(3);
	Engine_EventWait(80);
	Object_GetById(0)->steeringFlags |= ACTOR_FACES_MOTION;
	Object_GetById(1)->steeringFlags |= ACTOR_FACES_MOTION;
	Object_GetById(2)->steeringFlags |= ACTOR_FACES_MOTION;
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	Actor_SetSpeed(GARET, 0xcccc, 0x6666);
	Actor_SetSpeed(IVAN, 0xcccc, 0x6666);
	table = (s32)gImil_PrimaryScript2;
	Engine_ActorEnableActionCallback(GARET, table);
	Object_SetActionCallbackAndRefreshById(2, table);
	Engine_EventWait(20);
	work = gEventWork;
	work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	work->transitionFrames = 24;
	Engine_GameFlagSet(0x82b);
	Engine_EventEnd();
}

static void SceneActor_UpdateCountdownArcPosition(CountdownArcObject *o)
{
	s32	 buf[3];
	s32 *b;
	s32	 n;
	s32	 t;

	if (o != 0)
	{
		n = o->unk64 - 1;
		o->unk64 = n;
		t = (s16)n;
		if (t != 0)
		{
			b = buf;
			b[0] = gImil_ArcOrigin[0];
			b[1] = gImil_ArcOrigin[1] + 0x80000;
			b[2] = gImil_ArcOrigin[2];
			Vector_AddPolarOffset(t << 16, (t << 11) + o->unk66, b);
			o->unk8 = b[0];
			o->unkC = b[1];
			o->unk10 = b[2];
		}
		else
		{
			Engine_ObjectDispatchRelease(o);
		}
	}
}

static void Imil_SwayAndSpark(struct FieldActor *actor)
{
	s32				   pos[3];
	struct FieldActor *spark;
	s32				   radius;

	actor->x.fixed =
		gImil_ArcOrigin[0] + Iwram_MulQ16(0x60000, Engine_MathSin(actor->timer << 10));
	actor->timer++;
	actor->timer = (actor->timer + 64) % 64;
	if (gFrameCount % 3 == 0)
	{
		pos[0] = actor->x.fixed;
		pos[1] = actor->y.fixed + 0x20000;
		pos[2] = actor->z.fixed;
		radius = Engine_RandomNext();
		Vector_AddPolarOffset(radius * 6, Engine_RandomNext(), pos);
		spark = Engine_ObjectCreate(0x11d, pos[0], pos[1], pos[2]);
		if (spark != NULL)
		{
			spark->sprite->priority = 0;
			Engine_ActorSetSpriteFlags(spark, 0);
			Object_SetMode(spark, 1);
			spark->scaleX = 0x9999;
			spark->scaleY = 0x9999;
			spark->priorityFlags = 2;
			spark->motionFlags = 0;
			ObjectGroup_SetChildValue(spark, 9);
			Engine_ObjectSetScript(spark, gImil_SparkScript);
		}
	}
}


static void Imil_ShowActorNineTurnMessage(void)
{
	Engine_ActorEnableActionCallback(9, (s32)gImil_TurnScript);
	Engine_EventShowMessage(9, 0);
}

static void Imil_CastPsynergyAtActorNine(void)
{
	u8 *work;

	Engine_PsynergyBegin(93, 1);
	work = gEffectWork;
	Engine_PsynergySetTarget(3, 9);
	*(s32 *)(work + 36) = (s32)Imil_ShowActorNineTurnMessage;
	Engine_PsynergyRaiseHands();
	Engine_PsynergyPlayEffect(1);
	Engine_PsynergyLowerHands();
	BattleEffect_CleanupSceneObjects();
}

static void SceneActor_ResetActorAndCenterOffsets(struct FieldActor *work)
{
	struct FieldSpritePriority *rec;

	work->motionFlags = 0;
	work->timer = 0;
	work->priorityFlags &= ~1;

	rec = (struct FieldSpritePriority *)work->sprite;
	rec->priority = 1;

	ObjectGroup_SetChildValue(work, 9);
	Engine_ActorSetSpriteFlags(work, 0);

	work->scaleX = 0x8000;
	work->scaleY = 0x8000;
}

static void FieldScene_RunSupplementalSequenceTwo(union FieldObject *object)
{
	object->effect.x += (s16)object->effect.spin << 8;
	object->effect.y += 0x8000;
	object->effect.scaleX += 0x7ae;
	object->effect.scaleY += 0x7ae;
	object->effect.spin += 2;
	if (--object->effect.countdown == 0)
	{
		Engine_ObjectDispatchRelease(object);
	}
}

static void FieldScene_RunScene399SequenceA(void)
{
	// volatile: the type of &gFrameCount (tagged in frame.h); the six editions build the same bytes without it
	volatile u32	  *frame;
	union FieldObject *object;

	frame = &gFrameCount;
	if (((u32)*frame % (u32)60) == 0)
	{
		object = (union FieldObject *)Object_Create(222, 0x1cf0000, 0, 0x1240000);
		if (object != NULL)
		{
			SceneActor_ResetActorAndCenterOffsets(object);
			object->effect.countdown = 60;
			object->effect.update = FieldScene_RunSupplementalSequenceTwo;
			Object_SetMode(object, 5);
		}
	}
	if (((u32)(*frame + 30) % (u32)60) == 0)
	{
		object = (union FieldObject *)Object_Create(222, 0x1400000, 0x200000, 0x1640000);
		if (object != NULL)
		{
			SceneActor_ResetActorAndCenterOffsets(object);
			object->effect.countdown = 60;
			object->effect.update = FieldScene_RunSupplementalSequenceTwo;
			Object_SetMode(object, 5);
		}
	}
	if (((u32)(*frame + 10) % (u32)60) == 0)
	{
		object = (union FieldObject *)Object_Create(222, 0x760000, 0, 0x460000);
		if (object != NULL)
		{
			SceneActor_ResetActorAndCenterOffsets(object);
			object->effect.countdown = 60;
			object->effect.update = FieldScene_RunSupplementalSequenceTwo;
			Object_SetMode(object, 5);
		}
	}
	if (((u32)(*frame + 50) % (u32)60) == 0)
	{
		object = (union FieldObject *)Object_Create(222, 0x1560000, 0, 0x7c0000);
		if (object != NULL)
		{
			SceneActor_ResetActorAndCenterOffsets(object);
			object->effect.countdown = 60;
			object->effect.update = FieldScene_RunSupplementalSequenceTwo;
			Object_SetMode(object, 5);
		}
	}
	if (((u32)(*frame + 80) % (u32)60) == 0)
	{
		object = (union FieldObject *)Object_Create(222, 0x1af0000, 0, 0xab0000);
		if (object != NULL)
		{
			SceneActor_ResetActorAndCenterOffsets(object);
			object->effect.countdown = 60;
			object->effect.update = FieldScene_RunSupplementalSequenceTwo;
			Object_SetMode(object, 5);
		}
	}
}

static void SceneState_UpdateZoneFlagsFromActorZero(void)
{
	struct FieldActor *obj;
	s32				   x;
	s32				   cx;
	s32				   r;
	s32				   g;
	s32				   h;
	State			  *st;

	obj = Object_GetById(0);
	x = obj->x.fixed;
	cx = x >> 19;
	g = 0x200;
	h = 0x201;
	if ((u32)(cx - 24) <= 7 || ((u32)((obj->z.fixed >> 19) - 36) <= 9 && (u32)(cx - 22) <= 9))
	{
		r = Engine_GameFlagIsSet(g);
		if (r != 0)
			return;
		((State *)gMapWork[0])->unk17 = r;
		Engine_GameFlagSet(g);
		Engine_GameFlagClear(h);
		return;
	}
	if (x > 0xE80000 && obj->y.fixed > 0x1E0000 && obj->z.fixed > 0xD40000)
	{
		st = gMapWork[0];
		st->unk17 = 0;
		Engine_GameFlagSet(g);
		Engine_GameFlagClear(h);
		return;
	}
	r = Engine_GameFlagIsSet(h);
	if (r != 0)
		return;
	st = gMapWork[0];
	st->unk17 = 1;
	Engine_GameFlagSet(h);
	Engine_GameFlagClear(g);
	return;
}

static void EV11_PSYNERGY(void)
{
	s32 a;
	s32 b;

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	struct FieldActor *actor = Object_GetById(11);

	a = actor->x.fixed >> 20;
	if (a != 15)
		return;
	b = actor->z.fixed >> 20;
	if (b != 7)
		return;
	Engine_GameFlagSet(0x203);
	Engine_ActorSetSpritePriority(11, 3);
#else
	Engine_GameFlagSet(0x203);
	Engine_ActorSetSpritePriority(11, 3);
	a = 15;
	b = 7;
#endif
	Map_CopyCellAttributes(15, 6, 1, 1, a, b);
}

static void EV04_TOUCH(void)
{
	struct FieldActor *work;

	Engine_EventBegin();
	work = Object_GetById(0);
	work->facing = 0xc000;
	Engine_AudioPlayCue(123);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(8);
}

static void RunEventScript02(void)
{
	Engine_EventBegin();
	Engine_EventOpenScreen();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x1999);
	Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 232, 204);
	Engine_EventEnd();
}

static void FieldScene_RunThreeActorChoreography(void)
{

	struct FieldActor *leader;
	struct EventWork  *work;

	Engine_EventBegin();
	Actor_FaceDirection(MIA, 0xa000, 0);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
	Engine_ActorWalkTo(0, 0x2b2, 200);

	Battle_GetWorkObject1e0()->motionFlags = 0;
	Engine_CameraSetSpeed(0xcccc, 0x1999);
	Camera_MoveTo(0x2b20000, 0, 0xa40000, 1);
	work = gEventWork;
	work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	work->transitionFrames = 48;
	Engine_EventOpenScreen();
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Actor_SetSpeed(MIA, 0x9999, 0x4ccc);
	leader = Object_GetById(0);
	if (leader != NULL)
	{
		Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
	}
	Actor_WalkToAndWait(MIA, 0x2a1, 183);
	Actor_FaceDirection(MIA, 0xc000, 0);
	Engine_ActorStartRepeatedMotion(19, 2);
	Engine_ActorRunRepeatedMotion(20, 2);
	Engine_EventWait(40);
	Engine_EventSetMessage(MSGNO(MsgImilMia));
	Event_ShowMessageAndWait(19, 0, 10);
	Actor_FaceDirection(MIA, 0xe000, 40);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Actor_SetAttachedEffect(20, 0x102);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x4014, 0, 10);
	Actor_FaceDirection(MIA, 0xa000, 40);
	Engine_ActorSetAnimationAndWait(MIA, 4);
	Event_ShowMessageAndWait(0x2003, 0, 10);
	Engine_ActorSetAnimationAndWait(20, 3);
	Engine_EventWait(20);
	Actor_SetAttachedEffect(19, 0x102);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(19, 0, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
	Actor_FaceDirection(MIA, 0xf000, 10);
	Actor_FaceDirection(MIA, 0x2000, 40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(MIA, 0xc000, 40);
	Engine_ActorRunRepeatedMotion(20, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x4014, 0, 20);
	Actor_FaceDirection(MIA, 0xa000, 20);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_EventWait(60);
	Actor_ShowEmote(MIA, 0x105, 60);
	Actor_ShowEmote(19, 0x101, 0);
	Actor_ShowEmote(20, 0x101, 60);
	Engine_ActorRunRepeatedMotion(19, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(19, 0, 10);
	Actor_FaceDirection(MIA, 0xe000, 40);
	Actor_FaceDirection(MIA, 0xa000, 40);
	Actor_FaceDirection(MIA, 0xe000, 20);
	Actor_FaceDirection(MIA, 0x6000, 80);
	Event_ShowMessageAndWait(0x2003, 0, 20);
	Actor_FaceDirection(20, 0xf000, 0);
	Actor_FaceDirection(19, 0x7000, 40);
	Actor_FaceDirection(19, 0x5000, 0);
	Actor_FaceDirection(20, 0x3000, 20);
	Actor_SetSpeed(20, 0x10000, 0x8000);
	Object_GetById(20)->steeringFlags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(20, 0x290, 166);
	Engine_EventWait(1);
	Object_GetById(20)->steeringFlags |= ACTOR_FACES_MOTION;
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x4014, 0, 10);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Engine_EventWait(40);
	Actor_FaceDirection(MIA, 0xa000, 10);
	Event_ShowMessageAndWait(0x2003, 0, 40);
	Actor_FaceDirection(MIA, 0x2000, 20);
	Event_ShowMessageAndWait(0x4003, 0, 10);
	Actor_SetAttachedEffect(19, 0x102);
	Actor_SetAttachedEffect(20, 0x102);
	Engine_EventWait(40);
	Actor_FaceDirection(MIA, 0xc000, 20);
	Engine_ActorSetAnimation(MIA, 4);
	Event_ShowMessageAndWait(0x2003, 0, 20);
	Engine_ActorRunRepeatedMotion(19, 1);
	Event_ShowMessageAndWait(19, 0, 10);
	Actor_FaceDirection(MIA, 0x2000, 40);
	Actor_FaceDirection(MIA, 0xc000, 20);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(20, 1);
	Event_ShowMessageAndWait(0x4014, 0, 20);
	Engine_ActorRunRepeatedMotion(MIA, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(MIA, 0xa000, 20);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Event_ShowMessageAndWait(0x2003, 0, 80);
	Actor_ShowEmote(19, 0x105, 0);
	Actor_ShowEmote(20, 0x105, 60);
	Engine_ActorSetAnimationAndWait(19, 4);
	Event_ShowMessageAndWait(19, 0, 10);
	Engine_ActorSetAnimation(20, 4);
	Event_ShowMessageAndWait(0x4014, 0, 20);
	Actor_ShowEmote(MIA, 0x102, 60);
	Event_ShowMessageAndWait(0x2003, 0, 40);
	Engine_ActorSetAnimationAndWait(19, 3);
	Event_ShowMessageAndWait(19, 0, 10);
	Actor_FaceDirection(MIA, 0xe000, 20);
	Engine_ActorSetAnimationAndWait(20, 3);
	Event_ShowMessageAndWait(0x4014, 0, 10);
	Actor_FaceDirection(MIA, 0xa000, 60);
	Actor_FaceDirection(MIA, 0xe000, 20);
	Actor_FaceDirection(MIA, 0xa000, 20);
	Actor_FaceDirection(MIA, 0xc000, 40);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Event_ShowMessageAndWait(0x2003, 0, 10);
	Engine_ActorSetAnimation(19, 3);
	Engine_ActorSetAnimationAndWait(20, 3);
	Engine_EventWait(40);
	Actor_FaceDirection(MIA, 0x2000, 20);
	Event_ShowMessageAndWait(0x4003, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Actor_WalkToAndWait(MIA, 0x2b0, 200);
	Actor_SetPosition(MIA, 0, 0);
	Object_GetById(20)->steeringFlags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(20, 0x284, 166);
	Engine_EventWait(1);
	*(u8 *)(((s32)Object_GetById(20)) + ACTOR_FLAGS_OFFSET) |= 1;
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	GameFlag_Set(0x82e);
	GameFlag_Clear(0x82d);
	Engine_EventEnd();
}

static void SceneActor_TurnTowardTableAngle(s32 z)
{
	T  *o;
	s32 t;
	s32 d;
	u16 prev;
	s32 n;

	o = (T *)z;
	n = o->unk64;
	z = 0;
	t = ((s16 *)&o->unk64)[z];
	if (t != 0)
	{
		o->unk64 = n - 1;
		return;
	}
	o->unk5A = t;
	z = 1;
	d = gImil_KeyHeadings[(gKeysHeld >> 4) & 0xF];
	z = -z;
	if (d == z)
	{
		Object_SetMode(o, 9);
		return;
	}
	prev = o->unk6;
	d = (s16)(d - prev);
	if (d > 0x1000)
		d = 0x1000;
	if (d < -0x1000)
		d = -0x1000;
	o->unk6 = prev + d;
	Object_SetMode(o, 2);
	ObjectDispatch_ApplyValueToChildren(o, 0x30);
}

static void StagedActor_RunHeadingProbeStep(void)
{

	struct FieldActor	*subject;
	struct FieldPosition probe;
	s32					 heading;
	s32					 goal;
	s32					 marker;
	s32					 z;
	s32					 x;
	u8					*p;

	subject = Engine_ActorLookup(((struct SelectedSubject *)&gGameState)->selectedSubject);

	for (;;)
	{
		heading = gImil_SwayHeadings[(gKeysHeld >> 4) & 15];
		if ((heading << 16) == -0x10000)
		{
			return;
		}

		Engine_EventBegin();

		probe.x = (subject->x.fixed & 0xfff00000) + 0x80000;
		probe.y = subject->y.fixed;
		probe.z = (subject->z.fixed & 0xfff00000) + 0x80000;
		z = probe.z;
		x = probe.x;
		p = (u8 *)subject;
		p += 34;
		goal = GetMapCellCollision(*p, x, z);
		Vector_AddPolarOffset(0x100000, heading, &probe);

		marker = GetMapCellCollision(*p, probe.x, probe.z);
		if (marker == 255
			|| Map_GetTerrainHeight(*p, probe.x, probe.z) - subject->y.fixed > 0x80000)
		{
			subject->facing = heading;
			goto tail;
		}

		probe.x = x;
		probe.z = z;
		subject->speed = 0x20000;
		subject->acceleration = 0x1999;
		subject->timer = 0;
		Engine_ObjectSetPosition(subject, x, subject->y.fixed, z);
		Object_SetMode(subject, 2);
		ObjectDispatch_ApplyValueToChildren(subject, 48);
		Engine_ObjectCommitPosition(subject);
		subject->update = (void (*)(union FieldObject *))SceneActor_TurnTowardTableAngle;

		for (;;)
		{
			FieldVector_AddPolarOffset(0x100000, heading, &probe);
			if ((marker = GetMapCellCollision(*p, probe.x, probe.z)) == 255)
			{
				break;
			}
			if (Map_GetTerrainHeight(*p, probe.x, probe.z) - subject->y.fixed > 0x80000)
			{
				break;
			}
			x = probe.x;
			z = probe.z;
			subject->speed = 0x20000;
			subject->acceleration = 0x1999;
			Engine_ObjectSetPosition(subject, probe.x, probe.y, probe.z);
			Engine_ObjectCommitPosition(subject);
			if (marker != goal)
			{
				goto blocked;
			}
		}

		subject->speed = 0x20000;
		subject->acceleration = 0x10000;
		Engine_ObjectSetPosition(subject, x, subject->y.fixed, z);
		Engine_ObjectCommitPosition(subject);
		Engine_TaskWait(2);
	}

blocked:
	subject->update = NULL;
	subject->steeringFlags |= 1;

	subject->acceleration = 0x4000;

tail:
	Engine_TaskWait(10);
	Engine_EventEnd();
}

s16 gImil_KeyHeadings[] = {
	-1, FACING_EAST, FACING_WEST, -1, FACING_NORTH, FACING_NORTHEAST, FACING_NORTHWEST, FACING_NORTH,
	FACING_SOUTH, FACING_SOUTHEAST, FACING_SOUTHWEST, FACING_SOUTH, -1, FACING_EAST, FACING_WEST, -1,
	-1, FACING_EAST, FACING_WEST, -1, FACING_NORTH, -1, -1, FACING_NORTH,
	FACING_SOUTH, -1, -1, FACING_SOUTH, -1, FACING_EAST, FACING_WEST, -1,
};

s16 gImil_SwayHeadings[] = {
	-1, FACING_EAST, FACING_WEST, -1, FACING_NORTH, FACING_EAST, FACING_WEST, FACING_NORTH,
	FACING_SOUTH, FACING_EAST, FACING_WEST, FACING_SOUTH, -1, FACING_EAST, FACING_WEST, -1,
};

s32 gImilPlacementData[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_ADD(SW_SCALE_X, -0x5000),
	SC_ADD(SW_SCALE_Y, 0x1000),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x2800),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x1400),
	SC_ADD(SW_SCALE_Y, -1024),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x1400),
	SC_ADD(SW_SCALE_Y, -1024),
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gImil_ActorScriptA[] = {
	SC_CALL(OverlayObject_UpdateWobbleByCounter),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gImil_ActorScriptB[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(136), 0, PIXELS(192)),
	SC_SYNC,
	SC_MOVE(PIXELS(136), 0, PIXELS(136)),
	SC_SYNC,
	SC_MOVE(PIXELS(149), 0, PIXELS(116)),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_EAST),
	SC_ARG(1),
	SC_END,
};

s32 gImil_ActorScriptC[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(155), 0, PIXELS(135)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0xd000),
	SC_END,
	SC_ADD(SW_SCALE_Y, 0x6000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, -0x3000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, -0x1800),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, -0x1800),
	SC_WAIT(4),
	SC_END,
};

s32 gImil_TurnScript[] = {
	SC_ADD(SW_SCALE_Y, 0x6000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, -0x3000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, -0x1800),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, -0x1800),
	SC_WAIT(4),
	SC_LOOP(2, SC_TOP),
	SC_END,
};

s32 gImil_SparkScript[] = {
	SC_ADD(SW_Y, -0x6666),
	SC_WAIT(1),
	SC_LOOP(72, SC_TOP),
	SC_FREE,
};

s32 gImil_MiaScriptA[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(177), 0, PIXELS(136)),
	SC_SYNC,
	SC_MOVE(PIXELS(161), 0, PIXELS(124)),
	SC_SYNC,
	SC_SET(0x23, 1),
	SC_MOVE(PIXELS(134), 0, PIXELS(139)),
	SC_SYNC,
	SC_MOVE(PIXELS(135), 0, PIXELS(189)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 gImil_MiaScriptB[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(165), 0, PIXELS(207)),
	SC_SYNC,
	SC_SET(0x23, 1),
	SC_MOVE(PIXELS(147), 0, PIXELS(252)),
	SC_SYNC,
	SC_MOVE(PIXELS(150), 0, PIXELS(268)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 gImil_PrimaryScript[] = {
	SC_CALL(OverlayObject_UpdateFacingTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gImil_PrimaryScript2[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(132), 0, PIXELS(206)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

struct SceneEntrance gImilEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 288, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 480, 0, 280, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 312, 0, 424, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 424, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 456, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 376, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 120, 0, 208, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 64, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 9, CONDITION_ALWAYS, 229, 0, 88, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImilEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 136, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 144, 0, 242, FACING_NORTH, 0, 32, 52, 256, 280, 0 },
	{ 2, CONDITION_ALWAYS, 143, 0, 596, FACING_NORTH, 0, 42, 424, 256, 631, 0 },
	{ 3, CONDITION_ALWAYS, 399, 0, 322, FACING_NORTH, 0, 320, 58, 536, 360, 0 },
	{ 4, CONDITION_ALWAYS, 400, 0, 659, FACING_NORTH, 0, 307, 382, 545, 697, 0 },
	{ 5, CONDITION_ALWAYS, 129, 0, 915, FACING_NORTH, 0, 50, 706, 350, 949, 0 },
	{ 6, CONDITION_ALWAYS, 446, 0, 825, FACING_SOUTH, 0, 402, 740, 670, 921, 0 },
	{ 7, CONDITION_ALWAYS, 687, 0, 264, FACING_NORTH, 0, 594, 49, 806, 295, 0 },
	{ 8, CONDITION_ALWAYS, 182, 0, 772, FACING_SOUTH, 0, 50, 706, 350, 949, 0 },
	{ SCENE_TABLE_END },
};

u32 gImil_SceneTable[] = {
	SCENE_EXITS(50),
	SCENE_EXIT(1, 51, 1),
	SCENE_EXIT(2, 51, 5),
	SCENE_EXIT(3, 51, 3),
	SCENE_EXIT(4, 51, 4),
	SCENE_EXIT(5, 51, 2),
	SCENE_EXIT(6, 51, 7),
	SCENE_EXIT(8, 52, 1),
	SCENE_EXIT(11, 2, 9),
	SCENE_EXIT(12, 2, 39),
	SCENE_EXIT(0, 51, 0),
	SCENE_EXIT(1, 50, 3),
	SCENE_EXIT(2, 50, 7),
	SCENE_EXIT(3, 50, 5),
	SCENE_EXIT(4, 50, 6),
	SCENE_EXIT(5, 50, 4),
	SCENE_EXIT(6, 51, 8),
	SCENE_EXIT(7, 50, 8),
	SCENE_EXIT(8, 51, 6),
	SCENE_EXITS_END,
};

struct ScenePlacement gImilPlacements[] = {
	{ 0x6c, CONDITION_ALWAYS, (s32)gImilPlacementData,
		PIXELS(99), 0, PIXELS(234), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(376), 0, PIXELS(408), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(408), 0, PIXELS(248), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xea, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(262), 0, PIXELS(124), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImilPlacementsFlag881[] = {
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(99), 0, PIXELS(234), FACING_NORTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(376), 0, PIXELS(408), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(408), 0, PIXELS(248), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xea, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(262), 0, PIXELS(124), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImilPlacements2[] = {
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(111), 0, PIXELS(215), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x67, 0x0, ACTOR_STAND,
		PIXELS(166), 0, PIXELS(116), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, 0x0, 7,
		PIXELS(174), 0, PIXELS(173), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x67, 0x0, ACTOR_STAND,
		PIXELS(118), 0, PIXELS(488), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x67, 0x0, ACTOR_STAND,
		PIXELS(370), 0, PIXELS(468), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x67, 0x0, ACTOR_STAND,
		PIXELS(478), 0, PIXELS(468), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x66, 0x0, ACTOR_STAND,
		PIXELS(460), 0, PIXELS(266), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x73, 0x0, ACTOR_STAND,
		PIXELS(436), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x77, 0x0, ACTOR_STAND,
		PIXELS(100), 0, PIXELS(830), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x80, 0x0, ACTOR_WANDER,
		PIXELS(160), 0, PIXELS(880), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6c, 0x0, ACTOR_STAND,
		PIXELS(249), 0, PIXELS(832), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6b, 0x0, ACTOR_STAND,
		PIXELS(686), 0, PIXELS(148), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x66, 0x0, ACTOR_STAND,
		PIXELS(717), 0, PIXELS(229), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ GARET, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ IVAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ MIA, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xdf, 0x0, 7,
		PIXELS(192), 0, PIXELS(124), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gImilEventsOther[] = {
	{ EVENT_EXIT, 60, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 61, CONDITION_ALWAYS, 12 },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, 0x200, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 51, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunExitDoor) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 52, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunExitDoor) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 53, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunExitDoor) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 54, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunExitDoor) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 55, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunExitDoor) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 56, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunExitDoor) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EV04_TOUCH) },
	{ EVENT_TOUCH, 90, CONDITION_ALWAYS, EVENT_SCRIPT(EV90_TOUCH) },
	{ EVENT_TALK, 8, 0x881, EVENT_SCRIPT(EV08_TALK) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgImilTheMercuryLighthouseIsShiningMia) },
	{ EVENT_TALK, 9, 0x881, EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgImilImilWillBeFineDiseaseIs) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x881, EVENT_SCRIPT(EV08_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgImilMiaWasOnlyInImilTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x881, MSGNO(MsgImilWeveHadALotOfStrangers) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgImilDontWorryAboutUsMiaWell) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x881, MSGNO(MsgImilPeopleGetSickBecauseTheyStay) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgImilNowThatIveLetMyGuard) },
	{ SEARCH(SEARCH_JAR), 100, 0xf5f, FIND_COINS(9) },
	{ SEARCH(SEARCH_CHEST), 101, 0xf60, FIND_ITEM(ITEM_VIAL) },
	{ SEARCH(SEARCH_GRAVE), 102, 0xf61, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImilEvents2[] = {
	{ EVENT_TALK, 8, 0x82c, MSGNO(MsgImilTheresBeenAnEpidemicGoingAround) },
	{ EVENT_TALK, 8, 0x82b, MSGNO(MsgImilGetMiaHurry) },
	{ EVENT_TALK, 8, 0x881, EVENT_SCRIPT(EV08_TALK_2) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgImilThatFountainWaterReallyMadeGrandpa) },
	{ EVENT_TALK, 9, 0x82c, EVENT_SCRIPT(FieldScene_RunSupplementalSequenceOne) },
	{ EVENT_TALK, 9, 0x82b, MSGNO(MsgImilItHurts) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunSupplementalSequenceOne) },
	{ EVENT_TALK, 10, IF_FLAG_SET(0x82b), EVENT_SCRIPT(EV10_TALK_2) },
	{ EVENT_TALK, 11, 0x881, MSGNO(MsgImilCoughTheEpidemicHasHitAlmost) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgImilItsNiceThatTheFountainCan) },
	{ EVENT_TALK, 12, 0x881, EVENT_SCRIPT(EV12_TALK) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunWeaponShop) },
	{ EVENT_TALK, 13, 0x881, MSGNO(MsgImilWereClosedSorryIGetA) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunArmorShop) },
	{ EVENT_TALK, 15, 0x881, MSGNO(MsgImilIfYouWantToBuyAnything) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunItemShop) },
	{ EVENT_TALK, 14, 0x881, EVENT_SCRIPT(Imil_RunItemShop) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgImilImSoHappyThatMommyIs) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TALK) },
	{ EVENT_TALK, 17, 0x881, MSGNO(MsgImilWereTheOnlyInnThatStays) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgImilThankGoodnessTheTownIsBack) },
	{ EVENT_TALK, 18, 0x881, EVENT_SCRIPT(EV18_TALK) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgImilImGladTheBeaconHasBeen) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 20, 0x881, EVENT_SCRIPT(EV20_TALK) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgImilIWillBeAKindGirl) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x881, EVENT_SCRIPT(EV08_PSYNERGY_2) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgImilGrandpasAllBetterNowThanksTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x409, 0x82c, EVENT_SCRIPT(FieldScene_RunSupplementalSequenceOne) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x82b, MSGNO(MsgImilOoohIDontFeelSoGood) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x881, MSGNO(MsgImilMiaIsAGoodGirlI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgImilNowMiaCanLeaveTheVillage) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x82b), EVENT_SCRIPT(EV10_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x881, MSGNO(MsgImilHerPalmsShineWithABlue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgImilThatBlueLightFromHerHands) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x881, MSGNO(MsgImilWhoAreThesePeopleTheyMust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgImilCustomersAreHardToComeBy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x881, MSGNO(MsgImilWhyDoTheyAlwaysComeWhen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgImilTheCustomersAreBackIThought) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x881, MSGNO(MsgImilIfOnlyWeHadSomeOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgImilMiaSaidItWouldntBeGood) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x881, MSGNO(MsgImilIWonderIfTheyAreHeaded) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgImilMyDaddysStillInKolimaIll) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x881, MSGNO(MsgImilSoManyCustomersItJustGoes) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgImilMiaWillBeLeavingWithYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x881, MSGNO(MsgImilWintersAreToughHereSoIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgImilTheyCameToOurVillageAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x881, MSGNO(MsgImilWeDontGetToEatMany) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgImilINeverToldMiaThatI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0x881, EVENT_SCRIPT(EV19_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgImilMiasGoingToDefendTheHonor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0x881, EVENT_SCRIPT(EV20_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgImilImSorryMiaButICant) },
	{ 0xc403, 10, 0x881, EVENT_SCRIPT(Imil_RunWeaponShop) },
	{ 0xc403, 11, 0x881, EVENT_SCRIPT(Imil_RunArmorShop) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf26, FIND_ITEM(0xb9) },
	{ SEARCH(SEARCH_BARREL), 101, 0xf62, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SEARCH(SEARCH_OVEN), 102, 0xf63, FIND_ITEM(0xc4) },
	{ SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgImilItLooksLikePorridgeItIs)) },
	{ SEARCH(SEARCH_OVEN), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgImilItsSomeHotStewItWarms)) },
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ SCENE_EVENTS_END },
};

u16 gImil_ExitCellStep5[] = {
	MAP_CELL_STEP(40, 37, 4, 2, 5), MAP_CELL_STEP(40, 39, 4, 2, 5), MAP_CELL_STEPS_END,
	MAP_CELL_STEP(35, 35, 2, 2, 5), MAP_CELL_STEP(33, 35, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_ExitCellStep4[] = {
	MAP_CELL_STEP(35, 37, 2, 2, 5), MAP_CELL_STEP(33, 37, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_ExitCellStep1[] = {
	MAP_CELL_STEP(35, 43, 2, 2, 5), MAP_CELL_STEP(33, 43, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_ExitCellStep0[] = {
	MAP_CELL_STEP(35, 39, 2, 2, 5), MAP_CELL_STEP(33, 39, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_ExitCellStep2[] = {
	MAP_CELL_STEP(42, 48, 2, 2, 5), MAP_CELL_STEP(40, 48, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_ExitCellStep3[] = {
	MAP_CELL_STEP(42, 46, 2, 2, 5), MAP_CELL_STEP(40, 46, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 *gImil_ExitCellSteps[] = {
	gImil_ExitCellStep0,
	gImil_ExitCellStep1,
	gImil_ExitCellStep2,
	gImil_ExitCellStep3,
	gImil_ExitCellStep4,
	gImil_ExitCellStep5,
};

// 出口ごとのセル位置 (x, y)
s16 gImil_ExitCellPoints[][2] = {
	{ 50, 24 }, { 57, 22 }, { 59, 13 }, { 54, 10 }, { 38, 8 }, { 34, 19 },
};

u16 gImil_CellStepsA[] = {
	MAP_CELL_STEP(1, 125, 1, 2, 10), MAP_CELL_STEP(0, 125, 1, 2, 10), MAP_CELL_STEPS_END,
};

u16 gImil_CellStepsB[] = {
	MAP_CELL_STEP(1, 125, 1, 2, 10), MAP_CELL_STEP(2, 125, 1, 2, 10), MAP_CELL_STEPS_END,
};

s32 gImil_ArcOrigin[3];
