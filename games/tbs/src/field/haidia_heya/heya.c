#include "game_state.h"
#include "unit/import.h"
#include "unit/field/haidia_heya/heya.h"
#include "unit/battle/effect/slots.h"
#include "unit/field/common/script/script.h"
#include "unit/system/scheduler/callback.h"
#include "timed_events.h"
#include "call.h"
#include "fxblend.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"
#include "facing_object.h"

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
	s32					 delta;
	u16					 old;
	s32					 angle;
	struct FacingObject *target;

	target = obj->facing_target;
	if (target != 0)
	{
		obj->facing_flags = (0xFE & obj->facing_flags);
		angle = (u16)ArcTan2(
			target->position_z - obj->position_z, target->position_x - obj->position_x);
		old = obj->facing;
		delta = (s16)(angle - old);
		if (delta != 0)
		{
			if (delta > 0x1000)
			{
				delta = 0x1000;
			}

			if (delta < -0x1000)
			{
				delta = -0x1000;
			}
			obj->facing = (old + delta);
		}
	}
	return 1;
}

s32 AdvancePositionScaleAndVelocity(ScaledMotion *motion)
{
	motion->x += motion->velocity_x << 8;
	motion->y += motion->velocity_y << 8;
	motion->scale_x += 0x666;
	motion->scale_y += 0x666;
	motion->velocity_x += 5;
	motion->velocity_y -= 1;
	return 0;
}

void *HaidiaHeya_GetEntrances(void)
{
	return gValeHouseEntrances;
}

int SceneData_ReturnZero(void)
{
	return 0;
}

void *HaidiaHeya_SelectExits(void)
{
	if (Engine_GameFlagIsSet(0x834) != 0)
		return gValeHouseLateExits;
	return gValeHouseExits;
}

void *SceneData_SelectFlaggedTable(void)
{
	void *tbl;

	if (Engine_GameFlagIsSet(0x87a))
	{
		tbl = gValeHouseReturnPlacements;
	}
	else if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE))
	{
		tbl = gValeHousePlacementsAfterLeaving;
	}
	else
	{
		tbl = gValeHousePlacements;
	}
	SceneEvents_UpdateInView(tbl);
	return tbl;
}

void *HaidiaHeya_SelectEvents(void)
{
	if (Engine_GameFlagIsSet(0x834) != 0)
		return gValeHouseLateEvents;
	if (Engine_GameFlagIsSet(0x87a) != 0)
		return gValeHouseReturnEvents;
	if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
		return gValeHouseEventsAfterLeaving;
	return gValeHouseEvents;
}

void Villager_AskWhySukuretaCame(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgHaidiaSukuretaCameToStudyMt);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 15, 6);
	Engine_EventAskYesNo(15, 0);
	Engine_EventEnd();
}

void Villager_PlanToScareVisitors(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgHaidiaLetsScareSukuretasVisitors);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 19, 6);
	Engine_EventAskYesNo(19, 0);
	Engine_EventEnd();
}

void Scene_GiveFarewellHerb(void)
{
	s32 gift;

	Engine_EventBegin();
	if (GameFlag_IsSet(FLAG_GOT_FAREWELL_HERB) != 0)
	{
		Engine_EventSetMessage((s32)MsgHaidiaYouMustSaveJasmine);
		Event_ShowMessage(20, 0);
		FieldObject_SetTargetAndCallback(20, 0x10000, gValeFaceTargetScript);
	}
	else
	{
		gift = (s32)MsgHaidiaThisIsMyFarewellGift;
		Engine_EventSetMessage(gift);
		Event_ShowMessageAndWait(20, 0, 20);
		Engine_MessageShowCentered((gift + 1), 1);
		Engine_PartyGiveItem(ITEM_HERB, 0);
		GameFlag_Set(FLAG_GOT_FAREWELL_HERB);
	}
	Engine_EventEnd();
}

void Villager_AskAboutStrangePowers(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgHaidiaTheCulpritsHadStrangePowers);
	Engine_EventAskYesNo(16, 0);
	Engine_EventEnd();
}

void Villager_AskAboutDora(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgHaidiaDidYouHearAboutDora);
	Engine_EventAskYesNo(10, 0);
	Engine_EventEnd();
}

void HaidiaHeya_TalkHopeDidntGetSick(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgHaidiaHopeDidntGet);
	Engine_EventShowMessage(0x800b, 0);
	Engine_EventEnd();
}

void HaidiaHeya_RequestWindowExit(s32 value)
{
	if (Engine_GameFlagIsSet(0x834) != 0)
		BattleFx_SetBlock30ValuesMaxZero();
	Engine_AudioPlayCue(123);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	gEventWork->transition_frames = 16;
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(value);
}

void FieldScene_RunIndexedStep1(void)
{
	HaidiaHeya_RequestWindowExit(1);
}

void FieldScene_RunIndexedStep2(void)
{
	HaidiaHeya_RequestWindowExit(2);
}

void FieldScene_RunIndexedStep3(void)
{
	HaidiaHeya_RequestWindowExit(3);
}

void FieldScene_RunIndexedStep4(void)
{
	HaidiaHeya_RequestWindowExit(4);
}

void FieldScene_RunIndexedStep5(void)
{
	HaidiaHeya_RequestWindowExit(5);
}

void FieldScene_RunIndexedStep6(void)
{
	HaidiaHeya_RequestWindowExit(6);
}

void FieldScene_RunIndexedStep7(void)
{
	HaidiaHeya_RequestWindowExit(7);
}

void FieldScene_RunIndexedStep8(void)
{
	HaidiaHeya_RequestWindowExit(8);
}

s32 Scene_RunSupplementalSequenceOne(void)
{

	struct FieldActor *record;
	u8				 **scene = (u8 **)&gEventWork;

	*(s32 *)(scene[0] + 0x1c0) = 0x209;
	if (Engine_GameFlagIsSet(0x834) != 0)
	{
		Engine_ActorSetPosition(8, 0, 0);
		Engine_ActorSetPosition(9, 0, 0);
		Engine_ActorSetPosition(10, 0, 0);
		Engine_ActorSetPosition(11, 0, 0);
		Engine_ActorSetPosition(12, 0, 0);
		Engine_ActorSetPosition(13, 0, 0);
		Engine_ActorSetPosition(14, 0, 0);
		Engine_ActorSetPosition(15, 0, 0);
		Engine_ActorSetPosition(16, 0, 0);
		Engine_ActorSetPosition(17, 0, 0);
		Engine_ActorSetPosition(18, 0, 0);
		Engine_ActorSetPosition(19, 0, 0);
		Engine_ActorSetPosition(20, 0, 0);
		Engine_ActorSetPosition(21, 0, 0);
		Engine_ActorSetPosition(22, 0, 0);
		BattleFx_StartTwelveFrameBlend();
		((struct FieldBlendWork *)scene[3])->loud = 1;
		BattleFx_SetBlock30Values12Zero();
		Engine_TaskWait(30);
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		BattleFx_SetBlock30Values128One();
	}
	if (Engine_GameFlagIsSet(0x87a) != 0)
	{
		if (gGameState.entrance == 6)
		{
			if (Engine_GameFlagIsSet(0x81d) == 0)
			{
				FieldScene_RunLongPresentationSequence();
			}
		}
		Object_GetById(10)->collision_flags |= 0x80;
	}
	if (gGameState.entrance == 2)
	{
		if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
		{
			Engine_ActorSetPosition(13, 0x1c60000, 0x960000);
			record = Object_GetById(13);
			Engine_ActorSetSpriteFlags((s32)record, 0);
			Engine_ActorSetAnimation(13, 5);
			Map_ClearLayerEntryFlag(4);
		}
	}
	return 0;
}

void FieldScene_RunByActorDirectionAndFlags(void)
{

	struct FieldActor *p;
	u32				   dir;

	p = Object_GetById(ACTOR_PARTY_LEADER);
	dir = p->facing;
	dir += 0xffff5fff;

	if (dir <= 0x3ffe)
	{
		Engine_ShopOpen(1, 21);
		return;
	}

	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x87a) != 0)
	{
		Engine_EventSetMessage((s32)MsgHaidiaYouCameBack);
		Engine_EventAskYesNo(21, 0);
	}
	else
	{
		if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
		{
			Engine_EventSetMessage((s32)MsgHaidiaValeFeelsEmpty);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgHaidiaGoodWeaponsDrawOutStrength);
		}
		Engine_EventShowMessage(21, 0);
	}
	Engine_EventEnd();
}

void HaidiaHeya_OpenArmorShop(void)
{
	struct FieldActor *actor;

	actor = Object_GetById(ACTOR_PARTY_LEADER);
	if ((u32)(actor->facing - 0xa001) <= 0x3ffe)
	{
		Engine_ShopOpen(2, 22);
	}
	else
	{
		Engine_EventBegin();
		if (Engine_GameFlagIsSet(0x87a) != 0)
		{
			Engine_EventSetMessage((s32)MsgHaidiaTheRumorWasTrue);
		}
		else
		{
			if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
			{
				Engine_EventSetMessage((s32)MsgHaidiaImNotSadJustGo);
			}
			else
			{
				Engine_EventSetMessage((s32)MsgHaidiaGoodArmorDrawsOutStrength);
			}
		}
		Engine_EventShowMessage(22, 0);
		Engine_EventEnd();
	}
}

void HaidiaHeya_OpenItemShop(void)
{
	struct FieldActor *actor;

	actor = Object_GetById(ACTOR_PARTY_LEADER);
	if ((u32)(actor->facing - 0xa001) <= 0x3ffe)
	{
		Engine_ShopOpen(3, 20);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x87a) != 0)
		{
			Engine_EventBegin();
			Engine_EventSetMessage((s32)MsgHaidiaWhenDidYouComeBack);
			Engine_EventShowMessage(20, 0);
			Engine_EventEnd();
		}
		else
		{
			if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
			{
				Scene_GiveFarewellHerb();
			}
			else
			{
				Engine_EventBegin();
				Engine_EventSetMessage((s32)MsgHaidiaAWiseManFleesWhen);
				Engine_EventShowMessage(20, 0);
				Engine_EventEnd();
			}
		}
	}
}

void FieldScene_RunLongPresentationSequence(void)
{
	void Camera_MoveTo();

	struct FieldActor *record;
	struct FieldActor *actor;
	s32				   v6;
	s32				   base7_20090c1;
	s32				   base5_2009400;

	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Engine_ActorSetSpritePriority(MEARI, 1);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
	Actor_SetSpeed(GERALD, 0x6666, 0x3333);
	Actor_SetSpeed(IWAN, 0x6666, 0x3333);
	Actor_SetSpeed(MEARI, 0x6666, 0x3333);
	Engine_ActorSetAnimation(8, 5);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x328, 0x1fc);
	record = Actor_Get(23);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Actor_Get(24);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Actor_Get(25);
	Engine_ActorSetSpriteFlags(record, 0);
	v6 = 0;
	Object_GetById(23)->motion_flags = v6;
	Object_GetById(24)->motion_flags = v6;
	Object_GetById(25)->motion_flags = v6;
	base7_20090c1 = (s32)Scene_UpdateTimedActor;
	Engine_TaskAddCallback((void (*)(void))base7_20090c1, 0xc80);
	Engine_TaskWait(1);
	gEventWork->transition_frames = 32;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	actor = Actor_Get(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		Actor_SetPosition(GERALD, actor->x.fixed, actor->z.fixed);
	}
	actor = Actor_Get(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		Actor_SetPosition(IWAN, actor->x.fixed, actor->z.fixed);
	}
	actor = Actor_Get(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		Actor_SetPosition(MEARI, actor->x.fixed, actor->z.fixed);
	}
	Actor_WalkTo(GERALD, 0x318, 0x200);
	Actor_WalkTo(IWAN, 0x338, 0x1f8);
	Actor_WalkToAndWait(MEARI, 0x332, 0x20c);
	Engine_ActorSetAnimation(GERALD, 1);
	Engine_ActorSetAnimation(IWAN, 1);
	Engine_EventWait(10);
	FieldObject_SetTargetAndCallback(0, 0x1000a, gValeFaceTargetScript);
	FieldObject_SetTargetAndCallback(1, 0x1000a, gValeFaceTargetScript);
	FieldObject_SetTargetAndCallback(2, 0x1000a, gValeFaceTargetScript);
	FieldObject_SetTargetAndCallback(3, 0x1000a, gValeFaceTargetScript);
	Engine_EventWait(0x12c);
	Battle_GetWorkObject1e0()->motion_flags = v6;
	Camera_SetSpeed(0x1999, 0x333);
	Camera_MoveTo(0x3120000, 0, 0x1ae0000, 1);
	Engine_EventWait(240);
	Engine_ActorStop(10);
	Actor_ShowEmote(10, 0x102, 80);
	Actor_WalkToAndWait(10, 0x333, 0x195);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(10, 4);
	Engine_EventWait(40);
	Actor_FaceDirection(10, 0xd000, 20);
	Engine_EventSetMessage((s32)MsgHaidiaHey2);
	Event_ShowMessageAndWait(0x900a, 0, 20);
	Engine_ActorStop(ACTOR_PARTY_LEADER);
	Engine_ActorStop(GERALD);
	Engine_ActorStop(IWAN);
	Engine_ActorStop(MEARI);
	Actor_ShowEmote(11, 0x100, 40);
	Event_ShowMessageAndWait(0x200b, 0, 20);
	Engine_ActorRunRepeatedMotion(10, 2);
	Engine_EventWait(40);
	Event_ShowMessageAndWait(0x900a, 0, 10);
	Actor_FaceDirection(11, 0x5000, 10);
	Event_ShowMessageAndWait(0x200b, 0, 40);
	Engine_ActorRunRepeatedMotion(10, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x900a, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
	Actor_ShowEmote(11, 0x106, 40);
	Event_ShowMessageAndWait(0x200b, 0, 40);
	Engine_ActorStartRepeatedMotion(10, 2);
	Actor_ShowEmote(10, 0x102, 20);
	Engine_ActorSetAnimation(10, 4);
	Event_ShowMessageAndWait(0x900a, 0, 10);
	Engine_ActorStartRepeatedMotion(11, 1);
	Engine_ActorSetAnimationAndWait(11, 3);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(10, 1);
	Engine_ActorSetAnimationAndWait(10, 4);
	Engine_ActorStartRepeatedMotion(11, 1);
	Engine_ActorSetAnimationAndWait(11, 3);
	Engine_ActorStartRepeatedMotion(10, 1);
	Engine_ActorSetAnimationAndWait(10, 4);
	Actor_ShowEmote(9, 0x105, 0);
	Engine_ActorRunRepeatedMotion(9, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(9, 0x1000, 40);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(9, 3);
	Engine_EventWait(40);
	Event_ShowMessageAndWait(0x4009, 0, 40);
	Engine_ActorSetAnimation(11, 0);
	Engine_ActorRunRepeatedMotion(11, 2);
	Event_ShowMessageAndWait(0x200b, 0, 10);
	Engine_ActorSetAnimationAndWait(9, 4);
	Engine_ActorRunRepeatedMotion(9, 2);
	Event_ShowMessageAndWait(0x4009, 0, 10);
	Actor_ShowEmote(10, 0x100, 20);
	Actor_FaceDirection(10, 0x5000, 40);
	Engine_ActorSetAnimationAndWait(10, 3);
	Event_ShowMessageAndWait(0x400a, 0, 10);
	Engine_ActorSetAnimationAndWait(9, 4);
	Actor_FaceDirection(9, 0xd000, 10);
	Engine_ActorJump(9, 2, 0);
	Engine_ActorSetAnimation(9, 4);
	Event_ShowMessageAndWait(0x4009, 0, 10);
	Actor_ShowEmote(11, 0x101, 0);
	Actor_ShowEmote(10, 0x101, 40);
	Actor_FaceDirection(10, 0xd000, 80);
	Actor_FaceDirection(10, 0x5000, 60);
	Engine_ActorStartRepeatedMotion(10, 2);
	Engine_ActorStartRepeatedMotion(11, 2);
	Event_ShowTwoMessagesAndWait(10, 11, 6, 6, 6, 11, 12, 1, 7, 1, v6);
	Engine_EventWait(20);
	Camera_SetSpeed(0x19999, 0x3333);
	Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Event_ShowMessageAndWait(0x1001, 0, 20);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_TaskRemoveCallback((void (*)(void))base7_20090c1);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(8, 6);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x4008, 0, 20);
	Camera_MoveTo(0x2ee0000, 0, 0x1c30000, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(11, 0x5000, 0);
	Actor_FaceDirection(10, 0x5000, 10);
	Actor_FaceDirection(8, 0x1000, 40);
	Actor_ShowEmote(8, 0x100, 40);
	Actor_FaceDirection(8, 0x3000, 20);
	Actor_FaceDirection(8, 0x1000, 20);
	Actor_FaceDirection(8, 0x3000, 40);
	Engine_ActorSetAnimationAndWait(8, 6);
	Engine_EventWait(60);
	Engine_ActorJump(8, 6, 0);
	Event_ShowMessageAndWait(0x4008, 0, 20);
	Actor_SetSpeed(GERALD, 0x19999, 0xcccc);
	Actor_WalkToAndWait(GERALD, 0x315, 0x1d9);
	Actor_FaceDirection(GERALD, 0x7000, 20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Event_ShowMessageAndWait(0x4001, 0, 10);
	Actor_FaceDirection(8, 0x1000, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Event_OpenMessage(0x4008, 0);
	Actor_FaceDirection(10, 0x5000, 0);
	Actor_FaceDirection(9, 0x1000, 0);
	Actor_FaceDirection(GERALD, 0x3000, 0);
	Actor_FaceDirection(IWAN, 0x7000, 0);
	Actor_FaceDirection(MEARI, 0xb000, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		gEventWork->message++;
	}
	Camera_MoveTo(0x3090000, 0, 0x1ac0000, 1);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(10, 2);
	Event_ShowMessage(10, 0);
	Engine_ActorSetAnimationAndWait(11, 4);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgHaidiaYoureLeavingAgainSoon);
	Event_ShowMessage(0x200b, 0);
	Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(GERALD, 0xd000, 20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(9, 4);
	Actor_FaceDirection(9, 0xd000, 10);
	Event_ShowMessage(0x4009, 0);
	Engine_ActorSetAnimationAndWait(8, 3);
	Event_ShowMessage(0x4008, 0);
	Actor_FaceDirection(GERALD, 0x7000, 10);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Actor_FaceDirection(9, 0x1000, 10);
	Engine_ActorSetAnimation(11, 3);
	Engine_ActorSetAnimation(10, 3);
	Engine_ActorSetAnimation(9, 3);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(GERALD, 0x3000, 20);
	Actor_ShowEmote(GERALD, 0x102, 80);
	Actor_FaceDirection(GERALD, 0x7000, 20);
	Event_ShowMessageAndWait(0x4001, 0, 20);
	Actor_FaceDirection(GERALD, 0x3000, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 20);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(GERALD, 0x1000, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
	Actor_SetSpeed(IWAN, 0x10000, 0x8000);
	Actor_WalkToAndWait(IWAN, 0x333, 0x1e9);
	Actor_FaceDirection(IWAN, 0xb000, 40);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_ActorSetAnimation(8, 3);
	Engine_ActorSetAnimation(9, 3);
	Engine_ActorSetAnimation(10, 3);
	Engine_ActorSetAnimationAndWait(9, 3);
	Object_GetById(3)->priority_flags &= 254;
	Engine_ActorSetSpritePriority(MEARI, 1);
	Actor_SetSpeed(MEARI, 0x10000, 0x8000);
	Actor_WalkToAndWait(MEARI, 0x31a, 0x208);
	Actor_FaceDirection(GERALD, 0x5000, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
	Actor_WalkToAndWait(MEARI, 0x310, 0x1f0);
	Actor_FaceDirection(MEARI, 0x9000, 10);
	Object_GetById(3)->priority_flags |= 1;
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Engine_ActorSetAnimation(8, 3);
	Engine_ActorSetAnimation(9, 3);
	Engine_ActorSetAnimation(10, 3);
	Engine_ActorSetAnimationAndWait(9, 3);
	Engine_EventWait(20);
	Camera_MoveTo(0x3090000, 0, 0x1ac0000, 1);
	Engine_EventWait(20);
	Actor_SetSpeed(11, 0x6666, 0x3333);
	Actor_WalkToAndWait(11, 0x343, 0x184);
	Actor_FaceDirection(11, 0x5000, 0);
	Actor_ShowEmote(11, 0x108, 40);
	Event_ShowMessageAndWait(0x200b, 0, 20);
	Camera_MoveTo(0x3090000, 0, 0x1d40000, 1);
	Engine_EventWait(40);
	Actor_FaceDirection(IWAN, 0x7000, 0);
	Actor_FaceDirection(MEARI, 0xf000, 40);
	Actor_FaceDirection(IWAN, 0x9000, 0);
	Actor_FaceDirection(MEARI, 0xd000, 20);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(10, 1);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(10, 3);
	Event_ShowMessageAndWait(10, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xd000, 0);
	Actor_FaceDirection(IWAN, 0xb000, 0);
	Actor_FaceDirection(MEARI, 0xd000, 40);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	Actor_SetSpeed(IWAN, 0x10000, 0x8000);
	base5_2009400 = (s32)gValePartyScript;
	Engine_ActorEnableActionCallback(GERALD, base5_2009400);
	Engine_ActorEnableActionCallback(2, base5_2009400);
	Object_SetActionCallbackAndRefreshById(3, base5_2009400);
	Engine_ActorEnableActionCallback(10, (s32)gValeActor10Script);
	Actor_WalkToAndWait(11, 0x345, 0x178);
	Actor_FaceDirection(11, 0xd000, 20);
	GameFlag_Set(0x81d);
	Engine_EventEnd();
}

void Scene_UpdateTimedActor(void)
{
	s32				  no;
	u32				  phase;
	union SceneActor *actor;
	s32				 *other;

	phase = gFrameCount % 180;
	no = 23;
	switch (phase)
	{
	case 10:
		break;
	case 20:
		no = 24;
		break;
	case 30:
		no = 25;
		break;
	default:
		return;
	}
	actor = Object_GetById(no);
	if (actor == NULL)
	{
		return;
	}
	other = Object_GetById(8);
	if (other != NULL)
	{
		Engine_ActorSetPosition(no, other[2], other[4]);
	}
	actor->words[6] = 0x6666;
	actor->words[7] = 0x6666;
	{
		s32				  y = actor->words[3] + 0x180000;
		union SceneField *dst = (union SceneField *)(actor->halfwords + 50);
		s32				  value;
		actor->words[3] = y;
		actor->words[15] = y;
		value = 25;
		dst->value = value;
		dst++;
		value = 128;
		dst->value = value;
	}
	Engine_ActorEnableActionCallback(no, gValeTimedActorScript);
}

s32 gValeFaceTargetScript[] = {
	SC_CALL(OverlayObject_UpdateFacingTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeActor10Script[] = {
	SC_SET(SW_SPEED, 0x9999),
	SC_SET(SW_ACCEL, 0x4ccc),
	SC_ARG(2),
	SC_MOVE(PIXELS(784), 0, PIXELS(405)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(40),
	SC_LABEL(1),
	SC_ADD(SW_FACING, -1024),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(1)),
	SC_WAIT(40),
	SC_LABEL(3),
	SC_ADD(SW_FACING, -1024),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(3)),
	SC_ARG(2),
	SC_MOVE(PIXELS(819), 0, PIXELS(405)),
	SC_SYNC,
	SC_ARG(1),
	SC_LABEL(2),
	SC_ADD(SW_FACING, -2048),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(2)),
	SC_WAIT(40),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValePartyScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(808), 0, PIXELS(508)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 gValeTimedActorScript[] = {
	SC_CALL(AdvancePositionScaleAndVelocity),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

struct SceneEntrance gValeHouseEntrances[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 96, 0, 222, FACING_NORTH, 0, 4, 4, 244, 292, 0 },
	{ 2, CONDITION_ALWAYS, 337, 0, 199, FACING_NORTH, 0, 252, 4, 548, 236, 0 },
	{ 3, CONDITION_ALWAYS, 656, 0, 244, FACING_NORTH, 0, 556, 4, 900, 276, 0 },
	{ 4, CONDITION_ALWAYS, 159, 0, 525, FACING_NORTH, 0, 4, 324, 332, 564, 0 },
	{ 5, CONDITION_ALWAYS, 528, 0, 486, FACING_NORTH, 0, 348, 284, 612, 516, 0 },
	{ 6, CONDITION_ALWAYS, 816, 0, 522, FACING_NORTH, 0, 628, 284, 884, 548, 0 },
	{ 7, CONDITION_ALWAYS, 463, 0, 851, FACING_NORTH, 0, 324, 604, 588, 884, 0 },
	{ 8, CONDITION_ALWAYS, 769, 0, 857, FACING_NORTH, 0, 580, 628, 852, 884, 0 },
	{ SCENE_TABLE_END },
};

u32 gValeHouseExits[] = {
	SCENE_EXITS(7),
	SCENE_EXIT(1, 4, 10),
	SCENE_EXIT(2, 4, 9),
	SCENE_EXIT(3, 5, 3),
	SCENE_EXIT(4, 5, 5),
	SCENE_EXIT(5, 4, 7),
	SCENE_EXIT(6, 4, 6),
	SCENE_EXIT(7, 5, 4),
	SCENE_EXIT(8, 5, 1),
	SCENE_EXITS_END,
};

u32 gValeHouseLateExits[] = {
	SCENE_EXITS(7),
	SCENE_EXIT(1, 3, 10),
	SCENE_EXIT(2, 3, 9),
	SCENE_EXIT(3, 5, 3),
	SCENE_EXIT(4, 5, 5),
	SCENE_EXIT(5, 3, 7),
	SCENE_EXIT(6, 3, 6),
	SCENE_EXIT(7, 5, 4),
	SCENE_EXIT(8, 5, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement gValeHousePlacements[] = {
	{ 0x75, 0x0, ACTOR_STAND,
		PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x36, 0x0, ACTOR_WANDER,
		PIXELS(815), 0, PIXELS(386), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x26, 0x0, ACTOR_STAND,
		PIXELS(841), 0, PIXELS(455), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x67, 0x0, ACTOR_STAND,
		PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(331), 0, PIXELS(144), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x73, 0x0, ACTOR_STAND,
		PIXELS(486), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x65, 0x0, ACTOR_STAND,
		PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x73, 0x0, ACTOR_STAND,
		PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6c, 0x0, ACTOR_WANDER,
		PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, 0x0, ACTOR_STAND,
		PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x74, 0x0, ACTOR_STAND,
		PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x76, 0x0, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7c, 0x0, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x7d, 0x0, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gValeHousePlacementsAfterLeaving[] = {
	{ 0x75, 0x0, ACTOR_STAND,
		PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, 3, 0 },
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x36, 0x0, ACTOR_WANDER,
		PIXELS(815), 0, PIXELS(386), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x26, 0x0, ACTOR_STAND,
		PIXELS(841), 0, PIXELS(455), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x67, 0x0, ACTOR_STAND,
		PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(456), 0, PIXELS(156), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x73, 0x0, ACTOR_STAND,
		PIXELS(486), 0, PIXELS(118), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x65, 0x0, ACTOR_STAND,
		PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x73, 0x0, ACTOR_STAND,
		PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6c, 0x0, ACTOR_WANDER,
		PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, 0x0, ACTOR_STAND,
		PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x74, 0x0, ACTOR_STAND,
		PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x76, 0x0, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(424), FACING_SOUTH, 3, 0 },
	{ 0x7c, 0x0, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x7d, 0x0, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gValeHouseReturnPlacements[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x75, 0x0, ACTOR_STAND,
		PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x36, 0x0, (s32)gValeActor10Script,
		PIXELS(819), 0, PIXELS(405), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x26, 0x0, ACTOR_STAND,
		PIXELS(841), 0, PIXELS(370), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x67, 0x0, ACTOR_STAND,
		PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(456), 0, PIXELS(156), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x73, 0x0, ACTOR_STAND,
		PIXELS(494), 0, PIXELS(112), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x65, 0x0, ACTOR_STAND,
		PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x73, 0x0, ACTOR_STAND,
		PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6c, 0x0, ACTOR_WANDER,
		PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, 0x0, ACTOR_STAND,
		PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x74, 0x0, ACTOR_STAND,
		PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x76, 0x0, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x7c, 0x0, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x7d, 0x0, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x118, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x118, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x118, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

struct SceneEvent gValeHouseLateEvents[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep1) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep2) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep3) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep4) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep5) },
	{ EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep6) },
	{ EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep7) },
	{ EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep8) },
	{ SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBooksAboutContinents)) },
	{ SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookLawsOfHaidia)) },
	{ 0xc4f3, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookPowerOfNature)) },
	{ 0xc4f3, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookPsynergyStone)) },
	{ 0xc4f3, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaAuntNotebook)) },
	{ 0xc4f3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookMakingGreatVillage)) },
	{ SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaNotebookStudyOfAlchemy)) },
	{ SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookAncientLemurians)) },
	{ SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookFountainOfTolbi)) },
	{ SEARCH(SEARCH_BOOKCASE), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaEncyclopediaOfAlchemy)) },
	{ SEARCH(SEARCH_BOOKCASE), 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookLegendOfAleph)) },
	{ SEARCH(SEARCH_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookBeingAdept)) },
	{ SEARCH(SEARCH_BARREL), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKuupuappuButDidntFindAnything)) },
	{ SEARCH(SEARCH_JAR), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaJarEmpty)) },
	{ SEARCH(SEARCH_JAR), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaJarEmpty)) },
	{ SEARCH(SEARCH_JAR), 103, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaJarEmpty)) },
	{ SEARCH(SEARCH_JAR), 104, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaJarEmpty)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gValeHouseEvents[] = {
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgHaidiaAtTheVeryLeastVisitorsShould) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgHaidiaIHearKradenHasGuestsAgain) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgHaidiaGeraldHasGrownQuiteStrongIn) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgHaidiaYouAndGeraldAreOlderNow) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaMyGranddaughterJasminIsFinallyHappy) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgHaidiaIHopeYouTwoWontBe) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgHaidiaJasminIsTheSpittingImageOf) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Villager_AskWhySukuretaCame) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgHaidiaIfThereAreVisitorsInTown) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgHaidiaDoYouEverDreamOfSeeing) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgHaidiaFewPeopleOutsideOfValePossess) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Villager_PlanToScareVisitors) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunByActorDirectionAndFlags) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaHeya_OpenArmorShop) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaHeya_OpenItemShop) },
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep1) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep2) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep3) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep4) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep5) },
	{ EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep6) },
	{ EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep7) },
	{ EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep8) },
	{ SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBooksAboutContinents)) },
	{ SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookLawsOfHaidia)) },
	{ 0xc4f3, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookPowerOfNature)) },
	{ 0xc4f3, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookPsynergyStone)) },
	{ 0xc4f3, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaAuntNotebook)) },
	{ 0xc4f3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookMakingGreatVillage)) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf42, FIND_ITEM(ITEM_ANTIDOTE) },
	{ SEARCH(SEARCH_JAR), 101, 0xf43, FIND_COINS(6) },
	{ SEARCH(SEARCH_JAR), 102, 0xf44, FIND_COINS(2) },
	{ SEARCH(SEARCH_JAR), 103, 0xf45, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ SEARCH(SEARCH_JAR), 104, 0xf46, FIND_COINS(3) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gValeHouseEventsAfterLeaving[] = {
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgHaidiaRobinPleaseTakeCareOfMy) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgHaidiaReceivingAQuestDirectlyFromThe) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgHaidiaImProudOfYouGeraldMy) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgHaidiaAsMothersDoraAndIWill) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaIHeardThatJasminBrotherGarsia) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgHaidiaPleaseReturnMyPreciousGranddaughterJasmin) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgHaidiaAfterJasminWasTakenGrandmotherGrew) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgHaidiaItsBecauseKradenWasStudyingAlchemy) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Villager_AskAboutStrangePowers) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgHaidiaImSoJealousOfYouBeing) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgHaidiaOnceYouLeaveTownTakeCare) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgHaidiaIHadNoIdeaThatSuch) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunByActorDirectionAndFlags) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaHeya_OpenArmorShop) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaHeya_OpenItemShop) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgHaidiaRobinYoureGoingOnADifficult) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgHaidiaIfYouFaceAnyHardshipIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgHaidiaSoIfThatWasGarsiaWho) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgHaidiaItsHardLettingMyPreciousGerald) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgHaidiaTheWiseOneChoseGeraldHimself) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgHaidiaOnceGeraldLeavesIWontBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgHaidiaIAmConcernedForGeraldBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaGarsiaInheritedHisFathersSkillWith) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgHaidiaJasminAnytimeYouFaceDangerJust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgHaidiaWhyWouldGarsiaTakeHisSister) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgHaidiaWeAlmostLostTheTownOver) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgHaidiaIdLikeToBelieveThatOnly) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgHaidiaIfOnlyIdGoneToSol) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgHaidiaValesLawsStateThatYouCannot) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgHaidiaIWishIWereTheOne) },
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep1) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep2) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep3) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep4) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep5) },
	{ EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep6) },
	{ EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep7) },
	{ EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep8) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf42, FIND_ITEM(ITEM_ANTIDOTE) },
	{ SEARCH(SEARCH_JAR), 101, 0xf43, FIND_COINS(6) },
	{ SEARCH(SEARCH_JAR), 102, 0xf44, FIND_COINS(2) },
	{ SEARCH(SEARCH_JAR), 103, 0xf45, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ SEARCH(SEARCH_JAR), 104, 0xf46, FIND_COINS(3) },
	{ SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBooksAboutContinents)) },
	{ SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookLawsOfHaidia)) },
	{ 0xc4f3, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookPowerOfNature)) },
	{ 0xc4f3, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookPsynergyStone)) },
	{ 0xc4f3, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaAuntNotebook)) },
	{ 0xc4f3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookMakingGreatVillage)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gValeHouseReturnEvents[] = {
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgHaidiaMaryIsSuchANiceGirl) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgHaidiaThatIvanSeemsToBeA) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Villager_AskAboutDora) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaHeya_TalkHopeDidntGetSick) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaOhSoYouStillHaventRescued) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgHaidiaIGetOverColdsQuicklyBut) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgHaidiaAColdIsGoingAroundI) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgHaidiaWhatWasThatAlchemyThatKraden) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgHaidiaEarthquakesStillStrikeOnceInA) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgHaidiaTheMountainIsApparentlyStillCasting) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgHaidiaTheNorthwestFaceOfMtAleph) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgHaidiaTheGreatHealersAreTooAfraid) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunByActorDirectionAndFlags) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaHeya_OpenArmorShop) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaHeya_OpenItemShop) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgHaidiaItMustBeNiceToBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgHaidiaHisNewFriendsAreAllSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgHaidiaItWasntTooSmartToTalk) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgHaidiaGeraldSeemsToHaveGrownUp) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgHaidiaWeveSeenManyMonstersLatelyAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgHaidiaIWishICouldLearnThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0x81b, MSGNO(MsgHaidiaWhatHappenedToJasminHaventYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgHaidiaIGaveHimAFarewellGift) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaWillIEverSeeJasminAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgHaidiaDoraSaysItsJustACold) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgHaidiaIKnowThatThereAreAlways) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgHaidiaEvenIfTheEruptionHadNo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgHaidiaEveryTimeTheGroundShakesI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgHaidiaIfThePsynergyStonesInThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgHaidiaTheySayTheLavaLookedLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgHaidiaTheHealersAreSupposedToGuard) },
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep1) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep2) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep3) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep4) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep5) },
	{ EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep6) },
	{ EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep7) },
	{ EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep8) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf42, FIND_ITEM(ITEM_ANTIDOTE) },
	{ SEARCH(SEARCH_JAR), 101, 0xf43, FIND_COINS(6) },
	{ SEARCH(SEARCH_JAR), 102, 0xf44, FIND_COINS(2) },
	{ SEARCH(SEARCH_JAR), 103, 0xf45, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ SEARCH(SEARCH_JAR), 104, 0xf46, FIND_COINS(3) },
	{ SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBooksAboutContinents)) },
	{ SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookLawsOfHaidia)) },
	{ 0xc4f3, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookPowerOfNature)) },
	{ 0xc4f3, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookPsynergyStone)) },
	{ 0xc4f3, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaAuntNotebook)) },
	{ 0xc4f3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHaidiaBookMakingGreatVillage)) },
	{ SCENE_EVENTS_END },
};
