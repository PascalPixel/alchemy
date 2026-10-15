#include "game_state.h"
#include "unit/scene/lalivero.h"
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
#include "field_scene.h"
#include "field_service.h"
#include "field_effect.h"
#include "item_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"

// 前方宣言
static void FieldScene_RunScene3c6SequenceA(void);

enum
{
	ENTRANCE_PRIMARY_SEQUENCE = 20,
	ENTRANCE_PRIMARY_SEQUENCE_ALTERNATE = 21,
	ENTRANCE_FROM_AERIE = 90,
	ENTRANCE_FROM_AERIE_ALTERNATE = 91
};

enum
{
	FLAG_AERIE_EVENTS_DONE = 0x9a7,
	FLAG_PRIMARY_SEQUENCE_SEEN = 0x9b8,
	FLAG_ACTOR_18_MOVED = 0x9bb,
	FLAG_ARRIVED_FROM_AERIE = 0x9bf
};

enum
{
	ACTOR_BLACK_ORB = 25
};

enum
{
	HEAP_ITEM_ICON = 17,
	ITEM_ICON_BUFFER_SIZE = 0x608,
	ITEM_ICON_TILES = 0x400,
	ITEM_ICON_TILE_BYTES = 128
};

static void EV23_TALK(void)
{
	BattleFx_RunPageEffectForSlot(0x17, 2, 6);
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

s32 SceneData_SelectTableByFlag9a7(void)
{
	if (Engine_GameFlagIsSet(0x9A7) != 0)
	{
		return (s32)Placement_Actors9a7;
	}
	return (s32)Placement_Actors;
}

static void EV10_TALK(s32 actor)
{
	void Actor_FaceDirection();

	struct FieldActor *object;

	object = Actor_Get(actor);
	object->scaleX = 0x10000;
	object = Actor_Get(actor);
	object->scaleY = 0x10000;
	Engine_EventSetMessage(MsgLaliveroOhhTheyTookShebaHeaded);
	Event_ShowMessage(actor, 0);
	Actor_FaceDirection(actor, 0xc000, 0);
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(actor, Lalivero_LamentActions);
}

static void EV11_TALK(s32 obj)
{
	s32 msg = MsgLaliveroLeavingLalivero;
	Engine_EventSetMessage(msg);
	Engine_EventOpenMessage(obj, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(10);
		Engine_EventSetMessage(msg + 1);
	}
	else
	{
		Engine_EventSetMessage(msg + 2);
	}
	Engine_EventShowMessage(obj, 0);
}

static void SceneActor_SetupActor18Event(void)
{

	Engine_GameFlagSet(2491);
	Engine_EventSetMessage(MsgLaliveroWasToldLetInIf);
	Engine_EventShowMessage(18, 0);
	Actor_SetSpeed(18, 0x10000, 0x8000);
	Actor_WalkByAndWait(18, -16, 0);
	Engine_ActorFaceDirection(18, 0, 0);
	Engine_EventWait(10);
}

static void Scene_RunTableTransition(void)
{
	s32 no = gEventWork->touchedTrigger;
	s32 x = Lalivero_DoorCells[no][0];
	s32 y = Lalivero_DoorCells[no][1];

	Object_GetById(0)->motionFlags = 2;
	Engine_AudioPlayCue(158);
	if (no == 6)
	{
		Map_AnimateCells((s32)Lalivero_GateOpenSteps, (u16)x, (u16)y);
		Actor_WalkBy(0, 0, -16);
	}
	else
	{
		Map_AnimateCells((s32)Lalivero_DoorOpenSteps, (u16)x, (u16)y);
		Actor_CenterAndWalk(0, 2, -16);
	}
	Engine_EventWait(10);
	gEventWork->transitionFrames = 16;
	Engine_EventRequestExit(no);
}

static void Lalivero_RequestTouchedExit(void)
{
	struct EventWork *work = gEventWork;
	s16				 *p = (s16 *)&work->touchedTrigger;
	s32				  n = *p;

	work->transitionFrames = 16;
	Engine_EventRequestExit(n);
}

static void Scene_RunPrimarySequence(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	Engine_EventSetMessage(MsgLaliveroMoreTolbisSoldiersLayDefeated);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0xf80000, 0x1a80000);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Engine_ActorSetAnimation(8, 0);
	Engine_ActorSetAnimation(9, 0);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	FieldMotion_LaunchFromFocusedObject(22, 8, -16, 0xc000);
	Engine_ActorWaitForMove(22);
	Engine_EventWait(20);
	Actor_ShowEmote(22, 0x102, 60);
	Actor_SetSpeed(22, 0x10000, 0x8000);
	Actor_WalkByAndWait(22, 0, -16);
	Engine_EventWait(10);
	Actor_FaceActor(22, 8, 40);
	Actor_FaceActor(22, 9, 40);
	Actor_FaceActor(22, 8, 40);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(22, 2);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(22, 0x4000, 0);
	Engine_EventWait(30);
	Event_OpenMessage(22, 0);
	Engine_EventChooseYesNo(0, 0);
	Engine_EventWait(30);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	Event_ShowMessage(8, 0);
	Engine_EventWait(10);
	Actor_FaceActor(22, 8, 30);
	Engine_ActorRunRepeatedMotion(22, 2);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_ActorSetAnimation(8, 1);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(30);
	Event_ShowMessage(8, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_EventWait(20);
	Event_ShowMessage(9, 0);
	Engine_EventWait(10);
	Actor_FaceActor(22, 9, 0);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 30);
	Engine_ActorSetAnimation(9, 1);
	Engine_EventWait(10);
	Actor_ShowEmote(22, 0x108, 40);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Engine_EventWait(10);
	Actor_FaceDirection(22, 0x4000, 0);
	Engine_EventWait(30);
	Engine_ActorRunRepeatedMotion(22, 2);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_EventWait(30);
	Event_ShowMessage(9, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(22, 0x102, 50);
	Actor_FaceActor(22, 9, 20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	Event_ShowMessage(8, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(22, 0x4000, 0);
	Engine_EventWait(30);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Engine_EventWait(20);
	Actor_ShowEmote(22, 0x102, 50);
	Actor_SetSpeed(22, 0x1cccc, 0xe666);
	Actor_WalkToAndWait(22, 0x100, 0x168);
	Engine_EventWait(10);
	Actor_FaceDirection(22, 0, 0);
	Engine_EventWait(30);
	Actor_FaceDirection(22, 0x8000, 0);
	Engine_EventWait(30);
	Actor_FaceDirection(22, 0xc000, 0);
	Engine_EventWait(30);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_EventWait(20);
	Event_ShowMessage(9, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(22, 0x100, 40);
	Actor_FaceDirection(22, 0x2000, 0);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(9, 3);
	Engine_EventWait(30);
	Event_ShowMessage(9, 0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(22, 2);
	Engine_EventWait(20);
	Actor_SetSpeed(22, 0x19999, 0xcccc);
	Actor_WalkToAndWait(22, 0x100, 0x180);
	Actor_FaceDirection(22, 0, 0);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	Event_ShowMessage(8, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(22, 0x8000, 0);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Actor_SetSpeed(22, 0x13333, 0x9999);
	Actor_WalkByAndWait(22, 0, 16);
	Engine_EventWait(10);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
	Actor_ShowEmote(22, 0x101, 80);
	Actor_FaceActor(22, 8, 40);
	Actor_FaceActor(22, 9, 40);
	Actor_FaceDirection(22, 0x4000, 0);
	Engine_EventWait(30);
	Engine_ActorRunRepeatedMotion(22, 2);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_EventWait(20);
	Event_ShowMessage(9, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(22, 0, 0);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 30);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	Event_ShowMessage(8, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(22, 0x8000, 0);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 70);
	Actor_FaceDirection(22, 0x4000, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(22, 2);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventWait(30);
	Actor_SetSpeed(22, 0x13333, 0x9999);
	Engine_ActorSetAnimation(22, 2);
	record = Actor_Get(ACTOR_PARTY_LEADER);
	if (record != 0)
	{
		Actor_SetDestination(22, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(22);
	Actor_SetPosition(22, 0, 0);
	Engine_EventWait(10);
	Engine_EventEnd();
}

static void EV50_TOUCH(void)
{
	struct FieldActor *record;

	GameFlag_Set(0x9ba);
	Engine_EventBegin();
	Engine_EventSetMessage(MsgLaliveroWhereGoingIsaacIodemAsked);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 0x178);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
	FieldMotion_LaunchFromFocusedObject(1, -32, 0, 0);
	FieldMotion_LaunchFromFocusedObject(3, -16, 16, 0xe000);
	Motion_LaunchFromFocusedObject(2, 0, 16, 0xc000);
	Engine_ActorWaitForMove(GARET);
	Engine_EventWait(30);
	Event_OpenMessage(GARET, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
	Engine_EventWait(10);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(MIA, 4);
		Engine_EventWait(20);
		Event_ShowMessage(MIA, 0);
		gEventWork->message += 1;
	}
	else
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(MIA, 4);
		Engine_EventWait(20);
		gEventWork->message += 1;
		Event_ShowMessage(MIA, 0);
	}
	Engine_EventWait(10);
	Actor_ShowEmote(IVAN, 0x108, 40);
	Event_ShowMessage(IVAN, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(40);
	Engine_ActorSetAnimation(GARET, 3);
	Engine_ActorSetAnimation(IVAN, 3);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_EventWait(30);
	Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -16, 0);
	Actor_WalkBy(IVAN, 0, -16);
	Actor_WalkBy(MIA, 0, -8);
	Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
	Engine_ActorSetAnimation(MIA, 1);
	Engine_ActorSetAnimation(IVAN, 1);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GARET, 0xc000, 0);
	Actor_FaceDirection(MIA, 0xc000, 0);
	Actor_FaceDirection(IVAN, 0xc000, 0);
	Actor_FaceDirection(22, 0x4000, 0);
	Engine_EventWait(80);
	Actor_SetSpeed(22, 0xcccc, 0x6666);
	Object_GetById(22)->motionFlags = 2;
	Engine_ActorSetSpritePriority(22, 2);
	Map_CopyCellsLayered(34, 0, 1, 2, 4, 18);
	Event_PlayCue(158);
	Engine_EventWait(20);
	Actor_SetPosition(22, 0x480000, 0x1380000);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
	Object_GetById(22)->targetZ = ACTOR_NO_TARGET;
	Actor_SetPosition(22, 0x480000, 0x1380000);
	Engine_EventWait(20);
	Actor_WalkToAndWait(22, 72, 328);
#else
	Engine_EventWait(20);
	Actor_WalkByAndWait(22, 0, 16);
#endif
	Engine_MapCopyCellsLayered(32, 0, 1, 2, 4, 18);
	Event_PlayCue(159);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GARET, 3);
	Engine_ActorSetAnimation(IVAN, 3);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_EventWait(40);
	Actor_WalkByAndWait(22, 16, 0);
	Actor_FaceDirection(22, 0x4000, 0);
	Engine_EventWait(20);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventWait(30);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_EventWait(30);
	Event_ShowMessage(MIA, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(22, 2);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(GARET, 2);
	Engine_EventWait(20);
	Event_ShowMessage(GARET, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(22, 0x102, 40);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(IVAN, 0x100, 40);
	Event_ShowMessage(IVAN, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventWait(30);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
	Engine_ActorFaceEachOther(MIA, IVAN, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GARET, 0xc000, 0);
	Actor_FaceDirection(IVAN, 0xc000, 0);
	Actor_FaceDirection(MIA, 0xc000, 0);
	Engine_EventWait(40);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventWait(30);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(MIA, 0x102, 40);
	Event_ShowMessage(MIA, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(GARET, 0xe000, 0);
	Engine_EventWait(20);
	Event_OpenMessage(GARET, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(22, 4);
		Engine_EventWait(20);
		Event_ShowMessage(22, 0);
		gEventWork->message += 1;
	}
	else
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(22, 4);
		Engine_EventWait(20);
		gEventWork->message += 1;
		Event_ShowMessage(22, 0);
	}
	Engine_EventWait(10);
	Actor_ShowEmote(IVAN, 0x102, 40);
	Event_ShowMessage(IVAN, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(GARET, 0xc000, 0);
	Actor_ShowEmote(22, 0x106, 50);
	Event_ShowMessage(22, 0);
	Engine_EventWait(20);
	Actor_SetPosition(25, 0x580000, 0x14c0000);
	Event_ShowMessage(-1, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(MIA, 0x100, 40);
	Engine_ActorJump(MIA, 4, 13);
	Engine_ActorJump(MIA, 4, 30);
	Event_ShowMessage(MIA, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventWait(30);
	Event_ShowMessage(22, 0);
	if (GameFlag_IsSet(0x9bf) != 0)
	{
		FieldScene_RunScene3c6SequenceA();
	}
	Engine_EventSetMessage(MsgLaliveroBabiToldMeShipAncients);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(22, 2);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventWait(30);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(GARET, 0x100, 40);
	Event_ShowMessage(GARET, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(22, 2);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(IVAN, 3);
	Engine_EventWait(30);
	Event_ShowMessage(IVAN, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventWait(30);
	Actor_SetSpeed(25, 0xcccc, 0x6666);
	Actor_SetDestinationOffset(25, 0, 16);
	Actor_WalkByAndWait(22, 0, 16);
	Engine_EventWait(30);
	Actor_SetPosition(25, 0, 0);
	gEventWork->message += 1;
	Engine_PartyGiveItem(242, 0);
	Engine_EventWait(10);
	Actor_Get(22)->steeringFlags &= ~ACTOR_FACES_MOTION;
	Actor_WalkByAndWait(22, 0, -16);
	Actor_Get(22)->steeringFlags |= ACTOR_FACES_MOTION;
	Actor_FaceDirection(22, 0x4100, 0);
	Engine_EventWait(30);
	Engine_EventWait(10);
	Actor_ShowEmote(22, 0x100, 40);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventWait(30);
	Event_ShowMessage(22, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GARET, 3);
	Engine_ActorSetAnimation(IVAN, 3);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_EventWait(50);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventWait(30);
	Actor_WalkByAndWait(22, -16, 0);
	Actor_FaceDirection(22, 0xc000, 0);
	Engine_EventWait(20);
	Map_CopyCellsLayered(34, 0, 1, 2, 4, 18);
	Event_PlayCue(158);
	Engine_EventWait(10);
	Actor_WalkByAndWait(22, 0, -16);
	Actor_SetPosition(22, 0, 0);
	Engine_EventWait(10);
	Engine_MapCopyCellsLayered(32, 0, 1, 2, 4, 18);
	Event_PlayCue(159);
	Engine_EventWait(50);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
	Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Event_ShowMessage(GARET, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Engine_EventWait(20);
	Event_ShowMessage(MIA, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(IVAN, 3);
	Engine_EventWait(30);
	Event_ShowMessage(IVAN, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(40);
	Engine_ActorSetAnimation(GARET, 3);
	Engine_ActorSetAnimation(IVAN, 3);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_EventWait(30);
	Actor_SetSpeed(GARET, 0x13333, 0x9999);
	Actor_SetSpeed(MIA, 0x13333, 0x9999);
	Actor_SetSpeed(IVAN, 0x13333, 0x9999);
	Engine_ActorSetAnimation(GARET, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(GARET, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(GARET);
	Actor_SetPosition(GARET, 0, 0);
	Engine_ActorSetAnimation(MIA, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(MIA, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(MIA);
	Actor_SetPosition(MIA, 0, 0);
	Engine_ActorSetAnimation(IVAN, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(IVAN, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(IVAN);
	Actor_SetPosition(IVAN, 0, 0);
	Engine_EventWait(10);
	Engine_EventEnd();
}

static void FieldScene_RunScene3c6SequenceA(void)
{
	Engine_EventSetMessage(MsgLaliveroThoughtShipWeSawAt);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(22, 2);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(22, 4);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GARET, 0x100, 0);
	Actor_ShowEmote(MIA, 0x100, 0);
	Actor_ShowEmote(IVAN, 0x100, 55);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(GARET, 2);
	Engine_EventWait(20);
	Event_ShowMessage(GARET, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(22, 4);
	Engine_EventWait(20);
	Event_ShowMessage(22, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(MIA, 0x102, 40);
	Event_ShowMessage(MIA, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(GARET, 3);
	Engine_EventWait(30);
	Event_ShowMessage(GARET, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(IVAN, 0x8000, 0);
	Engine_EventWait(65);
	Actor_FaceDirection(IVAN, 0xc000, 0);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(IVAN, 3);
	Engine_EventWait(30);
	Event_ShowMessage(IVAN, 0);
}

static void EV51_TOUCH(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage(MsgLaliveroWhereGoingArentWeTaking);
	Event_ShowMessage(GARET, 0);
	Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
	Engine_EventEnd();
}

s32 SceneData_SelectSecondaryTableByFlag9a7(void)
{
	if (Engine_GameFlagIsSet(0x9A7) != 0)
	{
		return (s32)Placement_Effects9a7;
	}
	return (s32)Placement_Effects;
}

s32 Scene_Initialize(void)
{
	union FieldObject  *object;
	struct FieldSprite *sprite;
	u8				   *icon;

	if (gGameState.entrance == ENTRANCE_FROM_AERIE)
	{
		GameFlag_Set(FLAG_AERIE_EVENTS_DONE);
		GameFlag_Set(FLAG_ARRIVED_FROM_AERIE);
	}
	if (gGameState.entrance == ENTRANCE_FROM_AERIE_ALTERNATE)
	{
		GameFlag_Set(FLAG_AERIE_EVENTS_DONE);
	}
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	gEventWork->transitionFrames = 24;

	Engine_ActorSetAnimation(19, 3);
	Actor_Get(19)->collisionFlags = 0;
	Engine_ActorSetSpriteFlags(Actor_Get(19), 0);
	Engine_ActorSetAnimation(20, 3);
	Actor_Get(20)->collisionFlags = 0;
	Engine_ActorSetSpriteFlags(Actor_Get(20), 0);
	Engine_ActorSetAnimation(21, 3);
	Actor_Get(21)->collisionFlags = 0;
	Engine_ActorSetSpriteFlags(Actor_Get(21), 0);
	Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_BLACK_ORB), 0);

	object = (union FieldObject *)Actor_Get(ACTOR_BLACK_ORB);
	object->actor.unk5c = 1;
	object->actor.motionFlags = 0;
	sprite = object->actor.sprite;
	object->actor.y.fixed = PIXELS(10);
	sprite->partCount = 0;
	sprite->fullColor = 0;
	sprite->palette = 0;
	icon = Engine_HeapAllocate(HEAP_ITEM_ICON, ITEM_ICON_BUFFER_SIZE);
	Engine_ItemLoadIcon(ITEM_BLACK_ORB);
	Engine_VramLoad(sprite->vramBlock, ITEM_ICON_TILE_BYTES, &icon[ITEM_ICON_TILES]);
	Engine_HeapRelease(HEAP_ITEM_ICON);

	if (GameFlag_IsSet(FLAG_AERIE_EVENTS_DONE))
	{
		Engine_ActorSetSpriteFlags(Actor_Get(24), 0);
		Actor_Get(24)->collisionFlags = 0;
		Map_CopyCellAttributes(20, 23, 1, 1, 14, 4);
		Map_CopyCellAttributes(20, 23, 1, 1, 15, 4);
		Map_CopyCellAttributes(20, 23, 1, 1, 16, 4);
		if (GameFlag_IsSet(FLAG_ACTOR_18_MOVED))
		{
			Actor_SetPosition(18, PIXELS(56), PIXELS(184));
		}
	}
	else
	{
		object = (union FieldObject *)Actor_Get(8);
		object->actor.collisionFlags = 0;
		object->actor.priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
		object->actor.sprite->flags = 0;
		object->actor.sprite->rotation = FACING_NORTH;
		object = (union FieldObject *)Actor_Get(9);
		object->actor.collisionFlags = 0;
		object->actor.priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
		object->actor.sprite->flags = 0;
		object->actor.sprite->rotation = FACING_SOUTH;
		Map_CopyCellAttributes(20, 23, 1, 1, 13, 23);
		Map_CopyCellAttributes(20, 23, 1, 1, 14, 23);
		Map_CopyCellAttributes(20, 23, 1, 1, 78, 23);
		Map_CopyCellAttributes(20, 23, 1, 1, 17, 23);
		Map_CopyCellAttributes(20, 23, 1, 1, 18, 23);
		if ((gGameState.entrance == ENTRANCE_PRIMARY_SEQUENCE
				|| gGameState.entrance == ENTRANCE_PRIMARY_SEQUENCE_ALTERNATE)
			&& !GameFlag_IsSet(FLAG_PRIMARY_SEQUENCE_SEEN))
		{
			GameFlag_Set(FLAG_PRIMARY_SEQUENCE_SEEN);
			object = (union FieldObject *)Actor_Get(11);
			object->actor.paused = 1;
			object = (union FieldObject *)Actor_Get(17);
			object->actor.paused = 1;
			Scene_RunPrimarySequence();
			object = (union FieldObject *)Actor_Get(11);
			object->actor.paused = 0;
			object = (union FieldObject *)Actor_Get(17);
			object->actor.paused = 0;
		}
	}
	return 0;
}

s32 Lalivero_LamentActions[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_ADD(SW_SCALE_X, -0x4000),
	SC_ADD(SW_SCALE_Y, 0x2000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x2000),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_X, -0x4000),
	SC_ADD(SW_SCALE_Y, 0x2000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x2000),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(80),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance Placement_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 72, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 152, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 200, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 296, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 360, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 432, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 72, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 104, 0, 312, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 9, CONDITION_ALWAYS, 360, 0, 280, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 440, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 424, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 20, CONDITION_ALWAYS, 248, 0, 416, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 21, CONDITION_ALWAYS, 248, 0, 64, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 Placement_Messages[] = {
	SCENE_EXITS(178),
	SCENE_EXIT(1, 180, 30),
	SCENE_EXIT(2, 179, 1),
	SCENE_EXIT(3, 180, 23),
	SCENE_EXIT(4, 179, 6),
	SCENE_EXIT(5, 179, 4),
	SCENE_EXIT(6, 179, 11),
	SCENE_EXIT(7, 180, 21),
	SCENE_EXIT(8, 180, 22),
	SCENE_EXIT(9, 179, 9),
	SCENE_EXIT(10, 179, 2),
	SCENE_EXIT(11, 179, 10),
	SCENE_EXIT(20, 2, 30),
	SCENE_EXIT(21, 2, 59),
	SCENE_EXITS_END,
};

struct ScenePlacement Placement_Actors[] = {
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(224), 0, PIXELS(392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(288), 0, PIXELS(392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x6c, CONDITION_ALWAYS, (s32)Lalivero_LamentActions,
		PIXELS(248), 0, PIXELS(96), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(312), 0, PIXELS(280), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x8f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(304), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(296), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(440), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(200), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x66, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(248), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(160), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(440), 0, PIXELS(400), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x39, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x5c, 0x5e, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(136), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement Placement_Actors9a7[] = {
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(248), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(312), 0, PIXELS(296), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x8f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(304), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(296), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(440), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(200), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x66, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(248), 0, PIXELS(232), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(160), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(440), 0, PIXELS(400), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x39, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x5c, 0x5e, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(136), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x112, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ GARET, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ IVAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ MIA, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

s16 Lalivero_DoorCells[][2] = {
	{ 0, 0 }, { 4, 9 }, { 9, 11 }, { 12, 8 }, { 18, 8 }, { 22, 8 }, { 26, 7 }, { 4, 18 },
	{ 0, 0 }, { 22, 15 }, { 27, 18 }, { 0, 0 },
};

u16 Lalivero_DoorOpenSteps[] = {
	MAP_CELL_STEP(33, 0, 1, 2, 8), MAP_CELL_STEP(34, 0, 1, 2, 1), MAP_CELL_STEPS_END,
};

u16 Lalivero_GateOpenSteps[] = {
	MAP_CELL_STEP(34, 2, 2, 2, 8), MAP_CELL_STEP(36, 2, 2, 2, 1), MAP_CELL_STEPS_END,
};

struct SceneEvent Placement_Effects[] = {
	{ EVENT_EXIT, 20, CONDITION_ALWAYS, 20 },
	{ EVENT_EXIT, 21, CONDITION_ALWAYS, 21 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Lalivero_RequestTouchedExit) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Lalivero_RequestTouchedExit) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroUhhnnWeWereTakenBySurprise) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyWereTerribleWeWereNo) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorIsOurOnlyHopeIf) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroThisAllHappenedBecauseTolbiTook) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroFaranFollowedActorImSureHell) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheRumblingAtTheRuinsMust) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroThereWasAnEarthquakeAtThe) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfIWereBiggerIWould) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroIBetActorCanUseHer) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgLaliveroBabisMostPreciousBelongingIsKept) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfOnlyTheyDidntHaveActor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyUsedFireOnUsThats) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroCursesWeCouldntStopActorAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroWhatDoTheyPlanToDo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroWeNeverShouldHaveLetTolbi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroFaranStillDoesNotReturnWhat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroWeHaveSeenManyOmensOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheStonesWeStackedForThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroITriedToStepInFront) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyAllHaveALessonComing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWonderWhatBabisMostPrecious) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EV23_TALK) },
	{ SEARCH(SEARCH_JAR), 100, 0xfb6, FIND_ITEM(ITEM_WATER_OF_LIFE) },
	{ SEARCH(SEARCH_STONE_PILLAR), 101, 0xfb7, FIND_ITEM(0xba) },
	{ 0x8413, 102, 0xfb8, FIND_ITEM(0x97) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent Placement_Effects9a7[] = {
	{ EVENT_TOUCH, 50, 0x9ba, EVENT_SCRIPT(EV50_TOUCH) },
	{ EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(EV51_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Lalivero_RequestTouchedExit) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunTableTransition) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Lalivero_RequestTouchedExit) },
	{ EVENT_TALK, 18, 0x9bb, EVENT_SCRIPT(SceneActor_SetupActor18Event) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x412, 0x9bb, EVENT_SCRIPT(SceneActor_SetupActor18Event) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroHaHaIThoughtTheyHad) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWillNeverLetMyGuard) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroWhereCouldActorBeNowIm) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TALK) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroISawTheBeaconOfVenus) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheExplosionInTheVenusRuins) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroThePeninsulaBehindVenusLighthouseIs) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroICantBelieveYouEscapedVenus) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfYouDefeatedThatEvilBunch) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorJumpedIntoTheOceanFrom) },
	{ EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgLaliveroNoTrespassingDangerBeyondThisPoint) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgLaliveroApparentlyAShipHasBeenDocked) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroIAmAllHealedNowBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWantedToBeAPart) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfActorUsedHerGreatPowers) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroAfterEverythingThatsHappenedLaliveroNo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroVenusLighthouseHasBeenTheSymbol) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroIodemOfTolbiDoesntSeemToo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheOceanIsSoRoughSince) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroVenusLighthouseIsStillStandingStrong) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroYouMustBeGreatWarriorsTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorWouldBeFineEvenAfter) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgLaliveroThisSureSeemsLikeAStrange) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EV23_TALK) },
	{ SEARCH(SEARCH_JAR), 100, 0xfb6, FIND_ITEM(ITEM_WATER_OF_LIFE) },
	{ SEARCH(SEARCH_STONE_PILLAR), 101, 0xfb7, FIND_ITEM(0xba) },
	{ SEARCH(SEARCH_CHEST), 102, 0xfb8, FIND_ITEM(0x97) },
	{ SCENE_EVENTS_END },
};
