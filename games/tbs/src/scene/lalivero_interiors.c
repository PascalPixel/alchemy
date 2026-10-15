#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/lalivero_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psyfx.h"
#include "unit/game/game.h"
#include "unit/shop/shop.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "lalivero_interiors.h"
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

u8 *LaliveroInteriors_GetEntrances(void)
{
	return gLaliveroEntrances;
}

u8 *LaliveroInteriors_GetRegions(void)
{
	if (gGameState.scene == (s32)&SceneId_LaliveroInteriors1)
	{
		return gLaliveroSanctumRegions;
	}
	return gLaliveroRegions;
}

u8 *LaliveroInteriors_GetExits(void)
{
	return gLaliveroExits;
}

u8 *LaliveroInteriors_GetPlacements(void)
{
	if (gGameState.scene == (s32)&SceneId_LaliveroInteriors2)
	{
		if (Engine_GameFlagIsSet(0x9A7) != 0)
		{
			return gLaliveroInteriorsPlacements9a7;
		}
		return gLaliveroInteriorsPlacements;
	}
	return gLaliveroPlacements;
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
		Engine_EventSetMessage(MsgLaliveroWentOceanLook);
		Engine_EventShowMessage(keeper, 0);
	}
	else
	{
		s32 message = MsgLaliveroTakenHostageFaran;

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
		Engine_EventSetMessage(MsgLaliveroWantedSendShips);
		Engine_EventShowMessage(no, 0);
	}
	else
	{
		Engine_EventSetMessage(MsgLaliveroWarriorRightPlease);
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
		Engine_EventSetMessage(MsgLaliveroImpossibleGoOcean);
		Engine_EventShowMessage(no, 0);
	}
	else
	{
		Engine_EventSetMessage(MsgLaliveroGuessPeopleAngara);
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
		Engine_EventSetMessage(MsgLaliveroBabiLighthouseStruck);
		Engine_EventShowMessage(no, 0);
	}
	else
	{
		Engine_EventSetMessage(MsgLaliveroBabiIsntBuilding);
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
		Engine_EventSetMessage(MsgLaliveroDidntKnowTime);
		Engine_EventShowMessage(no, 0);
	}
	else
	{
		Engine_EventSetMessage(MsgLaliveroWantStayLet);
		Engine_EventShowMessage(no, 0);
	}
}

static void LaliveroScene_PlayPoseSequence(void)
{
	s16 facing;

	facing = (Actor_Get(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
	GameFlag_Set(0x300);
	Engine_EventBegin();
	Engine_ResetSceneEffectCounter();
	Engine_EventSetMessage(MsgLaliveroImSorry);
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
	Engine_ActorEnableActionCallback(14, gLaliveroPoseAction);
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
	Engine_EventSetMessage(MsgLaliveroPleaseWaitForMeOutside);
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
	Engine_EventSetMessage(MsgLaliveroPleaseWaitForMeOutside);
	Engine_EventShowMessage(0xC, 0);
}

static void LaliveroInteriors_RequestTouchedExitAndPlayCue(void)
{
	struct EventWork *work = gEventWork;
	s16				 *p = (s16 *)&work->touched_trigger;

	Engine_EventRequestExit(*p);
	Engine_AudioPlayCue(0x7B);
}

u8 *LaliveroInteriors_GetEvents(void)
{
	if (gGameState.scene == (s32)&SceneId_LaliveroInteriors2)
	{
		if (Engine_GameFlagIsSet(0x9a7) != 0)
		{
			return gLaliveroInteriorsEvents9a7;
		}
		return gLaliveroInteriorsEvents;
	}
	if (Engine_GameFlagIsSet(0x9a7) != 0)
	{
		return gLaliveroEvents9a7;
	}
	return gLaliveroEvents;
}

static void FieldScene_RunPrimaryScript(void)
{

	Engine_EventBegin();
	Owner_RefreshActiveRatios(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 6291456, 12058624);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 49152, 0);
	Actor_FaceDirection(11, 0, 0);
	Actor_FaceDirection(12, 32768, 0);
	Engine_EventSetMessage(MsgLaliveroHowDidSearchForSheba);
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
		Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 50);
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
		Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 30);
		Event_OpenMessage(8194, 0);
	}
	Engine_EventSetMessage(MsgLaliveroDoNotWorryAboutSheba);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 0);
	Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 20);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 49152, 0);
		Actor_FaceDirection(GARET, 49152, 0);
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
		Actor_FaceDirection(GARET, 49152, 0);
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
	Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
	Engine_ActorFaceEachOther(3, 2, 0);
	Engine_EventWait(40);
	Engine_EventWait(10);
	Actor_ShowEmote(11, 258, 40);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 49152, 0);
	Actor_FaceDirection(GARET, 49152, 0);
	Actor_FaceDirection(IVAN, 49152, 0);
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
	Actor_ShowEmote(MIA, 256, 40);
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
	Engine_ActorStartRepeatedMotion(GARET, 2);
	Engine_ActorStartRepeatedMotion(MIA, 2);
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
	Actor_ShowEmote(IVAN, 263, 60);
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
	Engine_ActorStartRepeatedMotion(GARET, 2);
	Engine_ActorStartRepeatedMotion(MIA, 2);
	Engine_ActorRunRepeatedMotion(2, 2);
	Engine_EventWait(30);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(11, 3);
	Engine_EventWait(30);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GARET, 3);
	Engine_ActorSetAnimation(MIA, 3);
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
	Actor_FaceDirection(GARET, 57344, 0);
	Engine_ActorFaceDirection(2, 40960, 0);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(0, 3);
	Engine_EventWait(10);
	Engine_ActorSetAnimation(GARET, 3);
	Engine_ActorSetAnimation(IVAN, 3);
	Engine_ActorSetAnimationAndWait(3, 3);
	Engine_EventWait(30);
	Actor_SetSpeed(GARET, 78643, 39321);
	Actor_SetSpeed(MIA, 78643, 39321);
	Actor_SetSpeed(IVAN, 78643, 39321);
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
	Actor_SetPosition(GARET, 0, 0);
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
	Actor_SetPosition(MIA, 0, 0);
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
	Engine_EventSetMessage(MsgLaliveroTellOthers);
	Engine_EventWait(20);

	Engine_ActorRunRepeatedMotion(11, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(11, 0);
	Engine_EventWait(10);

	Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 50);
	Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
	Engine_EventWait(10);
	Engine_ActorFaceDirection(GARET, 0xc000, 0);
	Engine_EventWait(20);
	Engine_EventShowMessage(GARET, 0);
	Engine_EventWait(10);

	Engine_ActorSetAnimationAndWait(MIA, 4);
	Engine_EventWait(20);
	Engine_EventShowMessage(MIA, 0);
	Engine_EventWait(10);

	Engine_ActorRunRepeatedMotion(IVAN, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(IVAN, 0);
	Engine_EventWait(10);

	Engine_ActorRunRepeatedMotion(12, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(12, 0);
	Engine_EventWait(20);

	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(20);
	Engine_EventWait(25);

	Engine_ActorRunRepeatedMotion(GARET, 2);
	Engine_EventWait(30);
	Engine_EventShowMessage(GARET, 0);
	Engine_EventWait(30);

	Engine_ActorSetAnimationAndWait(GARET, 4);
	Engine_EventWait(20);
	Engine_EventShowMessage(GARET, 0);
	Engine_EventWait(10);

	Engine_ActorSetAnimationAndWait(MIA, 4);
	Engine_EventWait(20);
	Engine_EventShowMessage(MIA, 0);
	Engine_EventWait(10);

	Engine_ActorSetAnimationAndWait(IVAN, 3);
	Engine_EventWait(30);
	Engine_EventShowMessage(IVAN, 0);
	Engine_EventWait(10);

	Engine_ActorFaceActor(IVAN, ACTOR_PARTY_LEADER, 30);
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
	if (scene == (s32)&SceneId_LaliveroInteriors1)
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
	if (gGameState.scene == (s32)&SceneId_LaliveroInteriors2)
	{
		actor = Object_GetById(ACTOR_HOUSE_RESIDENT);
		actor->collision_flags |= 4;
		actor->priority_flags = 0;
		actor->sprite->priority = 2;
		if (Engine_GameFlagIsSet(0x300) != 0)
		{
			Engine_ActorEnableActionCallback(14, gLaliveroPoseAction);
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

s32 gLaliveroPoseAction[] = {
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

struct SceneEntrance gLaliveroEntrances[] = {
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

s16 gLaliveroSanctumRegions[] = {
	196, 28, 595, 204, 36, 603, -1, 10,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gLaliveroRegions[] = {
	212, 28, 83, 220, 36, 91, -1, 22,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gLaliveroExits[] = {
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

struct ScenePlacement gLaliveroPlacements[] = {
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

struct ScenePlacement gLaliveroInteriorsPlacements[] = {
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

struct ScenePlacement gLaliveroInteriorsPlacements9a7[] = {
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
	{ GARET, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ IVAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ MIA, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gLaliveroEvents[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsOfCourseIHaveSeenActor) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThereAreSomeWhoDoNot) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsCouldFaranHaveTrickedUsAll) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfActorCantUseHerPowers) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorCameFlyingToTheRuins) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEvenActorCouldntHaveEscapedSix) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheySayAManNamedIodem) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsNoOnesCalledMeSoMaybe) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorSaidTheCaveAtThe) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheRuinsThatFormTheLighthouse) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorIfThePowerThatYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorIsTheSaviorOfLalivero) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsALegendarySaviorWillAppearIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIdLikeToBelieveInActor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEvenThoughSheFellOutOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfSomethingHappensToActorWhat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhenIodemGetsHereHesGoing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsItsBeenSoLongSinceI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIWonderHowActorKnewAbout) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThoseCavesAreTooDangerousFor) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheLaliveransAreATimidPeople) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsICantRescueActorIfI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfAnythingHappensToActorIll) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIBetBabisTowerWillBe) },
	{ SEARCH(SEARCH_BARREL), 100, 0xfb9, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_BARREL), 101, 0xfba, FIND_ITEM(ITEM_ANTIDOTE) },
	{ SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingMushroomSoupInLalivero)) },
	{ SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsALaliveroSaladWithPotato)) },
	{ SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsThePotIsFullOfA)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gLaliveroEvents9a7[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheContinentBrokeApartAroundThe) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhatHappenedToThoseLivingOn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIShudderToThinkWhatWould) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWereLuckyThatWeLiveNorth) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEveryoneSaysThatActorIsOkay) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIRealizedHowThoughtlessIWas) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfSheIsIndeedTheChild) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeWantToBelieveInSomething) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorShouldBeFineSheWas) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorWillWakeWhenTheBeacon) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSeeingActorDiveMustHaveBeen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsICantWaitUntilActorIs) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsBabiLighthouseCrumbledWhenAllThe) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIAlwaysWantedABreakBut) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSoTheRuinsReallyAreConnected) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouFinallySolvedTheMysteryOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWillTheyContinueOnOrQuit) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsImWorriedThatWeWontGet) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeCantPossiblyDigAHole) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheMysteryOfTheRuinsShould) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsNoOneCouldPossiblySwimIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhenWillTheOceanBeCalm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheChampaArePeopleFromA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFirstTheLighthouseWasStruckDown) },
	{ SEARCH(SEARCH_BARREL), 100, 0xfb9, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_BARREL), 101, 0xfba, FIND_ITEM(ITEM_ANTIDOTE) },
	{ SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingMushroomSoupInLalivero)) },
	{ SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsALaliveroSaladWithPotato)) },
	{ SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsThePotIsFullOfA)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gLaliveroInteriorsEvents[] = {
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 28, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 29, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeReceivedActorFromTheSky) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIJustKnowThatFatherWill) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsHisWifeSeemsVeryCalmConsidering) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsItsSoPainfulToEvenThink) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIJustWishSomeoneWouldPlease) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThatCoupleRaisedActorAsThough) },
	{ EVENT_TALK, 14, 0x300, EVENT_SCRIPT(LaliveroScene_PlayPoseSequence) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40e, 0x300, EVENT_SCRIPT(LaliveroScene_PlayPoseSequence) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWorkingLikeThisKeepsMyMind) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeMustObeyTolbiThatsWhat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyDidThisHaveToHappen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsHousecleaningChangingSheetsItsAllDone) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsOnceThisIncidentIsOverI) },
	{ SEARCH(SEARCH_BARREL), 102, 0xfbb, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesFryingFreshwaterFishFromThe)) },
	{ SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsRiceMadeWithGrainsFrom)) },
	{ SEARCH(SEARCH_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingRoastChickenThisScent)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gLaliveroInteriorsEvents9a7[] = {
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 28, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 29, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(LaliveroInteriors_RequestTouchedExitAndPlayCue) },
	{ EVENT_TOUCH, 51, 0x9bc, EVENT_SCRIPT(EV51_TOUCH) },
	{ EVENT_TALK, 12, 0x9ba, EVENT_SCRIPT(EV12_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x9ba, MSGNO(MsgLaliveroInteriorsIMustExplainTheirPromiseTo) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFaranSaysKnowingHerPowersActor) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouAreLeavingAfterAllArent) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheySayTheLighthouseCrumbledDown) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouWillFindJennaSomedayFarewell) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIsaacThatBoatBelongsToYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIDontNeedFameOrFortune) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyWontAnyoneStopIsaacAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTolbiIsToBeBlamedFor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSoThereAreSomePeopleLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsPleaseUseThatShipToFind) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIMustSayItIsNice) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFishingIsSoDifficultNowadaysBecause) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyWouldTheLightingMakeSuch) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsImSleepyTheseBrightNightsHave) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsGeeWhatAmIGoingTo) },
	{ SEARCH(SEARCH_BARREL), 102, 0xfbb, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesFryingFreshwaterFishFromThe)) },
	{ SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsRiceMadeWithGrainsFrom)) },
	{ SEARCH(SEARCH_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingRoastChickenThisScent)) },
	{ SCENE_EVENTS_END },
};
