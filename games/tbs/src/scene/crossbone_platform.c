#include "game_state.h"
#include "global_cells.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/crossbone_platform.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "unit/scene/common/colosso_item_icon.h"
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
#include "edition.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "io_reg.h"
#include "resource.h"
#include "field_obj.h"
#include "animspr.h"
#include "mem.h"
#include "dma.h"
#include "callback_scheduler.h"
#include "call.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"

// 前方宣言
static void SceneActor_BranchOnSlotZeroAtTile38(void);
static void ActorPresentation_PlaceActorFourteenOnActorNine(void);
static void SceneActor_PassActorNinePositionWithId107(void);
static void CrossbonePlatform_InitializeScene1(void);
static void CrossbonePlatform_InitializeScene2(void);
static void CrossbonePlatform_InitializeScene3(void);

enum
{
	ITEM_RED_KEY = 243,
	ITEM_BLUE_KEY = 244
};

static __inline__ void SceneState_StoreStep(s16 *field, s32 step)
{
	*field = step;
}

static __inline__ void LiftBlock(s32 column, u8 lowered)
{
	{
		s32 fifth = column;
		Map_CopyCellAttributes(61, 36, 1, 1, fifth, 42);
	}
	Object_GetById(8)->motionFlags = lowered;
	Object_GetById(8)->y.fixed = 0x200000;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/object_staged_actor.h"

static void EV15_SCRIPT(void)
{
	BattleFx_RunPageEffectForSlot(15, 0, 6);
}

static void Resource3b4_EmptyHookA(void)
{
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 selector = gGameState.scene;

	if (selector == (s32)&SceneId_CrossbonePlatform1)
	{
		return gCrossbonePlatformEntrances1;
	}
	if (selector == (s32)&SceneId_CrossbonePlatform2)
	{
		return gCrossbonePlatformEntrances2;
	}
	if (selector == (s32)&SceneId_CrossbonePlatform3)
	{
		return gCrossbonePlatformEntrances3;
	}
	return gCrossbonePlatformEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetSceneTable(void)
{
	return gCrossbonePlatform_SceneTable;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 selector = gGameState.scene;

	if (selector == (s32)&SceneId_CrossbonePlatform1)
	{
		return gCrossbonePlatformPlacements1;
	}
	if (selector == (s32)&SceneId_CrossbonePlatform2)
	{
		return gCrossbonePlatformPlacements2;
	}
	if (selector == (s32)&SceneId_CrossbonePlatform3)
	{
		return gCrossbonePlatformPlacements3;
	}
	return gCrossbonePlatformPlacementsOther;
}

static void Resource3b4_EmptyHookB(void)
{
}

static s32 SceneActor_FaceTowardActorZero(struct FieldActor *actor)
{
	struct FieldActor *leader = Object_GetById(0);

	actor->facing = ArcTan2(leader->z.fixed - actor->z.fixed, leader->x.fixed - actor->x.fixed);
	return 0;
}

static void EV20_TOUCH(void)
{
	struct FieldActor *record;

	if (GameFlag_IsSet(0x9c8) == 0)
	{
		GameFlag_Set(0x9c8);
		Engine_EventBegin();
		Camera_SetSpeed(0x20000, 0x4000);
		Engine_CameraMoveToActor(15, 1);
		Engine_CameraWaitForMove();
		Actor_FaceDirection(15, 0, 20);
		Actor_SetAttachedEffect(15, 0x102);
		Engine_ActorRunRepeatedMotion(15, 2);
		Engine_EventWait(20);
		Actor_SetSpeed(15, 0x10000, 0x8000);
		Event_PlayCue(152);
		record = Object_GetById(15);
		record->velocityY = 0x80000;
		Actor_WalkToAndWait(15, 0x248, 0x2a8);
		Actor_FaceDirection(15, 0x4000, 20);
		Engine_EventEnd();
	}
}

static void EV21_TOUCH(void)
{
	struct FieldActor *rec7;
	s32				   rec8;
	struct FieldActor *record;

	if (GameFlag_IsSet(0x9c8) != 0)
	{
		rec8 = GameFlag_IsSet(0x9c9);
		if (rec8 == 0)
		{
			GameFlag_Set(0x9c9);
			Engine_EventBegin();
			Camera_SetSpeed(0x20000, 0x4000);
			Engine_CameraMoveToActor(15, 1);
			Engine_CameraWaitForMove();
			Actor_FaceDirection(15, 0x4000, 20);
			Actor_SetAttachedEffect(15, 0x102);
			Engine_ActorRunRepeatedMotion(15, 2);
			Engine_EventWait(20);
			Actor_SetSpeed(15, 0x10000, 0x8000);
			Event_PlayCue(152);
			record = Object_GetById(15);
			record->velocityY = 0xa0000;
			Actor_WalkToAndWait(15, 0x248, 0x298);
			Actor_FaceDirection(15, 0x4000, 20);
			Actor_SetAttachedEffect(15, 0x102);
			Engine_EventWait(30);
			Actor_SetSpeed(15, 0x80000, 0x4000);
			Actor_WalkToAndWait(15, 0x298, 0x298);
			Actor_WalkToAndWait(15, 0x2e8, 0x298);
			Actor_WalkToAndWait(15, 0x338, 0x298);
			Engine_EventWait(10);
			Event_PlayCue(208);
			Work_SetValuesIfNonNegative(0x40000, 0x20000, 0x10000);
			Engine_EventWait(20);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_EventWait(30);
			Actor_SetPosition(15, 0x3780000, 0x2980000);
			rec7 = Object_GetById(15);
			rec7->sprite->rotation = 0xf800;
			rec7->facing = 0;
			ObjectDispatch_ApplyValueToChildren(rec7, 0);
			Engine_ObjectSetScript(rec7, gCrossbonePlatform_ActorFifteenMotionScript);
			Engine_EventEnd();
		}
	}
}

static void EV22_TOUCH(void)
{
	struct FieldActor *record;

	if (GameFlag_IsSet(0x9c9) != 0 && GameFlag_IsSet(0x9ca) == 0)
	{
		GameFlag_Set(0x9ca);
		Engine_EventBegin();
		record = Object_GetById(15);
		record->sprite->rotation = 0;
		ObjectDispatch_ApplyValueToChildren(record, 16);
		Event_PlayCue(152);
		record = Object_GetById(15);
		record->velocityY = 0x80000;
		Actor_FaceDirection(15, 0x8000, 30);
		Actor_SetAttachedEffect(15, 0x102);
		Engine_ActorRunRepeatedMotion(15, 2);
		Engine_EventWait(20);
		Actor_SetSpeed(15, 0x10000, 0x8000);
		Event_PlayCue(152);
		record = Object_GetById(15);
		record->velocityY = 0x40000;
		Actor_WalkToAndWait(15, 0x370, 0x2a8);
		Engine_EventWait(10);
		Actor_SetAttachedEffect(15, 0x101);
		Actor_SetSpeed(15, 0x20000, 0x10000);
		Actor_WalkToAndWait(15, 0x370, 0x2b8);
		Actor_WalkToAndWait(15, 0x372, 0x2c0);
		Actor_WalkToAndWait(15, 0x370, 0x2c8);
		Actor_WalkToAndWait(15, 0x36e, 0x2d0);
		Actor_WalkToAndWait(15, 0x370, 0x2d8);
		Actor_WalkToAndWait(15, 0x372, 0x2e0);
		Actor_WalkToAndWait(15, 0x370, 0x2e8);
		Actor_WalkToAndWait(15, 0x36e, 0x2f0);
		Actor_WalkToAndWait(15, 0x370, 0x2f8);
		Actor_SetPosition(15, 0x3580000, 0x3380000);
		Engine_EventWait(10);
		Actor_FaceDirection(15, 0xc000, 20);
		Actor_SetAttachedEffect(15, 0x100);
		record = Object_GetById(15);
		record->update = (void (*)(union FieldObject *))SceneActor_FaceTowardActorZero;
		Engine_EventEnd();
	}
}

static void FieldScene_CopyActorPosition(void)
{
	struct FieldActor *dst;
	struct FieldActor *src;
	s32				   idx;
	s32				   table;
	s32				   idx4;
	struct EventWork  *work;

	work = gEventWork;
	if (Engine_GameFlagIsSet(0x9ca) != 0)
	{
		if (gGameState.pausedObject != 15)
		{
			idx = work->touchedTrigger;
			dst = Object_GetById(15);
			src = Object_GetById(0);
			dst->speed = src->speed;
			dst = Object_GetById(15);
			src = Object_GetById(0);
			dst->acceleration = src->speed;
			idx -= 30;
			table = (s32)gCrossbonePlatform_ActorFifteenWalkTargets;
			idx <<= 3;
			idx4 = idx + 4;
			Engine_ActorWalkTo(15, *(s32 *)(table + idx), *(s32 *)(table + idx4));
		}
	}
}

static void EV15_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(15, 0);
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 selector = gGameState.scene;

	if (selector == (s32)&SceneId_CrossbonePlatform1)
	{
		return gCrossbonePlatformEvents1;
	}
	if (selector == (s32)&SceneId_CrossbonePlatform2)
	{
		return gCrossbonePlatformEvents2;
	}
	if (selector == (s32)&SceneId_CrossbonePlatform3)
	{
		return gCrossbonePlatformEvents3;
	}
	return gCrossbonePlatformEventsOther;
}

static void CrossbonePlatform_CheckLeaderAhead(void)
{
	Engine_LeaderCheckAhead();
}

static void CrossbonePlatform_OpenPassage(s32 arg)
{
	s32 step = (arg & 0xff) * 4;
	s32 dest = step + 77;
	s32 column = step + 13;

	if (arg & 0x100)
	{
		Engine_AudioPlayCue(157);
		Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		Map_CopyCells(79, 29, 1, 3, dest, 40);
		Engine_TaskWait(40);
	}
	Map_CopyCells(80, 29, 1, 3, dest, 40);
	Map_CopyCellAttributes(column, 40, 1, 1, column, 41);
	Map_CopyCellAttributes(column, 40, 1, 1, column, 42);
}

static void CrossbonePlatform_OpenPlatformPassage(s32 a0)
{
	if ((a0 & 0x100) != 0)
	{
		Event_PlayCue(157);
		Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		Map_CopyCells(84, 29, 1, 3, 70, 49);
		Engine_TaskWait(60);
	}
	Map_CopyCells(85, 29, 1, 3, 70, 49);
	Map_CopyCellAttributes(6, 49, 1, 1, 6, 50);
	Map_CopyCellAttributes(6, 49, 1, 1, 6, 51);
}

static void EV50_TOUCH(void)
{
	struct FieldActor *record;

	record = Object_GetById(0);
	if (record->facing == 0xc000)
	{
		if (GameFlag_IsSet(0x9c4) == 0)
		{
			if (PartyInventory_FindOwner(243) != -1)
			{
				GameFlag_Set(0x9c4);
				CrossbonePlatform_OpenPlatformPassage(0x100);
				PartyInventory_Remove(243);
			}
		}
	}
}

static void SceneState_SetSelectorFlagWhenFacingC000(s32 selector)
{
	struct FieldActor *slot = Object_GetById(0);
	s32				   flag;

	if (slot->facing != 0xC000)
	{
		return;
	}
	flag = selector + 2496;
	if (Engine_GameFlagIsSet(flag) != 0)
	{
		return;
	}
	if (PartyInventory_FindOwner(244) == -1)
	{
		return;
	}
	Engine_GameFlagSet(flag);
	CrossbonePlatform_OpenPassage(0x100 | selector);
	PartyInventory_Remove(244);
}

static void EV51_TOUCH(void)
{
	SceneState_SetSelectorFlagWhenFacingC000(0);
}

static void EV52_TOUCH(void)
{
	SceneState_SetSelectorFlagWhenFacingC000(1);
}

static void EV53_TOUCH_1(void)
{
	struct FieldActor *slot;
	s16				  *cnt;
	s32				   reset;

	SceneState_SetSelectorFlagWhenFacingC000(2);
	slot = Object_GetById(0);
	if (slot->facing == 0x4000)
	{
		cnt = &gEventWork->blockedSteps;
		if (*cnt > 12)
		{
			Engine_LeaderCheckAhead();
			reset = 0;
			*cnt = reset;
		}
	}
}

static void EV54_TOUCH_1(void)
{
	struct FieldActor *p;
	s16				  *cnt;
	s32				   zero;

	SceneState_SetSelectorFlagWhenFacingC000(3);
	p = Object_GetById(0);
	if (p->facing == 0x4000)
	{
		cnt = &gEventWork->blockedSteps;
		if (*cnt > 12)
		{
			Engine_LeaderCheckAhead();
			zero = 0;
			*cnt = zero;
		}
	}
}

static void SceneState_ApplyRectAndPlaceSlot12(void)
{
	s32 slot = 12;
	s32 x = 0x1980000;
	s32 z = 0x3080000;

	Map_CopyCells(25, 45, 1, 2, 25, 48);
	if (Engine_GameFlagIsSet(0xeeb) == 0)
		Engine_ActorSetPosition(slot, x, z);
	Engine_EventWait(1);
}

static void ConfigureAndPlaceActorTwelve(void)
{
	Map_CopyCells(24, 48, 1, 2, 25, 48);
	Actor_SetPosition(12, 0x00080000, 0x00080000);
}

static void EV08_PSYNERGY_1(void)
{
	Engine_ItemShowFound(ITEM_BLUE_KEY, 3);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_PartyGiveItem(ITEM_BLUE_KEY, 0);
	Engine_ActorSetPosition(8, 0, 0);
	Engine_GameFlagSet(0xEE7);
}

static void EV09_PSYNERGY_1(void)
{
	Engine_ItemShowFound(ITEM_BLUE_KEY, 3);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_PartyGiveItem(ITEM_BLUE_KEY, 0);
	Engine_ActorSetPosition(9, 0, 0);
	Engine_GameFlagSet(0xEE8);
}

static void EV10_PSYNERGY_1(void)
{
	Engine_ItemShowFound(ITEM_BLUE_KEY, 3);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_PartyGiveItem(ITEM_BLUE_KEY, 0);
	Engine_ActorSetPosition(0xA, 0, 0);
	Engine_GameFlagSet(0xEE9);
}

static void EV11_PSYNERGY_1(void)
{
	Engine_ItemShowFound(ITEM_BLUE_KEY, 3);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_PartyGiveItem(ITEM_BLUE_KEY, 0);
	Engine_ActorSetPosition(0xB, 0, 0);
	Engine_GameFlagSet(0xEEA);
}

static void EV12_PSYNERGY_1(void)
{
	Engine_ItemShowFound(ITEM_RED_KEY, 3);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_PartyGiveItem(ITEM_RED_KEY, 0);
	Engine_ActorSetPosition(0xC, 0, 0);
	Engine_GameFlagSet(0xEEB);
}

static void CrossbonePlatform_RaiseTriggerOnStand(void)
{
	struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
	s32				   x = leader->x.fixed / 0x100000;
	s32				   z = leader->z.fixed / 0x100000;
	struct EventWork  *event = gEventWork;

	if (!GameFlag_IsSet(0x220) && gGameState.cloaked == 0 && gGameState.pausedObject != 8
		&& (u32)(x - 21) <= 2 && z >= 10 && z <= 11)
	{
		Engine_GameFlagSet(0x220);
		event->raisedTrigger = 91;
	}
}

static void EV91_RAISED(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1e666, 0xf333);
	Actor_SetSpeed(8, 0x1e666, 0xf333);
	Event_PlayCue(188);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(8, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(8);
	Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
	Engine_EventWait(4);
	Event_PlayCue(188);
#if EDITION_INTERNATIONAL
	Actor_SetDestinationOffset(8, 0, 16);
#else
	Actor_SetDestinationOffset(8, 0, 24);
#endif
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Actor_SetDestination(8, 0x168, 152);
	Engine_ActorWaitForMove(8);
	Engine_EventEnd();
	GameFlag_Clear(0x220);
}

static void SceneState_TriggerColumnTen(void)
{
	struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
	s32				   x = leader->x.fixed / 0x100000;
	s32				   z = leader->z.fixed / 0x100000;
	struct EventWork  *work = gEventWork;

	if (GameFlag_IsSet(0x220) == 0 && gGameState.cloaked == 0 && gGameState.pausedObject != 9
		&& x == 10 && (u32)(z - 16) <= 2)
	{
		GameFlag_Set(0x220);
		SceneState_StoreStep(&work->raisedTrigger, 92);
	}
}

static void EV92_RAISED(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
	Actor_SetSpeed(9, 0x1b333, 0xd999);
	Event_PlayCue(188);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(9, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(9);
#if EDITION_INTERNATIONAL
	Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
#else
	Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
#endif
	Event_PlayCue(188);
	Engine_EventWait(4);
	Actor_SetDestinationOffset(9, 0, 16);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Actor_SetDestination(9, 168, 0x108);
	Engine_ActorWaitForMove(9);
	Engine_EventEnd();
	GameFlag_Clear(0x220);
}

static void SceneActor_TrackOriginColumnForSlot(s32 no)
{

	struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
	s32				   col = leader->x.fixed / 0x100000;
	s32				   row = leader->z.fixed / 0x100000;
	s32				   slot = no + 10;

	if (gGameState.pausedObject == slot)
		return;
	if (col == gCrossbonePlatform_SlotColumns[no])
		return;

	Engine_ActorSetSpeed(slot, 0x48000, 0x24000);
	Engine_AudioPlayCue(188);
	Engine_ActorSetDestination(slot, (col << 4) + 8, 360);

	gCrossbonePlatform_SlotColumns[no] = col;

	if (row <= 22)
	{
		Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 8);
	}
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
}

static void EV53_TOUCH_2(void)
{
	SceneActor_TrackOriginColumnForSlot(0);
}

static void EV54_TOUCH_2(void)
{
	SceneActor_TrackOriginColumnForSlot(1);
}

static void FieldScene_RunLateIndexedStep2(void)
{
	SceneActor_TrackOriginColumnForSlot(2);
}

static void EV61_TOUCH(void)
{
	struct FieldActor *record;

	if (gGameState.pausedObject != 10)
	{
		Engine_EventBegin();
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
		Actor_SetSpeed(10, 0x1b333, 0xd999);
		Event_PlayCue(188);
		record = Object_GetById(0);
		if (record != 0)
		{
			Actor_SetDestination(10, record->x.part.pixel, record->z.part.pixel);
		}
		Engine_ActorWaitForMove(10);
#if EDITION_INTERNATIONAL
		Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
#else
		Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
#endif
		Engine_EventWait(4);
		Event_PlayCue(188);
		Actor_SetDestinationOffset(10, 0, 16);
		Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
		Actor_SetDestination(10, 0x108, 0x168);
		Engine_ActorWaitForMove(10);
		Engine_EventWait(10);
		Engine_EventEnd();
	}
}

static void EV62_TOUCH(void)
{
	struct FieldActor *record;
	s32				   v3;

	if (gGameState.pausedObject != 11)
	{
		Engine_EventBegin();
		Actor_SetSpeed(0, 0x1b333, 0xd999);
		Engine_ActorSetSpeed(11, 0x1b333, 0xd999);
		Engine_AudioPlayCue(188);
		v3 = Object_GetById(0)->x.fixed / 0x100000;
		if (v3 > Object_GetById(11)->x.fixed / 0x100000)
		{
			Engine_ActorSetDestinationOffset(11, 8, 0);
		}
		v3 = Object_GetById(0)->x.fixed / 0x100000;
		if (v3 < Object_GetById(11)->x.fixed / 0x100000)
		{
			FieldObjectMotion_OffsetPositionAndResetMotion(11, -8, 0);
		}
		Engine_ActorWaitForMove(11);
		record = Object_GetById(0);
		if (record != 0)
		{
			Engine_ActorSetDestination(11, record->x.part.pixel, record->z.part.pixel);
		}
		Engine_ActorWaitForMove(11);
#if EDITION_INTERNATIONAL
		Engine_ActorSetDestinationOffset(0, 0, 24);
#else
		Engine_ActorSetDestinationOffset(0, 0, 16);
#endif
		Engine_EventWait(4);
		Engine_AudioPlayCue(188);
		Engine_ActorSetDestinationOffset(11, 0, 16);
		Engine_ActorWaitForMove(0);
		Engine_ActorSetDestination(11, 0x158, 0x168);
		Engine_ActorWaitForMove(11);
		Engine_EventWait(10);
		Engine_EventEnd();
	}
}

static void SceneState_TriggerColumnNineteen(void)
{
	struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
	s32				   x = leader->x.fixed / 0x100000;
	s32				   z = leader->z.fixed / 0x100000;
	struct EventWork  *work = gEventWork;
	struct GameState  *state = &gGameState;

	if (state->pausedObject != 12 && GameFlag_IsSet(0x220) == 0 && state->cloaked == 0 && x == 19
		&& (u32)(z - 15) <= 1)
	{
		GameFlag_Set(0x220);
		SceneState_StoreStep(&work->raisedTrigger, 96);
	}
}

static void EV96_RAISED(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
	Actor_SetSpeed(12, 0x1b333, 0xd999);
	Event_PlayCue(188);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(12, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(12);
#if EDITION_INTERNATIONAL
	Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
#else
	Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
#endif
	Event_PlayCue(188);
	Actor_SetDestinationOffset(12, 0, 16);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Actor_SetDestination(12, 0x138, 232);
	Engine_ActorWaitForMove(12);
	Engine_EventEnd();
	GameFlag_Clear(0x220);
}

static void SetBlendAlphaCoefficients(void)
{
	u32 coefficient = 208;

	coefficient <<= 4;
	REG_BLDALPHA = coefficient;
}

static void SceneEffect_SetBlendAlpha0607(void)
{
	u16 value = 0x0607;

	REG_BLDALPHA = value;
}

static void EV60_SCRIPT(void)
{
	struct FieldActor *slot = Object_GetById(ACTOR_PARTY_LEADER);
	u16				   facing = slot->facing;

	if (facing == 0xc000)
	{
		if (Engine_GameFlagIsSet(0x206) != 0)
		{
			{
				s32 fifth = 45;
				s32 last = 43;
				Map_CopyCellAttributes(46, 43, 1, 1, fifth, last);
			}
		}
		Engine_GameFlagClear(0x207);
		battle_owner_69();
	}
	else if (facing == 0x4000)
	{
		FieldEffect_UpdateGridPlacement();
	}
	else if (facing == 0)
	{
		if (Engine_GameFlagIsSet(0x206) != 0)
		{
			{
				s32 fifth = 45;
				s32 last = 43;
				Map_CopyCellAttributes(58, 36, 1, 1, fifth, last);
			}
		}
		Engine_GameFlagSet(0x207);
		Engine_LeaderCheckAhead();
	}
	else if (facing == 0x8000)
	{
		if (slot->y.fixed == 0)
		{
			SceneActor_BranchOnSlotZeroAtTile38();
		}
		else
		{
			Engine_LeaderCheckAhead();
		}
	}
}

static void FieldScene_NoOpCallback(void)
{
}

static void EV13_PSYNERGY(void)
{
	struct FieldActor *slot = Object_GetById(13);

	Map_CopyCellAttributes(40, 54, 1, 1, 40, 55);
	if (slot != 0)
	{
		u8 *other = &Object_GetById(13)->motionFlags;
		u8 *flags = &slot->priorityFlags;

		*other = 0;
		*flags = 2;
	}
	Engine_GameFlagSet(512);
}

static void EV70_SCRIPT(void)
{
	struct Slot *slot = Object_GetById(0);

	if (slot->f06 == 0)
	{
		Engine_LeaderCheckAhead();
	}
	else
	{
		SceneActor_BranchOnSlotZeroAtTile38();
	}
}

static void CrossbonePlatform_RestoreActorEightTerrain(void)
{
	Map_CopyCellAttributes(57, 42, 1, 1, 40, 42);
	Map_CopyCellAttributes(57, 42, 1, 1, 41, 42);
	Map_CopyCellAttributes(58, 42, 1, 1, 42, 42);
	Map_CopyCellAttributes(62, 37, 3, 1, 37, 42);

	Object_GetById(8)->motionFlags = 1;
}

static void ActorPresentation_SetSceneCell58AndMarkActorEight(void)
{

	Map_CopyCellAttributes(58, 41, 1, 1, 42, 42);
	Object_GetById(8)->priorityFlags = 2;
}

static void CrossbonePlatform_RestoreRightUnderfootCell(void)
{

	Map_CopyCellAttributes(44, 42, 1, 1, 41, 42);
	Object_GetById(8)->priorityFlags = 2;
}

static void CrossbonePlatform_RestoreLeftUnderfootCell(void)
{

	Map_CopyCellAttributes(39, 42, 1, 1, 40, 42);
	Object_GetById(8)->priorityFlags = 2;
}

static void EV08_PSYNERGY_3(void)
{
	struct FieldActor *actor = Object_GetById(8);
	s32				   x = actor->x.fixed;
	s32				   height = actor->y.fixed;
	s32				   column = x / 0x100000;
	u8				   lowered;

	if (height == 0)
	{
		Object_GetById(8)->priorityFlags = 2;
	}
	CrossbonePlatform_RestoreActorEightTerrain();
	lowered = 0;
	Object_GetById(8)->motionFlags = 3;
	if (column == 40)
	{
		CrossbonePlatform_RestoreLeftUnderfootCell();
	}
	else if (column == 42)
	{
		ActorPresentation_SetSceneCell58AndMarkActorEight();
	}
	else if (column == 41)
	{
		CrossbonePlatform_RestoreRightUnderfootCell();
	}
	else if (column == 39)
	{
		LiftBlock(column, lowered);
	}
	else if (column == 38)
	{
		LiftBlock(column, lowered);
	}
	else if (column == 37)
	{
		LiftBlock(column, lowered);
	}
}

static void SceneActor_BranchOnSlotZeroAtTile38(void)
{
	s32 *slot0 = Object_GetById(0);
	s32 *slot8 = Object_GetById(8);
	s32	 x0 = slot0[2] / 0x100000;
	s32	 x8 = slot8[2] / 0x100000;

	if (x0 == 38 && x8 != 38)
	{
		u16 facing = ((u16 *)slot0)[3];

		if (facing == 0xc000)
		{
			battle_owner_69();
			return;
		}
		if (facing == 0x4000)
		{
			FieldEffect_UpdateGridPlacement();
			return;
		}
	}

	CrossbonePlatform_RestoreActorEightTerrain();
	StagedActor_AdvancePair();
	EV08_PSYNERGY_3();
}

static void CrossbonePlatform_ReleaseActorNinePlatform(void)
{
	Scheduler_RemoveCallback((s32)ActorPresentation_PlaceActorFourteenOnActorNine);
	Actor_SetPosition(14, 0, 0);
	if (GameFlag_IsSet(0x207) != 0)
	{
		Map_CopyCellAttributes(58, 36, 1, 1, 45, 43);
	}
	else
	{
		Map_CopyCellAttributes(46, 43, 1, 1, 45, 43);
	}
	SceneActor_PassActorNinePositionWithId107();
	GameFlag_Set(0x206);
}

static void EV09_PSYNERGY_3(void)
{
	s32 *slot = Object_GetById(9);
	s32	 x = slot[2] / 0x100000;
	s32	 z = slot[4] / 0x100000;

	if (x == 45 && z == 43)
	{
		CrossbonePlatform_ReleaseActorNinePlatform();
	}
}

static void EV52_SCRIPT(void)
{
	StagedActor_AdvancePair();
	EV09_PSYNERGY_3();
}

static void SceneState_ApplyTwoRectsAtRow56(void)
{

	Map_CopyCellAttributes(38, 56, 1, 1, 38, 55);
	Map_CopyCellAttributes(42, 56, 1, 1, 42, 55);
}

static void CrossbonePlatform_RestoreActorTenLeftCell(void)
{

	Map_CopyCellAttributes(40, 54, 1, 1, 38, 55);
	Object_GetById(10)->motionFlags = 0;
}

static void CrossbonePlatform_RestoreActorTenRightCell(void)
{

	Map_CopyCellAttributes(40, 54, 1, 1, 42, 55);
	Object_GetById(10)->motionFlags = 0;
}

static void EV10_PSYNERGY_3(void)
{
	struct FieldActor *block = Object_GetById(10);
	s32				   y = block->y.fixed / 0x100000;
	s32				   x = block->x.fixed / 0x100000;
	s32				   z = block->z.fixed / 0x100000;

	if (!GameFlag_IsSet(0x300) && y <= 2)
	{
		CrossbonePlatform_RestoreActorTenLeftCell();
		Engine_GameFlagSet(0x300);
	}
	if (z == 55)
	{
		if (x == 42)
		{
			CrossbonePlatform_RestoreActorTenRightCell();
		}
		if (x == 38)
		{
			CrossbonePlatform_RestoreActorTenLeftCell();
		}
	}
	else
	{
		SceneState_ApplyTwoRectsAtRow56();
	}
}

static void EV50_SCRIPT(void)
{
	SceneState_ApplyTwoRectsAtRow56();
	StagedActor_AdvancePair();
	EV10_PSYNERGY_3();
}

static void EV54_SCRIPT(void)
{
	struct Slot *slot = Object_GetById(0);

	if (slot->f06 == 0x4000)
	{
		FieldBattleFx_RunRisingObjectSequence(0, 6, 0);
	}
	else
	{
		EV50_SCRIPT();
	}
}

static void SceneState_ApplyFourRectsAt48_55(void)
{

	Map_CopyCellAttributes(48, 55, 1, 1, 49, 55);
	Map_CopyCellAttributes(48, 55, 1, 1, 50, 55);
	Map_CopyCellAttributes(48, 55, 1, 1, 51, 55);
	Map_CopyCellAttributes(48, 55, 1, 1, 52, 55);
}

static void ActorPresentation_RepaintCellsAtActorsElevenAndTwelve(void)
{
	s32 *slot;
	s32	 tile;

	SceneState_ApplyFourRectsAt48_55();

	slot = Object_GetById(11);
	tile = slot[2] / 0x100000;
	Map_CopyCellAttributes(53, 55, 1, 1, tile, 55);
	slot = Object_GetById(12);
	tile = slot[2] / 0x100000;
	Map_CopyCellAttributes(53, 55, 1, 1, tile, 55);
}

static void EV11_PSYNERGY_3(void)
{
	ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
}

static void EV53_SCRIPT(void)
{
	SceneState_ApplyFourRectsAt48_55();
	StagedActor_AdvancePair();
	EV11_PSYNERGY_3();
}

static void EV12_PSYNERGY_3(void)
{
	ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
}

static void FieldScene_RunThreeStepSequence(void)
{
	SceneState_ApplyFourRectsAt48_55();
	StagedActor_AdvancePair();
	EV12_PSYNERGY_3();
}

static void ActorPresentation_PlaceActorFourteenOnActorNine(void)
{
	struct FieldActor *target = Object_GetById(14);
	struct FieldActor *source = Object_GetById(9);

	target->y.fixed = 0x200000;
	target->x.fixed = source->x.fixed;
	target->z.fixed = source->z.fixed + 0x10000;
}

static void SceneActor_PassActorNinePositionWithId107(void)
{
	struct FieldActor *frame = Object_GetById(9);

	Engine_MapObjectSetPosition(107, frame->x.fixed, frame->z.fixed + 0x10000);
}

static void SceneActor_PublishMarkerBySlotZeroHeight(void)
{
	struct FieldActor *leader = Object_GetById(0);
	u8				   priority;

	if (leader->y.fixed > 0x100000)
	{
		priority = ACTOR_PRIORITY_UNDERFOOT;
		Object_GetById(8)->priorityFlags = priority;
		if (Object_GetById(10)->y.fixed == 0)
			Object_GetById(10)->priorityFlags = priority;
		Object_GetById(11)->priorityFlags = priority;
	}
	else
	{
		if (Object_GetById(10)->y.fixed == 0 && Object_GetById(0)->z.fixed / 0x100000 > 56)
		{
			Engine_ActorSetSpritePriority(10, 3);
		}
		else
		{
			Engine_ActorSetSpritePriority(10, 1);
			Object_GetById(10)->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
		}
		priority = 0;
		Object_GetById(11)->priorityFlags = priority;
	}
	Object_GetById(12)->priorityFlags = priority;
}

s32 Scene_Initialize(void)
{
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (gGameState.scene == (s32)&SceneId_CrossbonePlatform1)
	{
		CrossbonePlatform_InitializeScene1();
	}
	if (gGameState.scene == (s32)&SceneId_CrossbonePlatform2)
	{
		CrossbonePlatform_InitializeScene2();
	}
	if (gGameState.scene == (s32)&SceneId_CrossbonePlatform3)
	{
		CrossbonePlatform_InitializeScene3();
	}
	return 0;
}

static void CrossbonePlatform_UpdateItemIcons(void)
{
	if (++gCrossbonePlatform_IconTimer > 16)
	{
		gCrossbonePlatform_IconTimer = 0;
	}
	switch (gCrossbonePlatform_IconTimer)
	{
	case 12:
		if (Engine_GameFlagIsSet(0xee7) == 0)
		{
			Actor_SetPosition(8, 0xe80000, 0x3680000);
		}
		if (Engine_GameFlagIsSet(0xee8) == 0)
		{
			Actor_SetPosition(9, 0x1280000, 0x3380000);
		}
		if (Engine_GameFlagIsSet(0xee9) == 0)
		{
			Actor_SetPosition(10, 0x1480000, 0x2f80000);
		}
		if (Engine_GameFlagIsSet(0xeea) == 0)
		{
			Actor_SetPosition(11, 0x1680000, 0x3680000);
		}
		break;
	case 10:
		Colosso_ShowItemIcon(8, 0);
		break;
	case 8:
		Colosso_ShowItemIcon(9, 0);
		break;
	case 6:
		Colosso_ShowItemIcon(10, 0);
		break;
	case 4:
		Colosso_ShowItemIcon(11, 0);
		break;
	case 2:
		Colosso_ShowItemIcon(12, 1);
		break;
	}
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/colosso_item_icon.h"

static void CrossbonePlatform_InitializeScene1(void)
{
	Engine_TaskWait(1);
	Colosso_ShowItemIcon(12, 243);
	Colosso_ShowItemIcon(11, 244);
	Colosso_ShowItemIcon(10, 244);
	Colosso_ShowItemIcon(9, 244);
	Colosso_ShowItemIcon(8, 244);
	if (GameFlag_IsSet(0xee7) == 0)
	{
		Actor_SetPosition(8, 0xe80000, 0x3680000);
	}
	if (GameFlag_IsSet(0xee8) == 0)
	{
		Actor_SetPosition(9, 0x1280000, 0x3380000);
	}
	if (GameFlag_IsSet(0xee9) == 0)
	{
		Actor_SetPosition(10, 0x1480000, 0x2f80000);
	}
	if (GameFlag_IsSet(0xeea) == 0)
	{
		Actor_SetPosition(11, 0x1680000, 0x3680000);
	}
	if (GameFlag_IsSet(0x9c0) != 0)
	{
		CrossbonePlatform_OpenPassage(0);
	}
	if (GameFlag_IsSet(0x9c1) != 0)
	{
		CrossbonePlatform_OpenPassage(1);
	}
	if (GameFlag_IsSet(0x9c2) != 0)
	{
		CrossbonePlatform_OpenPassage(2);
	}
	if (GameFlag_IsSet(0x9c3) != 0)
	{
		CrossbonePlatform_OpenPassage(3);
	}
	if (GameFlag_IsSet(0x9c4) != 0)
	{
		CrossbonePlatform_OpenPlatformPassage(0);
	}
}

static void CrossbonePlatform_InitializeScene2(void)
{
	Object_GetById(8)->collisionFlags = 1;
	Object_GetById(9)->collisionFlags = 1;
	Object_GetById(10)->collisionFlags = 1;
	Object_GetById(11)->collisionFlags = 1;
	Object_GetById(8)->scaleX = 0xb333;
	Object_GetById(9)->scaleX = 0xb333;
	Object_GetById(10)->scaleX = 0xb333;
	Object_GetById(11)->scaleX = 0xb333;
	Object_GetById(12)->scaleX = 0xb333;
	FieldScheduler_AddOrUpdateCallback((s32)SceneState_TriggerColumnNineteen, 0xc80);
	FieldScheduler_AddOrUpdateCallback((s32)SceneState_TriggerColumnTen, 0xc80);
	FieldScheduler_AddCallback((s32)CrossbonePlatform_RaiseTriggerOnStand, 0xc80);
	Io_Write16(0x3f42, &REG_BLDCNT);
	Io_Write16(0x607, &REG_BLDALPHA);
}

static void CrossbonePlatform_InitializeScene3(void)
{
	s32 record;

	Object_GetById(14)->motionFlags = 0;
	FieldScheduler_AddOrUpdateCallback((s32)ActorPresentation_PlaceActorFourteenOnActorNine, 0xc80);
	FieldScheduler_AddCallback((s32)SceneActor_PublishMarkerBySlotZeroHeight, 0xc80);
	MapObject_SetPosition(107, 0, 0);
	if (GameFlag_IsSet(0xed9) != 0)
	{
		Engine_ActorSetAnimation(14, 2);
	}
	EV08_PSYNERGY_3();
	EV09_PSYNERGY_3();
	EV10_PSYNERGY_3();
	EV11_PSYNERGY_3();
	EV12_PSYNERGY_3();
#if EDITION_INTERNATIONAL
	Engine_ActorSetSpritePriority(8, 3);
#endif
	Object_GetById(11)->motionFlags = 0;
	Object_GetById(12)->motionFlags = 0;
	ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
	if (GameFlag_IsSet(0x200) != 0)
	{
		EV13_PSYNERGY();
		Engine_ActorSetAnimation(13, 5);
	}
	if (GameFlag_IsSet(0x109) == 0)
	{
		if (GameFlag_IsSet(0x9ca) != 0)
		{
			Actor_SetPosition(15, 0x3580000, 0x3380000);
			record = Object_GetById(15);
			*(s32 *)(record + 108) = (s32)SceneActor_FaceTowardActorZero;
		}
		else if (GameFlag_IsSet(0x9c9) != 0)
		{
			Actor_SetPosition(15, 0x3780000, 0x2980000);
			record = Object_GetById(15);
			*(u16 *)(*(s32 *)(record + 80) + 30) = 0;
			ObjectDispatch_ApplyValueToChildren(record, 16);
		}
		else if (GameFlag_IsSet(0x9c8) != 0)
		{
			Actor_SetPosition(15, 0x2480000, 0x2a80000);
		}
		else
		{
			Actor_SetPosition(15, 0x2480000, 0x2980000);
		}
	}
}

// 向きごとの一歩: 上位16ビットがX、下位16ビットがZ (ピクセル)
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(16, 0),
};

u32 gStagedActor_FootprintKinds[] = {
	207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
	-32, -8, 32, 8,
	-8, -32, 8, 32,
	-32, -16, 32, 0,
	-8, -32, 8, 32,
	-32, -8, 32, 8,
	-8, -32, 8, 32,
};

s32 gCrossbonePlatform_ActorFifteenMotionScript[] = {
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
	SC_WAIT(64),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gCrossbonePlatform_ActorFifteenWalkTargets[] = {
	0x378, 0x358,
	0x368, 0x358,
	0x358, 0x358,
	0x348, 0x358,
	0x338, 0x358,
	0x338, 0x348,
	0x338, 0x338,
	0x348, 0x338,
	0x358, 0x338,
	0x368, 0x338,
	0x378, 0x338,
	0x378, 0x348,
};

struct SceneEntrance gCrossbonePlatformEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gCrossbonePlatformEntrances1[] = {
	{ 1, CONDITION_ALWAYS, 200, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
	{ 2, CONDITION_ALWAYS, 88, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gCrossbonePlatformEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
	{ 2, CONDITION_ALWAYS, 88, 0, 392, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gCrossbonePlatformEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 776, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
	{ 2, CONDITION_ALWAYS, 664, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
	{ SCENE_TABLE_END },
};

u32 gCrossbonePlatform_SceneTable[] = {
	SCENE_EXITS(117),
	SCENE_EXIT(1, 128, 3),
	SCENE_EXIT(2, 128, 4),
	SCENE_EXIT(0, 118, 0),
	SCENE_EXIT(1, 129, 3),
	SCENE_EXIT(2, 129, 4),
	SCENE_EXIT(0, 120, 0),
	SCENE_EXIT(1, 131, 3),
	SCENE_EXIT(2, 131, 4),
	SCENE_EXITS_END,
};

struct ScenePlacement gCrossbonePlatformPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gCrossbonePlatformPlacements1[] = {
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gCrossbonePlatformPlacements2[] = {
	{ 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(152), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(232), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gCrossbonePlatformPlacements3[] = {
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(616), 0, PIXELS(680), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(728), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(600), 0, PIXELS(856), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(824), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(840), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(648), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x14, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(728), 0, PIXELS(760), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x5a, 0x36, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gCrossbonePlatformEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gCrossbonePlatformEvents1[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectAndPlaceSlot12) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(ConfigureAndPlaceActorTwelve) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_PSYNERGY_1) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_PSYNERGY_1) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_PSYNERGY_1) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_PSYNERGY_1) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_PSYNERGY_1) },
	{ EVENT_TOUCH, 50, CONDITION_ALWAYS, EVENT_SCRIPT(EV50_TOUCH) },
	{ EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(EV51_TOUCH) },
	{ EVENT_TOUCH, 52, CONDITION_ALWAYS, EVENT_SCRIPT(EV52_TOUCH) },
	{ EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(EV53_TOUCH_1) },
	{ EVENT_TOUCH, 54, CONDITION_ALWAYS, EVENT_SCRIPT(EV54_TOUCH_1) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 53, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_CheckLeaderAhead) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 54, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_CheckLeaderAhead) },
	{ SEARCH(SEARCH_CHEST), 100, 0xeca, FIND_COINS(333) },
	{ SEARCH(SEARCH_CHEST), 101, 0xecb, FIND_ITEM(0x10b) },
	{ SEARCH(SEARCH_CHEST), 102, 0xecc, FIND_ITEM(0xc0) },
	{ SEARCH(SEARCH_CHEST), 103, 0xecd, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gCrossbonePlatformEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(EV53_TOUCH_2) },
	{ EVENT_TOUCH, 54, CONDITION_ALWAYS, EVENT_SCRIPT(EV54_TOUCH_2) },
	{ EVENT_TOUCH, 61, CONDITION_ALWAYS, EVENT_SCRIPT(EV61_TOUCH) },
	{ EVENT_TOUCH, 62, CONDITION_ALWAYS, EVENT_SCRIPT(EV62_TOUCH) },
	{ EVENT_RAISED, 96, CONDITION_ALWAYS, EVENT_SCRIPT(EV96_RAISED) },
	{ EVENT_RAISED, 92, CONDITION_ALWAYS, EVENT_SCRIPT(EV92_RAISED) },
	{ EVENT_RAISED, 91, CONDITION_ALWAYS, EVENT_SCRIPT(EV91_RAISED) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(SetBlendAlphaCoefficients) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_SetBlendAlpha0607) },
	{ PSYNERGY_ON_ACTOR(0x91), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
	{ PSYNERGY_ON_ACTOR(0x91), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
	{ PSYNERGY_ON_ACTOR(0x91), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
	{ PSYNERGY_ON_ACTOR(0x91), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
	{ PSYNERGY_ON_ACTOR(0x91), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
	{ SEARCH(SEARCH_CHEST), 100, 0xece, FIND_COINS(444) },
	{ SEARCH(SEARCH_CHEST), 101, 0xecf, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SEARCH(SEARCH_CHEST), 102, 0xed0, FIND_ITEM(0x60) },
	{ SEARCH(SEARCH_CHEST), 103, 0xed1, FIND_ITEM(0xba) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gCrossbonePlatformEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 50, CONDITION_ALWAYS, EVENT_SCRIPT(EV50_SCRIPT) },
	{ 0x202, 51, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_BranchOnSlotZeroAtTile38) },
	{ 0x202, 52, CONDITION_ALWAYS, EVENT_SCRIPT(EV52_SCRIPT) },
	{ 0x202, 53, CONDITION_ALWAYS, EVENT_SCRIPT(EV53_SCRIPT) },
#if EDITION_INTERNATIONAL
	{ 0x202, 61, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_BranchOnSlotZeroAtTile38) },
#endif
	{ 0x202, 54, CONDITION_ALWAYS, EVENT_SCRIPT(EV54_SCRIPT) },
	{ 0x202, 60, CONDITION_ALWAYS, EVENT_SCRIPT(EV60_SCRIPT) },
	{ 0x202, 70, CONDITION_ALWAYS, EVENT_SCRIPT(EV70_SCRIPT) },
	{ EVENT_TOUCH, 20, 0x36, EVENT_SCRIPT(EV20_TOUCH) },
	{ EVENT_TOUCH, 21, 0x36, EVENT_SCRIPT(EV21_TOUCH) },
	{ EVENT_TOUCH, 22, 0x36, EVENT_SCRIPT(EV22_TOUCH) },
	{ EVENT_TOUCH, 30, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 31, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 32, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 33, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 34, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 35, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 36, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 37, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 38, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 39, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 40, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 41, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ 0x7, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(0x91), 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, 0x200, EVENT_SCRIPT(EV13_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x201, EVENT_SCRIPT(EV08_PSYNERGY_3) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x206, EVENT_SCRIPT(EV09_PSYNERGY_3) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x203, EVENT_SCRIPT(EV10_PSYNERGY_3) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, 0x204, EVENT_SCRIPT(EV11_PSYNERGY_3) },
	{ PSYNERGY_ON_ACTOR(0x8c), 12, 0x205, EVENT_SCRIPT(EV12_PSYNERGY_3) },
	{ SEARCH(SEARCH_CHEST), 104, 0xed6, FIND_COINS(666) },
	{ SEARCH(SEARCH_CHEST), 105, 0xed7, FIND_ITEM(ITEM_WATER_OF_LIFE) },
	{ SEARCH(SEARCH_CHEST), 106, 0xed8, FIND_ITEM(ITEM_ELIXIR) },
	{ SEARCH(SEARCH_CHEST), 107, 0xed9, FIND_ITEM(0x26) },
	{ SCENE_EVENTS_END },
};

u32 gCrossbonePlatform_SlotColumns[] = {
	16, 21, 0,
};

s32 gCrossbonePlatform_IconTimer = 0;
