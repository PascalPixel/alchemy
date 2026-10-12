#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/field/raribero_heya/heya.h"
#include "unit/battle/effect/object.h"
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
#include "unit/game/character/chara.h"
#include "unit/game/character/chara2.h"
#include "unit/game/character/chara3.h"
#include "unit/game/character/chara4.h"
#include "unit/game/character/chara5.h"
#include "unit/game/character/chara6.h"
#include "unit/game/character/chara7.h"
#include "unit/game/character/chara8.h"
#include "unit/menu/shop/shop.h"
#include "unit/menu/shop/shop2.h"
#include "unit/menu/shop/shop3.h"
#include "unit/menu/shop/shop4.h"
#include "unit/menu/shop/shop5.h"
#include "unit/menu/shop/shop6.h"
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
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "heya.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

// 前方宣言
static void FieldScene_RunSecondaryScript(void);

enum
{
	ENTRANCE_SANCTUM_RETURN = 12,
	ENTRANCE_HOUSE_AFTER_REPORT = 21,
	ENTRANCE_FROM_AERIE = 90,
	ENTRANCE_HOUSE_REPORT = 99
};

enum
{
	ACTOR_HOUSE_REPORT_SECOND = 11,
	ACTOR_HOUSE_REPORT_FIRST = 12,
	ACTOR_HOUSE_RESIDENT = 13,
	ACTOR_SANCTUM_SECOND = 18,
	ACTOR_SANCTUM_THIRD = 19,
	ACTOR_SANCTUM_FIRST = 20
};

enum
{
	FLAG_AERIE_EVENTS_DONE = 0x9a7
};

static s32 SceneActor_SetActor14Pose258(void)
{
	Actor_SetAttachedEffect(14, 258);
	return 0;
}

u8 *RariberoHeya_GetEntrances(void)
{
	return gRariberoEntrances;
}

u8 *RariberoHeya_GetRegions(void)
{
	if (gGameState.scene == (s32)&SceneId_RariberoHeya1)
	{
		return gRariberoSanctumRegions;
	}
	return gRariberoRegions;
}

u8 *RariberoHeya_GetExits(void)
{
	return gRariberoExits;
}

u8 *RariberoHeya_GetPlacements(void)
{
	if (gGameState.scene == (s32)&SceneId_RariberoHeya2)
	{
		if (Engine_GameFlagIsSet(0x9A7) != 0)
		{
			return gRariberoHeyaPlacements9a7;
		}
		return gRariberoHeyaPlacements;
	}
	return gRariberoPlacements;
}

static void EV18_TALK(s32 keeper)
{
	struct FieldActor *leader = Object_GetById(0);
	u16				   facing = (leader->facing + 0x2000) & 0xc000;

	if (facing == 0xc000)
	{
		Shop_Run(32, keeper);
	}
	else if (Engine_GameFlagIsSet(0x9a7))
	{
		Engine_EventSetMessage((s32)MsgRariberoWentOceanLook);
		Engine_EventShowMessage(keeper, 0);
	}
	else
	{
		s32 message = (s32)MsgRariberoTakenHostageFaran;

		Engine_EventSetMessage(message);
		Engine_EventOpenMessage(keeper, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_EventWait(10);
			Engine_EventSetMessage(message + 1);
		}
		else
		{
			Engine_EventSetMessage(message + 2);
		}
		Engine_EventShowMessage(keeper, 0);
	}
}

static void EV19_TALK(s32 no)
{
	u16 party_facing = (Object_GetById(0)->facing + 0x2000) & ~0x3fff;
	if (party_facing == 0xc000)
	{
		Shop_Run(33, no);
	}
	else if (Engine_GameFlagIsSet(0x9a7))
	{
		Engine_EventSetMessage((s32)MsgRariberoWantedSendShips);
		Engine_EventShowMessage(no, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgRariberoWarriorRightPlease);
		Engine_EventShowMessage(no, 0);
	}
}

static void EV20_TALK(s32 no)
{
	u16 party_facing = (Object_GetById(0)->facing + 0x2000) & ~0x3fff;
	if (party_facing == 0xc000)
	{
		Shop_Run(34, no);
	}
	else if (Engine_GameFlagIsSet(0x9a7))
	{
		Engine_EventSetMessage((s32)MsgRariberoImpossibleGoOcean);
		Engine_EventShowMessage(no, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgRariberoGuessPeopleAngara);
		Engine_EventShowMessage(no, 0);
	}
}

static void EV21_TALK(s32 no)
{
	u16 party_facing = (Object_GetById(0)->facing + 0x2000) & ~0x3fff;
	if (party_facing == 0xc000)
	{
		Shop_ConfirmAct(no);
	}
	else if (Engine_GameFlagIsSet(0x9a7))
	{
		Engine_EventSetMessage((s32)MsgRariberoBabiLighthouseStruck);
		Engine_EventShowMessage(no, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgRariberoBabiIsntBuilding);
		Engine_EventShowMessage(no, 0);
	}
}

static void EV13_TALK(s32 no)
{
	u16 party_facing = (Object_GetById(0)->facing + 0x2000) & ~0x3fff;
	if (party_facing == 0xc000)
	{
		Inn_CheckIn(11, no);
	}
	else if (Engine_GameFlagIsSet(0x9a7))
	{
		Engine_EventSetMessage((s32)MsgRariberoDidntKnowTime);
		Engine_EventShowMessage(no, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgRariberoWantStayLet);
		Engine_EventShowMessage(no, 0);
	}
}

static void RariberoScene_PlayPoseSequence(void)
{
	s16 facing;

	facing = (Actor_Get(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
	GameFlag_Set(0x300);
	Engine_EventBegin();
	Engine_ResetSceneEffectCounter();
	Engine_EventSetMessage((s32)MsgRariberoImSorry);
	Engine_EventWait(50);
	Actor_ShowEmote(14, 0x102, 50);
	Actor_FaceActor(14, ACTOR_PARTY_LEADER, 20);
	Event_ShowMessage(14, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(14, 4);
	Engine_EventWait(30);
	Event_ShowMessage(14, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(14, 2);
	Engine_EventWait(30);
	Event_ShowMessage(14, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(14, 3);
	Engine_EventWait(20);
	Event_ShowMessage(14, 0);
	if ((u16)facing == 0x8000)
	{
		Engine_ActorWalkByAndWait(0, 0, 16);
		Engine_ActorFaceDirection(0, 0xc000, 0);
		Engine_EventWait(20);
	}
	Engine_ActorEnableActionCallback(14, gRariberoPoseAction);
	Engine_EventEnd();
}

static void EV51_TOUCH(void)
{

	Engine_GameFlagSet(0x9BC);
	Engine_EventBegin();
	Engine_ResetSceneEffectCounter();
	Engine_EventWait(0xA);
	Engine_CameraMoveTo(0x780000, -1, 0x600000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(0x1E);
	Engine_EventSetMessage((s32)MsgRariberoPleaseWaitForMeOutside);
	Engine_EventShowMessage(0xC, 0);
	Engine_EventWait(0xA);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 0xC, 0);
	Engine_EventWait(0x1E);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(0x1E);
	Engine_EventEnd();
}

static void EV12_TALK(void)
{

	Engine_GameFlagSet(0x9BC);
	Engine_EventSetMessage((s32)MsgRariberoPleaseWaitForMeOutside);
	Engine_EventShowMessage(0xC, 0);
}

static void RariberoHeya_RequestTouchedExitAndPlayCue(void)
{
	struct EventWork *work = gEventWork;
	s16				 *p = (s16 *)&work->touched_trigger;

	Engine_EventRequestExit(*p);
	Engine_AudioPlayCue(0x7B);
}

u8 *RariberoHeya_GetEvents(void)
{
	if (gGameState.scene == (s32)&SceneId_RariberoHeya2)
	{
		if (Engine_GameFlagIsSet(0x9a7) != 0)
		{
			return gRariberoHeyaEvents9a7;
		}
		return gRariberoHeyaEvents;
	}
	if (Engine_GameFlagIsSet(0x9a7) != 0)
	{
		return gRariberoEvents9a7;
	}
	return gRariberoEvents;
}

static void FieldScene_RunPrimaryScript(void)
{

	Engine_EventBegin();
	Owner_RefreshActiveRatios(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 6291456, 12058624);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 49152, 0);
	Actor_FaceDirection(11, 0, 0);
	Actor_FaceDirection(12, 32768, 0);
	Engine_EventSetMessage((s32)MsgRariberoHowDidSearchForSheba);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(10);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 65536, 32768);
	Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
	Engine_ActorWalkToAndWait(0, 104, 136);
	Engine_EventWait(10);
	FieldMotion_LaunchFromFocusedObject(1, -16, 16, 49152);
	FieldMotion_LaunchFromFocusedObject(3, 0, 24, 49152);
	FieldMotion_LaunchFromFocusedObject(2, 16, 16, 49152);
	Engine_ActorWaitForMove(1);
	Engine_EventWait(20);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(2, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(2, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(11, 16384, 0);
	Engine_ActorFaceDirection(12, 16384, 0);
	Engine_EventWait(30);
	Actor_ShowEmote(11, 261, 70);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(11, 4);
	Engine_EventWait(20);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(11, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(20);
	Actor_ShowEmote(12, 258, 40);
	Engine_EventShowMessage(12, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(12, 4);
	Engine_EventWait(20);
	Engine_EventShowMessage(12, 0);
	Engine_EventWait(20);
	Engine_EventWait(10);
	Actor_ShowEmote(11, 261, 50);
	Engine_EventShowMessage(11, 0);
	if (GameFlag_IsSet(2495) == 0)
	{
		FieldScene_RunSecondaryScript();
	}
	else
	{
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(11, 2);
		Engine_EventWait(20);
		Engine_EventShowMessage(11, 0);
		Engine_EventWait(10);
		Actor_FaceActor(GERALD, ACTOR_PARTY_LEADER, 50);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 261, 60);
		Engine_EventWait(10);
		Engine_ActorFaceDirection(1, 49152, 0);
		Engine_EventWait(20);
		Engine_EventShowMessage(1, 0);
		Engine_EventWait(10);
		Engine_ActorSetAnimationAndWait(3, 4);
		Engine_EventWait(20);
		Engine_EventShowMessage(3, 0);
		Engine_EventWait(10);
		Engine_ActorRunRepeatedMotion(2, 2);
		Engine_EventWait(20);
		Engine_EventShowMessage(2, 0);
		Engine_EventWait(20);
		Actor_ShowEmote(12, 257, 40);
		Event_OpenMessage(12, 0);
		Engine_EventChooseYesNo(0, 0);
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(12, 2);
		Engine_EventWait(20);
		Engine_EventShowMessage(12, 0);
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(0, 3);
		Engine_EventWait(30);
		Engine_EventWait(10);
		Engine_ActorRunRepeatedMotion(1, 2);
		Engine_EventWait(20);
		Engine_EventShowMessage(1, 0);
		Engine_EventWait(10);
		Engine_ActorSetAnimationAndWait(3, 4);
		Engine_EventWait(20);
		Engine_EventShowMessage(3, 0);
		Engine_EventWait(10);
		Engine_ActorSetAnimationAndWait(2, 3);
		Engine_EventWait(30);
		Engine_EventShowMessage(2, 0);
		Engine_EventWait(10);
		Actor_FaceActor(IWAN, ACTOR_PARTY_LEADER, 30);
		Event_OpenMessage(8194, 0);
	}
	Engine_EventSetMessage((s32)MsgRariberoDoNotWorryAboutSheba);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 0);
	Actor_FaceActor(GERALD, ACTOR_PARTY_LEADER, 20);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 49152, 0);
		Actor_FaceDirection(GERALD, 49152, 0);
		Engine_ActorFaceDirection(2, 49152, 0);
		Engine_EventWait(30);
		Engine_EventWait(10);
		Engine_ActorSetAnimationAndWait(11, 3);
		Engine_EventWait(30);
		Engine_EventShowMessage(11, 0);
		(gEventWork->message)++;
	}
	else
	{
		Engine_EventWait(20);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 49152, 0);
		Actor_FaceDirection(GERALD, 49152, 0);
		Engine_ActorFaceDirection(2, 49152, 0);
		Engine_EventWait(30);
		Engine_EventWait(10);
		Engine_ActorSetAnimationAndWait(11, 4);
		Engine_EventWait(20);
		(gEventWork->message)++;
		Event_ShowMessage(11, 0);
	}
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(11, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Engine_ActorFaceEachOther(GERALD, ACTOR_PARTY_LEADER, 0);
	Engine_ActorFaceEachOther(3, 2, 0);
	Engine_EventWait(40);
	Engine_EventWait(10);
	Actor_ShowEmote(11, 258, 40);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 49152, 0);
	Actor_FaceDirection(GERALD, 49152, 0);
	Actor_FaceDirection(IWAN, 49152, 0);
	Actor_FaceDirection(3, 49152, 0);
	Engine_EventWait(50);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(11, 3);
	Engine_EventWait(30);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(11, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(MEARI, 256, 40);
	Engine_EventShowMessage(3, 0);
	Engine_EventWait(20);
	Engine_ActorFaceDirection(11, 0, 0);
	Engine_EventWait(20);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(12, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(12, 32768, 0);
	Engine_EventWait(20);
	Engine_EventShowMessage(12, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(11, 4);
	Engine_EventWait(20);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(2, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(2, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(11, 16384, 0);
	Engine_EventWait(30);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(12, 258, 50);
	Engine_EventShowMessage(12, 0);
	Engine_EventWait(20);
	Actor_ShowEmote(11, 257, 65);
	Engine_ActorFaceDirection(11, 0, 0);
	Engine_EventWait(20);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(20);
	Actor_ShowEmote(12, 256, 40);
	Engine_EventShowMessage(12, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(12, 16384, 0);
	Engine_EventWait(40);
	Engine_EventShowMessage(12, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(11, 16384, 0);
	Engine_EventWait(50);
	Actor_ShowEmote(11, 262, 60);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(11, 3);
	Engine_EventWait(30);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorStartRepeatedMotion(MEARI, 2);
	Engine_ActorRunRepeatedMotion(2, 2);
	Engine_EventWait(30);
	Engine_EventWait(10);
	Engine_ActorFaceDirection(11, 0, 0);
	Engine_EventWait(20);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(12, 2);
	Engine_EventWait(30);
	Actor_FaceDirection(12, 32768, 0);
	Engine_EventWait(20);
	Engine_EventShowMessage(12, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(11, 257, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(1, 57344, 0);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(1, 4);
	Engine_EventWait(20);
	Engine_EventShowMessage(1, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(12, 16384, 0);
	Engine_EventWait(30);
	Actor_ShowEmote(12, 261, 60);
	Engine_EventShowMessage(12, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(1, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(1, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(11, 16384, 0);
	Engine_EventWait(40);
	Actor_ShowEmote(11, 258, 40);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Engine_ActorFaceDirection(2, 32768, 0);
	Engine_EventWait(30);
	Actor_ShowEmote(IWAN, 263, 60);
	Engine_ActorFaceDirection(2, 49152, 0);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(2, 4);
	Engine_EventWait(20);
	Engine_EventShowMessage(2, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(11, 4);
	Engine_EventWait(20);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorStartRepeatedMotion(MEARI, 2);
	Engine_ActorRunRepeatedMotion(2, 2);
	Engine_EventWait(30);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(11, 3);
	Engine_EventWait(30);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(2, 3);
	Engine_EventWait(30);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(11, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(1, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(1, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(11, 3);
	Engine_EventWait(30);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(3, 3);
	Engine_EventWait(30);
	Engine_EventShowMessage(3, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 0);
	Actor_FaceDirection(GERALD, 57344, 0);
	Engine_ActorFaceDirection(2, 40960, 0);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(0, 3);
	Engine_EventWait(10);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(3, 3);
	Engine_EventWait(30);
	Actor_SetSpeed(GERALD, 78643, 39321);
	Actor_SetSpeed(MEARI, 78643, 39321);
	Actor_SetSpeed(IWAN, 78643, 39321);
	Engine_ActorSetAnimation(1, 2);
	{
		u8 *rec = Object_GetById(ACTOR_PARTY_LEADER);
		if (rec != 0)
		{
			s16 y = *(s16 *)(rec + 10);
			s16 x = *(s16 *)(rec + 18);
			Engine_ActorSetDestination(1, y, x);
		}
	}
	Engine_ActorWaitForMove(1);
	Actor_SetPosition(GERALD, 0, 0);
	Engine_ActorSetAnimation(3, 2);
	{
		u8 *rec = Object_GetById(ACTOR_PARTY_LEADER);
		if (rec != 0)
		{
			s16 y = *(s16 *)(rec + 10);
			s16 x = *(s16 *)(rec + 18);
			Engine_ActorSetDestination(3, y, x);
		}
	}
	Engine_ActorWaitForMove(3);
	Actor_SetPosition(MEARI, 0, 0);
	Engine_ActorSetAnimation(2, 2);
	{
		u8 *rec = Object_GetById(ACTOR_PARTY_LEADER);
		if (rec != 0)
		{
			s16 y = *(s16 *)(rec + 10);
			s16 x = *(s16 *)(rec + 18);
			Engine_ActorSetDestination(2, y, x);
		}
	}
	Engine_ActorWaitForMove(2);
	Engine_ActorSetPosition(2, 0, 0);
	Engine_EventWait(10);
	Engine_EventEnd();
}

static void FieldScene_RunSecondaryScript(void)
{
	Engine_EventSetMessage((s32)MsgRariberoTellOthers);
	Engine_EventWait(20);

	Engine_ActorRunRepeatedMotion(11, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);

	Engine_ActorFaceActor(GERALD, ACTOR_PARTY_LEADER, 50);
	Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
	Engine_EventWait(10);
	Engine_ActorFaceDirection(GERALD, 0xc000, 0);
	Engine_EventWait(20);
	Engine_EventShowMessage(GERALD, 0);
	Engine_EventWait(10);

	Engine_ActorSetAnimationAndWait(MEARI, 4);
	Engine_EventWait(20);
	Engine_EventShowMessage(MEARI, 0);
	Engine_EventWait(10);

	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(IWAN, 0);
	Engine_EventWait(10);

	Engine_ActorRunRepeatedMotion(12, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(12, 0);
	Engine_EventWait(20);

	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(20);
	Engine_EventWait(25);

	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(30);
	Engine_EventShowMessage(GERALD, 0);
	Engine_EventWait(30);

	Engine_ActorSetAnimationAndWait(GERALD, 4);
	Engine_EventWait(20);
	Engine_EventShowMessage(GERALD, 0);
	Engine_EventWait(10);

	Engine_ActorSetAnimationAndWait(MEARI, 4);
	Engine_EventWait(20);
	Engine_EventShowMessage(MEARI, 0);
	Engine_EventWait(10);

	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(30);
	Engine_EventShowMessage(IWAN, 0);
	Engine_EventWait(10);

	Engine_ActorFaceActor(IWAN, ACTOR_PARTY_LEADER, 30);
	Engine_EventOpenMessage(0x2002, 0);
}

s32 Scene_Initialize(void)
{
	s32				   scene;
	s16				   entrance;
	struct FieldActor *actor;

	if (gGameState.entrance == ENTRANCE_FROM_AERIE)
	{
		Engine_GameFlagSet(FLAG_AERIE_EVENTS_DONE);
	}
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	scene = gGameState.scene;
	if (scene == (s32)&SceneId_RariberoHeya1)
	{
		actor = Object_GetById(ACTOR_SANCTUM_FIRST);
		actor->priority_flags = 0;
		actor->collision_flags |= 4;
		actor->sprite->priority = 2;
		actor = Object_GetById(ACTOR_SANCTUM_SECOND);
		actor->priority_flags = 0;
		actor->collision_flags |= 4;
		actor->sprite->priority = 2;
		actor = Object_GetById(ACTOR_SANCTUM_THIRD);
		actor->collision_flags |= 4;
		actor->priority_flags = 0;
		actor->sprite->priority = 2;
		Engine_ActorSetAnimation(15, 6);
		entrance = gGameState.entrance;
		if (entrance == ENTRANCE_SANCTUM_RETURN)
		{
			gGameState.saved_scene = scene;
			gGameState.saved_entrance = entrance;
		}
	}
	if (gGameState.scene == (s32)&SceneId_RariberoHeya2)
	{
		actor = Object_GetById(ACTOR_HOUSE_RESIDENT);
		actor->collision_flags |= 4;
		actor->priority_flags = 0;
		actor->sprite->priority = 2;
		if (Engine_GameFlagIsSet(0x300) != 0)
		{
			Engine_ActorEnableActionCallback(14, gRariberoPoseAction);
		}
		if (gGameState.entrance == ENTRANCE_HOUSE_REPORT)
		{
			FieldScene_RunPrimaryScript();
			Engine_ActorSetActionCallback(Object_GetById(ACTOR_HOUSE_REPORT_FIRST), 6);
			Engine_ActorSetActionCallback(Object_GetById(ACTOR_HOUSE_REPORT_SECOND), 6);
			gGameState.entrance = ENTRANCE_HOUSE_AFTER_REPORT;
		}
	}
	return 0;
}

s32 gRariberoPoseAction[] = {
	SC_SET(SW_COLLISION, 129),
	SC_SET(SW_SPEED, 0x26666),
	SC_SET(SW_ACCEL, 0x13333),
	SC_LABEL(1),
	SC_ARG(2),
	SC_MOVE(PIXELS(128), 0, PIXELS(300)),
	SC_SYNC,
	SC_SET(SW_FACING, 0xb000),
	SC_ARG(3),
	SC_WAIT(30),
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(300)),
	SC_SYNC,
	SC_SET(SW_FACING, 0xd000),
	SC_ARG(3),
	SC_WAIT(50),
	SC_ARG(2),
	SC_MOVE(PIXELS(128), 0, PIXELS(300)),
	SC_SYNC,
	SC_SET(SW_FACING, 0xb000),
	SC_ARG(3),
	SC_WAIT(20),
	SC_CALL(SceneActor_SetActor14Pose258),
	SC_WAIT(30),
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(300)),
	SC_SYNC,
	SC_SET(SW_FACING, 0xd000),
	SC_ARG(3),
	SC_WAIT(30),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct SceneEntrance gRariberoEntrances[] = {
	{ 1, CONDITION_ALWAYS, 144, 0, 168, FACING_NORTH, 0, 0, 8, 240, 192, 0 },
	{ 2, CONDITION_ALWAYS, 400, 0, 168, FACING_NORTH, 0, 256, 8, 496, 192, 0 },
	{ 3, CONDITION_ALWAYS, 648, 0, 72, FACING_NORTH, 0, 488, 8, 728, 184, 0 },
	{ 4, CONDITION_ALWAYS, 128, 0, 472, FACING_NORTH, 0, 0, 216, 240, 488, 0 },
	{ 5, CONDITION_ALWAYS, 136, 0, 296, FACING_SOUTH, 0, 0, 216, 240, 488, 0 },
	{ 6, CONDITION_ALWAYS, 368, 0, 472, FACING_NORTH, 0, 240, 216, 480, 488, 0 },
	{ 7, CONDITION_ALWAYS, 392, 0, 296, FACING_SOUTH, 0, 240, 216, 480, 488, 0 },
	{ 8, CONDITION_ALWAYS, 648, 0, 280, FACING_NORTH, 0, 480, 216, 720, 488, 0 },
	{ 9, CONDITION_ALWAYS, 96, 0, 680, FACING_NORTH, 0, 8, 520, 312, 696, 0 },
	{ 10, CONDITION_ALWAYS, 200, 0, 600, FACING_SOUTH, 0, 8, 520, 312, 696, 0 },
	{ 11, CONDITION_ALWAYS, 432, 0, 728, FACING_NORTH, 0, 312, 520, 552, 744, 0 },
	{ 12, CONDITION_ALWAYS, 432, 0, 630, FACING_NORTH, 0, 312, 520, 552, 744, 0 },
	{ 21, CONDITION_ALWAYS, 96, 0, 168, FACING_NORTH, 0, 8, 8, 312, 192, 0 },
	{ 22, CONDITION_ALWAYS, 216, 0, 88, FACING_SOUTH, 0, 8, 8, 312, 192, 0 },
	{ 23, CONDITION_ALWAYS, 224, 0, 392, FACING_NORTH, 0, 8, 232, 312, 416, 0 },
	{ 24, CONDITION_ALWAYS, 88, 0, 312, FACING_SOUTH, 0, 8, 232, 312, 416, 0 },
	{ 25, CONDITION_ALWAYS, 384, 0, 368, FACING_NORTH, 0, 320, 240, 632, 400, 0 },
	{ 26, CONDITION_ALWAYS, 456, 0, 376, FACING_NORTH, 0, 320, 240, 632, 400, 0 },
	{ 27, CONDITION_ALWAYS, 504, 0, 376, FACING_NORTH, 0, 320, 240, 632, 400, 0 },
	{ 28, CONDITION_ALWAYS, 472, 0, 72, FACING_SOUTH, 0, 344, 8, 584, 168, 0 },
	{ 29, CONDITION_ALWAYS, 744, 0, 72, FACING_SOUTH, 0, 632, 8, 872, 168, 0 },
	{ 30, CONDITION_ALWAYS, 832, 0, 872, FACING_NORTH, 0, 280, 504, 912, 896, 0 },
	{ 31, CONDITION_ALWAYS, 464, 0, 832, FACING_SOUTH, 0, 280, 504, 912, 896, 0 },
	{ 99, CONDITION_ALWAYS, 96, 0, 168, FACING_NORTH, 0, 8, 8, 312, 192, 0 },
	{ SCENE_TABLE_END },
};

s16 gRariberoSanctumRegions[] = {
	196, 28, 595, 204, 36, 603, -1, 10,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gRariberoRegions[] = {
	212, 28, 83, 220, 36, 91, -1, 22,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gRariberoExits[] = {
	SCENE_EXITS(179),
	SCENE_EXIT(1, 178, 2),
	SCENE_EXIT(2, 178, 10),
	SCENE_EXIT(3, 179, 5),
	SCENE_EXIT(4, 178, 5),
	SCENE_EXIT(5, 179, 3),
	SCENE_EXIT(6, 178, 4),
	SCENE_EXIT(7, 179, 8),
	SCENE_EXIT(8, 179, 7),
	SCENE_EXIT(9, 178, 9),
	SCENE_EXIT(10, 178, 11),
	SCENE_EXIT(11, 178, 6),
	SCENE_EXIT(0, 180, 0),
	SCENE_EXIT(21, 178, 7),
	SCENE_EXIT(22, 178, 8),
	SCENE_EXIT(23, 178, 3),
	SCENE_EXIT(24, 180, 25),
	SCENE_EXIT(25, 180, 24),
	SCENE_EXIT(26, 180, 28),
	SCENE_EXIT(27, 180, 29),
	SCENE_EXIT(28, 180, 26),
	SCENE_EXIT(29, 180, 27),
	SCENE_EXIT(30, 178, 1),
	SCENE_EXIT(31, 188, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement gRariberoPlacements[] = {
	{ 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(176), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(80), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x8f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(136), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(120), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x67, CONDITION_ALWAYS, 3,
		PIXELS(616), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(664), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x72, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(400), 0, PIXELS(416), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x72, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(550), 0, PIXELS(368), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(304), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(600), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x7c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(616), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x7d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(92), 0, PIXELS(616), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x76, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(68), 0, PIXELS(340), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(432), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gRariberoHeyaPlacements[] = {
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(240), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x94, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(88), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x46, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(-64), 0, PIXELS(-64), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x39, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(-64), 0, PIXELS(-64), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(336), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x80, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(300), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(544), 0, PIXELS(368), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gRariberoHeyaPlacements9a7[] = {
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(240), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x94, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x46, CONDITION_ALWAYS, 6,
		PIXELS(104), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x39, CONDITION_ALWAYS, 6,
		PIXELS(128), 0, PIXELS(96), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(336), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x80, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(160), 0, PIXELS(304), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(544), 0, PIXELS(368), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gRariberoEvents[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaOfCourseIHaveSeenActor) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaThereAreSomeWhoDoNot) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaCouldFaranHaveTrickedUsAll) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIfActorCantUseHerPowers) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaActorCameFlyingToTheRuins) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaEvenActorCouldntHaveEscapedSix) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaTheySayAManNamedIodem) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaNoOnesCalledMeSoMaybe) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaActorSaidTheCaveAtThe) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaTheRuinsThatFormTheLighthouse) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaActorIfThePowerThatYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaActorIsTheSaviorOfLalivero) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaALegendarySaviorWillAppearIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIdLikeToBelieveInActor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaEvenThoughSheFellOutOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIfSomethingHappensToActorWhat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaWhenIodemGetsHereHesGoing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaItsBeenSoLongSinceI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIWonderHowActorKnewAbout) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaThoseCavesAreTooDangerousFor) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaTheLaliveransAreATimidPeople) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaICantRescueActorIfI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIfAnythingHappensToActorIll) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIBetBabisTowerWillBe) },
	{ SEARCH(SEARCH_BARREL), 100, 0xfb9, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_BARREL), 101, 0xfba, FIND_ITEM(ITEM_ANTIDOTE) },
	{ SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgRariberoHeyaShesMakingMushroomSoupInLalivero)) },
	{ SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgRariberoHeyaItsALaliveroSaladWithPotato)) },
	{ SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgRariberoHeyaThePotIsFullOfA)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gRariberoEvents9a7[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaTheContinentBrokeApartAroundThe) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaWhatHappenedToThoseLivingOn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIShudderToThinkWhatWould) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaWereLuckyThatWeLiveNorth) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaEveryoneSaysThatActorIsOkay) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIRealizedHowThoughtlessIWas) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIfSheIsIndeedTheChild) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaWeWantToBelieveInSomething) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaActorShouldBeFineSheWas) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaActorWillWakeWhenTheBeacon) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaSeeingActorDiveMustHaveBeen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaICantWaitUntilActorIs) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaBabiLighthouseCrumbledWhenAllThe) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIAlwaysWantedABreakBut) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaSoTheRuinsReallyAreConnected) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaYouFinallySolvedTheMysteryOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaWillTheyContinueOnOrQuit) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaImWorriedThatWeWontGet) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaWeCantPossiblyDigAHole) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaTheMysteryOfTheRuinsShould) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaNoOneCouldPossiblySwimIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaWhenWillTheOceanBeCalm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaTheChampaArePeopleFromA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaFirstTheLighthouseWasStruckDown) },
	{ SEARCH(SEARCH_BARREL), 100, 0xfb9, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_BARREL), 101, 0xfba, FIND_ITEM(ITEM_ANTIDOTE) },
	{ SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgRariberoHeyaShesMakingMushroomSoupInLalivero)) },
	{ SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgRariberoHeyaItsALaliveroSaladWithPotato)) },
	{ SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgRariberoHeyaThePotIsFullOfA)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gRariberoHeyaEvents[] = {
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 28, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 29, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaWeReceivedActorFromTheSky) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIJustKnowThatFatherWill) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaHisWifeSeemsVeryCalmConsidering) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaItsSoPainfulToEvenThink) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIJustWishSomeoneWouldPlease) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaThatCoupleRaisedActorAsThough) },
	{ EVENT_TALK, 14, 0x300, EVENT_SCRIPT(RariberoScene_PlayPoseSequence) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40e, 0x300, EVENT_SCRIPT(RariberoScene_PlayPoseSequence) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaWorkingLikeThisKeepsMyMind) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaWeMustObeyTolbiThatsWhat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaWhyDidThisHaveToHappen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaHousecleaningChangingSheetsItsAllDone) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaOnceThisIncidentIsOverI) },
	{ SEARCH(SEARCH_BARREL), 102, 0xfbb, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgRariberoHeyaShesFryingFreshwaterFishFromThe)) },
	{ SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgRariberoHeyaItsRiceMadeWithGrainsFrom)) },
	{ SEARCH(SEARCH_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgRariberoHeyaShesMakingRoastChickenThisScent)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gRariberoHeyaEvents9a7[] = {
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 28, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 29, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(RariberoHeya_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 51, 0x9bc, EVENT_SCRIPT(EV51_TOUCH) },
	{ EVENT_TALK, 12, 0x9ba, EVENT_SCRIPT(EV12_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x9ba, MSGNO(MsgRariberoHeyaIMustExplainTheirPromiseTo) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaFaranSaysKnowingHerPowersActor) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaYouAreLeavingAfterAllArent) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaTheySayTheLighthouseCrumbledDown) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaYouWillFindJasminSomedayFarewell) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaRobinThatBoatBelongsToYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIDontNeedFameOrFortune) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaWhyWontAnyoneStopRobinAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaTolbiIsToBeBlamedFor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaSoThereAreSomePeopleLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaPleaseUseThatShipToFind) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaIMustSayItIsNice) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaFishingIsSoDifficultNowadaysBecause) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaWhyWouldTheLightingMakeSuch) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaImSleepyTheseBrightNightsHave) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgRariberoHeyaGeeWhatAmIGoingTo) },
	{ SEARCH(SEARCH_BARREL), 102, 0xfbb, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgRariberoHeyaShesFryingFreshwaterFishFromThe)) },
	{ SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgRariberoHeyaItsRiceMadeWithGrainsFrom)) },
	{ SEARCH(SEARCH_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgRariberoHeyaShesMakingRoastChickenThisScent)) },
	{ SCENE_EVENTS_END },
};
