#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/mercury_aerie.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/scene/common/object_nearest.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "field_event.h"
#include "engine_call.h"
#include "field_scene.h"
#include "overlay_object.h"
#include "create_configured_overlay_object.h"
#include "global_cells.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "iwram_call.h"
#include "map_scroll.h"
#include "text/msg_ids.h"
#include "field_obj.h"

// 前方宣言
static void FieldScene_RunFourActorPresentation(void);
static void EV11_TOUCH(void);
static void RunScene59Sequence(void);
static void MercuryAerie_SinkActorPair(void);
static void MercuryAerie_RiseActorPair(void);
static void MercuryAerie_LowerCollectedActors(void);
static void MercuryAerie_RaiseCollectedActors(void);
static void MercuryAerie_FlickerActorEight(void);

#define NULL ((void *)0)

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/effect_create.h"

struct EffectParams
{
	s32 count;
	s32 kind;
	s32 spread;
	s32 rise;
	u8	pad10[8];
	u16 tile;
	u16 pad1a;
	u8	pad1c[12];
};

struct ActorTable
{
	u8				   unk00[0x14];
	struct FieldActor *actors[66];
};

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/effect_spawn.h"
#include "objcmd.h"

const struct SceneEntrance *Scene_GetEntrances(void)
{
	return gMercuryAerieEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return gMercuryAerieExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	if (gGameState.entrance != 1)
	{
		Engine_GameFlagSet(0x253);
	}
	return gMercuryAeriePlacements;
}

const struct SceneEvent *Scene_GetEvents(void)
{
	return gMercuryAerieEvents;
}

s32 Scene_Initialize(void)
{
	u32				   i;
	s32				   actor;
	struct FieldActor *obj;

	Engine_GameFlagSet(0x111);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (gGameState.scene == (s32)&SceneId_MercuryAerie1)
	{
		Engine_GameFlagSet(0x144);
		FieldScheduler_AddCallback(MercuryAerie_FlickerActorEight, 0xc80);
		Engine_ActorSetSpritePriority(0, 1);
		Engine_ActorSetSpritePriority(1, 1);
		Engine_ActorSetSpritePriority(2, 1);
		Engine_ActorSetSpritePriority(3, 1);
		Engine_ActorSetSpritePriority(5, 1);
		Engine_ActorSetSpritePriority(20, 1);
		Engine_ActorSetSpritePriority(21, 1);
		Engine_ActorSetSpritePriority(22, 1);
		Engine_ActorSetSpritePriority(23, 1);
		Engine_ActorSetSpritePriority(24, 1);
		Engine_ActorSetSpritePriority(8, 1);
		Engine_ActorSetSpritePriority(9, 1);
		Engine_ActorSetSpritePriority(10, 1);
		Engine_ActorSetSpritePriority(11, 1);
		Engine_ActorSetSpritePriority(12, 1);
		Engine_ActorSetSpritePriority(13, 1);
		for (i = 14; i < 20; i++)
		{
			Engine_ActorSetSpritePriority(i, 1);
			Object_GetById(i)->motionFlags = 4;
			Object_GetById(i)->priorityFlags |= 2;
			Object_GetById(i)->y.fixed = -0x328000;
		}
		if (Engine_GameFlagIsSet(0x109) && (actor = SceneActor_FindNearestSlotOfKindF2()) != 0
			&& (obj = Object_GetById(actor)) != NULL)
		{
			obj->motionFlags = 0;
		}
		Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
		Object_GetById(12)->scaleX = -0x10000;
		Object_GetById(13)->scaleX = -0x10000;
		if (gGameState.entrance == 1)
		{
			if (!Engine_GameFlagIsSet(0x109))
			{
				FieldScene_RunFourActorPresentation();
			}
		}
		else if (gGameState.entrance == 2)
		{
			if (!Engine_GameFlagIsSet(0x251))
			{
				{
					struct MapLayerScroll *lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];

					lift->offsetY = 0x4000000;
				}
				Engine_MapRedraw();
				Engine_TaskWait(1);
				Map_CopyCellsTo(4, 70, 4, 74, 5, 4);
				Engine_ActorSetPosition(9, 0, 0);
				if (!Engine_GameFlagIsSet(0x109))
				{
					RunScene59Sequence();
				}
			}
		}
		else if (gGameState.entrance == 5)
		{
			Engine_GameFlagSet(0x251);
		}
	}
	return 0;
}

static void FieldScene_RunFourActorPresentation(void)
{
	struct FieldActor *leader;

	Engine_EventBegin();
	Owner_RefreshRatiosOnFlag();
	Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(1), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(2), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(3), 0);
	Camera_MoveTo(0x1300000, -1, 0x780000, 0);
	Engine_TaskWait(1);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Event_PlayCue(141);
	Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
	Event_PlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_MapRenderWaitForValues();
	Engine_EventWait(30);
	Engine_EventGetViewCenter()->motionFlags = 0;
	Camera_SetSpeed(0xcccc, 0x1999);
	Camera_MoveTo(0x2000000, -0x180000, 0xa00000, 1);
	Engine_CameraWaitForMove();
	ColorBuffer_ApplySource(0x10000, 0);
	ColorBuffer_ApplyTarget(0x10005, 0);
	Engine_ColorBufferInterpolate(50);
	Engine_EventWait(50);
	ColorBuffer_ApplyTarget(0x7fff, 0);
	Engine_ColorBufferInterpolate(30);
	Engine_EventWait(30);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1f80000, 0xa80000);
	Actor_SetPosition(GARET, 0x2100000, 0x900000);
	Actor_SetPosition(IVAN, 0x1e80000, 0x900000);
	Actor_SetPosition(MIA, 0x2000000, 0x980000);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
	Engine_ActorSetAnimation(GARET, 19);
	Engine_ActorSetAnimation(IVAN, 19);
	Engine_ActorSetAnimation(MIA, 19);
	Engine_EventWait(10);
	ColorBuffer_ApplyTarget(0x10000, 0);
	Engine_ColorBufferInterpolate(30);
	Engine_EventWait(30);
	Engine_EventWait(80);
	Engine_ActorSetSpriteFlags(Object_GetById(0), 1);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(60);
	Engine_ActorSetSpriteFlags(Object_GetById(1), 1);
	Engine_ActorSetAnimation(GARET, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(GARET, FACING_EAST, 0);
	Engine_ActorSetSpriteFlags(Object_GetById(2), 1);
	Engine_ActorSetAnimation(IVAN, 1);
	Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
	Engine_EventWait(40);
	Engine_ActorSetSpriteFlags(Object_GetById(3), 1);
	Engine_ActorSetAnimation(MIA, 1);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_EventWait(20);
	Actor_SetSpeed(GARET, 0xcccc, 0x6666);
	Actor_SetSpeed(IVAN, 0xcccc, 0x6666);
	Actor_SetSpeed(MIA, 0xcccc, 0x6666);
	Engine_ActorSetAnimation(GARET, 2);
	Engine_ActorSetAnimation(IVAN, 2);
	Engine_ActorSetAnimation(MIA, 2);
	leader = Object_GetById(ACTOR_PARTY_LEADER);
	if (leader != 0)
		Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
	leader = Object_GetById(ACTOR_PARTY_LEADER);
	if (leader != 0)
		Actor_SetDestination(IVAN, leader->x.part.pixel, leader->z.part.pixel);
	leader = Object_GetById(ACTOR_PARTY_LEADER);
	if (leader != 0)
		Actor_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
	Engine_ActorWaitForMove(MIA);
	Actor_SetPosition(MIA, 0, 0);
	Engine_ActorWaitForMove(IVAN);
	Actor_SetPosition(IVAN, 0, 0);
	Engine_ActorWaitForMove(GARET);
	Actor_SetPosition(GARET, 0, 0);
	Engine_EventEnd();
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/object_distance.h"
#include "common/object_nearest.h"

static void SceneActor_InitializeMotion(void)
{
	struct FieldActor *actor;
	s32				   position[3];
	s32				   angle;
	u8				   flags;

	actor = Object_GetById(ACTOR_PARTY_LEADER);
	angle = (actor->facing + 0x1000) & 0xe000;
	flags = actor->motionFlags;
	position[0] = (actor->x.fixed & 0xfff00000) + 0x80000;
	position[1] = actor->y.fixed;
	position[2] = (actor->z.fixed & 0xfff00000) + 0x80000;
	Vector_AddPolarOffset(0x200000, angle, position);
	if (FieldObject_CheckMovementCollision(actor, position) == 0)
	{
		Engine_GameFlagClear(592);
		EV11_TOUCH();
		Object_SetMode(actor, 6);
		Engine_TaskWait(6);
		Engine_AudioPlayCue(152);
		Object_SetMode(actor, 7);
		actor->speed = 0x30000;
		actor->acceleration = 0x20000;
		actor->velocityY = 0x40000;
		actor->motionFlags &= 0x7e;
		Engine_ActorSetSpriteFlags(actor, 0);
		Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, position[0] >> 16, position[2] >> 16);
		Object_SetMode(actor, 6);
		Engine_ActorSetSpriteFlags(actor, 1);
		actor->motionFlags = flags;
	}
}

static void EV10_TOUCH(void)
{
	struct FieldActor *leader;
	struct FieldActor *actor;
	s32				   id;

	leader = Object_GetById(ACTOR_PARTY_LEADER);
	Engine_EventBegin();
	id = SceneActor_FindNearestSlotOfKindF2();
	gMercuryAerie_NearestActor = id;
	if (id != 0)
	{
		Engine_GameFlagSet(0x250);
		actor = Object_GetById(gMercuryAerie_NearestActor);
		actor->motionFlags = 0;
		leader->motionFlags &= 0xfe;
		actor->y.fixed -= 0x30000;
		leader->y.fixed -= 0x30000;
		leader->terrainHeight -= 0x30000;
		Engine_TaskWait(2);
		actor->y.fixed -= 0x20000;
		leader->y.fixed -= 0x20000;
		leader->terrainHeight -= 0x20000;
		Engine_TaskWait(10);
		actor->y.fixed += 0x20000;
		leader->y.fixed += 0x20000;
		leader->terrainHeight += 0x20000;
		Engine_TaskWait(4);
		actor->y.fixed += 0x20000;
		leader->y.fixed += 0x20000;
		leader->terrainHeight += 0x20000;
		Engine_TaskWait(4);
		actor->y.fixed += 0x10000;
		leader->y.fixed += 0x10000;
		leader->terrainHeight += 0x10000;
	}
	Engine_EventEnd();
}

static void EV11_TOUCH(void)
{
	Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 3;
	Object_GetById(14)->motionFlags = 4;
	Object_GetById(15)->motionFlags = 4;
	Object_GetById(16)->motionFlags = 4;
	Object_GetById(17)->motionFlags = 4;
	Object_GetById(18)->motionFlags = 4;
	Object_GetById(19)->motionFlags = 4;
}

static void EV05_TOUCH(void)
{
	struct FieldActor *actor;

	Engine_EventBegin();
	Actor_Get(0x11)->motionFlags &= 0xfa;
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x0000cccc, 0x00006666);
	Actor_SetSpeed(GARET, 0x0000cccc, 0x00006666);
	Actor_SetSpeed(IVAN, 0x0000cccc, 0x00006666);
	Actor_SetSpeed(MIA, 0x0000cccc, 0x00006666);
	actor = Actor_Get(ACTOR_PARTY_LEADER);
	if (actor != 0)
	{
		Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
	}
	actor = Actor_Get(ACTOR_PARTY_LEADER);
	if (actor != 0)
	{
		Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
	}
	actor = Actor_Get(ACTOR_PARTY_LEADER);
	if (actor != 0)
	{
		Actor_SetPosition(MIA, actor->x.fixed, actor->z.fixed);
	}
	Engine_TaskWait(1);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x158, 0xE8);
	Actor_WalkTo(GARET, 0x148, 0xE8);
	Actor_WalkTo(IVAN, 0x158, 0xF8);
	Actor_WalkTo(MIA, 0x148, 0xF8);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Engine_ActorWaitForMove(GARET);
	Actor_FaceDirection(GARET, FACING_NORTH, 0);
	Engine_ActorWaitForMove(IVAN);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
	Engine_ActorWaitForMove(MIA);
	Actor_FaceDirection(MIA, FACING_NORTH, 0);
	Engine_EventWait(50);
	Engine_ActorRunRepeatedMotion(GARET, 2);
	Engine_EventWait(20);
	Camera_SetSpeed(0x18000, 0x3000);
	Camera_MoveTo(PIXELS(0x148), PIXELS(0x28), PIXELS(0xB0), 1);
	Engine_EventGetViewCenter()->motionFlags = 0;
	Actor_SetSpeed(GARET, 0x18000, 0xC000);
	Actor_WalkToAndWait(GARET, 0x148, 0xD8);
	Engine_CameraWaitForMove();
	Actor_SetSpeed(GARET, 0x0000cccc, 0x00006666);
	Engine_EventWait(60);
	Camera_MoveTo(PIXELS(0x158), PIXELS(0x18), PIXELS(0xE8), 1);
	Actor_FaceDirection(GARET, FACING_SOUTHEAST, 20);
	Engine_CameraWaitForMove();
	Engine_ActorSetAnimationAndWait(GARET, 4);
	Engine_EventSetMessage(MsgMercuryLighthouseAlreadyLit);
	Event_ShowMessageAndWait(GARET, 0, 20);
	Actor_SetAttachedEffect(MIA, 0x102);
	Engine_EventWait(60);
	Actor_SetSpeed(MIA, 0x18000, 0xC000);
	Actor_WalkToAndWait(MIA, 0x148, 0xE8);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Engine_ActorSetAnimationAndWait(MIA, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MIA, 0, 30);
	Engine_ActorFaceEachOther(2, 0, 0);
	Engine_EventWait(60);
	Actor_WalkToAndWait(IVAN, 0x150, 0xF8);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Engine_EventWait(30);
	Event_ShowMessageAndWait(IVAN, 0, 20);
	Actor_FaceDirection(MIA, FACING_EAST, 30);
	Engine_EventOpenMessage(3, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Actor_ShowEmote(MIA, 0x100, 60);
		Actor_FaceDirection(GARET, FACING_SOUTH, 10);
		Engine_ActorSetAnimationAndWait(GARET, 3);
		Event_ShowMessageAndWait(GARET, 0, 20);
		Actor_FaceDirection(MIA, FACING_NORTH, 0);
		Engine_ActorSetAnimationAndWait(GARET, 4);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(GARET, 0, 20);
		gEventWork->message += 2;
	}
	else
	{
		gEventWork->message += 2;
		Engine_EventWait(20);
		Actor_ShowEmote(MIA, 0x00000101, 60);
		Actor_FaceDirection(GARET, FACING_SOUTH, 10);
		Engine_ActorSetAnimationAndWait(GARET, 4);
		Event_ShowMessageAndWait(GARET, 0, 20);
		Actor_FaceDirection(MIA, FACING_NORTH, 0);
		Engine_ActorRunRepeatedMotion(GARET, 2);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(GARET, 0, 20);
	}
	Actor_ShowEmote(MIA, 0x00000101, 60);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Engine_ActorFaceEachOther(0, 1, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GARET, FACING_SOUTH, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(GARET, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(GARET, 0, 20);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GARET, 3);
	Engine_ActorSetAnimationAndWait(IVAN, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(MIA, 4);
	Engine_EventWait(10);
	Actor_FaceDirection(MIA, FACING_EAST, 10);
	Engine_ActorSetAnimation(MIA, 0x10);
	Event_ShowMessageAndWait(MIA, 0, 60);
	Event_PlayCue(0x11);
	Actor_SetPosition(5, PIXELS(0xD8), PIXELS(0xC8));
	Event_ShowMessageAndWait(JENNA, 0, 20);
	Actor_SetPosition(5, PIXELS(0x78), PIXELS(0xA0));
	Actor_FaceDirection(5, FACING_EAST, 0);
	Engine_ActorSetAnimation(MIA, 1);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_ActorRunRepeatedMotion(GARET, 1);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 10);
	Actor_FaceDirection(GARET, FACING_NORTHEAST, 20);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 10);
	Actor_FaceDirection(MIA, FACING_WEST, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 5);
	Actor_FaceDirection(GARET, FACING_NORTH, 10);
	Actor_FaceDirection(IVAN, FACING_WEST, 5);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 5);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Actor_FaceDirection(GARET, FACING_WEST, 10);
	Engine_ActorRunRepeatedMotion(GARET, 2);
	Engine_EventWait(20);
	Camera_SetSpeed(0x30000, 0x6000);
	Camera_MoveTo(PIXELS(0x78), PIXELS(0xFFE8), PIXELS(0xA8), 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(0x15, 1);
	Engine_EventWait(20);
	Event_PlayCue(0x3D);
	Event_ShowMessageAndWait(0x15, 0, 20);
	Engine_ActorRunRepeatedMotion(0x17, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x17, 0, 20);
	Actor_FaceDirection(0x17, FACING_NORTH, 60);
	Actor_FaceDirection(0x17, FACING_EAST, 20);
	Event_ShowMessageAndWait(0x17, 0, 20);
	Actor_SetPosition(GARET, PIXELS(0x108), PIXELS(0x120));
	Event_ShowMessageAndWait(GARET, 0, 20);
	Actor_SetPosition(GARET, PIXELS(0x148), PIXELS(0xD8));
	Engine_ActorSetAnimationAndWait(0x17, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x17, 0, 20);
	Engine_ActorSetAnimationAndWait(0x17, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x17, 0, 20);
	Actor_FaceDirection(0x17, FACING_NORTHWEST, 10);
	Actor_FaceDirection(0x15, FACING_SOUTHEAST, 10);
	Engine_ActorRunRepeatedMotion(0x15, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(5, FACING_WEST, 0);
	Actor_FaceDirection(0x14, FACING_WEST, 30);
	Actor_FaceDirection(0x15, FACING_EAST, 30);
	Engine_ActorRunRepeatedMotion(5, 2);
	Engine_ActorSetAnimationAndWait(0x15, 4);
	Engine_EventWait(20);
	Actor_FaceDirection(0x15, FACING_SOUTHEAST, 10);
	Engine_ActorSetAnimationAndWait(0x15, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(0x17, 3);
	Actor_SetSpeed(0x15, 0x0000cccc, 0x00006666);
	Actor_WalkToAndWait(0x15, 0x68, 0xA8);
	Engine_EventWait(20);
	Actor_FaceDirection(0x17, FACING_EAST, 20);
	Actor_FaceDirection(0x14, FACING_EAST, 10);
	Actor_ShowEmote(5, 0x102, 0);
	Actor_ShowEmote(0x14, 0x102, 70);
	Actor_SetPosition(0x16, PIXELS(0x108), PIXELS(0x120));
	Event_ShowMessageAndWait(0x16, 0, 20);
	Actor_SetPosition(0x16, PIXELS(0x128), PIXELS(0x78));
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GARET, FACING_NORTH, 0);
	Actor_FaceDirection(IVAN, FACING_NORTH, 0);
	Actor_FaceDirection(MIA, FACING_NORTH, 0);
	Actor_FaceDirection(5, FACING_EAST, 0);
	Actor_FaceDirection(0x14, FACING_EAST, 0);
	Camera_SetSpeed(0x18000, 0x3000);
	Camera_MoveTo(PIXELS(0xE8), PIXELS(0x28), PIXELS(0x98), 1);
	Actor_SetSpeed(0x16, 0x0000cccc, 0x00006666);
	Actor_WalkToAndWait(0x16, 0x110, 0x80);
	Actor_WalkToAndWait(0x16, 0x108, 0x98);
	Actor_WalkToAndWait(0x16, 0x118, 0xA8);
	Engine_CameraWaitForMove();
	Actor_FaceDirection(0x16, 0x5000, 20);
	Engine_EventWait(20);
	Event_ShowMessage(0x17, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(0x16, 3);
	Engine_EventWait(20);
	Event_ShowMessage(0x16, 0);
	Camera_MoveTo(PIXELS(0x128), PIXELS(0x28), PIXELS(0xD8), 1);
	Actor_WalkToAndWait(0x16, 0x130, 0xB0);
	Actor_FaceDirection(0x16, FACING_SOUTHEAST, 0);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_ActorStartRepeatedMotion(GARET, 1);
	Engine_ActorStartRepeatedMotion(IVAN, 1);
	Engine_ActorRunRepeatedMotion(MIA, 1);
	Engine_EventWait(20);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(0x16, 3);
	Engine_EventWait(20);
	Event_ShowMessage(0x16, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x00000107, 0);
	Actor_ShowEmote(GARET, 0x00000107, 0);
	Actor_ShowEmote(MIA, 0x00000107, 0);
	Actor_ShowEmote(IVAN, 0x00000107, 70);
	Engine_ActorRunRepeatedMotion(0x16, 1);
	Engine_EventWait(30);
	Event_ShowMessage(0x16, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x00000105, 0);
	Actor_ShowEmote(GARET, 0x00000105, 0);
	Actor_ShowEmote(IVAN, 0x00000105, 0);
	Actor_ShowEmote(MIA, 0x00000105, 70);
	Engine_ActorSetAnimationAndWait(0x16, 3);
	Engine_EventWait(20);
	Event_ShowMessage(0x16, 0);
	Engine_EventWait(20);
	Event_ShowMessage(0x17, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(0x16, 0x5000, 0);
	Engine_EventWait(40);
	Event_ShowMessage(0x16, 0);
	Engine_EventWait(30);
	Event_ShowMessage(0x17, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(0x16, 0x3000, 0);
	Engine_EventWait(30);
	Event_ShowMessage(0x16, 0);
	Engine_EventWait(20);
	Actor_Get(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
	Actor_WalkTo(GARET, 0x148, 0xE0);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x158, 0xE0);
	Actor_WalkTo(IVAN, 0x158, 0xE8);
	Engine_ActorWaitForMove(GARET);
	Actor_Get(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
	Actor_FaceDirection(GARET, FACING_NORTH, 0);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Engine_ActorWaitForMove(IVAN);
	Actor_FaceDirection(IVAN, FACING_NORTH, 0);
	Engine_EventWait(30);
	Actor_SetPosition(0x17, PIXELS(0xA8), PIXELS(0xC8));
	Event_ShowMessageAndWait(0x17, 0, 20);
	Actor_SetPosition(0x17, PIXELS(0x68), PIXELS(0xC8));
	Engine_ActorRunRepeatedMotion(0x16, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x16, 0, 20);
	Camera_SetSpeed(0x30000, 0x6000);
	Camera_MoveTo(PIXELS(0x78), PIXELS(0xFFE8), PIXELS(0xA8), 1);
	Engine_CameraWaitForMove();
	Engine_ActorSetAnimationAndWait(0x17, 3);
	Engine_EventWait(10);
	Actor_FaceDirection(0x17, 0xD000, 20);
	Engine_ActorSetAnimationAndWait(0x17, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x17, 0, 20);
	Actor_FaceDirection(5, 0x5000, 0);
	Actor_FaceDirection(0x14, 0x3000, 70);
	Engine_ActorFaceEachOther(5, 0x14, 0);
	Engine_EventWait(50);
	Actor_FaceDirection(5, 0x5000, 0);
	Actor_FaceDirection(0x14, 0x3000, 20);
	Engine_ActorSetAnimationAndWait(JENNA, 4);
	Engine_EventWait(20);
	Actor_ShowEmote(0x17, 0x100, 60);
	Event_ShowMessageAndWait(0x17, 0, 20);
	Actor_ShowEmote(5, 0x00000105, 60);
	Actor_FaceDirection(0x15, 0xD000, 20);
	Engine_ActorRunRepeatedMotion(0x15, 1);
	Engine_ActorSetAnimationAndWait(0x17, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x17, 0, 20);
	Engine_ActorSetAnimationAndWait(0x17, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x17, 0, 20);
	Actor_SetAttachedEffect(0x15, 0x102);
	Engine_EventWait(60);
	Engine_ActorSetAnimationAndWait(0x15, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x15, 0, 20);
	Actor_SetAttachedEffect(JENNA, 0x102);
	Engine_EventWait(60);
	Event_ShowMessageAndWait(5, 0, 40);
	Engine_ActorSetAnimationAndWait(0x15, 3);
	Engine_EventWait(60);
	Actor_FaceDirection(5, FACING_NORTHWEST, 10);
	Engine_ActorSetAnimationAndWait(0x14, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x14, 0, 30);
	Actor_SetSpeed(5, 0x0000b333, 0x00005999);
	Actor_SetSpeed(0x14, 0x0000b333, 0x00005999);
	Actor_WalkTo(5, 0x80, 0x90);
	Actor_WalkToAndWait(0x14, 0x78, 0x88);
	Actor_FaceDirection(0x14, 0x5000, 0);
	Engine_ActorWaitForMove(5);
	Actor_FaceDirection(JENNA, FACING_EAST, 20);
	Actor_FaceDirection(0x15, 0x3000, 20);
	Object_GetById(0x15)->steeringFlags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(0x15, 0x58, 0x98);
	Actor_Get(0x15)->steeringFlags |= ACTOR_FACES_MOTION;
	Actor_FaceDirection(0x17, 0xB000, 20);
	Engine_ActorSetAnimation(0x17, 3);
	Engine_ActorSetAnimationAndWait(0x15, 3);
	Engine_EventWait(40);
	Actor_SetSpeed(0x17, 0x30000, 0x20000);
	Actor_Get(0x17)->velocityY = 0x40000;
	Event_PlayCue(0x98);
	Actor_Get(0x17)->motionFlags &= 0x7e;
	Engine_ActorSetSpriteFlags(Object_GetById(0x17), 0);
	Object_GetById(0x11)->motionFlags = 4;
	Engine_ActorMoveToAndWait(0x17, 0x68, 0xA8);
	Engine_ActorSetSpriteFlags(Object_GetById(0x17), 1);
	Actor_Get(0x17)->motionFlags = 3;
	Actor_FaceDirection(0x17, FACING_EAST, 30);
	Actor_FaceDirection(0x15, FACING_EAST, 10);
	Event_ShowMessageAndWait(0x15, 0, 20);
	Engine_ActorRunRepeatedMotion(5, 2);
	Engine_ActorSetAnimationAndWait(5, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(5, 0, 20);
	Event_ShowMessageAndWait(0x17, 0, 20);
	Engine_ActorSetSpritePriority(0x11, 0);
	Engine_ActorSetSpritePriority(0x12, 0);
	MercuryAerie_LowerCollectedActors();
	Engine_ActorSetSpritePriority(0x11, 1);
	Engine_ActorSetSpritePriority(0x12, 1);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(MIA, FACING_SOUTHWEST, 0);
	Camera_MoveTo(PIXELS(0x130), PIXELS(0x20), PIXELS(0xD8), 1);
	Engine_CameraWaitForMove();
	Actor_SetPosition(0x14, PIXELS(0x110), PIXELS(0x118));
	Event_ShowMessageAndWait(0x14, 0, 20);
	Actor_SetPosition(0x14, 0, 0);
	Engine_ActorRunRepeatedMotion(GARET, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(GARET, 0, 20);
	Actor_SetSpeed(GARET, 0x18000, 0xC000);
	Actor_WalkToAndWait(GARET, 0x138, 0xD8);
	Actor_FaceDirection(GARET, FACING_SOUTHWEST, 10);
	Event_ShowMessageAndWait(0x16, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GARET, FACING_NORTH, 0);
	Actor_FaceDirection(IVAN, FACING_NORTH, 0);
	Actor_FaceDirection(MIA, FACING_NORTH, 0);
	Actor_WalkToAndWait(0x16, 0x138, 0xB8);
	Actor_FaceDirection(0x16, 0x3000, 20);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Actor_ShowEmote(GARET, 0x100, 60);
	Actor_SetSpeed(GARET, 0x20000, 0x10000);
	Actor_Get(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(GARET, 0x148, 0xE0);
	Engine_EventWait(1);
	Actor_Get(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_ActorStartRepeatedMotion(GARET, 1);
	Engine_ActorStartRepeatedMotion(IVAN, 1);
	Engine_ActorRunRepeatedMotion(MIA, 1);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(0x16, 1);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(0x16, 0, 20);
	Engine_ActorRunRepeatedMotion(GARET, 2);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(GARET, 4);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(GARET, 0, 20);
	Engine_ActorRunRepeatedMotion(IVAN, 1);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(IVAN, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IVAN, 0, 20);
	Actor_ShowEmote(0x16, 0x00000101, 60);
	Engine_EventOpenMessage(0x16, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(0x16, 4);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(0x16, 0, 20);
		gEventWork->message++;
	}
	else
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(0x16, 4);
		Engine_EventWait(20);
		gEventWork->message++;
		Event_ShowMessageAndWait(0x16, 0, 20);
	}
	Engine_ActorRunRepeatedMotion(0x16, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x16, 0, 20);
	Actor_WalkToAndWait(0x16, 0x148, 0xC8);
	Engine_ActorRunRepeatedMotion(0x16, 2);
	Actor_FaceDirection(0x16, 0xB000, 20);
	Actor_Get(0x16)->steeringFlags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(0x16, 0x150, 0xD0);
	Engine_EventWait(1);
	Object_GetById(0x16)->steeringFlags |= ACTOR_FACES_MOTION;
	Actor_ShowEmote(0x16, 0x102, 60);
	Actor_ShowEmote(MIA, 0x00000101, 60);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Event_ShowMessageAndWait(0x16, 0, 20);
	Actor_FaceDirection(0x16, 0x5000, 20);
	Engine_ActorRunRepeatedMotion(0x16, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x16, 0, 20);
	Actor_WalkToAndWait(0x16, 0x150, 0xD8);
	Party_SetFields1ceAnd1d0((s32)&SceneId_MercuryAerie1, 2);
	Battle_SetStart(3);
	BattleFx_SetWeightedResult(0x24, 2);
	Engine_EventEnd();
}

static void RunScene59Sequence(void)
{

	Engine_EventBegin();
	Actor_Get(9)->motionFlags = 0;
	Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(0x158), PIXELS(0xE0));
	Actor_SetPosition(GARET, PIXELS(0x148), PIXELS(0xE0));
	Actor_SetPosition(IVAN, PIXELS(0x158), PIXELS(0xE8));
	Actor_SetPosition(MIA, PIXELS(0x148), PIXELS(0xE8));
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GARET, FACING_NORTH, 0);
	Actor_FaceDirection(IVAN, FACING_NORTH, 0);
	Actor_FaceDirection(MIA, FACING_NORTH, 0);
	Actor_SetPosition(0x16, PIXELS(0x150), PIXELS(0xB0));
	Engine_ActorSetAnimation(0x16, 9);
	Engine_ActorSetSpriteFlags(Object_GetById(0x16), 0);
	Camera_MoveTo(PIXELS(0x150), -1, PIXELS(0xD0), 0);
	Engine_TaskWait(1);
	Engine_MapRedraw();
	Engine_TaskWait(1);

	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(GARET, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(GARET, FACING_EAST, 20);
	Engine_EventSetMessage(MsgMercuryGot);
	Event_ShowMessageAndWait(GARET, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(0x16, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x16, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GARET, FACING_NORTH, 20);
	Actor_ShowEmote(IVAN, 0x00000101, 60);
	Event_ShowMessageAndWait(IVAN, 0, 20);
	Engine_ActorRunRepeatedMotion(0x16, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x16, 0, 30);
	Actor_SetPosition(0x18, PIXELS(0x138), PIXELS(0x70));
	Event_PlayCue(0x120);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Actor_ShowEmote(MIA, 0x100, 60);
	Event_ShowMessageAndWait(MIA, 0, 30);
	Event_PlayCue(0x1D);
	Camera_SetSpeed(0x0000cccc, 0x00001999);
	Engine_EventGetViewCenter()->motionFlags = 0;
	Camera_MoveTo(PIXELS(0x150), -1, PIXELS(0xA8), 1);
	Engine_EventWait(20);
	Actor_SetSpeed(0x18, 0x0000cccc, 0x00006666);
	Actor_WalkToAndWait(0x18, 0x158, 0x88);
	Actor_FaceDirection(0x18, 0x5000, 20);
	Engine_CameraWaitForMove();
	Event_ShowMessageAndWait(MIA, 0, 30);
	Camera_MoveTo(PIXELS(0x150), -1, PIXELS(0xB8), 1);
	Actor_WalkToAndWait(0x18, 0x158, 0xA0);
	Actor_WalkToAndWait(0x18, 0x148, 0xA8);
	Actor_WalkToAndWait(0x18, 0x138, 0xB0);
	Actor_FaceDirection(0x18, 0x3000, 20);
	Engine_CameraWaitForMove();
	Engine_ActorSetAnimationAndWait(0x18, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GARET, FACING_SOUTH, 0);
	Actor_FaceDirection(IVAN, FACING_WEST, 20);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Engine_ActorSetAnimationAndWait(0x18, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
	Actor_FaceDirection(MIA, FACING_NORTHEAST, 60);
	Actor_FaceDirection(0x18, 0xB000, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GARET, FACING_NORTH, 0);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
	Actor_FaceDirection(MIA, FACING_NORTH, 20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Engine_ActorSetAnimationAndWait(MIA, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Engine_ActorStartRepeatedMotion(0x18, 2);
	Actor_ShowEmote(0x18, 0x100, 60);
	Actor_FaceDirection(0x18, 0x3000, 20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Actor_ShowEmote(MIA, 0x00000101, 60);
	Engine_ActorSetAnimationAndWait(0x18, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Engine_ActorSetAnimationAndWait(0x18, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Engine_ActorRunRepeatedMotion(MIA, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Actor_WalkToAndWait(0x18, 0x138, 0xB8);
	Actor_FaceDirection(0x18, 0x3000, 20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Engine_ActorRunRepeatedMotion(GARET, 2);
	Engine_EventWait(10);
	Actor_FaceDirection(GARET, FACING_EAST, 20);
	Engine_EventOpenMessage(1, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_ActorRunRepeatedMotion(GARET, 2);
		Event_ShowMessageAndWait(GARET, 0, 20);
		gEventWork->message++;
	}
	else
	{
		gEventWork->message++;
		Engine_ActorRunRepeatedMotion(GARET, 1);
		Event_ShowMessageAndWait(GARET, 0, 20);
	}
	Event_ShowMessageAndWait(IVAN, 0, 20);
	Actor_FaceDirection(GARET, FACING_NORTH, 20);
	Engine_ActorSetAnimationAndWait(0x18, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Actor_FaceDirection(0x18, 0xD000, 20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Actor_FaceDirection(0x18, 0x3000, 20);
	Engine_ActorSetAnimationAndWait(0x18, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Actor_ShowEmote(MIA, 0x00000101, 60);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Engine_ActorSetAnimationAndWait(0x18, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Actor_FaceDirection(0x18, 0xB000, 20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(0x18, 2);
	Engine_EventWait(30);
	Event_ShowMessage(0x18, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(GARET, 2);
	Engine_EventWait(30);
	Event_ShowMessage(1, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(0x18, FACING_SOUTHEAST, 0);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(0x18, 4);
	Engine_EventWait(20);
	Event_ShowMessage(0x18, 0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(0x18, 2);
	Engine_EventWait(30);
	Event_ShowMessage(0x18, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(IVAN, 0x100, 60);
	Event_ShowMessage(2, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(0x18, 3);
	Engine_EventWait(30);
	Event_ShowMessage(0x18, 0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(0x18, 2);
	Engine_EventWait(30);
	Event_ShowMessage(0x18, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(0x18, 0x1000, 0);
	Engine_EventWait(30);
	Actor_ShowEmote(0x18, 0x108, 60);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(0x16, 2);
	Engine_EventWait(30);
	Engine_ActorSetAnimation(0x16, 8);
	Engine_EventWait(45);
	Engine_ActorSetAnimation(0x16, 1);
	Engine_ActorSetSpriteFlags(Object_GetById(0x16), 1);
	Engine_EventWait(40);
	Actor_ShowEmote(IVAN, 0x102, 60);
	Event_ShowMessage(2, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(0x16, FACING_SOUTHEAST, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(0x16, 4);
	Engine_EventWait(30);
	Event_ShowMessage(0x16, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(0x18, FACING_SOUTHEAST, 0);
	Engine_EventWait(30);
	Event_ShowMessage(0x18, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Engine_EventWait(30);
	Event_ShowMessage(3, 0);
	Engine_EventWait(10);
	Engine_ActorFaceEachOther(0x18, 0x16, 0);
	Engine_EventWait(35);
	Engine_ActorSetAnimationAndWait(0x16, 3);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(0x18, 3);
	Engine_EventWait(20);
	Actor_WalkToAndWait(0x18, 0x148, 0xB0);
	Actor_FaceDirection(0x18, 0x3000, 20);
	Engine_ActorSetAnimation(0x18, 5);
	Engine_ActorSetAnimation(0x16, 7);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(GARET, 1);
	Engine_EventWait(20);
	Event_ShowMessage(1, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(0x18, 0x00000101, 60);
	Engine_EventOpenMessage(0x18, 0);
	Engine_EventWait(30);
	Engine_ActorStartRepeatedMotion(GARET, 2);
	Engine_ActorStartRepeatedMotion(IVAN, 2);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Actor_FaceDirection(GARET, FACING_EAST, 0);
	Actor_FaceDirection(IVAN, FACING_NORTH, 0);
	Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
	Engine_EventWait(20);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(30);
		Event_ShowMessage(0x18, 0);
		gEventWork->message++;
	}
	else
	{
		Engine_EventWait(30);

		gEventWork->message++;
		Event_ShowMessage(0x18, 0);
	}
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(0x18, 4);
	Engine_EventWait(20);
	Actor_FaceDirection(GARET, FACING_NORTH, 0);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
	Actor_FaceDirection(MIA, FACING_NORTH, 0);
	Engine_EventWait(20);
	Event_ShowMessage(0x18, 0);
	Engine_ActorRunRepeatedMotion(0x16, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x16, 0, 20);
	MercuryAerie_SinkActorPair();
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GARET, 2);
	Engine_ActorStartRepeatedMotion(IVAN, 2);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 5);
	Actor_FaceDirection(GARET, FACING_NORTHEAST, 5);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 5);
	Actor_FaceDirection(MIA, FACING_WEST, 5);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 5);
	Actor_FaceDirection(GARET, FACING_NORTHWEST, 5);
	Actor_FaceDirection(IVAN, FACING_WEST, 5);
	Actor_FaceDirection(MIA, FACING_EAST, 5);
	Engine_EventGetViewCenter()->motionFlags = 0;
	Camera_MoveTo(PIXELS(0x118), -1, PIXELS(0xE8), 1);
	Engine_CameraWaitForMove();
	Actor_SetChildValue(0x16, 0xF);
	Actor_SetChildValue(0x18, 0xF);
	Actor_SetPosition(0x16, PIXELS(0xF0), PIXELS(0xD0));
	Actor_SetPosition(0x18, PIXELS(0xE8), PIXELS(0xD0));
	Actor_FaceDirection(0x16, 0x5000, 0);
	Actor_FaceDirection(0x18, 0x3000, 0);
	MercuryAerie_RiseActorPair();
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
	Actor_FaceDirection(GARET, FACING_WEST, 15);
	Actor_FaceDirection(IVAN, FACING_WEST, 0);
	Actor_FaceDirection(MIA, FACING_WEST, 20);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x148, 0xD0);
	Actor_WalkTo(IVAN, 0x150, 0xE0);
	Actor_WalkToAndWait(GARET, 0x138, 0xD8);
	Actor_FaceDirection(GARET, FACING_WEST, 0);
	Engine_ActorWaitForMove(IVAN);
	Actor_FaceDirection(IVAN, FACING_WEST, 0);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
	Event_ShowMessageAndWait(GARET, 0, 20);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Actor_FaceDirection(GARET, 0x5000, 20);
	Event_ShowMessageAndWait(GARET, 0, 20);
	Actor_FaceDirection(GARET, FACING_WEST, 20);
	Engine_ActorRunRepeatedMotion(IVAN, 1);
	Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 20);
	Event_ShowMessageAndWait(IVAN, 0, 20);
	Engine_ActorSetAnimationAndWait(MIA, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Actor_FaceDirection(IVAN, FACING_WEST, 20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Camera_SetSpeed(0x30000, 0x6000);
	{
		struct MapLayerScroll *lift;
		void				 **cells;
		Camera_MoveTo(PIXELS(0x98), -1, PIXELS(0xD8), 1);
		lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];
		lift->offsetY = 0x3800000;
		cells = gMapWork;
		Engine_MapRedraw();
		Engine_TaskWait(1);
		Actor_Get(9)->motionFlags = 0;
		Actor_SetPosition(9, PIXELS(0x68), PIXELS(0x108));
		Actor_Get(9)->y.fixed = -0x200000;
		Object_GetById(9)->targetY = -0x200000;
		Map_CopyCellsTo(0x1D, 0x4A, 4, 0x4A, 5, 4);
		Engine_ActorSetSpritePriority(0x11, 0);
		Engine_ActorSetSpritePriority(0x12, 0);
		MercuryAerie_RaiseCollectedActors();
		Engine_ActorSetSpritePriority(0x11, 1);
		Engine_ActorSetSpritePriority(0x12, 1);
		Engine_CameraWaitForMove();
		Engine_EventWait(30);
		Actor_FaceDirection(0x18, FACING_WEST, 20);
		Event_ShowMessageAndWait(0x18, 0, 20);
		Camera_MoveTo(PIXELS(0x118), -1, PIXELS(0xD8), 1);
		Engine_CameraWaitForMove();
		Actor_FaceDirection(0x18, 0x3000, 20);
		Engine_ActorSetAnimation(0x18, 5);
		Event_ShowMessageAndWait(0x18, 0, 20);
		Engine_ActorRunRepeatedMotion(IVAN, 2);
		Engine_EventWait(20);
		Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
		Event_ShowMessageAndWait(IVAN, 0, 20);
		Engine_ActorRunRepeatedMotion(GARET, 1);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(GARET, 0, 20);
		Actor_ShowEmote(0x18, 0x00000101, 60);
		Engine_EventOpenMessage(0x18, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_EventWait(20);
			Engine_ActorRunRepeatedMotion(0x16, 2);
			Engine_EventWait(20);
			Event_ShowMessageAndWait(0x16, 0, 20);
			((struct EventWork *)cells[19])->message++;
		}
		else
		{
			((struct EventWork *)cells[19])->message++;
			Engine_EventWait(20);
			Event_ShowMessageAndWait(0x16, 0, 20);
		}
	}
	Engine_ActorRunRepeatedMotion(0x18, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x18, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x00000101, 0);
	Actor_ShowEmote(GARET, 0x00000101, 0);
	Actor_ShowEmote(IVAN, 0x00000101, 0);
	Actor_ShowEmote(MIA, 0x00000101, 60);
	Engine_EventOpenMessage(0x18, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(0x18, 2);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(0x18, 0, 20);
		gEventWork->message++;
	}
	else
	{
		gEventWork->message++;
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(0x18, 1);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(0x18, 0, 20);
	}
	Engine_EventWait(20);
	MercuryAerie_SinkActorPair();
	Engine_EventWait(20);
	Camera_MoveTo(PIXELS(0x80), -1, PIXELS(0xC8), 1);
	Engine_CameraWaitForMove();
	Engine_ActorSetAnimation(0x16, 1);
	Engine_ActorSetAnimation(0x18, 1);
	Actor_SetChildValue(0x16, 0xF);
	Actor_SetChildValue(0x18, 0xF);
	Actor_SetPosition(0x16, PIXELS(0x78), PIXELS(0x98));
	Actor_SetPosition(0x18, PIXELS(0x70), PIXELS(0xA0));
	Actor_FaceDirection(0x16, 0x5000, 0);
	Actor_FaceDirection(0x18, 0x3000, 0);
	MercuryAerie_RiseActorPair();
	Engine_EventWait(30);
	Actor_FaceDirection(0x18, FACING_EAST, 20);
	Engine_EventOpenMessage(0x18, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_ActorSetAnimationAndWait(0x18, 3);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(0x18, 0, 20);
		gEventWork->message++;
	}
	else
	{
		gEventWork->message++;
		Engine_ActorSetAnimationAndWait(0x18, 4);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(0x18, 0, 20);
	}
	Event_ShowMessageAndWait(0x18, 0, 20);
	Engine_ActorRunRepeatedMotion(0x16, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(0x16, FACING_EAST, 20);
	Event_ShowMessageAndWait(0x16, 0, 20);
	Engine_EventWait(20);
	Actor_FaceDirection(0x18, FACING_EAST, 20);
	Engine_ActorSetSpritePriority(0x11, 0);
	Engine_ActorSetSpritePriority(0x12, 0);
	MercuryAerie_LowerCollectedActors();
	Engine_ActorSetSpritePriority(0x11, 1);
	Engine_ActorSetSpritePriority(0x12, 1);
	Event_PlayCue(0x11);
	Camera_MoveTo(PIXELS(0x150), -1, PIXELS(0xD8), 1);
	Engine_CameraWaitForMove();
	Engine_ActorSetAnimationAndWait(IVAN, 4);
	Engine_EventWait(20);
	Audio_PlayCueFromEventWork();
	Event_ShowMessageAndWait(IVAN, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(GARET, 0x3000, 22);
	Engine_ActorRunRepeatedMotion(GARET, 2);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(GARET, 0, 20);
	Actor_ShowEmote(MIA, 0x00000105, 60);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Actor_FaceDirection(MIA, FACING_WEST, 20);
	Engine_ActorSetAnimation(MIA, 0x10);
	Actor_Get(MIA)->scaleX = -0x10000;
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MIA, 0, 40);
	Engine_ActorRunRepeatedMotion(IVAN, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
	Engine_ActorSetAnimationAndWait(IVAN, 4);
	Engine_EventWait(20);
	Engine_EventOpenMessage(2, 0);
	Engine_ActorSetAnimation(MIA, 1);
	Actor_Get(MIA)->scaleX = 0x10000;
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
	Actor_FaceDirection(MIA, FACING_NORTH, 20);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_ActorSetAnimationAndWait(IVAN, 3);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(IVAN, 0, 20);
		gEventWork->message++;
	}
	else
	{
		gEventWork->message++;
		Engine_ActorRunRepeatedMotion(IVAN, 2);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(IVAN, 0, 20);
	}
	Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 20);
	Engine_ActorSetAnimationAndWait(IVAN, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(IVAN, 0, 20);
	Actor_FaceDirection(MIA, FACING_NORTHEAST, 30);
	Actor_FaceDirection(GARET, FACING_SOUTHEAST, 20);
	Engine_ActorRunRepeatedMotion(GARET, 1);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(GARET, 0, 20);
	Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Engine_ActorRunRepeatedMotion(MIA, 1);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x00000101, 0);
	Actor_ShowEmote(GARET, 0x00000101, 0);
	Actor_ShowEmote(IVAN, 0x00000101, 60);
	Actor_SetSpeed(MIA, 0x0000cccc, 0x00006666);
	Actor_SetSpeed(GARET, 0x0000cccc, 0x00006666);
	Actor_WalkToAndWait(MIA, 0x148, 0xD8);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
	Actor_Get(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
	Actor_WalkTo(GARET, 0x138, 0xC8);
	Actor_WalkTo(MIA, 0x118, 0xD8);
	Engine_ActorWaitForMove(GARET);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
	Engine_ActorWaitForMove(MIA);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Engine_EventWait(30);
	Actor_FaceDirection(MIA, FACING_EAST, 20);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Engine_ActorRunRepeatedMotion(GARET, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(GARET, 0, 20);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(IVAN, 2);
	Engine_ActorStartRepeatedMotion(GARET, 2);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Actor_SetAttachedEffect(GARET, 0x102);
	Actor_SetAttachedEffect(IVAN, 0x102);
	Engine_EventWait(60);
	Event_ShowMessageAndWait(IVAN, 0, 20);
	Engine_ActorSetAnimationAndWait(MIA, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Actor_FaceDirection(MIA, FACING_EAST, 20);
	Engine_ActorSetAnimationAndWait(MIA, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MIA, 0, 30);
	Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GARET, 3);
	Engine_ActorSetAnimationAndWait(IVAN, 3);
	Engine_EventWait(30);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 40);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GARET, 3);
	Engine_ActorSetAnimationAndWait(IVAN, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(MIA, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(MIA, 0, 20);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x138, 0xD8);
	Actor_WalkTo(GARET, 0x138, 0xD8);
	Actor_WalkTo(IVAN, 0x138, 0xD8);
	Actor_WalkTo(MIA, 0x138, 0xD8);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_ActorWaitForMove(GARET);
	Actor_SetPosition(GARET, 0, 0);
	Engine_ActorWaitForMove(IVAN);
	Actor_SetPosition(IVAN, 0, 0);
	Engine_ActorWaitForMove(MIA);
	Actor_SetPosition(MIA, 0, 0);
	{
		struct MapLayerScroll *lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];

		lift->offsetY = 0x4000000;
	}
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Map_CopyCellsTo(4, 0x46, 4, 0x4A, 5, 4);
	GameFlag_Set(0x880);
	GameFlag_Set(0x00000881);
	Engine_EventEnd();
}

static void EV06_TOUCH(void)
{
	struct MapLayerScroll *lift;

	lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];
	Engine_EventBegin();
	lift->offsetY = 0x3800000;
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Object_GetById(9)->motionFlags = 0;
	Engine_ActorSetPosition(9, PIXELS(0x68), PIXELS(0x108));
	Object_GetById(9)->y.fixed = -0x200000;
	Object_GetById(9)->targetY = -0x200000;
	Engine_EventGetViewCenter()->motionFlags = 0;
	Engine_CameraSetSpeed(0xcccc, 0x1999);
	Engine_CameraMoveTo(PIXELS(0x80), -1, PIXELS(0xB8), 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(30);
	Map_CopyCellsTo(29, 74, 4, 74, 5, 4);
	Engine_ActorSetSpritePriority(17, 0);
	Engine_ActorSetSpritePriority(18, 0);
	MercuryAerie_RaiseCollectedActors();
	Engine_ActorSetSpritePriority(17, 1);
	Engine_ActorSetSpritePriority(18, 1);
	Engine_EventWait(20);
	Engine_GameFlagSet(0x251);
	Engine_EventEnd();
}

static void EV12_TOUCH(void)
{
	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 152);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 60);
	Engine_ActorSetSpritePriority(17, 0);
	Engine_ActorSetSpritePriority(18, 0);
	MercuryAerie_LowerCollectedActors();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_EventRequestExit(1);
	Engine_EventEnd();
}

static void MercuryAerie_SinkActorPair(void)
{
	struct EffectParams	 params;
	struct EffectParams *p;
	struct FieldActor	*left;
	struct FieldActor	*right;
	u32					 i;
	s32					 x;
	s32					 r;

	left = Object_GetById(22);
	right = Object_GetById(24);
	Engine_AudioPlayCue(190);
	Actor_SetChildValue(22, 0x100);
	Engine_ActorSetChildValue(24, 0x100);
	Engine_ActorSetSpriteFlags(Object_GetById(22), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(24), 0);
	p = &params;
	p->count = 1;
	p->kind = 5;
	p->tile = 0x11c;
	p->spread = 0x6666;
	p->rise = 0x30000;
	for (i = 0; i <= 31; i++)
	{
		Engine_EventWait(1);
		if (i & 1)
		{
			r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
			x = left->x.fixed;
			x += r;
			x += -0xc0000;
			Effect_Spawn(x,
				left->y.fixed + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + -0x100000,
				left->z.fixed, 0, 0x40000, 0, 0x1b0000, p);
		}
		else
		{
			r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
			x = right->x.fixed;
			x += r;
			x += -0xc0000;
			Effect_Spawn(x,
				right->y.fixed + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + -0x100000,
				right->z.fixed, 0, 0x40000, 0, 0x1b0000, p);
		}
	}
	Engine_ActorSetPosition(22, 0, 0);
	Engine_ActorSetPosition(24, 0, 0);
}

static void MercuryAerie_RiseActorPair(void)
{
	struct EffectParams	 params;
	struct EffectParams *p;
	struct FieldActor	*left;
	struct FieldActor	*right;
	u32					 i;
	s32					 x;
	s32					 r;

	left = Object_GetById(22);
	right = Object_GetById(24);
	Engine_AudioPlayCue(190);
	Engine_ActorSetSpriteFlags(Object_GetById(22), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(24), 0);
	p = &params;
	p->count = 1;
	p->kind = 5;
	p->tile = 0x11c;
	p->spread = 0x6666;
	p->rise = 0x30000;
	for (i = 0; i <= 31; i++)
	{
		Engine_EventWait(1);
		if (i & 1)
		{
			r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
			x = left->x.fixed;
			x += r;
			x += -0xc0000;
			Effect_Spawn(x,
				left->y.fixed + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + 0x200000,
				left->z.fixed, 0, -0x40000, 0, 0x1b0000, p);
		}
		else
		{
			r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
			x = right->x.fixed;
			x += r;
			x += -0xc0000;
			Effect_Spawn(x,
				right->y.fixed + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + 0x200000,
				right->z.fixed, 0, -0x40000, 0, 0x1b0000, p);
		}
		if (i == 20)
		{
			Actor_SetChildValue(22, 0x100);
			Engine_ActorSetChildValue(24, 0x100);
		}
	}
	Engine_ActorSetChildValue(22, 0);
	Engine_ActorSetChildValue(24, 0);
	Engine_ActorSetSpriteFlags(Object_GetById(22), 1);
	Engine_ActorSetSpriteFlags(Object_GetById(24), 1);
}

// MercuryAerie_LowerCollectedActors passes its lift as a third argument,
// which this function does not take.
static void MercuryAerie_CollectBandSlots(s32 *out, s32 y)
{
	struct ActorTable *table;
	u32				   slot;

	table = (struct ActorTable *)gEventWork;
	y = 64 - (y >> 20);
	for (slot = 0; slot <= 65; slot++)
	{
		struct FieldActor *actor = table->actors[slot];

		if (actor != 0)
		{
			s32 cell = actor->x.fixed >> 20;
			s32 row = actor->z.fixed >> 20;

			if (cell >= 4 && cell <= 8 && y + 8 <= row && row < y + 11)
				*out++ = slot;
		}
	}
}

static void MercuryAerie_LowerCollectedActors(void)
{
	s32					   ids[5];
	struct MapLayerScroll *lift;
	u32					   i;
	u32					   j;
	u32					   n;
	s32					   speed;
	struct FieldActor	  *actor;

	lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];
	speed = 0x1999;
	n = 0;
	for (i = 0; i <= 4; i++)
		ids[i] = 66;
	// the lift goes as a third argument, as the bytes show: the cast keeps it
	((void (*)())MercuryAerie_CollectBandSlots)(ids, lift->offsetY, lift);
	for (i = 0; i <= 4; i++)
	{
		if (ids[i] == 66)
			break;
		Object_GetById(ids[i])->motionFlags = 0;
		n++;
	}
	Engine_AudioPlayCue(223);
	for (i = 0; i <= 227; i++)
	{
		lift->offsetY -= speed;
		for (j = 0; j < n; j++)
		{
			Object_GetById(ids[j])->z.fixed += speed;
			actor = Object_GetById(ids[j]);
			actor->targetZ = Object_GetById(ids[j])->z.fixed;
		}
		if ((i & 3) == 3)
			speed += 0x1999;
		if (speed > 0x17fff)
			speed = 0x18000;
		Engine_TaskWait(1);
	}
	for (i = 0; i < n; i++)
		Object_GetById(ids[i])->motionFlags = 0;
}

static void MercuryAerie_RaiseCollectedActors(void)
{
	s32					   ids[5];
	struct MapLayerScroll *lift;
	u32					   i;
	u32					   j;
	u32					   n;
	s32					   speed;
	struct FieldActor	  *actor;

	lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];
	speed = 0x18000;
	n = 0;
	for (i = 0; i <= 4; i++)
		ids[i] = 66;
	MercuryAerie_CollectBandSlots(ids, lift->offsetY);
	for (i = 0; i <= 4; i++)
	{
		if (ids[i] == 66)
			break;
		Object_GetById(ids[i])->motionFlags = 0;
		n++;
	}
	Engine_AudioPlayCue(223);
	for (i = 0; i <= 85; i++)
	{
		lift->offsetY += speed;
		for (j = 0; j < n; j++)
		{
			Object_GetById(ids[j])->z.fixed -= speed;
			actor = Object_GetById(ids[j]);
			actor->targetZ = Object_GetById(ids[j])->z.fixed;
		}
		if ((i & 3) == 3 && i > 75)
			speed += -0x3333;
		if (speed < 0xccc)
			speed = 0xccc;
		Engine_TaskWait(1);
	}
	lift->offsetY = 0x4000000;
	Engine_MapRedraw();
	Engine_TaskWait(2);
}

static void FieldScene_ConfigureValue93Scene(void)
{
	Engine_PsynergyBegin(93, 1);
	Engine_PsynergySetTarget(24, 9);
	Engine_PsynergyRaiseHands();
	Engine_PsynergyPlayEffect(1);
	Engine_PsynergyLowerHands();
	BattleEffect_CleanupSceneObjects();
}

static void SceneEffect_UpdateArcPosition(struct OverlayObject *object)
{
	struct OverlayObject *parent;
	u16					  angle;

	parent = object->linkedObject;
	angle = object->angle64;
	object->coordinate08 = parent->coordinate08 + Engine_MathCos(angle) * (object->field30 + 28);
	object->coordinate10 = (Engine_MathSin(angle) << 4) + 0x900000;
	object->coordinate38 = object->coordinate08;
	object->coordinate40 = object->coordinate10;
	object->angle64 -= 0x200;
}

struct LampSpark
{
	u8					unk00[0x30];
	s32					swing;
	u8					unk34[0x1c];
	struct FieldSprite *sprite;
	u8					unk54;
	u8					motionFlags;
	u8					unk56[0x0e];
	u16					angle;
	u16					turn;
	struct FieldActor  *source;
	void (*update)();
};

static void MercuryAerie_FlickerActorEight(void)
{
	struct FieldActor	 *lamp;
	struct LampSpark	 *spark;
	struct FieldSprite	 *sprite;
	struct MapScrollWork *map;
	s32					  height;
	s32					  scale;
	s32					  tick;
	u32					  angle;

	lamp = Object_GetById(8);
	map = gMapWork[0];
	height = ((u32)(Engine_RandomNext() * 48) >> 16) << 16;
	if (((union FieldCoordinate *)&map->viewY)->part.pixel <= 129)
	{
		if (gFrameCount & 1)
		{
			Engine_ActorSetPosition(8, 0x1300000, 0x900000);
			scale = 0x10000;
			Object_GetById(8)->scaleX = scale;
		}
		else
		{
			Engine_ActorSetPosition(8, 0x1300000, 0x970000);
			scale = 0x14ccc;
			Object_GetById(8)->scaleX = scale;
		}
		Object_GetById(8)->scaleY = scale;
	}
	else
	{
		Actor_SetPosition(8, 0x80000, 0x80000);
	}
	if (lamp == NULL)
	{
		return;
	}
	tick = gFrameCount & 15;
	if (tick != 0)
	{
		return;
	}
	spark = (struct LampSpark *)Object_Create(
		0x11c, lamp->x.fixed + 0x80000, lamp->y.fixed + height + 0x80000, lamp->z.fixed);
	height = height / 0x60000;
	height <<= 16;
	if (spark == NULL)
	{
		return;
	}
	sprite = spark->sprite;
	Engine_ObjectSetScript((struct FieldActor *)spark, gMercuryAerie_SparkScript);
	ObjectGroup_SetChildValue((struct FieldActor *)spark, 3);
	spark->motionFlags = tick;
	angle = Engine_RandomNext() & 0x0ffff000;
	spark->angle = angle;
	spark->turn = tick;
	spark->source = lamp;
	spark->update = SceneEffect_UpdateArcPosition;
	spark->swing = (Engine_MathSin((height & 0xfffff) >> 4) * 24) >> 16;
	/* FAKEMATCH: the byte view keeps this store ahead of the lamp's reload; the field store lets the reload schedule first (score 210). Field store: differ 2. */
	*(u8 *)&sprite->flags = 0;
	sprite->priority = lamp->sprite->priority;
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

struct SceneEntrance gMercuryAerieEntrances[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 8, 0, 8, FACING_SOUTH, 0, 0, 0, 608, 424, 0 },
	{ 2, CONDITION_ALWAYS, 8, 0, 8, FACING_SOUTH, 0, 0, 0, 608, 424, 0 },
	{ 5, CONDITION_ALWAYS, 504, 0, 152, FACING_SOUTH, 0, 0, 0, 608, 424, 0 },
	{ 51, CONDITION_ALWAYS, 504, 0, 168, FACING_SOUTH, 0, 0, 0, 608, 424, 0 },
	{ SCENE_TABLE_END },
};

u32 gMercuryAerieExits[] = {
	SCENE_EXITS(58),
	SCENE_EXIT(1, 57, 15),
	SCENE_EXITS_END,
};

struct ScenePlacement gMercuryAeriePlacements[] = {
	{ GARET, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTHWEST, TALK_KEEP_FACING, 0 },
	{ IVAN, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTHEAST, TALK_KEEP_FACING, 0 },
	{ MIA, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ JENNA, 0x253, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(160), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0xf4, CONDITION_ALWAYS, 7,
		PIXELS(304), PIXELS(40), PIXELS(144), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf2, CONDITION_ALWAYS, 7,
		PIXELS(440), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf2, CONDITION_ALWAYS, 7,
		PIXELS(472), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf2, CONDITION_ALWAYS, 7,
		PIXELS(504), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf2, CONDITION_ALWAYS, 7,
		PIXELS(104), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf2, CONDITION_ALWAYS, 7,
		PIXELS(136), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf2, CONDITION_ALWAYS, 7,
		PIXELS(168), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x1e, 0x253, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(152), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x23, 0x253, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(160), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x20, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x21, 0x253, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(200), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x2b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gMercuryAerieEvents[] = {
	{ EVENT_TOUCH, 5, 0x880, EVENT_SCRIPT(EV05_TOUCH) },
	{ EVENT_TOUCH, 6, 0x251, EVENT_SCRIPT(EV06_TOUCH) },
	{ 0x202, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_InitializeMotion) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TOUCH) },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_InitializeMotion) },
	{ EVENT_TOUCH, 10, 0x250, EVENT_SCRIPT(EV10_TOUCH) },
	{ SEARCH(SEARCH_UNNAMED), 100, 0x350, FIND_PSYNERGY_STONE },
	{ EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TOUCH) },
	{ SCENE_EVENTS_END },
};

s32 gMercuryAerie_NearestActor = 0;

s32 gMercuryAerie_SparkScript[] = {
	SC_SET(SW_SCALE_Y, 0x16666),
	SC_SET(SW_SCALE_X, 0x16666),
	SC_ARG(4),
	SC_WAIT(30),
	SC_FREE,
};
