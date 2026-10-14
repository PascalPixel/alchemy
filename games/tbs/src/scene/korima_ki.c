#include "battle_effect_runtime.h"
#include "game_state.h"
#include "unit/scene/korima_ki.h"
#include "unit/scene/common/korima_palette.h"
#include "unit/object/object.h"
#include "unit/object/psyfx.h"
#include "unit/game/game.h"
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
#include "resource.h"
#include "korima_ki.h"
#include "types.h"
#include "call.h"
#include "field_event.h"
#include "engcall.h"
#include "scene_ids.h"
#include "palette_scene.h"
#include "io_reg.h"
#include "field_scene.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"

// 前方宣言
static void RunEventScript01(void);
static void PaletteScene_RunActorTransitionSequence(void);
static void PaletteScene_SpawnEffect(void);
static void PaletteScene_AdvanceTransition(void);
static void KorimaKi_PlayGesture(s32 actor, s32 gesture);
static void KorimaKi_SpawnOrbitSparks(void);
static void PaletteScene_SetRecordValue(s32 key, s32 value);

s32 gKorimaKiSparkOrigin[3] __attribute__((section(".bss")));
s32 gKorimaKiSparkCount __attribute__((section(".bss")));
s32 gKorimaKiSparkSound __attribute__((section(".bss")));
s32 gKorimaKiTransitionStep __attribute__((section(".bss")));

u8 *KorimaKi_GetEntrances(void)
{
	return gKorimaKiEntrances;
}

u8 *KorimaKi_GetRegions(void)
{
	return gKorimaKiRegions;
}

u8 *KorimaKi_GetExits(void)
{
	return gKorimaKiExits;
}

u8 *KorimaKi_GetPlacements(void)
{
	return gKorimaKiPlacements;
}

static void PaletteScene_Initialize(void)
{
	struct EventWork *scene;

	scene = gEventWork;
	Engine_EventBegin();
	Engine_ActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 0);
	Engine_EventRequestExit(scene->touched_trigger);
	Engine_EventEnd();
}

u8 *KorimaKi_GetEvents(void)
{
	return gKorimaKiEvents;
}

static void EV13_TALK(void)
{
	Engine_EventBegin();
	KorimaKi_PlayGesture(11, 1);
	if (Engine_GameFlagIsSet(0x845) != 0)
	{
		Engine_EventSetMessage(MsgKorimaForestKolimaAlive);
		Engine_EventShowMessage(9, 0);
	}
	else if (GameFlag_IsSet(0x84c) != 0)
	{
		Engine_EventSetMessage(MsgKorimaLeaveBeforeForest);
		Engine_EventShowMessage(9, 0);
	}
	else
	{
		Engine_EventSetMessage(MsgKorimaWho);
		Engine_EventShowMessageAndWait(9, 0, 20);
		KorimaKi_PlayGesture(11, 0);
		Engine_EventWait(60);
		KorimaKi_PlayGesture(11, 1);
		Engine_EventShowMessageAndWait(9, 0, 10);
		Engine_ActorSetAnimationAndWait(0, 3);
		Engine_EventWait(40);
		Engine_EventShowMessage(9, 0);
		KorimaKi_PlayGesture(11, 0);
		Engine_EventWait(80);
		Engine_EventShowMessageAndWait(9, 0, 20);
		KorimaKi_PlayGesture(11, 1);
		Engine_EventShowMessageAndWait(9, 0, 20);
		Engine_GameFlagSet(0x84c);
	}
	KorimaKi_PlayGesture(11, 0);
	Engine_EventEnd();
}

static void KorimaKi_TalkToToreto(void)
{
	s32 record;

	Engine_EventBegin();
	if (GameFlag_IsSet(0x845) != 0)
	{
		KorimaKi_PlayGesture(10, 1);
		Engine_EventSetMessage(MsgKorimaNowHaveSuchPowerAxe);
		Event_ShowMessage(8, 0);
		KorimaKi_PlayGesture(10, 0);
	}
	else
	{
		if (GameFlag_IsSet(0x844) != 0)
		{
			KorimaKi_PlayGesture(10, 1);
			Engine_EventSetMessage(MsgKorimaSilence2);
			Event_ShowMessage(8, 0);
			KorimaKi_PlayGesture(10, 0);
			record = PartyInventory_FindOwner(184);
			if (record != -1)
			{
				u16 *target = &gEventWork->unknown_172;
				u16	 shown = 1;

				*target = shown;
			}
		}
		else
		{
			Engine_EventSetMessage(MsgKorimaSilence);
			Event_ShowMessage(8, 0);
			ColorBuffer_ApplyTarget(0x406218, 1);
			Engine_ColorBufferInterpolate(20);
			Engine_TaskWait(40);
			Event_ShowMessageAndWait(0x200e, 0, 10);
			Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			Event_ShowMessage(0x200e, 0);
			ColorBuffer_ApplyTarget(0x10000, 1);
			Engine_ColorBufferInterpolate(20);
			Engine_TaskWait(40);
		}
	}
	Engine_EventEnd();
}

static void EV13_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x845) != 0)
	{
		Engine_EventSetMessage(MsgKorimaMustHorribleBeyondRiverAm);
	}
	else
	{
		Engine_EventSetMessage(MsgKorimaHealingWatersMercuryLighthouseMight);
	}
	Engine_EventShowMessage(9, 0);
	Engine_EventEnd();
}

static void EV12_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x845) != 0)
	{
		Engine_EventSetMessage(MsgKorimaKnowCannotStopButPlease);
	}
	else
	{
		Engine_EventSetMessage(MsgKorimaPeopleKolimaForgiveMe);
	}
	Engine_EventShowMessage(8, 0);
	Engine_EventEnd();
}

static void EV12_SCRIPT(void)
{
	Engine_EventBegin();
	Battle_ResetEffectCounter();
	if (Engine_GameFlagIsSet(0x844) == 0)
	{
		RunEventScript01();
	}
	else
	{
		PaletteScene_RunActorTransitionSequence();
	}
	Engine_EventEnd();
}

static void RunEventScript01(void)
{

	struct FieldActor *rec8;

	rec8 = Actor_Get(ACTOR_PARTY_LEADER);
	Engine_ActorFaceDirection(0, FACING_NORTH, 0);
	ColorBuffer_ApplyTarget(0x406218, 1);
	Engine_ColorBufferInterpolate(20);
	Engine_TaskWait(40);
	Event_PlayCue(17);
	gKorimaKiSparkSound = 1;
	FieldScheduler_AddCallback(PaletteScene_SpawnEffect, 0xc80);
	Engine_TaskWait(30);
	gKorimaKiSparkSound = 0;
	Camera_MoveTo(0x1480000, -1, 0xeb0000, 1);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
	Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x20000);
	Event_PlayCue(133);
	rec8->velocity_y = 0x50000;
	rec8->gravity = 0x4000;
	rec8->bounce = 0xa000;
	Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x14f, 0x102);
	while (rec8->velocity_y >= 0)
	{
		Engine_TaskWait(1);
	}
	do
	{
		Engine_TaskWait(1);
	} while (rec8->velocity_y <= 0);
	Event_PlayCue(161);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
	Engine_EventWait(120);
	Engine_TaskRemoveCallback(PaletteScene_SpawnEffect);
	Engine_TaskWait(40);
	rec8->bounce = 0x4000;
	{
		struct FieldActor *record = Actor_Get(ACTOR_PARTY_LEADER);
		record->steering_flags = ACTOR_FACES_MOTION | record->steering_flags;
	}
	Engine_EventWait(80);
	Engine_EventSetMessage(MsgKorimaControlTretsHeartShallNot);
	Event_ShowMessageAndWait(0x200e, 0, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(20);
	Event_ShowMessage(0x200e, 0);
	Audio_PlayCueFromEventWork();
	ColorBuffer_ApplyTarget(0x10000, 1);
	Engine_ColorBufferInterpolate(20);
	Engine_TaskWait(40);
	{
		s32 shown = 0xc000;

		rec8->facing = shown;
	}
	rec8->gravity = 0x10000;
	rec8->bounce = 0x4000;
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(40);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(20);
}

static void PaletteScene_RunActorTransitionSequence(void)
{
	s32				   actorThreeEnabled;
	struct FieldActor *object;
	s32				  *transitionState;
	s32				   cycle;
	s32				   effectCallback;
	const s32		  *finalActions;

	actorThreeEnabled = GameFlag_IsSet(3);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x148, 212);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
	Event_PlayCue(17);
	Engine_MessageShowCentered(MsgKorimaWaterHermesSeepedIntoTret, 1);
	Actor_SetSpeed(GERALD, 0x10000, 0x8000);
	Actor_SetSpeed(IWAN, 0x10000, 0x8000);
	object = Actor_Get(ACTOR_PARTY_LEADER);
	if (object != 0)
	{
		Actor_SetPosition(GERALD, object->x.fixed, object->z.fixed);
	}
	object = Actor_Get(ACTOR_PARTY_LEADER);
	if (object != 0)
	{
		Actor_SetPosition(IWAN, object->x.fixed, object->z.fixed);
	}
	Engine_ActorEnableActionCallback(GERALD, SceneAction_ActorOneEntry);
	Engine_ActorEnableActionCallback(IWAN, SceneAction_ActorTwoEntry);
	if (actorThreeEnabled != 0)
	{
		Actor_SetSpeed(MEARI, 0x10000, 0x8000);
		object = Actor_Get(ACTOR_PARTY_LEADER);
		if (object != 0)
		{
			Actor_SetPosition(MEARI, object->x.fixed, object->z.fixed);
		}
		Engine_ActorEnableActionCallback(MEARI, SceneAction_ActorThreeEntry);
	}
	Object_RefreshSelectorById(2);
	Engine_EventWait(40);
	KorimaPalette_Restore(0);
	Engine_ColorBufferInterpolate(32);
	Engine_TaskWait(40);
	transitionState = &gKorimaKiTransitionStep;
	*transitionState = 0;
	FieldScheduler_AddCallback(PaletteScene_AdvanceTransition, 0xc80);
	Engine_EventWait(40);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 20);
	Camera_SetSpeed(0x33333, 0x6666);
	Camera_MoveTo(0x1000000, -1, 0xfe0000, 1);
	Engine_CameraWaitForMove();
	Event_PlayCue(246);
	Engine_EventWait(40);
	Actor_FaceDirection(IWAN, FACING_SOUTHEAST, 20);
	Camera_MoveTo(0x19d0000, -1, 0x1050000, 1);
	Engine_CameraWaitForMove();
	Event_PlayCue(246);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(MEARI, FACING_SOUTH, 20);
	Camera_MoveTo(0x1460000, -1, 0x1800000, 1);
	Engine_CameraWaitForMove();
	Event_PlayCue(246);
	if (*transitionState != 24)
	{
		do
		{
			Engine_TaskWait(1);
		} while (*transitionState != 24);
	}
	Engine_TaskRemoveCallback(PaletteScene_AdvanceTransition);
	Engine_TaskWait(10);
	cycle = 0;
	do
	{
		KorimaPalette_Restore(0);
		Engine_ColorBufferInterpolate(6);
		Engine_TaskWait(6);
		KorimaPalette_Restore(1);
		Engine_ColorBufferInterpolate(6);
		cycle = (cycle + 1);
		Engine_TaskWait(6);
	} while ((u32)cycle <= 3);
	KorimaPalette_Restore(0);
	Engine_ColorBufferInterpolate(40);
	Engine_TaskWait(80);
	Camera_MoveTo(0x1480000, 0x80000, 0xd40000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	KorimaKi_PlayGesture(10, 1);
	Engine_EventWait(40);
	Event_PlayCue(7);
	Engine_EventSetMessage(MsgKorimaFeelGreatPowerSpreadingThrough);
	Event_ShowMessage(8, 0);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorStartRepeatedMotion(MEARI, 2);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_FaceDirection(MEARI, FACING_NORTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 20);
	KorimaKi_PlayGesture(10, 2);
	Engine_EventWait(20);
	KorimaKi_PlayGesture(10, 3);
	Engine_EventWait(40);
	KorimaKi_PlayGesture(10, 1);
	Engine_EventWait(20);
	Event_ShowMessage(8, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
	Actor_ShowEmote(GERALD, 0x105, 0);
	Actor_ShowEmote(MEARI, 0x105, 0);
	Actor_ShowEmote(IWAN, 0x105, 40);
	Camera_MoveTo(0xea0000, 0, 0xe80000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	KorimaKi_PlayGesture(11, 1);
	Engine_EventWait(40);
	KorimaKi_PlayGesture(11, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x4009, 0, 20);
	KorimaKi_PlayGesture(11, 2);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_SOUTHWEST, 20);
	KorimaKi_PlayGesture(11, 3);
	Engine_EventWait(20);
	KorimaKi_PlayGesture(11, 2);
	Engine_EventWait(20);
	KorimaKi_PlayGesture(11, 3);
	Event_ShowMessageAndWait(0x4009, 0, 10);
	KorimaKi_PlayGesture(10, 0);
	Engine_EventWait(20);
	Event_ShowMessage(0x8008, 0);
	KorimaKi_PlayGesture(10, 1);
	Engine_EventWait(20);
	Event_OpenMessage(0x8008, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
	Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTH, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Event_ShowMessage(0x4009, 0);
		Event_ShowMessage(0x8008, 0);
	}
	else
	{
		gEventWork->message += 2;
		Actor_ShowEmote(MEARI, 0x103, 0);
		Actor_ShowEmote(GERALD, 0x103, 0);
		Actor_ShowEmote(IWAN, 0x103, 40);
		Engine_ActorSetAnimation(GERALD, 4);
		Event_ShowMessage(GERALD, 0);
		if (actorThreeEnabled != 0)
		{
			Engine_ActorRunRepeatedMotion(MEARI, 2);
			Event_ShowMessage(MEARI, 0);
		}
		else
		{
			gEventWork->message += 1;
		}
		Engine_ActorSetAnimationAndWait(IWAN, 3);
		Event_ShowMessage(IWAN, 0);
		Event_ShowMessage(0x4009, 0);
		Event_ShowMessage(0x8008, 0);
	}
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Camera_MoveTo(0x1480000, 0x80000, 0xd40000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	KorimaKi_PlayGesture(10, 0);
	Engine_EventWait(20);
	KorimaPalette_Restore(0);
	Engine_ColorBufferInterpolate(1);
	Engine_TaskWait(1);
	ColorBuffer_ApplyTarget(0x406218, 1);
	Engine_ColorBufferInterpolate(40);
	Engine_EventWait(60);
	gKorimaKiSparkCount = 0;
	gKorimaKiSparkOrigin[0] = 0x1480000;
	gKorimaKiSparkOrigin[1] = 0x300000;
	effectCallback = (s32)KorimaKi_SpawnOrbitSparks;
	gKorimaKiSparkOrigin[2] = 0xcd0000;
	FieldScheduler_AddCallback(effectCallback, 0xc80);
	Engine_EventWait(100);
	Engine_TaskRemoveCallback(effectCallback);
	ColorBuffer_ApplyTarget(0x7fff, 0);
	Engine_ColorBufferInterpolate(60);
	Engine_EventWait(100);
	KorimaPalette_Restore(0);
	Engine_ColorBufferInterpolate(20);
	Engine_EventWait(40);
	KorimaKi_PlayGesture(10, 1);
	Engine_EventWait(10);
	Engine_EventSetMessage(MsgKorimaShouldDoPeopleKolimaCursed);
	Event_ShowMessage(0x8008, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Camera_MoveTo(0xea0000, 0, 0xe80000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Event_ShowMessage(0x4009, 0);
	Event_ShowMessageAndWait(0x8008, 0, 10);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 10);
	Event_OpenMessage(GERALD, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Actor_ShowEmote(GERALD, 0x102, 40);
	}
	else
	{
		Engine_ActorSetAnimationAndWait(GERALD, 4);
		gEventWork->message += 1;
	}
	Event_ShowMessage(GERALD, 0);
	KorimaKi_PlayGesture(10, 4);
	Engine_EventWait(20);
	Engine_EventSetMessage(MsgKorimaWasIndeedAngryPeopleHad);
	Event_ShowMessage(0x8008, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Event_ShowMessage(0x8008, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	KorimaKi_PlayGesture(10, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x8008, 0, 20);
	KorimaKi_PlayGesture(11, 0);
	Event_ShowMessageAndWait(0x4009, 0, 20);
	KorimaKi_PlayGesture(11, 3);
	Engine_EventWait(40);
	KorimaKi_PlayGesture(11, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x4009, 0, 20);
	KorimaKi_PlayGesture(10, 2);
	Engine_EventWait(20);
	Event_ShowMessage(0x8008, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(GERALD, 0x102, 0);
	Actor_ShowEmote(MEARI, 0x102, 0);
	Actor_ShowEmote(IWAN, 0x102, 80);
	KorimaKi_PlayGesture(11, 5);
	Engine_EventWait(60);
	KorimaKi_PlayGesture(11, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x4009, 0, 20);
	KorimaKi_PlayGesture(10, 5);
	Engine_EventWait(40);
	KorimaKi_PlayGesture(10, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x4008, 0, 20);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Actor_FaceDirection(GERALD, FACING_WEST, 10);
	Event_ShowMessage(GERALD, 0);
	Actor_FaceDirection(IWAN, FACING_WEST, 20);
	Event_ShowMessage(0x8002, 0);
	KorimaKi_PlayGesture(11, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x4009, 0, 20);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	KorimaKi_PlayGesture(10, 1);
	Event_ShowMessageAndWait(0x8008, 0, 10);
	KorimaKi_PlayGesture(10, 2);
	Engine_EventWait(20);
	KorimaKi_PlayGesture(11, 3);
	Engine_EventWait(40);
	KorimaKi_PlayGesture(11, 0);
	Engine_EventWait(20);
	KorimaPalette_Restore(0);
	Engine_ColorBufferInterpolate(1);
	Engine_TaskWait(1);
	ColorBuffer_ApplyTarget(0x406218, 1);
	Engine_ColorBufferInterpolate(40);
	Engine_EventWait(60);
	gKorimaKiSparkCount = 0;
	gKorimaKiSparkOrigin[0] = 0x880000;
	gKorimaKiSparkOrigin[1] = 0x140000;
	effectCallback = (s32)KorimaKi_SpawnOrbitSparks;
	gKorimaKiSparkOrigin[2] = 0x1020000;
	FieldScheduler_AddCallback(effectCallback, 0xc80);
	Engine_EventWait(100);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 40);
	Engine_ActorStartRepeatedMotion(IWAN, 1);
	Actor_ShowEmote(IWAN, 0x100, 20);
	Event_ShowMessageAndWait(0x8002, 0, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	KorimaKi_PlayGesture(10, 4);
	Engine_EventWait(20);
	Event_ShowMessage(0x8008, 0);
	Actor_ShowEmote(IWAN, 0x101, 60);
	Actor_FaceDirection(IWAN, FACING_NORTH, 10);
	Event_ShowMessageAndWait(0x8002, 0, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(MEARI, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 20);
	Event_ShowMessageAndWait(0x8008, 0, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 120);
	Engine_TaskRemoveCallback(effectCallback);
	Engine_EventWait(60);
	KorimaPalette_Restore(0);
	Engine_ColorBufferInterpolate(40);
	KorimaKi_PlayGesture(10, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x8008, 0, 20);
	KorimaKi_PlayGesture(11, 3);
	Event_ShowMessage(0x4009, 0);
	Event_ShowMessage(0x8008, 0);
	KorimaKi_PlayGesture(11, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x4009, 0, 10);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorStartRepeatedMotion(MEARI, 2);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	KorimaKi_PlayGesture(10, 1);
	Event_OpenMessage(0x8008, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		gEventWork->message += 1;
	}
	Engine_EventWait(10);
	KorimaKi_PlayGesture(10, 2);
	Engine_EventWait(20);
	KorimaKi_PlayGesture(11, 3);
	Engine_EventWait(40);
	KorimaKi_PlayGesture(10, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x8008, 0, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(GERALD, FACING_EAST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTH, 0);
	Actor_FaceDirection(IWAN, FACING_WEST, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Event_PlayCue(17);
	finalActions = SceneAction_GroupFinish;
	Engine_ActorEnableActionCallback(GERALD, finalActions);
	if (actorThreeEnabled != 0)
	{
		Engine_ActorEnableActionCallback(MEARI, finalActions);
	}
	Object_SetActionCallbackAndRefreshById(2, (s32)finalActions);
	KorimaKi_PlayGesture(10, 4);
	KorimaKi_PlayGesture(10, 4);
	Engine_EventWait(20);
	Engine_EventSetMessage(MsgKorimaOweGreatDebtHaveSaved);
	Event_ShowMessage(0x8008, 0);
	KorimaKi_PlayGesture(11, 4);
	KorimaKi_PlayGesture(11, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x4009, 0, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	GameFlag_Set(0x845);
	Event_PlayCue(1);
	PaletteScene_SetRecordValue(184, 185);
}

s32 KorimaKi_PrepareActors(void)
{
	struct FieldActor *first;
	struct FieldActor *third;
	struct FieldActor *second;

	first = Object_GetById(10);
	third = Object_GetById(14);
	second = Object_GetById(11);
	Engine_TaskWait(1);
	Engine_ActorSetChildValue(14, 15);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	gGameState.retreat_scene = (s32)&SceneId_KorimaHashi;
	gGameState.retreat_entrance = 4;
	if (!Engine_GameFlagIsSet(0x845))
		SceneEffect_AdjustPaletteColors(3);
	Object_GetById(8)->radius = 6;
	Object_GetById(9)->radius = 6;
	Object_GetById(12)->radius = 6;
	Object_GetById(13)->radius = 6;
	Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
	Engine_ActorSetSpritePriority(8, 2);
	Engine_ActorSetSpritePriority(14, 2);
	Engine_ActorSetSpritePriority(9, 2);
	first->motion_flags = 0;
	first->y.fixed = 0x1c0000;
	second->motion_flags = 0;
	second->y.fixed = 0x1c0000;
	third->motion_flags = 0;
	third->y.fixed = 0x1c0000;
	Engine_ActorSetAnimation(9, 3);
	Engine_ActorSetAnimation(8, 3);
	Object_GetById(8)->collision_flags |= 8;
	Object_GetById(9)->collision_flags |= 8;
	Object_GetById(10)->collision_flags |= 8;
	Object_GetById(11)->collision_flags |= 8;
	Object_GetById(14)->collision_flags |= 8;
	return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/korima_grow.h"

static void PaletteScene_SpawnEffect(void)
{

	struct PaletteEffect *effect;
	struct EffectSprite	 *sprite;
	s32					  phase;
	s32					  effect_flags;
	s32					  sprite_flags;
	s32					  spawn_x = 0x01460000;
	s32					  spawn_y = 0x00200000;
	s32					  spawn_z = 0x00c00000;
	s32					  target_x = 0x01460000;
	s32					  target_z = 0x00f00000;

	phase = gFrameCount & 3;
	if (phase != 0)
		return;
	if (gKorimaKiSparkSound != 0)
		Engine_AudioPlayCue(200);
	effect = (struct PaletteEffect *)Engine_ObjectCreate(26, spawn_x, spawn_y, spawn_z);
	if (effect == 0)
		return;
	sprite = effect->sprite;
	sprite->state = phase;
	effect_flags = 0xfe;
	effect_flags &= effect->flags;
	effect->flags = effect_flags;
	sprite_flags = ~12;
	sprite_flags &= sprite->flags;
	sprite_flags |= 4;
	sprite->flags = sprite_flags;
	effect->progress = 0x1999;
	effect->rate_x = 0x40000;
	effect->rate_y = 0x40000;
	effect->mode = phase;
	Object_SetMode(effect, 2);
	Engine_ObjectSetPosition((struct FieldActor *)effect, target_x, 0, target_z);
	Engine_ObjectSetScript(effect, gKorimaKiEffectScript);
}

static void PaletteScene_AdvanceTransition(void)
{
	s32 step = gKorimaKiTransitionStep;

	if (step == 0)
	{
		KorimaPalette_Restore(0);
		Engine_ColorBufferInterpolate(20);
	}
	else if (step == 20)
	{
		KorimaPalette_Restore(1);
		Engine_ColorBufferInterpolate(8);
	}
	step = gKorimaKiTransitionStep + 1;
	gKorimaKiTransitionStep = step;
	if (step == 30)
	{
		gKorimaKiTransitionStep = 0;
	}
}

static void KorimaKi_PlayGesture(s32 actor, s32 gesture)
{
	if (actor == 10)
	{
		switch (gesture)
		{
		case 0:
			Engine_ActorSetAnimation(8, 1);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 3);
			break;
		case 1:
			Engine_ActorSetAnimation(8, 1);
			break;
		case 2:
			Engine_ActorSetAnimation(8, 1);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 5);
			break;
		case 3:
			Engine_ActorSetAnimation(8, 1);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 4);
			break;
		case 4:
			Engine_ActorSetAnimation(8, 1);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 3);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 1);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 3);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 1);
			break;
		case 5:
			Engine_ActorSetAnimation(8, 1);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 2);
			break;
		case 6:
			Engine_ActorSetAnimation(8, 6);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 8);
			break;
		case 8:
			Engine_ActorSetAnimation(8, 6);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 9);
			break;
		case 9:
			Engine_ActorSetAnimation(8, 6);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 10);
			break;
		case 10:
			Engine_ActorSetAnimation(8, 6);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 8);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 6);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 8);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(8, 6);
			break;
		case 7:
		case 11:
			Engine_ActorSetAnimation(8, 6);
			break;
		}
	}
	else
	{
		switch (gesture)
		{
		case 0:
			Engine_ActorSetAnimation(9, 1);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(9, 3);
			break;
		case 1:
			Engine_ActorSetAnimation(9, 1);
			break;
		case 2:
			Engine_ActorSetAnimation(9, 1);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(9, 5);
			break;
		case 3:
			Engine_ActorSetAnimation(9, 1);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(9, 4);
			break;
		case 4:
			Engine_ActorSetAnimation(9, 1);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(9, 3);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(9, 1);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(9, 3);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(9, 1);
			break;
		case 5:
			Engine_ActorSetAnimation(9, 1);
			Engine_TaskWait(6);
			Engine_ActorSetAnimation(9, 2);
			break;
		}
	}
	Engine_TaskWait(12);
}

static void PaletteScene_AdvanceOrbit(struct OrbitingPaletteEffect *effect)
{
	s32 position[3];
	s32 step = effect->step;
	s32 heading;

	if (step <= 119)
	{
		position[0] = effect->anchor_x;
		position[1] = effect->anchor_y;
		position[2] = effect->anchor_z;
		heading = effect->heading;
		Vector_AddPolarOffset(step << 16, step * 768 + heading, position);
		effect->x = position[0];
		effect->y = position[1];
		effect->z = position[2];
		effect->angle_x += 0x147;
		effect->angle_y += 0x147;
		effect->step++;
	}
	else
	{
		Resource_ResetEntry(effect->owner[0x1c]);
		Engine_ObjectDispatchRelease(effect);
	}
}

static void KorimaKi_SpawnOrbitSparks(void)
{
	struct FieldActor *spark;
	s32				   previous;
	u32				   i;

	previous = 0;
	switch (gKorimaKiSparkCount)
	{
	case 0:
	case 10:
	case 20:
	case 30:
	case 40:
		Engine_AudioPlayCue(220);
		for (i = 0; i <= 5; i++)
		{
			spark = Engine_ObjectCreate(
				0x11d, gKorimaKiSparkOrigin[0], gKorimaKiSparkOrigin[1], gKorimaKiSparkOrigin[2]);
			if (spark != 0)
			{
				previous = Object_ReplaceResourceEntry(spark->sprite, previous);
				spark->motion_flags = 0;
				spark->sprite->priority = 1;
				Engine_ActorSetSpriteFlags(spark, 0);
				Object_SetMode(spark, 1);
				spark->timer = 0;
				spark->work = (i * 60 << 16) / 360;
				spark->target_x = gKorimaKiSparkOrigin[0];
				spark->target_y = gKorimaKiSparkOrigin[1];
				spark->target_z = gKorimaKiSparkOrigin[2];
				spark->speed = 0x19999;
				spark->update = (void (*)(union FieldObject *))PaletteScene_AdvanceOrbit;
			}
		}
		break;
	}
	if (++gKorimaKiSparkCount > 120)
	{
		gKorimaKiSparkCount = 0;
	}
}

static void PaletteScene_SetRecordValue(s32 key, s32 value)
{
	s32 slot = PartyInventory_FindOwner(key);

	if (slot != -1)
	{
		s32 index = Inventory_Find(slot, key);

		if (index != -1)
		{
			((struct PaletteSceneRecord *)UnitGet(slot))->values[index] = value;
		}
	}
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/korima_palette.h"

s32 SceneAction_ActorOneEntry[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(312), 0, PIXELS(230)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(1),
	SC_END,
};

s32 SceneAction_ActorTwoEntry[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(346), 0, PIXELS(224)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(1),
	SC_END,
};

s32 SceneAction_ActorThreeEntry[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(333), 0, PIXELS(240)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(1),
	SC_END,
};

s32 SceneAction_GroupFinish[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(328), 0, PIXELS(212)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

struct SceneEntrance gKorimaKiEntrances[] = {
	{ 0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, 0, 0, 512, 448, 0 },
	{ 1, CONDITION_ALWAYS, 136, 0, 424, FACING_NORTH, 0, 0, 0, 512, 448, 0 },
	{ 2, CONDITION_ALWAYS, 328, 16, 200, FACING_SOUTH, 0, 0, 0, 512, 448, 0 },
	{ SCENE_TABLE_END },
};

s16 gKorimaKiRegions[] = {
	320, 88, 176, 336, 104, 192, -1, 2,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gKorimaKiExits[] = {
	SCENE_EXITS(44),
	SCENE_EXIT(1, 43, 2),
	SCENE_EXIT(2, 45, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement gKorimaKiPlacements[] = {
	{ 0x47, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x38, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(270), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(136), 0, PIXELS(270), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(327), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(136), 0, PIXELS(272), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x37, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gKorimaKiEvents[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ 0xb814, 12, 0x845, EVENT_SCRIPT(EV12_SCRIPT) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(KorimaKi_TalkToToreto) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40c, 0x844, EVENT_SCRIPT(KorimaKi_TalkToToreto) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_PSYNERGY) },
	{ SCENE_EVENTS_END },
};

s32 gKorimaKiEffectScript[] = {
	SC_ADD(SW_SCALE_X, 0x1999),
	SC_CALL(SceneEffect_AdvanceAngleUntilIdle),
	SC_LOOP(SC_FOREVER, SC_TOP),
};
