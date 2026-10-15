#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/tolbi_ship_mast.h"
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
#include "global_cells.h"
#include "tolbi_ship_mast.h"
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"

// 前方宣言
static void TolbiShipMast_SetupHiddenLeaderLookout(void);
static void TolbiShipMast_SetupVisibleLeaderLookout(void);
static void TolbiShipMast_AnnounceClearCourse(void);
static void Scene_RunFourActorStagingSequence(void);
static void FieldScene_RunActorNinePresentationCycles(void);
static void OverlayObject_InitWithRandomFields(s32 a);
static void FieldScene_RunPrimarySequence(void);
static void FieldScene_RunSevenActorEnsemble(void);
static void TolbiShipMast_UpdateSway(void);
static s32 SceneData_GetDifferenceOfPairSums(void);
static s32 SceneData_GetValueByFirstSetFlag(u32 a);

struct SwayActor
{
	u8	unk00[8];
	s32 x;
	s32 y;
	u8	unk10[0x3c];
	s32 drift;
	u8	unk50[0x14];
	s16 xState;
	s16 yState;
};

struct FieldGlobals
{
	s32				**map;
	u8				  unk04[0x48];
	struct EventWork *event;
};

s32 TolbiShipMast_DriftY __attribute__((section(".bss")));
s32 TolbiShipMast_DriftX __attribute__((section(".bss")));
s32 TolbiShipMast_SwayY __attribute__((section(".bss")));
s32 TolbiShipMast_SwayUnused __attribute__((section(".bss")));
s32 TolbiShipMast_CameraCenter[2] __attribute__((section(".bss")));
s32 TolbiShipMast_CameraOrigin[2] __attribute__((section(".bss")));
s32 TolbiShipMast_SwayX __attribute__((section(".bss")));

static s32 Object_PlaceFromCameraOffset(struct FieldActor *object)
{
	s32 *position = *((s32 **)gMapWork[0]);
	s32 *origin = TolbiShipMast_CameraOrigin;
	s32 *center = TolbiShipMast_CameraCenter;
	s32	 q0 = *position++;
	s32	 q1 = *position;

	object->x.fixed = center[0] + (q0 - origin[0]);
	object->y.fixed = center[1] + (q1 - origin[1]) / 2;
	object->sprite->rotation += 0x600;
	return 0;
}

static s32 SceneActor_SetFacingFromSample(struct FieldActor *object)
{
	u32 sample = (u32)(Engine_RandomNext() << 6) >> 16;

	if (sample == 6)
	{
		s32 value = 0xd000;

		object->facing = value;
	}
	else if (sample == 9)
	{
		s32 value = 0xb000;

		object->facing = value;
	}

	return 1;
}

static void TolbiShipMast_GrowActorScale(struct FieldActor *o)
{
	if (o->scaleX < 0x10000)
	{
		o->scaleX += 160;
		o->scaleY += 160;
	}
}

static s32 TolbiShipMast_SwayActor(struct SwayActor *actor)
{
	s16 *state = &actor->xState;
	s32	 next;

	if (*state == 9)
	{
		actor->drift = 0;
	}
	else if (*state != 0)
	{
		actor->drift -= (u32)(Engine_RandomNext() << 11) >> 16;
		if (actor->drift < -0xc000)
		{
			next = 0;
			*state = next;
		}
	}
	else
	{
		actor->drift += (u32)(Engine_RandomNext() << 11) >> 16;
		if (actor->drift > 0xc000)
		{
			next = 1;
			*state = next;
		}
	}
	if (actor->x > 0x280000 && actor->x < 0x1400000)
	{
		actor->x += actor->drift;
	}
	state = &actor->yState;
	if (*state == 9)
	{
		actor->y = 0;
	}
	else if (*state != 0)
	{
		actor->y -= (u32)(Engine_RandomNext() * 3 << 14) >> 16;
		if (actor->y < 0)
		{
			next = 0;
			*state = next;
		}
	}
	else
	{
		actor->y += (u32)(Engine_RandomNext() * 3 << 14) >> 16;
		if (actor->y > 0x100000)
		{
			next = 1;
			*state = next;
		}
	}
	return 1;
}

static s32 OverlayObject_ShrinkScaleThenStop(struct FieldActor *o)
{
	u8 *t = (u8 *)o->sprite;

	t[9] |= 12;
	o->speed = 0x20000;
	o->acceleration = 0x10000;
	if (o->scaleX > 0x1000)
	{
		o->scaleX -= 0x400;
		o->scaleY -= 0x400;
	}
	else
	{
		o->x.fixed = 0;
		o->y.fixed = 0;
		o->z.fixed = 0;
		o->velocityX = 0;
		o->velocityY = 0;
		o->velocityZ = 0;
	}
	return 1;
}

u8 *TolbiShipMast_GetEntrances(void)
{
	return (u8 *)TolbiShipMast_SceneTableA;
}

u8 *TolbiShipMast_GetRegions(void)
{
	return (u8 *)TolbiShipMast_SceneTableB;
}

u8 *TolbiShipMast_GetExits(void)
{
	return (u8 *)TolbiShipMast_SceneTableC;
}

u8 *TolbiShipMast_GetPlacements(void)
{
	return (u8 *)TolbiShipMast_SceneTableD;
}

u8 *TolbiShipMast_GetEvents(void)
{
	return (u8 *)TolbiShipMast_SceneTableE;
}

static void EV10_TOUCH(void)
{
	FieldEffect_UpdateGridPlacement();
}

static void EV11_SEARCH(void)
{
	if (Engine_GameFlagIsSet(FLAG_MAST_923) != 0 || Engine_GameFlagIsSet(FLAG_MAST_922) != 0)
	{
		Engine_EventBegin();
		Engine_ItemShowFound(ITEM_ANCHOR_CHARM, 3);
		Engine_PartyGiveItem(ITEM_ANCHOR_CHARM, 0);
		Engine_GameFlagSet(FLAG_MAST_924);
		Engine_EventEnd();
	}
}

s32 TolbiShipMast_ApplyEntryState(void)
{
	struct FieldActor *leader;

	Engine_GameFlagSet(0x144);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	if ((Engine_GameFlagIsSet(0x927) != 0 || Engine_GameFlagIsSet(0x928) != 0)
		&& Engine_GameFlagIsSet(0x93e) == 0 && Engine_GameFlagIsSet(0x8a0) == 0)
	{
		TolbiShipMast_SwayX = (u16)Engine_RandomNext();
		TolbiShipMast_SwayY = (u16)Engine_RandomNext();
		FieldScheduler_AddCallback(TolbiShipMast_UpdateSway, 0xc80);
	}
	if (Engine_GameFlagIsSet(0x925) != 0 && Engine_GameFlagIsSet(0x93e) == 0)
	{
		Actor_SetPosition(8, 0xa40000, 0x1480000);
	}
	switch (gGameState.entrance)
	{
	case 1:
		if (Engine_GameFlagIsSet(0x109) == 0)
		{
			leader = Object_GetById(ACTOR_PARTY_LEADER);
			Engine_EventBegin();
			battle_owner_69();
			leader->y.fixed = 0x380000;
			Camera_MoveTo(-1, -1, -1, 0);
			Engine_TaskWait(1);
			Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
			Engine_MapRedraw();
			Engine_TaskWait(1);
			Engine_EventEnd();
		}
		break;
	case 10:
		if (Engine_GameFlagIsSet(0x928) != 0)
		{
			TolbiShipMast_SetupVisibleLeaderLookout();
		}
		else
		{
			TolbiShipMast_SetupHiddenLeaderLookout();
		}
		break;
	case 11:
		gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
		gGameState.savedEntrance = 30;
		Scene_RunFourActorStagingSequence();
		break;
	case 12:
		gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
		gGameState.savedEntrance = 30;
		FieldScene_RunActorNinePresentationCycles();
		break;
	case 13:
		gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
		gGameState.savedEntrance = 30;
		FieldScene_RunPrimarySequence();
		break;
	case 14:
		FieldScene_RunSevenActorEnsemble();
		break;
	}
	return 0;
}

static void TolbiShipMast_SetupHiddenLeaderLookout(void)
{
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Battle_GetWorkObject1e0()->motionFlags = 0;
	Camera_MoveTo(0xa40000, 0x400000, 0x1410000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	TolbiShipMast_AnnounceClearCourse();
	Engine_EventEnd();
}

static void TolbiShipMast_SetupVisibleLeaderLookout(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0xa40000, 0x1410000);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
	record = Object_GetById(0);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_TaskWait(1);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	TolbiShipMast_AnnounceClearCourse();
	Engine_EventEnd();
}

static void TolbiShipMast_AnnounceClearCourse(void)
{
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Actor_SetSpeed(8, 0x10000, 0x8000);
	Actor_WalkToAndWait(8, 164, 0x141);
	Actor_FaceDirection(8, 0xd000, 40);
	Actor_FaceDirection(8, 0xb000, 40);
	Actor_FaceDirection(8, 0xd000, 40);
	Actor_FaceDirection(8, 0x3000, 10);
	Actor_WalkToAndWait(8, 164, 0x14e);
	Engine_ActorJump(8, 4, 40);
	Engine_ActorStartRepeatedMotion(8, 2);
	Engine_EventSetMessage(MsgTolbiShipShipsCourseClear);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(10);
}

static void Scene_RunFourActorStagingSequence(void)
{
	s32				  *placement = *(*(struct FieldGlobals *)gMapWork).map;
	struct FieldActor *actor;
	struct EventWork **event;
	s32				   x;
	s32				   zero = 0;
	s32				   scale;
	s32				   update;

	Engine_EventBegin();
	Event_CallWithLastActiveObjectId((s32)TolbiShipMast_StagingObjects);
	Engine_TaskWait(1);
	Engine_ActorSetChildValue(0, 15);
	Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
	Engine_ActorEnableActionCallback(8, TolbiShipMast_LookoutActions);
	event = &(*(struct FieldGlobals *)gMapWork).event;
	(*event)->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	TolbiShipMast_CameraOrigin[0] = *placement++;
	TolbiShipMast_CameraOrigin[1] = *placement;
	Actor_SetPosition(9, 0x500000, 0xd20000);
	x = 0x500000;
	Object_GetById(9)->motionFlags = zero;
	TolbiShipMast_CameraCenter[0] = x;
	TolbiShipMast_CameraCenter[1] = zero;
	Engine_ActorEnableActionCallback(9, TolbiShipMast_FollowCameraActions);
	Engine_EventWait(20);
	Engine_AudioPlayCue(29);
	Engine_GameFlagSet(0x8f0);
	Engine_ActorStop(8);
	Engine_TaskWait(1);
	Actor_ShowEmote(8, 0x100, 0);
	Engine_ActorFaceDirection(8, 0xb000, 0);
	Engine_EventSetMessage(MsgTolbiShipAvast);
	Engine_EventShowMessageAndWait(8, 0, 10);
	Actor_SetPosition(10, x, 0xd20000);
	Actor_SetPosition(11, x, 0xd20000);
	Engine_ActorSetPosition(12, x, 0xd20000);
	Engine_ActorSetSpritePriority(10, 3);
	Engine_ActorSetSpritePriority(11, 3);
	Engine_ActorSetSpritePriority(12, 3);
	Engine_ActorSetChildValue(10, 3);
	Engine_ActorSetChildValue(11, 3);
	Engine_ActorSetChildValue(12, 3);
	actor = Object_GetById(10);
	scale = 0x8000;
	update = (s32)TolbiShipMast_GrowActorScale;
	actor->scaleY = scale;
	actor->scaleX = scale;
	actor->update = (void (*)(union FieldObject *))update;
	actor = Object_GetById(11);
	actor->scaleY = scale;
	actor->scaleX = scale;
	actor->update = (void (*)(union FieldObject *))update;
	actor = Object_GetById(12);
	actor->scaleY = scale;
	actor->scaleX = scale;
	actor->update = (void (*)(union FieldObject *))update;
	Engine_TaskWait(1);
	Actor_SetSpeed(10, 0x851e, 0x428f);
	Actor_SetSpeed(11, 0x7333, 0x3999);
	Actor_SetSpeed(12, 0x9999, 0x4ccc);
	Actor_SetDestination(10, 128, 345);
	Actor_SetDestination(11, 136, 330);
	Engine_ActorSetDestination(12, 156, 340);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(8, 2);
	Actor_WalkToAndWait(8, 164, 344);
	Engine_ActorJump(8, 4, 10);
	Engine_ActorJump(8, 6, 40);
	Engine_ActorStartRepeatedMotion(8, 3);
	Engine_EventShowMessageAndWait(8, 0, 20);
	(*event)->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(11);
	Engine_EventEnd();
}

static void FieldScene_RunActorNinePresentationCycles(void)
{
	struct FieldActor *rec9;
	struct FieldActor *record;

	Engine_EventBegin();
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
	record = Object_GetById(0);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_TaskWait(1);
	Event_CallWithLastActiveObjectId((s32)TolbiShipMast_EnsembleObjects);
	Engine_TaskWait(1);
	OverlayObject_InitWithRandomFields(9);
	OverlayObject_InitWithRandomFields(10);
	OverlayObject_InitWithRandomFields(11);
	OverlayObject_InitWithRandomFields(12);
	OverlayObject_InitWithRandomFields(13);
	OverlayObject_InitWithRandomFields(14);
	OverlayObject_InitWithRandomFields(15);
	Engine_ActorEnableActionCallback(8, TolbiShipMast_LookoutActions);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(120);
	rec9 = Object_GetById(9);
	Engine_ActorStop(9);
	rec9->targetX = ACTOR_NO_TARGET;
	rec9->targetY = ACTOR_NO_TARGET;
	rec9->targetZ = ACTOR_NO_TARGET;
	rec9->velocityX = 0;
	rec9->velocityY = 0;
	rec9->velocityZ = 0;
	rec9->riseBaseY = 0;
	Engine_EventWait(20);
	Actor_SetSpeed(9, 0x80000, 0x40000);
	FieldObject_SetPosition(rec9, 0xa40000, 0x900000, 0x1410000);
	Object_CommitPosition(rec9);
	FieldObject_SetPosition(rec9, 0xa40000, 0x680000, 0x1410000);
	Object_CommitPosition(rec9);
	FieldObject_SetPosition(rec9, 0xcc0000, 0x7c0000, 0x1410000);
	Object_CommitPosition(rec9);
	FieldObject_SetPosition(rec9, 0x900000, 0, 0xa90000);
	Engine_ActorStop(8);
	Engine_TaskWait(1);
	Actor_FaceDirection(8, 0x8000, 0);
	Actor_ShowEmote(8, 0x103, 60);
	Actor_SetSpeed(9, 0x20000, 0x10000);
	OverlayObject_InitWithRandomFields(9);
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(8, TolbiShipMast_LookoutActions);
	Engine_EventWait(120);
	Engine_ActorStop(9);
	rec9->targetX = ACTOR_NO_TARGET;
	rec9->targetY = ACTOR_NO_TARGET;
	rec9->targetZ = ACTOR_NO_TARGET;
	rec9->velocityX = 0;
	rec9->velocityY = 0;
	rec9->velocityZ = 0;
	rec9->riseBaseY = 0;
	Engine_EventWait(20);
	Actor_SetSpeed(9, 0x80000, 0x40000);
	FieldObject_SetPosition(rec9, 0xa40000, 0x900000, 0x1410000);
	Object_CommitPosition(rec9);
	Actor_SetSpeed(9, 0x50000, 0x28000);
	FieldObject_SetPosition(rec9, 0xa40000, 0x680000, 0x1410000);
	Object_CommitPosition(rec9);
	FieldObject_SetPosition(rec9, 0xa40000, 0x720000, 0x1410000);
	Object_CommitPosition(rec9);
	FieldObject_SetPosition(rec9, 0xa40000, 0x680000, 0x1410000);
	Object_CommitPosition(rec9);
	FieldObject_SetPosition(rec9, 0xcc0000, 0x7c0000, 0x1410000);
	Object_CommitPosition(rec9);
	Engine_ObjectSetPosition(rec9, 0x900000, 0, 0xa90000);
	Engine_ActorStop(8);
	Engine_TaskWait(1);
	Actor_FaceDirection(8, 0x8000, 0);
	Actor_ShowEmote(8, 0x103, 60);
	Actor_SetSpeed(9, 0x20000, 0x10000);
	OverlayObject_InitWithRandomFields(9);
	Engine_ActorJump(8, 4, 20);
	Engine_ActorJump(8, 6, 40);
	Event_PlayCue(29);
	GameFlag_Set(0x8f0);
	Engine_EventSetMessage(MsgTolbiShipMonsters2);
	Event_ShowMessageAndWait(16, 0, 20);
	Actor_ShowEmote(8, 0x100, 0);
	Actor_WalkToAndWait(8, 164, 0x158);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(12);
	Engine_EventEnd();
}

static void OverlayObject_InitWithRandomFields(s32 a)
{
	struct FieldActor *obj;
	u32				   x;

	obj = Object_GetById(a);
	Engine_ActorSetSpritePriority(a, 1);
	obj->motionFlags = 0;
	obj->timer = (u32)Engine_RandomNext() >> 15;
	obj->work = (u32)Engine_RandomNext() >> 15;
	x = Engine_RandomNext();
	x <<= 2;
	x >>= 16;
	x <<= 16;
	x += 0x60000;
	obj->y.fixed = x;
	x = Engine_RandomNext();
	obj->riseBaseY = ((x * 3 << 13) >> 16) - 0x3000;
	obj->scaleX = 0x14000;
	obj->scaleY = 0x14000;
	Engine_ActorEnableActionCallback(a, TolbiShipMast_DriftActions);
}

static void FieldScene_RunPrimarySequence(void)
{
	u32				   i;
	struct FieldActor *actor;

	Engine_EventBegin();
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
	Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
	Engine_TaskWait(1);
	Event_CallWithLastActiveObjectId((s32)TolbiShipMast_EnsembleObjects);
	Engine_TaskWait(1);
	Event_CallWithLastActiveObjectId((s32)TolbiShipMast_LandingObjects);
	Engine_TaskWait(1);
	OverlayObject_InitWithRandomFields(9);
	OverlayObject_InitWithRandomFields(10);
	OverlayObject_InitWithRandomFields(11);
	OverlayObject_InitWithRandomFields(12);
	OverlayObject_InitWithRandomFields(13);
	OverlayObject_InitWithRandomFields(14);
	OverlayObject_InitWithRandomFields(15);
	Engine_ActorEnableActionCallback(8, TolbiShipMast_LookoutActions);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(0x12c);
	Event_PlayCue(147);
	Engine_EventWait(100);
	Engine_ActorStop(9);
	Engine_ActorStop(10);
	Engine_ActorStop(11);
	Engine_ActorStop(12);
	Engine_ActorStop(13);
	Engine_ActorStop(14);
	Engine_ActorStop(15);
	Actor_SetSpeed(9, 0x30000, 0x18000);
	Actor_SetSpeed(10, 0x30000, 0x18000);
	Actor_SetSpeed(11, 0x30000, 0x18000);
	Actor_SetSpeed(12, 0x30000, 0x18000);
	Actor_SetSpeed(13, 0x30000, 0x18000);
	Actor_SetSpeed(14, 0x30000, 0x18000);
	Actor_SetSpeed(15, 0x30000, 0x18000);
	Actor_SetDestination(9, 0, 100);
	Actor_SetDestination(10, 60, 100);
	Actor_SetDestination(11, 120, 100);
	Actor_SetDestination(12, 180, 100);
	Actor_SetDestination(13, 240, 100);
	Actor_SetDestination(14, 0x140, 100);
	Actor_SetDestination(15, 0x17c, 100);
	Engine_EventWait(40);
	Actor_ShowEmote(8, 0x101, 0);
	Engine_EventWait(20);
	Actor_SetPosition(9, 0, 0);
	Actor_SetPosition(10, 0, 0);
	Actor_SetPosition(11, 0, 0);
	Actor_SetPosition(12, 0, 0);
	Actor_SetPosition(13, 0, 0);
	Actor_SetPosition(14, 0, 0);
	Actor_SetPosition(15, 0, 0);
	Engine_EventWait(100);
	actor = Object_GetById(18);
	actor->scaleX = 0x1999;
	actor->scaleY = 0x1999;
	Actor_SetPosition(18, 0xac0000, 0x1540000);
	Engine_ActorStop(8);
	Engine_TaskWait(1);
	Engine_ActorRunRepeatedMotion(8, 1);
	Actor_FaceDirection(8, 0x3000, 0);
	Event_PlayCue(29);
	GameFlag_Set(0x8f0);
	for (i = 0; i < 32; i++)
	{
		actor->scaleX += 0xccc;
		actor->scaleY += 0xccc;
		Engine_TaskWait(1);
	}
	Actor_ShowEmote(8, 0x101, 60);
	Engine_ActorRunRepeatedMotion(8, 2);
	Actor_WalkToAndWait(8, 168, 0x154);
	Actor_WalkToAndWait(8, 200, 0x154);
	Actor_FaceDirection(8, 0x8000, 0);
	actor = Object_GetById(17);
	actor->scaleX = 0x12666;
	actor->scaleY = 0x12666;
	actor->x.fixed = 0xac0000;
	actor->y.fixed = 0xa00000;
	actor->z.fixed = 0x1540000;
	actor->facing = 0;
	actor->bounce = 0x6666;
	actor->gravity = 0x30000;
	Engine_EventWait(20);
	Engine_ActorJump(8, 6, 20);
	Event_PlayCue(147);
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(8, TolbiShipMast_WaveActions);
	Engine_EventWait(80);
	Engine_ActorSetSpritePriority(17, 1);
	Actor_SetSpeed(17, 0x10000, 0x8000);
	actor->bounce = 0x1999;
	actor->gravity = 0xb333;
	Event_PlayCue(153);
	actor->velocityY = 0x80000;
	Actor_SetDestination(17, 132, 0x168);
	Actor_SetDestination(18, 132, 0x168);
	Engine_EventWait(40);
	Actor_SetPosition(17, 0, 0);
	actor = Object_GetById(8);
	actor->scaleX = 0x10000;
	actor->scaleY = 0x10000;
	actor->facing = 0x5000;
	Engine_EventWait(40);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(13);
	Engine_EventEnd();
}

static void FieldScene_RunSevenActorEnsemble(void)
{
	Engine_EventBegin();
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
	Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
	Event_CallWithLastActiveObjectId((s32)TolbiShipMast_EnsembleObjects);
	Engine_TaskWait(1);
	OverlayObject_InitWithRandomFields(9);
	OverlayObject_InitWithRandomFields(10);
	OverlayObject_InitWithRandomFields(11);
	OverlayObject_InitWithRandomFields(12);
	OverlayObject_InitWithRandomFields(13);
	OverlayObject_InitWithRandomFields(14);
	OverlayObject_InitWithRandomFields(15);
	Engine_ActorEnableActionCallback(8, TolbiShipMast_LookoutActions);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(400);
	Engine_ActorStop(9);
	Engine_ActorStop(10);
	Engine_ActorStop(11);
	Engine_ActorStop(12);
	Engine_ActorStop(13);
	Engine_ActorStop(14);
	Engine_ActorStop(15);
	Actor_SetSpeed(9, 0x30000, 0x18000);
	Actor_SetSpeed(10, 0x30000, 0x18000);
	Actor_SetSpeed(11, 0x30000, 0x18000);
	Actor_SetSpeed(12, 0x30000, 0x18000);
	Actor_SetSpeed(13, 0x30000, 0x18000);
	Actor_SetSpeed(14, 0x30000, 0x18000);
	Actor_SetSpeed(15, 0x30000, 0x18000);
	Engine_ActorEnableActionCallback(9, TolbiShipMast_EnsembleWalk9);
	Engine_ActorEnableActionCallback(10, TolbiShipMast_EnsembleWalk10);
	Engine_ActorEnableActionCallback(11, TolbiShipMast_EnsembleWalk11);
	Engine_ActorEnableActionCallback(12, TolbiShipMast_EnsembleWalk12);
	Engine_ActorEnableActionCallback(13, TolbiShipMast_EnsembleWalk13);
	Engine_ActorEnableActionCallback(14, TolbiShipMast_EnsembleWalk14);
	Engine_ActorEnableActionCallback(15, TolbiShipMast_EnsembleWalk15);
	Engine_EventWait(40);
	Engine_ActorStartRepeatedMotion(8, 3);
	Actor_SetAttachedEffect(8, 258);
	Engine_EventWait(120);
	Engine_ActorStartRepeatedMotion(8, 1);
	Actor_ShowEmote(8, 256, 60);
	Actor_SetSpeed(8, 0x10000, 0x8000);
	Actor_WalkToAndWait(8, 164, 344);
	Engine_ActorJump(8, 4, 10);
	Engine_ActorJump(8, 6, 20);
	Engine_EventSetMessage(MsgTolbiShipLandHo);
	Event_ShowMessageAndWait(8, 0, 20);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
	gGameState.savedEntrance = 2;
	if (SceneData_GetDifferenceOfPairSums() == 11)
	{
		Engine_EventRequestExit(15);
	}
	else
	{
		Engine_EventRequestExit(14);
	}
	Engine_EventEnd();
}

static void TolbiShipMast_UpdateSway(void)
{
	s32 *pos = *((s32 **)gMapWork[0]);
	s32	 dx = Engine_MathCos(TolbiShipMast_SwayX);
	s32	 dy = Engine_MathSin(TolbiShipMast_SwayY);

	*pos++ += dx;
	dy <<= 2;
	*pos += dy;
	TolbiShipMast_DriftX += dx;
	TolbiShipMast_DriftY += dy;
	TolbiShipMast_SwayX += (u32)(Engine_RandomNext() * 3 << 7) >> 16;
	TolbiShipMast_SwayY += (u32)(Engine_RandomNext() << 9) >> 16;
	TolbiShipMast_SwayX &= 0xffff;
	TolbiShipMast_SwayY &= 0xffff;
}

static s32 SceneData_GetDifferenceOfPairSums(void)
{
	s32 a;
	s32 b;

	a = SceneData_GetValueByFirstSetFlag(0);
	a += SceneData_GetValueByFirstSetFlag(2);
	b = SceneData_GetValueByFirstSetFlag(1);
	b += SceneData_GetValueByFirstSetFlag(3);
	return a - b;
}

static s32 SceneData_GetValueByFirstSetFlag(u32 a)
{
	s32 base = 0;
	u32 i;

	switch (a)
	{
	case 0:
		base = 0x92c;
		break;
	case 1:
		base = 0x935;
		break;
	case 2:
		base = 0x917;
		break;
	case 3:
		base = 0x990;
		break;
	}
	for (i = 0; i <= 8; i++)
	{
		if (Engine_GameFlagIsSet(base + i) != 0)
			return TolbiShipMast_FlagValues[i];
	}
	return 0;
}

s32 TolbiShipMast_FollowCameraActions[] = {
	SC_ADD(SW_SCALE_X, 2048),
	SC_ADD(SW_SCALE_Y, 2048),
	SC_CALL(Object_PlaceFromCameraOffset),
	SC_WAIT(1),
	SC_LOOP(10, SC_TOP),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -2048),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_CALL(Object_PlaceFromCameraOffset),
	SC_WAIT(1),
	SC_LOOP(24, SC_TO(1)),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 TolbiShipMast_LookoutActions[] = {
	SC_CALL(SceneActor_SetFacingFromSample),
};

s32 TolbiShipMast_DriftActions[] = {
	SC_CALL(TolbiShipMast_SwayActor),
};

s32 TolbiShipMast_WaveActions[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_ADD(SW_SCALE_X, -0x5000),
	SC_ADD(SW_SCALE_Y, 0x1000),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x2800),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x1400),
	SC_ADD(SW_SCALE_Y, -1024),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x1400),
	SC_ADD(SW_SCALE_Y, -1024),
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 TolbiShipMast_EnsembleWalk9[] = {
	SC_MOVE(PIXELS(72), 0, PIXELS(216)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(196)),
	SC_CALL(OverlayObject_ShrinkScaleThenStop),
	SC_END,
};

s32 TolbiShipMast_EnsembleWalk10[] = {
	SC_MOVE(PIXELS(132), 0, PIXELS(242)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(196)),
	SC_CALL(OverlayObject_ShrinkScaleThenStop),
	SC_END,
};

s32 TolbiShipMast_EnsembleWalk11[] = {
	SC_MOVE(PIXELS(88), 0, PIXELS(166)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(196)),
	SC_CALL(OverlayObject_ShrinkScaleThenStop),
	SC_END,
};

s32 TolbiShipMast_EnsembleWalk12[] = {
	SC_MOVE(PIXELS(220), 0, PIXELS(134)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(196)),
	SC_CALL(OverlayObject_ShrinkScaleThenStop),
	SC_END,
};

s32 TolbiShipMast_EnsembleWalk13[] = {
	SC_MOVE(PIXELS(264), 0, PIXELS(228)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(196)),
	SC_CALL(OverlayObject_ShrinkScaleThenStop),
	SC_END,
};

s32 TolbiShipMast_EnsembleWalk14[] = {
	SC_MOVE(PIXELS(282), 0, PIXELS(168)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(196)),
	SC_CALL(OverlayObject_ShrinkScaleThenStop),
	SC_END,
};

u32 TolbiShipMast_EnsembleWalk15[] = {
	3, 0x9a0000, 0, 0x870000, 1, 3, 0x980000, 0,
	0xc40000, 34, (u32)OverlayObject_ShrinkScaleThenStop, 16, 0x9090000, 0xd70090d, 0xdb315cb, 0x15f5164c,
	0x1f0e1aad, 0x1f6f227a, 0x23f223b1, 0x2bf727f4,
};

struct SceneEntrance TolbiShipMast_SceneTableA[] = {
	{ 0, CONDITION_ALWAYS, 165, 0, 235, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 184, 56, 352, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 12, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 13, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 14, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

s16 TolbiShipMast_SceneTableB[] = {
	176, 16, 348, 192, 32, 364, -1, 1,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 TolbiShipMast_SceneTableC[] = {
	SCENE_EXITS(110),
	SCENE_EXIT(1, 109, 4),
	SCENE_EXIT(10, 111, 10),
	SCENE_EXIT(11, 111, 13),
	SCENE_EXIT(12, 109, 13),
	SCENE_EXIT(13, 109, 14),
	SCENE_EXIT(14, 109, 17),
	SCENE_EXIT(15, 109, 19),
	SCENE_EXITS_END,
};

struct ScenePlacement TolbiShipMast_SceneTableD[] = {
	{ 0x91, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement TolbiShipMast_StagingObjects[] = {
	{ 0x11e, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbd, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbd, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xbd, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

struct ScenePlacement TolbiShipMast_EnsembleObjects[] = {
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(144), 0, PIXELS(169), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(134), 0, PIXELS(189), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(124), 0, PIXELS(209), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(129), 0, PIXELS(174), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(139), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(149), 0, PIXELS(204), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(154), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x42, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(182), 0, PIXELS(420), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement TolbiShipMast_LandingObjects[] = {
	{ 0xc5, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent TolbiShipMast_SceneTableE[] = {
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
	{ EVENT_TALK, 8, 0x928, MSGNO(MsgTolbiShipWatchSeasHelpKaja) },
	{ EVENT_TALK, 8, 0x8a0, MSGNO(MsgTolbiShipLookoutCannotRow) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipIslandMonstersUnknown) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x928, MSGNO(MsgTolbiShipHiddenAnchorDiscovered) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x8a0, MSGNO(MsgTolbiShipSeaTooVast) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipIslandTreasures) },
	{ SEARCH(SEARCH_UNNAMED), 11, 0x924, EVENT_SCRIPT(EV11_SEARCH) },
	{ SCENE_EVENTS_END },
};

u32 TolbiShipMast_FlagValues[] = {
	7, 5, 8, 4, 9, 12, 6, 3,
	0,
};
