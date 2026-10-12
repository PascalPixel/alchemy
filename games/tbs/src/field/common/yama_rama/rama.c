#include "game_state.h"
#include "unit/field/common/yama_rama/rama.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
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
#include "field_event.h"
#include "field_scene.h"
#include "scene_ids.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "field_effect.h"

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
	/* FAKEMATCH: forwarding inline: called directly, ConfigureAndPlaceActorFourteen has 2 instructions different (argument load order) */
	Engine_ActorSetPosition(actor, x, y);
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{
	gEventWork->message += amount;
}

static s32 EventScript_PrepareActorRenderFlags(union FieldObject *object)
{
	object->actor.priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
	object->actor.sprite->priority = 3;
	object->actor.sprite->second_priority = 3;
	return 0;
}

static s32 OverlayObject_SetFacingTowardObject10(void *self)
{
	struct FieldActor *actor = self;
	struct FieldActor *obj;

	obj = Object_GetById(0xA);
	actor->facing = ArcTan2(obj->z.fixed - actor->z.fixed, obj->x.fixed - actor->x.fixed);
	return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	if (gGameState.scene == (s32)&SceneId_YamaRama1)
	{
		return YamaRama_TempleEntrances;
	}
	return YamaRama_Entrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return YamaRama_Exits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	if (gGameState.scene == (s32)&SceneId_YamaRama1)
	{
		return YamaRama_TemplePlacements;
	}
	return YamaRama_Placements;
}

static void SceneDialogue_RunMessage1958Step(void)
{

	struct EventWork *work;

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgYamaYoungWarriorsDoComeFrom);
	Engine_EventOpenMessage(10, 0);

	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		Engine_EventWait(20);
		Engine_EventShowMessage(10, 0);
	}
	else
	{
		work = gEventWork;
		work->message += 1;
		Engine_EventAskYesNo(10, 0);
	}

	Engine_EventEnd();
}

static void EV11_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgYamaDoKnowMeditation);
	Engine_EventAskYesNo(11, 0);
	Engine_EventEnd();
}

static void EV13_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgYamaDidKnowMasterHamaGreatest);
	Engine_EventAskYesNo(13, 0);
	Engine_EventEnd();
}

static void EV02_TOUCH(void)
{
	Event_PlayCue(188);
	Map_AnimateCells(YamaRama_BoulderCells, 67, 6);
	Object_GetById(0)->motion_flags = 0;
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
	Engine_EventWait(16);
	Engine_EventRequestExit(2);
}

static void FieldScene_RunScene3a2SequenceA(void)
{
	void Event_ShowMessageAndWait();

	Engine_EventBegin();
	Actor_SetPosition(8, 0x880000, 0xa80000);
	Actor_FaceDirection(8, 0x5000, 0);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x900000, 0xc80000);
	Actor_SetPosition(GERALD, 0xa00000, 0xc00000);
	Actor_SetPosition(IWAN, 0x800000, 0xc80000);
	Actor_SetPosition(MEARI, 0x700000, 0xc00000);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xa000, 0);
	Actor_FaceDirection(IWAN, 0xc000, 0);
	Actor_FaceDirection(MEARI, 0xe000, 0);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(60);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgYamaAdeptsLetMeThankAgain);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(8, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimationAndWait(MEARI, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Engine_ActorRunRepeatedMotion(8, 2);
	Actor_FaceDirection(8, 0x5000, 20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(IWAN, 0x102, 60);
	Engine_EventWait(120);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(GERALD, 0x6000, 20);
	Event_OpenMessage(GERALD, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Actor_FaceDirection(MEARI, 0, 20);
		Actor_ShowEmote(MEARI, 0x101, 60);
		Event_ShowMessageAndWait(MEARI, 0, 20);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
		Actor_FaceDirection(IWAN, 0xe000, 0);
		Engine_ActorStartRepeatedMotion(GERALD, 2);
		Actor_SetAttachedEffect(GERALD, 0x102);
		Engine_EventWait(60);
		Actor_FaceDirection(GERALD, 0x8000, 20);
		Event_ShowMessageAndWait(GERALD, 0, 20);
		gEventWork->message += 2;
	}
	else
	{
		gEventWork->message += 2;
		Engine_EventWait(20);
		Actor_FaceDirection(MEARI, 0, 20);
		Engine_ActorSetAnimationAndWait(MEARI, 3);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(MEARI, 0, 20);
		Actor_ShowEmote(GERALD, 0x102, 60);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
		Actor_FaceDirection(IWAN, 0xe000, 0);
		Event_ShowMessageAndWait(GERALD, 0, 20);
	}
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xa000, 0);
	Actor_FaceDirection(IWAN, 0xc000, 0);
	Actor_FaceDirection(MEARI, 0xe000, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(8, 0x3000, 20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(8, 3);
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
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(8, 0xc000, 30);
	Event_PlayCue(188);
	Map_AnimateCells(YamaRama_BoulderCells, 67, 6);
	Actor_SetSpeed(8, 0xcccc, 0x6666);
	Actor_WalkToAndWait(8, 136, 136);
	Actor_SetPosition(8, 0, 0);
	Event_PlayCue(188);
	Map_AnimateCells(YamaRama_BoulderCellsBack, 67, 6);
	Engine_EventWait(60);
	BattleFx_PlayQueuedSound();
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(GERALD, 0x6000, 20);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(MEARI, 0x2000, 20);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 20);
	Event_AskYesNo(GERALD, 0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	Engine_ActorRunRepeatedMotion(MEARI, 1);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(IWAN, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Engine_ActorRunRepeatedMotion(MEARI, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Actor_FaceDirection(IWAN, 0xb000, 20);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Actor_ShowEmote(GERALD, 0x102, 60);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_FaceDirection(IWAN, 0xe000, 20);
	Engine_ActorSetAnimationAndWait(IWAN, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(MEARI, 0x102, 60);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Actor_FaceDirection(IWAN, 0xa000, 20);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	Engine_ActorRunRepeatedMotion(MEARI, 1);
	Engine_EventWait(20);
	Actor_SetSpeed(IWAN, 0x8000, 0x4000);
	Actor_WalkToAndWait(IWAN, 128, 184);
	Actor_FaceDirection(IWAN, 0x4000, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xa000, 0);
	Actor_FaceDirection(MEARI, 0xe000, 20);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	Actor_SetSpeed(GERALD, 0x8000, 0x4000);
	Actor_SetSpeed(MEARI, 0x8000, 0x4000);
	Actor_WalkTo(GERALD, 144, 200);
	Actor_WalkTo(IWAN, 144, 200);
	Actor_WalkTo(MEARI, 144, 200);
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	Engine_ActorWaitForMove(IWAN);
	Actor_SetPosition(IWAN, 0, 0);
	Engine_ActorWaitForMove(MEARI);
	Actor_SetPosition(MEARI, 0, 0);
	Engine_EventEnd();
}

static void EV05_SEARCH(void)
{

	Engine_EventBegin();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_MessageShowCentered((s32)MsgYamaHeWhoHasPowerSee, 1);
	Engine_EventEnd();
}

static void ConfigureAndPlaceActorFourteen(void)
{

	s32 a = 21, b = 9;
	Engine_MapCopyCellAttributes(85, 9, 1, 1, a, b);
	Engine_MapObjectSetPosition(100, 0, 0);
	PlaceActor(14, 0x01580000, 0x00980000);
}

static void YamaRama_ClearActorPlacement(void)
{
	Map_CopyCellAttributes(21, 73, 1, 1, 21, 9);
	MapObject_SetPosition(100, -1, -1);
	Actor_SetPosition(14, 0, 0);
}

static void EV15_TALK(void)
{

	u32 heading = Object_GetById(0)->facing;

	Engine_EventBegin();
	if (heading - 0xA001 <= 0x3FFE)
	{
		Engine_SanctumOpen(15);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgYamaAmTravelingAroundWorldSpread);
		Engine_EventShowMessage(15, 0);
	}
	Engine_EventEnd();
}

static void EV10_TOUCH(void)
{
	struct FieldActor *record;
	s32				   none;

	if (GameFlag_IsSet(0x89a) == 0)
	{
	}
	else
	{
		Engine_EventBegin();
		Actor_SetPosition(10, 0x2180000, 0xd80000);
		Engine_EventSetMessage((s32)MsgYamaYahhSilkRoadBouldersBlock);
		Event_ShowMessageAndWait(10, 0, 20);
		Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		Engine_EventWait(20);
		record = Object_GetById(0);
		record->update = (void (*)(union FieldObject *))OverlayObject_SetFacingTowardObject10;
		record = Object_GetById(0);
		if ((record->z.fixed >> 20) == 13)
		{
			Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b8, 200);
		}
		Actor_SetSpeed(10, 0x20000, 0x10000);
		Engine_ActorSetSpritePriority(10, 2);
		Actor_WalkToAndWait(10, 0x198, 216);
		{
			struct FieldActor *record = Object_GetById(10);

			record->priority_flags = ACTOR_PRIORITY_AUTOMATIC | record->priority_flags;
		}
		Engine_EventWait(10);
		Actor_FaceDirection(10, 0x8000, 20);
		Event_ShowMessageAndWait(10, 0, 20);
		Engine_ActorStartRepeatedMotion(10, 2);
		Actor_SetAttachedEffect(10, 0x102);
		Engine_EventWait(60);
		Event_ShowMessageAndWait(10, 0, 20);
		Engine_ActorEnableActionCallback(10, YamaRama_HsuAction);
		Camera_MoveTo(0x1280000, -1, 0x1580000, 1);
		GameFlag_Set(0x8b0);
		Object_RefreshSelectorById(10);
		Engine_CameraWaitForMove();
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
		Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, YamaRama_LeaderAction);
		Object_RefreshSelectorById(0);
		Engine_EventWait(10);
		none = 0;
		record = Object_GetById(0);
		record->update = (void (*)(union FieldObject *))none;
		Engine_EventWait(30);
		Engine_ActorRunRepeatedMotion(10, 2);
		Engine_EventWait(20);
		Actor_FaceDirection(10, 0x5000, 120);
		Actor_ShowEmote(10, 0x105, 60);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
		Engine_ActorSetAnimationAndWait(10, 4);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(10, 0, 20);
		Engine_EventEnd();
	}
}

static void Scene_RunActorCue(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgYamaDoDoWarriorShouldReturn);
	Actor_ShowEmote(10, 0x105, 60);
	Event_OpenMessage(10, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		Scene_AdvanceStep(1);
	}
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(10, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(10, 0, 20);
	Engine_EventEnd();
}

static void Scene_RunActorExchange(void)
{
	Engine_EventBegin();
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(30);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
	Engine_CameraMoveToActor(9, 1);
	Engine_CameraWaitForMove();
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgYamaHsuOkay);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorRunRepeatedMotion(9, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(9, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_FaceDirection(10, 0xd000, 20);
	Event_ShowMessageAndWait(10, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(60);
	Actor_ShowEmote(8, 0x102, 60);
	Engine_ActorSetAnimationAndWait(8, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorStartRepeatedMotion(10, 2);
	Actor_SetAttachedEffect(10, 0x102);
	Engine_EventWait(60);
	Actor_FaceDirection(10, 0xb000, 20);
	Engine_ActorSetAnimation(9, 5);
	Engine_EventEnd();
	GameFlag_Set(0x8b1);
}

static void EV11_PSYNERGY(void)
{

	s32 mask;

	Engine_EventBegin();
	Actor_SetAttachedEffect(8, 0x102);
	Engine_ActorStartRepeatedMotion(8, 2);
	Engine_EventWait(60);
	Engine_EventSetMessage((s32)MsgYamaRobinDidLiftBoulder);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_SetAttachedEffect(10, 0x102);
	Engine_ActorJump(10, 4, 0);
	Engine_EventWait(60);
	Event_ShowMessageAndWait(10, 0, 20);
	Engine_ActorRunRepeatedMotion(8, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_FaceDirection(10, 0xd000, 20);
	Engine_ActorSetAnimationAndWait(10, 3);
	Engine_EventWait(20);
	Actor_WalkTo(8, 178, 0x114);
	Actor_WalkToAndWait(10, 172, 0x11c);
	Engine_ActorWaitForMove(8);
	Actor_FaceDirection(8, 0x5000, 0);
	Actor_FaceDirection(10, 0xb000, 0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	mask = 254;
	Event_ShowMessageAndWait(8, 0, 20);
	Object_GetById(8)->steering_flags &= mask;
	Object_GetById(10)->steering_flags &= mask;
	Actor_SetSpeed(8, 0x3333, 0x1999);
	Actor_SetSpeed(10, 0x3333, 0x1999);
	Engine_ActorSetAnimation(8, 5);
	Engine_ActorSetAnimation(10, 6);
	Engine_EventWait(20);
	Event_PlayCue(125);
	Actor_SetDestinationOffset(8, 2, 0);
	Actor_SetDestinationOffset(9, 2, 0);
	Actor_SetDestinationOffset(10, 2, 0);
	Engine_ActorWaitForMove(10);
	Engine_EventWait(30);
	Engine_ActorSetAnimation(8, 5);
	Engine_ActorSetAnimation(10, 6);
	Engine_EventWait(20);
	Event_PlayCue(125);
	Actor_SetDestinationOffset(8, 4, 0);
	Actor_SetDestinationOffset(9, 4, 0);
	Actor_SetDestinationOffset(10, 4, 0);
	Engine_ActorWaitForMove(10);
	Engine_ActorStop(9);
	Engine_ActorSetAnimation(8, 1);
	Engine_ActorSetAnimation(10, 1);
	Engine_EventWait(50);
	Engine_ActorJump(10, 2, 0);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(10, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_AskYesNo(8, 0);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimation(8, 5);
	Engine_ActorSetAnimation(10, 6);
	Engine_EventWait(20);
	Event_PlayCue(125);
	Actor_SetDestinationOffset(8, 2, 0);
	Actor_SetDestinationOffset(9, 2, 0);
	Actor_SetDestinationOffset(10, 2, 0);
	Engine_ActorWaitForMove(10);
	Engine_EventWait(30);
	Engine_ActorSetAnimation(8, 5);
	Engine_ActorSetAnimation(10, 6);
	Engine_EventWait(20);
	Event_PlayCue(125);
	Actor_SetDestinationOffset(8, 4, 0);
	Actor_SetDestinationOffset(9, 4, 0);
	Actor_SetDestinationOffset(10, 4, 0);
	Engine_ActorWaitForMove(10);
	Engine_EventWait(40);
	Engine_ActorSetAnimation(8, 1);
	Engine_ActorSetAnimation(10, 1);
	Engine_ActorJump(10, 2, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(10, 0xd000, 20);
	Event_ShowMessageAndWait(10, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 30);
	Event_ShowMessageAndWait(8, 0, 20);
	{
		struct FieldActor *record = Object_GetById(10);

		record->steering_flags = ACTOR_FACES_MOTION | record->steering_flags;
	}
	Actor_SetSpeed(10, 0xcccc, 0x6666);
	Actor_WalkToAndWait(10, 168, 0x128);
	Actor_FaceDirection(10, 0xd000, 20);
	Engine_ActorSetAnimation(10, 5);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimationAndWait(10, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(10, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Engine_EventEnd();
	GameFlag_Set(0x8b2);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	Engine_EventRequestExit(6);
}

static void EV07_SEARCH(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_MessageShowCentered((s32)MsgYamaNorthAltinMineWestLama, 1);
	Engine_EventEnd();
}

static void FieldScene_RunPairedLayoutStepsThenSetOne(void)
{
	{
		s32 fifth = 1;
		s32 sixth = 2;

		Engine_MapCopyCellsTo(5, 28, 5, 13, fifth, sixth);
	}
	{
		s32 fifth = 5;
		s32 sixth = 13;

		Engine_MapCopyCellAttributes(5, 28, 1, 2, fifth, sixth);
	}
	Engine_EventWait(1);
}

static void SceneState_RunRect6x28Step(void)
{
	{
		s32 fifth = 1;
		s32 sixth = 2;

		Engine_MapCopyCellsTo(6, 28, 5, 13, fifth, sixth);
	}
	{
		s32 fifth = 5;
		s32 sixth = 13;

		Engine_MapCopyCellAttributes(6, 28, 1, 2, fifth, sixth);
	}
	Engine_EventWait(1);
}

static s32 SceneActor_SetFlagBitByRankAgainstActorZero(struct FieldActor *actor)
{
	if (Object_GetById(0)->y.fixed > actor->y.fixed)
	{
		actor->priority_flags |= 2;
	}
	else
	{
		actor->priority_flags &= 0xFD;
	}
}

static void EV14_SCRIPT(void)
{
	struct FieldActor *current = Object_GetById(0);
	struct FieldActor *other = Object_GetById(14);

	if (current->z.fixed <= other->z.fixed)
	{
		Engine_ActorSetSpritePriority(14, 1);
	}
}

static void EV14_PSYNERGY(void)
{
	u8 zero;

	zero = 0;
	Engine_EventBegin();

	Object_GetById(14)->priority_flags &= 0xfd;
	Object_GetById(14)->collision_flags &= 0xfd;
	Object_GetById(14)->motion_flags = zero;
	Object_GetById(14)->update =
		(void (*)(union FieldObject *))SceneActor_SetFlagBitByRankAgainstActorZero;

	Engine_MapCopyCellAttributes(55, 16, 1, 1, 56, 18);
	Engine_MapCopyCellAttributes(55, 16, 1, 1, 20, 18);

	Engine_TaskWait(1);
	Engine_GameFlagSet(512);
	Engine_ActorSetSpritePriority(14, 2);
	Engine_EventEnd();
}

static void EV15_PSYNERGY(void)
{
	Engine_EventBegin();
	{
		s32 fifth = 21;
		s32 sixth = 11;

		Engine_MapCopyCellAttributes(14, 6, 1, 2, fifth, sixth);
	}
	Object_GetById(15)->collision_flags = 254;
	Engine_GameFlagSet(0x201);
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	if (gGameState.scene == (s32)&SceneId_YamaRama1)
	{
		return YamaRama_TempleEvents;
	}
	return YamaRama_Events;
}

s32 YamaRama_ApplyEntryState(void)
{
	s32 entrance;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if (gGameState.scene == (s32)&SceneId_YamaRama2)
	{
		BattleFx_SetQueuedSoundAndPlay(169);
		Engine_ActorSetAnimation(11, 5);
		Engine_ActorSetAnimation(12, 5);
		Engine_ActorSetAnimation(14, 2);
		Map_CopyCellAttributes(21, 9, 1, 1, 21, 73);
		ConfigureAndPlaceActorFourteen();
		if (Engine_GameFlagIsSet(0x8b2))
		{
			Actor_SetPosition(13, 0x880000, 0x1000000);
			Engine_ActorFaceDirection(13, 0, 0);
		}
		entrance = gGameState.entrance;
		if (entrance == 2)
		{
			Engine_GameFlagClear(0x12f);
		}
		else if (entrance == 3 && !Engine_GameFlagIsSet(0x109))
		{
			FieldScene_RunScene3a2SequenceA();
		}
	}
	else if (gGameState.scene == (s32)&SceneId_YamaRama1)
	{
		Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
		Object_GetById(14)->priority_flags |= 2;
		if (Engine_GameFlagIsSet(0x200))
		{
			Engine_ActorSetAnimation(14, 5);
			EV14_PSYNERGY();
		}
		if (Engine_GameFlagIsSet(0x201))
		{
			Engine_ActorSetAnimation(15, 4);
			EV15_PSYNERGY();
		}
		if (gGameState.entrance == 4 || gGameState.entrance == 5)
		{
			Engine_GameFlagClear(0x12f);
		}
		if (!Engine_GameFlagIsSet(0x89a) && !Engine_GameFlagIsSet(0x895)
			&& !Engine_GameFlagIsSet(0x8b2))
		{
			Engine_ActorSetPosition(10, 0, 0);
		}
		if (!GameFlag_IsSet(0x8b2) && Engine_GameFlagIsSet(0x895) && gGameState.entrance == 2)
		{
			Engine_ActorSetPosition(11, 0, 0);
			Engine_GameFlagSet(0x8b2);
			Engine_GameFlagSet(0x8b3);
			Engine_ActorSetPosition(10, 0, 0);
		}
		if (GameFlag_IsSet(0x8b2))
		{
			Engine_MapCopyCellsTo(54, 21, 53, 21, 1, 2);
			Engine_MapCopyCellAttributes(18, 20, 1, 3, 17, 21);
			Engine_MapCopyCellsTo(44, 18, 43, 17, 1, 1);
			Engine_MapCopyCellAttributes(8, 17, 1, 1, 7, 17);
		}
		if (Engine_GameFlagIsSet(0x895) && !Engine_GameFlagIsSet(0x8b2))
		{
			Engine_ActorSetPosition(12, 0, 0);
			Engine_ActorSetPosition(13, 0, 0);
			Actor_SetPosition(8, 0xc00000, 0x1080000);
			Actor_SetPosition(9, 0xa40000, 0x1180000);
			Actor_SetPosition(10, 0xb80000, 0x1300000);
			Actor_FaceDirection(8, 0x5000, 0);
			Actor_FaceDirection(10, 0xb000, 0);
			Engine_ActorEnableActionCallback(9, YamaRama_ActorNineAction);
			Object_GetById(9)->scale_x = -0x10000;
		}
		if (!GameFlag_IsSet(0x8b2))
		{
			Actor_SetPosition(9, 0xa40000, 0x1180000);
			Engine_ActorEnableActionCallback(9, YamaRama_ActorNineAction);
			Object_GetById(9)->scale_x = -0x10000;
		}
		if (gGameState.entrance == 5 && !Engine_GameFlagIsSet(0x8b1) && !Engine_GameFlagIsSet(0x109)
			&& !Engine_GameFlagIsSet(0x8b2))
		{
			Scene_RunActorExchange();
		}
	}
	return 0;
}

s32 YamaRama_TemplePlacement1[] = {
	SC_CALL(EventScript_PrepareActorRenderFlags),
	SC_END,
};

s32 YamaRama_HsuAction[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(392), 0, PIXELS(232)),
	SC_SYNC,
	SC_MOVE(PIXELS(376), 0, PIXELS(232)),
	SC_SYNC,
	SC_MOVE(PIXELS(344), 0, PIXELS(264)),
	SC_SYNC,
	SC_MOVE(PIXELS(344), 0, PIXELS(312)),
	SC_SYNC,
	SC_MOVE(PIXELS(312), 0, PIXELS(344)),
	SC_SYNC,
	SC_MOVE(PIXELS(296), 0, PIXELS(344)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 YamaRama_LeaderAction[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(392), 0, PIXELS(232)),
	SC_SYNC,
	SC_MOVE(PIXELS(376), 0, PIXELS(232)),
	SC_SYNC,
	SC_MOVE(PIXELS(344), 0, PIXELS(264)),
	SC_SYNC,
	SC_MOVE(PIXELS(344), 0, PIXELS(312)),
	SC_SYNC,
	SC_MOVE(PIXELS(328), 0, PIXELS(328)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 YamaRama_ActorNineAction[] = {
	SC_ARG(1),
	SC_ARG(5),
	SC_WAIT(240),
	SC_ARG(1),
	SC_ARG(5),
	SC_WAIT(60),
	SC_ARG(1),
	SC_ARG(5),
	SC_WAIT(300),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

u16 YamaRama_BoulderCells[] = {
	MAP_CELL_STEP(66, 46, 3, 2, 5), MAP_CELL_STEP(63, 46, 3, 2, 5), MAP_CELL_STEPS_END,
};

u16 YamaRama_BoulderCellsBack[] = {
	MAP_CELL_STEP(66, 46, 3, 2, 5), MAP_CELL_STEP(69, 46, 3, 2, 5), MAP_CELL_STEPS_END,
};

struct SceneEntrance YamaRama_Entrances[] = {
	{ 0, CONDITION_ALWAYS, 496, 0, 216, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 216, 0, 520, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 136, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 144, 0, 200, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 232, 0, 352, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance YamaRama_TempleEntrances[] = {
	{ 0, CONDITION_ALWAYS, 496, 0, 216, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 496, 0, 216, FACING_WEST, 0, 16, 32, 520, 416, 0 },
	{ 2, CONDITION_ALWAYS, 384, 0, 72, FACING_SOUTH, 0, 16, 32, 520, 416, 0 },
	{ 3, CONDITION_ALWAYS, 40, 0, 280, FACING_EAST, 0, 16, 32, 520, 416, 0 },
	{ 4, CONDITION_ALWAYS, 88, 0, 248, FACING_SOUTH, 0, 16, 32, 520, 416, 0 },
	{ 5, CONDITION_ALWAYS, 216, 0, 248, FACING_SOUTH, 0, 16, 32, 520, 416, 0 },
	{ SCENE_TABLE_END },
};

u32 YamaRama_Exits[] = {
	SCENE_EXITS(88),
	SCENE_EXIT(1, 2, 17),
	SCENE_EXIT(2, 61, 3),
	SCENE_EXIT(0, 74, 0),
	SCENE_EXIT(1, 2, 15),
	SCENE_EXIT(2, 2, 50),
	SCENE_EXIT(3, 2, 51),
	SCENE_EXIT(4, 80, 6),
	SCENE_EXIT(5, 80, 7),
	SCENE_EXIT(6, 88, 3),
	SCENE_EXITS_END,
};

struct ScenePlacement YamaRama_Placements[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x3d, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x9e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(336), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x9e, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(216), 0, PIXELS(240), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x9e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(320), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(320), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(176), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x125, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(152), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x79, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement YamaRama_TemplePlacements[] = {
	{ 0x3d, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x29, CONDITION_ALWAYS, (s32)YamaRama_ActorNineAction,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x27, 0x8b2, ACTOR_STAND,
		PIXELS(296), 0, PIXELS(344), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x127, 0x8b2, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(276), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x82, IF_FLAG_SET(0x895), (s32)YamaRama_TemplePlacement1,
		PIXELS(456), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x83, IF_FLAG_SET(0x895), ACTOR_STAND,
		PIXELS(184), 0, PIXELS(264), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(194), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent YamaRama_Events[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
	{ EVENT_TALK, 9, 0x8b2, MSGNO(MsgYamaRamaThisIsLamaTempleWelcomeTravelers) },
	{ EVENT_TALK, 10, 0x8b2, EVENT_SCRIPT(SceneDialogue_RunMessage1958Step) },
	{ EVENT_TALK, 11, 0x8b2, EVENT_SCRIPT(EV11_TALK) },
	{ EVENT_TALK, 12, 0x8b2, MSGNO(MsgYamaRamaYouWhoSpeakToMeI) },
	{ EVENT_TALK, 13, 0x8b2, EVENT_SCRIPT(EV13_TALK) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgYamaRamaItIsVeryDangerousToCross) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgYamaRamaTheEvilDesertIsHotterThan) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgYamaRamaIBelieveItIsPossibleTo) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgYamaRamaIWillBeMostThankfulOnce) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgYamaRamaWhenYourTemperatureReachesItsMaximum) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x8b2, MSGNO(MsgYamaRamaTheySeemToBeAnxiousThey) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x8b2, MSGNO(MsgYamaRamaTheySayAPersonCanOvercome) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x8b2, MSGNO(MsgYamaRamaIAmHungry) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x8b2, MSGNO(MsgYamaRamaISenseImpureThoughtsFromThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x8b2, MSGNO(MsgYamaRamaChiIsButAFragmentOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgYamaRamaICannotCrossTheDesertIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgYamaRamaTheEvilInTheDesertDistorts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgYamaRamaHowNiceItWouldBeIf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgYamaRamaMasterHamaWillBeVerySad) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgYamaRamaWithRevealYouWillFindOases) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgYamaRamaForSomeReasonThesePeopleGive) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(YamaRama_ClearActorPlacement) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(ConfigureAndPlaceActorFourteen) },
	{ SEARCH(SEARCH_UNNAMED), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_SEARCH) },
	{ 0xc403, 110, 0x350, FIND_PSYNERGY_STONE },
	{ 0xe403, 110, 0x350, FIND_PSYNERGY_STONE },
	{ 0xa403, 110, 0x350, FIND_PSYNERGY_STONE },
	{ 0x8403, 110, 0x350, FIND_PSYNERGY_STONE },
	{ SEARCH(SEARCH_CHEST), 100, 0xf7a, FIND_ITEM(ITEM_WATER_OF_LIFE) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent YamaRama_TempleEvents[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 5 },
	{ EVENT_TOUCH, 10, 0x8b0, EVENT_SCRIPT(EV10_TOUCH) },
	{ EVENT_TALK, 10, 0x895, EVENT_SCRIPT(Scene_RunActorCue) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgYamaRamaWellRobinAdeptsYouCameTo) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgYamaRamaIAmTrappedBetweenTheseTwo) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgYamaRamaBeStrongHsuWeWillHelp) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgYamaRamaWeMovedTheRocksNowPeople) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgYamaRamaManyPeopleFromXianHelpedMove) },
	{ PSYNERGY_ON_ACTOR(0x8f), 11, 0x8b2, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x895, MSGNO(MsgYamaRamaHsuIsInDangerICan) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgYamaRamaHowShouldWeMoveTheBoulders) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgYamaRamaIWantToScreamButFeizhi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgYamaRamaHsuTiresHeLooksExhaustedI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgYamaRamaWeClearedTheWayButWhat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgYamaRamaIAlmostHurtMyBackIt) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPairedLayoutStepsThenSetOne) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_RunRect6x28Step) },
	{ 0x10001815, 14, 0x200, EVENT_SCRIPT(EV14_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 14, 0x200, EVENT_SCRIPT(EV14_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0xc), 15, 0x201, EVENT_SCRIPT(EV15_PSYNERGY) },
	{ SEARCH(SEARCH_UNNAMED), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_SEARCH) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf71, FIND_ITEM(0xbf) },
	{ SCENE_EVENTS_END },
};
