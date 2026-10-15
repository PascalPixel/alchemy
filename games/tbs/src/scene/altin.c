#include "game_state.h"
#include "scene_ids.h"
#include "unit/scene/altin.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "engine_call.h"
#include "field_scene.h"
#include "altin.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

// 前方宣言
static void FieldScene_RunMiddleSequence(void);
static void FieldScene_RunScene3a3SequenceD(void);
static void SceneEffect_SpawnDriftingParticle(void);

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 v = gGameState.scene;

	if (v == (s32)&SceneId_Altin1)
	{
		return gAltinEntrances1;
	}
	if (v == (s32)&SceneId_Altin2)
	{
		return gAltinEntrances2;
	}
	return gAltinEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return (u8 *)gPlacement_Messages;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s32 id = gGameState.scene;
	if (id == (s32)&SceneId_Altin1)
	{
		if (Engine_GameFlagIsSet(0x909))
		{
			gAltinPlacements1[5].talkFacing = TALK_FACE_PARTY;
			gAltinPlacements1[6].talkFacing = TALK_FACE_PARTY;
		}
		return gAltinPlacements1;
	}
	if (id == (s32)&SceneId_Altin2)
	{
		if (Engine_GameFlagIsSet(0x8fd))
			gAltinPlacements2[1].talkFacing = TALK_FACE_PARTY_AND_BACK;
		if (Engine_GameFlagIsSet(0x8fe) || Engine_GameFlagIsSet(0x907))
			gAltinPlacements2[3].talkFacing = TALK_FACE_PARTY_AND_BACK;
		FieldScene_PrepareActors(gAltinPlacements2);
		return gAltinPlacements2;
	}
	return gAltinPlacementsOther;
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 v = gGameState.scene;

	if (v == (s32)&SceneId_Altin1)
	{
		return gAltinEvents1;
	}
	if (v == (s32)&SceneId_Altin2)
	{
		return gAltinEvents2;
	}
	return gAltinEventsOther;
}

static void EV08_TALK(void)
{
	struct EventWork *work;

	Engine_EventBegin();
	Engine_EventSetMessage(MsgAltinYourFirstTimeVisitAltin);

	Engine_EventOpenMessage(8, 0);

	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		Engine_EventShowMessage(8, 0);
	}
	else
	{
		work = gEventWork;
		work->message = (work->message + 1);
		Engine_EventAskYesNo(8, 0);
	}

	Engine_EventEnd();
}

static void EV09_TALK_1(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage(MsgAltinDidSeeWaterGushingOut);
	Engine_EventAskYesNo(9, 0);
	Engine_EventEnd();
}

static void EV14_TALK_1(void)
{

	struct FieldActor *rec18;
	u8				  *ready_flag;
	struct FieldActor *record;

	Engine_EventBegin();
	if (GameFlag_IsSet(0x909) != 0)
	{
		Engine_EventSetMessage(MsgAltinOnesWhoDefeatedWaterBeasts);
		Event_AskYesNo(14, 0);
	}
	else
	{
		Engine_ActorSetAnimation(14, 4);
		Engine_EventSetMessage(MsgAltinWeCantDrinkWaterMonsters);
		Event_ShowMessageAndWait(14, 0, 10);
		ready_flag = GameFlag_IsSet(0x8ff);
		if (ready_flag == 0)
		{
			rec18 = Object_GetById(18);

			Battle_GetWorkObject1e0()->motionFlags = ready_flag;
			Camera_SetSpeed(0x10000, 0x2000);
			Camera_MoveTo(rec18->x.fixed, rec18->y.fixed, rec18->z.fixed, 1);
			Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
			Actor_FaceDirection(14, 0x3000, 0);
			Engine_CameraWaitForMove();
			Engine_EventWait(120);
			record = Object_GetById(0);
			Camera_MoveTo(record->x.fixed, record->y.fixed, record->z.fixed, 1);
			Engine_CameraWaitForMove();
		}
		Engine_ActorSetAnimationAndWait(14, 4);
	}
	Engine_EventEnd();
}

static void EV17_TALK(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage(MsgAltinTrueFoundAncientRuinsIn);
	Engine_EventAskYesNo(17, 0);
	Engine_EventEnd();
}

static void EV09_TALK_2(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage(MsgAltinGirlFromXianWasAsking);
	Engine_EventAskYesNo(9, 0);
	Engine_EventEnd();
}

static void EV10_TALK(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage(MsgAltinTryingFindYourWayWest);
	Engine_EventAskYesNo(10, 0);
	Engine_EventEnd();
}

static void EV14_TALK_2(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage(MsgAltinDefeatedThoseMonstersDidnt);
	Engine_EventAskYesNo(14, 0);
	Engine_EventEnd();
}

static void EV21_TALK(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage(MsgAltinThereFewBeastsInMine);
	Engine_EventAskYesNo(21, 0);
	Engine_EventEnd();
}

static s32 SceneActor_IsSlotZeroAngleInRange(void)
{
	struct AngledSlot *slot = (struct AngledSlot *)Object_GetById(0);

	if ((u32)((slot->angle + 0x5FFF) << 16) <= 0x3FFE0000)
	{
		return 1;
	}
	return 0;
}

static void EV15_TALK(void)
{

	if (Engine_GameFlagIsSet(0x242) == 0)
	{
		Engine_EventBegin();
		Engine_EventSetMessage(MsgAltinDoWantWeapons);

		Engine_EventAskYesNo(15, 0);
		Engine_EventEnd();
		return;
	}

	if (SceneActor_IsSlotZeroAngleInRange() != 0)
	{
		Engine_ShopOpen(19, 15);
		return;
	}

	Engine_EventBegin();
	Engine_EventSetMessage(MsgAltinThankGoodnessWaterHasReceded);
	if (Engine_GameFlagIsSet(0x909) != 0)
	{
		Engine_EventSetMessage(MsgAltinYoullHaveFindPassageIn);
	}
	Engine_EventShowMessage(15, 0);
	Engine_EventEnd();
}

static void FieldScene_RunActorTwentyFlagBranch(void)
{

	if (Engine_GameFlagIsSet(0x241) == 0)
	{
		Engine_EventBegin();
		Engine_EventSetMessage(MsgAltinMyStoreSubmergedWantSell);
		Engine_EventShowMessage(20, 0);
		Engine_EventEnd();
		return;
	}

	if (SceneActor_IsSlotZeroAngleInRange() != 0)
	{
		Engine_ShopOpen(20, 17);
		return;
	}

	Engine_EventBegin();
	Engine_EventSetMessage(MsgAltinItsGreatCanSellArmor);
	if (Engine_GameFlagIsSet(0x909) != 0)
	{
		Engine_EventSetMessage(MsgAltinHowAboutArentImpressedBy);
	}
	Engine_EventShowMessage(17, 0);
	Engine_EventEnd();
}

static void FieldScene_RunActorTwentyOneFlagBranch(void)
{
	if (Engine_GameFlagIsSet(0x240) == 0)
	{
		Engine_EventBegin();
		Engine_EventSetMessage(MsgAltinWillDoIfMyMerchandise);
		Engine_EventShowMessage(21, 0);
		Engine_EventEnd();
		return;
	}

	if (SceneActor_IsSlotZeroAngleInRange() != 0)
	{
		Engine_ShopOpen(21, 16);
		return;
	}

	Engine_EventBegin();
	Engine_EventSetMessage(MsgAltinNoneMyGoodsWereDamaged);
	if (Engine_GameFlagIsSet(0x909) != 0)
	{
		Engine_EventSetMessage(MsgAltinGirlFromXianBoughtLot);
	}
	Engine_EventShowMessage(16, 0);
	Engine_EventEnd();
}

static void EV18_TALK(void)
{
	struct FieldActor *rec;

	rec = Object_GetById(0);

	if ((u32)((rec->facing + 0x5fff) << 16) <= 0x3ffe0000)
	{
		Engine_InnOpen(6, 18);
		return;
	}

	Engine_EventBegin();

	if (Engine_GameFlagIsSet(0x909) != 0)
	{
		Engine_EventSetMessage(MsgAltinThereSmallTempleWestAltin);
		Engine_EventShowMessage(18, 0);
	}
	else
	{
		Engine_EventSetMessage(MsgAltinWeGotLittleDampBut);
		Engine_EventAskYesNo(18, 0);
	}

	Engine_EventEnd();
}

static void FieldScene_RunEarlySequence(void)
{
	u32				   i;
	struct EventWork  *work;
	struct FieldActor *record;
	s32				   idx;
	u8				  *table;
	s32				   off;
	s32				   o4;
	s32				   a;
	s32				   b;
	s32				   c;

	work = gEventWork;
	Engine_EventBegin();
	for (i = 8; i <= 65; i++)
	{
		record = Object_GetById(i);
		if (record != 0)
		{
			record->motionFlags = 0;
		}
	}
	table = (u8 *)gAltin_TriggerTable;
	idx = ((*(u16 *)&work->touchedTrigger - 2) << 16) >> 16;
	off = idx << 3;
	o4 = off + 4;
	a = *(s16 *)(table + o4);
	b = *(s16 *)(table + o4 + 2);
	if (idx == 1)
	{
		Event_PlayCue(188);
		Map_CopyCellsTo(42, 33, a, b, 2, 2);
		c = a + 2;
		Map_CopyCellsTo(42, 35, c, b, 2, 2);
		Engine_EventWait(4);
		Map_CopyCellsTo(40, 33, a, b, 2, 2);
		Map_CopyCellsTo(40, 35, c, b, 2, 2);
		Engine_EventWait(4);
	}
	else
	{
		Event_PlayCue(158);
		if (idx == 3)
		{
			Map_CopyCellsTo(33, 42, 8, 17, 1, 2);
		}
		Map_AnimateCells(*(s32 *)(table + off), a, b);
	}
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Object_GetById(0)->motionFlags = 0;
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	if (idx == 6)
	{
		Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, 0);
	}
	else
	{
		if (idx != 1)
		{
			Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -4);
		}
		else
		{
			Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
			Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
		}
	}
	Engine_EventWait(10);
	Engine_EventRequestExit(work->touchedTrigger);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventEnd();
}

static void FieldScene_RunScene3a3SequenceB(void)
{
	struct EventWork *work;

	work = gEventWork;
	Engine_EventBegin();
	Object_GetById(0)->motionFlags = 0;
	Event_PlayCue(123);
	Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
	Engine_EventRequestExit(work->touchedTrigger);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventEnd();
}

static void SceneMotion_UpdateTimedActor(struct SceneMotion *work)
{
	if (work->delay != 0)
	{
		if (--work->delay == 1)
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	}
	if (work->velocity == 0)
	{
		Object_SetMode(work, 1);
		work->y += -0x18000;
		if (work->y < work->ground)
		{
			if (work->active != 0)
			{
				Event_PlayCue(229);
				work->active = 0;
				work->delay = 4;
				Work_SetValuesIfNonNegative(0, 0x10000, 0x10000);
			}
			work->y = work->ground;
		}
		work->state = 1;
	}
	else
	{
		work->state = 0;
	}
	if (work->timer == 0)
	{
		Event_PlayCue(152);
		work->active = 1;
		Object_SetMode(work, 2);
		work->velocity = 0x30000;
	}
	if (++work->timer == 60)
		work->timer = 0;
}

static void FieldScene_RunScene3a3SequenceC(void)
{
	struct SceneMotion *motion;

	motion = (struct SceneMotion *)Object_GetById(18);
	motion->timer = 0;
	motion->delay = 0;
	((struct FieldActor *)motion)->gravity = 0x6666;
	motion->callback = SceneMotion_UpdateTimedActor;
	Actor_SetSpeed(18, 0x13333, 0x9999);
	Actor_MoveToAndWait(18, 28, 0x1cc);
	Actor_MoveToAndWait(18, 24, 0x1c0);
	Event_PlayCue(229);
	Engine_ActorDestroy(18);
	Work_SetValuesIfNonNegative(0, 0x10000, 0x10000);
	Engine_EventWait(4);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(40);
	Engine_ActorSetAnimation(18, 1);
}

static void EV19_PSYNERGY(void)
{
	struct Actor *p = (struct Actor *)Object_GetById(19);

	if ((p->f08 >> 20) == 22)
	{
		Engine_GameFlagSet(0x906);
	}
	else
	{
		Engine_GameFlagClear(0x906);
	}
}

s32 Scene_Initialize(void)
{
	s16 scene;

	if (Engine_GameFlagIsSet(0x8fd) != 0)
	{
		Engine_GameFlagSet(0x240);
	}

	if (Engine_GameFlagIsSet(0x8fe) != 0 || Engine_GameFlagIsSet(0x907) != 0)
	{
		Engine_GameFlagSet(0x241);
	}

	if (Engine_GameFlagIsSet(0x8fe) != 0 && Engine_GameFlagIsSet(0x907) != 0)
	{
		Engine_GameFlagSet(0x242);
	}

	scene = gGameState.scene;
	if (scene == (s32)&SceneId_Altin1)
	{
		FieldScene_RunMiddleSequence();
	}
	else if (scene == (s32)&SceneId_Altin2)
	{
		FieldScene_RunScene3a3SequenceD();
	}

	return 0;
}

static void FieldScene_RunMiddleSequence(void)
{
	struct FieldActor *leader;

	leader = Object_GetById(ACTOR_PARTY_LEADER);
	if (GameFlag_IsSet(0x242) != 0)
	{
		Map_CopyCellsTo(64, 32, 0, 32, 32, 32);
		Map_CopyCellAttributes(64, 32, 32, 32, 0, 0);
		Engine_ActorDestroy(20);
		Engine_ActorDestroy(21);
	}
	else if (GameFlag_IsSet(0x241) != 0)
	{
		Map_CopyCellsTo(64, 0, 0, 32, 32, 32);
		Map_CopyCellAttributes(64, 0, 32, 32, 0, 0);
		Engine_ActorDestroy(17);
		Engine_ActorDestroy(20);
		Engine_ActorDestroy(21);
	}
	else if (GameFlag_IsSet(0x240) != 0)
	{
		Map_CopyCellsTo(0, 64, 0, 32, 32, 32);
		Map_CopyCellAttributes(0, 64, 32, 32, 0, 0);
		Engine_ActorDestroy(16);
		Engine_ActorDestroy(17);
		Engine_ActorDestroy(21);
	}
	else
	{
		Map_CopyCellAttributes(0, 32, 32, 32, 0, 0);
		Engine_ActorDestroy(15);
		Engine_ActorDestroy(16);
		Engine_ActorDestroy(17);
	}
	if (GameFlag_IsSet(0x8ff) != 0)
	{
		Engine_ActorDestroy(18);
	}
	else
	{
		BattleFx_SetQueuedSoundAndPlay(170);
		Actor_SetChildValue(18, 2);
		Engine_ActorSetAnimation(18, 3);
		FieldScheduler_AddCallback(SceneEffect_SpawnDriftingParticle, 0xc80);
	}
	if (gGameState.entrance == 3)
	{
		GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
	}
	Map_CopyCellAttributes(0, 33, 4, 3, 20, 41);
	if (GameFlag_IsSet(0x906) != 0)
	{
		Actor_SetPosition(19, 0x1680000, 0xa80000);
	}
	Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
	Actor_SetChildValue(22, 15);
	Engine_ActorSetChildValue(23, 15);
	Actor_SetChildValue(24, 15);
	Object_GetById(22)->collisionFlags |= 8;
	Object_GetById(23)->collisionFlags |= 8;
	Object_GetById(24)->collisionFlags |= 8;
	Object_GetById(22)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
	Object_GetById(23)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
	Object_GetById(24)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
	Engine_ActorSetSpritePriority(22, 1);
	Engine_ActorSetSpritePriority(23, 1);
	Engine_ActorSetSpritePriority(24, 1);
	Engine_TaskWait(1);
	Engine_EventBegin();
	Camera_MoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0);
	Engine_MapRedraw();
	Engine_EventEnd();
	Engine_TaskWait(1);
}

static void FieldScene_RunScene3a3SequenceD(void)
{
	struct FieldActor *actor;
	s32				   facing;

	if (GameFlag_IsSet(0x240) == 0)
	{
		Actor_SetPosition(8, 0x3280000, 0x2d70000);
		actor = Object_GetById(8);
		facing = 0x3000;
		actor->facing = facing;
		Actor_SetPosition(9, 0x31a0000, 0x3390000);
	}
	if (GameFlag_IsSet(0x241) == 0)
	{
		Actor_SetPosition(10, 0x2300000, 0x2c60000);
		actor = Object_GetById(10);
		facing = 0x1000;
		actor->facing = facing;
		Actor_SetPosition(11, 0x2400000, 0x2c60000);
	}
	if (GameFlag_IsSet(0x242) == 0)
	{
		Actor_SetPosition(15, 0x1270000, 0x2e80000);
		actor = Object_GetById(15);
		facing = 0xb000;
		actor->facing = facing;
	}
	else
	{
		u8 flags;

		actor = Object_GetById(15);
		flags = 4;
		flags |= actor->collisionFlags;
		actor->collisionFlags = flags;
	}
	actor = Object_GetById(17);
	if (actor != 0)
	{
		actor->collisionFlags = 4 | actor->collisionFlags;
	}
	actor = Object_GetById(16);
	if (actor != 0)
	{
		actor->collisionFlags = 4 | actor->collisionFlags;
	}
}

static void SceneActor_ResetStateAndSpan(struct SpanActor *actor)
{

	u8 *state = &actor->state;
	s32 clear = 0;
	u8 *attached;

	*state = clear;
	attached = actor->attached;
	clear -= 13;
	attached[9] = (clear & attached[9]) | 4;
	ObjectGroup_SetChildValue(actor, 3);
	Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 0);
	actor->span = 0x4CCC;
	actor->reach = 0x4CCC;
}

static void SceneEffect_UpdateDriftingParticle(struct SceneMotion *work)
{
	work->x += (work->timer << 12) + ((s16)((s32)(((u32)Engine_RandomNext() * 2) >> 16) - 1) << 15);
	if (work->timer <= 3)
	{
		work->z += -(((u32)Engine_RandomNext() * 0x8000) >> 16) - 0x10000;
		work->scaleX += 0x2666;
		work->scaleY += -0xa3d;
	}
	else
	{
		work->z += 0x20000;
		work->scaleX += 0x7ae;
		work->scaleY += 0x7ae;
	}
	if (((u32)Engine_RandomNext() * work->timer) >> 16 == 0)
		ObjectGroup_SetChildValue(work, 7);
	if (work->timer != 0)
		work->timer--;
	else
		work->timer = (((u32)Engine_RandomNext() * 5) >> 16) * 2 + 2;
	if (--work->active == 0)
	{
		work->callback = 0;
		Engine_ObjectDispatchRelease(work);
	}
}

static void SceneEffect_SpawnDriftingParticle(void)
{
	struct SceneMotion *work;
	if ((gFrameCount & 3) == 0)
	{
		work = ((struct SceneMotion *)Object_Create(222, 0x400000, 0, 0x1900000));
		if (work != 0)
		{
			work->timer = 20;
			work->delay = 0;
			work->active = 20;
			SceneActor_ResetStateAndSpan(work);
			work->callback = SceneEffect_UpdateDriftingParticle;
			Object_SetMode(work, 1);
		}
	}
}

static void EV20_TOUCH(void)
{
	Engine_EventBegin();
	Camera_SetSpeed(0x6666, 0xccc);
	Camera_MoveTo(0x3f0000, -1, 0x1c20000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(30);
	Engine_ActorSetAnimation(18, 1);
	BattleFx_SetQueuedSoundAndPlay(-1);
	Engine_TaskRemoveCallback(SceneEffect_SpawnDriftingParticle);
	Engine_EventWait(20);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
	Actor_FaceDirection(18, 0, 20);
	Actor_FaceDirection(18, 0xd000, 40);
	Event_PlayCue(147);
	Engine_ActorRunRepeatedMotion(18, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(18, 0xb000, 40);
	FieldScene_RunScene3a3SequenceC();
	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
	Engine_CameraWaitForMove();
	Engine_ActorSetAnimationAndWait(14, 4);
	GameFlag_Set(0x8ff);
	Engine_EventEnd();
}

s32 gAltinPlacements2_Data[] = {
	SC_ADD(SW_COLLISION, 128),
	SC_SET(SW_SPEED, 0x26666),
	SC_SET(SW_ACCEL, 0x13333),
	SC_LABEL(1),
	SC_ARG(2),
	SC_MOVE(PIXELS(342), 0, PIXELS(746)),
	SC_SYNC,
	SC_SET(SW_FACING, 0xb000),
	SC_ARG(3),
	SC_WAIT(30),
	SC_ARG(2),
	SC_MOVE(PIXELS(356), 0, PIXELS(746)),
	SC_SYNC,
	SC_SET(SW_FACING, 0xd000),
	SC_ARG(3),
	SC_WAIT(50),
	SC_ARG(2),
	SC_MOVE(PIXELS(342), 0, PIXELS(746)),
	SC_SYNC,
	SC_SET(SW_FACING, 0xb000),
	SC_ARG(3),
	SC_WAIT(20),
	SC_ARG(2),
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
	SC_MOVE(PIXELS(384), 0, PIXELS(746)),
#else
	SC_MOVE(PIXELS(380), 0, PIXELS(746)),
#endif
	SC_SYNC,
	SC_SET(SW_FACING, 0xd000),
	SC_ARG(3),
	SC_WAIT(30),
	SC_SET(SW_FACING, 0x1000),
	SC_WAIT(90),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct SceneEntrance gAltinEntrancesOther[] = {
	{0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gAltinEntrances1[] = {
	{0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 488, 0, 184, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{2, CONDITION_ALWAYS, 168, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{3, CONDITION_ALWAYS, 336, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{4, CONDITION_ALWAYS, 424, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{5, CONDITION_ALWAYS, 136, 0, 312, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{6, CONDITION_ALWAYS, 280, 0, 408, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{7, CONDITION_ALWAYS, 456, 0, 224, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{8, CONDITION_ALWAYS, 184, 0, 408, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{9, CONDITION_ALWAYS, 168, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{10, CONDITION_ALWAYS, 24, 0, 456, FACING_SOUTHEAST, 0, -1, -1, -1, -1, 0},
	{11, CONDITION_ALWAYS, 264, 0, 224, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{12, CONDITION_ALWAYS, 328, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{13, CONDITION_ALWAYS, 343, 0, 177, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gAltinEntrances2[] = {
	{0, CONDITION_ALWAYS, 200, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 159, 0, 258, FACING_NORTH, 0, 25, 16, 255, 300, 0},
	{2, CONDITION_ALWAYS, 336, 0, 213, FACING_NORTH, 0, 268, 40, 500, 250, 0},
	{3, CONDITION_ALWAYS, 688, 0, 204, FACING_NORTH, 0, 542, 40, 768, 250, 0},
	{4, CONDITION_ALWAYS, 240, 0, 500, FACING_NORTH, 0, 110, 336, 350, 540, 0},
	{5, CONDITION_ALWAYS, 496, 0, 504, FACING_NORTH, 0, 360, 336, 600, 540, 0},
	{6, CONDITION_ALWAYS, 752, 0, 516, FACING_NORTH, 0, 635, 280, 845, 556, 0},
	{7, CONDITION_ALWAYS, 80, 0, 864, FACING_NORTH, 0, 16, 638, 432, 912, 0},
	{8, CONDITION_ALWAYS, 68, 0, 728, FACING_SOUTH, 0, 16, 638, 432, 912, 0},
	{9, CONDITION_ALWAYS, 690, 0, 732, FACING_SOUTH, 0, 480, 626, 896, 882, 0},
	{SCENE_TABLE_END},
};

u32 gPlacement_Messages[] = {
	SCENE_EXITS(75),
	SCENE_EXIT(1, 2, 16),
	SCENE_EXIT(2, 76, 7),
	SCENE_EXIT(3, 9, 29),
	SCENE_EXIT(4, 76, 1),
	SCENE_EXIT(5, 76, 2),
	SCENE_EXIT(6, 76, 3),
	SCENE_EXIT(7, 76, 5),
	SCENE_EXIT(8, 76, 6),
	SCENE_EXIT(9, 76, 4),
	SCENE_EXIT(10, 77, 1),
	SCENE_EXIT(11, 80, 1),
	SCENE_EXIT(12, 84, 4),
	SCENE_EXIT(13, 83, 3),
	SCENE_EXIT(0, 76, 0),
	SCENE_EXIT(1, 75, 4),
	SCENE_EXIT(2, 75, 5),
	SCENE_EXIT(3, 75, 6),
	SCENE_EXIT(4, 75, 9),
	SCENE_EXIT(5, 75, 7),
	SCENE_EXIT(6, 75, 8),
	SCENE_EXIT(7, 75, 2),
	SCENE_EXIT(8, 76, 9),
	SCENE_EXIT(9, 76, 8),
	SCENE_EXITS_END,
};

struct ScenePlacement gAltinPlacementsOther[] = {
	{SCENE_TABLE_END},
};

struct ScenePlacement gAltinPlacements1[] = {
	{0x72, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(440), 0, PIXELS(200), FACING_SOUTH + FACING_STEP,
		TALK_FACE_PARTY_AND_BACK, 0},
	{0x74, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(408), 0, PIXELS(216), FACING_SOUTH + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{0x6a, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(310), 0, PIXELS(217), FACING_SOUTH + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{0x65, CONDITION_ALWAYS, ACTOR_WANDER, PIXELS(246), 0, PIXELS(186), FACING_SOUTH + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{0x67, CONDITION_ALWAYS, 3, PIXELS(58), 0, PIXELS(206), FACING_EAST + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{0x73, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(104), 0, PIXELS(286), FACING_EAST + FACING_STEP,
		TALK_FACE_PARTY_AND_BACK, 0},
	{0x6f, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(41), 0, PIXELS(441),
		FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0},
	{0x6c, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(176), 0, PIXELS(296),
		FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0},
	{0x72, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(136), 0, PIXELS(461), FACING_EAST + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{0x67, CONDITION_ALWAYS, ACTOR_WANDER, PIXELS(350), 0, PIXELS(286), FACING_NORTH,
		TALK_FACE_PARTY, 0},
	{0xbe, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(60), 0, PIXELS(474), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{0xcb, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(344), 0, PIXELS(168), FACING_SOUTH,
		TALK_FACE_PARTY, 0},
	{0x7d, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(88), 0, PIXELS(397), FACING_EAST + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{0x76, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(422), 0, PIXELS(208), FACING_EAST + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{0xdf, CONDITION_ALWAYS, 7, PIXELS(452), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0},
	{0xdf, CONDITION_ALWAYS, 7, PIXELS(332), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING,
		PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{0xdf, CONDITION_ALWAYS, 7, PIXELS(332), 0, PIXELS(396), FACING_SOUTH, TALK_KEEP_FACING,
		PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SCENE_TABLE_END},
};

struct ScenePlacement gAltinPlacements2[] = {
	{0x6c, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(104), 0, PIXELS(120), FACING_EAST + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{0x6a, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(105), 0, PIXELS(233), FACING_SOUTH + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{0x67, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(456), 0, PIXELS(182), FACING_WEST + FACING_STEP, 3,
		0},
	{0x6f, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(376), 0, PIXELS(120),
		FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0},
	{0x6c, 0x0, ACTOR_STAND, PIXELS(690), 0, PIXELS(128), FACING_SOUTHWEST + FACING_STEP,
		TALK_FACE_PARTY_AND_BACK, 0},
	{0x6a, 0x0, ACTOR_STAND, PIXELS(669), 0, PIXELS(128), FACING_EAST + FACING_STEP,
		TALK_FACE_PARTY_AND_BACK, 0},
	{0x66, 0x0, ACTOR_STAND, PIXELS(604), 0, PIXELS(145), FACING_SOUTHEAST + FACING_STEP, 3, 0},
	{0x7c, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(263), 0, PIXELS(436), FACING_SOUTH + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{0x76, 0x0, ACTOR_STAND, PIXELS(520), 0, PIXELS(454), FACING_SOUTH + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{0x7d, 0x0, ACTOR_STAND, PIXELS(743), 0, PIXELS(454), FACING_SOUTHEAST + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{0x77, 0x0, ACTOR_STAND, PIXELS(103), 0, PIXELS(744), FACING_SOUTHEAST + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{0x80, 0x0, 3, PIXELS(228), 0, PIXELS(756), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0},
	{0x7b, 0x0, (s32)gAltinPlacements2_Data, PIXELS(350), 0, PIXELS(746),
		FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0},
	{0x6c, 0x0, ACTOR_STAND, PIXELS(646), 0, PIXELS(734), FACING_SOUTHEAST + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{SCENE_TABLE_END},
};

struct SceneEvent gAltinEventsOther[] = {
	{SCENE_EVENTS_END},
};

struct SceneEvent gAltinEvents1[] = {
	{0xc402, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3a3SequenceB)},
	{0xc402, 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3a3SequenceB)},
	{0xc402, 12, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3a3SequenceB)},
	{EVENT_EXIT, 13, CONDITION_ALWAYS, 13},
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_RunEarlySequence)},
	{TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_RunEarlySequence)},
	{TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_RunEarlySequence)},
	{TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_RunEarlySequence)},
	{TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_RunEarlySequence)},
	{TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_RunEarlySequence)},
	{TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_RunEarlySequence)},
	{TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_RunEarlySequence)},
	{EVENT_TALK, 8, 0x909, EVENT_SCRIPT(EV08_TALK)},
	{EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAltinThankYouSoMuchForDefeating)},
	{EVENT_TALK, 9, 0x909, MSGNO(MsgAltinHowCanThoseCreaturesSpitUp)},
	{EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK_1)},
	{EVENT_TALK, 10, 0x909, MSGNO(MsgAltinHowHorribleMyHouseIsTotally)},
	{EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAltinOnceTheMineOpensAgainWell)},
	{EVENT_TALK, 11, 0x909, MSGNO(MsgAltinEvenTheMineIsSubmergedNo)},
	{EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAltinTheyAlreadyStartedMiningTheUpper)},
	{EVENT_TALK, 12, 0x909, MSGNO(MsgAltinIfWeCouldJustDestroyThe)},
	{EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAltinTheWaterDrainedOutOfThe)},
	{EVENT_TALK, 13, 0x909, MSGNO(MsgAltinAltinAlwaysUsedToRunShort)},
	{EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAltinThankGoodnessTheVillageIsFree)},
	{EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK_1)},
	{EVENT_TALK, 15, 0x909, MSGNO(MsgAltinTheWatersAroundHereHaveSubsided)},
	{EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgAltinTheVillageWentRightBackTo)},
	{EVENT_TALK, 16, 0x909, MSGNO(MsgAltinTheWaterBeastsLookedExactlyLike)},
	{EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgAltinSoThoseHydrosCreaturesReallyWere)},
	{EVENT_TALK, 17, 0x909, MSGNO(MsgAltinTheWatersHaveFlushedOutTons)},
	{EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_TALK)},
	{EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTwentyFlagBranch)},
	{EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTwentyOneFlagBranch)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x909, MSGNO(MsgAltinYouCameToAltinAtA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
		MSGNO(MsgAltinWeCanGoBackToWork)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x909,
		MSGNO(MsgAltinItsAlmostLikeTheyreTurningThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
		MSGNO(MsgAltinIWasAfraidMoreWaterWould)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x909, MSGNO(MsgAltinImGladISavedMyVery)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
		MSGNO(MsgAltinIHopeAltinMineWillThrive)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x909,
		MSGNO(MsgAltinEvenTheToughestOfMinersCant)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
		MSGNO(MsgAltinTheWaterSeepedIntoTheMine)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x909,
		MSGNO(MsgAltinIfTheWaterBeastsContinueTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
		MSGNO(MsgAltinItsSoHumidInsideThatCave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x909, MSGNO(MsgAltinWaterIsNiceButNotWhen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
		MSGNO(MsgAltinThatStrangeChamberInTheMines)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x909,
		MSGNO(MsgAltinUghTheMonstersSpewedOutAll)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
		MSGNO(MsgAltinYouFoughtPrettyWellDespiteAll)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x909, MSGNO(MsgAltinAsTheMonstersGoAwayThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
		MSGNO(MsgAltinThatMineIsFullOfHoles)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x909, MSGNO(MsgAltinComeToThinkOfItIt)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
		MSGNO(MsgAltinTheOldGuardianStatuesAlwaysCreeped)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x909,
		MSGNO(MsgAltinItllBeBetterThanDiggingUp)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
		MSGNO(MsgAltinIUsedToFindAllSorts)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
		MSGNO(MsgAltinAllMyArmorInventoryIsRusting)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
		MSGNO(MsgAltinIfMyGoodsAreWetIll)},
	{PSYNERGY_ON_ACTOR(0x8c), 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_PSYNERGY)},
	{EVENT_TOUCH, 20, 0x8ff, EVENT_SCRIPT(EV20_TOUCH)},
	{SCENE_EVENTS_END},
};

struct SceneEvent gAltinEvents2[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
	{EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
	{EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
	{EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
	{EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
	{EVENT_EXIT, 7, CONDITION_ALWAYS, 7},
	{EVENT_EXIT, 8, CONDITION_ALWAYS, 8},
	{EVENT_EXIT, 9, CONDITION_ALWAYS, 9},
	{EVENT_TALK, 8, 0x240, MSGNO(MsgAltinNoMatterWhatHappensIdNever)},
	{EVENT_TALK, 8, 0x909, MSGNO(MsgAltinOhNoTheWholeHouseIs)},
	{EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAltinIHearThatSilkRoadIs)},
	{EVENT_TALK, 9, 0x240, MSGNO(MsgAltinOfCourseOurHouseWillBe)},
	{EVENT_TALK, 9, 0x909, MSGNO(MsgAltinIfTheMonsterThatDidThis)},
	{EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK_2)},
	{EVENT_TALK, 10, 0x241, MSGNO(MsgAltinNotManyPeopleComeUpSilk)},
	{EVENT_TALK, 10, 0x909, EVENT_SCRIPT(EV10_TALK)},
	{EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAltinAncientRuinsAndTreasureDefinitelyGo)},
	{EVENT_TALK, 11, 0x241, MSGNO(MsgAltinItsBeenALongTimeSince)},
	{EVENT_TALK, 11, 0x909, MSGNO(MsgAltinDontBeTooHarshOnThose)},
	{EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAltinIfOnlyTheyHadLeftA)},
	{EVENT_TALK, 12, 0x909, MSGNO(MsgAltinIDidntWantToLeaveMy)},
	{EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAltinReallyAncientRuinsAtTheBottom)},
	{EVENT_TALK, 13, 0x909, MSGNO(MsgAltinIHeardSomeoneKilledTheWater)},
	{EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAltinThereProbablyIsntMuchTreasureIn)},
	{EVENT_TALK, 14, 0x909, EVENT_SCRIPT(EV14_TALK_2)},
	{EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAltinSomethingsWrongWithMomAndPop)},
	{EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK)},
	{EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTwentyFlagBranch)},
	{EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTwentyOneFlagBranch)},
	{EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK)},
	{EVENT_TALK, 19, 0x909, MSGNO(MsgAltinIdGiveAnythingForSomeoneTo)},
	{EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAltinThereIsAnEndlessDesertWest)},
	{EVENT_TALK, 20, 0x909, MSGNO(MsgAltinICameFromXianICook)},
	{EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAltinWestOfTheMountainsIsVery)},
	{EVENT_TALK, 21, 0x240, MSGNO(MsgAltinNoneOfUsCanGoHome)},
	{EVENT_TALK, 21, 0x241, MSGNO(MsgAltinTheGuestsInRoomOneMoved)},
	{EVENT_TALK, 21, 0x909, MSGNO(MsgAltinTheGuestsInRoomTwoAlso)},
	{EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x240,
		MSGNO(MsgAltinLeavingAltinForTolbiWellThats)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x909, MSGNO(MsgAltinItsGoingToTakeALot)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
		MSGNO(MsgAltinTheMerchantsMustBeHavingA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x240, MSGNO(MsgAltinNowIFinallyHaveAnExcuse)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x909, MSGNO(MsgAltinOneBlowsNotEnoughJabJab)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
		MSGNO(MsgAltinIWonderWhatBusinessThatGirl)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x241, MSGNO(MsgAltinOfCourseYouCantGoWest)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x909,
		MSGNO(MsgAltinIThoughtTheVillageWasDone)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
		MSGNO(MsgAltinTheySayYouCanStillFind)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x241,
		MSGNO(MsgAltinIHateSittingAroundLikeThis)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x909, MSGNO(MsgAltinImJustGladToGetMy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
		MSGNO(MsgAltinHowCanITrulyBeHappy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x909, MSGNO(MsgAltinTheBottomOfTheMineThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
		MSGNO(MsgAltinCripesAncientRuinsDownInThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x909,
		MSGNO(MsgAltinMyHusbandsSenseForFindingTreasure)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
		MSGNO(MsgAltinIHadNoIdeaThereWere)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x909,
		MSGNO(MsgAltinMomAndPopKnowSomethingAbout)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
		MSGNO(MsgAltinIWonderIfMomAndPop)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x242, MSGNO(MsgAltinCripesIsThisHowMyWeapon)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x909, MSGNO(MsgAltinOnlyAMerchantKnowsTheJoy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
		MSGNO(MsgAltinIveHeardOfAPassageIn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x909, MSGNO(MsgAltinWeDriedOffAllThatArmor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
		MSGNO(MsgAltinWellIfYouAreImpressedThen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x909, MSGNO(MsgAltinAFewThingsGotDampBut)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
		MSGNO(MsgAltinTheGirlFromXianBoughtAll)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x909,
		MSGNO(MsgAltinWellComeThroughThisAllRight)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
		MSGNO(MsgAltinIThinkThatTempleWasCalled)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0x909, MSGNO(MsgAltinIKnowTheyDontLookSo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
		MSGNO(MsgAltinYouWontBeAbleToGo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0x909, MSGNO(MsgAltinIBecameACookAfterI)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
		MSGNO(MsgAltinNoOneCanGoWestOf)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, 0x240, MSGNO(MsgAltinMostOfTheVillageIsStill)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, 0x909, MSGNO(MsgAltinItLooksLikeTheWaterHas)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
		MSGNO(MsgAltinItWillTakeSomeTimeBefore)},
	{SEARCH(SEARCH_BARREL), 100, 0xf73, FIND_ITEM(ITEM_NUT)},
	{SEARCH(SEARCH_BARREL), 101, 0xf74, FIND_COINS(9)},
	{SEARCH(SEARCH_BED), 102, 0xf75, FIND_ITEM(ITEM_SLEEP_BOMB)},
	{SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgAltinThePotsAndPansAreRusted))},
	{SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgAltinHeIsMakingRoastBeefEven))},
	{SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgAltinHeIsMakingShrimpInA))},
	{SEARCH(SEARCH_BOOKCASE), 203, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgAltinThereIsABookTitledAltin))},
	{SCENE_EVENTS_END},
};

u16 gAltin_TriggerTable1[] = {
	MAP_CELL_STEP(42, 33, 2, 2, 0),
	MAP_CELL_STEP(42, 35, 2, 2, 5),
	MAP_CELL_STEP(40, 33, 2, 2, 0),
	MAP_CELL_STEP(40, 35, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

u16 gAltin_TriggerTable0[] = {
	MAP_CELL_STEP(35, 48, 2, 2, 5),
	MAP_CELL_STEP(33, 48, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

u16 gAltin_TriggerTable5[] = {
	MAP_CELL_STEP(35, 35, 2, 2, 5),
	MAP_CELL_STEP(33, 35, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

u16 gAltin_TriggerTable6[] = {
	MAP_CELL_STEP(35, 46, 2, 2, 5),
	MAP_CELL_STEP(33, 46, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

u16 gAltin_TriggerTable2[] = {
	MAP_CELL_STEP(35, 50, 2, 2, 5),
	MAP_CELL_STEP(33, 50, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

u16 gAltin_TriggerTable3[] = {
	MAP_CELL_STEP(35, 33, 2, 2, 5),
	MAP_CELL_STEP(33, 33, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

u16 gAltin_TriggerTable4[] = {
	MAP_CELL_STEP(35, 52, 2, 2, 5),
	MAP_CELL_STEP(33, 52, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

struct MapCellAnim gAltin_TriggerTable[] = {
	{gAltin_TriggerTable0, 41, 5},
	{gAltin_TriggerTable1, 51, 6},
	{gAltin_TriggerTable2, 57, 17},
	{gAltin_TriggerTable3, 39, 17},
	{gAltin_TriggerTable4, 48, 25},
	{gAltin_TriggerTable5, 59, 9},
	{gAltin_TriggerTable6, 42, 23},
	{gAltin_TriggerTable3, 41, 27},
};
