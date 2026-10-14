#include "game_state.h"
#include "scene_ids.h"
#include "unit/scene/arutin_mura.h"
#include "unit/object/object.h"
#include "unit/object/evbasic.h"
#include "unit/object/evactor.h"
#include "unit/object/evcamera.h"
#include "unit/object/djinnev.h"
#include "unit/object/psydisp.h"
#include "unit/object/psyfx.h"
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
#include "engcall.h"
#include "field_scene.h"
#include "arutin_mura.h"
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

	if (v == (s32)&SceneId_ArutinMura1)
	{
		return gArutinMuraEntrances1;
	}
	if (v == (s32)&SceneId_ArutinMura2)
	{
		return gArutinMuraEntrances2;
	}
	return gArutinMuraEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return (u8 *)Placement_Messages;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s32 id = gGameState.scene;
	if (id == (s32)&SceneId_ArutinMura1)
	{
		if (Engine_GameFlagIsSet(0x909))
		{
			gArutinMuraPlacements1[5].talk_facing = TALK_FACE_PARTY;
			gArutinMuraPlacements1[6].talk_facing = TALK_FACE_PARTY;
		}
		return gArutinMuraPlacements1;
	}
	if (id == (s32)&SceneId_ArutinMura2)
	{
		if (Engine_GameFlagIsSet(0x8fd))
			gArutinMuraPlacements2[1].talk_facing = TALK_FACE_PARTY_AND_BACK;
		if (Engine_GameFlagIsSet(0x8fe) || Engine_GameFlagIsSet(0x907))
			gArutinMuraPlacements2[3].talk_facing = TALK_FACE_PARTY_AND_BACK;
		FieldScene_PrepareActors(gArutinMuraPlacements2);
		return gArutinMuraPlacements2;
	}
	return gArutinMuraPlacementsOther;
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 v = gGameState.scene;

	if (v == (s32)&SceneId_ArutinMura1)
	{
		return gArutinMuraEvents1;
	}
	if (v == (s32)&SceneId_ArutinMura2)
	{
		return gArutinMuraEvents2;
	}
	return gArutinMuraEventsOther;
}

static void EV08_TALK(void)
{
	struct EventWork *work;

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgArutinYourFirstTimeVisitAltin);

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
	Engine_EventSetMessage((s32)MsgArutinDidSeeWaterGushingOut);
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
		Engine_EventSetMessage((s32)MsgArutinOnesWhoDefeatedWaterBeasts);
		Event_AskYesNo(14, 0);
	}
	else
	{
		Engine_ActorSetAnimation(14, 4);
		Engine_EventSetMessage((s32)MsgArutinWeCantDrinkWaterMonsters);
		Event_ShowMessageAndWait(14, 0, 10);
		ready_flag = GameFlag_IsSet(0x8ff);
		if (ready_flag == 0)
		{
			rec18 = Object_GetById(18);

			Battle_GetWorkObject1e0()->motion_flags = ready_flag;
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
	Engine_EventSetMessage((s32)MsgArutinTrueFoundAncientRuinsIn);
	Engine_EventAskYesNo(17, 0);
	Engine_EventEnd();
}

static void EV09_TALK_2(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgArutinGirlFromXianWasAsking);
	Engine_EventAskYesNo(9, 0);
	Engine_EventEnd();
}

static void EV10_TALK(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgArutinTryingFindYourWayWest);
	Engine_EventAskYesNo(10, 0);
	Engine_EventEnd();
}

static void EV14_TALK_2(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgArutinDefeatedThoseMonstersDidnt);
	Engine_EventAskYesNo(14, 0);
	Engine_EventEnd();
}

static void EV21_TALK(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgArutinThereFewBeastsInMine);
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
		Engine_EventSetMessage((s32)MsgArutinDoWantWeapons);

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
	Engine_EventSetMessage((s32)MsgArutinThankGoodnessWaterHasReceded);
	if (Engine_GameFlagIsSet(0x909) != 0)
	{
		Engine_EventSetMessage((s32)MsgArutinYoullHaveFindPassageIn);
	}
	Engine_EventShowMessage(15, 0);
	Engine_EventEnd();
}

static void FieldScene_RunActorTwentyFlagBranch(void)
{

	if (Engine_GameFlagIsSet(0x241) == 0)
	{
		Engine_EventBegin();
		Engine_EventSetMessage((s32)MsgArutinMyStoreSubmergedWantSell);
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
	Engine_EventSetMessage((s32)MsgArutinItsGreatCanSellArmor);
	if (Engine_GameFlagIsSet(0x909) != 0)
	{
		Engine_EventSetMessage((s32)MsgArutinHowAboutArentImpressedBy);
	}
	Engine_EventShowMessage(17, 0);
	Engine_EventEnd();
}

static void FieldScene_RunActorTwentyOneFlagBranch(void)
{
	if (Engine_GameFlagIsSet(0x240) == 0)
	{
		Engine_EventBegin();
		Engine_EventSetMessage((s32)MsgArutinWillDoIfMyMerchandise);
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
	Engine_EventSetMessage((s32)MsgArutinNoneMyGoodsWereDamaged);
	if (Engine_GameFlagIsSet(0x909) != 0)
	{
		Engine_EventSetMessage((s32)MsgArutinGirlFromXianBoughtLot);
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
		Engine_EventSetMessage((s32)MsgArutinThereSmallTempleWestAltin);
		Engine_EventShowMessage(18, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgArutinWeGotLittleDampBut);
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
	u8				  *tbl;
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
			record->motion_flags = 0;
		}
	}
	tbl = (u8 *)ArutinMura_TriggerTable;
	idx = ((*(u16 *)&work->touched_trigger - 2) << 16) >> 16;
	off = idx << 3;
	o4 = off + 4;
	a = *(s16 *)(tbl + o4);
	b = *(s16 *)(tbl + o4 + 2);
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
		Map_AnimateCells(*(s32 *)(tbl + off), a, b);
	}
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Object_GetById(0)->motion_flags = 0;
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
	Engine_EventRequestExit(work->touched_trigger);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventEnd();
}

static void FieldScene_RunScene3a3SequenceB(void)
{
	struct EventWork *work;

	work = gEventWork;
	Engine_EventBegin();
	Object_GetById(0)->motion_flags = 0;
	Event_PlayCue(123);
	Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
	Engine_EventRequestExit(work->touched_trigger);
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
	if (scene == (s32)&SceneId_ArutinMura1)
	{
		FieldScene_RunMiddleSequence();
	}
	else if (scene == (s32)&SceneId_ArutinMura2)
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
	Object_GetById(22)->collision_flags |= 8;
	Object_GetById(23)->collision_flags |= 8;
	Object_GetById(24)->collision_flags |= 8;
	Object_GetById(22)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
	Object_GetById(23)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
	Object_GetById(24)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
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
		flags |= actor->collision_flags;
		actor->collision_flags = flags;
	}
	actor = Object_GetById(17);
	if (actor != 0)
	{
		actor->collision_flags = 4 | actor->collision_flags;
	}
	actor = Object_GetById(16);
	if (actor != 0)
	{
		actor->collision_flags = 4 | actor->collision_flags;
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
		work->scale_x += 0x2666;
		work->scale_y += -0xa3d;
	}
	else
	{
		work->z += 0x20000;
		work->scale_x += 0x7ae;
		work->scale_y += 0x7ae;
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

s32 gArutinMuraPlacements2_Data[] = {
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

struct SceneEntrance gArutinMuraEntrancesOther[] = {
	{0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gArutinMuraEntrances1[] = {
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

struct SceneEntrance gArutinMuraEntrances2[] = {
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

u32 Placement_Messages[] = {
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

struct ScenePlacement gArutinMuraPlacementsOther[] = {
	{SCENE_TABLE_END},
};

struct ScenePlacement gArutinMuraPlacements1[] = {
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

struct ScenePlacement gArutinMuraPlacements2[] = {
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
	{0x7b, 0x0, (s32)gArutinMuraPlacements2_Data, PIXELS(350), 0, PIXELS(746),
		FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0},
	{0x6c, 0x0, ACTOR_STAND, PIXELS(646), 0, PIXELS(734), FACING_SOUTHEAST + FACING_STEP,
		TALK_FACE_PARTY, 0},
	{SCENE_TABLE_END},
};

struct SceneEvent gArutinMuraEventsOther[] = {
	{SCENE_EVENTS_END},
};

struct SceneEvent gArutinMuraEvents1[] = {
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
	{EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgArutinMuraThankYouSoMuchForDefeating)},
	{EVENT_TALK, 9, 0x909, MSGNO(MsgArutinMuraHowCanThoseCreaturesSpitUp)},
	{EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK_1)},
	{EVENT_TALK, 10, 0x909, MSGNO(MsgArutinMuraHowHorribleMyHouseIsTotally)},
	{EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgArutinMuraOnceTheMineOpensAgainWell)},
	{EVENT_TALK, 11, 0x909, MSGNO(MsgArutinMuraEvenTheMineIsSubmergedNo)},
	{EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgArutinMuraTheyAlreadyStartedMiningTheUpper)},
	{EVENT_TALK, 12, 0x909, MSGNO(MsgArutinMuraIfWeCouldJustDestroyThe)},
	{EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgArutinMuraTheWaterDrainedOutOfThe)},
	{EVENT_TALK, 13, 0x909, MSGNO(MsgArutinMuraAltinAlwaysUsedToRunShort)},
	{EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgArutinMuraThankGoodnessTheVillageIsFree)},
	{EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK_1)},
	{EVENT_TALK, 15, 0x909, MSGNO(MsgArutinMuraTheWatersAroundHereHaveSubsided)},
	{EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgArutinMuraTheVillageWentRightBackTo)},
	{EVENT_TALK, 16, 0x909, MSGNO(MsgArutinMuraTheWaterBeastsLookedExactlyLike)},
	{EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgArutinMuraSoThoseHydrosCreaturesReallyWere)},
	{EVENT_TALK, 17, 0x909, MSGNO(MsgArutinMuraTheWatersHaveFlushedOutTons)},
	{EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_TALK)},
	{EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTwentyFlagBranch)},
	{EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTwentyOneFlagBranch)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x909, MSGNO(MsgArutinMuraYouCameToAltinAtA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraWeCanGoBackToWork)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x909,
		MSGNO(MsgArutinMuraItsAlmostLikeTheyreTurningThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraIWasAfraidMoreWaterWould)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x909, MSGNO(MsgArutinMuraImGladISavedMyVery)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraIHopeAltinMineWillThrive)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x909,
		MSGNO(MsgArutinMuraEvenTheToughestOfMinersCant)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraTheWaterSeepedIntoTheMine)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x909,
		MSGNO(MsgArutinMuraIfTheWaterBeastsContinueTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraItsSoHumidInsideThatCave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x909, MSGNO(MsgArutinMuraWaterIsNiceButNotWhen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraThatStrangeChamberInTheMines)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x909,
		MSGNO(MsgArutinMuraUghTheMonstersSpewedOutAll)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraYouFoughtPrettyWellDespiteAll)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x909, MSGNO(MsgArutinMuraAsTheMonstersGoAwayThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraThatMineIsFullOfHoles)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x909, MSGNO(MsgArutinMuraComeToThinkOfItIt)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraTheOldGuardianStatuesAlwaysCreeped)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x909,
		MSGNO(MsgArutinMuraItllBeBetterThanDiggingUp)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraIUsedToFindAllSorts)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraAllMyArmorInventoryIsRusting)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraIfMyGoodsAreWetIll)},
	{PSYNERGY_ON_ACTOR(0x8c), 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_PSYNERGY)},
	{EVENT_TOUCH, 20, 0x8ff, EVENT_SCRIPT(EV20_TOUCH)},
	{SCENE_EVENTS_END},
};

struct SceneEvent gArutinMuraEvents2[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
	{EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
	{EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
	{EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
	{EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
	{EVENT_EXIT, 7, CONDITION_ALWAYS, 7},
	{EVENT_EXIT, 8, CONDITION_ALWAYS, 8},
	{EVENT_EXIT, 9, CONDITION_ALWAYS, 9},
	{EVENT_TALK, 8, 0x240, MSGNO(MsgArutinMuraNoMatterWhatHappensIdNever)},
	{EVENT_TALK, 8, 0x909, MSGNO(MsgArutinMuraOhNoTheWholeHouseIs)},
	{EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgArutinMuraIHearThatSilkRoadIs)},
	{EVENT_TALK, 9, 0x240, MSGNO(MsgArutinMuraOfCourseOurHouseWillBe)},
	{EVENT_TALK, 9, 0x909, MSGNO(MsgArutinMuraIfTheMonsterThatDidThis)},
	{EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK_2)},
	{EVENT_TALK, 10, 0x241, MSGNO(MsgArutinMuraNotManyPeopleComeUpSilk)},
	{EVENT_TALK, 10, 0x909, EVENT_SCRIPT(EV10_TALK)},
	{EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgArutinMuraAncientRuinsAndTreasureDefinitelyGo)},
	{EVENT_TALK, 11, 0x241, MSGNO(MsgArutinMuraItsBeenALongTimeSince)},
	{EVENT_TALK, 11, 0x909, MSGNO(MsgArutinMuraDontBeTooHarshOnThose)},
	{EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgArutinMuraIfOnlyTheyHadLeftA)},
	{EVENT_TALK, 12, 0x909, MSGNO(MsgArutinMuraIDidntWantToLeaveMy)},
	{EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgArutinMuraReallyAncientRuinsAtTheBottom)},
	{EVENT_TALK, 13, 0x909, MSGNO(MsgArutinMuraIHeardSomeoneKilledTheWater)},
	{EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgArutinMuraThereProbablyIsntMuchTreasureIn)},
	{EVENT_TALK, 14, 0x909, EVENT_SCRIPT(EV14_TALK_2)},
	{EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgArutinMuraSomethingsWrongWithMomAndPop)},
	{EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK)},
	{EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTwentyFlagBranch)},
	{EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTwentyOneFlagBranch)},
	{EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK)},
	{EVENT_TALK, 19, 0x909, MSGNO(MsgArutinMuraIdGiveAnythingForSomeoneTo)},
	{EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgArutinMuraThereIsAnEndlessDesertWest)},
	{EVENT_TALK, 20, 0x909, MSGNO(MsgArutinMuraICameFromXianICook)},
	{EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgArutinMuraWestOfTheMountainsIsVery)},
	{EVENT_TALK, 21, 0x240, MSGNO(MsgArutinMuraNoneOfUsCanGoHome)},
	{EVENT_TALK, 21, 0x241, MSGNO(MsgArutinMuraTheGuestsInRoomOneMoved)},
	{EVENT_TALK, 21, 0x909, MSGNO(MsgArutinMuraTheGuestsInRoomTwoAlso)},
	{EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x240,
		MSGNO(MsgArutinMuraLeavingAltinForTolbiWellThats)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x909, MSGNO(MsgArutinMuraItsGoingToTakeALot)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraTheMerchantsMustBeHavingA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x240, MSGNO(MsgArutinMuraNowIFinallyHaveAnExcuse)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x909, MSGNO(MsgArutinMuraOneBlowsNotEnoughJabJab)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraIWonderWhatBusinessThatGirl)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x241, MSGNO(MsgArutinMuraOfCourseYouCantGoWest)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x909,
		MSGNO(MsgArutinMuraIThoughtTheVillageWasDone)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraTheySayYouCanStillFind)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x241,
		MSGNO(MsgArutinMuraIHateSittingAroundLikeThis)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x909, MSGNO(MsgArutinMuraImJustGladToGetMy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraHowCanITrulyBeHappy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x909, MSGNO(MsgArutinMuraTheBottomOfTheMineThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraCripesAncientRuinsDownInThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x909,
		MSGNO(MsgArutinMuraMyHusbandsSenseForFindingTreasure)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraIHadNoIdeaThereWere)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x909,
		MSGNO(MsgArutinMuraMomAndPopKnowSomethingAbout)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraIWonderIfMomAndPop)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x242, MSGNO(MsgArutinMuraCripesIsThisHowMyWeapon)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x909, MSGNO(MsgArutinMuraOnlyAMerchantKnowsTheJoy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraIveHeardOfAPassageIn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x909, MSGNO(MsgArutinMuraWeDriedOffAllThatArmor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraWellIfYouAreImpressedThen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x909, MSGNO(MsgArutinMuraAFewThingsGotDampBut)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraTheGirlFromXianBoughtAll)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x909,
		MSGNO(MsgArutinMuraWellComeThroughThisAllRight)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraIThinkThatTempleWasCalled)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0x909, MSGNO(MsgArutinMuraIKnowTheyDontLookSo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraYouWontBeAbleToGo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0x909, MSGNO(MsgArutinMuraIBecameACookAfterI)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraNoOneCanGoWestOf)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, 0x240, MSGNO(MsgArutinMuraMostOfTheVillageIsStill)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, 0x909, MSGNO(MsgArutinMuraItLooksLikeTheWaterHas)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
		MSGNO(MsgArutinMuraItWillTakeSomeTimeBefore)},
	{SEARCH(SEARCH_BARREL), 100, 0xf73, FIND_ITEM(ITEM_NUT)},
	{SEARCH(SEARCH_BARREL), 101, 0xf74, FIND_COINS(9)},
	{SEARCH(SEARCH_BED), 102, 0xf75, FIND_ITEM(ITEM_SLEEP_BOMB)},
	{SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgArutinMuraThePotsAndPansAreRusted))},
	{SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgArutinMuraHeIsMakingRoastBeefEven))},
	{SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgArutinMuraHeIsMakingShrimpInA))},
	{SEARCH(SEARCH_BOOKCASE), 203, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgArutinMuraThereIsABookTitledAltin))},
	{SCENE_EVENTS_END},
};

u16 ArutinMura_TriggerTable1[] = {
	MAP_CELL_STEP(42, 33, 2, 2, 0),
	MAP_CELL_STEP(42, 35, 2, 2, 5),
	MAP_CELL_STEP(40, 33, 2, 2, 0),
	MAP_CELL_STEP(40, 35, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

u16 ArutinMura_TriggerTable0[] = {
	MAP_CELL_STEP(35, 48, 2, 2, 5),
	MAP_CELL_STEP(33, 48, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

u16 ArutinMura_TriggerTable5[] = {
	MAP_CELL_STEP(35, 35, 2, 2, 5),
	MAP_CELL_STEP(33, 35, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

u16 ArutinMura_TriggerTable6[] = {
	MAP_CELL_STEP(35, 46, 2, 2, 5),
	MAP_CELL_STEP(33, 46, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

u16 ArutinMura_TriggerTable2[] = {
	MAP_CELL_STEP(35, 50, 2, 2, 5),
	MAP_CELL_STEP(33, 50, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

u16 ArutinMura_TriggerTable3[] = {
	MAP_CELL_STEP(35, 33, 2, 2, 5),
	MAP_CELL_STEP(33, 33, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

u16 ArutinMura_TriggerTable4[] = {
	MAP_CELL_STEP(35, 52, 2, 2, 5),
	MAP_CELL_STEP(33, 52, 2, 2, 5),
	MAP_CELL_STEPS_END,
};

struct MapCellAnim ArutinMura_TriggerTable[] = {
	{ArutinMura_TriggerTable0, 41, 5},
	{ArutinMura_TriggerTable1, 51, 6},
	{ArutinMura_TriggerTable2, 57, 17},
	{ArutinMura_TriggerTable3, 39, 17},
	{ArutinMura_TriggerTable4, 48, 25},
	{ArutinMura_TriggerTable5, 59, 9},
	{ArutinMura_TriggerTable6, 42, 23},
	{ArutinMura_TriggerTable3, 41, 27},
};
