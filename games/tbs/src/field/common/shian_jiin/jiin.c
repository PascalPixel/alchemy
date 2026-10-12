#include "game_state.h"
#include "scene_ids.h"
#include "unit/field/common/shian_jiin/jiin.h"
#include "unit/field/common/effect/move.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/sparkle.h"
#include "unit/object/evbasic.h"
#include "unit/object/evactor.h"
#include "unit/object/evcamera.h"
#include "unit/object/djinnev.h"
#include "unit/object/psydisp.h"
#include "unit/object/psyfx.h"
#include "unit/runtime/sched.h"
#include "unit/runtime/math.h"
#include "fieldobj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "global_cells.h"

// 前方宣言
static void FieldScene_RunSecondEnsembleBeat(void);
static void FieldScene_RunEnsembleStoryBeat(void);
static void FieldScene_RunOpeningAuxiliarySequence(s32 a0, s32 a1);
static void FieldScene_SetFlag140AndFinishSequence(s32 arg0, s32 arg1);
static void FieldScene_FinishSequence(void);
static void FieldScene_SpawnEightShots(void);
static void FieldScene_RunParticleRain(void);

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../effect/create.c"
#include "../effect/move.c"
#include "../effect/spawn.c"

static s32 StopXianActor(void *actor)
{
	Engine_ActorSetSpriteFlags(actor, 0);
	return 0;
}

static s32 FaceXianActorToPlayer(struct FieldActor *actor)
{
	struct FieldActor *player = Object_GetById(ACTOR_PARTY_LEADER);
	actor->facing = ArcTan2(player->z.fixed - actor->z.fixed, player->x.fixed - actor->x.fixed);
	return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_ShianJiin1)
	{
		return gShianJiinEntrances1;
	}
	return gShianJiinEntrancesOther;
}

s32 GetXianInitialState(void)
{
	return 0;
}

s32 GetXianMessageData(void)
{
	return (s32)ShianJiin_XianMessages;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	if (gGameState.scene == (s32)&SceneId_ShianJiin1)
	{
		return gShianJiinPlacements1;
	}
	if (gGameState.entrance == 3)
	{
		return gShianJiinPlacementsEntrance3;
	}
	if (GameFlag_IsSet(0x895))
	{
		gShianJiinPlacements[5].condition = 0x895;
		gShianJiinPlacements[7].condition = 0x895;
		gShianJiinPlacements[8].x = 0x1200000;
		gShianJiinPlacements[8].z = 0xf80000;
		gShianJiinPlacements[11].condition = 0x895;
		gShianJiinPlacements[12].condition = 0x895;
	}
	FieldScene_PrepareActors(gShianJiinPlacements);
	return gShianJiinPlacements;
}

static void EV08_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgShianHowDidGetHereBridge);
	if (GameFlag_IsSet(0x890) != 0)
	{
		gEventWork->message += 4;
	}
	Event_OpenMessage(8, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		GameFlag_Set(0x890);
	}
	else
	{
		gEventWork->message++;
	}
	Event_ShowMessage(8, 0);
	Engine_EventEnd();
}

static void EV12_TALK_1(void)
{

	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x88F) != 0)
	{
		Engine_EventSetMessage((s32)MsgShianNowHeTrulyBeyondWorlds);
		Engine_EventAskYesNo(12, 0);
		Engine_EventEnd();
	}
	else
	{
		Engine_EventSetMessage((s32)MsgShianIsntNobleHimTrySave);
		Engine_EventOpenMessage(12, 0);
		if (Engine_EventChooseYesNo(0, 0) == 1)
		{
			u16 *q = &gEventWork->message;
			q[0]++;
			Engine_EventOpenMessage(12, 0);
			if (Engine_EventChooseYesNo(0, 0) == 1)
			{
				u16 *r = &gEventWork->message;
				r[0]++;
			}
		}
		Engine_EventShowMessage(12, 0);
		Engine_EventEnd();
	}
}

static void EV09_TALK_1(void)
{
	s32				  *p8;
	struct FieldActor *master;
	struct FieldActor *actor;
	u8				   slot16[40];

	master = Object_GetById(9);
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgShianAlreadyHeardTest);
	Engine_EventShowMessageAndWait(9, 0, 20);
	Actor_WalkToAndWait(0, 168, 0x188);
	Engine_ActorFaceDirection(0, 0xc000, 20);
	Engine_AudioPlayCue(132);
	actor = Object_GetById(9);
	actor->velocity_y = 0x140000;
	actor = Object_GetById(9);
	actor->gravity = 0x40000;
	Actor_SetSpeed(9, 0x30000, 0x18000);
	Actor_SetDestination(9, 152, 0x188);
	Engine_ActorWaitForMove(9);
	actor = Object_GetById(9);
	actor->gravity = 0x10000;
	Engine_ActorFaceDirection(9, 0, 0);
	Engine_AudioPlayCue(132);
	p8 = (s32 *)slot16;
	p8[1] = 7;
	Effect_Spawn(
		master->x.fixed, master->y.fixed, (master->z.fixed + 0x40000), 0x8000, 0, 0, 0x10000, p8);
	Effect_Spawn(
		master->x.fixed, master->y.fixed, (master->z.fixed + 0x40000), 0, 0, 0, 0x10000, p8);
	Effect_Spawn(
		master->x.fixed, master->y.fixed, (master->z.fixed + 0x40000), -0x8000, 0, 0, 0x10000, p8);
	Engine_EventWait(30);
	BattleFx_PlayQueuedSound();
	Map_CopyCellAttributes(10, 24, 1, 1, 10, 22);
	Engine_GameFlagSet(0x892);
	Engine_EventEnd();
}

static void EV05_TOUCH(void)
{
	Engine_EventBegin();
	Engine_GameFlagSet(2196);
	Engine_ActorFaceActor(9, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(10);
	Engine_EventSetMessage((s32)MsgShianYoungMasterDidCompleteTest);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(0, 32768, 20);
	Engine_EventAskYesNo(9, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(9, 256, 80);
	Actor_FaceDirection(9, 53248, 20);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_EventWait(20);
	Engine_EventShowMessageAndWait(9, 0, 20);
	Engine_ActorFaceDirection(9, 0, 20);
	Engine_ActorSetAnimationAndWait(9, 3);
	Engine_EventWait(20);
	Engine_EventShowMessageAndWait(9, 0, 20);
	Map_CopyCellAttributes(10, 26, 1, 1, 10, 24);
	Engine_EventEnd();
}

static void Scene_RunScene39eSequenceB(void)
{
	s32				   rec7;
	struct FieldActor *record;

	Engine_EventBegin();
	rec7 = GameFlag_IsSet(0x300);
	if (rec7 != 0)
	{
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 0x1f8);
		Engine_EventWait(5);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
		Object_GetById(8)->paused = 0;
		Event_PlayCue(152);
		record = Actor_Get(8);
		record->velocity_y = 0x80000;
		Engine_ActorSetAnimation(8, 1);
		Engine_EventWait(30);
		Engine_EventSetMessage((s32)MsgShianMustSurviveTestGrottoBefore);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgShianMmmmWhoWhoSpeaksMy);
		FieldScene_SetFlag140AndFinishSequence(0, 8);
		Engine_EventWait(30);
		Event_ShowMessage(8, 0);
		FieldScene_FinishSequence();
		Engine_EventWait(20);
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 0x1f8);
		Engine_EventWait(5);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
		Event_PlayCue(152);
		Object_GetById(8)->paused = rec7;
		record = Actor_Get(8);
		record->velocity_y = 0x80000;
		Engine_ActorSetAnimation(8, 1);
		Engine_EventWait(30);
		Event_OpenMessage(8, 0);
		if (Engine_EventChooseYesNo(0, 0) == 1)
		{
			Engine_ActorRunRepeatedMotion(8, 2);
			Engine_EventWait(20);
			Event_ShowMessage(8, 0);
			Engine_EventWait(20);
			FieldScene_SetFlag140AndFinishSequence(8, 0);
			Engine_EventWait(30);
			Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			Engine_EventWait(50);
			FieldScene_FinishSequence();
			Engine_EventWait(30);
			Engine_ActorSetAnimationAndWait(8, 3);
			Event_ShowMessage(8, 0);
		}
		else
		{
			gEventWork->message += 2;
			Event_ShowMessage(8, 0);
		}
		Engine_ActorSetAnimationAndWait(8, 3);
		Engine_EventWait(30);
		Actor_ShowEmote(8, 0x100, 60);
		Engine_EventSetMessage((s32)MsgShianFollowThemDoNot);
		Event_OpenMessage(8, 0);
		if (Engine_EventChooseYesNo(0, 0) == 1)
		{
			Actor_ShowEmote(8, 0x105, 60);
			FieldScene_SetFlag140AndFinishSequence(8, 0);
			Engine_EventWait(30);
			Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			Engine_EventWait(50);
			FieldScene_FinishSequence();
			Engine_EventWait(30);
			Event_ShowMessage(8, 0);
			gEventWork->message += 1;
		}
		else
		{
			gEventWork->message += 1;
			Engine_EventWait(20);
			Engine_ActorSetAnimationAndWait(8, 3);
			Engine_EventWait(20);
			Event_ShowMessage(8, 0);
		}
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(8, 4);
		Engine_EventWait(20);
		Event_ShowMessage(8, 0);
		Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		Engine_EventWait(20);
		Event_ShowMessage(8, 0);
		Engine_EventWait(20);
		Actor_FaceDirection(8, 0xc000, 20);
		Actor_SetSpeed(8, 0x4ccc, 0x2666);
		Actor_WalkToAndWait(8, 168, 0x1d0);
		Engine_EventWait(60);
		Actor_FaceDirection(8, 0x4000, 40);
		Event_ShowMessageAndWait(8, 0, 10);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
		Actor_WalkToAndWait(8, 168, 0x1d8);
	}
	Event_OpenMessage(8, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		Engine_EventSetMessage((s32)MsgShianThenCannotTellGiveUp);
		Event_ShowMessage(8, 0);
		GameFlag_Set(0x300);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgShianDoNotWorryWillPermitted);
		Engine_EventWait(30);
		Engine_ActorSetAnimationAndWait(8, 3);
		Engine_EventWait(20);
		Camera_SetSpeed(0x8000, 0x1000);
		Engine_CameraMoveToActor(8, 1);
		ColorBuffer_ApplySource(0x10000, 0);
		ColorBuffer_ApplyTarget(0x10003, 1);
		Engine_ColorBufferInterpolate(30);
		Engine_EventWaitForScreen();
		Engine_CameraWaitForMove();
		FieldScene_SpawnEightShots();
		ColorBuffer_ApplyTarget(0x10000, 0);
		Engine_ColorBufferInterpolate(30);
		Event_ShowMessage(8, 0);
		Engine_ActorRunRepeatedMotion(8, 2);
		Engine_EventWait(20);
		Event_ShowMessage(8, 0);
		Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
		Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
		Engine_EventWait(60);
		Event_ShowMessageAndWait(8, 0, 10);
		GameFlag_Set(0x891);
	}
	Engine_ActorSetAnimation(8, 5);
	Engine_EventEnd();
}

static void EV1032_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgShianEnjoyReadingMindsOthersDo);
	Engine_EventAskYesNo(8, 0);
	Engine_EventEnd();
}

static void EV09_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgShianMasterFehsSchoolCameWatch);
	Engine_EventAskYesNo(9, 0);
	Engine_EventEnd();
}

static void EV12_TALK(void)
{
	Engine_EventBegin();

	Object_GetById(12)->paused = 0;

	goto testPendingWork;
waitPendingWork:
	Engine_TaskWait(1);
testPendingWork:
	if (Object_GetById(12)->y.fixed > 0)
	{
		goto waitPendingWork;
	}

	Object_GetById(12)->y.fixed = 0;

	Object_GetById(12)->target_y = ACTOR_NO_TARGET;

	Object_GetById(12)->velocity_y = 0;

	Object_GetById(12)->paused = 1;

	Engine_ActorFaceActor(12, ACTOR_PARTY_LEADER, 0);

	if (Engine_GameFlagIsSet(0x895) != 0)
	{
		Engine_EventSetMessage((s32)MsgShianHsuDidNotPracticeJumping);
	}
	else if (Engine_GameFlagIsSet(0x89b) != 0)
	{
		Engine_EventSetMessage((s32)MsgShianLamaTempleFarWestIn);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgShianFlexibilityJumpingVeryImportantIn);
	}

	Engine_EventShowMessage(12, 0);

	Object_GetById(12)->facing = 128 << 7;

	Object_GetById(12)->paused = 0;

	Engine_ActorEnableActionCallback(12, ShianJiin_ActorTwelveScript);
	Engine_EventEnd();
}

static void EV15_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgShianMasterFehVeryBusyDo);
	Engine_EventAskYesNo(15, 0);
	Engine_EventEnd();
}

static void FieldScene_RunForwardArcBurst(void)
{
	struct FieldActor *actor = Object_GetById(19);
	u32				   index;
	s32				   angle;

	for (index = 8; index > 3; index--)
	{
		angle = index << 12;
		actor->sprite->rotation = angle;
		Engine_TaskWait((index - 4) * 2);
		actor->x.fixed += Engine_MathCos(angle) * 6;
		actor->z.fixed += Engine_MathSin(angle) * 6;
	}

	actor->y.fixed = 0x120000;
	actor->target_y = 0x120000;

	Engine_AudioPlayCue(227);

	Effect_Spawn(actor->x.fixed - 0xc0000, actor->y.fixed, actor->z.fixed + 0x80000, 0xffffcccd,
		0x6666, 0, 0, 0);
	Effect_Spawn(
		actor->x.fixed, actor->y.fixed, actor->z.fixed + 0x80000, 0xffff3334, 0x4ccc, 0, 0, 0);
	Effect_Spawn(actor->x.fixed + 0xa0000, actor->y.fixed, actor->z.fixed + 0x80000, 0xffff0000,
		0x3333, 0, 0, 0);
}

static void ShianJiin_SpinAway(void)
{
	struct FieldActor *actor;
	u32				   i;
	s32				   angle;

	actor = Object_GetById(19);
	for (i = 8; i <= 12; i++)
	{
		angle = i << 12;
		actor->sprite->rotation = angle;
		Engine_TaskWait((12 - i) * 2);
		actor->x.fixed -= Engine_MathCos(angle) * 6;
		actor->z.fixed -= Engine_MathSin(angle) * 6;
	}
	actor->y.fixed = 0x120000;
	actor->target_y = 0x120000;
	actor->scale_x = 0xffff3334;
	Engine_AudioPlayCue(227);
	Effect_Spawn(actor->x.fixed - 0xc0000, actor->y.fixed, actor->z.fixed + 0x80000, 0x10000,
		0x3333, 0, 0, 0);
	Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed + 0x80000, 0xcccc, 0x4ccc, 0, 0, 0);
	Effect_Spawn(actor->x.fixed + 0xa0000, actor->y.fixed, actor->z.fixed + 0x80000, 0x3333, 0x6666,
		0, 0, 0);
}

static void FieldScene_RunDescentBurst(void)
{
	struct FieldActor *actor = Object_GetById(19);
	u32				   i = 0;
	s32				   step = 8;
	u8				   zero;
	do
	{
		Engine_TaskWait(step);
		actor->z.fixed += 0xffff0000;
		actor->target_z = ACTOR_NO_TARGET;
		i++;
		step -= 2;
	} while (i <= 3);
	zero = 0;
	actor->sprite->rotation = zero;
	Engine_AudioPlayCue(227);
	Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed + 0xfff80000, 0xffff3334, 0,
		0xffffcccd, 0, 0);
	Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed + 0xfff80000, 0x0000cccc, 0,
		0xffffcccd, 0, 0);
	Effect_Spawn(actor->x.fixed + 0xfffa0000, actor->y.fixed, actor->z.fixed + (160 << 12),
		0x00003333, 0, 0xffff0000, 0, 0);
	Effect_Spawn(actor->x.fixed + (192 << 11), actor->y.fixed, actor->z.fixed + (160 << 12),
		0x00003333, 0, 0xffff0000, 0, 0);
}

static void Scene_RunPrimarySequence(void)
{
	struct FieldActor *actor;
	u32				   count;
	s32				   base;
	s32				   slot;

	actor = Object_GetById(19);
	count = 0;
	slot = 8;
	do
	{
		Engine_TaskWait(slot);
		actor->z.fixed += 0x10000;
		actor->target_z = ACTOR_NO_TARGET;
		count++;
		slot -= 2;
	} while (count <= 3);

	actor->sprite->rotation = 0;
	actor->z.fixed += 0x180000;
	actor->target_z = ACTOR_NO_TARGET;
	Engine_AudioPlayCue(227);

	Effect_Spawn(
		actor->x.fixed, actor->y.fixed, actor->z.fixed + 0xc0000, -0xcccc, 0, 0x3333, 0, 0);
	base = 0x3333;
	Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed + 0xc0000, 0xcccc, 0, base, 0, 0);
	Effect_Spawn(
		actor->x.fixed - 0x60000, actor->y.fixed, actor->z.fixed - 0x80000, base, 0, 0x10000, 0, 0);
	Effect_Spawn(
		actor->x.fixed + 0x60000, actor->y.fixed, actor->z.fixed - 0x80000, base, 0, 0x10000, 0, 0);
}

static void ShianJiin_WalkByFacing(void)
{
	u32 dir;
	u32 limit;

	dir = Object_GetById(0)->facing;
	limit = 0x3fff;
	if (((dir + -0x2000) << 16) <= 0x3fff0000)
	{
		Engine_ActorWalkToAndWait(15, 216, 168);
		Engine_ActorWalkToAndWait(15, 224, 168);
		Actor_FaceDirection(15, 0x2000, 20);
	}
	else if ((u16)(dir - 0x6000) <= limit)
	{
		Engine_ActorWalkToAndWait(15, 232, 160);
		Actor_FaceDirection(15, 0x5000, 20);
	}
	else if ((u16)(dir + 0x6000) <= limit)
	{
		Engine_ActorWalkToAndWait(15, 216, 168);
		Engine_ActorWalkToAndWait(15, 224, 172);
		Actor_FaceDirection(15, 0xe000, 20);
	}
	else
	{
		Engine_ActorWalkToAndWait(15, 232, 160);
		Actor_FaceDirection(15, 0x2000, 20);
	}
}

static void ShianJiin_RunMasterScene(s32 mode)
{
	s32 msg;

	Actor_SetSpeed(15, 0xcccc, 0x6666);
	Engine_EventWait(60);
	msg = (s32)MsgShianCannotPushHands;
	Engine_EventSetMessage(msg);
	if (mode == 0)
	{
		Engine_EventSetMessage(msg - 1);
		Actor_ShowEmote(15, 0x101, 60);
		Engine_EventShowMessageAndWait(15, 0, 20);
		Engine_ActorRunRepeatedMotion(15, 2);
		Engine_EventSetMessage((s32)MsgShianWarriorsCannotUse);
		Engine_EventShowMessageAndWait(15, 0, 20);
		Engine_ActorSetAnimationAndWait(15, 4);
		Engine_EventWait(20);
		Engine_EventShowMessageAndWait(15, 0, 20);
		Engine_ActorSetAnimationAndWait(15, 3);
		Engine_EventWait(20);
	}
	if (mode == 2)
	{
		Engine_EventSetMessage((s32)MsgShianGreatWarriorTrain);
		Engine_ActorRunRepeatedMotion(15, 2);
		Engine_EventWait(20);
	}
	Engine_EventShowMessageAndWait(15, 0, 20);
	ShianJiin_WalkByFacing();
	Engine_ActorRunRepeatedMotion(15, 3);
	Actor_SetPosition(19, 0xe80000, 0xa80000);
	Engine_ActorSetPosition(20, 0xe80000, 0xa80000);
	Object_GetById(19)->y.fixed = 0xc0000;
	Object_GetById(19)->target_y = ACTOR_NO_TARGET;
	Object_GetById(19)->scale_x = 0xcccc;
	Object_GetById(19)->sprite->rotation = 0x8000;
	Engine_AudioPlayCue(124);
	Engine_EventWait(40);
	Engine_ActorWalkToAndWait(15, 216, 152);
	Actor_FaceDirection(15, 0x2000, 30);
}

static void ShianJiin_RunGatheringScene(void)
{
	struct FieldActor *record;

	Engine_ActorFaceActor(13, 19, 0);
	Engine_ActorFaceActor(14, 19, 0);
	Engine_ActorFaceActor(15, 19, 0);
	Engine_ActorFaceActor(16, 19, 0);
	Engine_ActorFaceActor(18, 19, 0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(15, 2);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgShianLookTreeFell);
	Engine_EventShowMessageAndWait(15, 0, 20);
	Engine_EventShowMessageAndWait(16, 0, 20);
	Actor_ShowEmote(18, 0x105, 60);
	Actor_ShowEmote(16, 0x101, 60);
	Engine_EventShowMessageAndWait(16, 0, 20);
	Engine_ActorSetAnimationAndWait(18, 4);
	Engine_EventWait(20);
	Engine_EventShowMessageAndWait(18, 0, 20);
	Actor_ShowEmote(16, 0x102, 60);
	Engine_EventShowMessageAndWait(16, 0, 20);
	Engine_ActorFaceEachOther(15, 18, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(18, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(15, 3);
	Engine_EventWait(20);
	Actor_SetSpeed(15, 0xcccc, 0x6666);
	ShianJiin_WalkByFacing();
	Engine_ActorRunRepeatedMotion(15, 3);
	Actor_SetPosition(19, 0xe80000, 0xa80000);
	Engine_ActorSetPosition(20, 0xe80000, 0xa80000);
	record = Object_GetById(19);
	record->y.fixed = 0xc0000;
	record = Object_GetById(19);
	record->target_y = ACTOR_NO_TARGET;
	record = Object_GetById(19);
	record->scale_x = 0xcccc;
	record = Object_GetById(19);
	{
		struct FieldSprite *sprite = record->sprite;
		s32					rotation = 0x8000;

		sprite->rotation = rotation;
	}
	Engine_AudioPlayCue(124);
	Engine_EventWait(40);
	Engine_ActorWalkToAndWait(15, 216, 152);
	Actor_FaceDirection(15, 0x4000, 30);
	Engine_GameFlagSet(0x301);
}

static void EV19_PSYNERGY(void)
{
	Engine_EventBegin();

	if (Object_GetById(0)->facing > (128 << 7) && Object_GetById(0)->facing < (192 << 8))
	{
		FieldScene_RunForwardArcBurst();
	}
	else
	{
		ShianJiin_SpinAway();
	}

	if (Engine_GameFlagIsSet(0x898) != 0)
	{
		ShianJiin_RunGatheringScene();
	}
	else
	{
		ShianJiin_RunMasterScene(0);
	}

	Engine_EventEnd();
}

static void FieldScene_DispatchByFacing(void)
{
	struct FieldActor *record = Object_GetById(ACTOR_PARTY_LEADER);
	u16				   angle;

	Engine_EventBegin();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
	Engine_EventWait(20);

	angle = record->facing;

	if ((u16)(angle - 0x2000) <= 0x3fffu)
	{
		Scene_RunPrimarySequence();
	}
	else if ((u16)(angle - 0x6000) <= 0x3fffu)
	{
		FieldScene_RunForwardArcBurst();
	}
	else if ((u16)(angle + (192 << 7)) <= 0x3fffu)
	{
		FieldScene_RunDescentBurst();
	}
	else
	{
		ShianJiin_SpinAway();
	}

	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	ShianJiin_RunMasterScene(1);
	Engine_EventEnd();
}

static void EV20_PSYNERGY(void)
{
	struct FieldActor *record = Object_GetById(ACTOR_PARTY_LEADER);
	u16				   facing;

	Engine_EventBegin();

	facing = record->facing;
	if ((u16)(facing - 0x2000) <= 0x3fff)
	{
		Scene_RunPrimarySequence();
	}
	else if ((u16)(facing - 0x6000) <= 0x3fff)
	{
		FieldScene_RunForwardArcBurst();
	}
	else if ((u16)(facing + 0x6000) <= 0x3fff)
	{
		FieldScene_RunDescentBurst();
	}
	else
	{
		ShianJiin_SpinAway();
	}

	Engine_CameraSetSpeed(0x10000, 0x2000);
	Engine_CameraMoveToActor(20, 1);
	Engine_CameraWaitForMove();

	if (record->z.part.pixel <= 209)
	{
		if (Engine_GameFlagIsSet(0x89a) == 0)
			goto scene0;
		if (Engine_GameFlagIsSet(0x89b) != 0)
			goto scene0;
		goto scene1;
	scene0:
		ShianJiin_RunMasterScene(0);
		goto firstSceneComplete;
	scene1:
		ShianJiin_RunGatheringScene();
	firstSceneComplete:
		Engine_EventEnd();
		return;
	}

	if (Engine_GameFlagIsSet(0x89b) != 0)
	{
		ShianJiin_RunMasterScene(2);
	}
	else if (Engine_GameFlagIsSet(0x89a) == 0)
	{
		FieldScene_RunSecondEnsembleBeat();
	}
	else
	{
		FieldScene_RunEnsembleStoryBeat();
	}
	Engine_EventEnd();
}

static void FieldScene_RunSecondEnsembleBeat(void)
{
	s32				   id;
	struct FieldActor *rec;

	GameFlag_Set(0x89a);
	Engine_EventWait(30);
	Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(15, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(16, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Actor_ShowEmote(13, 128 << 1, 0);
	Actor_ShowEmote(15, 128 << 1, 0);
	Actor_ShowEmote(16, 128 << 1, 0);
	Engine_EventWait(60);
	Engine_EventSetMessage((s32)MsgShianDidDoWarrior);
	Event_ShowMessageAndWait(13, 0, 20);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
	Engine_ActorRunRepeatedMotion(15, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(15, 0, 20);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 15, 0);
	Engine_ActorRunRepeatedMotion(16, 2);
	Engine_EventWait(20);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 16, 0);
	Event_AskYesNo(16, 0);
	Engine_EventWait(50);
	Engine_CameraFollowActor(16, 1);
	Actor_SetSpeed(16, 0xcccc, 0x6666);
	Actor_WalkToAndWait(16, 176, 248);
	Actor_WalkToAndWait(16, 154 << 1, 248);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 128 << 6, 0);
	Actor_FaceDirection(16, 192 << 8, 20);
	Event_PlayCue(158);
	Map_AnimateCells(ShianJiin_CellStepsB, 78, 13);
	Engine_ActorRunRepeatedMotion(16, 2);
	Engine_EventWait(20);
	Actor_SetSpeed(16, 192 << 9, 192 << 8);
	rec = Object_GetById(16);
	rec->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(16, 154 << 1, 136 << 1);
	Engine_EventWait(1);
	rec = Actor_Get(16);

	rec->steering_flags = ACTOR_FACES_MOTION | rec->steering_flags;
	Event_ShowMessageAndWait(16, 0, 50);
	Actor_SetPosition(17, 152 << 17, 216 << 16);
	Actor_WalkToAndWait(17, 152 << 1, 248);
	Actor_FaceActor(9, 17, 0);
	Actor_FaceActor(10, 17, 0);
	Actor_FaceActor(11, 17, 0);
	Actor_FaceActor(12, 17, 0);
	Actor_FaceActor(13, 17, 0);
	Actor_FaceActor(14, 17, 0);
	Actor_FaceActor(15, 17, 0);
	Actor_FaceActor(16, 17, 0);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 17, 0);
	Engine_EventWait(10);
	Engine_ActorStartRepeatedMotion(9, 2);
	Engine_ActorStartRepeatedMotion(10, 2);
	Engine_ActorStartRepeatedMotion(11, 2);
	Engine_ActorStartRepeatedMotion(12, 2);
	Engine_ActorStartRepeatedMotion(13, 2);
	Engine_ActorStartRepeatedMotion(14, 2);
	Engine_ActorStartRepeatedMotion(15, 2);
	Engine_ActorRunRepeatedMotion(16, 2);
	Actor_ShowEmote(17, 0x103, 60);
	Actor_SetPosition(18, 152 << 17, 216 << 16);
	Actor_WalkTo(18, 152 << 1, 248);
	Actor_WalkTo(17, 140 << 1, 132 << 1);
	Engine_ActorWaitForMove(18);
	Actor_FaceDirection(18, 160 << 7, 0);
	Engine_ActorWaitForMove(17);
	Event_PlayCue(159);
	Map_AnimateCells(ShianJiin_CellStepsC, 78, 13);
	Event_ShowMessageAndWait(18, 0, 20);
	Actor_FaceActor(9, 17, 0);
	Actor_FaceActor(10, 17, 0);
	Actor_FaceActor(11, 17, 0);
	Actor_FaceActor(12, 17, 0);
	Actor_FaceActor(13, 17, 0);
	Actor_FaceActor(14, 17, 0);
	Actor_FaceActor(15, 17, 0);
	Actor_FaceActor(16, 17, 0);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 17, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(17, 2);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(18, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Engine_ActorSetAnimationAndWait(17, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(18, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Actor_FaceDirection(17, 208 << 8, 20);
	Event_ShowMessageAndWait(17, 0, 20);
	Actor_ShowEmote(18, 0x102, 60);
	Event_ShowMessageAndWait(18, 0, 20);
	Engine_ActorSetAnimationAndWait(17, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(17, 0, 20);
	Engine_ActorSetAnimationAndWait(18, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Engine_ActorSetAnimationAndWait(17, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(18, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Engine_ActorRunRepeatedMotion(17, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(17, 0, 20);
	Event_ShowMessageAndWait(17, 0, 20);
	Actor_FaceDirection(16, 128 << 8, 20);
	Engine_ActorSetAnimationAndWait(16, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(17, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(17, 0, 20);
	Engine_ActorSetAnimationAndWait(16, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(16, 0, 20);
	Actor_FaceDirection(17, 128 << 8, 20);
	Event_ShowMessageAndWait(17, 0, 20);
	Engine_ActorSetAnimationAndWait(9, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(9, 0, 20);
	Actor_FaceDirection(17, 208 << 8, 20);
	Engine_ActorRunRepeatedMotion(17, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(17, 0, 20);
	Actor_ShowEmote(18, 0x102, 60);
	Event_ShowMessageAndWait(18, 0, 20);
	Actor_ShowEmote(17, 0x101, 60);
	Event_ShowMessageAndWait(17, 0, 20);
	Engine_ActorSetAnimationAndWait(18, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Actor_ShowEmote(17, 0x100, 60);
	Event_ShowMessageAndWait(17, 0, 20);
	Engine_ActorSetAnimationAndWait(18, 4);
	Engine_EventWait(20);
	Actor_ShowEmote(17, 0x103, 60);
	Event_ShowMessageAndWait(17, 0, 20);
	Actor_ShowEmote(18, 0x100, 60);
	Event_ShowMessageAndWait(18, 0, 20);
	Engine_ActorSetAnimationAndWait(17, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(17, 0, 20);
	Engine_ActorRunRepeatedMotion(18, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Engine_ActorRunRepeatedMotion(17, 2);
	Engine_EventWait(10);
	Actor_WalkToAndWait(17, 128 << 1, 140 << 1);
	Actor_FaceDirection(17, 128 << 7, 20);
	Actor_SetPosition(17, 0, 0);
	Engine_ActorDestroy(17);
	Engine_EventWait(30);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(9, 0, 20);
	Engine_ActorRunRepeatedMotion(15, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(15, 0, 20);
	Actor_FaceActor(16, 18, 0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(16, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(16, 0, 20);
	Actor_FaceActor(18, 16, 0);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Engine_ActorSetAnimationAndWait(18, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Engine_ActorSetAnimationAndWait(18, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Actor_SetSpeed(18, 0xcccc, 0x6666);
	Actor_WalkToAndWait(18, 128 << 1, 248);
	Actor_FaceDirection(18, 192 << 8, 20);
	Engine_ActorStartRepeatedMotion(18, 1);
	Actor_ShowEmote(18, 0x100, 60);
	Actor_WalkToAndWait(18, 240, 184);
	Engine_ActorRunRepeatedMotion(18, 2);
	Engine_EventWait(20);
	Actor_SetPosition(19, 232 << 16, 168 << 16);
	Actor_SetPosition(20, 232 << 16, 168 << 16);
	rec = Actor_Get(19);
	rec->y.fixed = 0xc0000;
	rec = Actor_Get(19);
	rec->target_y = ACTOR_NO_TARGET;
	rec = Actor_Get(19);
	rec->scale_x = 0xcccc;
	rec = Actor_Get(19);
	{
		struct FieldSprite *sprite = rec->sprite;
		s32					rotation = 0x8000;

		sprite->rotation = rotation;
	}
	Event_PlayCue(124);
	Event_ShowMessageAndWait(18, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 192 << 8, 20);
	Actor_WalkToAndWait(16, 128 << 1, 240);
	Actor_FaceDirection(16, 176 << 8, 20);
	Engine_ActorRunRepeatedMotion(16, 1);
	Event_ShowMessageAndWait(16, 0, 20);
	Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(15, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(16, ACTOR_PARTY_LEADER, 0);
	Engine_ActorRunRepeatedMotion(18, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(18, 160 << 7, 20);
	Actor_WalkToAndWait(18, 248, 208);
	Actor_FaceDirection(18, 160 << 7, 20);
	Event_OpenMessage(18, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		((s32 (*)())Engine_ActorRunRepeatedMotion)(16, 1);
		Engine_EventWait(20);
		id = 16;
		goto joinBeat;
	}

	Event_SkipMessages(1);
	Engine_EventWait(20);
	Actor_ShowEmote(18, 0x105, 60);
	Actor_FaceDirection(18, 128 << 7, 20);
	Engine_ActorRunRepeatedMotion(16, 2);
	Engine_EventWait(20);
	Event_OpenMessage(16, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		goto skipTwice;
	}
	Engine_ActorSetAnimationAndWait(16, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(18, 176 << 8, 20);
	id = 18;

joinBeat:
	Event_ShowMessageAndWait(id, 0, 20);
	GameFlag_Set(0x898);
	goto finish;

skipTwice:
	Event_SkipMessages(1);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(18, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	GameFlag_Set(0x899);

finish:
	Actor_FaceDirection(10, 128 << 8, 0);
	Actor_FaceDirection(11, 128 << 8, 20);
	Engine_ActorSetAnimation(10, 5);
	Engine_ActorSetAnimation(11, 5);
	Engine_ActorEnableActionCallback(12, ShianJiin_ActorTwelveScript);
}

static void EV18_TALK(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgShianWarriorWillShowMeYour);
	Engine_EventOpenMessage(18, 0);

	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_EventShowMessageAndWait(18, 0, 20);
		Engine_GameFlagSet(0x898);
		Engine_EventEnd();
	}
	else
	{
		gEventWork->message += 1;
		Engine_EventShowMessageAndWait(18, 0, 20);
		Engine_EventEnd();
	}
}

static void FieldScene_RunEnsembleStoryBeat(void)
{
	struct FieldActor *actor;

	actor = Actor_Get(18);
	actor->update = 0;
	actor = Object_GetById(13);
	actor->update = 0;
	actor = Object_GetById(14);
	actor->update = 0;
	actor = Object_GetById(15);
	actor->update = 0;
	actor = Actor_Get(16);
	actor->update = 0;
	Engine_ActorSetAnimation(11, 1);
	Camera_SetSpeed(0x8000, 0x1000);
	Camera_MoveTo(0xe80000, -1, 0xc80000, 1);
	Engine_CameraWaitForMove();
	Engine_EventSetMessage((s32)MsgShianYaTreeFell);
	Actor_SetSpeed(10, 0xcccc, 0x6666);
	Actor_SetSpeed(12, 0xcccc, 0x6666);
	Actor_WalkTo(10, 152, 200);
	Actor_WalkToAndWait(12, 144, 248);
	Engine_ActorWaitForMove(10);
	Actor_FaceActor(9, 19, 0);
	Actor_FaceActor(11, 19, 0);
	Actor_FaceActor(13, 19, 0);
	Actor_FaceActor(14, 19, 0);
	Actor_FaceActor(15, 19, 0);
	Actor_FaceActor(16, 19, 0);
	Actor_FaceActor(18, 19, 0);
	Actor_SetSpeed(10, 0x18000, 0xc000);
	Actor_SetSpeed(12, 0x20000, 0x10000);
	Actor_WalkTo(10, 152, 200);
	Actor_WalkToAndWait(12, 144, 248);
	Actor_FaceActor(12, 19, 0);
	Engine_ActorWaitForMove(10);
	Actor_FaceActor(10, 19, 0);
	Engine_ActorRunRepeatedMotion(18, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 40);
	Actor_FaceActor(9, 18, 0);
	Actor_FaceActor(10, 18, 0);
	Actor_FaceDirection(11, 0x3000, 0);
	Actor_FaceActor(12, 18, 0);
	Actor_FaceDirection(13, 0x3000, 0);
	Actor_FaceActor(14, 18, 0);
	Actor_FaceActor(15, 18, 0);
	Actor_FaceActor(16, 18, 0);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(16, 0, 20);
	Engine_ActorSetAnimationAndWait(18, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(16, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(16, 0, 20);
	Actor_ShowEmote(18, 0x105, 60);
	Actor_ShowEmote(16, 0x101, 60);
	Event_ShowMessageAndWait(16, 0, 20);
	Event_ShowMessageAndWait(18, 0, 20);
	Actor_ShowEmote(16, 0x102, 60);
	Actor_ShowEmote(15, 0x101, 60);
	Actor_SetSpeed(15, 0xcccc, 0x6666);
	Actor_WalkToAndWait(15, 216, 176);
	Actor_FaceDirection(15, 0x3000, 20);
	Event_ShowMessageAndWait(15, 0, 20);
	Actor_FaceDirection(18, 0xb000, 20);
	Engine_ActorSetAnimationAndWait(18, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Engine_ActorStartRepeatedMotion(9, 2);
	Engine_ActorStartRepeatedMotion(10, 2);
	Engine_ActorStartRepeatedMotion(11, 2);
	Engine_ActorStartRepeatedMotion(12, 2);
	Engine_ActorStartRepeatedMotion(13, 2);
	Engine_ActorStartRepeatedMotion(14, 2);
	Engine_ActorStartRepeatedMotion(15, 2);
	Engine_ActorStartRepeatedMotion(16, 2);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(13, 2);
	Event_ShowMessageAndWait(13, 0, 20);
	Engine_ActorSetAnimationAndWait(18, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 20);
	Actor_FaceDirection(18, 0x5000, 20);
	Event_AskYesNo(18, 0);
	Actor_ShowEmote(9, 0x101, 0);
	Engine_EventWait(5);
	Actor_ShowEmote(10, 0x101, 0);
	Engine_EventWait(5);
	Actor_ShowEmote(11, 0x101, 0);
	Engine_EventWait(5);
	Actor_ShowEmote(12, 0x101, 0);
	Engine_EventWait(5);
	Actor_ShowEmote(13, 0x101, 0);
	Engine_EventWait(5);
	Actor_ShowEmote(14, 0x101, 0);
	Engine_EventWait(5);
	Actor_ShowEmote(15, 0x101, 0);
	Engine_EventWait(5);
	Actor_ShowEmote(16, 0x101, 0);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(16, 2);
	Event_ShowMessageAndWait(16, 0, 20);
	Engine_ActorSetAnimationAndWait(18, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Actor_ShowEmote(15, 0x101, 60);
	Event_ShowMessageAndWait(15, 0, 20);
	Actor_FaceActor(18, 15, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(18, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 40);
	Engine_ActorFaceEachOther(11, 10, 0);
	Engine_ActorFaceEachOther(12, 14, 0);
	Engine_ActorFaceEachOther(13, 15, 0);
	Engine_EventWait(60);
	Actor_FaceActor(10, 18, 0);
	Actor_FaceActor(11, 18, 0);
	Actor_FaceActor(12, 18, 0);
	Actor_FaceActor(13, 18, 0);
	Actor_FaceActor(14, 18, 0);
	Actor_FaceActor(15, 18, 0);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Engine_ActorRunRepeatedMotion(18, 2);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(9, 3);
	Engine_ActorSetAnimation(10, 3);
	Engine_ActorSetAnimation(11, 3);
	Engine_ActorSetAnimation(12, 3);
	Engine_ActorSetAnimation(13, 3);
	Engine_ActorSetAnimation(14, 3);
	Engine_ActorSetAnimation(15, 3);
	Engine_ActorSetAnimationAndWait(16, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(18, 0x5000, 20);
	Event_AskYesNo(18, 0);
	Engine_ActorSetAnimationAndWait(18, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(9, 3);
	Engine_ActorSetAnimation(10, 3);
	Engine_ActorSetAnimation(11, 3);
	Engine_ActorSetAnimation(12, 3);
	Engine_ActorSetAnimation(13, 3);
	Engine_ActorSetAnimation(14, 3);
	Engine_ActorSetAnimation(15, 3);
	Engine_ActorSetAnimationAndWait(16, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(18, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Event_ShowMessageAndWait(18, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(18, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(18, 0x8000, 20);
	Engine_ActorRunRepeatedMotion(18, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(18, 0, 20);
	Engine_ActorSetAnimation(9, 3);
	Engine_ActorSetAnimation(10, 3);
	Engine_ActorSetAnimation(11, 3);
	Engine_ActorSetAnimation(12, 3);
	Engine_ActorSetAnimation(13, 3);
	Engine_ActorSetAnimation(14, 3);
	Engine_ActorSetAnimation(15, 3);
	Engine_ActorSetAnimationAndWait(16, 3);
	Engine_EventWait(20);
	Actor_WalkTo(10, 120, 200);
	Actor_WalkTo(12, 120, 248);
	Engine_ActorWaitForMove(10);
	Actor_FaceDirection(11, 0x8000, 20);
	Engine_ActorSetAnimation(10, 5);
	Engine_ActorSetAnimation(11, 5);
	Engine_ActorWaitForMove(12);
	Engine_ActorEnableActionCallback(12, ShianJiin_ActorTwelveScript);
	Actor_SetSpeed(15, 0xcccc, 0x6666);
	Actor_WalkToAndWait(15, 216, 168);
	Actor_WalkToAndWait(15, 232, 168);
	Actor_FaceDirection(15, 0xc000, 20);
	Engine_ActorRunRepeatedMotion(15, 3);
	Actor_SetPosition(19, 0xe80000, 0xa80000);
	actor = Actor_Get(19);
	actor->y.fixed = 0xc0000;
	actor = Actor_Get(19);
	actor->target_y = ACTOR_NO_TARGET;
	actor = Actor_Get(19);
	{
		struct FieldSprite *sprite = actor->sprite;
		s32					rotation = 0x8000;

		sprite->rotation = rotation;
	}
	Event_PlayCue(124);
	Engine_EventWait(40);
	Actor_WalkToAndWait(15, 216, 152);
	Actor_FaceDirection(15, 0x4000, 30);
	GameFlag_Clear(0x898);
	GameFlag_Set(0x89b);
}

static void EV11_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgShianWaterMonstersFloodedAltinDid);
	Engine_EventAskYesNo(11, 0);
	Engine_EventEnd();
}

static void EV03_TOUCH(void)
{
	Engine_EventBegin();
	if (GameFlag_IsSet(2202) == 0 && GameFlag_IsSet(2197) == 0)
	{
		Engine_MessageShowCentered((s32)MsgShianItIsLocked, 1);
		Engine_EventEnd();
	}
	else
	{
		Event_PlayCue(158);
		Map_AnimateCells(ShianJiin_CellStepsB, 78, 13);
		Actor_SetSpeed(0, 0x8000, 0x4000);
		Actor_WalkToAndWait(0, 306, 248);
		Actor_WalkTo(ACTOR_PARTY_LEADER, 304, 216);
		Engine_EventWait(20);
		Engine_EventRequestExit(4);
		Engine_EventEnd();
	}
}

static void EV13_TALK(void)
{
	struct FieldActor *record;
	u32				   biased;

	record = Object_GetById(ACTOR_PARTY_LEADER);
	biased = record->facing;
	Engine_EventBegin();

	biased += 0xffff5fff;
	if (biased <= 0x3ffe)
	{
		Engine_SanctumOpen(13);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgShianImTravelingAroundWorldSpread);
		Engine_EventShowMessage(13, 0);
	}

	Engine_EventEnd();
}

static void EV08_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventSetMessage((s32)MsgShianRobinAmCountingOnBring);
	Engine_EventShowMessage(8, 0);
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	if (gGameState.scene == (s32)&SceneId_ShianJiin1)
	{
		return gShianJiinEvents1;
	}
	if (gGameState.entrance == 3)
	{
		return gShianJiinEventsEntrance3;
	}
	return gShianJiinEvents;
}

static void FieldScene_SpawnRandomizedParticle(void)
{
	struct EffectOptions params;
	struct FieldActor	*record;
	s32					 draw;
	s32					 offset;

	record = Object_GetById(ACTOR_PARTY_LEADER);

	params.palette = 7;
	draw = (u32)(Engine_RandomNext() * 7) >> 16;
	if ((draw & 7) == 0)
		params.palette = 5;

	params.start_scale_x = 0xb333;
	params.start_scale_y = 0xcccc;

	offset = ((u32)(Engine_RandomNext() * 8) >> 16) * 13107;

	Effect_Spawn(record->x.fixed + ((8 - (gFrameCount & 15)) << 16), record->y.fixed + (192 << 13),
		record->z.fixed, 0, -offset, 0, 144 << 12, &params);

	if ((gFrameCount & 1) != 0)
		Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
	else
		Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 1);
}

static void FieldScene_ApplyOffset0Neg32(void)
{
	FieldScene_RunOpeningAuxiliarySequence(0, -32);
}

static void FieldScene_ApplyOffset0Pos32(void)
{
	FieldScene_RunOpeningAuxiliarySequence(0, 32);
}

static void FieldScene_ApplyOffsetNeg32_0(void)
{
	FieldScene_RunOpeningAuxiliarySequence(-32, 0);
}

static void FieldScene_RunOpeningAuxiliarySequence(s32 a0, s32 a1)
{
	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
	Engine_ActorSetDestinationOffset(0, a0, a1);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 7);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
	Engine_EventEnd();
}

static void EV03_TOUCH_1(void)
{
	struct FieldActor *record;
	s32				   particle_task;

	Engine_EventBegin();
	particle_task = (s32)FieldScene_SpawnRandomizedParticle;
	((void (*)())Engine_TaskAddCallback)(particle_task, 0xc80);
	Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
	gEventWork->transition_frames = 60;
	Engine_EventCloseScreen();
	Engine_AudioPlayCue(154);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
	record = Object_GetById(ACTOR_PARTY_LEADER);
	Engine_ActorSetSpriteFlags(record, 0);
	Scheduler_RemoveCallback(particle_task);
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(3);
	Engine_EventEnd();
}

static void EV01_SCRIPT(void)
{
	Engine_AudioPlayCue(123);
	Engine_EventRequestExit(1);
}

static void EV02_TOUCH(void)
{
	Engine_EventBegin();
	Event_PlayCue(188);
	Map_AnimateCells(ShianJiin_CellStepsA, 77, 8);
	Object_GetById(0)->motion_flags = 0;
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
	Engine_EventWait(16);
	Engine_EventRequestExit(2);
	Engine_EventEnd();
}

static void ShianJiin_AskIfCurious(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 168, 0x1f8);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(8, 2);
	Actor_SetAttachedEffect(8, 0x102);
	Engine_EventWait(60);
	Object_GetById(8)->paused = 0;
	Event_PlayCue(152);
	record = Actor_Get(8);
	record->velocity_y = 0x80000;
	Engine_ActorSetAnimation(8, 1);
	Engine_EventSetMessage((s32)MsgShianExcellentRobin);
	Event_ShowMessageAndWait(8, 0, 20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
	Event_OpenMessage(8, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(10);
		Engine_ActorSetAnimationAndWait(8, 3);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(8, 0, 20);
		gEventWork->message += 2;
	}
	else
	{
		Engine_EventWait(10);
		Engine_ActorRunRepeatedMotion(8, 2);
		Engine_EventWait(20);
		gEventWork->message++;
		Event_ShowMessageAndWait(8, 0, 20);
		Engine_ActorSetAnimationAndWait(8, 3);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(8, 0, 20);
	}
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
	Event_OpenMessage(8, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		Engine_EventWait(10);
		Actor_ShowEmote(8, 0x102, 60);
		Engine_EventSetMessage((s32)MsgShianCanReadMindsKnowCurious);
		Event_OpenMessage(8, 0);
		do
		{
			if (Engine_EventChooseYesNo(0, 0) != 1)
			{
				break;
			}
			Engine_EventWait(10);
			Actor_ShowEmote(8, 0x102, 60);
			Engine_EventSetMessage((s32)MsgShianDontTryHard);
			Event_OpenMessage(8, 0);
		} while (1);
	}
	Engine_EventSetMessage((s32)MsgShianMonstersWaitInHidingWould);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_OpenMessage(8, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(10);
		Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(8, 0, 20);
		gEventWork->message++;
	}
	else
	{
		Engine_EventWait(10);
		Engine_ActorRunRepeatedMotion(8, 2);
		gEventWork->message++;
		Event_ShowMessageAndWait(8, 0, 20);
	}
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(8, 5);
	GameFlag_Set(0x893);
	Engine_EventEnd();
}

static void FieldScene_RunRoofEnsembleSequence(void)
{
	struct FieldActor *source_record;

	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1d8, 0x218);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Actor_FaceDirection(9, 0, 20);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgShianMasterHamaMeditatingPleaseExtremely);
	Event_ShowMessageAndWait(9, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(30);
	Actor_FaceDirection(8, 0x4000, 30);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(9, 0xd000, 0);
	source_record = Actor_Get(ACTOR_PARTY_LEADER);
	if (source_record != 0)
	{
		Actor_SetPosition(GERALD, source_record->x.fixed, source_record->z.fixed);
	}
	source_record = Actor_Get(ACTOR_PARTY_LEADER);
	if (source_record != 0)
	{
		Actor_SetPosition(IWAN, source_record->x.fixed, source_record->z.fixed);
	}
	source_record = Actor_Get(ACTOR_PARTY_LEADER);
	if (source_record != 0)
	{
		Actor_SetPosition(MEARI, source_record->x.fixed, source_record->z.fixed);
	}
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	Actor_SetSpeed(GERALD, 0x8000, 0x4000);
	Actor_SetSpeed(IWAN, 0x8000, 0x4000);
	Actor_SetSpeed(MEARI, 0x8000, 0x4000);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1d0, 0x1f8);
	Actor_WalkTo(IWAN, 0x1e0, 0x1f8);
	Actor_WalkTo(GERALD, 0x1f0, 0x1f0);
	Actor_WalkTo(MEARI, 0x1c0, 0x1f0);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_ActorWaitForMove(IWAN);
	Engine_ActorWaitForMove(MEARI);
	Actor_FaceDirection(MEARI, 0xe000, 0);
	Engine_ActorWaitForMove(GERALD);
	Actor_FaceDirection(GERALD, 0xa000, 0);
	Engine_EventWait(20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Actor_ShowEmote(IWAN, 0x101, 0);
	Actor_ShowEmote(MEARI, 0x101, 60);
	Actor_FaceDirection(GERALD, 0x5000, 20);
	Event_AskYesNo(GERALD, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_FaceDirection(GERALD, 0xa000, 20);
	Actor_ShowEmote(MEARI, 0x100, 60);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(IWAN, 0x100, 60);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Actor_FaceDirection(8, 0x3000, 20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
	Actor_FaceDirection(GERALD, 0x6000, 0);
	Actor_FaceDirection(MEARI, 0x2000, 0);
	Engine_ActorStartRepeatedMotion(IWAN, 2);
	Actor_SetAttachedEffect(IWAN, 0x102);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xa000, 0);
	Actor_FaceDirection(MEARI, 0xe000, 0);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IWAN, 50);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(IWAN, 0xc000, 30);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(MEARI, 0x101, 60);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Actor_FaceDirection(8, 0x5000, 20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorRunRepeatedMotion(MEARI, 1);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
	Actor_FaceDirection(GERALD, 0x6000, 0);
	Engine_EventWait(30);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xa000, 20);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 4);
	Engine_EventWait(20);
	Actor_FaceActor(8, IWAN, 0);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	Actor_FaceActor(GERALD, IWAN, 0);
	Actor_FaceActor(MEARI, IWAN, 0);
	Actor_ShowEmote(IWAN, 0x102, 60);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_ShowEmote(8, 0x100, 60);
	Actor_FaceActor(8, GERALD, 0);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(IWAN, 0x100, 60);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Actor_FaceActor(8, GERALD, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xa000, 0);
	Actor_FaceDirection(MEARI, 0xe000, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorStartRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorRunRepeatedMotion(MEARI, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Actor_FaceDirection(8, 0x5000, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorStartRepeatedMotion(IWAN, 2);
	Engine_ActorRunRepeatedMotion(MEARI, 2);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(8, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(GERALD, 0x103, 0);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_EventWait(60);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 60);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_ShowEmote(8, 0x102, 0);
	Engine_ActorStartRepeatedMotion(8, 1);
	Engine_EventWait(60);
	Engine_ActorSetAnimationAndWait(8, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	Engine_ActorStartRepeatedMotion(IWAN, 1);
	Engine_ActorRunRepeatedMotion(MEARI, 1);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(8, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(GERALD, 0x101, 60);
	Actor_FaceDirection(GERALD, 0x5000, 20);
	Event_AskYesNo(GERALD, 0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Actor_FaceDirection(GERALD, 0xa000, 20);
	Actor_FaceDirection(8, 0xc000, 20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(8, 0x4000, 20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorStartRepeatedMotion(IWAN, 2);
	Engine_ActorRunRepeatedMotion(MEARI, 2);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(8, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(IWAN, 0x101, 60);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Actor_SetAttachedEffect(IWAN, 0x102);
	Actor_SetAttachedEffect(MEARI, 0x102);
	Engine_EventWait(60);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Actor_FaceDirection(8, 0x5000, 20);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Actor_ShowEmote(IWAN, 0x101, 0);
	Actor_ShowEmote(MEARI, 0x101, 60);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_FaceActor(ACTOR_PARTY_LEADER, GERALD, 0);
	Actor_FaceActor(IWAN, GERALD, 0);
	Actor_FaceActor(MEARI, GERALD, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(IWAN, 0xc000, 0);
	Actor_FaceDirection(MEARI, 0xd000, 0);
	Engine_EventWait(20);
	Event_AskYesNo(8, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(GERALD, 0x102, 0);
	Actor_ShowEmote(IWAN, 0x102, 0);
	Actor_ShowEmote(MEARI, 0x102, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(GERALD, 0x6000, 20);
	Event_AskYesNo(GERALD, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(MEARI, 0x2000, 20);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Actor_FaceDirection(GERALD, 0xa000, 0);
	Actor_FaceDirection(MEARI, 0xe000, 30);
	Actor_FaceDirection(8, 0, 20);
	Actor_ShowEmote(8, 0x105, 60);
	Actor_ShowEmote(IWAN, 0x101, 60);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Actor_FaceDirection(8, 0xc000, 20);
	Actor_ShowEmote(8, 0x105, 60);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Actor_ShowEmote(IWAN, 0x101, 0);
	Actor_ShowEmote(MEARI, 0x101, 60);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Actor_ShowEmote(IWAN, 0x101, 0);
	Actor_ShowEmote(MEARI, 0x101, 60);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(8, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
	Actor_FaceDirection(GERALD, 0x6000, 0);
	Actor_FaceDirection(IWAN, 0x8000, 0);
	Actor_FaceDirection(MEARI, 0x2000, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xa000, 0);
	Actor_FaceDirection(IWAN, 0xc000, 0);
	Actor_FaceDirection(MEARI, 0xe000, 0);
	Engine_EventWait(30);
	Actor_FaceDirection(8, 0x4000, 20);
	Engine_ActorSetAnimationAndWait(8, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_SetPosition(10, 0x1d80000, 0x2600000);
	Event_ShowMessageAndWait(10, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Actor_ShowEmote(IWAN, 0x100, 0);
	Actor_ShowEmote(MEARI, 0x100, 0);
	Actor_ShowEmote(8, 0x100, 0);
	Actor_ShowEmote(9, 0x100, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
	Actor_FaceDirection(GERALD, 0x4000, 0);
	Actor_FaceDirection(IWAN, 0x4000, 0);
	Actor_FaceDirection(MEARI, 0x4000, 0);
	Actor_FaceDirection(9, 0, 0);
	Engine_EventWait(30);
	Actor_SetSpeed(10, 0xcccc, 0x6666);
	Actor_WalkToAndWait(10, 0x1d8, 0x218);
	Actor_ShowEmote(8, 0x101, 60);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_FaceDirection(IWAN, 0xc000, 40);
	Actor_WalkToAndWait(IWAN, 0x1e8, 0x200);
	Engine_EventWait(10);
	Actor_FaceDirection(IWAN, 0x6000, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 30);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1c8, 0x200);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 20);
	Engine_ActorRunRepeatedMotion(10, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(10, 0, 20);
	Actor_ShowEmote(8, 0x101, 60);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorStartRepeatedMotion(10, 2);
	Actor_SetAttachedEffect(10, 0x102);
	Engine_EventWait(60);
	Event_ShowMessageAndWait(10, 0, 20);
	Actor_ShowEmote(8, 0x101, 60);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(10, 0x100, 60);
	Event_ShowMessageAndWait(10, 0, 20);
	Engine_ActorStartRepeatedMotion(8, 1);
	Actor_ShowEmote(8, 0x100, 60);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
	Actor_FaceDirection(GERALD, 0xa000, 0);
	Actor_FaceDirection(IWAN, 0xa000, 0);
	Actor_FaceDirection(MEARI, 0xe000, 0);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(10, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(10, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(8, 0x101, 60);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorStartRepeatedMotion(17, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(10, 0, 20);
	Actor_ShowEmote(8, 0x105, 60);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(8, 0x100, 30);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(2);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_EventWait(1);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_EventWait(5);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_SetSpeed(10, 0xcccc, 0x6666);
	Actor_WalkToAndWait(10, 0x1d8, 0x1f8);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(10, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(10, 0, 20);
	Actor_FaceActor(8, 10, 0);
	Actor_SetSpeed(8, 0xcccc, 0x6666);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
	Actor_FaceDirection(GERALD, 0x6000, 0);
	Actor_FaceDirection(IWAN, 0x6000, 0);
	Actor_FaceDirection(MEARI, 0x2000, 0);
	Engine_ActorWaitForMove(8);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_WalkTo(10, 0x1d8, 0x238);
	Actor_WalkToAndWait(8, 0x1d8, 0x218);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_ActorWaitForMove(10);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Engine_ActorStartRepeatedMotion(8, 2);
	Actor_ShowEmote(8, 0x102, 60);
	Actor_FaceDirection(10, 0xd000, 0);
	Actor_FaceDirection(8, 0xd000, 20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorStartRepeatedMotion(IWAN, 2);
	Engine_ActorRunRepeatedMotion(MEARI, 2);
	Actor_WalkToAndWait(8, 0x1d8, 0x200);
	Actor_FaceDirection(8, 0, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
	Actor_FaceDirection(IWAN, 0x8000, 0);
	Actor_FaceDirection(9, 0xd000, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(20);
	Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1c0, 0x200);
	Engine_EventWait(1);
	{
		struct FieldActor *record = Actor_Get(ACTOR_PARTY_LEADER);
		record->steering_flags = ACTOR_FACES_MOTION | record->steering_flags;
	}
	Engine_EventWait(20);
	FieldScene_RunParticleRain();
	Engine_EventWait(60);
	UiText_DrawQuantityPairWithCue(2, 144);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
	Actor_FaceDirection(GERALD, 0x6000, 0);
	Actor_FaceDirection(IWAN, 0x6000, 0);
	Actor_FaceDirection(MEARI, 0x2000, 0);
	Actor_FaceDirection(9, 0x3000, 0);
	Engine_EventWait(20);
	Actor_WalkToAndWait(8, 0x1d8, 0x228);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(8, 0xd000, 20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_SetSpeed(8, 0x8000, 0x4000);
	Actor_WalkToAndWait(8, 0x1e0, 0x21c);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_SetSpeed(8, 0xcccc, 0x6666);
	Actor_SetSpeed(9, 0xcccc, 0x6666);
	Actor_WalkTo(8, 0x1d8, 0x260);
	Actor_WalkTo(9, 0x1d8, 0x220);
	Actor_WalkToAndWait(10, 0x1d8, 0x260);
	Engine_ActorWaitForMove(9);
	Actor_WalkTo(9, 0x1d8, 0x260);
	Actor_SetPosition(10, 0, 0);
	Engine_ActorWaitForMove(8);
	Actor_SetPosition(8, 0, 0);
	Engine_ActorWaitForMove(9);
	Actor_SetPosition(9, 0, 0);
	Actor_WalkToAndWait(IWAN, 0x1e8, 0x208);
	Actor_ShowEmote(IWAN, 0x101, 60);
	Engine_ActorRunRepeatedMotion(MEARI, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(MEARI, 0x2000, 20);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
	Actor_FaceDirection(IWAN, 0xa000, 20);
	Engine_ActorSetAnimationAndWait(IWAN, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Actor_ShowEmote(MEARI, 0x105, 60);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Actor_FaceDirection(GERALD, 0x6000, 20);
	Event_AskYesNo(GERALD, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(MEARI, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0x8000, 0);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 60);
	Engine_ActorSetAnimationAndWait(MEARI, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(GERALD, 0x102, 60);
	Event_OpenMessage(MEARI, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(GERALD, 3);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(GERALD, 0, 20);
		gEventWork->message++;
	}
	else
	{
		Engine_EventWait(20);
		gEventWork->message++;
		Engine_ActorSetAnimationAndWait(MEARI, 3);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(MEARI, 0, 20);
	}
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	Actor_WalkTo(GERALD, 0x1c0, 0x200);
	Actor_WalkToAndWait(MEARI, 0x1c0, 0x200);
	Actor_SetPosition(MEARI, 0, 0);
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	Engine_EventWait(20);
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 20);
	Engine_EventWait(30);
	Actor_FaceActor(IWAN, ACTOR_PARTY_LEADER, 20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
	Engine_ActorStartRepeatedMotion(IWAN, 2);
	Actor_SetAttachedEffect(IWAN, 0x102);
	Engine_EventWait(60);
	Engine_ActorSetAnimationAndWait(IWAN, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Actor_WalkToAndWait(IWAN, 0x1c0, 0x200);
	Actor_SetPosition(IWAN, 0, 0);
	Engine_EventEnd();
	GameFlag_Set(0x895);
}

s32 ShianJiin_ApplyEntryState(void)
{
	struct FieldActor *actor;
	s32				   set;

	if (gGameState.scene == (s32)&SceneId_ShianJiin2)
	{
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
		if (gGameState.entrance == 1)
		{
			if (Engine_GameFlagIsSet(0x88f))
			{
				Engine_ActorSetAnimation(8, 6);
			}
			else
			{
				Engine_ActorSetAnimation(8, 5);
				if (Engine_GameFlagIsSet(0xf14) && !Engine_GameFlagIsSet(0x893)
					&& !Engine_GameFlagIsSet(0x109))
				{
					ShianJiin_AskIfCurious();
				}
			}
		}
		else if (gGameState.entrance == 2 || gGameState.entrance == 4)
		{
			Engine_GameFlagClear(0x12f);
			set = Engine_GameFlagIsSet(0x895);
			if (set == 0)
			{
				actor = Object_GetById(19);
				actor->motion_flags = set;
				actor->y.fixed = 0xc0000;
				actor->target_y = 0xc0000;
				actor->scale_x = 0xcccc;
				actor->scale_y = 0x8000;
				actor->sprite->rotation = 0x8000;
				if (Engine_GameFlagIsSet(0x89a))
				{
					Engine_ActorSetPosition(18, 0xf80000, 0xd00000);
					if (!Engine_GameFlagIsSet(0x89b))
					{
						Engine_ActorSetPosition(16, 0x1000000, 0xf00000);
						Object_GetById(18)->update = FaceXianActorToPlayer;
						Object_GetById(13)->update = FaceXianActorToPlayer;
						Object_GetById(14)->update = FaceXianActorToPlayer;
						Object_GetById(15)->update = FaceXianActorToPlayer;
						Object_GetById(16)->update = FaceXianActorToPlayer;
					}
				}
			}
			else
			{
				actor = Object_GetById(19);
				actor->motion_flags = 0;
				actor->y.fixed = 0xc0000;
				actor->target_y = 0xc0000;
				actor->scale_x = 0xcccc;
				actor->scale_y = 0x8000;
				actor->collision_flags |= 8;
				actor->sprite->rotation = 0x8000;
				Map_CopyCellAttributes(14, 11, 1, 1, 14, 10);
			}
			OverlayObject_PrepareObject(0x1300000, 0x180000, 0xe00000, 223);
			Engine_ActorSetAnimation(10, 5);
			Engine_ActorSetAnimation(11, 5);
		}
		else if (gGameState.entrance == 3)
		{
			Engine_GameFlagClear(0x12f);
			if (!Engine_GameFlagIsSet(0x895))
			{
				FieldScene_RunRoofEnsembleSequence();
			}
			else if (!Engine_GameFlagIsSet(0x8b2))
			{
				Engine_ActorSetPosition(8, 0, 0);
				Engine_ActorSetPosition(9, 0, 0);
			}
		}
	}
	else
	{
		BattleFx_SetQueuedSoundAndPlay(170);
		Object_GetById(9)->collision_flags |= 16;
		if (gGameState.entrance == 3 && Engine_GameFlagIsSet(0xf14) && !Engine_GameFlagIsSet(0x894))
		{
			Map_CopyCellAttributes(10, 84, 1, 1, 10, 24);
		}
		if (Engine_GameFlagIsSet(0x892))
		{
			Actor_SetPosition(9, 0x980000, 0x1880000);
			Engine_ActorFaceDirection(9, 0, 0);
			Map_CopyCellAttributes(10, 26, 1, 1, 10, 22);
		}
	}
	return 0;
}

static void FieldScene_SetFlag140AndFinishSequence(s32 arg0, s32 arg1)
{
	u8 *globalCtx;

	Engine_GameFlagSet(160 << 1);
	Engine_PsynergyBegin(141, 1);
	globalCtx = gEffectWork;
	Engine_PsynergySetTarget(arg0, arg1);
	globalCtx[0x23] = 0;
	Engine_PsynergyRaiseHands();
	Engine_PsynergyPlayEffect(1);
	Engine_TaskWait(1);
}

static void FieldScene_FinishSequence(void)
{
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_PsynergyPlayEffect(2);
	Engine_PsynergyLowerHands();
}

static void FieldScene_SpawnEightShots(void)
{
	struct EffectOptions descriptor;
	struct FieldActor	*record;
	u32					 i;

	record = Object_GetById(8);
	descriptor.priority = 1;
	descriptor.type = 0x0119;
	descriptor.script = ShianJiin_ShotScript;
	descriptor.target_scale_x = 224 << 10;
	descriptor.target_scale_y = 192 << 9;
	for (i = 0; i <= 7; i++)
	{
		Engine_EventWait(10);
		if (i & 1)
		{
			Engine_AudioPlayCue(0x82);
		}
		Effect_Spawn(record->x.fixed, record->y.fixed, record->z.fixed + 0xffe80000, 0, 0x9999, 0,
			0x00360001, &descriptor);
	}
	Engine_EventWait(60);
}

static void FieldScene_SelectActorModeFromInputBit(s32 arg0)
{
	if ((*(u32 *)&gFrameCount >> 1) & 1)
	{
		ObjectGroup_SetChildValue(arg0, 10);
	}
	else
	{
		ObjectGroup_SetChildValue(arg0, 9);
	}
}

static void FieldScene_RunParticleRain(void)
{
	struct EffectOptions descriptor;
	struct FieldActor	*actor;
	u32					 i;
	s32					 x;
	s32					 y;
	s32					 scale;

	Engine_AudioPlayCue(0x83);
	Object_GetById(8)->update =
		(void (*)(union FieldObject *))FieldScene_SelectActorModeFromInputBit;
	Engine_EventWait(40);
	Engine_ColorBufferApplySource(128 << 9, 0);
	Engine_ColorBufferApplyTarget(0x205c54, 1);
	Engine_ColorBufferInterpolate(60);
	Engine_EventWait(40);
	Engine_AudioPlayCue(0x83);
	Object_GetById(2)->update =
		(void (*)(union FieldObject *))FieldScene_SelectActorModeFromInputBit;
	Engine_EventWait(120);
	actor = Object_GetById(8);
	descriptor.priority = 1;
	descriptor.palette = 2;
	descriptor.type = 0x011d;
	for (i = 0; i <= 63; i++)
	{
		if ((i & 3) == 0)
		{
			Engine_AudioPlayCue(246);
		}
		x = actor->x.fixed + ((((u32)(Engine_RandomNext() * 3) << 4) >> 16) << 16) + 0xfff40000;
		y = actor->y.fixed + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + 0xfff00000;
		scale = ((((u32)Engine_RandomNext() << 2) >> 16) << 15) + (128 << 8);
		Effect_Spawn(x, y, actor->z.fixed, 0, scale, 0, 152 << 13, &descriptor);
		Engine_TaskWait(2);
	}
	Engine_AudioPlayCue(220);
	Engine_EventWait(30);
	Engine_ColorBufferApplyTarget(128 << 9, 1);
	Engine_ColorBufferInterpolate(60);
	Engine_EventWait(40);
	Object_GetById(8)->update = NULL;
	Object_GetById(2)->update = NULL;
	Engine_ActorSetChildValue(8, 0);
	Engine_ActorSetChildValue(IWAN, 0);
}

s32 gEffectScript0[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript1[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript2[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

const s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

s32 ShianJiin_ActorTwelveScript[] = {
	SC_ARG(0),
	SC_SET(SW_GRAVITY, 0x9999),
	SC_LABEL(1),
	SC_SET(SW_VELOCITY_Y, 0x30000),
	SC_WAIT(30),
	SC_SET(SW_VELOCITY_Y, 0x40000),
	SC_WAIT(30),
	SC_SET(SW_VELOCITY_Y, 0x50000),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(2),
	SC_SET(SW_FACING, 0x5000),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(2),
	SC_SET(SW_FACING, 0xb000),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(2),
	SC_SET(SW_FACING, 0xd000),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(2),
	SC_SET(SW_FACING, 0x3000),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(2),
	SC_WAIT(60),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

s32 gShianJiinPlacement1[] = {
	SC_CALL(StopXianActor),
	SC_WAIT(1),
	SC_END,
};

u16 ShianJiin_CellStepsA[] = {
	MAP_CELL_STEP(66, 46, 3, 2, 5), MAP_CELL_STEP(63, 46, 3, 2, 5), MAP_CELL_STEPS_END,
};

u16 ShianJiin_CellStepsB[] = {
	MAP_CELL_STEP(62, 19, 2, 2, 5), MAP_CELL_STEP(64, 19, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 ShianJiin_CellStepsC[] = {
	MAP_CELL_STEP(62, 19, 2, 2, 5), MAP_CELL_STEP(60, 19, 2, 2, 5), MAP_CELL_STEPS_END,
};

struct SceneEntrance gShianJiinEntrances1[] = {
	{ 0, CONDITION_ALWAYS, 296, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 296, 0, 504, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 296, 0, 304, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 168, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 232, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gShianJiinEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 168, 0, 552, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 168, 0, 552, FACING_NORTH, 0, 48, 384, 288, 576, 0 },
	{ 2, CONDITION_ALWAYS, 256, 0, 264, FACING_NORTH, 0, 48, 80, 368, 288, 0 },
	{ 3, CONDITION_ALWAYS, 472, 0, 552, FACING_NORTH, 0, 352, 384, 592, 576, 0 },
	{ 4, CONDITION_ALWAYS, 304, 0, 248, FACING_SOUTH, 0, 48, 80, 368, 288, 0 },
	{ SCENE_TABLE_END },
};

u32 ShianJiin_XianMessages[] = {
	SCENE_EXITS(60),
	SCENE_EXIT(1, 2, 12),
	SCENE_EXIT(2, 61, 1),
	SCENE_EXIT(3, 62, 1),
	SCENE_EXIT(0, 61, 0),
	SCENE_EXIT(1, 60, 2),
	SCENE_EXIT(2, 72, 10),
	SCENE_EXIT(3, 88, 2),
	SCENE_EXIT(4, 73, 8),
	SCENE_EXITS_END,
};

struct ScenePlacement gShianJiinPlacements1[] = {
	{ 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(296), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x95, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x97, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(272), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x97, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(392), 0, PIXELS(392), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x97, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x79, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gShianJiinPlacements[] = {
	{ 0x33, 0x0, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(472), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x6c, 0x0, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(264), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x82, 0x0, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(200), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x83, 0x0, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(152), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x83, 0x0, (s32)ShianJiin_ActorTwelveScript,
		PIXELS(120), 0, PIXELS(248), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x83, 0x0, ACTOR_STAND,
		PIXELS(176), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x66, 0x0, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(264), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x82, 0x0, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(152), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x82, 0x0, ACTOR_STAND,
		PIXELS(176), 0, PIXELS(216), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x27, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x28, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(232), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x16, CONDITION_ALWAYS, (s32)gShianJiinPlacement1,
		PIXELS(232), 0, PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gShianJiinPlacementsEntrance3[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x3d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(472), 0, PIXELS(472), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x97, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(440), 0, PIXELS(536), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x27, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gShianJiinEvents1[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH_1) },
	{ EVENT_TOUCH, 5, 0x894, EVENT_SCRIPT(EV05_TOUCH) },
	{ EVENT_TALK, 8, IF_FLAG_SET(0x88f), MSGNO(MsgShianJiinTheFuchinTempleIsAPleasant) },
	{ EVENT_TALK, 9, IF_FLAG_SET(0x88f), MSGNO(MsgShianJiinOhRobinWhatTookYouSo) },
	{ EVENT_TALK, 10, IF_FLAG_SET(0x88f), MSGNO(MsgShianJiinAhhhMasterNyunpaHeHasPassed) },
	{ EVENT_TALK, 11, IF_FLAG_SET(0x88f), MSGNO(MsgShianJiinMasterNyunpaHasWastedAwayAt) },
	{ EVENT_TALK, 12, IF_FLAG_SET(0x88f), EVENT_SCRIPT(EV12_TALK_1) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
	{ EVENT_TALK, 9, 0x891, MSGNO(MsgShianJiinOmOmBlueLightsInThe) },
	{ EVENT_TALK, 9, 0x892, EVENT_SCRIPT(EV09_TALK_1) },
	{ EVENT_TALK, 9, 0xf14, MSGNO(MsgShianJiinTheTestOfFuchinFallsIs) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShianJiinMasterNyunpaIsWaitingForYou) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShianJiinGemsFromTheSkyBlueLights) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShianJiinNyunpaHasNotEatenInDays) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK_1) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ SEARCH(SEARCH_UNNAMED), 110, 0x350, FIND_PSYNERGY_STONE },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(0x88f), MSGNO(MsgShianJiinGemsFallingFromTheSkyMountains) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(0x88f), MSGNO(MsgShianJiinIKnewMasterNyunpaWasFasting) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x88f), MSGNO(MsgShianJiinMasterNyunpaReachedEnlightenmentDuringHis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(0x88f), MSGNO(MsgShianJiinMasterNyunpaIsNothingButSkin) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, IF_FLAG_SET(0x88f), MSGNO(MsgShianJiinHeCantLeaveUsYetHe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x890, MSGNO(MsgShianJiinIBeginningToThinkIllBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgShianJiinIDontFeelSafeGoingHome) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x891, MSGNO(MsgShianJiinManyStarsFellFromTheSky) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x892, MSGNO(MsgShianJiinCanSuchYoungWarriorsEndureThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0xf14, MSGNO(MsgShianJiinCanSuchYoungWarriorsEndureThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgShianJiinTheWarriorsHaveMasteredTheSecrets) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgShianJiinThereIsNoPointInTraining) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgShianJiinICantImagineFastingItGets) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgShianJiinIWillTrainToBecomeA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgShianJiinSinceImHereIdLikeTo) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gShianJiinEvents[] = {
	{ 0x4402, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_SCRIPT) },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH) },
	{ EVENT_TALK, 8, IF_FLAG_SET(0x88f), MSGNO(MsgShianJiin) },
	{ EVENT_TALK, 8, IF_FLAG_SET(0x893), MSGNO(MsgShianJiinUseThisEnergyToFindThe) },
	{ EVENT_TALK, 8, IF_FLAG_SET(0x891), MSGNO(MsgShianJiinSpeakToTheMonkStandingBeneath) },
	{ EVENT_TALK, 8, IF_FLAG_SET(0x300), EVENT_SCRIPT(Scene_RunScene39eSequenceB) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgShianNyunpaMeditates) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(0x88f), EVENT_SCRIPT(EV08_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(0x893), MSGNO(MsgShianJiinNowThatIHavePassedMy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, IF_FLAG_SET(0x891), EVENT_SCRIPT(EV1032_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunScene39eSequenceB) },
	{ EVENT_TALK, 9, IF_FLAG_SET(0x895), MSGNO(MsgShianJiinThisIsMasterFehsKungFu) },
	{ EVENT_TALK, 10, IF_FLAG_SET(0x895), MSGNO(MsgShianJiinBouldersOvercameHsuKungFuShould) },
	{ EVENT_TALK, 11, IF_FLAG_SET(0x895), EVENT_SCRIPT(EV11_TALK) },
	{ EVENT_TALK, 12, IF_FLAG_SET(0x895), EVENT_SCRIPT(EV12_TALK) },
	{ EVENT_TALK, 14, IF_FLAG_SET(0x895), MSGNO(MsgShianJiinFeizhiPredictedHsusDangerHerProphecy) },
	{ EVENT_TALK, 16, IF_FLAG_SET(0x895), MSGNO(MsgShianJiinHsuIsRestingFeizhiIsWatching) },
	{ EVENT_TALK, 18, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinPracticeEveryoneChiHasItsLimits) },
	{ EVENT_TALK, 9, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinWarriorYouAreGreatYouHave) },
	{ EVENT_TALK, 10, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinEveryoneWantsToLearnTheChi) },
	{ EVENT_TALK, 11, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinConcentrateEnergyIntoTheHandsAnd) },
	{ EVENT_TALK, 12, IF_FLAG_SET(0x89b), EVENT_SCRIPT(EV12_TALK) },
	{ EVENT_TALK, 13, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinIDidNotThinkMuchOf) },
	{ EVENT_TALK, 14, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinWhereDidFeizhiGoIAm) },
	{ EVENT_TALK, 15, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinMasterHsuIsLateICan) },
	{ EVENT_TALK, 16, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinIWillLearnChiIWill) },
	{ EVENT_TALK, 18, IF_FLAG_SET(0x301), MSGNO(MsgShianJiinWarriorUseYourChiFromThe) },
	{ EVENT_TALK, 16, IF_FLAG_SET(0x301), MSGNO(MsgShianJiinWarriorPleaseUseYourChiFrom) },
	{ EVENT_TALK, 18, IF_FLAG_SET(0x898), MSGNO(MsgShianJiinWarriorLetMeSeeYourChi) },
	{ EVENT_TALK, 13, IF_FLAG_SET(0x898), MSGNO(MsgShianJiinFeizhiIsGoneItIsDangerous) },
	{ EVENT_TALK, 14, IF_FLAG_SET(0x898), MSGNO(MsgShianJiinFeizhiIsStrongerThanSheLooks) },
	{ EVENT_TALK, 15, IF_FLAG_SET(0x898), MSGNO(MsgShianJiinWarriorShowUsYourChiLike) },
	{ EVENT_TALK, 16, IF_FLAG_SET(0x898), MSGNO(MsgShianJiinWarriorThatWasGreatButMaster) },
	{ EVENT_TALK, 18, IF_FLAG_SET(0x899), EVENT_SCRIPT(EV18_TALK) },
	{ EVENT_TALK, 13, IF_FLAG_SET(0x899), MSGNO(MsgShianJiinFeizhiIsGoneItIsDangerousOutside) },
	{ EVENT_TALK, 14, IF_FLAG_SET(0x899), MSGNO(MsgShianJiinFeizhiIsStrongerThanSheLooksShe) },
	{ EVENT_TALK, 15, IF_FLAG_SET(0x899), MSGNO(MsgShianJiinIKnowWhyYouCanUse) },
	{ EVENT_TALK, 16, IF_FLAG_SET(0x899), MSGNO(MsgShianJiinWarriorChiIsAGreatPower) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShianJiinStrongKicksAreVeryImportantKick) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShianJiinIWantToLearnSoI) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgShianJiinEveryoneInXianCanDoMartial) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgShianJiinJustListeningDoesNotMakeOne) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgShianJiinMasterFehIsVeryBusyHe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(0x895), MSGNO(MsgShianJiinPeaceHasReturnedToXianBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x895), MSGNO(MsgShianJiinNoMatterHowMuchYouPractice) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(0x895), MSGNO(MsgShianJiinTheMonstersAreVeryLargeStriking) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, IF_FLAG_SET(0x895), MSGNO(MsgShianJiinWhenABoulderFallsJumpRepeat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0x895), MSGNO(MsgShianJiinWhereDidFeizhiGainThePower) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x895), MSGNO(MsgShianJiinMasterFehIsVeryWorriedAbout) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinCanSuchAYoungWarriorReally) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinYourChiIsFuchinTemplesHow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinIDontCareWhichChiIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinMasterHamasChiCanShatterBoulders) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinFollowSilkRoadStraightPassThrough) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinWarriorYouUseWeaponsAndChi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinFeizhiIsNotInTheFeh) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinFeizhiWorriesOnlyAboutMasterHsu) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x89b), MSGNO(MsgShianJiinLearningChiIsVeryHardMaster) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(0x301), MSGNO(MsgShianJiinWarriorTheRulesRequireYouTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x301), MSGNO(MsgShianJiinSoCloseWarriorUseChiFrom) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(0x898), MSGNO(MsgShianJiinWarriorShowMeYourChiI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(0x898), MSGNO(MsgShianJiinFeizhiIKnowIKnowHow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0x898), MSGNO(MsgShianJiinMyKickAndPunchAreVery) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(0x898), MSGNO(MsgShianJiinShowUsYourChiNowDo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x898), MSGNO(MsgShianJiinIAmShockedThatThisWarrior) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(0x899), MSGNO(MsgShianJiinIWantToSeeTheChi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(0x899), MSGNO(MsgShianJiinFeizhiIKnowHowYouFeel) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0x899), MSGNO(MsgShianJiinMyKickAndPunchAreVeryLoud) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(0x899), MSGNO(MsgShianJiinWarriorShowMeYourChiNow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x899), MSGNO(MsgShianJiinWarriorDoNotHideYourChi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgShianJiinWarriorsFightWithWeaponsKungFu) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgShianJiinPracticingKicksHurtsMyFeetVery) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgShianJiinThatPunchWasGoodYouSurprise) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgShianJiinIMoveTooQuicklyYouCant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgShianJiinWarriorsCameToLearnTheFeh) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgShianJiinGreatKickKickFromTheHip) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgShianJiinYouAreTheWarriorsWhoCame) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgShianJiinMasterFehIsTalkingWithFeizhi) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_DispatchByFacing) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_DispatchByFacing) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_DispatchByFacing) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_DispatchByFacing) },
	{ PSYNERGY_ON_ACTOR(0x8c), 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8e), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_PSYNERGY) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf69, FIND_ITEM(ITEM_ANTIDOTE) },
	{ SEARCH(SEARCH_JAR), 101, 0xf68, FIND_ITEM(0x10a) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gShianJiinEventsEntrance3[] = {
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 3 },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShianJiinMasterHamaIsMeditatingAskingFor) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgShianJiinTheFutureHoldsManyHardshipsBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgShianJiinIDoNotThinkMasterHama) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgShianJiinICannotSeeYourFutureClearly) },
	{ SEARCH(SEARCH_JAR), 102, 0xf6a, FIND_COINS(6) },
	{ SCENE_EVENTS_END },
};

s32 ShianJiin_ShotScript[] = {
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(44),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};
