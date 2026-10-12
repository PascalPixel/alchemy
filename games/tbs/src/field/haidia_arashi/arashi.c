#include "game_state.h"
#include "unit/import.h"
#include "unit/field/haidia_arashi/arashi.h"
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
#include "callback_scheduler.h"
#include "field_event.h"
#include "field_scene.h"
#include "call.h"
#include "field_service.h"
#include "fieldobj.h"
#include "animspr.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "field_effect.h"
#include "tbs_edition.h"
#include "b5_context.h"
#include "objcmd.h"
#include "global_cells.h"
#include "vram_tab.h"

// 前方宣言
union PairObject;
static void Scene_DoraSendsRobinToThePlaza(void);
static void HaidiaArashi_SetStormCellAttributes(void);
static void ActorPresentation_SetEightSceneCells(void);
static void SceneState_ApplyFourRects(void);
static void FieldScene_DrawFiveTileBlocks(void);
static void FieldScene_RunScene372SequenceA(void);
static void HaidiaArashi_RunCallOutSequence(void);
static void SceneActor_RunActor22PlacementSequence(s32 x, s32 y);
static void HaidiaArashi_RunRiverSearch(void);
static void Scene_RunActorGroupDepartureSequence(void);
static void HaidiaArashi_FlashLightning(void);
static void HaidiaArashi_UpdatePulsingGlow(void);
static void ActorPresentation_SelectActorTwentySevenState(void);
static void FieldScene_ConfigureActorTwentyTwoScene(void);
static void FieldScene_BuildPlacementGrid(void);
static void SceneActor_SetModeByFrameBit1(s32 o);
static void HaidiaArashi_SpawnEffectPair(union PairObject *parent);
static void SceneState_SetValue140Mode0(void);
static void FieldScene_RunSingleStep(void);
static void FieldScene_RunFourPairedSteps(void);
static void SceneState_SetValue19ThenCall(void);
static void HaidiaArashi_CopyLeaderSpritePriority(void);

enum
{
	FLAG_BOULDER_FELL = 0x838
};

enum StormNightActor
{
	ACTOR_DORA = ACTOR_FIRST_PLACED + 1,
	ACTOR_KYLE
};

#define ACTOR_ID 22

s32 Math_RemainderUnsigned(s32, s32);

union PairObject;

s32 HaidiaArashi_ShakeDone;
s32 HaidiaArashi_ShakeShift;

static s32 HaidiaArashi_RerollHeadingOnCountdown(struct FieldActor *object)
{
	s32 loaded = object->timer;
	s32 counter = (s16)loaded;

	if (loaded == 0)
	{
		object->facing = Engine_RandomNext();
		counter = Math_RemainderUnsigned(Engine_RandomNext(), 20) + 20;
		object->timer = counter;
	}
	object->timer = counter - 1;
	return 1;
}

static s32 UpdateFixedPointCountdown(struct FieldActor *state)
{
	switch (state->timer)
	{
	case 6:
		state->scale_x -= 0x4000;
		state->scale_y += 0x2000;
		break;
	case 4:
		state->scale_x += 0x2000;
		state->scale_y += -0x1000;
		break;
	case 2:
		state->scale_x += 0x1000;
		state->scale_y -= 0x800;
		break;
	case 0:
		state->scale_x = 0x10000;
		state->scale_y = 0x10000;
		state->timer = (Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60);
		break;
	}
	state->timer--;
	return 1;
}

u8 *HaidiaArashi_GetEntrances(void)
{
	return HaidiaArashi_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *HaidiaArashi_GetExits(void)
{
	return HaidiaArashi_SceneTable1;
}

u8 *HaidiaArashi_GetPlacements(void)
{
	return HaidiaArashi_SceneTable2;
}

u8 *HaidiaArashi_GetEvents(void)
{
	return HaidiaArashi_SceneTable3;
}

static void EV15_TOUCH(void)
{
	s32 a;
	s32 b;

	Engine_GameFlagSet(0x210);
	a = 10;
	b = 84;
	Engine_MapCopyCellAttributes(40, 84, 7, 4, a, b);
}

static void EV14_TOUCH(void)
{

	s32 a;
	s32 b;

	Engine_GameFlagClear(0x210);
	a = 10;
	b = 84;
	Engine_MapCopyCellAttributes(40, 89, 7, 4, a, b);
}

static void HaidiaArashi_RequestBackdropExit(u8 *o)
{

	struct EventWork *state;

	if (Engine_GameFlagIsSet(0x834) != 0)
	{
		BattleFx_SetBlock30ValuesMaxZero();
	}
	state = gEventWork;
	state->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->transition_frames = 16;
	Engine_EventRequestExit(o);
}

static void HaidiaArashi_OpenBackdropExit11(void)
{
	Engine_AudioPlayCue(0x9E);
	Engine_MapAnimateCells(HaidiaArashi_CellSteps0, 45, 11);
	Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x101, 0x1A4);
	Engine_EventWait(3);
	HaidiaArashi_RequestBackdropExit(11);
}

static void EV02_TOUCH(void)
{

	Engine_AudioPlayCue(0x7B);
	HaidiaArashi_RequestBackdropExit(1);
}

static void EV03_TOUCH(void)
{

	Engine_AudioPlayCue(0x7B);
	HaidiaArashi_RequestBackdropExit(3);
}

static void EV04_TOUCH(void)
{

	Engine_AudioPlayCue(0x7B);
	HaidiaArashi_RequestBackdropExit(4);
}

static void EV05_TOUCH(void)
{
	Engine_AudioPlayCue(0x7B);
	if (Engine_GameFlagIsSet(0x841) != 0 && Engine_GameFlagIsSet(0x842) == 0)
	{
		FieldScene_ConfigureActorTwentyTwoScene();
	}
	HaidiaArashi_RequestBackdropExit(2);
}

static void EV06_TOUCH(void)
{
	Engine_AudioPlayCue(0x9E);
	Engine_MapAnimateCells(HaidiaArashi_CellSteps1, 54, 32);
	Engine_ActorWalkTo(0, 0x196, 0x2d7);
	Engine_EventWait(3);
	HaidiaArashi_RequestBackdropExit(5);
}

static void EV07_TOUCH(void)
{
	s32 record;

	if (GameFlag_IsSet(0x206) == 0)
	{
		Event_PlayCue(158);
		Map_AnimateCells(HaidiaArashi_CellSteps2, 45, 39);
	}
	if (GameFlag_IsSet(0x835) == 0)
	{
		record = GameFlag_IsSet(0x831);
		if (record != 0)
		{
			goto L_020002b4;
		}
		FieldScene_RunScene372SequenceA();
		GameFlag_Set(0x206);
	}
	else
	{
	L_020002b4:;
		Actor_WalkTo(ACTOR_PARTY_LEADER, 0x106, 0x325);
		Engine_EventWait(3);
		HaidiaArashi_RequestBackdropExit(6);
	}
}

static void EV08_TOUCH(void)
{
	if (Engine_GameFlagIsSet(0x205) == 0)
	{
		Engine_AudioPlayCue(0x9E);
		Engine_MapAnimateCells(HaidiaArashi_CellSteps1, 50, 44);
	}
	Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x154, 0x378);
	Engine_EventWait(3);
	HaidiaArashi_RequestBackdropExit(7);
}

static void EV09_TOUCH(void)
{
	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(HaidiaArashi_CellSteps2, 49, 69);
	Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x146, 0x466);
	Engine_EventWait(3);
	HaidiaArashi_RequestBackdropExit(8);
}

static void EV10_TOUCH(void)
{
	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(HaidiaArashi_CellSteps3, 52, 76);
	Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x176, 0x4d6);
	Engine_EventWait(3);
	HaidiaArashi_RequestBackdropExit(9);
}

static void EV11_TOUCH(void)
{
	Event_PlayCue(158);
	Map_AnimateCells(HaidiaArashi_CellSteps1, 35, 74);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x4b6);
	Engine_EventWait(3);
	HaidiaArashi_RequestBackdropExit(10);
}

static void EV12_TOUCH(void)
{
	Event_PlayCue(158);
	Map_AnimateCells(HaidiaArashi_CellSteps1, 35, 73);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x4b6);
	Engine_EventWait(3);
	HaidiaArashi_RequestBackdropExit(12);
}

static void EV13_TOUCH(void)
{
	Event_PlayCue(158);
	Map_AnimateCells(HaidiaArashi_CellSteps2, 38, 72);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 146, 0x49e);
	Engine_EventWait(3);
	HaidiaArashi_RequestBackdropExit(13);
}

s32 FieldScene_RunFlagGatedActorSetup(void)
{

	BattleFx_SetQueuedSoundAndPlay(0xAA);
	Engine_ActorSetPosition(23, 0, 0);
	if (Engine_GameFlagIsSet(0x109) != 0)
	{
		Engine_GameFlagClear(0x205);
		Engine_GameFlagClear(0x206);
	}
	if (Engine_GameFlagIsSet(0x830) != 0)
	{
		Actor_SetPosition(11, 0x14E0000, 0x3A40000);
		HaidiaArashi_SetStormCellAttributes();
	}
	if (Engine_GameFlagIsSet(0x831) != 0)
	{
		Actor_SetPosition(12, 0xE00000, 0x3680000);
		ActorPresentation_SetEightSceneCells();
	}
	if (Engine_GameFlagIsSet(0x832) != 0)
	{
		Actor_SetPosition(13, 0x400000, 0x2BF0000);
		SceneState_ApplyFourRects();
	}
	if (Engine_GameFlagIsSet(0x833) != 0)
	{
		Actor_SetPosition(14, 0x1B00000, 0x47B0000);
		FieldScene_DrawFiveTileBlocks();
	}
	Object_GetById(11)->collision_flags |= 4;
	Object_GetById(12)->collision_flags |= 4;
	Object_GetById(13)->collision_flags |= 4;
	Object_GetById(14)->collision_flags |= 4;
	Object_GetById(15)->collision_flags |= 4;
	Object_GetById(16)->collision_flags |= 4;
	Object_GetById(17)->collision_flags |= 4;
	Object_GetById(18)->collision_flags |= 4;
	if (Engine_GameFlagIsSet(0x837) != 0)
	{
		Engine_ActorSetPosition(22, 0, 0);
	}
	{
		struct FieldActor *actor;

		actor = Object_GetById(19);
		actor->scale_x = 0x20000;
		actor->scale_y = 0x20000;
	}
	if (Engine_GameFlagIsSet(FLAG_BOULDER_FELL) != 0)
	{
		Actor_SetPosition(19, 0x720000, 0x14D0000);
	}
	else
	{
		Engine_ActorEnableActionCallback(19, HaidiaArashi_ActorNineteenScript);
	}
	if (Engine_GameFlagIsSet(0x841) != 0)
	{
		s32 h2;
		u8 *tbl;
		FieldScene_BuildPlacementGrid();
		Actor_SetPosition(9, 0xA50000, 0x4CD0000);
		{
			struct FieldActor *actor;

			actor = Object_GetById(9);
			h2 = 0xE000;
			actor->facing = h2;
			actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
			tbl = HaidiaArashi_ActorEightScript;
			actor->work = 1;
			Engine_ActorEnableActionCallback(9, tbl);
		}
		Engine_ActorSetPosition(26, 0xA50000, 0x4E60000);
		{
			struct FieldActor *actor;

			actor = Object_GetById(26);
			actor->facing = h2;
			actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
			actor->work = 2;
			Engine_ActorEnableActionCallback(26, tbl);
		}
		Engine_ActorSetPosition(22, 0x980000, 0x5050000);
		{
			struct FieldActor *actor;

			actor = Object_GetById(22);
			actor->facing = h2;
			actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
			actor->work = 3;
			Engine_ActorEnableActionCallback(22, tbl);
		}
		Engine_ActorSetPosition(8, 0xB80000, 0x5180000);
		{
			struct FieldActor *actor;

			actor = Object_GetById(8);
			actor->facing = h2;
			actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
			actor->work = 4;
			Engine_ActorEnableActionCallback(8, tbl);
		}
		Engine_ActorSetAnimation(8, 6);
		Object_GetById(22)->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
		Object_GetById(8)->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
		Engine_TaskAddCallback(HaidiaArashi_CopyLeaderSpritePriority, 0xC80);
		Engine_ActorSetPosition(24, 0, 0);
		Engine_ActorSetPosition(25, 0, 0);
		Engine_ActorSetPosition(23, 0, 0);
		Engine_ActorSetPosition(19, 0, 0);
		if (Engine_GameFlagIsSet(0x842) != 0)
		{
			Engine_ActorSetPosition(22, 0, 0);
		}
	}
	else if (Engine_GameFlagIsSet(0x83a) != 0)
	{
		u8 *tbl;
		Actor_SetPosition(10, 0xC00000, 0x4BE0000);
		Actor_FaceDirection(10, 0x2000, 0);
		Engine_ActorSetAnimation(10, 5);
		{
			struct FieldActor *actor;

			actor = Object_GetById(10);
			actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
		}
		tbl = HaidiaArashi_ActorEightScript;
		Engine_ActorEnableActionCallback(10, tbl);
		Actor_SetPosition(24, 0xE30000, 0x4BE0000);
		Actor_FaceDirection(24, 0x4000, 0);
		Engine_ActorSetAnimation(24, 6);
		{
			struct FieldActor *actor;

			actor = Object_GetById(24);
			actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
		}
		Engine_ActorEnableActionCallback(24, tbl);
		Actor_SetPosition(25, 0xF70000, 0x4BE0000);
		Actor_FaceDirection(25, 0x4000, 0);
		Engine_ActorSetAnimation(25, 6);
		{
			struct FieldActor *actor;

			actor = Object_GetById(25);
			actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
		}
		Engine_ActorEnableActionCallback(25, tbl);
		Actor_SetPosition(23, 0xF30000, 0x4FD0000);
		Engine_ActorFaceDirection(23, FACING_NORTH, 0);
		Engine_ActorSetSpriteFlags(Object_GetById(23), 0);
		Engine_ActorSetPosition(17, 0, 0);
		Engine_ActorSetPosition(18, 0, 0);
	}
	else
	{
		Engine_ActorSetPosition(17, 0, 0);
		Engine_ActorSetPosition(18, 0, 0);
	}
	if (gGameState.entrance != 15 || Engine_GameFlagIsSet(0x87b) != 0)
	{
		Effect_SoundAndFlash();
		BattleFx_StartTwelveFrameBlend();
	}
	if (Engine_GameFlagIsSet(0x210) != 0)
	{
		EV15_TOUCH();
	}
	Engine_GameFlagSet(0x834);
	Engine_MapCopyCellAttributes(29, 24, 1, 2, 26, 46);
	Engine_MapCopyCellAttributes(29, 25, 1, 1, 27, 46);
	Engine_MapCopyCellAttributes(29, 25, 1, 1, 28, 46);
	Engine_MapCopyCellAttributes(19, 0x5A, 1, 1, 20, 0x58);
	Engine_MapCopyCellAttributes(19, 0x5A, 1, 1, 20, 0x59);
	{
		struct FieldActor *actor;

		actor = Object_GetById(21);
		actor->motion_flags = 0;
		actor->y.fixed = 0xC00000;
		actor->collision_flags = 8;
		Engine_ActorSetSpriteFlags(actor, 0);
	}
	Engine_TaskWait(1);
	if (Engine_GameFlagIsSet(0x87b) == 0 && gGameState.entrance == 15)
	{
		Scene_DoraSendsRobinToThePlaza();
		return 0;
	}
	Engine_ActorSetAnimation(23, 7);
	if (Engine_GameFlagIsSet(0x837) == 0)
	{
		Engine_EventBegin();
		Actor_SetAttachedEffect(22, 0x101);
		Actor_SetPosition(22, 0x1900000, 0x2630000);
		Actor_SetPosition(21, 0x1A80000, 0x2730000);
		Actor_WalkTo(22, 0x190, 0x26B);
		Actor_WalkToAndWait(21, 0x1A8, 0x26B);
		Engine_ActorSetAnimation(21, 2);
		Engine_ActorSetAnimation(22, 5);
		Engine_EventEnd();
	}
	else
	{
		Engine_EventBegin();
		Actor_SetPosition(21, 0x1A80000, 0x2730000);
		Actor_WalkToAndWait(21, 0x1A8, 0x26B);
		Engine_ActorSetAnimation(21, 3);
		Engine_EventEnd();
	}
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	gEventWork->transition_frames = 24;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	BattleFx_SetBlock30Values128One();
	return 0;
}

static void Scene_DoraSendsRobinToThePlaza(void)
{
	s32 message;

	Engine_EventBegin();
	Effect_SoundAndFlash();
	BattleFx_StartTwelveFrameBlend();
	BattleFx_SetBlock30Values12Zero();
	Engine_TaskWait(60);
	Camera_SetSpeed(0x4000, 0x800);
	Camera_MoveTo(0x13c0000, 0xa00000, 0x3700000, 1);
	Actor_SetPosition(ACTOR_KYLE, 0x1260000, 0x3640000);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	gEventWork->transition_frames = 16;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	BattleFx_SetBlock30Values128One();
	Event_PlayCue(158);
	Map_AnimateCells(HaidiaArashi_CellSteps1, 50, 44);
	Actor_SetAttachedEffect(22, 0x101);
	Actor_SetSpeed(ACTOR_DORA, 0xcccc, 0x6666);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	Actor_SetSpeed(ACTOR_KYLE, 0xcccc, 0x6666);
	Actor_SetPosition(ACTOR_DORA, 0x1560000, 0x37a0000);
	Actor_WalkToAndWait(ACTOR_DORA, 0x156, 0x389);
	BattleFx_PlayQueuedSound();
	Actor_WalkTo(ACTOR_DORA, 0x128, 0x389);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1560000, 0x37a0000);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x156, 0x37a);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x156, 0x389);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x13e, 0x389);
	Engine_ActorSetAnimation(ACTOR_DORA, 1);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
	Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
	message = (s32)MsgHaidiaKyleAbleStop;
	Engine_EventSetMessage(message);
	Event_ShowMessage(ACTOR_DORA, 0);
	Actor_WalkToAndWait(ACTOR_KYLE, 0x126, 0x346);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(ACTOR_KYLE, 4);
	Event_ShowMessage(ACTOR_KYLE, 0);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTH, 0);
	Event_ShowMessageAndWait(ACTOR_KYLE, 0, 20);
	Actor_ShowEmote(ACTOR_DORA, 0x101, 20);
	Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 10);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_KYLE, 4);
	Event_ShowMessage(ACTOR_KYLE, 0);
	Actor_SetAttachedEffect(ACTOR_DORA, 0x102);
	Engine_EventWait(30);
	Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 50);
	Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 10);
	Actor_SetSpeed(ACTOR_DORA, 0x18000, 0xc000);
	Actor_WalkToAndWait(ACTOR_DORA, 0x121, 0x373);
	Actor_FaceDirection(ACTOR_DORA, FACING_NORTHEAST, 0);
	Event_ShowMessage(ACTOR_DORA, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_KYLE, 2);
	Event_ShowMessage(ACTOR_KYLE, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 10);
	Engine_EventSetMessage((message + 8));
	Event_OpenMessage(ACTOR_DORA, 0);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x12e, 0x389);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	while (Engine_EventChooseYesNo(0, 0) == 1)
	{
		Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
		Engine_EventSetMessage((s32)MsgHaidiaBigBoyWhy);
		Event_OpenMessage(ACTOR_DORA, 0);
	}
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
	Engine_EventSetMessage((s32)MsgHaidiaKnowWayGo);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Actor_SetSpeed(ACTOR_KYLE, 0x18000, 0xc000);
	Actor_WalkTo(ACTOR_KYLE, 0x129, 0x2ee);
	Engine_EventWait(10);
	Actor_WalkToAndWait(ACTOR_DORA, 0x129, 0x2ee);
	Actor_SetPosition(ACTOR_DORA, 0, 0);
	Actor_SetPosition(ACTOR_KYLE, 0, 0);
	Engine_ActorSetAnimation(ACTOR_KYLE, 1);
	Engine_ActorSetAnimation(21, 2);
	Engine_ActorSetAnimation(22, 5);
	GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
	GameFlag_Set(0x87b);
	GameFlag_Set(0x205);
	Engine_EventEnd();
}

static void EV41_TOUCH(void)
{
	struct FieldActor *rec7;
	struct FieldActor *record;
	s32				   v3;
	s32				   base5_396;
	u8				  *p6;

	if (GameFlag_IsSet(0x310) != 0)
	{
	}
	else
	{
		Engine_EventBegin();
		if (GameFlag_IsSet(0x830) == 0)
		{
			rec7 = Object_GetById(11);
			p6 = (u8 *)rec7->sprite;
			Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
			Engine_AudioPlayCue(141);
			Engine_TaskWait(40);
			Engine_AudioPlayCue(145);
			rec7->priority_flags &= 254;
			((struct Flags9 *)p6)->mode = 1;
			Actor_SetPosition(11, 0x1d90000, 0x3a40000);
			rec7->speed = 0x18000;
			rec7->acceleration = 0x18000;
			v3 = (rec7->y.fixed + 0xf00000);
			rec7->y.fixed += 0xf00000;
			rec7->target_y = v3;
			rec7->bounce = 0x6666;
			Actor_WalkToAndWait(11, 0x158, 0x3a4);
			((struct Flags9 *)p6)->mode = 3;
			rec7->priority_flags |= 1;
			Engine_EventWait(40);
			Engine_AudioPlayCue(0x121);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_MapWaitWorkValuesBelow256();
			BattleFx_PlayQueuedSound();
			Engine_GameFlagSet(0x830);
		}
		HaidiaArashi_SetStormCellAttributes();
		Engine_GameFlagSet(0x310);
		if (Engine_GameFlagIsSet(0x837) != 0)
		{
			if (Engine_GameFlagIsSet(0x841) == 0)
			{
				if (Engine_GameFlagIsSet(0x30c) == 0)
				{
					record = Object_GetById(0);
					if (record->y.fixed > 0x800000)
					{
						base5_396 = 0x396;
						SceneActor_RunActor22PlacementSequence(0x146, base5_396);
						Actor_WalkToAndWait(0, 0x123, base5_396);
					}
					else
					{
						SceneActor_RunActor22PlacementSequence(0x14f, 0x3bd);
					}
					Engine_GameFlagSet(0x30c);
				}
			}
		}
		Engine_EventEnd();
	}
}

static void HaidiaArashi_SetStormCellAttributes(void)
{
	Map_CopyCellAttributes(29, 64, 1, 1, 21, 57);
	Map_CopyCellAttributes(29, 64, 1, 1, 21, 58);
	Map_CopyCellAttributes(29, 64, 1, 1, 22, 58);
	Map_CopyCellAttributes(29, 64, 1, 1, 20, 58);
	Map_CopyCellAttributes(28, 20, 1, 1, 20, 57);
}

static void EV42_TOUCH(void)
{
	struct FieldActor *rec7;
	struct FieldActor *record;
	s32				   v3;

	if (GameFlag_IsSet(0x311) != 0)
	{
	}
	else
	{
		Engine_EventBegin();
		if (GameFlag_IsSet(0x831) == 0)
		{
			rec7 = Object_GetById(12);
			Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
			Engine_AudioPlayCue(141);
			Engine_TaskWait(40);
			Engine_AudioPlayCue(145);
			Actor_SetPosition(12, 0x17d0000, 0x3280000);
			rec7->speed = 0x18000;
			rec7->acceleration = 0x18000;
			v3 = (rec7->y.fixed + 0x1000000);
			rec7->y.fixed += 0x1000000;
			rec7->target_y = v3;
			rec7->bounce = 0x8000;
			Actor_WalkToAndWait(12, 0x122, 0x341);
			Engine_ActorSetSpritePriority(12, 1);
			Actor_WalkToAndWait(12, 0x102, 0x354);
			Engine_ActorSetSpritePriority(12, 2);
			Actor_WalkToAndWait(12, 224, 0x368);
			Engine_EventWait(40);
			Engine_AudioPlayCue(0x121);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_MapWaitWorkValuesBelow256();
			BattleFx_PlayQueuedSound();
			Engine_GameFlagSet(0x831);
		}
		ActorPresentation_SetEightSceneCells();
		Engine_GameFlagSet(0x311);
		if (Engine_GameFlagIsSet(0x837) != 0)
		{
			if (Engine_GameFlagIsSet(0x841) == 0)
			{
				if (Engine_GameFlagIsSet(0x30c) == 0)
				{
					record = Object_GetById(0);
					if (record->y.fixed > 0x800000)
					{
						SceneActor_RunActor22PlacementSequence(219, 0x34b);
						Actor_WalkToAndWait(0, 179, 0x33d);
					}
					else
					{
						SceneActor_RunActor22PlacementSequence(214, 0x38c);
						Actor_WalkToAndWait(0, 219, 0x38f);
					}
					Engine_GameFlagSet(0x30c);
				}
			}
		}
		Engine_EventEnd();
	}
}

static void ActorPresentation_SetEightSceneCells(void)
{
	s32 a = 15;
	s32 d = 0x35;
	s32 e;
	s32 b;
	s32 c;
	s32 f;

	Engine_MapCopyCellAttributes(29, 23, 1, 1, a, d);
	b = 14;
	Engine_MapCopyCellAttributes(29, 23, 1, 1, b, d);
	c = 13;
	Engine_MapCopyCellAttributes(29, 23, 1, 1, c, d);
	Engine_MapCopyCellAttributes(26, 20, 2, 1, b, 0x34);
	e = 0x36;
	Engine_MapCopyCellAttributes(25, 21, 1, 1, c, e);
	Engine_MapCopyCellAttributes(25, 21, 1, 1, a, e);
	Engine_MapCopyCellAttributes(14, 0x35, 1, 1, b, e);
	f = 0x37;
	Engine_MapCopyCellAttributes(13, 0x37, 1, 1, a, f);
}

static void EV43_TOUCH(void)
{
	if (GameFlag_IsSet(0x312) == 0)
	{
		Engine_EventBegin();
		if (GameFlag_IsSet(0x832) == 0)
		{
			struct FieldActor *actor = Actor_Get(13);
			struct FieldActor *leader = Actor_Get(ACTOR_PARTY_LEADER);
			u16				   priority = leader->sprite->priority;
			u8				   flags = leader->priority_flags;

			Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
			Event_PlayCue(141);
			Engine_TaskWait(40);
			Event_PlayCue(145);
			Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
			Actor_Get(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
			Actor_SetPosition(13, 0, 0x2bf0000);
			actor->speed = 0x18000;
			actor->acceleration = 0x18000;
			actor->y.fixed += 0x500000;
			actor->target_y = actor->y.fixed;
			actor->bounce = 0x8000;
			Actor_WalkToAndWait(13, 64, 0x2bf);
			Engine_EventWait(40);
			Event_PlayCue(0x121);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_MapWaitWorkValuesBelow256();
			BattleFx_PlayQueuedSound();
			GameFlag_Set(0x832);
			Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, priority);
			Actor_Get(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
			leader->priority_flags = flags;
		}
		SceneState_ApplyFourRects();
		GameFlag_Set(0x312);
		if (GameFlag_IsSet(0x837) != 0)
		{
			if (GameFlag_IsSet(0x841) == 0)
			{
				if (GameFlag_IsSet(0x30c) == 0)
				{
					if (Actor_Get(ACTOR_PARTY_LEADER)->z.fixed <= 0x2b4ffff)
					{
						SceneActor_RunActor22PlacementSequence(62, 0x29d);
						Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 27, 0x273);
					}
					else
					{
						SceneActor_RunActor22PlacementSequence(75, 0x2cb);
						Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 67, 0x2f5);
					}
					GameFlag_Set(0x30c);
				}
			}
		}
		Engine_EventEnd();
	}
}

static void SceneState_ApplyFourRects(void)
{

	s32 a = 0x2a;
	s32 b;

	Engine_MapCopyCellAttributes(29, 22, 1, 1, 3, a);
	b = 2;
	Engine_MapCopyCellAttributes(29, 21, 1, 1, b, a);
	Engine_MapCopyCellAttributes(29, 21, 1, 1, 4, a);
	Engine_MapCopyCellAttributes(23, 20, 3, 1, b, 0x2b);
}

static void EV44_TOUCH(void)
{
	struct FieldActor *rec7;
	struct FieldActor *record;
	s32				   v3;

	if (GameFlag_IsSet(0x313) != 0)
	{
	}
	else
	{
		Engine_EventBegin();
		if (GameFlag_IsSet(0x833) == 0)
		{
			rec7 = Object_GetById(14);
			Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
			Engine_AudioPlayCue(141);
			Engine_TaskWait(40);
			Engine_AudioPlayCue(145);
			Actor_SetPosition(14, 0x1da0000, 0x47b0000);
			rec7->speed = 0x10000;
			rec7->acceleration = 0x10000;
			v3 = (rec7->y.fixed + 0x480000);
			rec7->y.fixed += 0x480000;
			rec7->target_y = v3;
			rec7->bounce = 0x8000;
			Engine_ActorWalkToAndWait(14, 0x1b0, 0x47b);
			Engine_EventWait(40);
			Engine_AudioPlayCue(0x121);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_MapWaitWorkValuesBelow256();
			BattleFx_PlayQueuedSound();
			Engine_GameFlagSet(0x833);
		}
		FieldScene_DrawFiveTileBlocks();
		Engine_GameFlagSet(0x313);
		if (Engine_GameFlagIsSet(0x837) != 0)
		{
			if (Engine_GameFlagIsSet(0x841) == 0)
			{
				if (Engine_GameFlagIsSet(0x30c) == 0)
				{
					record = Object_GetById(0);
					if (record->z.fixed <= 0x479ffff)
					{
						SceneActor_RunActor22PlacementSequence(0x19c, 0x460);
						Actor_WalkToAndWait(0, 0x19e, 0x42c);
					}
					else
					{
						SceneActor_RunActor22PlacementSequence(0x1bd, 0x494);
						Actor_WalkToAndWait(0, 0x1bf, 0x4cb);
					}
					Engine_GameFlagSet(0x30c);
				}
			}
		}
		Engine_EventEnd();
	}
}

static void FieldScene_DrawFiveTileBlocks(void)
{
	s32 a = 26;
	s32 h = 0x47;
	s32 b;
	s32 a2;

	Engine_MapCopyCellAttributes(29, 20, 1, 1, a, h);
	b = 0x46;
	Engine_MapCopyCellAttributes(29, 20, 1, 1, a, b);
	a2 = 27;
	Engine_MapCopyCellAttributes(29, 20, 1, 1, a2, b);
	Engine_MapCopyCellAttributes(28, 21, 1, 1, 28, h);
	Engine_MapCopyCellAttributes(28, 22, 1, 1, a2, 0x48);
}

static void FieldScene_RunScene372SequenceA(void)
{
	s32 base;

	Engine_EventBegin();
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x106, 0x32a);
	Actor_SetPosition(20, 0x1060000, 0x3250000);
	Actor_WalkTo(20, 0x106, 0x339);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x11a, 0x357);
	Engine_ActorSetAnimation(20, 1);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 20, 0);
	BattleFx_PlayQueuedSound();
	Engine_EventWait(30);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Actor_ShowEmote(20, 0x100, 20);
	base = (s32)MsgHaidiaHuh;
	Engine_EventSetMessage(base);
	Event_ShowMessage(20, 0);
	Engine_EventWait(20);
	Event_AskYesNo(20, 0);
	Engine_ActorRunRepeatedMotion(20, 2);
	Engine_EventSetMessage((base + 4));
	Event_ShowMessageAndWait(20, 0, 20);
	Object_SetActionCallbackAndRefreshById(20, (s32)HaidiaArashi_ActorTwentyScript);
	GameFlag_Set(0x835);
	Engine_EventEnd();
}

static void EV30_TOUCH(void)
{
	if (GameFlag_IsSet(0x836) == 0)
	{
		if (GameFlag_IsSet(0x837) == 0)
		{
			Engine_EventBegin();
			Engine_EventSetMessage((s32)MsgHaidiaUghHrnghhh);
			Event_ShowMessageAndWait(22, 0, 20);
			Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 40);
			Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x17e, 0x26b);
			Actor_FaceActor(ACTOR_PARTY_LEADER, 22, 0);
			Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			Engine_EventWait(30);
			Event_ShowMessage(22, 0);
			GameFlag_Set(0x836);
			Engine_EventEnd();
		}
	}
}

static void EV31_TOUCH(void)
{
	if (Engine_GameFlagIsSet(0x837) == 0 && Engine_GameFlagIsSet(0x836) != 0)
	{
		Engine_EventBegin();
		Engine_ActorRunRepeatedMotion(22, 2);
		Engine_EventWait(20);
		Engine_EventSetMessage((s32)MsgHaidiaDontLeaveMeHere);
		HaidiaArashi_RunCallOutSequence();
		Engine_EventEnd();
	}
}

static void EV22_TALK(void)
{
	if (GameFlag_IsSet(0x841) != 0)
	{
		Engine_EventBegin();
		Actor_FaceActor(22, ACTOR_PARTY_LEADER, 0);
		Engine_EventWait(20);
		Engine_EventSetMessage((s32)MsgHaidiaBoulderNeedGet);
		Event_ShowMessage(22, 0);
		Actor_FaceDirection(22, FACING_NORTHEAST, 10);
		Engine_EventEnd();
	}
	else
	{
		if (GameFlag_IsSet(0x837) == 0)
		{
			Engine_EventBegin();
			Engine_EventSetMessage((s32)MsgHaidiaWantDumpStuff);
			HaidiaArashi_RunCallOutSequence();
			Engine_EventEnd();
		}
	}
}

static void HaidiaArashi_RunCallOutSequence(void)
{
	struct FieldActor *record;
	s32				   v5;

	Engine_EventOpenMessage(22, 0);
	Engine_ActorFaceEachOther(0, 22, 0);
	v5 = 0;
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventSetMessage((s32)MsgHaidiaThinkForgetThings);
		v5 = 1;
	}
	else
	{
		Engine_EventSetMessage((s32)MsgHaidiaRockHitsLose);
	}
	Engine_EventWait(20);
	Engine_EventShowMessageAndWait(22, 0, 40);
	Actor_SetAttachedEffect(22, 0x100);
	Engine_ActorSetAnimation(21, 3);
	Engine_ActorSetAnimation(22, 1);
	Engine_EventWait(40);
	Engine_ActorFaceActor(22, 0, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(22, 3);
	if (v5 != 0)
	{

		Engine_EventSetMessage((s32)MsgHaidiaKnowRightOk);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgHaidiaRightDitchStuff);
	}
	Engine_EventShowMessage(22, 0);
	Engine_ActorSetAnimation(22, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Engine_ActorSetDestination(22, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(22);
	Engine_ActorSetPosition(22, 0, 0);
	Event_PrepareObjectAndApplyValue(1, 1);
	Engine_GameFlagSet(0x837);
}

static void EV32_TOUCH(void)
{
	struct FieldActor *record;
	s32				   base;

	if (GameFlag_IsSet(0x837) == 0)
	{
		Engine_EventBegin();
		Actor_SetAttachedEffect(22, 0x100);
		base = (s32)MsgHaidiaHey;
		Engine_EventSetMessage(base);
		Event_ShowMessage(22, 0);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 20);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		Camera_SetSpeed(0x6666, 0xccc);
		Camera_MoveTo(0x1000000, -1, 0x24c0000, 1);
		Actor_SetSpeed(22, 0x20000, 0x10000);
		Object_SetActionCallbackAndRefreshById(22, (s32)HaidiaArashi_ActorTwentyTwoScriptA);
		Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
		Engine_EventWait(30);
		Engine_ActorEnableActionCallback(22, HaidiaArashi_ActorTwentyTwoScriptB);
		Event_ShowMessage(22, 0);
		record = Actor_Get(22);
		record->scale_y = 0x10000;
		Engine_ActorRunRepeatedMotion(22, 1);
		Engine_EventWait(20);
		Event_AskYesNo(22, 0);
		Engine_EventWait(40);
		Engine_ActorRunRepeatedMotion(22, 1);
		Engine_EventSetMessage((base + 5));
		Event_ShowMessageAndWait(22, 0, 20);
		Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
		Engine_ActorSetAnimationAndWait(22, 3);
		Event_ShowMessage(22, 0);
		Actor_SetSpeed(22, 0x10000, 0x8000);
		Engine_ActorSetAnimation(22, 2);
		record = Object_GetById(ACTOR_PARTY_LEADER);
		if (record != 0)
		{
			Actor_SetDestination(22, record->x.part.pixel, record->z.part.pixel);
		}
		Engine_ActorWaitForMove(22);
		Actor_SetPosition(22, 0, 0);
		Event_PrepareObjectAndApplyValue(1, 1);
		Engine_ActorSetAnimation(21, 3);
		GameFlag_Set(0x837);
		Engine_EventEnd();
	}
}

static void EV33_TOUCH(void)
{
	struct FieldActor *record;
	struct FieldActor *actor;

	Engine_EventBegin();
	actor = Actor_Get(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		Actor_SetPosition(22, actor->x.fixed, actor->z.fixed);
	}
	Actor_SetSpeed(22, 0x10000, 0x8000);
	Actor_WalkToAndWait(22, 0x119, 0x1fb);
	Engine_ActorFaceEachOther(22, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(30);
	Engine_EventSetMessage((s32)MsgHaidiaNorthLeadsToMtAleph);
	Event_ShowMessage(22, 0);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 22, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(22, FACING_SOUTH, 0);
	Event_ShowMessage(22, 0);
	Engine_ActorSetAnimation(22, 2);
	record = Object_GetById(ACTOR_PARTY_LEADER);
	if (record != 0)
	{
		Actor_SetDestination(22, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(22);
	Actor_SetPosition(22, 0, 0);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x100, 0x205);
	Engine_EventEnd();
}

static void SceneActor_RunActor22PlacementSequence(s32 x, s32 y)
{
	struct FieldActor *a;
	s32				   w = 0x10000;
	s32				   h = 0x8000;
	struct FieldActor *b;

	a = Object_GetById(ACTOR_PARTY_LEADER);
	if (a != 0)
	{
		Engine_ActorSetPosition(22, a->x.fixed, a->z.fixed);
	}
	Engine_ActorSetSpeed(22, w, h);
	Engine_ActorWalkToAndWait(22, x, y);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
	Engine_EventWait(20);
	Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Engine_EventWait(40);
	Engine_EventSetMessage((s32)MsgHaidiaCantGetAroundThisRock);
	Engine_EventShowMessage(22, 0);
	Engine_ActorRunRepeatedMotion(22, 2);
	Engine_EventShowMessage(22, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(22, 2);
	b = Object_GetById(ACTOR_PARTY_LEADER);
	if (b != 0)
	{
		Engine_ActorSetDestination(22, b->x.part.pixel, b->z.part.pixel);
	}
	Engine_ActorWaitForMove(22);
	Engine_ActorSetPosition(22, 0, 0);
}

static void EV34_TOUCH(void)
{
	u32				   i;
	struct FieldActor *record;
	s32				  *phase;
	struct FieldActor *actor;
	s32				   steps;
	s32				   callback;
	s32				   shifted;

	if (GameFlag_IsSet(FLAG_BOULDER_FELL) == 0)
	{
		Engine_EventBegin();
		Event_CallWithLastActiveObjectId((s32)HaidiaArashi_LastObjectCall);
		SceneState_SetValue140Mode0();
		Engine_TaskWait(1);
		Event_PlayCue(141);
		Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
		Engine_EventWait(30);
		Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
		Event_PlayCue(145);
		Engine_EventWait(30);
		actor = Actor_Get(ACTOR_PARTY_LEADER);
		if (actor != NULL)
		{
			Actor_SetPosition(22, actor->x.fixed, actor->z.fixed);
		}
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
		Actor_SetSpeed(22, 0x20000, 0x10000);
		Engine_ActorEnableActionCallback(0, HaidiaArashi_LeaderScript);
		Object_SetActionCallbackAndRefreshById(22, (s32)HaidiaArashi_ActorTwentyTwoScript);
		Object_RefreshSelectorById(0);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
		Actor_ShowEmote(22, 0x100, 30);
		Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
		Event_PlayCue(145);
		Engine_EventWait(40);
		Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
		Event_PlayCue(145);
		Engine_EventWait(20);
		Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
		Actor_SetAttachedEffect(22, 0x102);
		Engine_EventWait(40);
		Engine_ActorSetAnimation(32, 5);
		Engine_ActorSetAnimation(33, 5);
		Engine_ActorSetAnimation(30, 8);
		Engine_ActorSetAnimation(29, 8);
		Actor_Get(30)->scale_x = -0x10000;
		Engine_ActorSetSpritePriority(32, 2);
		Engine_ActorSetSpritePriority(33, 2);
		Engine_ActorSetSpritePriority(30, 3);
		Engine_ActorSetSpritePriority(29, 3);
		Engine_EventSetMessage((s32)MsgHaidiaTheBoulderIsFalling);
		Event_ShowMessageAndWait(28, 0, 20);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		Actor_FaceDirection(22, FACING_NORTH, 20);
		Camera_SetSpeed(0x40000, 0x8000);
		Camera_MoveTo(0x700000, -1, 0x14b0000, 1);
		Engine_CameraWaitForMove();
		for (i = 0; i < 40; i++)
		{
			SceneActor_SetModeByFrameBit1(Actor_Get(32));
			SceneActor_SetModeByFrameBit1(Actor_Get(33));
			SceneActor_SetModeByFrameBit1(Actor_Get(30));
			SceneActor_SetModeByFrameBit1(Actor_Get(29));
			Engine_TaskWait(1);
		}
		phase = &HaidiaArashi_ShakeShift;
		steps = (s32)FieldScene_RunFourPairedSteps;
		HaidiaArashi_ShakeDone = 0;
		*phase = 0;
		FieldScheduler_AddOrUpdateCallback(steps, 0xc80);
		callback = (s32)SceneState_SetValue19ThenCall;
		Engine_TaskAddCallback(callback, 0xc80);
		Engine_EventWait(40);
		*phase = 1;
		Engine_EventWait(30);
		Actor_SetPosition(19, 0x720000, 0x1220000);
		record = Actor_Get(19);
		shifted = record->y.fixed + 0x400000;
		record->y.fixed = shifted;
		record->target_y = shifted;
		Actor_SetSpeed(19, 0xcccc, 0x6666);
		Event_PlayCue(145);
		Actor_MoveToAndWait(19, 114, 0x14d);
		Engine_ActorSetAnimation(19, 2);
		Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
		Event_PlayCue(145);
		*phase = 0;
		Actor_SetSpeed(19, 0x6666, 0x3333);
		Actor_MoveToAndWait(19, 114, 0x12c);
		Engine_ActorSetAnimation(19, 2);
		Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
		Event_PlayCue(145);
		*phase = 2;
		Actor_SetSpeed(19, 0xcccc, 0x6666);
		Actor_MoveToAndWait(19, 114, 0x14d);
		Engine_ActorSetAnimation(19, 2);
		Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
		Event_PlayCue(145);
		*phase = 0;
		Actor_SetSpeed(19, 0x6666, 0x3333);
		Actor_MoveToAndWait(19, 114, 0x12c);
		Engine_ActorSetAnimation(19, 2);
		Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
		Event_PlayCue(145);
		*phase = 2;
		Actor_SetSpeed(19, 0xcccc, 0x6666);
		Actor_MoveToAndWait(19, 114, 0x14d);
		Engine_ActorSetAnimation(19, 2);
		Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
		Event_PlayCue(145);
		*phase = 1;
		Engine_EventWait(20);
		Actor_SetAttachedEffect(32, 0x102);
		Engine_ActorRunRepeatedMotion(32, 2);
		Event_ShowMessage(31, 0);
		Actor_ShowEmote(33, 0x100, 0);
		Engine_ActorRunRepeatedMotion(33, 2);
		Event_ShowMessageAndWait(28, 0, 40);
		Actor_SetAttachedEffect(30, 0x102);
		Engine_ActorRunRepeatedMotion(30, 2);
		Event_ShowMessage(30, 0);
		HaidiaArashi_ShakeDone = 1;
		Engine_ActorSetAnimation(29, 1);
		Engine_TaskWait(1);
		Actor_SetChildValue(29, 0);
		Actor_ShowEmote(29, 0x105, 20);
		Actor_FaceDirection(29, FACING_WEST, 40);
		Actor_FaceDirection(29, FACING_EAST, 20);
		Actor_FaceDirection(29, FACING_WEST, 20);
		Actor_FaceDirection(29, FACING_SOUTH, 40);
		Actor_ShowEmote(29, 0x100, 0);
		Engine_ActorRunRepeatedMotion(29, 2);
		Engine_ActorJump(29, 4, 40);
		Engine_ActorSetAnimation(29, 9);
		Engine_EventWait(10);
		Event_ShowMessageAndWait(29, 0, 20);
		Event_PlayCue(0x121);
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		Camera_SetSpeed(0x60000, 0xc000);
		Camera_MoveTo(0x540000, -1, 0x2340000, 1);
		Engine_CameraWaitForMove();
		BattleFx_PlayQueuedSound();
		Actor_FaceActor(22, ACTOR_PARTY_LEADER, 0);
		Engine_EventWait(20);
		Actor_SetAttachedEffect(22, 0x102);
		Engine_EventWait(30);
		Scheduler_RemoveCallback(steps);
		Scheduler_RemoveCallback(callback);
		Event_ShowMessage(22, 0);
		Actor_FaceActor(ACTOR_PARTY_LEADER, 22, 0);
		Engine_EventWait(20);
		FieldScene_RunSingleStep();
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
		Engine_ActorSetAnimationAndWait(22, 3);
		Engine_EventWait(20);
		Engine_ActorSetAnimation(22, 2);
		record = Object_GetById(ACTOR_PARTY_LEADER);
		if (record != 0)
		{
			Actor_SetDestination(22, record->x.part.pixel, record->z.part.pixel);
		}
		Engine_ActorWaitForMove(22);
		Actor_SetPosition(22, 0, 0);
		Engine_ActorDestroy(31);
		Engine_ActorDestroy(28);
		Engine_ActorDestroy(30);
		Engine_ActorDestroy(29);
		Engine_ActorDestroy(32);
		Engine_ActorDestroy(33);
		GameFlag_Set(FLAG_BOULDER_FELL);
		Engine_EventEnd();
	}
}

static void Arashi_SetActorFrameMode(struct FieldActor *actor, s32 mode)
{
	if ((actor->active & 15) == 1)
	{
		struct AnimationObject *animation = (struct AnimationObject *)actor->sprite;
		s32						param = mode - 1;
		u8						count;
		if (mode == 0)
		{
			param = HaidiaArashi_FrameModes[(gFrameCount >> 1) & (actor->active & 15)];
		}
		count = animation->count;
		if (count != 0)
		{
			struct AnimationEntry **entries = animation->entries;
			s32						k = count;
			do
			{
				struct AnimationEntry *entry = *entries++;
				if (entry != NULL && entry->script != NULL)
				{
					entry->param = param;
				}
				k--;
			} while (k != 0);
		}
		animation->dirty = 1;
	}
}

static void ActorPresentation_SetFourActorsModeByBit(void)
{
	if (((gFrameCount >> HaidiaArashi_ShakeShift) & 3) != 0)
	{
		Arashi_SetActorFrameMode(Object_GetById(32), 1);
		Arashi_SetActorFrameMode(Object_GetById(33), 1);
		Arashi_SetActorFrameMode(Object_GetById(30), 1);
		Arashi_SetActorFrameMode(Object_GetById(29), 1);
	}
	else
	{
		Arashi_SetActorFrameMode(Object_GetById(32), 8);
		Arashi_SetActorFrameMode(Object_GetById(33), 8);
		Arashi_SetActorFrameMode(Object_GetById(30), 8);
		Arashi_SetActorFrameMode(Object_GetById(29), 8);
	}
}

static void EV35_TOUCH(void)
{
	u8 *tbl;

	if (GameFlag_IsSet(0x83a) != 0)
	{
		return;
	}
	Engine_EventBegin();
	Actor_SetPosition(10, 0xC00000, 0x4BE0000);
	Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
	Engine_ActorSetAnimation(10, 5);
	{
		struct FieldActor *o;
		s32				   v;
		o = Object_GetById(10);
		v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
		tbl = HaidiaArashi_ActorEightScript;
		o->timer = v;
		Engine_ActorEnableActionCallback(10, (s32)tbl);
	}
	Actor_SetPosition(9, 0xC00000, 0x4A50000);
	Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
	Actor_SetPosition(24, 0xE30000, 0x4BE0000);
	Actor_FaceDirection(24, FACING_SOUTH, 0);
	Engine_ActorSetAnimation(24, 6);
	{
		struct FieldActor *o;
		s32				   v;
		o = Object_GetById(24);
		v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
		o->timer = v;
		Engine_ActorEnableActionCallback(24, (s32)tbl);
	}
	Actor_SetPosition(25, 0xFA0000, 0x4BE0000);
	Actor_FaceDirection(25, FACING_SOUTH, 0);
	Engine_ActorSetAnimation(25, 6);
	{
		struct FieldActor *o;
		s32				   v;
		o = Object_GetById(25);
		v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
		o->timer = v;
		Engine_ActorEnableActionCallback(25, (s32)tbl);
	}
	Actor_SetPosition(26, 0xE30000, 0x4A50000);
	Actor_FaceDirection(26, 0x3000, 0);
	Actor_SetPosition(23, 0xF30000, 0x4FD0000);
	Actor_FaceDirection(23, FACING_NORTH, 0);
	Engine_ActorSetSpriteFlags(Object_GetById(23), 0);
	Engine_TaskWait(3);
	Engine_EventSetMessage((s32)MsgHaidiaNoBrother);
	Engine_EventShowMessage(0x201a, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 20);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 150, 0x446);
	{
		struct FieldActor *p;
		p = Object_GetById(ACTOR_PARTY_LEADER);
		if (p != 0)
		{
			Engine_ActorSetPosition(22, p->x.fixed, p->z.fixed);
		}
	}
	Actor_WalkToAndWait(22, 132, 0x446);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(22, FACING_SOUTH, 20);
	Camera_SetSpeed(0x40000, 0x8000);
	Camera_MoveTo(0xD80000, -1, 0x4D00000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(10, 2);
	Engine_EventShowMessageAndWait(10, 0, 10);
	Engine_ActorRunRepeatedMotion(23, 3);
	Engine_ActorFaceDirection(9, FACING_EAST, 10);
	Engine_ActorSetAnimationAndWait(9, 3);
	Engine_EventShowMessageAndWait(9, 0, 10);
	Actor_FaceDirection(9, 0x3000, 10);
	Camera_SetSpeed(0x30000, 0x6000);
	Camera_MoveTo(0xE80000, -1, 0x4E50000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Engine_AudioPlayCue(134);
	Engine_ActorJump(23, 4, 0);
	Engine_ActorSetAnimation(23, 6);
	Engine_EventWait(10);
	Engine_ActorSetPosition(23, 0, 0);
	Engine_EventWait(60);
	BattleFx_PlayQueuedSound();
	Engine_ActorSetAnimation(10, 1);
	{
		struct FieldActor *o;
		o = Object_GetById(10);
		o->scale_x = 0x10000;
		o->scale_y = 0x10000;
	}
	Engine_ActorSetAnimation(24, 1);
	{
		struct FieldActor *o;
		o = Object_GetById(24);
		o->scale_x = 0x10000;
		o->scale_y = 0x10000;
	}
	Engine_ActorSetAnimation(25, 1);
	{
		struct FieldActor *o;
		o = Object_GetById(25);
		o->scale_x = 0x10000;
		o->scale_y = 0x10000;
	}
	Engine_ActorStartRepeatedMotion(10, 2);
	Engine_ActorStartRepeatedMotion(9, 2);
	Engine_ActorStartRepeatedMotion(24, 2);
	Engine_ActorStartRepeatedMotion(25, 2);
	Engine_ActorRunRepeatedMotion(26, 2);
	Camera_SetSpeed(0x9999, 0x1333);
	Camera_MoveTo(0xD80000, -1, 0x4D00000, 1);
	Engine_CameraWaitForMove();
	Actor_SetAttachedEffect(26, 0x102);
	Engine_ActorSetAttachedEffect(9, 0x102);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(26, 2);
	Engine_ActorStartRepeatedMotion(26, 3);
	Engine_EventShowMessage(26, 0);
	Engine_ActorJump(25, 2, 0);
	Actor_SetDestination(25, 234, 0x4B5);
	Engine_ActorJump(26, 2, 0);
	Actor_SetDestination(26, 227, 0x4B1);
	Engine_EventWait(90);
	Camera_MoveTo(0xE80000, -1, 0x4E50000, 1);
	Engine_CameraWaitForMove();
	Engine_ActorSetPosition(23, 0xF30000, 0x4FD0000);
	Engine_TaskWait(1);
	Engine_AudioPlayCue(106);
	{
		struct FieldActor *o;
		o = Object_GetById(23);
		o->velocity_y = 0x20000;
	}
	Engine_EventWait(6);
	Engine_ActorSetAnimation(23, 7);
	Engine_EventWait(20);
	BattleFx_PlayQueuedSound();
	Engine_EventWait(20);
	Engine_CameraSetSpeed(0x19999, 0x3333);
	Engine_CameraMoveTo(0xD80000, -1, 0x4D00000, 1);
	Engine_CameraWaitForMove();
	Engine_ActorRunRepeatedMotion(24, 2);
	Engine_EventWait(20);
	Actor_ShowEmote(24, 0x105, 40);
	Engine_ActorFaceEachOther(24, 10, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(10, 2);
	Event_ShowMessage(0x800A, 0);
	{
		struct FieldActor *o;
		o = Object_GetById(25);
		o->steering_flags &= 0xFE;
	}
	{
		struct FieldActor *o;
		o = Object_GetById(26);
		o->steering_flags &= 0xFE;
	}
	Actor_SetSpeed(25, 0x9999, 0x4CCC);
	Actor_SetSpeed(26, 0x9999, 0x4CCC);
	Actor_SetDestination(25, 247, 0x4BA);
	Actor_MoveToAndWait(26, 227, 0x4A5);
	{
		struct FieldActor *o;
		o = Object_GetById(25);
		o->steering_flags |= 1;
	}
	Object_GetById(26)->steering_flags |= 1;
	Engine_ActorFaceDirection(26, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(25, FACING_WEST, 10);
	Engine_ActorSetAnimation(24, 4);
	Engine_EventShowMessageAndWait(0x8018, 0, 10);
	Actor_FaceDirection(10, FACING_NORTH, 20);
	Engine_ActorFaceDirection(10, FACING_EAST, 10);
	Engine_ActorSetAnimation(10, 4);
	Event_ShowMessageAndWait(0x800A, 0, 10);
	Actor_ShowEmote(24, 0x105, 0);
	Actor_ShowEmote(10, 0x105, 60);
	Actor_ShowEmote(9, 0x106, 20);
	Actor_FaceDirection(9, FACING_WEST, 40);
	Actor_FaceDirection(9, FACING_NORTH, 20);
	Engine_ActorFaceDirection(9, FACING_EAST, 30);
	Actor_FaceDirection(9, FACING_SOUTH, 10);
	Engine_EventShowMessageAndWait(9, 0, 10);
	Actor_FaceDirection(10, FACING_NORTH, 0);
	Actor_FaceDirection(25, 0x9000, 0);
	Actor_FaceDirection(24, FACING_NORTHWEST, 0);
	Actor_FaceDirection(26, FACING_WEST, 10);
	Engine_ActorRunRepeatedMotion(10, 1);
	Event_ShowMessageAndWait(0x800A, 0, 10);
	Engine_ActorSetAnimation(9, 4);
	Engine_EventShowMessageAndWait(9, 0, 10);
	Actor_ShowEmote(10, 0x105, 0);
	Actor_ShowEmote(24, 0x105, 0);
	Actor_ShowEmote(25, 0x105, 0);
	Actor_ShowEmote(26, 0x105, 40);
	Engine_ActorFaceDirection(9, FACING_EAST, 10);
	Engine_ActorFaceEachOther(24, 25, 0);
	Engine_EventWait(20);
	Engine_ActorFaceDirection(9, FACING_EAST, 0);
	Engine_ActorFaceDirection(10, FACING_EAST, 10);
	Actor_FaceDirection(24, FACING_WEST, 0);
	Actor_FaceDirection(25, FACING_WEST, 10);
	Engine_ActorSetAnimation(24, 3);
	Engine_ActorSetAnimationAndWait(25, 3);
	Engine_ActorFaceEachOther(10, 9, 0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(10, 1);
	Engine_EventShowMessageAndWait(0x800A, 0, 10);
	Engine_ActorSetAnimationAndWait(9, 3);
	Actor_FaceDirection(24, 0xD000, 10);
	Engine_ActorStartRepeatedMotion(24, 1);
	Engine_EventShowMessageAndWait(24, 0, 10);
	Engine_ActorFaceDirection(10, FACING_EAST, 0);
	Engine_ActorFaceDirection(9, FACING_EAST, 0);
	Engine_ActorRunRepeatedMotion(26, 1);
	Actor_FaceDirection(26, FACING_SOUTHEAST, 20);
	Actor_FaceDirection(25, FACING_NORTHWEST, 20);
	Engine_ActorSetAnimationAndWait(25, 3);
	Engine_EventShowMessageAndWait(25, 0, 10);
	Engine_ActorSetAnimationAndWait(26, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_ActorSetAnimationAndWait(9, 3);
	Engine_EventShowMessageAndWait(9, 0, 10);
	Actor_FaceDirection(26, FACING_WEST, 10);
	Engine_ActorSetAnimationAndWait(26, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(9, 1);
	Engine_EventShowMessageAndWait(9, 0, 10);
	HaidiaArashi_RunRiverSearch();
	Engine_GameFlagSet(0x83A);
	Engine_EventEnd();
}

static void HaidiaArashi_RunRiverSearch(void)
{
	struct FieldActor *entry;
	s32				   record;
	s32				   script;
	s32				   north;
	s32				   suppose;

	Actor_FaceDirection(26, 0x3000, 0);
	Actor_FaceDirection(24, 0xd000, 0);
	Actor_FaceDirection(25, 0xb000, 0);
	Actor_FaceDirection(9, 0x3000, 0);
	Actor_FaceDirection(10, 0xd000, 20);
	Engine_ActorSetAnimation(26, 3);
	Engine_ActorSetAnimation(24, 3);
	Engine_ActorSetAnimation(25, 3);
	Engine_ActorSetAnimation(9, 3);
	Engine_ActorSetAnimationAndWait(25, 3);
	Engine_EventWait(20);
	Camera_SetSpeed(0x10000, 0x2000);
	Camera_MoveTo(0x860000, -1, 0x4ab0000, 1);
	Actor_SetSpeed(26, 0x19999, 0xcccc);
	Actor_SetSpeed(9, 0x19999, 0xcccc);
	Engine_ActorEnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptA);
	Object_SetActionCallbackAndRefreshById(9, (s32)HaidiaArashi_ActorNineScriptA);
	Event_PlayCue(158);
	Map_AnimateCells(HaidiaArashi_CellSteps2, 38, 72);
	Engine_EventWait(10);
	Actor_WalkToAndWait(9, 149, 0x497);
	Actor_SetPosition(9, 0, 0);
	Actor_WalkToAndWait(25, 250, 0x4be);
	BattleFx_PlayQueuedSound();
	Actor_FaceDirection(10, 0x3000, 0);
	Actor_FaceDirection(24, 0x3000, 0);
	Actor_FaceDirection(25, 0x3000, 0);
	Engine_ActorSetAnimation(10, 5);
	Engine_ActorSetAnimation(24, 6);
	Engine_ActorSetAnimation(25, 6);
	entry = Actor_Get(10);
	record = Engine_RandomNext();
	entry->timer = (Math_RemainderUnsigned(record, 90) + 60);
	entry = Actor_Get(24);
	record = Engine_RandomNext();
	entry->timer = (Math_RemainderUnsigned(record, 90) + 60);
	entry = Actor_Get(25);
	record = Engine_RandomNext();
	entry->timer = (Math_RemainderUnsigned(record, 90) + 60);
	script = (s32)HaidiaArashi_ActorEightScript;
	Engine_ActorEnableActionCallback(10, script);
	Engine_ActorEnableActionCallback(24, script);
	Engine_ActorEnableActionCallback(25, script);
	Object_RefreshSelectorById(26);
	Engine_EventWait(10);
	Event_PlayCue(159);
	Map_AnimateCells(HaidiaArashi_CellSteps5, 38, 72);
	Engine_EventWait(30);
	BattleFx_PlayQueuedSound();
	Camera_MoveTo(0x700000, -1, 0x4c90000, 1);
	Event_PlayCue(158);
	Map_AnimateCells(HaidiaArashi_CellSteps1, 35, 73);
	Engine_EventWait(20);
	BattleFx_PlayQueuedSound();
	Engine_ActorEnableActionCallback(9, HaidiaArashi_ActorNineScriptB);
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptB);
	Engine_EventWait(40);
	Event_PlayCue(159);
	Map_AnimateCells(HaidiaArashi_CellSteps4, 35, 73);
	Object_RefreshSelectorById(26);
	BattleFx_PlayQueuedSound();
	Engine_EventWait(40);
	north = (s32)MsgHaidiaGoLookNorth;
	Engine_EventSetMessage(north);
	Event_ShowMessageAndWait(9, 0, 20);
	Engine_ActorSetAnimationAndWait(26, 3);
	Event_ShowMessageAndWait(0x201a, 0, 40);
	Engine_ActorSetAnimation(9, 3);
	Engine_ActorSetAnimationAndWait(26, 3);
	Engine_EventWait(30);
	Engine_ActorEnableActionCallback(9, HaidiaArashi_ActorNineScriptC);
	Engine_ActorEnableActionCallback(26, HaidiaArashi_ActorTwentySixScriptC);
	Engine_EventWait(40);
	Camera_SetSpeed(0x20000, 0x4000);
	Camera_MoveTo(0x690000, -1, 0x43e0000, 1);
	Object_RefreshSelectorById(9);
	Actor_FaceDirection(9, FACING_EAST, 0);
	Actor_ShowEmote(9, 0x100, 40);
	Event_ShowMessageAndWait(9, 0, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(22, FACING_WEST, 10);
	Actor_WalkToAndWait(9, 105, 0x43e);
	Engine_ActorRunRepeatedMotion(9, 2);
	Event_OpenMessage(0x8009, 0);
	Actor_FaceDirection(22, FACING_EAST, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_ActorSetAnimationAndWait(9, 3);
		Engine_EventSetMessage((north + 4));
	}
	else
	{
		Engine_ActorRunRepeatedMotion(9, 2);
		Engine_EventSetMessage((north + 5));
	}
	Event_ShowMessage(0x8009, 0);
	Actor_FaceDirection(22, FACING_WEST, 40);
	Actor_ShowEmote(9, 0x100, 30);
	suppose = (s32)MsgHaidiaDontSupposeTwo;
	Engine_EventSetMessage(suppose);
	Event_OpenMessage(0x8009, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_ActorSetAnimationAndWait(9, 3);
		Engine_EventSetMessage((suppose + 1));
		Event_ShowMessageAndWait(0x8009, 0, 30);
		Actor_FaceDirection(22, FACING_WEST, 20);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
		Engine_ActorSetAnimation(22, 3);
		Engine_ActorSetAnimationAndWait(9, 3);
		Engine_EventWait(40);
	}
	else
	{
		Actor_ShowEmote(9, 0x105, 90);
		Actor_ShowEmote(9, 0x103, 40);
		Engine_ActorSetAnimation(9, 4);
		Engine_EventSetMessage((suppose + 2));
		Event_ShowMessage(0x8009, 0);
	}
	Engine_ActorEnableActionCallback(9, HaidiaArashi_ActorNineScriptD);
	Engine_EventWait(90);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
	Engine_EventWait(40);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(22, 2);
	record = (s32)Actor_Get(0);
	if (record != 0)
	{
		Actor_SetDestination(22, ((struct FieldActor *)record)->x.part.pixel,
			((struct FieldActor *)record)->z.part.pixel);
	}
	Engine_ActorWaitForMove(22);
	Actor_SetPosition(22, 0, 0);
}

static void EV10_TALK(void)
{
	s32 v2000 = 0x2000;
	u8 *tbl;

	Engine_EventBegin();
	Engine_ActorSetAnimation(10, 1);
	Engine_EventWait(10);
	Engine_ActorFaceEachOther(10, ACTOR_PARTY_LEADER, 20);
	if (Engine_GameFlagIsSet(0x30d) != 0)
	{
		Engine_EventSetMessage((s32)MsgHaidiaTwoDontEnough);
		Engine_EventShowMessageAndWait(10, 0, 10);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgHaidiaOh);
		Engine_ActorStartRepeatedMotion(10, 1);
		Engine_EventShowMessageAndWait(10, 0, 10);
		Engine_ActorStartRepeatedMotion(10, 2);
		Engine_EventShowMessageAndWait(10, 0, 10);
	}
	Engine_ActorFaceDirection(10, v2000, 20);
	Engine_ActorSetAnimation(10, 5);
	Engine_EventWait(10);
	{
		struct FieldActor *rec;
		s32				   v;
		rec = Object_GetById(10);
		v = Math_RemainderUnsigned(Engine_RandomNext(), 0x5A) + 60;
		tbl = HaidiaArashi_ActorEightScript;
		rec->timer = v;
		Engine_ActorEnableActionCallback(10, tbl);
	}
	Engine_EventWait(20);
	Engine_GameFlagSet(0x30d);
	Engine_EventEnd();
}

static void EV36_TOUCH(void)
{
	struct FieldActor *rec;
	struct FieldActor *record;
	s32				   v5;
	u8				  *p4;

	if (Engine_GameFlagIsSet(0x840) == 0)
	{
	}
	else
	{
		if (GameFlag_IsSet(0x841) != 0)
		{
		}
		else
		{
			Engine_EventBegin();
			Actor_SetSpeed(0, 0x10000, 0x8000);
			Actor_SetSpeed(22, 0x10000, 0x8000);
			Actor_SetSpeed(26, 0x10000, 0x8000);
			Actor_SetSpeed(8, 0x10000, 0x8000);
			Actor_WalkToAndWait(0, 217, 0x557);
			record = Object_GetById(0);
			if (record != 0)
			{
				Engine_ActorSetPosition(22, record->x.fixed, record->z.fixed);
			}
			Actor_WalkToAndWait(22, 235, 0x557);
			Actor_FaceDirection(22, 0xb000, 0);
			record = Object_GetById(0);
			if (record != 0)
			{
				Engine_ActorSetPosition(26, record->x.fixed, record->z.fixed);
			}
			Actor_WalkToAndWait(26, 199, 0x557);
			Actor_FaceDirection(26, 0xd000, 0);
			Actor_SetPosition(25, 0xf70000, 0x4ba0000);
			Engine_ActorFaceDirection(25, FACING_SOUTHWEST, 0);
			record = Object_GetById(8);
			p4 = (u8 *)record->sprite;
			record->priority_flags &= 254;
			((struct Flags9 *)p4)->mode = 1;
			rec = Object_GetById(0);
			p4 = (u8 *)rec->sprite;
			rec->priority_flags &= 254;
			((struct Flags9 *)p4)->mode = 2;
			record = Object_GetById(0);
			if (record != 0)
			{
				Engine_ActorSetPosition(8, record->x.fixed, record->z.fixed);
			}
			Actor_WalkToAndWait(8, 221, 0x569);
			Actor_FaceDirection(8, 0xb000, 60);
			Engine_ActorRunRepeatedMotion(26, 2);
			Engine_EventSetMessage((s32)MsgHaidiaMomDadBack);
			Engine_EventShowMessageAndWait(26, 0, 40);
			Actor_SetPosition(9, 0x650000, 0x4ad0000);
			Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
			Engine_EventShowMessageAndWait(0x1009, 0, 10);
			Actor_FaceDirection(26, FACING_NORTHWEST, 0);
			Engine_CameraSetSpeed(0x13333, 0x2666);
			Camera_MoveTo(0x650000, -1, 0x4ad0000, 1);
			Actor_SetSpeed(9, 0x16666, 0xb333);
			Engine_ActorEnableActionCallback(9, HaidiaArashi_StormRunActions);
			Engine_EventWait(60);
			Engine_CameraSetSpeed(0x9999, 0x1333);
			Camera_MoveTo(0xbb0000, -1, 0x5300000, 1);
			Engine_CameraWaitForMove();
			Engine_EventWait(40);
			Engine_ActorRunRepeatedMotion(26, 2);
			Engine_EventShowMessageAndWait(26, 0, 20);
			Engine_ActorRunRepeatedMotion(9, 2);
			Engine_EventShowMessageAndWait(0x4009, 0, 20);
			Engine_CameraSetSpeed(0x20000, 0x4000);
			Camera_MoveTo(0xdd0000, -1, 0x5690000, 1);
			Engine_ActorFaceActor(0, 8, 0);
			Engine_ActorFaceActor(22, 8, 0);
			Actor_FaceDirection(26, 0x3000, 80);
			Engine_CameraMoveTo(0xb60000, -1, 0x5500000, 1);
			Actor_WalkToAndWait(8, 182, 0x568);
			Engine_ActorFaceActor(8, 9, 0);
			Engine_EventWait(30);
			Engine_ActorSetAnimationAndWait(8, 3);
			Engine_EventWait(10);
			Engine_ActorFaceActor(0, 9, 0);
			Engine_ActorFaceActor(22, 9, 0);
			Engine_ActorFaceActor(26, 9, 0);
			Engine_ActorSetAnimationAndWait(9, 3);
			Engine_EventShowMessage(9, 0);
			Engine_ActorRunRepeatedMotion(26, 2);
			Engine_EventShowMessageAndWait(26, 0, 10);
			Actor_FaceDirection(9, FACING_NORTHEAST, 40);
			Actor_FaceDirection(9, 0x3000, 20);
			Engine_ActorSetAnimationAndWait(9, 3);
			Engine_EventShowMessage(9, 0);
			Engine_ActorFaceEachOther(26, 8, 0);
			Engine_ActorFaceEachOther(22, 0, 0);
			Engine_EventWait(40);
			Engine_ActorFaceActor(0, 9, 0);
			Engine_ActorFaceActor(22, 9, 0);
			Engine_ActorFaceActor(26, 9, 0);
			Engine_ActorFaceActor(8, 9, 0);
			Engine_ActorRunRepeatedMotion(9, 2);
			Engine_EventWait(20);
			Engine_EventShowMessageAndWait(9, 0, 10);
			Engine_ActorSetAnimation(0, 3);
			Engine_ActorSetAnimation(26, 3);
			Engine_ActorSetAnimation(22, 3);
			v5 = 1;
			Engine_ActorSetAnimationAndWait(8, 3);
			rec->priority_flags |= v5;
			record = Object_GetById(8);
			record->priority_flags |= v5;
			Scene_RunActorGroupDepartureSequence();
			Engine_GameFlagSet(0x841);
			Engine_EventEnd();
		}
	}
}

static __inline__ void HaidiaArashi_PlaceActor(struct FieldActor *object, s32 pos, s32 depth)
{
	object->x.fixed = pos;
	object->y.fixed = pos;
	object->target_x = pos;
	object->target_y = pos;
	object->z.fixed = depth;
	object->target_z = depth;
}

static void Scene_RunActorGroupDepartureSequence(void)
{
	struct FieldSprite *actorVisual;
	struct FieldSprite *groupVisual;
	struct FieldActor  *actor;
	struct FieldActor  *fieldActor;
	struct FieldActor  *groupActor;
	struct FieldActor  *work;
	u32					random;
	const u8		   *entryActions;
	s32					zero;
	u8					still;
	const u8		   *moveActions;
	s32					phase;
	const u8		   *departureActions;

	actor = Actor_Get(19);
	groupActor = Actor_Get(27);
	actorVisual = actor->sprite;
	groupVisual = groupActor->sprite;
	Camera_SetSpeed(0x10000, 0x2000);
	Camera_MoveTo(0x6e0000, -1, 0x58b0000, 1);
	Actor_SetSpeed(8, 0x13333, 0x9999);
	Actor_SetSpeed(26, 0x13333, 0x9999);
	Actor_SetSpeed(0, 0x13333, 0x9999);
	Actor_SetSpeed(22, 0x13333, 0x9999);
	entryActions = HaidiaArashi_ArriveScript;
	Actor_EnableActionCallback(8, entryActions);
	Event_Wait(10);
	Actor_EnableActionCallback(26, entryActions);
	BattleFx_SetBlock30Values12Zero();
	Event_Wait(10);
	Actor_EnableActionCallback(0, entryActions);
	Event_Wait(10);
	BattleFx_SetBlock30ValuesMaxZero();
	Actor_EnableActionCallback(22, entryActions);
	Event_Wait(128);
	EV14_TOUCH();
	Camera_MoveTo(0xae0000, -1, 0x5940000, 1);
	Event_Wait(104);
	Camera_MoveTo(0x990000, -1, 0x52d0000, 1);
	Actor_WalkToAndWait(9, 158, 0x4f8);
	Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
	Object_RefreshSelectorById(8);
	Actor_EnableActionCallback(8, HaidiaArashi_GatherPath8);
	Actor_EnableActionCallback(26, HaidiaArashi_GatherPath26);
	Actor_EnableActionCallback(0, HaidiaArashi_GatherPath0);
	Object_SetActionCallbackAndRefreshById(22, HaidiaArashi_GatherPath22);
	Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
	Event_PlayCue(145);
	Event_Wait(20);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Event_Wait(60);
	Actor_ShowEmote(0, 0x101, 0);
	Actor_ShowEmote(26, 0x101, 0);
	Actor_ShowEmote(22, 0x101, 0);
	Actor_ShowEmote(8, 0x101, 0);
	Actor_ShowEmote(9, 0x101, 60);
	Actor_FaceEachOther(26, 8, 0);
	Actor_FaceEachOther(22, 0, 0);
	Event_Wait(20);
	fieldActor = Actor_Get(0);
	random = Random_Next();
	random = Math_RemainderUnsigned(random, 20) + 20;
	fieldActor->timer = random;
	zero = 0;
	still = 0;
	fieldActor = Actor_Get(22);
	random = Random_Next();
	fieldActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
	fieldActor = Actor_Get(26);
	random = Random_Next();
	fieldActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
	fieldActor = Actor_Get(8);
	random = Random_Next();
	fieldActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
	fieldActor = Actor_Get(9);
	random = Random_Next();
	fieldActor->timer = (Math_RemainderUnsigned(random, 20) + 20);
	moveActions = HaidiaArashi_LookAroundScript;
	Actor_EnableActionCallback(9, moveActions);
	Event_Wait(30);
	Actor_EnableActionCallback(0, moveActions);
	Actor_EnableActionCallback(26, moveActions);
	Actor_EnableActionCallback(22, moveActions);
	Actor_EnableActionCallback(8, moveActions);
	Event_Wait(10);
	Event_PlayCue(17);
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	Event_PlayCue(145);
	Event_Wait(30);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Event_Wait(120);
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	Event_PlayCue(145);
	Event_Wait(40);
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	Event_Wait(60);
	Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
	Event_PlayCue(145);
	Event_Wait(20);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Event_Wait(60);
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	Event_PlayCue(145);
	Event_Wait(40);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Event_Wait(60);
	BattleFx_SetBlock30ValuesMaxZero();
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Event_Wait(1);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Camera_SetSpeed(0x80000, 0x80000);
	Camera_MoveTo(0xd90000, -1, 0x43c0000, 1);
	ColorBuffer_ApplyTarget(0, 0);
	Engine_ColorBufferInterpolate(40);
	Task_Wait(40);
	Actor_SetChildValue(19, 0);
	work = Actor_Get(19);
	Actor_SetSpriteFlags(work, 0);
	work = Actor_Get(27);
	Actor_SetSpriteFlags(work, 0);
	groupActor->scale_x = 0xcccc;
	groupActor->scale_y = 0xcccc;
	groupActor->priority_flags &= 254;
	groupVisual->priority = 1;
	HaidiaArashi_PlaceActor(actor, 0xc80000, 0x3820000);
	actor->motion_flags = still;
	actor->priority_flags &= 254;
	actorVisual->priority = 0;
	work = Battle_GetWorkObject1e0();
	work->target_x = 0x80000000;
	work = Battle_GetWorkObject1e0();
	work->target_y = 0x80000000;
	work = Battle_GetWorkObject1e0();
	work->target_z = 0x80000000;
	work = Battle_GetWorkObject1e0();
	work->velocity_x = zero;
	work = Battle_GetWorkObject1e0();
	work->velocity_y = zero;
	work = Battle_GetWorkObject1e0();
	work->velocity_z = zero;
	Task_Wait(1);
	Camera_MoveTo(0xf70000, 0x800000, 0x3950000, 0);
	Engine_MapRedraw();
	Task_Wait(1);
	ColorBuffer_ApplyTarget(0x10003, 1);
	ColorBuffer_ApplyTarget(0x10000, 2);
	Engine_ColorBufferInterpolate(30);
	Task_Wait(30);
	Engine_TaskAddCallback(HaidiaArashi_UpdatePulsingGlow, 0xc80);
	Actor_EnableActionCallback(19, HaidiaArashi_ActorNineteenRiseScript);
	Camera_SetSpeed(0x20000, 0x7ae);
	Camera_MoveTo(0xaf0000, 0x600000, 0x43e0000, 1);
	do
	{
		Task_Wait(1);
	} while ((s16)actor->work != 8);
	ColorBuffer_ApplyTarget(0, 0);
	Engine_ColorBufferInterpolate(60);
	Task_Wait(60);
	Engine_MapWaitWorkValuesBelow256();
	work = Battle_GetWorkObject1e0();
	work->target_x = 0x80000000;
	work = Battle_GetWorkObject1e0();
	work->target_y = 0x80000000;
	work = Battle_GetWorkObject1e0();
	work->target_z = 0x80000000;
	phase = 0;
	work = Battle_GetWorkObject1e0();
	work->velocity_x = phase;
	work = Battle_GetWorkObject1e0();
	work->velocity_y = phase;
	work = Battle_GetWorkObject1e0();
	work->velocity_z = phase;
	Scheduler_RemoveCallback(HaidiaArashi_UpdatePulsingGlow);
	Actor_Stop(19);
	Task_Wait(1);
	Actor_SetAnimation(19, 0);
	groupActor->scale_x = 0x14000;
	groupActor->scale_y = 0x14000;
	groupVisual->unknown_20[3] = 2;
	groupVisual->scale = 0x14000;
	actor->scale_x = 0x20000;
	actor->scale_y = 0x20000;
	actor->x.fixed = phase;
	actor->z.fixed = phase;
	actor->target_x = phase;
	actor->target_z = phase;
	Task_Wait(1);
	Actor_SetAnimation(23, 8);
	Actor_SetPosition(9, 0xa90000, 0x4f00000);
	Actor_FaceDirection(9, FACING_NORTH, 0);
	Actor_SetAnimation(9, 9);
	Actor_SetPosition(26, 0x970000, 0x50c0000);
	Actor_FaceDirection(26, FACING_WEST, 0);
	Actor_SetAnimation(26, 5);
	Actor_SetPosition(8, 0xaa0000, 0x5210000);
	Actor_FaceDirection(8, FACING_SOUTHWEST, 0);
	Actor_SetAnimation(8, 5);
	Actor_SetPosition(0, 0xb90000, 0x5350000);
	Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
	Actor_SetAnimation(0, 17);
	Actor_SetPosition(22, 0xa90000, 0x5680000);
	Actor_FaceDirection(22, FACING_SOUTH, 0);
	Actor_SetAnimation(22, 0);
	Camera_MoveTo(0xa60000, 0, 0x5390000, 0);
	Engine_MapRedraw();
	actor->motion_flags = phase;
	actor->target_x = 0x80000000;
	actor->target_y = 0x80000000;
	actor->target_z = 0x80000000;
	HaidiaArashi_FlashLightning();
	Actor_SetPosition(27, 0xda0000, 0x4980000);
	Camera_MoveTo(0xd20000, 0, 0x4ac0000, 0);
	Engine_MapRedraw();
	groupActor->scale_x = 0x20000;
	groupActor->scale_y = 0x20000;
	Engine_TaskAddCallback(ActorPresentation_SelectActorTwentySevenState, 0xc80);
	Actor_Stop(10);
	Actor_Stop(24);
	Actor_Stop(25);
	Task_Wait(1);
	groupActor = Actor_Get(10);
	groupVisual = groupActor->sprite;
	groupActor->priority_flags &= 254;
	groupActor->scale_x = 0x10000;
	groupActor->scale_y = 0x10000;
	groupActor->facing = 0xd000;
	groupVisual->priority = 0;
	Actor_SetAnimation(10, 0);
	groupActor = Actor_Get(24);
	groupVisual = groupActor->sprite;
	groupActor->priority_flags &= 254;
	groupActor->scale_x = 0x10000;
	groupActor->scale_y = 0x10000;
	groupActor->facing = 0xb000;
	groupVisual->priority = 0;
	Actor_SetAnimation(24, 5);
	groupActor = Actor_Get(25);
	groupVisual = groupActor->sprite;
	groupActor->priority_flags &= 254;
	groupActor->scale_x = 0x10000;
	groupActor->scale_y = 0x10000;
	groupActor->facing = 0xb000;
	groupVisual->priority = 0;
	Actor_SetAnimation(25, 5);
	groupActor = Actor_Get(27);
	groupVisual = groupActor->sprite;
	HaidiaArashi_FlashLightning();
	actor->y.fixed = 0x300000;
	actor->x.fixed = 0xd60000;
	actor->z.fixed = 0x4c00000;
	actor->target_x = 0x80000000;
	actor->target_y = 0x80000000;
	actor->target_z = 0x80000000;
	groupVisual->priority = 1;
	Actor_SetPosition(27, 0xd60000, 0x4c00000);
	Actor_FaceDirection(24, FACING_NORTH, 0);
	Actor_FaceDirection(25, FACING_NORTH, 20);
	GameFlag_Set(0x166);
	Map_SetLayerEntryFlag(0);
	Map_SetLayerEntryFlag(1);
	Map_SetLayerEntryFlag(2);
	Map_SetLayerEntryFlag(3);
	Map_SetLayerEntryFlag(4);
	Map_SetLayerEntryFlag(5);
	ColorBuffer_ApplyTarget(0x10003, 1);
	ColorBuffer_ApplyTarget(0x10000, 2);
	Engine_ColorBufferInterpolate(120);
	Task_Wait(160);
	ColorBuffer_ApplyTarget(0x7fff, 1);
	ColorBuffer_ApplyTarget(0x7fff, 2);
	Engine_ColorBufferInterpolate(80);
	Event_Wait(80);
	Event_Wait(100);
	Scheduler_RemoveCallback(ActorPresentation_SelectActorTwentySevenState);
	groupVisual->scale = groupActor->scale_x;
	GameFlag_Clear(0x166);
	Map_ClearLayerEntryFlag(0);
	Map_ClearLayerEntryFlag(1);
	Map_ClearLayerEntryFlag(2);
	Map_ClearLayerEntryFlag(3);
	Map_ClearLayerEntryFlag(4);
	Map_ClearLayerEntryFlag(5);
	FieldScene_BuildPlacementGrid();
	Actor_SetPosition(9, 0xa50000, 0x4cd0000);
	Actor_SetAnimation(9, 1);
	actor = Actor_Get(9);
	actor->facing = 0xe000;
	random = Random_Next();
	actor->timer = Math_RemainderUnsigned(random, 90) + 60;
	departureActions = HaidiaArashi_ActorEightScript;
	actor->work = 1;
	Actor_EnableActionCallback(9, departureActions);
	Actor_SetPosition(26, 0xa50000, 0x4e60000);
	Actor_SetAnimation(26, 1);
	actor = Actor_Get(26);
	actor->facing = 0xe000;
	random = Random_Next();
	actor->timer = Math_RemainderUnsigned(random, 90) + 60;
	actor->work = 2;
	Actor_EnableActionCallback(26, departureActions);
	Actor_SetPosition(22, 0x980000, 0x5050000);
	Actor_SetAnimation(22, 1);
	actor = Actor_Get(22);
	actor->facing = 0xe000;
	random = Random_Next();
	actor->timer = Math_RemainderUnsigned(random, 90) + 60;
	actor->work = 3;
	Actor_EnableActionCallback(22, departureActions);
	Actor_SetPosition(8, 0xb40000, 0x51f0000);
	actor = Actor_Get(8);
	actor->facing = 0xe000;
	random = Random_Next();
	actor->timer = Math_RemainderUnsigned(random, 90) + 60;
	actor->work = 4;
	Actor_EnableActionCallback(8, departureActions);
	Actor_SetAnimation(8, 6);
	Object_GetById(22)->priority_flags &= 254;
	Object_GetById(8)->priority_flags &= 254;
	Engine_TaskAddCallback(HaidiaArashi_CopyLeaderSpritePriority, 0xc80);
	Actor_SetPosition(0, 0xb50000, 0x4f90000);
	work = Actor_Get(0);
	work->facing = 0xe000;
	Actor_SetAnimation(0, 1);
	Camera_MoveTo(0xb50000, 0, 0x4f90000, 0);
	Engine_MapRedraw();
	Actor_SetPosition(10, 0, 0);
	Actor_SetPosition(19, 0, 0);
	Actor_SetPosition(24, 0, 0);
	Actor_SetPosition(25, 0, 0);
	Actor_SetPosition(23, 0, 0);
	Actor_SetPosition(27, 0, 0);
	Actor_SetPosition(17, 0x900000, 0x42e0000);
	Actor_SetPosition(18, 0x1140000, 0x4f60000);
	Task_Wait(60);
	ColorBuffer_ApplyTarget(0x10003, 1);
	ColorBuffer_ApplyTarget(0x10000, 2);
	Engine_ColorBufferInterpolate(80);
	Event_Wait(60);
	BattleFx_PlayQueuedSound();
	Event_Wait(60);
	Party_RemoveOwnerRestored(1);
	BattleFx_SetBlock30Values128One();
}

struct Pulse
{
	u8	unknown_00[0x64];
	s16 timer;
	s16 beat;
};

struct PairDetail
{
	u8 unknown_00[22];
	u8 field_16;
};

struct PairSprite
{
	struct FieldSprite sprite;
	struct PairDetail *detail;
};

union PairObject
{
	union FieldObject object;
	s32				  words[28];
	struct
	{
		u8				  unknown_00[0x68];
		union PairObject *parent;
	} link;
};

struct PairWork
{
	u8	unknown_00[70];
	u16 vram_block;
};

struct WorldMapOam
{
	u8	unknown_00[4];
	u16 attr0;
	u16 x : 9;
	u16 affine_index : 5;
	u16 size : 2;
};

static void HaidiaArashi_FlashLightning(void)
{
	Engine_TaskWait(20);
	GameFlag_Set(0x166);
	Map_SetLayerEntryFlag(0);
	Map_SetLayerEntryFlag(1);
	Map_SetLayerEntryFlag(2);
	Map_SetLayerEntryFlag(3);
	Map_SetLayerEntryFlag(4);
	Map_SetLayerEntryFlag(5);
	ColorBuffer_ApplyTarget(0x10003, 1);
	ColorBuffer_ApplyTarget(0x10000, 2);
	Engine_ColorBufferInterpolate(1);
	Engine_TaskWait(120);
	ColorBuffer_ApplyTarget(0, 0);
	Engine_ColorBufferInterpolate(60);
	Engine_TaskWait(60);
	GameFlag_Clear(0x166);
	Map_ClearLayerEntryFlag(0);
	Map_ClearLayerEntryFlag(1);
	Map_ClearLayerEntryFlag(2);
	Map_ClearLayerEntryFlag(3);
	Map_ClearLayerEntryFlag(4);
	Map_ClearLayerEntryFlag(5);
}

static void HaidiaArashi_UpdatePulsingGlow(void)
{
	struct FieldActor  *source;
	struct FieldActor  *glow;
	struct FieldSprite *sprite;
	s16				   *timer;

	source = Object_GetById(19);
	glow = Object_GetById(27);
	sprite = glow->sprite;
	timer = &((struct Pulse *)source)->timer;
	if (*timer != 0)
	{
		if (*timer == 60)
		{
			Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
		}
		if (*timer == 40)
		{
			Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
		}
		if (*timer == 30)
		{
			Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
		}
		if (*timer == 20)
		{
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		}
		(*timer)--;
	}
	glow->x.fixed = source->x.fixed;
	glow->z.fixed = source->z.fixed;
	glow->target_x = glow->x.fixed;
	((u8 *)sprite)[35] = 10;
	if (gFrameCount & 1)
	{
		switch (((struct Pulse *)source)->beat)
		{
		case 1:
		case 5:
			glow->scale_x += 0xa3d;
			glow->scale_y += 0xa3d;
			break;
		case 4:
			glow->scale_x += 0x51e;
			glow->scale_y += 0x51e;
			break;
		case 2:
		case 3:
		case 6:
		case 7:
		case 8:
		case 9:
			glow->scale_x += -0x7ae;
			glow->scale_y += -0x7ae;
			break;
		}
		sprite->scale = glow->scale_x;
	}
	else
	{
		sprite->scale = 0;
	}
}

static void ActorPresentation_SelectActorTwentySevenState(void)
{
	struct FieldActor *actor = Object_GetById(27);
	u32				   flags = gFrameCount;
	u8				  *presentation = (u8 *)actor->sprite;

	if (flags & 1)
	{
		u8 *state = presentation + 35;
		*state = 2;
	}
	else
	{
		u8 *state = presentation + 35;
		*state = 64;
	}
}

static void EV08_TALK(void)
{
	struct FieldActor *rec7;
	struct FieldActor *rec8;

	rec8 = Object_GetById(ACTOR_PARTY_LEADER);
	rec7 = Object_GetById(8);
	Engine_EventBegin();
	if (GameFlag_IsSet(0x305) != 0)
	{
		Engine_ActorStop(8);
		Engine_EventWait(10);
		Engine_ActorRunRepeatedMotion(8, 2);
		Engine_EventWait(40);
		if ((s16)rec8->facing >= 0)
		{
			Engine_ActorSetAnimation(8, 7);
		}
		else
		{
			Engine_ActorSetAnimation(8, 8);
		}
		Engine_ActorRunRepeatedMotion(8, 2);
		Engine_EventWait(20);
		Engine_EventSetMessage((s32)MsgHaidiaICantMoveGetHelp);
		Event_ShowMessage(8, 0);
		Engine_ActorEnableActionCallback(8, HaidiaArashi_ActorEightScript);
		Engine_ActorSetAnimation(8, 6);
	}
	else
	{
		Engine_ActorStop(8);
		rec7->scale_x = 0x10000;
		rec7->scale_y = 0x10000;
		Actor_FaceDirection(8, 0x1000, 0);
		if ((s16)rec8->facing >= 0)
		{
			Engine_ActorSetAnimation(8, 7);
		}
		else
		{
			Engine_ActorSetAnimation(8, 8);
		}
		Engine_EventWait(20);
		Engine_EventSetMessage((s32)MsgHaidiaIllGo);
		Event_ShowMessageAndWait(8, 0, 20);
		Engine_ActorSetAnimation(8, 1);
		Engine_ActorJump(8, 4, 0);
		Engine_EventWait(80);
		Engine_ActorRunRepeatedMotion(8, 2);
		Engine_EventWait(40);
		if ((s16)rec8->facing >= 0)
		{
			Engine_ActorSetAnimation(8, 7);
		}
		else
		{
			Engine_ActorSetAnimation(8, 8);
		}
		Engine_EventWait(2);
		Engine_ActorJump(8, 2, 0);
		Engine_EventWait(60);
		Engine_ActorRunRepeatedMotion(8, 2);
		Engine_EventWait(20);
		Event_ShowMessage(8, 0);
		Engine_ActorEnableActionCallback(8, HaidiaArashi_ActorEightScript);
		Engine_ActorSetAnimation(8, 6);
		GameFlag_Set(0x305);
	}
	Engine_EventEnd();
}

static void FieldScene_ConfigureActorTwentyTwoScene(void)
{
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_ActorStop(ACTOR_ID);
	Scheduler_RemoveCallback((s32)HaidiaArashi_CopyLeaderSpritePriority);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1e0, 0x570);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Actor_FaceDirection(ACTOR_ID, 0x3000, 20);
	Actor_Get(ACTOR_ID)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
	Actor_SetPosition(ACTOR_ID, 0xf90000, 0x4d80000);
	Engine_TaskWait(1);
	Engine_EventSetMessage((s32)MsgHaidiaRobin);
	Event_ShowMessage(0x1016, 0);
	Actor_SetPosition(ACTOR_ID, 0xac0000, 0x4fe0000);
	Engine_TaskWait(1);
	Camera_SetSpeed(0x40000, 0x8000);
	Camera_MoveTo(0xa20000, 0, 0x5050000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(ACTOR_ID, 4);
	Event_ShowMessageAndWait(0x1016, 0, 10);
	Actor_FaceDirection(ACTOR_ID, FACING_NORTH, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_ID, 2);
	Event_ShowMessageAndWait(0x1016, 0, 10);
	Actor_FaceDirection(ACTOR_ID, 0x1000, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_ID, 3);
	Actor_SetSpeed(ACTOR_ID, 0x20000, 0x10000);
	Actor_WalkToAndWait(ACTOR_ID, 165, 0x514);
	Actor_WalkToAndWait(ACTOR_ID, 195, 0x598);
	GameFlag_Set(0x842);
}

static void FieldScene_BuildPlacementGrid(void)
{
	Map_CopyCellsTo(16, 96, 11, 73, 6, 3);
	Map_CopyCellsTo(16, 96, 34, 68, 14, 10);
	Map_CopyCellsTo(16, 96, 64, 68, 7, 7);
	Map_CopyCellsTo(9, 95, 11, 73, 6, 3);
	Map_CopyCellsTo(40, 94, 34, 68, 14, 10);
	Map_CopyCellsTo(54, 94, 64, 68, 8, 7);
	Map_CopyCellsTo(72, 75, 72, 76, 1, 1);
	Map_CopyCellsTo(72, 75, 74, 76, 1, 1);
	Map_CopyCellAttributes(7, 75, 1, 1, 6, 75);
	Map_CopyCellAttributes(8, 70, 3, 1, 8, 71);
	Map_CopyCellAttributes(8, 70, 2, 1, 9, 72);
	Map_CopyCellAttributes(8, 70, 2, 1, 9, 73);
	Map_CopyCellAttributes(11, 66, 1, 1, 8, 73);
	Map_CopyCellAttributes(12, 66, 1, 4, 11, 73);
	Map_CopyCellAttributes(25, 0, 1, 1, 6, 74);
	Engine_MapRedraw();
}

static void EV16_TOUCH(void)
{

	struct EventWork *state;

	Engine_EventBegin();
	state = gEventWork;
	state->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	state->transition_frames = 64;
	Engine_GameFlagSet(0x87c);
	BattleFx_SetWeightedResult(12, 2);
	Engine_GameFlagSet(0x900);
	Engine_EventEnd();
}

static void EV17_TOUCH(void)
{

	struct EventWork *state;

	Engine_EventBegin();
	state = gEventWork;
	state->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	state->transition_frames = 64;
	Engine_GameFlagSet(0x87f);
	BattleFx_SetWeightedResult(12, 3);
	Engine_GameFlagSet(0x900);
	Engine_EventEnd();
}

static void SceneActor_SetModeByFrameBit1(s32 o)
{
	s32 v;

	if ((gFrameCount & 2) != 0)
	{
		Engine_ObjectSetPartPalettes(o, 7);
	}
	else
	{
		Engine_ObjectSetPartPalettes(o, 0);
	}
	{
		u32 *frame = &gFrameCount;

		v = (HaidiaArashi_ShakeShift << 3) + 16;
		if (Math_RemainderUnsigned(*frame, v) == 0)
		{
			HaidiaArashi_SpawnEffectPair((union PairObject *)o);
		}
	}
}

static void OverlayObject_UpdateRandomSlotByFrame(s32 obj)
{
	s32 t;
	s32 n;

	if ((gFrameCount & 1) != 0)
	{
		t = (gFrameCount >> 1);
		Engine_ObjectSetPartPalettes(obj, Math_RemainderUnsigned(t, 6));
	}
	n = (HaidiaArashi_ShakeShift << 3) + 16;
	if (Math_RemainderUnsigned(gFrameCount, n) == 0)
	{
		HaidiaArashi_SpawnEffectPair((union PairObject *)obj);
	}
}

static void HaidiaArashi_CycleObjectPalettes(s32 o)
{
	s32 t;

	if ((gFrameCount & 1) != 0)
	{
		t = (gFrameCount >> 1);
		Engine_ObjectSetPartPalettes(o, Math_RemainderUnsigned(t, 6));
	}
}

static void Effect_AnimateVerticalPositive(union FieldObject *object)
{
	struct FieldActor *self = &object->actor;
	struct FieldActor *anchor;
	s32				   frame;
	s32				   amplitude;

	anchor = self->linked_object;
	self->timer = (self->timer + 1);
	frame = self->timer;

	if (frame > 31)
	{
		Engine_ObjectDispatchRelease(self);
		return;
	}

	amplitude = Engine_MathSin(frame << 10);
	self->scale_x = amplitude;
	self->scale_y = amplitude;
	self->x.fixed = anchor->x.fixed;
	self->y.fixed += 0x10000;
	self->z.fixed = anchor->z.fixed + (0x10000 - amplitude) * 5 + 0x80000;
}

static void Effect_AnimateVerticalNegative(union FieldObject *object)
{
	struct FieldActor *self = &object->actor;
	struct FieldActor *anchor;
	s32				   frame;
	s32				   amplitude;

	anchor = self->linked_object;
	self->timer = (self->timer + 1);
	frame = self->timer;

	if (frame > 31)
	{
		Engine_ObjectDispatchRelease(self);
		return;
	}

	amplitude = Engine_MathSin(frame << 10);
	self->scale_x = amplitude;
	self->scale_y = -amplitude;
	self->x.fixed = anchor->x.fixed;
	self->y.fixed += 0x10000;
	self->z.fixed = anchor->z.fixed - (0x10000 - amplitude) * 5 + 0x100000;
}

static void HaidiaArashi_SpawnEffectPair(union PairObject *parent)
{
	union PairObject   *pair[2];
	union PairObject   *child;
	struct PairSprite  *part;
	struct FieldSprite *sprite;
	struct PairWork	   *work = gEffectWork;
	s32					i;

	Engine_AudioPlayCue(152);
	for (i = 0; i < 2; ++i)
	{
		child = (union PairObject *)Engine_ObjectCreate(26, parent->object.actor.x.fixed,
			parent->object.actor.y.fixed, parent->object.actor.z.fixed);
		pair[i] = child;
		if (child != NULL)
		{
			child->words[5] = parent->words[5];
			part = (struct PairSprite *)child->object.actor.sprite;
			child->object.actor.motion_flags = 0;
			child->object.effect.spin = 0;
			child->link.parent = parent;
			if (part != NULL)
			{
				sprite = &part->sprite;
				Object_InitializeMode(sprite, 0);
				sprite->flags = 0;
				Resource_ResetEntry(sprite->vram_block);
				sprite->vram_block = work->vram_block;
				sprite->unknown_1d_0 = 1;
				sprite->tile = (gVramBlockCache[sprite->vram_block].offset >> 5) & 0x3ff;
				sprite->full_color = 0;
				sprite->shape = 1;
				((struct WorldMapOam *)sprite)->size = 2;
				part->detail->field_16 = 0;
			}
		}
	}
	{
		union PairObject   *p = pair[0];
		struct FieldSprite *sp = p->object.actor.sprite;

		p->object.actor.update = Effect_AnimateVerticalNegative;
		sp->priority = parent->object.actor.sprite->priority;
	}
	{
		struct FieldActor  *p = &pair[1]->object.actor;
		struct FieldSprite *sp = p->sprite;

		sp->priority = parent->object.actor.sprite->priority;
		p->update = Effect_AnimateVerticalPositive;
		p->priority_flags = 2;
	}
}

static void SceneState_SetValue140Mode0(void)
{

	Engine_PsynergyBegin(140, 0);
}

static void FieldScene_RunSingleStep(void)
{
	BattleEffect_CleanupSceneObjects();
}

static void FieldScene_RunFourPairedSteps(void)
{
	OverlayObject_UpdateRandomSlotByFrame((s32)Object_GetById(32));
	OverlayObject_UpdateRandomSlotByFrame((s32)Object_GetById(33));
	OverlayObject_UpdateRandomSlotByFrame((s32)Object_GetById(30));
	if (HaidiaArashi_ShakeDone == 0)
	{
		OverlayObject_UpdateRandomSlotByFrame((s32)Object_GetById(29));
	}
}

static void SceneState_SetValue19ThenCall(void)
{

	HaidiaArashi_CycleObjectPalettes((s32)Object_GetById(19));
}

static void HaidiaArashi_CopyLeaderSpritePriority(void)
{
	struct FieldSpritePriority *src;
	struct FieldSpritePriority *dst;
	struct FieldSpritePriority *dst2;

	src = (struct FieldSpritePriority *)Object_GetById(0)->sprite;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	Engine_ActorSetSpritePriority(22, src->priority);
	Engine_ActorSetSpritePriority(8, src->priority);
#else
	dst = (struct FieldSpritePriority *)Object_GetById(22)->sprite;
	dst->priority = src->priority;
	dst2 = (struct FieldSpritePriority *)Object_GetById(8)->sprite;
	dst2->priority = src->priority;
#endif
}

static void EV24_SEARCH(void)
{

	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgHaidiaDoorWontOpen, 1);
	Engine_EventEnd();
}

static void EV19_SEARCH(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgHaidiaSChestValuables, 1);
	Engine_EventEnd();
}

u8 HaidiaArashi_FrameModes[] = {7, 0, 0, 0};

s32 HaidiaArashi_ActorTwentyScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(185), 0, PIXELS(824)),
	SC_SYNC,
	SC_MOVE(PIXELS(179), 0, PIXELS(836)),
	SC_SYNC,
	SC_MOVE(PIXELS(226), 0, PIXELS(840)),
	SC_SYNC,
	SC_MOVE(PIXELS(226), 0, PIXELS(944)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_ARG(1),
	SC_END,
};

s32 HaidiaArashi_ActorTwentyTwoScriptA[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(345), 0, PIXELS(612)),
	SC_SYNC,
	SC_MOVE(PIXELS(311), 0, PIXELS(638)),
	SC_SYNC,
	SC_MOVE(PIXELS(256), 0, PIXELS(608)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 HaidiaArashi_ActorTwentyTwoScriptB[] = {
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, 192),
	SC_ADD(SW_SCALE_Y, 384),
	SC_WAIT(1),
	SC_LOOP(24, SC_TO(1)),
	SC_LABEL(2),
	SC_ADD(SW_SCALE_X, -384),
	SC_ADD(SW_SCALE_Y, -768),
	SC_WAIT(1),
	SC_LOOP(12, SC_TO(2)),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

s32 HaidiaArashi_ActorNineteenScript[] = {
	SC_ARG(2),
	SC_END,
};

s32 HaidiaArashi_LeaderScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(94), 0, PIXELS(564)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(30),
	SC_END,
};

s32 HaidiaArashi_ActorTwentyTwoScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(76), 0, PIXELS(564)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(30),
	SC_END,
};

s32 HaidiaArashi_ActorNineScriptA[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(178), 0, PIXELS(1190)),
	SC_SYNC,
	SC_MOVE(PIXELS(149), 0, PIXELS(1194)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 HaidiaArashi_ActorTwentySixScriptA[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(198), 0, PIXELS(1176)),
	SC_SYNC,
	SC_MOVE(PIXELS(170), 0, PIXELS(1195)),
	SC_SYNC,
	SC_MOVE(PIXELS(149), 0, PIXELS(1194)),
	SC_SYNC,
	SC_MOVE(PIXELS(149), 0, PIXELS(1175)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_ARG(1),
	SC_END,
};

s32 HaidiaArashi_ActorNineScriptB[] = {
	SC_ARG(2),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(30),
	SC_POS(PIXELS(102), 0, PIXELS(1189)),
	SC_WAIT(1),
	SC_MOVE(PIXELS(102), 0, PIXELS(1218)),
	SC_SYNC,
	SC_MOVE(PIXELS(84), 0, PIXELS(1228)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_END,
};

s32 HaidiaArashi_ActorTwentySixScriptB[] = {
	SC_ARG(2),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(30),
	SC_POS(PIXELS(102), 0, PIXELS(1189)),
	SC_SYNC,
	SC_MOVE(PIXELS(102), 0, PIXELS(1218)),
	SC_SYNC,
	SC_MOVE(PIXELS(120), 0, PIXELS(1228)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_END,
};

s32 HaidiaArashi_ActorNineScriptC[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(50), 0, PIXELS(1214)),
	SC_SYNC,
	SC_MOVE(PIXELS(46), 0, PIXELS(1130)),
	SC_SYNC,
	SC_MOVE(PIXELS(67), 0, PIXELS(1100)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 HaidiaArashi_ActorTwentySixScriptC[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(153), 0, PIXELS(1251)),
	SC_SYNC,
	SC_MOVE(PIXELS(177), 0, PIXELS(1357)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_ARG(1),
	SC_END,
};

s32 HaidiaArashi_ActorNineScriptD[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(123), 0, PIXELS(1078)),
	SC_SYNC,
	SC_MOVE(PIXELS(148), 0, PIXELS(1078)),
	SC_SYNC,
	SC_MOVE(PIXELS(148), 0, PIXELS(1064)),
	SC_SYNC,
	SC_MOVE(PIXELS(35), 0, PIXELS(1064)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_ARG(1),
	SC_END,
};

s32 HaidiaArashi_StormRunActions[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(102), 0, PIXELS(1236)),
	SC_SYNC,
	SC_MOVE(PIXELS(148), 0, PIXELS(1256)),
	SC_SYNC,
	SC_MOVE(PIXELS(177), 0, PIXELS(1303)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 HaidiaArashi_ArriveScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(137), 0, PIXELS(1379)),
	SC_SYNC,
	SC_MOVE(PIXELS(114), 0, PIXELS(1416)),
	SC_SYNC,
	SC_MOVE(PIXELS(95), 0, PIXELS(1416)),
	SC_SYNC,
	SC_MOVE(PIXELS(83), 0, PIXELS(1427)),
	SC_SYNC,
	SC_MOVE(PIXELS(154), 0, PIXELS(1430)),
	SC_SYNC,
	SC_MOVE(PIXELS(179), 0, PIXELS(1398)),
	SC_SYNC,
	SC_MOVE(PIXELS(179), 0, PIXELS(1361)),
	SC_SYNC,
	SC_END,
};

s32 HaidiaArashi_GatherPath8[] = {
	SC_MOVE(PIXELS(184), 0, PIXELS(1309)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0xb000),
	SC_END,
};

s32 HaidiaArashi_GatherPath26[] = {
	SC_MOVE(PIXELS(165), 0, PIXELS(1306)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_END,
};

s32 HaidiaArashi_GatherPath0[] = {
	SC_MOVE(PIXELS(183), 0, PIXELS(1325)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_END,
};

s32 HaidiaArashi_GatherPath22[] = {
	SC_MOVE(PIXELS(166), 0, PIXELS(1325)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0xb000),
	SC_END,
};

s32 HaidiaArashi_LookAroundScript[] = {
	SC_CALL(HaidiaArashi_RerollHeadingOnCountdown),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 HaidiaArashi_ActorEightScript[] = {
	SC_CALL(UpdateFixedPointCountdown),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 HaidiaArashi_ActorNineteenRiseScript[] = {
	SC_ARG(2),
	SC_SET(SW_SPEED, 0x30000),
	SC_SET(SW_ACCEL, 0x10000),
	SC_SET(0x23, 0),
	SC_SET(0x24, 1),
	SC_SET(SW_SCALE_X, 0x20000),
	SC_SET(SW_SCALE_Y, 0x20000),
	SC_MOVE(PIXELS(256), PIXELS(128), PIXELS(931)),
	SC_SYNC,
	SC_SE(145),
	SC_SET(0x23, 60),
	SC_SET(0x24, 2),
	SC_MOVE(PIXELS(228), PIXELS(149), PIXELS(957)),
	SC_SYNC,
	SC_SET(0x24, 3),
	SC_MOVE(PIXELS(201), PIXELS(160), PIXELS(984)),
	SC_SYNC,
	SC_SET(0x24, 4),
	SC_MOVE(PIXELS(173), PIXELS(138), PIXELS(1011)),
	SC_SYNC,
	SC_SET(0x24, 5),
	SC_MOVE(PIXELS(146), PIXELS(96), PIXELS(1038)),
	SC_SYNC,
	SC_SE(145),
	SC_SET(SW_SPEED, 0x18000),
	SC_SET(SW_ACCEL, 0x8000),
	SC_SET(0x23, 60),
	SC_SET(0x24, 6),
	SC_MOVE(PIXELS(161), PIXELS(107), PIXELS(1050)),
	SC_SYNC,
	SC_SET(SW_SPEED, 0x20000),
	SC_SET(SW_ACCEL, 0x9999),
	SC_SET(0x24, 7),
	SC_MOVE(PIXELS(177), PIXELS(96), PIXELS(1062)),
	SC_SYNC,
	SC_SET(SW_SPEED, 0x28000),
	SC_SET(SW_ACCEL, 0xcccc),
	SC_SET(0x24, 8),
	SC_MOVE(PIXELS(201), PIXELS(74), PIXELS(1074)),
	SC_SYNC,
	SC_SET(SW_SPEED, 0x30000),
	SC_SET(SW_ACCEL, 0x10000),
	SC_SET(0x24, 9),
	SC_MOVE(PIXELS(225), PIXELS(-64), PIXELS(1086)),
	SC_SYNC,
	SC_SET(0x23, 0),
	SC_SET(0x24, 0),
	SC_END,
};

struct SceneEntrance HaidiaArashi_SceneTable0[] = {
	{ 0, CONDITION_ALWAYS, 167, 0, 1281, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 257, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 113, 0, 303, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 27, 0, 621, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 29, 0, 792, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 458, 0, 1393, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 406, 0, 743, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 262, 0, 821, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 340, 0, 904, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 9, CONDITION_ALWAYS, 326, 0, 1142, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 374, 0, 1254, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 102, 0, 1222, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 12, CONDITION_ALWAYS, 101, 0, 1216, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 13, CONDITION_ALWAYS, 146, 0, 1193, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 15, CONDITION_ALWAYS, 335, 0, 908, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 16, CONDITION_ALWAYS, 181, 0, 1273, FACING_NORTHEAST, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 HaidiaArashi_SceneTable1[] = {
	SCENE_EXITS(3),
	SCENE_EXIT(2, 5, 8),
	SCENE_EXIT(3, 6, 1),
	SCENE_EXIT(4, 6, 2),
	SCENE_EXIT(5, 7, 6),
	SCENE_EXIT(6, 7, 5),
	SCENE_EXIT(7, 8, 6),
	SCENE_EXIT(8, 7, 2),
	SCENE_EXIT(9, 7, 1),
	SCENE_EXIT(10, 7, 1),
	SCENE_EXIT(11, 7, 1),
	SCENE_EXIT(12, 8, 11),
	SCENE_EXIT(13, 8, 12),
	SCENE_EXIT(20, 3, 16),
	SCENE_EXITS_END,
};

struct ScenePlacement HaidiaArashi_SceneTable2[] = {
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x35, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(437), 0, PIXELS(762), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(322), 0, PIXELS(1433), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(144), 0, PIXELS(1070), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(276), 0, PIXELS(1270), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xd2, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(420), 0, PIXELS(619), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x13, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(396), 0, PIXELS(619), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x24, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x3b, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x3c, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x30, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x120, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement HaidiaArashi_LastObjectCall[] = {
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(124), 0, PIXELS(375), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(85), 0, PIXELS(361), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(135), 0, PIXELS(363), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(100), 0, PIXELS(375), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(124), 0, PIXELS(375), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(100), 0, PIXELS(375), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent HaidiaArashi_SceneTable3[] = {
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EV04_TOUCH) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EV06_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TOUCH) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_TALK) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK) },
	{ EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgHaidiaHangOnGarsiaHelpIsOn) },
	{ EVENT_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgHaidiaDontLetGoGarsia) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgHaidiaNoItCantBeKairuIt) },
	{ EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgHaidiaMomDadGarsiaPleaseDontLeave) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgHaidiaRockCannotBeMoved) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaRockCannotBeMoved) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgHaidiaRockCannotBeMoved) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgHaidiaRockCannotBeMoved) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgHaidiaRockCannotBeMoved) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgHaidiaRockCannotBeMoved) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgHaidiaRockCannotBeMoved) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgHaidiaRockCannotBeMoved) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TOUCH) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TOUCH) },
	{ EVENT_TOUCH, 41, CONDITION_ALWAYS, EVENT_SCRIPT(EV41_TOUCH) },
	{ EVENT_TOUCH, 42, CONDITION_ALWAYS, EVENT_SCRIPT(EV42_TOUCH) },
	{ EVENT_TOUCH, 43, CONDITION_ALWAYS, EVENT_SCRIPT(EV43_TOUCH) },
	{ EVENT_TOUCH, 44, CONDITION_ALWAYS, EVENT_SCRIPT(EV44_TOUCH) },
	{ EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(EV30_TOUCH) },
	{ EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(EV31_TOUCH) },
	{ EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(EV32_TOUCH) },
	{ EVENT_TOUCH, 33, CONDITION_ALWAYS, EVENT_SCRIPT(EV33_TOUCH) },
	{ EVENT_TOUCH, 34, CONDITION_ALWAYS, EVENT_SCRIPT(EV34_TOUCH) },
	{ EVENT_TOUCH, 35, CONDITION_ALWAYS, EVENT_SCRIPT(EV35_TOUCH) },
	{ EVENT_TOUCH, 36, CONDITION_ALWAYS, EVENT_SCRIPT(EV36_TOUCH) },
	{ EVENT_TOUCH, 16, 0x87c, EVENT_SCRIPT(EV16_TOUCH) },
	{ EVENT_TOUCH, 17, 0x87f, EVENT_SCRIPT(EV17_TOUCH) },
	{ SEARCH(SEARCH_UNNAMED), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EV24_SEARCH) },
	{ SEARCH(SEARCH_UNNAMED), 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_SEARCH) },
	{ SCENE_EVENTS_END },
};

u16 HaidiaArashi_CellSteps0[] = {
	MAP_CELL_STEP(0, 98, 2, 2, 2), MAP_CELL_STEP(2, 98, 2, 2, 2), MAP_CELL_STEPS_END,
};

u16 HaidiaArashi_CellSteps1[] = {
	MAP_CELL_STEP(0, 96, 2, 2, 2), MAP_CELL_STEP(2, 96, 2, 2, 2), MAP_CELL_STEPS_END,
};

u16 HaidiaArashi_CellSteps2[] = {
	MAP_CELL_STEP(4, 98, 2, 2, 2), MAP_CELL_STEP(6, 98, 2, 2, 2), MAP_CELL_STEPS_END,
};

u16 HaidiaArashi_CellSteps3[] = {
	MAP_CELL_STEP(4, 96, 2, 2, 2), MAP_CELL_STEP(6, 96, 2, 2, 2), MAP_CELL_STEPS_END,
};

u16 HaidiaArashi_CellSteps4[] = {
	MAP_CELL_STEP(0, 96, 2, 2, 2), MAP_CELL_STEP(50, 44, 2, 2, 2), MAP_CELL_STEPS_END,
};

u16 HaidiaArashi_CellSteps5[] = {
	MAP_CELL_STEP(4, 98, 2, 2, 2), MAP_CELL_STEP(44, 108, 2, 2, 2), MAP_CELL_STEPS_END,
};
