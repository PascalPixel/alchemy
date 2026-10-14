#include "game_state.h"
#include "io_write_queue.h"
#include "scene_ids.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/korosseo_kawa.h"
#include "unit/scene/common/korosseo_solo.h"
#include "unit/scene/common/korosseo_marker.h"
#include "unit/scene/common/korosseo_setup.h"
#include "unit/scene/common/korosseo_occupant.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/evtable.h"
#include "unit/object/marker.h"
#include "unit/object/sparkle.h"
#include "unit/object/evbasic.h"
#include "unit/object/evactor.h"
#include "unit/object/evcamera.h"
#include "unit/object/djinnev.h"
#include "unit/object/psydisp.h"
#include "unit/object/psyfx.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/btlcmd.h"
#include "unit/menu/menu.h"
#include "unit/sound/sound.h"
#include "unit/runtime/runtime.h"
#include "unit/scene/common/korosseo_item_icon.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "callback_scheduler.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "resource_ids.h"
#include "text/msg_ids.h"
#include "battle_unit.h"
#include "resource.h"
#include "object_runtime.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fieldobj.h"
#include "heap_state.h"

// 前方宣言
static void FieldScene_RunCommandSequence(s32 a0);
static void FieldScene_RunTwoArmSequence(s32 a);
static void FieldScene_RunLateSequence(s32 a0);
static void Resource3ba_NoOpCallback(void);
static void KorosseoKawa_WaitForStageReady(void);
static s32 FieldScene_RunFlag211ApproachScene(s32 handle_a, s32 handle_b);
static void StagedActor_PushActorAhead(void);

typedef struct PartyInteractionRecord
{
	u8	padding_00[10];
	s16 x;
	u8	padding_0c[6];
	s16 y;
} PartyInteractionRecord;

typedef void (*SceneTask)(void);

#define KEYS_HELD gKeysHeld // VBlank で書く

// コロッセオ共通 (scene/common/korosseo_*.c)

u8 *KorosseoKawa_GetEntrances(void)
{

	return (u8 *)KorosseoKawa_SceneTableA;
}

s32 get_default_result(void)
{
	return 0;
}

u8 *KorosseoKawa_GetExits(void)
{

	return (u8 *)KorosseoKawa_SceneTableB;
}

u8 *KorosseoKawa_GetPlacements(void)
{

	return (u8 *)KorosseoKawa_SceneTableC;
}

static void FieldScene_RunOpeningAuxiliarySequence(void)
{
	switch ((u32)KorosseoKawa_Countdown)
	{
	case 66:
		Engine_MapCopyCellsLayered(92, 31, 2, 2, 50, 38);
		Engine_MapCopyCellsLayered(92, 31, 2, 2, 54, 38);
		Engine_ActorSetAnimation(16, 10);
		break;
	case 60:
		Engine_MapCopyCellsLayered(92, 33, 2, 2, 50, 38);
		Engine_MapCopyCellsLayered(92, 33, 2, 2, 54, 38);
		Map_CopyCellAttributes(50, 25, 6, 1, 50, 12);
		Engine_ActorSetAnimation(16, 11);
		break;
	case 6:
		Engine_MapCopyCellsLayered(92, 31, 2, 2, 50, 38);
		Engine_MapCopyCellsLayered(92, 31, 2, 2, 54, 38);
		Engine_ActorSetAnimation(16, 10);
		break;
	case 0:
		Engine_MapCopyCellsLayered(92, 29, 2, 2, 50, 38);
		Engine_MapCopyCellsLayered(92, 29, 2, 2, 54, 38);
		Engine_ActorSetAnimation(16, 12);
		Map_CopyCellAttributes(50, 24, 6, 1, 50, 12);
		KorosseoKawa_Countdown = 120;
		break;
	}
	KorosseoKawa_Countdown--;
}

static void SceneState_ResetCounterAndStartTask(void)
{
	SceneTask task;

	KorosseoKawa_Countdown = 0;
	task = (SceneTask)FieldScene_RunOpeningAuxiliarySequence;
	Engine_TaskRemoveCallback(task);
	task();
}

static void SceneState_SetMode66AndPassOpeningSequence(void)
{
	KorosseoKawa_Countdown = 66;
	FieldScheduler_AddCallback((s32)FieldScene_RunOpeningAuxiliarySequence, 0xC80);
}

static void KorosseoKawa_WaitForCountdown(void)
{
	s32 i;

	Engine_TaskWait(10);
	i = 0;
	if (KorosseoKawa_Countdown != 22)
	{
		do
		{
			Engine_TaskWait(1);
			i++;
			if (i > 119)
			{
				break;
			}
		} while (KorosseoKawa_Countdown != 22);
	}
}

static void SceneState_ApplyRectsForActorsNineAndTen(void)
{

	struct FieldActor *actor;
	s32				   blocked;

	Map_CopyCellAttributes(27, 13, 3, 1, 23, 12);
	actor = Object_GetById(9);
	blocked = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
	if (actor->y.fixed == 0 && blocked == 0)
	{
		actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
		actor->motion_flags = 0;
		{
			s32 x = actor->x.fixed >> 20;
			s32 y = actor->z.fixed >> 20;

			Map_CopyCellAttributes(14, 13, 1, 1, x, y);
		}
	}
	actor = Object_GetById(10);
	{
		s32 x = actor->x.fixed >> 20;

		FieldGameFlag_SetByte(784, x);
	}
	{
		s32 x = actor->x.fixed >> 20;
		s32 y = actor->z.fixed >> 20;

		Map_CopyCellAttributes(14, 13, 1, 1, x, y);
	}
}

static void FieldScene_RunTwoCallSequence(void)
{

	StagedActor_PushActorAhead();
	SceneState_ApplyRectsForActorsNineAndTen();
}

static void EV15_PSYNERGY(void)
{
	{

		Map_CopyCellAttributes(47, 24, 1, 1, 47, 12);
	}
	Engine_GameFlagSet(0x303);
}

static void EV13_PSYNERGY(void)
{
	u8 *rec7;
	s32 none;

	GameFlag_Set(0x301);
	rec7 = Object_GetById(13);
	Engine_EventBegin();
	Camera_SetSpeed(0x20000, 0x4000);
	Camera_MoveTo(0x2580000, -1, 0xc80000, 1);
	Object_SetMode((s32)rec7, 3);
	Engine_CameraWaitForMove();
	none = 0;
	((struct FieldActor *)rec7)->motion_flags = none;
	((struct FieldActor *)rec7)->acceleration = 0x6666;
	((struct FieldActor *)rec7)->speed = 0xcccc;
	FieldObject_SetMoveTarget((s32)rec7, ((struct FieldActor *)rec7)->x.fixed, 0x80000,
		((struct FieldActor *)rec7)->z.fixed);
	rec7 = Actor_Get(14);
	((struct FieldActor *)rec7)->motion_flags = none;
	((struct FieldActor *)rec7)->acceleration = 0x6666;
	((struct FieldActor *)rec7)->speed = 0xcccc;
	Engine_ObjectSetPosition((s32)rec7, ((struct FieldActor *)rec7)->x.fixed, 0x200000,
		((struct FieldActor *)rec7)->z.fixed);
	Object_CommitPosition((s32)rec7);
	Engine_EventWait(45);
	Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
	Engine_EventEnd();
}

static void StagedActor_PlacePairAtOffsetAndRun(s32 actor_id, s32 dx, s32 dz)
{
	struct FieldActor *leader;
	struct FieldActor *actor;
	s32				   x;
	s32				   z;

	leader = Object_GetById(gGameState.selected_actor);
	actor = Object_GetById(actor_id);
	Engine_EventBegin();
	{
		x = ((leader->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
		z = ((leader->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

		leader->speed = 0x10000;
		leader->acceleration = 0x8000;
		Engine_ObjectSetPosition(leader, x, leader->y.fixed, z);
	}
	Object_SetMode(leader, 27);
	{
		x = ((actor->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
		z = ((actor->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

		actor->speed = 0x10000;
		actor->acceleration = 0x8000;
		Engine_ObjectSetPosition(actor, x, actor->y.fixed, z);
	}
	if (dx < 0 || dz < 0)
	{
		Object_SetMode(actor, 4);
	}
	else
	{
		Object_SetMode(actor, 3);
	}
	Engine_AudioPlayCue(226);
	Object_CommitPosition(leader);
	Object_SetMode(actor, 2);
	Engine_AudioPlayCue(288);
	Engine_EventEnd();
}

static void EV20_TOUCH(void)
{
	struct FieldActor *actor;
	s32				   row;
	s32				   offset;

	actor = Object_GetById(gGameState.selected_actor);
	row = actor->z.fixed >> 20;
	offset = -48;
	if (row <= 8)
	{
		offset = 48;
	}
	Map_CopyCellAttributes(67, 8, 3, 1, 64, row);
	StagedActor_PlacePairAtOffsetAndRun(17, 0, offset);
	actor = Object_GetById(17);
	row = actor->z.fixed >> 20;
	Map_CopyCellAttributes(64, 24, 3, 1, 64, row);
}

static void EV21_TOUCH(void)
{
	struct FieldActor *actor;
	s32				   column;
	s32				   offset;
	s32				   direction;

	actor = Object_GetById(gGameState.selected_actor);
	column = actor->x.fixed >> 20;
	if ((KEYS_HELD & 32) != 0)
	{
		direction = -1;
	}
	if ((KEYS_HELD & 16) != 0)
	{
		direction = 1;
	}
	actor = Object_GetById(17);
	offset = actor->z.fixed >> 20;
	if (column == 63)
	{
		if (offset == 11)
		{
			return;
		}
		offset = 160;
	}
	else if (column == 67)
	{
		if (offset == 11 && direction == -1)
		{
			return;
		}
		offset = 96;
	}
	else
	{
		if (offset == 11)
		{
			offset = 96;
		}
		else
		{
			offset = 160;
		}
		offset = -offset;
	}
	Map_CopyCellAttributes(72, 9, 1, 3, column, 9);
	StagedActor_PlacePairAtOffsetAndRun(18, offset, 0);
	actor = Object_GetById(18);
	column = actor->x.fixed >> 20;
	Map_CopyCellAttributes(63, 25, 1, 3, column, 9);
}

static void EV12_TOUCH(void)
{
	s32				   leader_id;
	struct FieldActor *leader;
	struct FieldActor *actor;
	s32				   none;

	leader_id = gGameState.selected_actor;
	leader = Object_GetById(leader_id);
	actor = Object_GetById(12);
	GameFlag_Set(0x302);
	Engine_EventBegin();
	Engine_ActorSetAnimation(leader_id, 8);
	Engine_EventWait(6);
	actor->speed = 0x8000;
	actor->acceleration = 0x3333;
	Event_PlayCue(239);
	Object_SetMode(actor, 2);
	Engine_ObjectSetPosition(actor, actor->x.fixed - 0x300000, 0, actor->z.fixed);
	Engine_EventWait(6);
	Engine_ActorSetAnimation(leader_id, 2);
	ObjectDispatch_InitFromTable4WithArgument(
		*(struct DispatchObject **)(Runtime_AllocateBlock(27, 0xccc) + 480), (s32)actor);
	Actor_SetSpeed(leader_id, 0x4ccc, 0x3333);
	Engine_ObjectSetPosition(leader, leader->x.fixed - 0x180000, 0, leader->z.fixed);
	Engine_ActorWaitForMove(leader_id);
	Engine_ActorSetAnimation(leader_id, 1);
	Object_CommitPosition(actor);
	Object_SetMode(actor, 1);
	Event_PlayCue(288);
	Event_PlayCue(213);
	Engine_EventWait(15);
	Engine_EventEnd();
	Map_CopyCellAttributes(37, 7, 1, 4, 34, 7);
	Map_CopyCellAttributes(36, 7, 1, 4, 37, 7);
	none = GameFlag_IsSet(0x301);
	if (none != 0)
	{
		Engine_EventBegin();
		Camera_SetSpeed(0x20000, 0x4000);
		Camera_MoveTo(0x2280000, -1, 0xc80000, 1);
		Engine_CameraWaitForMove();
		Map_CopyCells(96, 29, 1, 3, 34, 38);
		Engine_EventWait(3);
		Map_CopyCells(97, 29, 1, 3, 34, 38);
		Engine_EventWait(3);
		Map_CopyCells(98, 29, 1, 3, 34, 38);
		Engine_EventWait(3);
		Map_CopyCells(99, 29, 1, 3, 34, 38);
		Engine_EventWait(3);
		Map_CopyCells(100, 29, 1, 3, 34, 38);
		Engine_EventWait(15);
		Engine_EventEnd();
		return;
	}
	GameFlag_Set(0x301);
	Engine_EventBegin();
	Camera_SetSpeed(0x20000, 0x4000);
	Camera_MoveTo(0x2580000, -1, 0xc80000, 1);
	Engine_CameraWaitForMove();
	actor = Object_GetById(13);
	actor->motion_flags = none;
	actor->acceleration = 0x6666;
	actor->speed = 0xcccc;
	Engine_ObjectSetPosition(actor, actor->x.fixed, 0x80000, actor->z.fixed);
	Object_SetMode(actor, 3);
	Map_CopyCells(96, 29, 1, 3, 34, 38);
	Engine_EventWait(3);
	Map_CopyCells(97, 29, 1, 3, 34, 38);
	Engine_EventWait(3);
	Map_CopyCells(98, 29, 1, 3, 34, 38);
	Engine_EventWait(3);
	Map_CopyCells(99, 29, 1, 3, 34, 38);
	Engine_EventWait(3);
	Map_CopyCells(100, 29, 1, 3, 34, 38);
	actor = Object_GetById(14);
	actor->motion_flags = none;
	actor->acceleration = 0x6666;
	actor->speed = 0xcccc;
	Engine_ObjectSetPosition(actor, actor->x.fixed, 0x200000, actor->z.fixed);
	Object_CommitPosition(actor);
	Engine_EventWait(15);
	Engine_EventEnd();
	Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
}

static void SceneState_ApplyValue768(void)
{

	Engine_GameFlagSet(768);
}

static void EV50_TOUCH(void)
{
	s32 approach;
	s32 n;
	s32 base;

	base = 0;
	Resource3ba_NoOpCallback();
	Engine_EventBegin();
	approach = FieldScene_RunFlag211ApproachScene(120, 127);
	KorosseoKawa_WaitForStageReady();
	n = 9;
	do
	{
		Object_RefreshSelectorById(8);
		n = n - 1;
	} while (n >= 0);
	Actor_SetSpeed(8, 0x10000, 0x8000);
	Actor_WalkTo(8, 0x528, 192);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x508, 192);
	Engine_ActorSetAnimation(8, 1);
	Engine_ActorFaceEachOther(0, 8, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimation(8, 3);
	Engine_ActorSetAnimationAndWait(0, 3);
	Engine_EventWait(20);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
	Actor_SetSpeed(8, 0x20000, 0x10000);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x510, 192);
	Actor_WalkToAndWait(8, 0x520, 192);
	Engine_ActorSetAnimation(0, 16);
	Engine_ActorSetAnimation(8, 9);
	Engine_EventWait(10);
	/* FAKEMATCH: base is 0 from the top of the function, so 0 - approach + 1 is built as movs #0, subs, adds #1, as the native does. 1 - approach or -approach + 1 folds into movs #1, subs (differ 7 in tbs-en). */
	BattleFx_SetWeightedResult(72, base - approach + 1);
	Battle_SetStart(3);
	Party_SetFields1ceAnd1d0((s32)&SceneId_KorosseoKawa, 4);
	Event_SetDefeatDestination((s32)&SceneId_KorosseoKawa, 5);
	GameFlag_Set(0x11a);
}

static void EV90_SCRIPT(void)
{

	if (SceneActor_FindOccupantAheadOfSubject() == 0)
	{
		Engine_LeaderCheckAhead();
	}
	else
	{
		FieldScene_RunTwoCallSequence();
	}
}

u8 *KorosseoKawa_GetEvents(void)
{

	return (u8 *)KorosseoKawa_SceneTableD;
}

static void KorosseoKawa_RunPostMatchScene(s32 a0)
{


	Engine_ActorDestroy(24);
	Engine_ActorDestroy(25);
	Owner_RefreshActiveRatios(1);
	Engine_EventBegin();
	Actor_SetPosition(8, 0x5280000, 0xc00000);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x5080000, 0xc00000);
	Actor_FaceActor(8, 0x4000, 0);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
	if (a0 < 0)
	{
		Engine_ActorSetAnimation(8, 10);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 35);
	}
	else
	{
		Engine_ActorSetAnimation(8, 8);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 28);
	}
	Engine_TaskWait(1);
	Camera_MoveTo(0x5180000, 0, 0x800000, 0);
	FieldScene_RunLateSequence(a0);
	Engine_EventEnd();
}

static void SceneActor_MarkObjectAtTiles94To95(s32 actor)
{
	struct FieldActor *o;
	s32				   x;
	s32				   y;

	o = Object_GetById(actor);
	if (o != 0)
	{
		x = o->x.fixed >> 19;
		y = o->z.fixed >> 19;
		if (x >= 94 && x <= 95 && y > 23 && y <= 26)
		{
			o->terrain_id = 1;
		}
	}
}

s32 KorosseoKawa_ApplyEntryState(void)
{
	struct FieldActor *actor;
	s32				   hit;
	s32				   x;
	s32				   z;
	u8				   zero;
	s32				   col;

	gEventWork->start_transition = 0;
	GameFlag_Set(0x144);
	actor = Object_GetById(9);
	hit = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
	if (actor->y.fixed == 0 && hit == 0)
	{
		actor->priority_flags = 2;
		actor->motion_flags = hit;
		Map_CopyCellAttributes(14, 13, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
	}
	x = FieldGameFlag_GetByte(0x310);
	if (x == 0)
	{
		x = 25;
	}
	actor = Object_GetById(10);
	actor->x.fixed = (x << 20) + 0x80000;
	actor->motion_flags = 0;
	actor->priority_flags = 2;
	Map_CopyCellAttributes(14, 13, 1, 1, x, 12);
	FieldScheduler_AddCallback(FieldScene_RunOpeningAuxiliarySequence, 0xc80);
	actor = Object_GetById(15);
	actor->terrain_id = 1;
	zero = 0;
	if (Engine_GameFlagIsSet(0x303))
	{
		Object_SetMode(actor, 4);
		Engine_ActorSetSpriteFlags(actor, 0);
		actor->collision_flags = zero;
		actor->priority_flags = 3;
		Map_CopyCellAttributes(47, 24, 1, 1, 47, 12);
	}
	actor = Object_GetById(17);
	col = actor->z.fixed >> 20;
	actor->motion_flags = zero;
	actor->priority_flags = 2;
	Map_CopyCellAttributes(64, 24, 3, 1, 64, col);
	actor = Object_GetById(18);
	col = actor->x.fixed >> 20;
	actor->motion_flags = zero;
	actor->priority_flags = 2;
	Map_CopyCellAttributes(63, 25, 1, 3, col, 9);
	if (Engine_GameFlagIsSet(0x302))
	{
		Map_CopyCellAttributes(37, 7, 1, 4, 34, 7);
		Map_CopyCellAttributes(36, 7, 1, 4, 37, 7);
		Map_CopyCells(100, 29, 1, 3, 34, 38);
	}
	actor = Object_GetById(13);
	if (Engine_GameFlagIsSet(0x301))
	{
		Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
		actor->motion_flags = zero;
		actor->acceleration = 0x6666;
		actor->speed = 0xcccc;
		actor->y.fixed = 0x80000;
		Object_SetMode(actor, 3);
	}
	else
	{
		Object_SetMode(actor, 2);
	}
	Object_GetById(14)->priority_flags = 2;
	Korosseo_ShowItemIcon(24, 120);
	Korosseo_ShowItemIcon(25, 127);
	switch (gGameState.entrance)
	{
	case 1:
		FieldScene_BuildSceneDescriptorAndInstallTask(0, 8, 4, 0x5180000, 0xc00000, 24, 25);
		Map_CopyCellsLayered(127, 0, 1, 2, 19, 2);
		Engine_ActorDestroy(19);
		Engine_ActorDestroy(20);
		Engine_ActorDestroy(21);
		Engine_ActorDestroy(22);
		Engine_ActorDestroy(23);
		if (!Engine_GameFlagIsSet(0x109))
		{
			Engine_AudioPlayCue(17);
			Korosseo_SelectSoloCompetitor(0);
			SceneActor_PlacePartyAtSavedTiles();
			SceneActor_MarkObjectAtTiles94To95(1);
			SceneActor_MarkObjectAtTiles94To95(2);
			SceneActor_MarkObjectAtTiles94To95(3);
			FieldScene_RunTwoArmSequence(1);
		}
		Object_LinkObjectAndSetCallback(1, 0);
		Object_LinkObjectAndSetCallback(2, 0);
		Object_LinkObjectAndSetCallback(3, 0);
		SceneState_InitControlWhenFlag109Clear((s32)&ResourceId_RivalPathA);
		break;
	case 2:
		FieldScheduler_AddCallback(SceneState_RaiseTriggerOnPsynergyTarget, 0xc80);
		Engine_ActorDestroy(24);
		Engine_ActorDestroy(25);
		if (!Engine_GameFlagIsSet(0x109))
		{
			SceneActor_PlacePartyAtSavedTiles();
			Korosseo_SelectSoloCompetitor(1);
			FieldScene_RunTwoArmSequence(0);
		}
		break;
	case 3:
		if (!Engine_GameFlagIsSet(0x109))
		{
			FieldScene_RunCommandSequence(19);
			Korosseo_ClearSavedActorPositions();
		}
		break;
	case 4:
		KorosseoKawa_RunPostMatchScene(1);
		Engine_EventRequestExit(4);
		break;
	case 5:
		KorosseoKawa_RunPostMatchScene(-1);
		Engine_EventRequestExit(5);
		break;
	}
	return 0;
}

static void FieldScene_RunCommandSequence(s32 a0)
{
	struct FieldActor *actor;
	s32				   x;
	s32				   z;

	actor = Object_GetById(a0);
	x = actor->x.part.pixel;
	z = actor->z.part.pixel;
	Engine_EventBegin();
	Actor_SetSpeed(a0, 0x10000, 0x8000);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Actor_SetSpeed(GERALD, 0x10000, 0x8000);
	Actor_SetSpeed(IWAN, 0x10000, 0x8000);
	Actor_SetSpeed(MEARI, 0x10000, 0x8000);
	Engine_ActorSetPosition(0, x << 16, (z << 16) - 0x300000);
	Actor_SetPosition(GERALD, (x << 16) - 0x100000, (z << 16) - 0x280000);
	Actor_SetPosition(IWAN, (x << 16) + 0x100000, (z << 16) - 0x280000);
	Actor_SetPosition(MEARI, x << 16, (z << 16) - 0x200000);
	Actor_SetPosition(a0, x << 16, (z << 16) - 0x500000);
	actor = Object_GetById(0);
	actor->facing = 0xc000;
	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventSetMessage((s32)MsgKorosseoSiteFirstFinalsBattle);
	Event_ShowMessage(a0, 0);
	Actor_ShowEmote(MEARI, 0x101, 60);
	Event_ShowMessage(MEARI, 0);
	Engine_ActorStartRepeatedMotion(a0, 3);
	Event_ShowMessage(a0, 0);
	Actor_ShowEmote(IWAN, 0x101, 60);
	Event_ShowMessage(IWAN, 0);
	Actor_FaceActor(a0, IWAN, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(a0, 3);
	Event_ShowMessage(a0, 0);
	Actor_ShowEmote(GERALD, 0x101, 60);
	Event_ShowMessage(GERALD, 0);
	Actor_ShowEmote(MEARI, 0x101, 60);
	Event_ShowMessage(MEARI, 0);
	Actor_ShowEmote(a0, 0x102, 60);
	if (Event_AskYesNo(a0, 0) == 0)
	{
		do
		{
			Engine_EventSetMessage((s32)MsgKorosseoWarriorsEnterFinalsWithoutAny);
			Engine_ActorSetAnimation(IWAN, 3);
			Engine_EventWait(2);
			Engine_ActorSetAnimation(GERALD, 3);
			Engine_EventWait(2);
			Engine_ActorSetAnimation(MEARI, 3);
			Engine_EventWait(1);
			Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
			Engine_ActorSetAnimationAndWait(a0, 3);
			Event_ShowMessage(a0, 0);
			Actor_FaceDirection(a0, 0xa000, 0);
			Engine_EventWait(20);
			Event_ShowMessage(a0, 0);
			Camera_SetSpeed(0x30000, 0x6000);
			Camera_MoveTo(0x1380000, -1, 0x680000, 1);
			Engine_CameraWaitForMove();
			Event_ShowMessage(a0, 0);
			Camera_SetSpeed(0x18000, 0x3000);
			Camera_MoveTo(0x3080000, -1, 0x680000, 1);
			Event_ShowMessage(a0, 0);
			Engine_CameraWaitForMove();
			Event_ShowMessage(a0, 0);
			Camera_SetSpeed(0x30000, 0x6000);
			Camera_MoveTo(0x4d80000, -1, 0xa80000, 1);
			Engine_CameraWaitForMove();
			Actor_FaceActor(a0, 0x6000, 0);
			Event_ShowMessage(a0, 0);
			Camera_MoveTo(0x5180000, -1, 0xa80000, 1);
			Engine_CameraWaitForMove();
			Actor_FaceActor(a0, ACTOR_PARTY_LEADER, 0);
			Event_ShowMessage(a0, 0);
			Event_ShowMessage(a0, 0);
			Event_ShowMessage(a0, 0);
			Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
			Engine_ActorRunRepeatedMotion(a0, 2);
		} while (Event_AskYesNo(a0, 0) != 0);
		Engine_ActorRunRepeatedMotion(a0, 2);
		Engine_EventSetMessage((s32)MsgKorosseoAskAttendantsForExplanationsStages);
		Event_ShowMessage(a0, 0);
	}
	Engine_EventSetMessage((s32)MsgKorosseoRobinYoureContestantInFinals);
	Engine_ActorRunRepeatedMotion(a0, 2);
	Event_ShowMessage(a0, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
	Actor_FaceDirection(GERALD, 0, 0);
	Actor_FaceDirection(IWAN, 0x8000, 0);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Event_ShowMessage(GERALD, 0);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Event_ShowMessage(IWAN, 0);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Event_ShowMessage(MEARI, 0);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_EventWait(1);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_EventWait(2);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_EventWait(1);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(6);
	Engine_ActorSetAnimation(GERALD, 2);
	actor = Object_GetById(0);
	if (actor != 0)
	{
		Actor_SetDestination(GERALD, actor->x.part.pixel, actor->z.part.pixel);
	}
	Engine_ActorSetAnimation(IWAN, 2);
	actor = Object_GetById(0);
	if (actor != 0)
	{
		Actor_SetDestination(IWAN, actor->x.part.pixel, actor->z.part.pixel);
	}
	Engine_ActorSetAnimation(MEARI, 2);
	actor = Object_GetById(0);
	if (actor != 0)
	{
		Actor_SetDestination(MEARI, actor->x.part.pixel, actor->z.part.pixel);
	}
	Actor_WalkToAndWait(a0, x - 16, z - 64);
	Actor_SetPosition(GERALD, 0, 0);
	Actor_SetPosition(IWAN, 0, 0);
	Actor_SetPosition(MEARI, 0, 0);
	Actor_WalkToAndWait(a0, x - 16, z - 16);
	Actor_WalkToAndWait(a0, x, z);
	Actor_FaceDirection(a0, 0xc000, 10);
	Engine_EventEnd();
}

static void EV20_TALK(s32 scene)
{
	s32 path;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(scene);
	}
	else
	{
		Engine_EventBegin();
		path = SceneDialogue_RunFlagGatedPromptInteraction(scene, 1);
		if (path == 0)
		{
			Engine_EventSetMessage((s32)MsgKorosseoCallRockChallenge);
			Camera_SetSpeed(0x30000, 0x6000);
			Camera_MoveTo(PIXELS(328), -1, PIXELS(168), 1);
			Engine_CameraWaitForMove();
			Engine_EventShowMessage(scene, 0);
			Korosseo_FadeInCompetitor(0, 280, 200);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
			Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 200);
			Engine_EventWait(30);
			Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
			Engine_EventShowMessage(scene, 0);
			Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 312, 200);
			Engine_EventWait(30);
			Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 10);
			Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x106, 60);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
			OverlayObject_PlaceWithScale14000(0, 296, 184);
			OverlayObject_PlaceWithScale14000(0, 296, 152);
			OverlayObject_PlaceWithScale14000(0, 312, 152);
			Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 15);
			StagedActor_PushActorAhead();
			OverlayObject_ResetMotionFields(0);
			StagedActor_PushActorAhead();
			OverlayObject_ResetMotionFields(0);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
			Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 304, 184);
			Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 296, 192);
			Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 296, 200);
			Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0, 15);
			StagedActor_PushActorAhead();
			OverlayObject_ResetMotionFields(0);
			StagedActor_PushActorAhead();
			OverlayObject_ResetMotionFields(0);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
			Engine_EventShowMessage(scene, 0);
			Korosseo_RestoreCompetitor(0);
			Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
			Actor_SetPosition(9, 0x1380000, 0xa80000);
			Korosseo_ShowFollowUpPrompt(scene, 1);
		}
		else if (path == 1)
		{
			Engine_EventSetMessage((s32)MsgKorosseoObjectiveStageClear);
			Engine_EventShowMessage(scene, 0);
		}
		FieldScene_RunMiddleSequence(path, scene, 1);
		Engine_EventEnd();
	}
}

static void EV21_TALK(s32 a0)
{
	s32				   rec4;
	struct FieldActor *second;
	struct FieldActor *first;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(a0);
	}
	else
	{
		Engine_EventBegin();
		rec4 = SceneDialogue_RunFlagGatedPromptInteraction(a0, 2);
		if (rec4 == 0)
		{
			Engine_EventSetMessage((s32)MsgKorosseoAreaCalledPipeworks);
			Engine_CameraSetSpeed(0x30000, 0x6000);
			Camera_MoveTo(0x2500000, -1, 0x780000, 1);
			Engine_CameraWaitForMove();
			Engine_EventWait(60);
			Engine_CameraSetSpeed(0x18000, 0x3000);
			Engine_CameraMoveTo(0x2600000, -1, 0xd80000, 1);
			Engine_CameraWaitForMove();
			Engine_EventShowMessage(a0, 0);
			SceneEffect_SeedMarkerAndInstallTask(56, 64, 0);
			Engine_EventWait(60);
			SceneState_StartMarkerMove(160, 96, 10);
			Engine_EventWait(70);
			Engine_EventShowMessage(a0, 0);
			SceneEffect_RemoveMarkerTask();
			Engine_TaskWait(2);
			first = Object_GetById(13);
			first->motion_flags = 0;
			first->acceleration = 0x6666;
			first->speed = 0xcccc;
			Engine_ObjectSetPosition(first, first->x.fixed, 0x80000, first->z.fixed);
			second = Object_GetById(14);
			second->motion_flags = 0;
			second->acceleration = 0x6666;
			second->speed = 0xcccc;
			Engine_ObjectSetPosition(second, second->x.fixed, 0x200000, second->z.fixed);
			Object_CommitPosition((s32)second);
			Engine_EventWait(45);
			first = Object_GetById(13);
			first->motion_flags = 0;
			first->acceleration = 0x6666;
			first->speed = 0xcccc;
			Engine_ObjectSetPosition(first, first->x.fixed, 0x180000, first->z.fixed);
			second = Actor_Get(14);
			second->motion_flags = 0;
			second->acceleration = 0x6666;
			second->speed = 0xcccc;
			Engine_ObjectSetPosition(second, second->x.fixed, 0, second->z.fixed);
			Object_CommitPosition((s32)second);
			Engine_EventWait(15);
			Engine_EventShowMessage(a0, 0);
			SceneEffect_SeedMarkerAndInstallTask(56, 64, 0);
			Engine_EventWait(30);
			SceneState_StartMarkerMove(160, 96, 10);
			Engine_EventWait(40);
			SceneState_StartMarkerMove(56, 64, 10);
			Engine_EventWait(70);
			Engine_EventShowMessage(a0, 0);
			SceneEffect_RemoveMarkerTask();
			Engine_TaskWait(2);
			Engine_CameraFollowActor(0, 0);
			Korosseo_ShowFollowUpPrompt(a0, 2);
		}
		else if (rec4 == 1)
		{
			Engine_EventSetMessage((s32)MsgKorosseoObjectiveMakeGood);
			Engine_EventShowMessage(a0, 0);
		}
		FieldScene_RunMiddleSequence(rec4, a0, 2);
		Engine_EventEnd();
	}
}

static void EV22_TALK(s32 scene)
{
	s32 path;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(scene);
	}
	else
	{
		Engine_EventBegin();
		path = SceneDialogue_RunFlagGatedPromptInteraction(scene, 3);
		if (path == 0)
		{
			Engine_EventSetMessage((s32)MsgKorosseoTheyCallBrokenBridge);
			SceneState_ResetCounterAndStartTask();
			Camera_SetSpeed(0x30000, 0x6000);
			Camera_MoveTo(PIXELS(840), -1, PIXELS(216), 1);
			Engine_CameraWaitForMove();
			Event_ShowMessage(scene, 0);
			SceneState_SetMode66AndPassOpeningSequence();
			Engine_EventWait(60);
			Event_ShowMessage(scene, 0);
			Korosseo_FadeInCompetitor(0, 736, 200);
			/* FAKEMATCH: called as returning a value, the discarded result's call sets r0 after both other arguments, as the native does. Called directly, through Actor_FaceDirection or through Call3, r0 is set first (differ 2 in tbs-en). */
			((s32 (*)())Engine_ActorFaceDirection)(ACTOR_PARTY_LEADER, 0, 0);
			KorosseoKawa_WaitForCountdown();
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
			Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 816, 200);
			Engine_EventWait(30);
			Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
			Event_ShowMessage(scene, 0);
			Korosseo_RestoreCompetitor(0);
			Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
			Korosseo_ShowFollowUpPrompt(scene, 3);
		}
		else if (path == 1)
		{
			Engine_EventSetMessage((s32)MsgKorosseoOperatorBridgeWillAlsoCheer);
			Event_ShowMessage(scene, 0);
		}
		FieldScene_RunMiddleSequence(path, scene, 3);
		Engine_EventEnd();
	}
}

static void EV23_TALK(s32 scene)
{
	s32 path;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(scene);
		return;
	}
	Engine_EventBegin();
	path = SceneDialogue_RunFlagGatedPromptInteraction(scene, 4);
	if (path == 0)
	{
		Engine_EventSetMessage((s32)MsgKorosseoPlaceNormallyCalledLumberWater);
		Camera_SetSpeed(0x30000, 0x6000);
		Camera_MoveTo(PIXELS(1088), -1, PIXELS(168), 1);
		Engine_CameraWaitForMove();
		Event_ShowMessage(scene, 0);
		SceneEffect_SeedMarkerAndInstallTask(120, 72, 0);
		Event_ShowMessage(scene, 0);
		SceneEffect_RemoveMarkerTask();
		Engine_EventWait(15);
		Korosseo_FadeInCompetitor(0, 984, 200);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 10);
		Event_ShowMessage(scene, 0);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 30);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x106, 60);
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
		OverlayObject_PlaceWithScale14000(0, 1000, 192);
		OverlayObject_PlaceWithScale14000(0, 1000, 176);
		OverlayObject_PlaceWithScale14000(0, 1016, 168);
		Engine_EventWait(15);
		/* FAKEMATCH: called through a cast to the s32 call type the bytes show, its result discarded (as call.h's Value3 did, 2026-10-13); called directly or through Call3 the argument moves come in another order (differ 2 in tbs-en). */
		((s32 (*)())StagedActor_PlacePairAtOffsetAndRun)(18, 160, 0);
		Camera_MoveTo(0x4400000, -1, 0xa80000, 1);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
		Engine_EventWait(10);
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1192, 168);
		Engine_EventWait(10);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 30);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
		Event_ShowMessage(scene, 0);
		Korosseo_RestoreCompetitor(0);
		Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
		Actor_SetPosition(18, 0x3f80000, 0xa80000);
		Korosseo_ShowFollowUpPrompt(scene, 4);
	}
	else if (path == 1)
	{
		Engine_EventSetMessage((s32)MsgKorosseoLogsKeyClearingStage);
		Event_ShowMessage(scene, 0);
	}
	FieldScene_RunMiddleSequence(path, scene, 4);
	Engine_EventEnd();
}

static inline void InitializeActorZero(void)
{
	Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
	Engine_ActorSetSpeed(actorId, 0x10000, 0x8000);
}

static __inline__ void AdvanceMessage(s32 amount)
{
	gEventWork->message += amount;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/korosseo_solo.c"
#include "common/item.c"
#include "common/korosseo_task.c"

static void SceneData_SelectBlockAndResetCounters(u32 mode, u32 param)
{
	s32 handler;

	Korosseo_ModeTaskMode = mode;
	Korosseo_ModeTaskParam = (param << 4);

	FieldScheduler_AddCallback((s32)Korosseo_UpdateModeTask, 0xc80);

	handler = (s32)KorosseoKawa_RoundSpans;
	if (mode == 2)
	{
		handler = (s32)KorosseoKawa_ModeRecordTwo;
	}
	if (mode == 4)
	{
		handler = (s32)KorosseoKawa_ModeRecordFour;
	}
	if (mode == 3)
	{
		if (param != 0)
		{
			handler = (s32)KorosseoKawa_SpanB;
		}
		else
		{
			handler = (s32)KorosseoKawa_SpanA;
		}
	}

	Korosseo_ModeTaskTimer = 0;
	Korosseo_ModeTaskScript = (s16 *)handler;
	Korosseo_ModeMoveTarget = 0;
	Korosseo_ModeMoveDuration = 0;
	Korosseo_ModeTaskPosition = 0;
}

static void FieldScene_RunTwoArmSequence(s32 a)
{
	if (a == 0)
	{
		Engine_EventBegin();
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		Engine_EventWait(30);
		Engine_AudioPlayCue(89);
		Korosseo_LoadPortrait(0);
		SceneData_SelectBlockAndResetCounters(1, 0);
		Engine_EventWait(120);
		Engine_EventEnd();
	}
	else
	{
		Engine_AudioPlayCue(247);
		Engine_EventBegin();
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		KorosseoKawa_RoundSpans[15] = a * 60;
		Engine_EventWait(30);
		Engine_AudioPlayCue(a + 90);
		Korosseo_LoadPortrait(a);
		SceneData_SelectBlockAndResetCounters(1, 0);
		Engine_EventWait(120);
		while (AudioCommand_GetStateByte() != 0)
		{
			Engine_TaskWait(1);
		}
		Engine_AudioPlayCue(0x121);
		Korosseo_LoadPortrait(5);
		SceneData_SelectBlockAndResetCounters(2, 0);
		Engine_AudioPlayCue(236);
		Engine_EventWait(60);
		SceneData_SelectBlockAndResetCounters(2, 1);
		Engine_AudioPlayCue(236);
		Engine_EventWait(60);
		Korosseo_LoadPortrait(6);
		SceneData_SelectBlockAndResetCounters(2, 0);
		Engine_AudioPlayCue(236);
		Engine_EventWait(60);
		Korosseo_LoadPortrait(7);
		SceneData_SelectBlockAndResetCounters(4, 0);
		Engine_AudioPlayCue(237);
		Audio_PlayCueFromEventWork();
		Engine_EventEnd();
		Engine_GameFlagSet(0x123);
	}
}

static void FieldScene_RunLateSequence(s32 a0)
{

	s32 kind;

	Engine_AudioPlayCue(247);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	KorosseoKawa_SpanA[13] = a0 * 60;
	STORE_CONST(KorosseoKawa_SpanB[13]) = (a0 < 0 ? -a0 : a0) * 60;
	if (a0 < 0)
	{
		Engine_EventWait(30);
		Engine_AudioPlayCue(86);
		Korosseo_LoadPortrait(8);
		/* FAKEMATCH: called through a cast to the s32 call type the bytes show, its result discarded (as call.h's Value2 did, 2026-10-13); called directly or through Call2 the argument moves come in another order (differ 2 in tbs-en). */
		((s32 (*)())SceneData_SelectBlockAndResetCounters)(3, 1);
		Engine_EventWait(-a0 * 60 + 60);
		kind = 0;
	}
	else
	{
		Engine_EventWait(30);
		Engine_AudioPlayCue(a0 + 90);
		Korosseo_LoadPortrait(4);
		/* FAKEMATCH: called through a cast to the s32 call type the bytes show, its result discarded (as call.h's Value2 did, 2026-10-13); called directly or through Call2 the argument moves come in another order (differ 2 in tbs-en). */
		((s32 (*)())SceneData_SelectBlockAndResetCounters)(3, 0);
		Engine_EventWait(a0 * 60 + 60);
		kind = 8;
	}
	Engine_ActorShowEmote(kind, 0x105, 0);
	while (AudioCommand_GetStateByte() != 0)
	{
		Engine_TaskWait(1);
	}
	Engine_AudioPlayCue(19);
	Engine_EventWait(30);
	Engine_AudioPlayCue(0x121);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/korosseo_marker.c"
#include "common/korosseo_item_icon.c"

static void Text_WriteU32AsHex(u8 *buf, u32 value)
{
	s32 i;

	buf += 8;
	*buf = 0;
	buf--;
	for (i = 7; i >= 0; i--)
	{
		*buf = HexDigits[value & 15];
		value >>= 4;
		buf--;
	}
}

static void Resource3ba_NoOpCallback(void)
{
}

static void KorosseoKawa_MarkStageReady(void)
{
	s16 *ready = (s16 *)gSceneState;
	u16	 nine = 9;

	*ready = nine;
}

static void KorosseoKawa_WaitForStageReady(void)
{
	s16 *ready = (s16 *)gSceneState;

	while (*ready != 9)
	{
		Engine_TaskWait(1);
	}
}

static void SceneEffect_SpawnKind285AtRandomChance(struct FieldActor *a)
{
	s32 t[3];
	u32 n;

	if (a->velocity_y >= -255 && a->velocity_y <= 255)
	{
		a->motion_flags = 0;
	}
	n = Engine_RandomNext();
	if (n * 100 >> 16 <= 9)
	{
		struct FieldActor *o;
		s32				   u;
		s32				   w;

		t[0] = a->x.fixed;
		t[1] = a->y.fixed;
		t[2] = a->z.fixed;
		u = Engine_RandomNext();
		w = Engine_RandomNext();
		Vector_AddPolarOffset(u << 4, w, t);
		{
			s32 x = t[0];
			s32 y = t[1];
			s32 z = t[2];

			o = Engine_ObjectCreate(285, x, y, z);
		}
		if (o != 0)
		{
			o->motion_flags = 0;
			Engine_ActorSetSpriteFlags(o, 0);
			Engine_ObjectSetScript(o, KorosseoKawa_ScriptA);
			Object_SetMode(o, 1);
			Object_SetMode(o, 0);
		}
	}
}

static s32 SceneActor_PlaceLinkedActorAbove(struct FieldActor *a)
{
	struct FieldActor *o = Object_GetById(a->timer);

	Engine_ObjectSetPosition(o, a->x.fixed, a->y.fixed + 0x240000, a->z.fixed);
	o->motion_flags = 0;
	Engine_ObjectSetScript(o, KorosseoKawa_ScriptB);
	Engine_AudioPlayCue(83);
	a->timer = 0;
	return 0;
}

static s32 FieldScene_RunFlag211ApproachScene(s32 handle_a, s32 handle_b)
{

	u8				  *work = gKorosseoWork;
	u8				  *shared;
	struct FieldActor *rec;
	s32				   flag;
	s32				   x;
	s32				   z;
	u16				  *cuep;
	s16				  *waitp;

	flag = Engine_GameFlagIsSet(0x211);

	shared = (u8 *)&gGameState;
	rec = Object_GetById(((struct GameState *)shared)->selected_actor);

	if (*(s32 *)(work + 232) < rec->x.fixed)
	{
		x = *(s32 *)(work + 232) + 0xc0000;
	}
	else
	{
		x = *(s32 *)(work + 232) - 0xc0000;
	}

	if (flag != 0)
	{
		z = *(s32 *)(work + 236) + 0x100000;
		cuep = (u16 *)(work + 228);
	}
	else
	{
		z = *(s32 *)(work + 236) - 0x100000;
		cuep = (u16 *)(work + 226);
	}

	waitp = (s16 *)&rec->timer;
	*waitp = *cuep;
	rec->acceleration = 0x4000;
	rec->speed = 0x10000;

	Engine_ObjectSetPosition(rec, x, 0, z);
	Engine_GameFlagSet(0x211);
	Engine_ObjectSetScript(rec, KorosseoKawa_ApproachScript);

	while (*waitp != 0)
	{
		Engine_TaskWait(1);
	}

	if (flag == 0)
	{
		/* FAKEMATCH: the void function is called as returning s32, the call the bytes show; called by its prototype the argument moves come in another order (differ 2 in tbs-en). */
		((s32 (*)(s32, s32))SceneActor_ApplyValueAndMatchingSlots)(0, handle_a);
		UiWork_PushValueSlot(handle_a, 2);
	}
	else
	{
		/* FAKEMATCH: the same call as returning s32; by its prototype: differ 2 in tbs-en. */
		((s32 (*)(s32, s32))SceneActor_ApplyValueAndMatchingSlots)(0, handle_b);
		UiWork_PushValueSlot(handle_b, 2);
	}

	shared = (u8 *)&gGameState;
	UiWork_PushValueSlot(((struct GameState *)shared)->selected_actor, 1);
	Engine_MessageShowCentered((s32)MsgKorosseoRobinGotItem, 3);
	ObjectDispatch_WaitForValue16(rec);

	return flag;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/korosseo_path_rival.c"
#include "common/korosseo_setup.c"
#include "common/korosseo_scene_state.c"
#include "common/object_findslot.c"

static void StagedActor_PushActorAhead(void)
{

	struct FieldActor *subject;
	struct FieldActor *target;
	struct FieldActor *blocker;
	u32				   step;
	u32				   dir;
	Position3		   pos;
	u32				   idx = 250;
	s32				   zero;
	s32				   handle;

	handle = *(s32 *)((u8 *)&gGameState + (idx << 1));
	subject = Object_GetById(handle);

	dir = subject->facing >> 12;

	step = KorosseoKawa_DirectionSteps[dir];
	pos.x = subject->x.fixed + (step & 0xffff0000);
	pos.y = subject->y.fixed;
	step <<= 16;
	pos.z = subject->z.fixed + step;

	target = SceneData_FindSlotAtPosition(&pos, subject);
	if (target == 0)
	{
		return;
	}

	step = KorosseoKawa_DirectionSteps[dir];
	pos.x = target->x.fixed + (step & 0xffff0000);
	pos.y = target->y.fixed;
	step <<= 16;
	pos.z = target->z.fixed + step;

	blocker = SceneData_FindSlotAtPosition(&pos, target);
	if (blocker != 0 && (blocker->collision_flags & 1) != 0)
	{
		return;
	}

	pos.x = target->x.fixed;
	pos.y = target->y.fixed + 0x100000;
	pos.z = target->z.fixed;

	blocker = SceneData_FindSlotAtPosition(&pos, target);
	if (blocker != 0 && (blocker->collision_flags & 1) != 0)
	{
		return;
	}

	target->terrain_id = 2;
	zero = 0;

	step = KorosseoKawa_DirectionSteps[dir];
	pos.x = target->x.fixed + (step & 0xffff0000);
	pos.y = target->y.fixed;
	step <<= 16;
	pos.z = target->z.fixed + step;

	if (FieldObject_CheckMovementCollision(target, &pos) > 0)
	{
		return;
	}

	Object_SetMode(subject, 8);
	Engine_TaskWait(15);

	target->speed = 0x3333;
	target->acceleration = 0x3333;
	Engine_ObjectSetPosition(target, pos.x, pos.y, pos.z);

	subject->speed = 0x3333;
	subject->acceleration = 0x3333;
	Engine_ObjectSetPosition(subject, pos.x, pos.y, pos.z);

	Engine_AudioPlayCue(0xee);
	Object_CommitPosition(target);
	Engine_AudioPlayCue(0x120);

	target->x.fixed = pos.x;
	target->z.fixed = pos.z;
	target->velocity_x = zero;
	target->velocity_z = zero;

	Object_SetMode(subject, 1);
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/korosseo_occupant.c"

const u8 Korosseo_PortraitPaletteOffsets[] = {
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	0, 32, 32, 32, 96, 64, 64, 64, 128, 0,
#else
	0, 32, 32, 32, 96, 64, 64, 160, 128, 0,
#endif
};

const s16 KorosseoKawa_ModeRecordTwo[] = {
	0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
	30, 0x7fff, 30, -1,
};

const s16 KorosseoKawa_SpanB[] = {
	0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
	6, 0x7fff, 6, 0x1000, 240, 6, 0x7fff, 6,
	0x1000, 368, 6, 0x7fff, 6, 0x1000, 224, 6,
	0x7fff, 6, 0x2000, 0, 30, 0x1000, 352, 6,
	0x7fff, 6, 0x1000, 208, 6, 0x7fff, 6, 0x1000,
	336, 6, 0x7fff, 6, 0x1000, 192, 6, 0x7fff,
	6, 0x1000, 320, 6, 0x7fff, 6, -1,
};

char HexDigits[20] = "0123456789ABCDEF";

s32 KorosseoKawa_ScriptA[] = {
	SC_ADD(SW_Y, -0x4000),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_FREE,
};

s32 KorosseoKawa_ScriptB[] = {
	SC_SET(SW_HOOK, (s32)SceneEffect_SpawnKind285AtRandomChance),
	SC_LABEL(1),
	SC_WAIT(10),
	SC_SET(SW_SCALE_X, 0x16000),
	SC_SET(SW_SCALE_Y, 0x16000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x18000),
	SC_SET(SW_SCALE_Y, 0x18000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x12000),
	SC_SET(SW_SCALE_Y, 0x12000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x16000),
	SC_SET(SW_SCALE_Y, 0x16000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x18000),
	SC_SET(SW_SCALE_Y, 0x18000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x12000),
	SC_SET(SW_SCALE_Y, 0x12000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(2),
	SC_WAIT(50),
	SC_LOOP(2, SC_TO(1)),
	SC_POS(0, 0, 0),
	SC_SET(SW_HOOK, 0),
	SC_END,
};

// 向きごとの一歩: 上位16ビットがX、下位16ビットがZ (ピクセル)
#define DIRECTION_STEP(x, z) ((u32)(x) << 16 | (u16)(z))

u32 KorosseoKawa_DirectionSteps[] = {
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

struct SceneEntrance KorosseoKawa_SceneTableA[] = {
	{ 0, CONDITION_ALWAYS, 920, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 312, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 KorosseoKawa_SceneTableB[] = {
	SCENE_EXITS(143),
	SCENE_EXIT(10, 143, 1),
	SCENE_EXIT(11, 140, 12),
	SCENE_EXIT(4, 140, 21),
	SCENE_EXIT(5, 138, 98),
	SCENE_EXITS_END,
};

struct ScenePlacement KorosseoKawa_SceneTableC[] = {
	{ 0x0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(408), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(456), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe5, CONDITION_ALWAYS, 7,
		PIXELS(601), 0, PIXELS(146), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(552), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(664), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(760), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(944), 0, PIXELS(216), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(1048), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(1016), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(600), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(824), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1080), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1304), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1304), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

s32 KorosseoKawa_Countdown = 0;

struct SceneEvent KorosseoKawa_SceneTableD[] = {
	{ 0x202, 90, CONDITION_ALWAYS, EVENT_SCRIPT(EV90_SCRIPT) },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunTwoCallSequence) },
	{ 0x202, 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunTwoCallSequence) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 12, 0x302, EVENT_SCRIPT(EV12_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TOUCH) },
	{ EVENT_TOUCH, 50, 0x210, EVENT_SCRIPT(EV50_TOUCH) },
	{ SEARCH(SEARCH_CHEST), 100, 0x308, FIND_ITEM(ITEM_NUT) },
	{ SEARCH(SEARCH_CHEST), 101, 0x309, FIND_ITEM(0xee) },
	{ 0x8413, 103, 0x30a, FIND_ITEM(ITEM_NUT) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectsForActorsNineAndTen) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectsForActorsNineAndTen) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectsForActorsNineAndTen) },
	{ PSYNERGY_ON_ACTOR(0xc), 15, 0x303, EVENT_SCRIPT(EV15_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x21), 13, 0x301, EVENT_SCRIPT(EV13_PSYNERGY) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(RunPartyCountInteraction) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_TALK) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EV23_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKorosseoKawaThisYearsFinalsAreIncredibleThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgKorosseoKawaMovingSuchAHugeRockMust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgKorosseoKawaTheresOnlyOneFinalAndThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgKorosseoKawaDidYouSeeHisMovesOut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgKorosseoKawaHowManyWarriorsHaveSwallowedTheir) },
	{ EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_RAISED) },
	{ SCENE_EVENTS_END },
};

s16 Korosseo_PortraitSlot = -1;

s16 KorosseoKawa_RoundSpans[] = {
#if !EDITION_INTERNATIONAL
	0x4000, -188, 2048, 0x1000, 256, 1, 0x2000, 16,
#else
	0x4000, -188, 2048, 0x1000, 384, 1, 0x2000, 16,
#endif
	1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
	0x3000, 2048, 20, 0x7fff, 60, -1,
};

s16 KorosseoKawa_ModeRecordFour[] = {
#if defined(TBS_EDITION_FR)
	0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 384,
	6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 512,
	6, 0x7fff, 6, 0x1000, 384, 6, 0x7fff, 6,
	0x1000, 512, 6, 0x7fff, 6, 0x1000, 384, 6,
	0x7fff, 6, 0x2000, 0, 30, 0x1000, 512, 6,
	0x7fff, 6, 0x1000, 384, 6, 0x7fff, 6, 0x1000,
	512, 6, 0x7fff, 6, 0x1000, 384, 6, 0x7fff,
	6, 0x1000, 512, 6, 0x7fff, 6, -1,
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
	0x1000, 384, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 384,
	6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6,
	0x1000, 384, 6, 0x7fff, 6, 0x1000, 256, 6,
	0x7fff, 6, 0x2000, 0, 30, 0x1000, 384, 6,
	0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6, 0x1000,
	384, 6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff,
	6, 0x1000, 384, 6, 0x7fff, 6, -1,
#else
	0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 384,
	6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6,
	0x1000, 384, 6, 0x7fff, 6, 0x1000, 256, 6,
	0x7fff, 6, 0x2000, 0, 30, 0x1000, 384, 6,
	0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6, 0x1000,
	384, 6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff,
	6, 0x1000, 384, 6, 0x7fff, 6, -1,
#endif
};

s16 KorosseoKawa_SpanA[] = {
	0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
	6, 0x7fff, 6, 0x1000, 272, 6, 0x7fff, 6,
	0x1000, 400, 6, 0x7fff, 6, 0x1000, 288, 6,
	0x7fff, 6, 0x2000, 0, 30, 0x1000, 416, 6,
	0x7fff, 6, 0x1000, 304, 6, 0x7fff, 6, 0x1000,
	432, 6, 0x7fff, 6, 0x1000, 320, 6, 0x7fff,
	6, 0x1000, 448, 6, 0x7fff, 6, -1,
};

s16 Korosseo_MarkerSlot = -1;

s32 Korosseo_RivalFinishScript[] = {
	SC_ARG(2),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(6),
	SC_CALL(SceneActor_PlaceLinkedActorAbove),
	SC_WAIT(3),
	SC_ARG(10),
	SC_WAIT(150),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ARG(1),
	SC_END,
};

s32 KorosseoKawa_ApproachScript[] = {
	SC_ARG(2),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(6),
	SC_CALL(SceneActor_PlaceLinkedActorAbove),
	SC_WAIT(3),
	SC_ARG(28),
	SC_WAIT(150),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ARG(1),
	SC_END,
};
