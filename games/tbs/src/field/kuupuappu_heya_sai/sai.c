
#include "game_state.h"
#include "unit/field/kuupuappu_heya_sai/sai.h"
#include "unit/battle/effect/object.h"
#include "unit/game/character/chara.h"
#include "unit/game/character/chara4.h"
#include "unit/game/character/chara5.h"
#include "unit/game/character/chara6.h"
#include "unit/game/character/chara7.h"
#include "unit/game/character/chara8.h"
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
#include "resource_386_state.h"
#include "inventory.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"

// 前方宣言
static void OverlayObject_InitObject22(s32 a, s32 fixed_x, s32 fixed_y, s32 fixed_z);

static void SceneState_CheckPositionWindow(void)
{
	s32 v1;
	s32 v0;

	v0 = ((struct Resource386FirstView *)Object_GetById(0))->sample_08;
	v1 = ((struct Resource386SecondView *)Object_GetById(0))->sample_10 >> 0x14;
	if ((((v0 >> 0x14) - 0x22) <= 1U) && (v1 > 0x28) && (v1 <= 0x2A))
	{
		Engine_GameFlagSet(0x250);
		return;
	}
	Engine_GameFlagClear(0x250);
}

u8 *SceneData_GetScriptTable(void)
{
	return (u8 *)Placement_Scripts;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return (u8 *)Placement_Messages;
}

u8 *SceneData_GetPreparedActors(void)
{
	u8 *slot = (u8 *)Placement_Actors;

	FieldScene_PrepareActors(slot);
	return slot;
}

static void SceneActor_RunActorStep(s32 arg0)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(arg0, 1);
	Engine_EventShowMessage(arg0, 0);
	Engine_EventEnd();
}

static void EV09_TALK(void)
{
	Engine_EventSetMessage((s32)MsgKuupuappuThoughtSawCatGetUp);
	Engine_ActorFaceEachOther(9, ACTOR_PARTY_LEADER, 2);
	SceneActor_RunActorStep(9);
}

static void EV11_TALK(void)
{
	Engine_EventSetMessage((s32)MsgKuupuappuDodonpasHenchmenNeverAttackedUs);
	Engine_ActorFaceEachOther(11, ACTOR_PARTY_LEADER, 2);
	SceneActor_RunActorStep(11);
}

static void EV12_TALK(void)
{
	Engine_EventSetMessage((s32)MsgKuupuappuItsNearTimeForColosso);
	Engine_ActorFaceEachOther(12, ACTOR_PARTY_LEADER, 2);
	SceneActor_RunActorStep(12);
}

static void FieldScene_RunActor16Sequence(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuRobinYouveComeBackVault);
	Engine_ActorFaceEachOther(16, ACTOR_PARTY_LEADER, 2);
	Engine_ActorSetAnimation(16, 1);
	Event_ShowMessageAndWait(16, 0, 20);
	Engine_ActorSetAnimationAndWait(16, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(16, 0, 20);
	Actor_ShowEmote(16, 0x102, 60);
	Event_ShowMessageAndWait(16, 0, 30);
	Event_OpenMessage(16, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		gEventWork->message++;
	}
	Event_ShowMessageAndWait(16, 0, 20);
	GameFlag_Set(0x300);
	GameFlag_Set(0x868);
	Engine_EventEnd();
}

static void SceneDialogue_RunActor16Line(void)
{
	Engine_EventSetMessage((s32)MsgKuupuappuThoseGuysReallyWantedRevenge);
	Engine_ActorFaceEachOther(16, ACTOR_PARTY_LEADER, 2);
	SceneActor_RunActorStep(16);
}

static void EV23_TALK(void)
{
	Engine_EventSetMessage((s32)MsgKuupuappuDidSomeCookingNowIve);
	Engine_ActorFaceEachOther(23, ACTOR_PARTY_LEADER, 2);
	SceneActor_RunActorStep(23);
}

static void EV18_TALK(void)
{
	Engine_EventBegin();
	Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 0);
	if (GameFlag_IsSet(0x85b) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuHaveLotLeftoverBonesFrom);
		Event_OpenMessage(18, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuWantMoreBones);
		Event_OpenMessage(18, 0);
	}
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Event_ShowMessage(18, 0);
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(18, 2);
		Engine_EventWait(20);
		if (PartyInventory_HasSpace() == 0)
		{
			Engine_ActorSetAnimationAndWait(18, 4);
			Engine_EventWait(20);
			Engine_EventSetMessage((s32)MsgKuupuappuWowHaveManyThingsArent);
			Event_ShowMessage(18, 0);
			goto L_020002d4;
		}
		Engine_ItemShowFound(ITEM_BONE, 3);
		Engine_PartyGiveItem(ITEM_BONE, 0);
		GameFlag_Set(0x85b);
	}
	else
	{
		gEventWork->message++;
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(18, 3);
		Engine_EventWait(20);
		Event_ShowMessage(18, 0);
	}
L_020002d4:;
	Actor_FaceDirection(18, 0x4000, 0);
	Engine_EventEnd();
}

static void EV1040_PSYNERGY(void)
{
	u8 *slot;
	u8	clear = 0;

	Engine_EventBegin();
	Engine_ActorRunRepeatedMotion(16, 1);
	Engine_EventEnd();
	slot = &Object_GetById(16)->paused;
	*slot = 1;
	FieldScene_RunActor16Sequence();
	slot = &Object_GetById(16)->paused;
	*slot = clear;
	Engine_ActorEnableActionCallback(16, 2);
}

static void EV41_SEARCH(void)
{
	Engine_EventBegin();

	if (PartyInventory_HasSpace() == 0)
	{
		Engine_ActorSetAnimationAndWait(18, 4);
		Engine_EventWait(20);
		Engine_EventSetMessage((s32)MsgKuupuappuWowHaveManyThingsArent);
		Engine_EventShowMessage(18, 0);
	}
	else
	{
		Engine_ItemShowFound(ITEM_BONE, 3);
		Engine_PartyGiveItem(ITEM_BONE, 0);
	}

	Engine_EventEnd();
}

static void EV19_TALK(void)
{
	u32 place;

	place = Object_GetById(0)->facing;
	Engine_EventBegin();

	if (place + 0xFFFF5FFF <= 0x3FFE)
	{
		Engine_ShopOpen(4, 19);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuTheresRiverFireOnOther);
		Engine_EventShowMessage(19, 0);
	}

	Engine_EventEnd();
}

static void EV20_TALK(void)
{
	u32 place;

	place = Object_GetById(0)->facing;
	Engine_EventBegin();

	if (place + 0xFFFF5FFF <= 0x3FFE)
	{
		Engine_ShopOpen(5, 20);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuTheySayVolcanoOnMt);
		Engine_EventShowMessage(20, 0);
	}

	Engine_EventEnd();
}

static void EV21_TALK(void)
{
	u32 place;

	place = Object_GetById(0)->facing;
	Engine_EventBegin();

	if (place + 0xFFFF5FFF <= 0x3FFE)
	{
		Engine_ShopOpen(6, 21);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuWeHaventBeenGettingMany);
		Engine_EventShowMessage(21, 0);
	}

	Engine_EventEnd();
}

static void EV22_TALK(void)
{
	u32 place;

	place = Object_GetById(0)->facing;
	Engine_EventBegin();

	if (place + 0xFFFF5FFF <= 0x3FFE)
	{
		Engine_InnOpen(1, 22);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuMmmmNothingDoNothingDo);
		Engine_EventShowMessage(22, 0);
	}

	Engine_EventEnd();
}

static void EV18_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x85B) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuGeeAlwaysGetHungryWhen);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuHeReallyLikesBonesWonder);
	}
	Engine_EventShowMessage(18, 0);
	Engine_EventEnd();
}

u8 *SceneData_GetEffectTable(void)
{
	return (u8 *)Placement_Effects;
}

s32 FieldScene_InitSceneStateByStep(void)
{
	s32 scene;
	s32 zero;

	gEventWork->start_transition = 521;
	scene = gGameState.entrance;

	if (scene == 5)
	{
		s32 fifth = 4;
		s32 sixth = 3;

		Engine_MapCopyCellsTo(0, 120, 8, 67, fifth, sixth);
		zero = 0;
		Object_GetById(8)->motion_flags = zero;
		Object_GetById(8)->y.fixed = zero;
		Object_GetById(8)->terrain_height = zero;
	}
	else if (scene == 7 || scene == 11)
	{
		OverlayObject_InitObject22(0xe7, 0x02380000, 0x00100000, 0x02a00000);
		Engine_TaskAddCallback(SceneState_CheckPositionWindow, 0xc80);
	}

	return 0;
}

static void OverlayObject_InitObject22(s32 a, s32 fixed_x, s32 fixed_y, s32 fixed_z)
{
	u8 *o;
	u8 *q;
	u8 *p;
	u8 *v;
	s32 z;
	s32 m;

	z = 0;
	o = (u8 *)Engine_ObjectCreate(22, fixed_x, fixed_y, fixed_z);
	if (o != 0)
	{
		q = *(u8 **)(o + 0x50);
		p = q + 38;
		*p = z;
		p += 1;
		*p = z;
		m = 33;
		m = -m;
		q[5] &= m;
		q[9] &= 15;
		o[0x55] = z;
		o[0x5c] = 1;
		v = Engine_HeapAllocate(17, 0x608);
		Engine_ItemLoadIcon(a);
		v += 0x400;
		Engine_VramLoad(q[28], 0x80, v);
		Engine_HeapRelease(17);
	}
}

struct SceneEntrance Placement_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 192, 0, 220, FACING_NORTH, 0, 32, 0, 272, 240, 0 },
	{ 6, CONDITION_ALWAYS, 432, 0, 254, FACING_NORTH, 0, 288, 0, 528, 272, 0 },
	{ 7, CONDITION_ALWAYS, 720, 0, 782, FACING_NORTH, 0, 432, 576, 800, 800, 0 },
	{ 8, CONDITION_ALWAYS, 624, 0, 236, FACING_NORTH, 0, 560, 32, 848, 256, 0 },
	{ 9, CONDITION_ALWAYS, 144, 0, 492, FACING_NORTH, 0, 16, 288, 256, 512, 0 },
	{ 10, CONDITION_ALWAYS, 416, 0, 526, FACING_NORTH, 0, 320, 288, 592, 544, 0 },
	{ 11, CONDITION_ALWAYS, 744, 0, 640, FACING_SOUTH, 0, 432, 576, 800, 800, 0 },
	{ 12, CONDITION_ALWAYS, 358, 0, 632, FACING_SOUTH, 0, 32, 576, 400, 776, 0 },
	{ SCENE_TABLE_END },
};

u32 Placement_Messages[] = {
	SCENE_EXITS(24),
	SCENE_EXIT(5, 23, 5),
	SCENE_EXIT(6, 23, 6),
	SCENE_EXIT(7, 23, 7),
	SCENE_EXIT(8, 23, 8),
	SCENE_EXIT(9, 23, 9),
	SCENE_EXIT(10, 23, 10),
	SCENE_EXIT(11, 24, 11),
	SCENE_EXIT(12, 24, 12),
	SCENE_EXITS_END,
};

struct ScenePlacement Placement_Actors[] = {
	{ 0x6c, 0x0, ACTOR_STAND,
		PIXELS(160), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, 0x0, 3,
		PIXELS(152), 0, PIXELS(72), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x65, 0x0, ACTOR_STAND,
		PIXELS(456), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x6b, 0x0, ACTOR_WANDER,
		PIXELS(368), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x67, 0x0, ACTOR_WANDER,
		PIXELS(744), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(752), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6b, 0x0, ACTOR_STAND,
		PIXELS(496), 0, PIXELS(416), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x66, 0x0, ACTOR_STAND,
		PIXELS(512), 0, PIXELS(416), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x75, 0x0, ACTOR_STAND,
		PIXELS(536), 0, PIXELS(451), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x73, 0x0, ACTOR_STAND,
		PIXELS(416), 0, PIXELS(464), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, 0x0, ACTOR_STAND,
		PIXELS(568), 0, PIXELS(648), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x7c, 0x0, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7d, 0x0, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x76, 0x0, ACTOR_STAND,
		PIXELS(632), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x77, 0x0, ACTOR_STAND,
		PIXELS(696), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x80, 0x0, ACTOR_WANDER,
		PIXELS(248), 0, PIXELS(712), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent Placement_Effects[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 12 },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWaahACatAteMyPrecious) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuThePrisonersSaidTheyWantedTo) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TALK) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKuupuappuTheWarriorsISawFightingAt) },
	{ EVENT_TALK, 16, 0x300, EVENT_SCRIPT(FieldScene_RunActor16Sequence) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor16Line) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuTheMayorsFamilyUrnIsOn) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgKuupuappuThoseThievesYouGoToAll) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIHearThatABigFestival) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_TALK) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWeDontGetALotOf) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EV23_TALK) },
	{ EVENT_TALK, 18, 0x250, EVENT_SCRIPT(EV18_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIfIdKnownThisWasGoing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKuupuappuMyHusbandSaysMyCatAte) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWhyDoYouSupposeThoseThieves) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKuupuappuDaddyHasntBeenPayingAttentionSince) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWheneverMyWifeUsedToGo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKuupuappuLatelyMyHusbandsBeenGoingOn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x410, 0x300, EVENT_SCRIPT(EV1040_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgKuupuappuAsLongAsWeStayAlert) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWellBeSecuringItInA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKuupuappuItsJustEmbarrassingThatWeLet) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKuupuappuILoveFestivalsIllBetColosso) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKuupuappuJustImagineHowTerribleItWould) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgKuupuappuItsAGoodThingThatGiant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWhyKeepTheStoreOpenIf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWontAnyoneComeStayTheNight) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgKuupuappuAfterYouveBeenInThisBusiness) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_PSYNERGY) },
	{ SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(EV41_SEARCH) },
	{ SEARCH(SEARCH_JAR), 100, 0xf4b, FIND_COINS(7) },
	{ SEARCH(SEARCH_BARREL), 101, 0xf4c, FIND_COINS(4) },
	{ SEARCH(SEARCH_BARREL), 102, 0xf4d, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SEARCH(SEARCH_WOODEN_BOX), 103, 0xf4e, FIND_ITEM(0xc3) },
	{ 0xc4f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKuupuappuTheresABookEntitledHistoryOf)) },
	{ SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKuupuappuTheresABookEntitledGemsOfAngara)) },
	{ SCENE_EVENTS_END },
};
