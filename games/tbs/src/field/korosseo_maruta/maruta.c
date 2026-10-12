#include "game_state.h"
#include "io_write_queue.h"
#include "resource_ids.h"
#include "scene_ids.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/field/korosseo_maruta/maruta.h"
#include "unit/field/common/korosseo/solo_competitor.h"
#include "unit/field/common/korosseo/solo_round.h"
#include "unit/field/common/korosseo/compete.h"
#include "unit/field/common/korosseo/item_icon.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object3.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object7.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field2.h"
#include "unit/field/common/script/field3.h"
#include "unit/field/common/script/field4.h"
#include "unit/field/common/script/field5.h"
#include "unit/field/common/script/field6.h"
#include "unit/field/common/script/field7.h"
#include "unit/field/common/script/field8.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "unit/game/character/chara.h"
#include "unit/game/character/chara2.h"
#include "unit/game/character/chara3.h"
#include "unit/game/character/chara4.h"
#include "unit/game/character/chara5.h"
#include "unit/game/character/chara6.h"
#include "unit/game/character/chara7.h"
#include "unit/game/character/chara8.h"
#include "unit/graphics/window/window.h"
#include "unit/graphics/window/window2.h"
#include "unit/graphics/window/window3.h"
#include "unit/graphics/window/window4.h"
#include "unit/graphics/window/window5.h"
#include "unit/graphics/window/window6.h"
#include "unit/graphics/window/window7.h"
#include "unit/graphics/window/window8.h"
#include "unit/graphics/window/window9.h"
#include "unit/graphics/window/window10.h"
#include "unit/graphics/window/window11.h"
#include "unit/graphics/window/window12.h"
#include "unit/menu/item/menu.h"
#include "unit/menu/item/menu2.h"
#include "unit/menu/item/menu3.h"
#include "unit/menu/item/menu4.h"
#include "unit/menu/item/menu5.h"
#include "unit/menu/item/menu6.h"
#include "unit/menu/item/menu7.h"
#include "unit/menu/item/menu8.h"
#include "unit/sound/sound.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime2.h"
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
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "colosso_log_rolling_stage.h"
#include "io_reg.h"
#include "keystate.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "call.h"
#include "field_service.h"
#include "flag_ids.h"
#include "text/msg_ids.h"
#include "battle_unit.h"
#include "resource.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fieldobj.h"
#include "heap_state.h"

// 前方宣言
static void FieldScene_RunMultiPhaseActorSequence(s32 a0);
static void Korosseo_RunGreetScene(s32 a0);
static void ColossoLogRollingStage_RestoreActorPositions(void);
static void ColossoLogRollingStage_MarkSceneProgress(void);
static void ColossoLogRollingStage_ClearSavedActorPositions(void);
static s32 ColossoLogRollingStage_RunStateInteraction(s32 actor, s32 flags);
static void ColossoLogRollingStage_InitializeStateInteraction(s32 actor, s32 flags);
static void ColossoLogRollingStage_RunScriptedTransition(s32 mode);
static void ColossoLogRollingStage_ResetActorMotion(s32 selector);
static void ColossoLogRollingStage_StartPaletteTaskFromState(u32 first_value, u32 second_value, u32 mode);
static void ColossoLogRollingStage_StopPaletteTask(void);
static void ColossoLogRollingStage_PositionScaledObject(s32 id, s32 x, s32 z);
static void ColossoLogRollingStage_NoopSceneHook(void);
static void ColossoLogRollingStage_WaitForBalanceState(void);
static s32 ColossoLogRollingStage_PositionActiveActor(s32 first_handle, s32 second_handle);
static void ColossoLogRollingStage_InitializeSceneControl(s32 resource);
static void ColossoLogRollingStage_PushStagedActor(void);
static void *SceneActor_FindOccupantAheadOfSubject(void);

typedef struct ActorSpriteRef
{
	u8	 filler00[0x28];
	s16 *entry;
} ActorSpriteRef;

typedef struct StageActor
{
	u8				filler00[8];
	s32				x;
	u8				filler0C[4];
	s32				z;
	u8				filler14[0x3c];
	ActorSpriteRef *sprite;
	u8				state;
} StageActor;

typedef struct ScaledStageObject
{
	u8	pad00[12];
	s32 y;
	u8	pad10[32];
	s32 scale_x;
	s32 scale_z;
	u8	pad38[35];
	u8	state;
} ScaledStageObject;

typedef struct SceneControl
{
	s16 enabled;
	s16 active;
	s16 scene_variant;
	s16 phase;
	s16 timer;
} SceneControl;

typedef struct MarutaSceneRecord
{
	u8	pad_00[6];
	u16 facing;
	s32 x;
	s32 y;
	s32 z;
	u8	pad_14[14];
	u8	state;
	u8	pad_23;
	s32 motion_24;
	u8	pad_28[4];
	s32 motion_2c;
	s32 rate_x;
	s32 rate_z;
	u8	pad_38[33];
	u8	flags;
} MarutaSceneRecord;

typedef struct Position3
{
	s32 x;
	s32 y;
	s32 z;
} Position3;

typedef struct StageObstacleActor
{
	u8	unk_00[6];
	u16 attributes;
	s32 x;
	s32 y;
	s32 z;
	u8	unk_14[15];
	u8	direction_and_kind;
	u8	unk_24[12];
	s32 move_rate_x;
	s32 move_rate_z;
	u8	unk_38[29];
	u8	state;
} StageObstacleActor;

typedef void (*SceneTaskEntry)(void);

typedef struct PrimaryStageObject
{
	u8	pad00[8];
	s32 x;
	u8	pad0C[4];
	s32 y;
	u8	pad14[4];
	s32 scale_x;
	s32 scale_z;
	u8	pad20[0x10];
	s32 move_rate_x;
	s32 move_rate_z;
} PrimaryStageObject;

typedef struct SecondaryStageObject
{
	u8	filler00[8];
	s32 x;
	u8	filler0C[4];
	s32 z;
	u8	filler14[0x1c];
	s32 move_rate_x;
	s32 move_rate_z;
	u8	filler38[0x1d];
	u8	state;
} SecondaryStageObject;

typedef struct StageMotionEffect
{
	u8	filler00[0x30];
	s32 move_rate_x;
	s32 move_rate_z;
	u8	filler38[0x1d];
	u8	state;
} StageMotionEffect;

typedef struct PartyInteractionRecord
{
	u8	padding_00[10];
	s16 x;
	u8	padding_0c[6];
	s16 y;
} PartyInteractionRecord;

static inline void InitializeActorZero(void)
{
	Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
	Engine_ActorSetSpeed(actorId, 0x10000, 0x8000);
}

extern s32 gColossoParticleCount;

// コロッセオ共通 (field/common/korosseo)

u8 *ColossoLogRollingStage_GetScriptData(void)
{
	return (u8 *)gKorosseoMarutaEntrances;
}

s32 ColossoLogRollingStage_GetMessageData(void)
{
	return 0;
}

u8 *ColossoLogRollingStage_GetActorData(void)
{
	return (u8 *)gKorosseoMarutaExits;
}

u8 *ColossoLogRollingStage_GetEffectData(void)
{
	return (u8 *)gKorosseoMarutaPlacements;
}

static void ColossoLogRollingStage_SceneTask(void)
{
	struct EventWork  *work = gEventWork;
	struct FieldActor *actor = Object_GetById(gGameState.selected_actor);
	s32				   row = actor->z.fixed >> 20;
	s32				   i;
	s32				   kind;

	if (gColossoSceneTaskState == 0)
	{
		gColossoSceneTaskStatus = (gColossoSceneTaskStatus + 1) & 3;
		for (i = 18; i <= 22; i++)
		{
			kind = KorosseoMaruta_LogPattern[gColossoSceneTaskStatus][i - 18];
			Engine_ActorSetAnimation(i, kind);
			Engine_ActorSetAnimation(i + 5, kind + 8);
			Engine_MapCopyCellAttributes(32, 11, 1, 2, (i - 18) * 2 + 33, 11);
			if (kind != 7)
			{
				Engine_MapCopyCellAttributes(74, 12, 1, 1, (i - 18) * 2 + 33, 11);
			}
		}
		Engine_ActorSetAnimation(28, KorosseoMaruta_LogPattern[gColossoSceneTaskStatus][5]);
	}
	else
	{
		for (i = 18; i <= 22; i++)
		{
			kind = KorosseoMaruta_LogPattern[gColossoSceneTaskStatus][i - 18];
			if ((u32)(actor->x.fixed - (i << 21) + 0x31ffff) <= 0x13fffe)
			{
				if (row == 11 && kind == 4)
					work->raised_trigger = kind;
				if (row == 12 && kind == 5)
					work->raised_trigger = kind;
			}
		}
	}
	if ((u32)++gColossoSceneTaskState > 17)
	{
		gColossoSceneTaskState = 0;
	}
}

// 別ファイルの関数: 呼び出し側は宣言なし (暗黙の int) で呼んでいる

static void ColossoLogRollingStage_ResetAndRunSceneTask(void)
{
	SceneTaskEntry entry;

	gColossoSceneTaskState = 0;
	entry = ColossoLogRollingStage_SceneTask;
	gColossoSceneTaskStatus = 0;
	Engine_TaskRemoveCallback(entry);
	entry();
}

static void ColossoLogRollingStage_StartSceneTask(void)
{
	Engine_TaskAddCallback(ColossoLogRollingStage_SceneTask, 0xC80);
}

static void ColossoLogRollingStage_WaitForSceneTask(void)
{
	s32 polls;

	Engine_TaskWait(10);

	polls = 0;
	while (gColossoSceneTaskStatus != 3 || gColossoSceneTaskState != 1)
	{
		Engine_TaskWait(1);
		polls++;
		if (polls > 119)
		{
			return;
		}
	}
}

static void EV11_TOUCH(void)
{
	u8				   *workspace = (u8 *)gEventWork;
	s16				   *table = (s16 *)&gGameState;
	s32					id = ((struct GameState *)table)->selected_actor;
	StageObstacleActor *subject =
		(StageObstacleActor *)((struct EventWork *)workspace)->view_center;
	StageObstacleActor *actor = Object_GetById(id);
	s32					z = ((struct FieldActor *)actor)->z.part.pixel;

	if ((u32)(z - 183) <= 3)
	{
		subject->x += -0xcccc;
		actor->x += -0xcccc;
	}
}

static void EV29_PSYNERGY(void)
{
	Engine_GameFlagSet(0x360);
	{
		s32 width = 49;
		s32 height = 61;

		Engine_MapCopyCellAttributes(47, 61, 1, 4, width, height);
	}
}

static void EV09_SCRIPT(void)
{
	PrimaryStageObject *object;

	object = Object_GetById(9);
	object->scale_x = 0x10000;
	object->scale_z = 0x10000;

	object = Object_GetById(11);
	object->move_rate_z = 0x6666;
	object->move_rate_x = 0xCCCC;
	Engine_ObjectSetPosition(object, object->x, 0x200000, object->y);

	object = Object_GetById(10);
	object->move_rate_z = 0x6666;
	object->move_rate_x = 0xCCCC;
	Engine_ObjectSetPosition(object, object->x, 0x40000, object->y);

	Engine_GameFlagSet(0x362);
	Engine_MapCopyCellAttributes(15, 12, 1, 1, 13, 12);
	Engine_MapCopyCellAttributes(14, 12, 1, 1, 9, 12);
}

static void EV15_TOUCH(void)
{

	s32				   actor;
	s32				   lowered;
	struct FieldActor *log;
	struct FieldActor *rider;
	u8				  *flags;

	actor = gGameState.selected_actor;
	lowered = GameFlag_IsSet(0x362);
	if (lowered == 0)
	{
		log = Object_GetById(10);
		if (log != NULL)
		{
			Actor_SetDestination(actor, log->x.part.pixel, log->z.part.pixel);
		}
		Engine_ActorWaitForMove(actor);
		log = Object_GetById(11);
		log->motion_flags = lowered;
		log->acceleration = 0x6666;
		log->speed = 0xcccc;
		Engine_ObjectSetPosition(log, log->x.fixed, 0x200000, log->z.fixed);
		log = Object_GetById(10);
		log->motion_flags = lowered;
		log->acceleration = 0x6666;
		log->speed = 0xcccc;
		FieldObject_SetPosition(log, log->x.fixed, 0x40000, log->z.fixed);
		rider = Object_GetById(actor);
		flags = &rider->motion_flags;
		*flags = lowered;
		rider->acceleration = 0x6666;
		rider->speed = 0xcccc;
		Engine_ObjectSetPosition(rider, rider->x.fixed, 0x40000, rider->z.fixed);
		Engine_ActorSetSpriteFlags(rider, 1);
		Engine_ActorWaitForMove(actor);
		Map_CopyCellAttributes(0, 24, 1, 1, 9, 12);
		Engine_TaskWait(2);
		Engine_ActorSetSpriteFlags(rider, 1);
		*flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		rider->terrain_height = rider->y.fixed;
		Engine_GameFlagSet(0x367);
	}
}

static void EV16_TOUCH(void)
{

	s16					 *table;
	SecondaryStageObject *object;

	table = (s16 *)&gGameState;

	Engine_ActorSetAnimation(*(s32 *)&table[250], 1);

	object = Object_GetById(11);
	object->state = 0;
	object->move_rate_z = 0x6666;
	object->move_rate_x = 0xcccc;
	Engine_ObjectSetPosition(object, object->x, 0x40000, object->z);

	object = Object_GetById(10);
	object->state = 0;
	object->move_rate_z = 0x6666;
	object->move_rate_x = 0xcccc;
	Engine_ObjectSetPosition(object, object->x, 0x200000, object->z);

	Engine_ActorWaitForMove(10);
	{
		s32 stack_first = 9;
		s32 stack_second = 12;
		Engine_MapCopyCellAttributes(0, 25, 1, 1, stack_first, stack_second);
	}
	Engine_TaskWait(2);
	Engine_GameFlagClear(0x367);
}

static void EV12_PSYNERGY(void)
{

	struct FieldActor *probe;
	struct FieldActor *log;
	struct FieldActor *upper;
	struct FieldActor *lower;
	s32				   x;
	s32				   z;

	probe = Object_GetById(12);
	x = probe->x.fixed >> 20;
	if (x == 9)
	{
		z = probe->z.fixed >> 20;
		if (z == 12)
		{
			log = Object_GetById(12);
			Engine_ActorSetSpriteFlags(log, 0);
			log->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
			log->motion_flags = 0;
			log->acceleration = 0x6666;
			log->speed = 0xcccc;
			FieldObject_SetPosition(log, log->x.fixed, 0x40000, log->z.fixed);
			upper = Object_GetById(11);
			upper->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
			upper->acceleration = 0x6666;
			upper->speed = 0xcccc;
			Engine_ObjectSetPosition(upper, upper->x.fixed, 0x200000, upper->z.fixed);
			lower = Object_GetById(10);
			lower->acceleration = 0x6666;
			lower->speed = 0xcccc;
			Engine_ObjectSetPosition(lower, lower->x.fixed, 0x40000, lower->z.fixed);
			Engine_GameFlagSet(0x368);
			Engine_MapCopyCellAttributes(15, 12, 1, 1, 13, z);
			Engine_MapCopyCellAttributes(1, 25, 1, 1, x, z);
		}
	}
}

static void EV20_SCRIPT(void)
{
	ColossoLogRollingStage_PushStagedActor();
	EV12_PSYNERGY();
}

static void EV13_PSYNERGY(void)
{

	StageObstacleActor *actor;
	s32					x;

	actor = Object_GetById(13);
	x = actor->x >> 20;
	GameFlag_SetByte(880, x);
	Engine_MapCopyCellAttributes(18, 10, 3, 1, 18, 11);
	Engine_MapCopyCellAttributes(17, 11, 1, 1, x, 11);
}

static void EV14_PSYNERGY(void)
{
}

static void EV22_SCRIPT(void)
{
	ColossoLogRollingStage_PushStagedActor();
}

static void ColossoLogRollingStage_ActivateClearObstacleActors(void)
{

	StageObstacleActor *actor;
	s32					slot;
	s32					x;
	s32					z;
	s32					x2;
	s32					z2;

	for (slot = 15; slot <= 17; slot++)
	{
		actor = Object_GetById(slot);
		if (Map_GetTerrainHeight(0, actor->x, actor->z) == 0)
		{
			actor->direction_and_kind = 2;
			actor->state = 0;
			x = actor->x >> 20;
			z = actor->z >> 20;
			Engine_MapCopyCellAttributes(83, 13, 1, 1, x, z);
			x2 = actor->x >> 20;
			z2 = actor->z >> 20;
			Engine_MapCopyCellAttributes(83, 13, 1, 1, x2, z2 + 52);
			Engine_GameFlagSet(slot + 517);
		}
	}
}

static void ColossoLogRollingStage_ShowActorPositionMessage(void)
{

	StageObstacleActor *actor;
	s16				   *table;
	s32					x;
	s32					z;
	s32					message_id;

	table = (s16 *)&gGameState;
	actor = Object_GetById(*(s32 *)&table[250]);
	x = actor->x >> 20;
	message_id = 23;
	z = actor->z >> 20;
	if (x == 81 && z == 12)
	{
		if ((actor->attributes & 0xE000) == 0x4000)
		{
			message_id = 253;
		}
		SetMapCellCollision(0, x << 20, z << 20, message_id);
	}
}

static s32 ColossoLogRollingStage_CheckObstacleDestination(s32 x, s32 z)
{

	StageObstacleActor *actor;

	if (GetMapCellCollision(0, x, z) == 255)
	{
		return -2;
	}
	actor = Object_GetById(15);
	x >>= 20;
	z >>= 20;
	if (actor->x >> 20 == x && actor->z >> 20 == z)
	{
		return -1;
	}
	actor = Object_GetById(16);
	if (actor->x >> 20 == x && actor->z >> 20 == z)
	{
		return -1;
	}
	actor = Object_GetById(17);
	if (actor->x >> 20 == x && actor->z >> 20 == z)
	{
		return -1;
	}
	return 0;
}

static s32 ColossoLogRollingStage_CheckPathClearance(s32 x, s32 y)
{
	if (ColossoLogRollingStage_CheckObstacleDestination(x, y - 0x180000) != 0
		|| ColossoLogRollingStage_CheckObstacleDestination(x, y - 0x80000) != 0
		|| ColossoLogRollingStage_CheckObstacleDestination(x, y + 0x80000) != 0
		|| ColossoLogRollingStage_CheckObstacleDestination(x, y + 0x180000) != 0)
	{
		return -1;
	}
	return 0;
}

static void EV23_SCRIPT(void)
{

	struct FieldActor *rider;
	struct FieldActor *log;
	struct EventWork  *work;
	s32				   angle;
	s32				   actor;
	s32				   x;
	s32				   z;
	s32				   steps;
	s32				   next_x;
	s32				   column;
	s32				   acceleration;
	s32				   mode;
	s32				   step;
	s32				   ahead[3];

	rider = Object_GetById(gGameState.selected_actor);
	log = Object_GetById(31);
	steps = 0;
	angle = gColossoEarlySequenceData[rider->facing >> 13];
	actor = gGameState.selected_actor;
	ahead[0] = (rider->x.fixed & -0x100000) + 0x80000;
	ahead[1] = rider->y.fixed;
	ahead[2] = (rider->z.fixed & -0x100000) + 0x80000;
	Vector_AddPolarOffset(0x100000, angle, ahead);
	x = log->x.fixed;
	z = log->z.fixed;
	if ((ahead[0] - x >= 0 ? ahead[0] - x : x - ahead[0]) > 0x80000
		|| (ahead[2] - z >= 0 ? ahead[2] - z : z - ahead[2]) > 0x200000)
	{
		goto far;
	}
	if (gKeysHeld & KEY_LEFT)
	{
		mode = 2;
		step = -8;
		for (;;)
		{
			next_x = x - 0x100000;
			if (ColossoLogRollingStage_CheckPathClearance(next_x, z) != 0)
				goto moved;
			steps++;
			x = next_x;
		}
	}
	if (!(gKeysHeld & KEY_RIGHT))
	{
		return;
	}
	mode = 3;
	step = 8;
	for (;;)
	{
		next_x = x + 0x100000;
		if (ColossoLogRollingStage_CheckPathClearance(next_x, z) != 0)
			goto moved;
		steps++;
		x = next_x;
	}
moved:
	if (steps == 0)
	{
		return;
	}
	Map_CopyCellAttributes(74, 8, 1, 4, log->x.fixed >> 20, 9);
	Map_CopyCellAttributes(120, 60, 8, 5, 74, 60);
	Engine_EventBegin();
	Engine_ActorSetAnimation(actor, 8);
	Engine_EventWait(6);
	log->speed = 0x8000;
	acceleration = 0x3333;
	log->acceleration = acceleration;
	Object_SetMode(log, mode);
	Engine_ObjectSetPosition(log, x, 0, z);
	Engine_EventWait(6);
	Engine_ActorSetAnimation(actor, 2);
	work = Runtime_AllocateBlock(27, 0xccc);
	ObjectDispatch_InitFromTable4WithArgument(work->view_center, log);
	Actor_SetSpeed(actor, 0x8000, acceleration);
	Event_PlayCue(239);
	Object_SetMode(rider, 2);
	Engine_ObjectSetPosition(rider, ((steps * step) << 16) + rider->x.fixed, 0, rider->z.fixed);
	Object_CommitPosition(rider);
	Object_SetMode(rider, 1);
	Object_CommitPosition(log);
	if (x >= 0x5300000)
	{
		GameFlag_Set(0x369);
		Engine_ActorSetAnimation(31, 3);
		Actor_SetDestinationOffset(31, 18, 6);
		Engine_EventWait(30);
		Object_SetMode(log, 8);
		Object_CommitPosition(log);
		log->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
		column = 84;
		Map_CopyCellAttributes(86, 10, 1, 2, column, 10);
		Map_CopyCellAttributes(86, 9, 1, 1, column, 12);
		Event_PlayCue(0x120);
		Event_PlayCue(240);
	}
	else
	{
		Object_SetMode(log, 1);
		Event_PlayCue(0x120);
		Event_PlayCue(213);
		column = x >> 20;
		Map_CopyCellAttributes(85, 9, 1, 4, column, 9);
		Map_CopyCellAttributes(85, 9, 1, 4, column, 61);
	}
	Engine_EventWait(15);
	Engine_EventEnd();
	return;
far:
	ColossoLogRollingStage_PushStagedActor();
	ColossoLogRollingStage_ActivateClearObstacleActors();
}

static s32 ColossoLogRollingStage_SetSceneEventValues(void)
{

	Map_SetLayerEntryFlag(1);
	Map_ClearLayerEntryFlag(2);
	Engine_AudioPlayCue(288);
	Engine_AudioPlayCue(217);
	return 0;
}

static void EV30_SCRIPT(void)
{
	StageMotionEffect *effect;
	s32				   move_rate;

	effect = Object_GetById(30);
	effect->state = 0;
	move_rate = 0x19999;
	effect->move_rate_z = move_rate;
	effect->move_rate_x = move_rate;
	Object_SetMode(effect, 2);
	Engine_ObjectSetScript(effect, gColossoSceneEventEffect);
	Engine_GameFlagSet(0x363);
}

static void EV28_PSYNERGY(void)
{

	BattleEffect_PauseObject(28);
	Engine_GameFlagSet(0x361);
	Engine_TaskWait(10);
	while (gColossoSceneTaskStatus != 1 && gColossoSceneTaskStatus != 3)
		Engine_TaskWait(1);
	Engine_TaskWait(1);
	Engine_TaskRemoveCallback(ColossoLogRollingStage_SceneTask);
}

static void EV04_RAISED(void)
{

	StageObstacleActor *actor;
	s16				   *table;
	s32				   *slot;
	s32					z;

	table = (s16 *)&gGameState;
	slot = (s32 *)&table[250];
	actor = Object_GetById(*slot);
	actor->move_rate_z = 0x10000;
	actor->move_rate_x = 0x20000;
	Engine_ActorSetAttachedEffect(*slot, 258);
	Object_SetMode(actor, 5);
	z = actor->z & 0xFFF00000;
	Engine_ObjectSetPosition(actor, actor->x, actor->y, z + 0x180000);
	Object_CommitPosition(actor);
}

static void EV05_RAISED(void)
{

	StageObstacleActor *actor;
	s16				   *table;
	s32				   *slot;
	s32					z;

	table = (s16 *)&gGameState;
	slot = (s32 *)&table[250];
	actor = Object_GetById(*slot);
	if (actor->x > 0x2980000)
	{
		actor->x = 0x2980000;
	}
	actor->move_rate_z = 0x10000;
	actor->move_rate_x = 0x20000;
	Object_SetMode(actor, 5);
	z = actor->z & 0xFFF00000;
	Engine_ObjectSetPosition(actor, actor->x, actor->y, z + 0xC0000);
	Object_CommitPosition(actor);
	Engine_ActorSetAttachedEffect(*slot, 258);
	BattleFx_RunRisingObjectSequence(*slot, 6, 0);
}

static void ColossoLogRollingStage_NoopSceneEventHook(void)
{
}

static void KorosseoMaruta_RunCompetitorTalk(s32 a0)
{
	struct EventWork *work;
	s32				  owner;
	s32				  lines;
	s32				  entrance;

	work = gEventWork;
	owner = gGameState.selected_actor;
	entrance = gGameState.entrance;
	if (entrance == 2)
	{
		Engine_EventBegin();
		lines = a0 * 3;
		Engine_EventSetMessage(lines + (s32)MsgKorosseoDidntThinkBattles);
		Engine_EventOpenMessage(a0, 0);
		if (Engine_EventChooseYesNo(owner, 0) == 0)
		{
			Engine_EventSetMessage(lines + (s32)MsgKorosseoDidntThinkBattles + 1);
			Engine_EventShowMessage(a0, 0);
			work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
			work->transition_frames = 15;
			Engine_EventCloseScreen();
			Engine_EventWaitForScreen();
			Korosseo_SelectSoloCompetitor(a0);
			Engine_EventOpenScreen();
			Engine_EventWaitForScreen();
		}
		else
		{
			Engine_EventSetMessage(lines + (s32)MsgKorosseoDidntThinkBattles + 2);
			Engine_EventShowMessage(a0, 0);
		}
		Engine_EventEnd();
	}
}

static void EV50_TOUCH(void)
{
	s32 place;
	s32 i;
	s32 last;

	/* FAKEMATCH: the last place is set at the top as a forced temporary, so the weight stays 4 - place + 1 instead of folding to 5 - place. */
	last = 4;
	ColossoLogRollingStage_NoopSceneHook();
	Engine_EventBegin();
	place = ColossoLogRollingStage_PositionActiveActor(3, 17);
	ColossoLogRollingStage_WaitForBalanceState();
	for (i = 0; i < 10; i++)
	{
		Object_RefreshSelectorById(8);
	}
	Actor_SetSpeed(8, 0x10000, 0x8000);
	Actor_WalkTo(8, 0x5f8, 192);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x5d8, 192);
	Engine_ActorSetAnimation(8, 1);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 8, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimation(8, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
	Actor_SetSpeed(8, 0x20000, 0x10000);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x5e0, 192);
	Actor_WalkToAndWait(8, 0x5f0, 192);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
	Engine_ActorSetAnimation(8, 9);
	Engine_EventWait(10);
	BattleFx_SetWeightedResult(72, last - place + 1);
	Battle_SetStart(3);
	Party_SetFields1ceAnd1d0((s32)&SceneId_KorosseoMaruta, 4);
	Event_SetDefeatDestination((s32)&SceneId_KorosseoMaruta, 5);
	Engine_GameFlagSet(0x11a);
}

static void EV90_SCRIPT(void)
{
	if (SceneActor_FindOccupantAheadOfSubject() == 0)
	{
		Engine_LeaderCheckAhead();
	}
}

static void EV91_SCRIPT(void)
{
	if (SceneActor_FindOccupantAheadOfSubject() == 0)
	{
		Engine_LeaderCheckAhead();
	}
	else
	{
		EV20_SCRIPT();
	}
}

s32 ColossoLogRollingStage_GetSceneEventState(void)
{
	return (s32)gKorosseoMarutaEvents;
}

s32 StageSetup_BuildAndDispatch(void)
{
	struct FieldActor *actor;
	s32				   i;
	s32				   height;
	s32				   column;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_GameFlagSet(324);
	Engine_TaskAddCallback(ColossoLogRollingStage_ShowActorPositionMessage, 3200);
	Map_CopyCellAttributes(74, 60, 8, 6, 120, 60);

	Engine_ActorSetSpriteFlags(Object_GetById(9), 0);

	actor = Object_GetById(10);
	Engine_ActorSetSpriteFlags(actor, 0);
	actor->motion_flags = 0;
	actor->y.fixed = 32 << 16;

	actor = Object_GetById(11);
	Engine_ActorSetSpriteFlags(actor, 0);
	actor->motion_flags = 0;
	actor->y.fixed = 4 << 16;

	if (Engine_GameFlagIsSet(866) != 0)
	{
		Engine_ActorSetAnimation(9, 5);
		actor = Object_GetById(10);
		actor->y.fixed = 4 << 16;
		actor = Object_GetById(11);
		actor->y.fixed = 32 << 16;
		Map_CopyCellAttributes(15, 12, 1, 1, 13, 12);
	}
	else
	{
		actor = Object_GetById(9);
		actor->scale_x = 0x18000;
		actor->scale_y = 0x18000;
		if (Engine_GameFlagIsSet(871) != 0)
		{
			Map_CopyCellAttributes(0, 24, 1, 1, 9, 12);
		}
		else
		{
			Map_CopyCellAttributes(0, 25, 1, 1, 9, 12);
		}
	}

	if (Engine_GameFlagIsSet(872) != 0)
	{
		Engine_MapCopyCellAttributes(15, 12, 1, 1, 13, 12);
		Engine_MapCopyCellAttributes(1, 25, 1, 1, 9, 12);
		actor = Object_GetById(12);
		Engine_ActorSetSpriteFlags(actor, 0);
		actor->motion_flags = 0;
		actor->y.fixed = 2 << 16;
		actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
		actor = Object_GetById(10);
		actor->y.fixed = 4 << 16;
		actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
		actor = Object_GetById(11);
		actor->y.fixed = 32 << 16;
	}

	column = GameFlag_GetByte(880);
	if (column == 0)
	{
		column = 19;
	}
	actor = Object_GetById(13);
	actor->x.fixed = (column << 20) + (8 << 16);
	actor->motion_flags = 0;
	actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
	Map_CopyCellAttributes(18, 10, 3, 1, 18, 11);
	Engine_MapCopyCellAttributes(17, 11, 1, 1, column, 11);

	for (i = 15; i <= 17; i++)
	{
		actor = Object_GetById(i);
		height = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
		if (actor->y.fixed == 0 && height == 0)
		{
			actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
			actor->motion_flags = height;
			Map_CopyCellAttributes(83, 13, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
			Map_CopyCellAttributes(83, 13, 1, 1, actor->x.fixed >> 20, (actor->z.fixed >> 20) + 52);
		}
	}

	if (Engine_GameFlagIsSet(865) != 0)
	{
		for (i = 18; i <= 22; i++)
		{
			actor = Object_GetById(i);
			actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
			Object_SetMode(actor, 2);
			actor = Object_GetById(i + 5);
			actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
			actor->motion_flags = 0;
			actor->y.fixed = 32 << 16;
			Object_SetMode(actor, 10);
			Map_CopyCellAttributes(74, 12, 1, 1, i * 2 - 3, 11);
		}
		Engine_ActorSetAnimation(28, 10);
		BattleEffect_PauseObject(28);
	}
	else
	{
		for (i = 18; i <= 22; i++)
		{
			actor = Object_GetById(i);
			actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
			actor = Object_GetById(i + 5);
			actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
			actor->motion_flags = 0;
			actor->y.fixed = 32 << 16;
		}
		Engine_TaskAddCallback(ColossoLogRollingStage_SceneTask, 3200);
	}

	if (Engine_GameFlagIsSet(864) != 0)
	{
		Engine_ActorSetAnimation(29, 4);
		Map_CopyCellAttributes(47, 61, 1, 4, 49, 61);
	}

	if (Engine_GameFlagIsSet(867) != 0)
	{
		Map_SetLayerEntryFlag(1);
		actor = Object_GetById(30);
		actor->motion_flags = 0;
		actor->x.fixed = 1130 << 16;
		actor->z.fixed = 184 << 16;
		Engine_ActorSetSpriteFlags(actor, 0);
		Object_SetMode(actor, 3);
		Engine_ObjectSetScript(actor, KorosseoMaruta_Record30Script);
	}
	else
	{
		Map_SetLayerEntryFlag(2);
	}

	if (Engine_GameFlagIsSet(873) != 0)
	{
		actor = Object_GetById(31);
		Object_SetMode(actor, 8);
		actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
		Engine_MapCopyCellAttributes(86, 10, 1, 2, 84, 10);
		Engine_MapCopyCellAttributes(86, 9, 1, 1, 84, 12);
	}
	else
	{
		actor = Object_GetById(31);
		Map_CopyCellAttributes(85, 9, 1, 4, actor->x.fixed >> 20, 9);
		Map_CopyCellAttributes(85, 9, 1, 4, actor->x.fixed >> 20, 61);
	}

	actor = Object_GetById(9);
	actor->motion_flags = 0;
	actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
	actor = Object_GetById(10);
	actor->motion_flags = 0;
	actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
	actor = Object_GetById(11);
	actor->motion_flags = 0;
	actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
	Engine_ActorSetAnimation(8, 9);
	gGameState.movement_mode = 0;
	Korosseo_ShowItemIcon(39, 3);
	Korosseo_ShowItemIcon(40, 17);
	Engine_ActorSetChildValue(8, 2);

	switch (gGameState.entrance)
	{
	case 1:
		ColossoLogRollingStage_SetupSceneDescriptor(0, 8, 6, 1512 << 16, 192 << 16, 39, 40);
		Map_CopyCellsLayered(127, 0, 1, 2, 5, 2);
		Engine_ActorDestroy(32);
		Engine_ActorDestroy(33);
		Engine_ActorDestroy(34);
		Engine_ActorDestroy(35);
		Engine_ActorDestroy(36);
		Engine_ActorDestroy(37);
		Engine_ActorDestroy(38);
		if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0)
		{
			Engine_AudioPlayCue(17);
			Korosseo_SelectSoloCompetitor(0);
			ColossoLogRollingStage_RestoreActorPositions();
			Object_LinkObjectAndSetCallback(1, 0);
			ColossoLogRollingStage_RunScriptedTransition(3);
		}
		Object_LinkObjectAndSetCallback(1, 0);
		Object_LinkObjectAndSetCallback(2, 0);
		Object_LinkObjectAndSetCallback(3, 0);
		ColossoLogRollingStage_InitializeSceneControl((s32)&ResourceId_RivalPathC);
		break;

	case 2:
		Engine_TaskAddCallback(ColossoLogRollingStage_MarkSceneProgress, 3200);
		Engine_ActorDestroy(39);
		Engine_ActorDestroy(40);
		if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0)
		{
			break;
		}
		ColossoLogRollingStage_RestoreActorPositions();
		Korosseo_SelectSoloCompetitor(1);
		ColossoLogRollingStage_RunScriptedTransition(0);
		break;

	case 3:
		if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0)
		{
			break;
		}
		Korosseo_RunGreetScene(32);
		ColossoLogRollingStage_ClearSavedActorPositions();
		break;

	case 4:
		FieldScene_RunMultiPhaseActorSequence(1);
		Engine_EventRequestExit(4);
		Engine_GameFlagSet(2384);
		Engine_GameFlagSet(2385);
		break;

	case 5:
		FieldScene_RunMultiPhaseActorSequence(-1);
		Engine_EventRequestExit(5);
		Engine_GameFlagSet(2384);
		break;
	}
	return 0;
}

static s32 ColossoLogRollingStage_AdvanceParticleMotion(SceneParticle *particle)
{
	particle->x += particle->velocity_x << 8;
	particle->y += particle->velocity_y << 8;
	particle->scale_x += 0x666;
	particle->scale_y += 0x666;
	particle->velocity_x += 5;
	particle->velocity_y -= 1;
	return 0;
}

static void ColossoLogRollingStage_SpawnPeriodicParticle(void)
{

	SceneParticle *particle;
	SceneParticle *source;
	s32			   x;
	s32			   y;
	s32			   kind;
	s32			   count;

	particle = Object_GetById(0);
	count = gColossoParticleCount + 1;
	kind = 41;
	x = particle->x;
	y = particle->y;
	gColossoParticleCount = count;
	switch (count % 180)
	{
	case 10:
		break;
	case 20:
		kind = 42;
		break;
	case 30:
		kind = 43;
		break;
	default:
		return;
	}
	particle = Object_GetById(kind);
	if (particle == 0)
	{
		return;
	}
	source = Object_GetById(0);
	if (source != 0)
	{
		Engine_ActorSetPosition(kind, source->x, source->z);
	}
	Engine_ActorSetSpriteFlags(Object_GetById(kind), 0);
	particle->state = 0;
	particle->scale_x = 0x6666;
	particle->scale_y = 0x6666;
	{
		s32 t = 0x40000;
		particle->x = x + t;
		t += y;
		particle->y = t;
		particle->anchor_y = t;
	}
	particle->velocity_x = 25;
	particle->velocity_y = 128;
	Engine_ActorEnableActionCallback(kind, gColossoParticleKinds);
}

static void FieldScene_RunMultiPhaseActorSequence(s32 a0)
{

	struct FieldActor *record;
	s32				   data_table_addr;
	s32				   callback_target;

	Engine_ActorDestroy(39);
	Engine_ActorDestroy(40);
	Owner_RefreshActiveRatios(1);
	Engine_AudioPlayCue(17);
	Engine_EventBegin();
	Actor_SetPosition(8, 0x6080000, 0xc00000);
	if (a0 < 0)
	{
		Engine_ActorSetAnimation(8, 10);
	}
	else
	{
		Engine_ActorSetAnimation(8, 8);
	}
	Engine_ActorEnableActionCallback(8, KorosseoMaruta_Actor8Action);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x5e00000, 0xc00000);
	record = Object_GetById(0);
	{
		u16 shown = 0;

		record->facing = shown;
	}
	Engine_ActorEnableActionCallback(0, KorosseoMaruta_LeaderActionA);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 35);
	Actor_SetSpeed(1, 0x10000, 0x8000);
	Actor_SetSpeed(IWAN, 0x10000, 0x8000);
	Actor_SetSpeed(MEARI, 0x10000, 0x8000);
	Actor_SetPosition(GERALD, 0x5b80000, 0xb80000);
	Actor_SetPosition(IWAN, 0x5b80000, 0xc80000);
	Actor_SetPosition(MEARI, 0x5a80000, 0xc00000);
	record = Object_GetById(1);
	{
		u16 shown = 0;

		record->facing = shown;
	}
	record = Object_GetById(2);
	{
		u16 shown = 0;

		record->facing = shown;
	}
	record = Object_GetById(3);
	{
		u16 shown = 0;

		record->facing = shown;
	}
	Engine_TaskWait(1);
	Engine_CameraFollowActor(0, 0);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	ColorBuffer_ApplyTarget(0x10001, 1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventSetMessage((s32)MsgKorosseoRobin);
	Engine_EventWait(60);
	data_table_addr = (s32)gColossoMultiPhaseData;
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, data_table_addr);
	record = Object_GetById(0);
	record->scale_x = 0x10000;
	record = Object_GetById(0);
	record->scale_y = 0x10000;
	Engine_ActorSetAnimationAndWait(0, 36);
	record = Object_GetById(0);
	record->x.fixed += 0x30000;
	Engine_EventWait(10);
	record = Object_GetById(0);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(0, KorosseoMaruta_LeaderActionB);
	Event_ShowMessage(GERALD, 0);
	Engine_EventWait(20);
	Actor_WalkToAndWait(1, 0x5e0, 176);
	Actor_FaceDirection(GERALD, 0x4000, 10);
	Engine_ActorShowEmote(1, 0x100, 20);
	Event_ShowMessage(IWAN, 0);
	Object_LinkObjectAndSetCallback(1, 2);
	Engine_EventWait(30);
	Actor_WalkToAndWait(IWAN, 0x5d0, 176);
	Actor_WalkTo(GERALD, 0x5f0, 184);
	Actor_WalkToAndWait(IWAN, 0x5e0, 176);
	Engine_ActorSetAnimation(GERALD, 1);
	Actor_FaceDirection(GERALD, 0x6000, 0);
	Actor_FaceDirection(IWAN, 0x4000, 10);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(10);
	Object_LinkObjectAndSetCallback(2, 1);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(1, 4);
	Engine_EventWait(30);
	Event_ShowMessage(MEARI, 0);
	Object_LinkObjectAndSetCallback(1, 3);
	Object_LinkObjectAndSetCallback(2, 3);
	Actor_WalkToAndWait(MEARI, 0x5d0, 184);
	Object_LinkObjectAndSetCallback(2, 0);
	Object_LinkObjectAndSetCallback(1, 0);
	Actor_ShowEmote(GERALD, 0x102, 60);
	Object_LinkObjectAndSetCallback(2, 1);
	Object_LinkObjectAndSetCallback(1, 2);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(MEARI, 2);
	Engine_EventWait(10);
	Object_LinkObjectAndSetCallback(2, 3);
	Object_LinkObjectAndSetCallback(1, 3);
	Engine_EventWait(20);
	Event_ShowMessage(MEARI, 0);
	Engine_ActorEnableActionCallback(0, KorosseoMaruta_LeaderActionC);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Object_LinkObjectAndSetCallback(1, 0);
	Engine_EventWait(20);
	Event_ShowMessage(GERALD, 0);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Object_LinkObjectAndSetCallback(2, 0);
	Engine_EventWait(20);
	Event_ShowMessage(IWAN, 0);
	Engine_ActorRunRepeatedMotion(MEARI, 2);
	Object_LinkObjectAndSetCallback(3, 0);
	Engine_EventWait(20);
	Event_ShowMessage(MEARI, 0);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, data_table_addr);
	Engine_EventWait(60);
	callback_target = (s32)ColossoLogRollingStage_SpawnPeriodicParticle;
	gColossoParticleCount = 9;
	Task_AddCallback((void (*)(void))callback_target, 0xc80);
	Engine_EventWait(5);
	Engine_TaskRemoveCallback(callback_target);
	Engine_EventWait(55);
	Actor_ShowEmote(GERALD, 0x101, 60);
	Task_AddCallback((void (*)(void))callback_target, 0xc80);
	Engine_EventWait(20);
	Engine_TaskRemoveCallback(callback_target);
	Engine_EventWait(40);
	Actor_ShowEmote(IWAN, 0x101, 60);
	gColossoParticleCount = 9;
	Task_AddCallback((void (*)(void))callback_target, 0xc80);
	Engine_EventWait(35);
	Engine_TaskRemoveCallback(callback_target);
	Engine_EventWait(25);
	Actor_ShowEmote(MEARI, 0x102, 60);
	gColossoParticleCount = 9;
	Task_AddCallback((void (*)(void))callback_target, 0xc80);
	Engine_EventWait(35);
	Engine_TaskRemoveCallback(callback_target);
	Engine_EventWait(25);
	Actor_ShowEmote(2, 0x102, 60);
	Object_LinkObjectAndSetCallback(3, 2);
	Object_LinkObjectAndSetCallback(2, 3);
	Engine_EventWait(60);
	Object_LinkObjectAndSetCallback(3, 0);
	Object_LinkObjectAndSetCallback(2, 0);
	gColossoParticleCount = 9;
	Task_AddCallback((void (*)(void))callback_target, 0xc80);
	Engine_EventWait(35);
	Engine_TaskRemoveCallback(callback_target);
	Engine_EventWait(25);
	Actor_ShowEmote(MEARI, 0x108, 60);
	Engine_ActorStartRepeatedMotion(GERALD, 3);
	Engine_ActorStartRepeatedMotion(2, 3);
	Engine_ActorRunRepeatedMotion(3, 3);
	Object_LinkObjectAndSetCallback(3, 2);
	Object_LinkObjectAndSetCallback(1, 2);
	gColossoParticleCount = 9;
	Task_AddCallback((void (*)(void))callback_target, 0xc80);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_EventWait(60);
	Actor_WalkTo(MEARI, 0x5b8, 200);
	Engine_EventWait(5);
	Actor_WalkTo(IWAN, 0x558, 184);
	Engine_EventWait(3);
	Actor_WalkToAndWait(GERALD, 0x5e8, 184);
	Actor_WalkTo(GERALD, 0x558, 184);
	Engine_ActorWaitForMove(MEARI);
	Engine_ActorSetAnimation(3, 1);
	Object_LinkObjectAndSetCallback(3, 0);
	Engine_EventWait(60);
	Actor_WalkToAndWait(MEARI, 0x598, 200);
	Actor_WalkTo(MEARI, 0x558, 184);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(30);
	Actor_SetPosition(GERALD, 0x5e80000, 0xb00000);
	Actor_SetPosition(IWAN, 0x5b80000, 0xc00000);
	Actor_SetPosition(MEARI, 0x6180000, 0xc80000);
	Graphics_EnableObjLayerAndCallbacks();
	ColorBuffer_ApplyTarget(0x10000, 2);
	Engine_ColorBufferInterpolate(1);
	Engine_EventSetMessage((s32)MsgKorosseoRobinFellAsleep);
	Event_ShowMessage(GERALD, 0);
	Event_ShowMessage(IWAN, 0);
	Event_ShowMessage(MEARI, 0);
	Engine_EventWait(60);
	Engine_EventEnd();
}

static void Korosseo_RunGreetScene(s32 a0)
{
	s32				   p10;
	s32				   p9;
	struct FieldActor *record;

	record = Object_GetById(a0);
	p9 = record->x.part.pixel;
	p10 = record->z.part.pixel;
	Engine_EventBegin();
	Actor_SetSpeed(a0, 0x10000, 0x8000);
	Actor_SetSpeed(0, 0x10000, 0x8000);
	Actor_SetSpeed(1, 0x10000, 0x8000);
	Actor_SetSpeed(2, 0x10000, 0x8000);
	Actor_SetSpeed(3, 0x10000, 0x8000);
	Engine_ActorSetPosition(0, (p9 << 16), ((p10 << 16) - 0x300000));
	Engine_ActorSetPosition(1, (p9 << 16) - 0x100000, (p10 << 16) - 0x280000);
	Engine_ActorSetPosition(2, (p9 << 16) + 0x100000, (p10 << 16) - 0x280000);
	Engine_ActorSetPosition(3, (p9 << 16), ((p10 << 16) - 0x200000));
	Engine_ActorSetPosition(a0, (p9 << 16), ((p10 << 16) - 0x500000));
	record = Object_GetById(0);
	record->facing = 0xc000;
	Engine_CameraFollowActor(0, 0);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventSetMessage((s32)MsgKorosseoSiteThirdFinals);
	Engine_ActorSetAnimationAndWait(a0, 3);
	Engine_EventShowMessage(a0, 0);
	Engine_ActorRunRepeatedMotion(a0, 2);
	Engine_EventShowMessage(a0, 0);
	Engine_ActorRunRepeatedMotion(a0, 2);
	Engine_EventShowMessage(a0, 0);
	Engine_ActorRunRepeatedMotion(a0, 2);
	Engine_EventShowMessage(a0, 0);
	Engine_ActorSetAnimation(3, 3);
	Engine_ActorSetAnimation(1, 3);
	Engine_ActorSetAnimation(2, 3);
	Engine_ActorSetAnimationAndWait(0, 3);
	Engine_EventWait(6);
	Engine_ActorSetAnimation(1, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Engine_ActorSetDestination(1, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorSetAnimation(2, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Engine_ActorSetDestination(2, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorSetAnimation(3, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Engine_ActorSetDestination(3, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWalkToAndWait(a0, (p9 - 16), (p10 - 64));
	Engine_ActorSetPosition(1, 0, 0);
	Engine_ActorSetPosition(2, 0, 0);
	Engine_ActorSetPosition(3, 0, 0);
	Engine_ActorWalkToAndWait(a0, (p9 - 16), (p10 - 16));
	Engine_ActorWalkToAndWait(a0, p9, p10);
	Engine_ActorFaceDirection(a0, 0xc000, 10);
	Engine_EventEnd();
}

static void EV33_TALK(s32 scene)
{
	struct FieldActor *pan1;
	struct FieldActor *pan2;
	struct FieldActor *back1;
	struct FieldActor *back2;
	s32				   state;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(scene);
		return;
	}
	Engine_EventBegin();
	state = ColossoLogRollingStage_RunStateInteraction(scene, 1);
	if (state == 0)
	{
		Engine_EventSetMessage((s32)MsgKorosseoStageCalledScales);
		Engine_CameraSetSpeed(3 << 16, 0x6000);
		Engine_CameraMoveTo(152 << 16, -1, 200 << 16, 1);
		Engine_CameraWaitForMove();
		Engine_EventWait(30);
		Engine_EventShowMessage(scene, 0);
		ColossoLogRollingStage_StartPaletteTask(104, 68, 0);
		Engine_EventWait(60);
		ColossoLogRollingStage_StartPaletteTaskFromState(168, 96, 10);
		Engine_EventWait(70);
		Engine_EventShowMessage(scene, 0);
		ColossoLogRollingStage_StopPaletteTask();
		Engine_TaskWait(2);
		pan1 = Object_GetById(10);
		pan1->motion_flags = 0;
		pan1->acceleration = 0x6666;
		pan1->speed = 0xcccc;
		FieldObject_SetPosition(pan1, pan1->x.fixed, 4 << 16, pan1->z.fixed);
		pan2 = Object_GetById(11);
		pan2->motion_flags = 0;
		pan2->acceleration = 0x6666;
		pan2->speed = 0xcccc;
		FieldObject_SetPosition(pan2, pan2->x.fixed, 32 << 16, pan2->z.fixed);
		Object_CommitPosition(pan2);
		Engine_EventWait(45);
		back1 = Object_GetById(10);
		back1->motion_flags = 0;
		back1->acceleration = 0x6666;
		back1->speed = 0xcccc;
		Engine_ObjectSetPosition(back1, back1->x.fixed, 32 << 16, back1->z.fixed);
		back2 = Object_GetById(11);
		back2->motion_flags = 0;
		back2->acceleration = 0x6666;
		back2->speed = 0xcccc;
		Engine_ObjectSetPosition(back2, back2->x.fixed, 4 << 16, back2->z.fixed);
		Object_CommitPosition(back2);
		Engine_EventWait(15);
		Engine_EventShowMessage(scene, 0);
		ColossoLogRollingStage_StartPaletteTask(104, 68, 0);
		Engine_EventWait(30);
		ColossoLogRollingStage_StartPaletteTaskFromState(168, 96, 10);
		Engine_EventWait(40);
		ColossoLogRollingStage_StartPaletteTaskFromState(104, 68, 10);
		Engine_EventWait(70);
		Engine_EventShowMessage(scene, 0);
		ColossoLogRollingStage_StopPaletteTask();
		Engine_TaskWait(2);
		Engine_CameraFollowActor(0, 0);
		ColossoLogRollingStage_InitializeStateInteraction(scene, 1);
	}
	else if (state == 1)
	{
		Engine_EventSetMessage((s32)MsgKorosseoObjectiveGetAcross);
		Engine_EventShowMessage(scene, 0);
	}
	FieldScene_RunMiddleSequence(state, scene, 1);
	Engine_EventEnd();
}

static void EV34_TALK(s32 scene)
{
	s32 state;
	s32 x;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(scene);
		return;
	}
	Engine_EventBegin();
	state = ColossoLogRollingStage_RunStateInteraction(scene, 2);
	if (state == 0)
	{
		Engine_EventSetMessage((s32)MsgKorosseoSteppingStoneStage);
		Camera_SetSpeed(3 << 16, 0x6000);
		Camera_MoveTo(188 << 17, -1, 152 << 16, 1);
		x = 280;
		Engine_CameraWaitForMove();
		Engine_EventWait(30);
		Event_ShowMessage(scene, 0);
		Korosseo_FadeInCompetitor(0, x, 200);
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
		ColossoLogRollingStage_SpawnPositionedObject(0, x, 152);
		ColossoLogRollingStage_SpawnPositionedObject(0, 296, 152);
		Engine_EventWait(10);
		Engine_LeaderCheckAhead();
		Camera_MoveTo(-1, -1, -1, 0);
		Engine_ActorFaceDirection(0, 0xc000, 15);
		Engine_LeaderCheckAhead();
		Camera_MoveTo(-1, -1, -1, 0);
		Engine_ActorFaceDirection(0, 0, 15);
		Engine_LeaderCheckAhead();
		Camera_MoveTo(-1, -1, -1, 0);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 15);
		Event_ShowMessage(scene, 0);
		ColossoLogRollingStage_StartPaletteTask(96, 40, 0);
		ColossoLogRollingStage_StartPaletteTaskFromState(128, 40, 10);
		Engine_EventWait(30);
		ColossoLogRollingStage_StartPaletteTaskFromState(160, 40, 10);
		Engine_EventWait(30);
		ColossoLogRollingStage_StartPaletteTaskFromState(160, 72, 10);
		Engine_EventWait(30);
		Event_ShowMessage(scene, 0);
		ColossoLogRollingStage_StopPaletteTask();
		Korosseo_RestoreCompetitor(0);
		Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
		ColossoLogRollingStage_InitializeStateInteraction(scene, 2);
	}
	else if (state == 1)
	{
		Engine_EventSetMessage((s32)MsgKorosseoYourGoalInStageSimple);
		Event_ShowMessage(scene, 0);
	}
	FieldScene_RunMiddleSequence(state, scene, 2);
	Engine_EventEnd();
}

static void EV35_TALK(s32 scene)
{
	struct EventWork *work;
	s32				  state;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(scene);
	}
	else
	{
		Engine_EventBegin();
		state = ColossoLogRollingStage_RunStateInteraction(scene, 3);
		if (state == 0)
		{
			work = gEventWork;
			Engine_EventSetMessage((s32)MsgKorosseoPlaceCalledWall);
			ColossoLogRollingStage_ResetAndRunSceneTask();
			Engine_CameraSetSpeed(3 << 16, 0x6000);
			Engine_CameraMoveTo(154 << 18, -1, 184 << 16, 1);
			Engine_CameraWaitForMove();
			Engine_EventWait(30);
			Event_ShowMessage(scene, 0);
			ColossoLogRollingStage_StartSceneTask();
			Engine_EventWait(60);
			Engine_EventShowMessage(scene, 0);
			Korosseo_FadeInCompetitor(0, 504, 200);
			/* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
			Value3(Engine_ActorFaceDirection, 0, 0, 0);
			ColossoLogRollingStage_WaitForSceneTask();
			Actor_SetSpeed(0, 0x18000, 0xc000);
			ColossoLogRollingStage_PositionScaledObject(0, 680, 200);
			if (work->raised_trigger != 5)
			{
				do
				{
					Engine_TaskWait(1);
					if (++state > 239)
					{
						break;
					}
				} while (work->raised_trigger != 5);
			}
			EV05_RAISED();
			Actor_FaceDirection(0, 0xc000, 20);
			Actor_ShowEmote(0, 0x103, 60);
			Engine_EventShowMessage(scene, 0);
			Korosseo_RestoreCompetitor(0);
			Engine_CameraFollowActor(0, 0);
			ColossoLogRollingStage_InitializeStateInteraction(scene, 3);
			work->raised_trigger = 0;
		}
		else if (state == 1)
		{
			Engine_EventSetMessage((s32)MsgKorosseoOperatorWallsCheer);
			Engine_EventShowMessage(scene, 0);
		}
		FieldScene_RunMiddleSequence(state, scene, 3);
		Engine_EventEnd();
	}
}

static void EV36_TALK(s32 event)
{

	s32				   result;
	struct FieldActor *actor;
	s32				   x;
	s32				   y;
	s32				   raised_y;
	s32				   shifted_x;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(event);
	}
	else
	{
		Engine_EventBegin();
		result = ColossoLogRollingStage_RunStateInteraction(event, 4);
		if (result == 0)
		{
			Engine_EventSetMessage((s32)MsgKorosseoPlaceNormallyCalledFreeClimb);
			Camera_SetSpeed(0x30000, 0x6000);
			Camera_MoveTo(0x3580000, -1, 0xa80000, 1);
			Engine_CameraWaitForMove();
			Engine_EventWait(30);
			Event_ShowMessage(event, 0);
			Event_ShowMessage(event, 0);
			Korosseo_FadeInCompetitor(0, 0x330, 200);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
			Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x348, 200);
			Engine_ActorFaceDirection(0, 0xc000, 20);
			battle_owner_69();
			Camera_MoveTo(-1, -1, -1, 0);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
			actor = Object_GetById(0);
			y = actor->y.fixed;
			x = actor->x.fixed;
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 10);
			raised_y = 0x60000 + y;
			Engine_ObjectSetPosition(actor, x, raised_y, actor->z.fixed);
			Object_CommitPosition(actor);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 14);
			shifted_x = 0x400000 + x;
			Engine_ObjectSetPosition(actor, shifted_x, raised_y, actor->z.fixed);
			Object_CommitPosition(actor);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 10);
			Engine_ObjectSetPosition(actor, shifted_x, y + 0x360000, actor->z.fixed);
			Object_CommitPosition(actor);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 15);
			Engine_ObjectSetPosition(actor, x + 0x300000, y + 0x360000, actor->z.fixed);
			Object_CommitPosition(actor);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 12);
			Event_ShowMessage(event, 0);
			Korosseo_RestoreCompetitor(0);
			Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
			ColossoLogRollingStage_InitializeStateInteraction(event, 4);
		}
		else if (result == 1)
		{
			Engine_EventSetMessage((s32)MsgKorosseoClearStageMustAbleChange);
			Event_ShowMessage(event, 0);
		}
		FieldScene_RunMiddleSequence(result, event, 4);
		Engine_EventEnd();
	}
}

static void ColossoLogRollingStage_PositionActor(s32 selector, s32 x, s32 z)
{

	s32 *record;

	record = (s32 *)Engine_ActorLookup(selector);
	if (record != 0)
	{
		ObjectDispatch_InitFromTable6(record);
		Object_SetMode(record, 5);
		Engine_ObjectSetPosition(record, x << 16, record[3], z << 16);
	}
}

static void ColossoLogRollingStage_PositionAndActivateActor(s32 selector, s32 x, s32 z)
{

	s32 *record;

	record = (s32 *)Engine_ActorLookup(selector);
	if (record != 0)
	{
		ObjectDispatch_InitFromTable6(record);
		Object_SetMode(record, 5);
		Engine_ObjectSetPosition(record, x << 16, record[3], z << 16);
		Object_CommitPosition(record);
		Object_SetMode(record, 1);
	}
}

static void EV37_TALK(s32 event)
{

	s32				   i;
	s32				   result;
	struct FieldActor *actor;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(event);
	}
	else
	{
		Engine_EventBegin();
		result = ColossoLogRollingStage_RunStateInteraction(event, 5);
		if (result == 0)
		{
			Engine_EventSetMessage((s32)MsgKorosseoCalledMovingSidewalkStage);
			Camera_SetSpeed(0x30000, 0x6000);
			Camera_MoveTo(0x4380000, -1, 0xa80000, 1);
			Engine_CameraWaitForMove();
			Engine_EventWait(30);
			Event_ShowMessage(event, 0);
			Event_ShowMessage(event, 0);
			Korosseo_FadeInCompetitor(0, 0x3d8, 184);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
			ColossoLogRollingStage_PositionAndActivateActor(0, 0x3e0, 184);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
			ColossoLogRollingStage_PositionActor(0, 0x460, 184);
			Engine_EventWait(120);
			Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
			Engine_EventWait(120);
			ColossoLogRollingStage_ResetActorMotion(0);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_FR) && !defined(TBS_EDITION_IT)
			Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
#endif
			Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
			actor = Object_GetById(0);
			for (i = 119; i >= 0; i--)
			{
				if (actor->x.fixed > 0x3e00000)
				{
					actor->x.fixed -= 0x13333;
				}
				Engine_TaskWait(1);
			}
			Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x103, 60);
			ColossoLogRollingStage_PositionActor(0, 0x460, 184);
			Event_ShowMessage(event, 0);
			ColossoLogRollingStage_ResetActorMotion(0);
			gGameState.movement_mode = 1;
			Korosseo_RestoreCompetitor(0);
			Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
			ColossoLogRollingStage_InitializeStateInteraction(event, 5);
		}
		else if (result == 1)
		{
			Engine_EventSetMessage((s32)MsgKorosseoInStageMustTryOutpace);
			Event_ShowMessage(event, 0);
		}
		FieldScene_RunMiddleSequence(result, event, 5);
		Engine_EventEnd();
	}
}

static void EV38_TALK(s32 actor)
{

	s32 state;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(actor);
		return;
	}

	Engine_EventBegin();
	state = ColossoLogRollingStage_RunStateInteraction(actor, 6);

	if (state == 0)
	{
		Engine_EventSetMessage((s32)MsgKorosseoAnotherLogRollingArea);
		Engine_CameraSetSpeed(0x30000, 0x6000);
		Engine_CameraMoveTo(0x5080000, -1, 0x980000, 1);
		Engine_CameraWaitForMove();
		Engine_EventWait(30);
		Engine_EventShowMessage(actor, 0);
		ColossoLogRollingStage_StartPaletteTask(0xb4, 0x58, 0);
		Engine_EventWait(60);
		Engine_EventShowMessage(actor, 0);
		ColossoLogRollingStage_StartPaletteTaskFromState(0x20, 0x54, 10);
		Engine_EventWait(30);
		Engine_EventShowMessage(actor, 0);
		ColossoLogRollingStage_StartPaletteTaskFromState(0x60, 0x54, 30);
		Engine_EventWait(60);
		Engine_EventShowMessage(actor, 0);
		ColossoLogRollingStage_StopPaletteTask();
		Engine_EventWait(2);
		Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
		ColossoLogRollingStage_InitializeStateInteraction(actor, 6);
	}
	else if (state == 1)
	{
		Engine_EventSetMessage((s32)MsgKorosseoHereMustFigureOutHow);
		Engine_EventShowMessage(actor, 0);
	}

	FieldScene_RunMiddleSequence(state, actor, 6);
	Engine_EventEnd();
}

static void ColossoLogRollingStage_RestoreActorPositions(void)
{

	s32 center;

	{
		s32 x = GameFlag_GetByte(896);
		s32 z = GameFlag_GetByte(904);
		center = 0x80000;
		x <<= 20;
		x += center;
		z <<= 20;
		z += center;
		Engine_ActorSetPosition(GERALD, x, z);
	}
	{
		s32 x = GameFlag_GetByte(912);
		s32 z = GameFlag_GetByte(920);
		x <<= 20;
		x += center;
		z <<= 20;
		z += center;
		Engine_ActorSetPosition(IWAN, x, z);
	}
	{
		s32 x = GameFlag_GetByte(928);
		s32 z = GameFlag_GetByte(936);
		x <<= 20;
		x += center;
		z <<= 20;
		z += center;
		Engine_ActorSetPosition(MEARI, x, z);
	}
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korosseo/solo_competitor.c"

static void ColossoLogRollingStage_MarkSceneProgress(void)
{

	u8	*state;
	s16 *table;
	s32	 slotValue;
	s32	 value;
	u16 *field;

	state = *(u8 **)&gEventWork;
	table = (s16 *)&gGameState;
	slotValue = ((struct GameState *)table)->selected_actor;
	if (slotValue != 0)
	{
		if ((s16) * (u16 *)&((struct EventWork *)state)->psynergy_request >> 10 == slotValue)
		{
			if (Engine_GameFlagIsSet(0x141) != 0)
			{
				field = (u16 *)&((struct EventWork *)state)->raised_trigger;
				value = 99;
				*field = value;
			}
		}
	}
}

static void EV99_RAISED(void)
{

	struct EventWork *state;
	s16				 *table;
	StageActor		 *target;
	StageActor		 *actor;
	s32				 *frame;
	s32				  active_slot;
	s32				  slot;
	s32				  best;
	s32				  best_slot;
	s32				  dx;
	s32				  adx;
	s32				  dz;
	s32				  base;
	s32				  z;

	state = gEventWork;
	best_slot = 8;
	best = 0x100000;
	table = (s16 *)&gGameState;
	active_slot = *(s32 *)&table[250];
	target = Object_GetById(active_slot);
	Engine_EventBegin();
	for (slot = 8; slot <= 66; slot++)
	{
		actor = Object_GetById(slot);
		if (actor == 0)
		{
			continue;
		}
		if (actor->state != 1)
		{
			continue;
		}
		if (*actor->sprite->entry != 165)
		{
			continue;
		}
		dx = (target->x - actor->x) / 65536;
		dz = (target->z - actor->z) / 65536;
		if (dz > 0)
		{
			continue;
		}
		adx = dx;
		if (adx < 0)
		{
			adx = -adx;
		}
		if (dz < 0)
		{
			dz = -dz;
		}
		if (adx + dz < best)
		{
			best_slot = slot;
			best = adx + dz;
		}
	}
	Engine_EventSetMessage((s32)MsgKorosseoMatchAboutBeginPleaseTake);
	Engine_EventShowMessage(best_slot, 0);
	frame = &state->start_transition;
	*frame = 0x200;
	state->transition_frames = 15;
	Engine_EventWait(20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	base = active_slot * 16;
	GameFlag_SetByte(base + 880, target->x >> 20);
	z = target->z >> 20;
	GameFlag_SetByte(base + 888, z);
	active_slot++;
	if (active_slot > 3)
	{
		Engine_EventRequestExit(10);
		Engine_GameFlagSet(282);
	}
	else
	{
		Korosseo_SelectSoloCompetitor(active_slot);
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		*frame = 0;
	}
	Engine_EventEnd();
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korosseo/solo_round.c"

static void ColossoLogRollingStage_ClearSavedActorPositions(void)
{

	GameFlag_SetByte(896, 0);
	GameFlag_SetByte(904, 0);
	GameFlag_SetByte(912, 0);
	GameFlag_SetByte(920, 0);
	GameFlag_SetByte(928, 0);
	GameFlag_SetByte(936, 0);
}

static s32 ColossoLogRollingStage_RunStateInteraction(s32 actor, s32 flags)
{
	s32 scene;
	s32 msg;
	s32 result;

	Battle_ResetEffectCounter();
	UiText_DrawQuantity(flags, 5);
	scene = gGameState.scene;
	if (scene == (s32)&SceneId_KorosseoKawa)
	{
		msg = (s32)MsgKorosseoStageFirstFinalsMatch;
	}
	else if (scene == (s32)&SceneId_KorosseoKabe)
	{
		msg = (s32)MsgKorosseoStageSecondFinalsMatch;
	}
	else
	{
		msg = (s32)MsgKorosseoStageThirdFinalsMatch;
	}
	Engine_EventSetMessage(msg);
	Engine_EventShowMessage(actor, 0);
	if (Engine_GameFlagIsSet(flags + 512) != 0)
	{
		return 2;
	}
	if (Engine_GameFlagIsSet(flags + 520) != 0)
	{
		result = PartyTalkMenu_Choose(0);
		if (result == 1)
		{
			return 2;
		}
		if (result == 2 || result == -1)
		{
			return 3;
		}
		return result;
	}
	Engine_GameFlagSet(flags + 520);
	Engine_EventSetMessage((s32)MsgKorosseoWouldYouLikeHearDescription);
	Engine_EventOpenMessage(actor, 0);
	return Engine_EventChooseYesNo(0, 0);
}

static void ColossoLogRollingStage_InitializeStateInteraction(s32 actor, s32 flags)
{
	s32 scene;
	s32 msg;

	UiText_DrawQuantity(flags, 5);
	scene = gGameState.scene;
	if (scene == (s32)&SceneId_KorosseoKawa)
	{
		msg = (s32)MsgKorosseoStageFirstFinalsMatch;
	}
	else if (scene == (s32)&SceneId_KorosseoKabe)
	{
		msg = (s32)MsgKorosseoStageSecondFinalsMatch;
	}
	else
	{
		msg = (s32)MsgKorosseoStageThirdFinalsMatch;
	}
	Engine_EventSetMessage(msg + 1);
	Engine_EventShowMessage(actor, 0);
}

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{

	struct FieldActor *rec;
	struct FieldActor *record;
	s32				   p9;
	s32				   p11;
	s32				   count;
	s32				   state;
	s32				   obj;
	s32				   hi;
	s32				   lo;
	s32				   tail;
	s32				   sx;
	s32				   sy;
	s32				   i;
	u8				   buf[8];

	rec = Object_GetById(owner);
	p9 = rec->x.part.pixel;
	p11 = rec->z.part.pixel;
	if (mode != 3)
	{
		count = GetPartyMemberCount();
		for (i = 0; i < count; i++)
		{
			s32 at = 504 + i;

			buf[i] = ((u8 *)&gGameState)[at];
		}
		if (count <= 1)
		{
			Engine_EventSetMessage((s32)MsgKorosseoDoYourBest);
			Engine_EventShowMessage(owner, 0);
			return;
		}
		if (GameFlag_IsSet(base + 512) != 0)
		{
			Engine_EventSetMessage((s32)MsgKorosseoUnfortunatelyWeHaveFullHouse);
			Engine_EventShowMessage(owner, 0);
			return;
		}
		if (mode == 2)
		{
			state = 0;
			Engine_TaskWait(6);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgKorosseoWouldLikeFriendCheerFor);
			Event_OpenMessage(owner, 0);
			state = Engine_EventChooseYesNo(0, 0);
		}
		if (state == 0)
		{
			if (state < count)
			{
				for (i = 0; i < count; i++)
				{
					Party_RemoveActiveOwner((s8)buf[i]);
				}
			}
			for (i = 0; i < count; i++)
			{
				if ((s8)buf[i] != 0)
				{
					Party_AddActiveOwner((s8)buf[i]);
				}
			}
			obj = Menu_OpenCharacterSelector();
			for (i = 0; i < count; i++)
			{
				Party_RemoveActiveOwner((s8)buf[i]);
			}
			for (i = 0; i < count; i++)
			{
				Party_AddActiveOwner((s8)buf[i]);
			}
			if (obj != -1)
			{
				goto L_main;
			}
		}
	}
	Engine_EventSetMessage((s32)MsgKorosseoIfKnowWhoWantCheer);
	Engine_EventShowMessage(owner, 0);
	return;
L_main:
	UiText_DrawQuantity(obj, 1);
	Engine_EventSetMessage((s32)MsgKorosseoRobinWillCheerForWay);
	Event_ShowMessage(owner, 0);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Actor_SetSpeed(obj, 0x10000, 0x8000);
	Actor_SetSpeed(owner, 0x10000, 0x8000);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetPosition(obj, record->x.fixed, record->z.fixed);
	}
	hi = p11 + 16;
	Actor_WalkToAndWait(obj, p9, hi);
	lo = p9 + 16;
	Engine_ActorWalkToAndWait(0, lo, hi);
	Engine_ActorFaceEachOther(obj, ACTOR_PARTY_LEADER, 30);
	Engine_ActorSetAnimation(obj, 3);
	tail = hi - 32;
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Actor_WalkToAndWait(owner, p9, tail);
	Engine_ActorWalkTo(owner, lo, tail);
	Object_LinkObjectAndSetCallback(0, obj);
	Actor_WalkToAndWait(obj, p9, tail);
	Engine_ActorSetAnimation(owner, 1);
	Actor_FaceDirection(owner, 0x8000, 0);
	Actor_WalkToAndWait(obj, p9, p11 - 48);
	Actor_WalkToAndWait(owner, p9, tail);
	Actor_WalkToAndWait(owner, p9, p11);
	Party_RemoveActiveOwner(obj);
	GameFlag_Set(base + 512);
	rec = Object_GetById(obj);
	sx = rec->x.fixed >> 20;
	GameFlag_SetByte((obj << 4) + 880, sx);
	sy = rec->z.fixed >> 20;
	GameFlag_SetByte((obj << 4) + 888, sy);
}

static void ColossoLogRollingStage_ApplyItemToMatchingSlots(s32 handle, s32 item)
{

	u8 *record;
	s32 slot;

	record = UnitGet(handle);
	Inventory_AddItem(handle, item);

	for (slot = 0; slot <= 14; slot++)
	{
		if (((struct BtlUnit *)record)->inventory[slot] == item)
		{
			Inventory_Equip(handle, slot);
		}
	}
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korosseo/korosseo.c"

static void ColossoLogRollingStage_InitializeModeTask(u32 mode, u32 parameter)
{
	s32 handler;

	Korosseo_ModeTaskMode = mode;
	Korosseo_ModeTaskParam = (parameter << 4);

	{
		s32 budget = 0xc80;
		s32 task = (s32)Korosseo_UpdateModeTask;
		Engine_TaskAddCallback(task, budget);
	}

	handler = (s32)gColossoModeScriptDefault;
	if (mode == 2)
	{
		handler = (s32)gColossoModeScript2;
	}
	if (mode == 4)
	{
		handler = (s32)gColossoModeScript4;
	}
	if (mode == 3)
	{
		if (parameter != 0)
		{
			handler = (s32)gColossoModeScript3B;
		}
		else
		{
			handler = (s32)gColossoModeScript3A;
		}
	}

	Korosseo_ModeTaskTimer = 0;
	Korosseo_ModeTaskScript = (s16 *)handler;
	Korosseo_ModeMoveTarget = 0;
	Korosseo_ModeMoveDuration = 0;
	Korosseo_ModeTaskPosition = 0;
}

static void ColossoLogRollingStage_RunScriptedTransition(s32 mode)
{

	if (mode == 0)
	{
		Engine_EventBegin();
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		Engine_EventWait(30);
		Engine_AudioPlayCue(0x59);
		Korosseo_LoadPortrait(0);
		ColossoLogRollingStage_InitializeModeTask(1, 0);
		Engine_EventWait(120);
		Engine_EventEnd();
		return;
	}

	Engine_AudioPlayCue(0xf7);
	Engine_EventBegin();
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	{
		s16 *base = gColossoModeScriptDefault;
		*(s16 *)((u8 *)base + 30) = (mode * 60);
	}
	Engine_EventWait(30);
	Engine_AudioPlayCue(mode + 0x5a);
	Korosseo_LoadPortrait(mode);
	ColossoLogRollingStage_InitializeModeTask(1, 0);
	Engine_EventWait(120);

	goto check_transition;
wait_transition:
	Engine_TaskWait(1);
check_transition:
	if (AudioCommand_GetStateByte() != 0)
		goto wait_transition;

	Engine_AudioPlayCue(0x121);
	Korosseo_LoadPortrait(5);
	ColossoLogRollingStage_InitializeModeTask(2, 0);
	Engine_AudioPlayCue(0xec);
	Engine_EventWait(60);
	ColossoLogRollingStage_InitializeModeTask(2, 1);
	Engine_AudioPlayCue(0xec);
	Engine_EventWait(60);
	Korosseo_LoadPortrait(6);
	ColossoLogRollingStage_InitializeModeTask(2, 0);
	Engine_AudioPlayCue(0xec);
	Engine_EventWait(60);
	Korosseo_LoadPortrait(7);
	ColossoLogRollingStage_InitializeModeTask(4, 0);
	Engine_AudioPlayCue(0xed);
	Audio_PlayCueFromEventWork();
	Engine_EventEnd();
	Engine_GameFlagSet(0x123);
}

static void FieldScene_RunScene3bcSequenceA(s32 a0)
{

	s32 kind;

	Engine_AudioPlayCue(247);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	gColossoModeScript3A[13] = a0 * 60;
	STORE_CONST(gColossoModeScript3B[13]) = (a0 < 0 ? -a0 : a0) * 60;
	if (a0 < 0)
	{
		Engine_EventWait(30);
		Engine_AudioPlayCue(86);
		Korosseo_LoadPortrait(8);
		/* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
		Value2(ColossoLogRollingStage_InitializeModeTask, 3, 1);
		Engine_EventWait(-a0 * 60 + 60);
		kind = 0;
	}
	else
	{
		Engine_EventWait(30);
		Engine_AudioPlayCue(a0 + 90);
		Korosseo_LoadPortrait(4);
		/* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
		Value2(ColossoLogRollingStage_InitializeModeTask, 3, 0);
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
#include "../common/korosseo/compete.c"

static void ColossoLogRollingStage_ResetActorMotion(s32 selector)
{

	struct FieldActor *record;

	record = Object_GetById(selector);
	ObjectDispatch_InitFromTable6(record);

	record->velocity_x = 0;
	record->velocity_z = 0;
	record->target_x = 0x80000000;
	record->target_z = 0x80000000;
}

static void ColossoLogRollingStage_EnsurePaletteHandle(void)
{

	s16 *cursor = &Korosseo_MarkerSlot;

	if (*cursor == -1)
	{
		*cursor = Resource_LoadFixedBlockBIntoFreeSlot();
	}
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korosseo/marker.c"

void ColossoLogRollingStage_StartPaletteTask(u32 first_value, u32 second_value, u32 mode)
{
	ColossoLogRollingStage_EnsurePaletteHandle();

	Korosseo_MarkerX = first_value;
	Korosseo_MarkerY = second_value;
	Korosseo_MarkerPriority = (mode & 3);
	Korosseo_MarkerBlink = 0;
	Korosseo_MarkerSteps = 0;

	{
		s32 budget = 0xc80;
		s32 task = (s32)Korosseo_UpdateMarker;
		Engine_TaskAddCallback(task, budget);
	}
}

static void ColossoLogRollingStage_StartPaletteTaskFromState(u32 first_value, u32 second_value, u32 mode)
{
	Korosseo_MarkerEndX = first_value;
	Korosseo_MarkerEndY = second_value;
	*(u16 *)&Korosseo_MarkerStartX = Korosseo_MarkerX;
	*(u16 *)&Korosseo_MarkerStartY = Korosseo_MarkerY;
	Korosseo_MarkerSteps = mode;
	Korosseo_MarkerStep = 0;

	{
		s32 budget = 0xc80;
		s32 task = (s32)Korosseo_UpdateMarker;
		Engine_TaskAddCallback(task, budget);
	}
}

static void ColossoLogRollingStage_StopPaletteTask(void)
{

	Engine_TaskRemoveCallback(Korosseo_UpdateMarker);
	Resource_ResetEntry(Korosseo_MarkerSlot);
	Korosseo_MarkerSlot = -1;
}

static void ColossoLogRollingStage_PositionScaledObject(s32 id, s32 x, s32 z)
{

	ScaledStageObject *object = (ScaledStageObject *)Engine_ActorLookup(id);
	s32				   scale;

	if (object != 0)
	{
		scale = 0x20000;
		object->scale_x = scale;
		object->scale_z = scale / 2;
		object->state = 0;
		ObjectDispatch_InitFromTable6(object);
		Object_SetMode(object, 5);
		Engine_ObjectSetPosition(object, x << 16, object->y, z << 16);
	}
}

void ColossoLogRollingStage_SpawnPositionedObject(s32 object_id, s32 x, s32 z)
{

	u8 *object = (u8 *)Engine_ActorLookup(object_id);

	if (object == 0)
	{
		return;
	}

	{
		s32 move_rate = 0x14000;
		u8 *state_byte = object;
		u8	zero = 0;
		((struct FieldActor *)object)->speed = move_rate;
		((struct FieldActor *)object)->acceleration = move_rate >> 1;
		state_byte = &((struct FieldActor *)object)->paused;
		*state_byte = zero;
	}

	ObjectDispatch_InitFromTable6(object);
	Object_SetMode(object, 5);
	Engine_ObjectSetPosition(object, x << 16, ((struct FieldActor *)object)->y.fixed, z << 16);
	Object_CommitPosition(object);
	Object_SetMode(object, 1);
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korosseo/item_icon.c"

static void WriteU32AsHex(u8 *hex_text, u32 value)
{
	s32 digit_index;

	hex_text += 8;
	*hex_text = 0;
	hex_text--;
	for (digit_index = 7; digit_index >= 0; digit_index--)
	{
		*hex_text = gColossoHexChars[value & 15];
		value >>= 4;
		hex_text--;
	}
}

static void ColossoLogRollingStage_NoopSceneHook(void)
{
}

static void ColossoLogRollingStage_SetBalanceStateReady(void)
{
	SceneControl *control = (SceneControl *)gSceneState;

	control->enabled = 9;
}

static void ColossoLogRollingStage_WaitForBalanceState(void)
{
	SceneControl *control = (SceneControl *)gSceneState;

	while (control->enabled != 9)
	{
		Engine_TaskWait(1);
	}
}

static void ColossoLogRollingStage_SpawnRandomSceneEffect(StageEffect *source)
{

	s32 position[3];
	u32 random_value;

	if (source->vertical_motion >= -255 && source->vertical_motion <= 255)
	{
		source->state = 0;
	}
	random_value = Engine_RandomNext();
	if (random_value * 100 >> 16 <= 9)
	{
		StageEffect *effect;
		s32			 angle;
		s32			 radius;

		position[0] = source->x;
		position[1] = source->y;
		position[2] = source->z;
		angle = Engine_RandomNext();
		radius = Engine_RandomNext();
		Vector_AddPolarOffset(angle << 4, radius, position);
		{
			s32 x = position[0];
			s32 y = position[1];
			s32 z = position[2];

			effect = Engine_ObjectCreate(285, x, y, z);
		}
		if (effect != 0)
		{
			effect->state = 0;
			Engine_ActorSetSpriteFlags(effect, 0);
			Engine_ObjectSetScript(effect, gColossoRandomEffect);
			Object_SetMode(effect, 1);
			Object_SetMode(effect, 0);
		}
	}
}

static s32 ColossoLogRollingStage_RaiseLinkedSceneEffect(StageEffect_02003d88 *source)
{

	StageEffect_02003d88 *effect = Object_GetById(source->linked_effect_slot);

	Engine_ObjectSetPosition(effect, source->x, source->y + 0x240000, source->z);
	effect->state = 0;
	Engine_ObjectSetScript(effect, gColossoLinkedEffect);
	Engine_AudioPlayCue(83);
	source->linked_effect_slot = 0;
	return 0;
}

static s32 ColossoLogRollingStage_PositionActiveActor(s32 first_handle, s32 second_handle)
{
	u8				  *work = gKorosseoWork;
	struct FieldActor *actor;
	s32				   flag;
	s32				   x;
	s32				   z;
	u16				  *cue;
	s16				  *wait;

	flag = Engine_GameFlagIsSet(0x211);
	actor = Object_GetById(gGameState.selected_actor);
	if (*(s32 *)(work + 232) < actor->x.fixed)
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
		cue = (u16 *)(work + 228);
	}
	else
	{
		z = *(s32 *)(work + 236) - 0x100000;
		cue = (u16 *)(work + 226);
	}
	wait = (s16 *)&actor->timer;
	*wait = *cue;
	actor->acceleration = 0x4000;
	actor->speed = 0x10000;
	Engine_ObjectSetPosition(actor, x, 0, z);
	Engine_GameFlagSet(0x211);
	Engine_ObjectSetScript(actor, KorosseoMaruta_PlaceScript);
	while (*wait != 0)
	{
		Engine_TaskWait(1);
	}
	if (flag == 0)
	{
		/* FAKEMATCH: void functions called as returning s32; as void calls the argument moves are ordered differently. */
		((s32 (*)())ColossoLogRollingStage_ApplyItemToMatchingSlots)(0, first_handle);
		UiText_DrawQuantity(first_handle, 2);
	}
	else
	{
		((s32 (*)())ColossoLogRollingStage_ApplyItemToMatchingSlots)(0, second_handle);
		UiText_DrawQuantity(second_handle, 2);
	}
	UiText_DrawQuantity(gGameState.selected_actor, 1);
	Engine_MessageShowCentered((s32)MsgKorosseoRobinGotItem, 3);
	ObjectDispatch_WaitForValue16(actor);
	return flag;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korosseo/path_rival.c"

void ColossoLogRollingStage_SetupSceneDescriptor(s32 first_actor, s32 second_actor, s32 mode,
	s32 centre, s32 extra, s32 third_actor, s32 fourth_actor)
{

	u8				  *descriptor;
	struct FieldActor *first_record;
	struct FieldActor *second_record;
	s32				   handle;
	s32				   extent;

	descriptor = Runtime_AllocateBlock(59, 0x7170);
	handle = (s32)Runtime_BumpAllocateAlternatePool(512);

	*(u16 *)(descriptor + 222) = first_actor;
	*(u16 *)(descriptor + 224) = second_actor;
	*(u16 *)(descriptor + 226) = third_actor;
	*(u16 *)(descriptor + 228) = fourth_actor;
	*(u16 *)(descriptor + 230) = mode;
	*(s32 *)(descriptor + 232) = centre;
	*(s32 *)(descriptor + 236) = extra;

	first_record = Object_GetById(first_actor);
	second_record = Object_GetById(second_actor);

	if (Engine_GameFlagIsSet(0x109) == 0)
	{
		second_record->x.fixed = (centre << 1) - first_record->x.fixed;
		second_record->z.fixed = first_record->z.fixed;
	}

	*(u16 *)(descriptor + 218) = 0;
	*(u16 *)(descriptor + 220) = 0;

	Resource_DecodeType01(Korosseo_GaugeGraphics, handle);

	extent = Resource_FindFreeEntry();
	*(u16 *)(descriptor + 216) = extent;
	Engine_VramLoad((s16)extent, 512, handle);

	Engine_TaskAddCallback((s32)Korosseo_DrawGauge + 1, 0xc76);

	Runtime_BumpFree(handle);
}

static void ColossoLogRollingStage_InitializeSceneControl(s32 resource)
{
	u8			 *work = gKorosseoWork;
	SceneControl *control = (SceneControl *)gSceneState;

	Resource_DecodeType01(Resource_GetTableEntry(resource), work + 240);
	if (Engine_GameFlagIsSet(0x109) == 0)
	{
		control->enabled = 1;
		control->active = 1;
		control->scene_variant = *(u16 *)(work + 224);
		control->timer = 0;
		control->phase = 0;
	}
	Engine_TaskAddCallback(Korosseo_UpdatePathRival, 0xc85);
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korosseo/scene_state.c"

static void *SceneData_FindSlotAtPosition(void *position, void *self)
{
	s32				   *pos = position;
	struct FieldActor **slots = gEventWork->actors;
	u32					i;

	for (i = 8; i <= 65; i++)
	{
		struct FieldActor *p = slots[i];

		if ((pos[0] >> 20) == (p->x.fixed >> 20) && (pos[1] >> 20) == (p->y.fixed >> 20)
			&& (pos[2] >> 20) == (p->z.fixed >> 20))
		{
			return p;
		}
	}
	return 0;
}

static void ColossoLogRollingStage_PushStagedActor(void)
{

	MarutaSceneRecord *subject;
	MarutaSceneRecord *target;
	MarutaSceneRecord *blocker;
	u32				   step;
	u32				   direction;
	Position3		   position;
	u32				   data_index = 250;
	s32				   zero;
	s32				   subject_handle;

	subject_handle = *(s32 *)((u8 *)&gGameState + (data_index << 1));
	subject = Object_GetById(subject_handle);

	direction = subject->facing >> 12;

	step = StagedActor_DirectionSteps[direction];
	position.x = subject->x + (step & 0xffff0000);
	position.y = subject->y;
	step <<= 16;
	position.z = subject->z + step;

	target = SceneData_FindSlotAtPosition(&position, subject);
	if (target == 0)
	{
		return;
	}

	step = StagedActor_DirectionSteps[direction];
	position.x = target->x + (step & 0xffff0000);
	position.y = target->y;
	step <<= 16;
	position.z = target->z + step;

	blocker = SceneData_FindSlotAtPosition(&position, target);
	if (blocker != 0 && (blocker->flags & 1) != 0)
	{
		return;
	}

	position.x = target->x;
	position.y = target->y + 0x100000;
	position.z = target->z;

	blocker = SceneData_FindSlotAtPosition(&position, target);
	if (blocker != 0 && (blocker->flags & 1) != 0)
	{
		return;
	}

	target->state = 2;
	zero = 0;

	step = StagedActor_DirectionSteps[direction];
	position.x = target->x + (step & 0xffff0000);
	position.y = target->y;
	step <<= 16;
	position.z = target->z + step;

	if (Object_CheckMovementCollision(target, &position) > 0)
	{
		return;
	}

	Object_SetMode(subject, 8);
	Engine_TaskWait(15);

	target->rate_x = 0x3333;
	target->rate_z = 0x3333;
	Engine_ObjectSetPosition(target, position.x, position.y, position.z);

	subject->rate_x = 0x3333;
	subject->rate_z = 0x3333;
	Engine_ObjectSetPosition(subject, position.x, position.y, position.z);

	Engine_AudioPlayCue(0xee);
	Object_CommitPosition(target);
	Engine_AudioPlayCue(0x120);

	target->x = position.x;
	target->z = position.z;
	target->motion_24 = zero;
	target->motion_2c = zero;

	Object_SetMode(subject, 1);
}

static void *SceneActor_FindOccupantAheadOfSubject(void)
{
	struct FieldActor *record;
	s32				   facing;
	s32				   position[3];
	s32				  *occupant;

	record = Object_GetById(gGameState.selected_actor);

	facing = (record->facing + 0x2000) & 0xc000;

	position[0] = (record->x.fixed & 0xfff00000) + 0x80000;
	position[1] = record->y.fixed;
	position[2] = (record->z.fixed & 0xfff00000) + 0x80000;
	Vector_AddPolarOffset(0x100000, facing, position);

	occupant = SceneData_FindSlotAtPosition(position, record);
	if (occupant == 0)
	{
		position[0] = (record->x.fixed & 0xfff00000) + 0x80000;
		position[1] = record->y.fixed;
		position[2] = (record->z.fixed & 0xfff00000) + 0x80000;
		Vector_AddPolarOffset(0x200000, facing, position);

		occupant = SceneData_FindSlotAtPosition(position, record);
	}

	return occupant;
}

const s32 KorosseoMaruta_Record30Script[] = {
	SC_SET(SW_Y, PIXELS(1)),
	SC_WAIT(10),
	SC_SET(SW_Y, 0),
	SC_WAIT(10),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const s8 KorosseoMaruta_LogPattern[][6] = {
	{ 7, 5, 7, 5, 7, 11 },
	{ 4, 6, 4, 6, 4, 10 },
	{ 5, 7, 5, 7, 5, 12 },
	{ 6, 4, 6, 4, 6, 10 },
};

const u16 gColossoEarlySequenceData[] = {
	0, 0, 0x4000, 0x8000, 0x8000, 0x8000, 0xc000, 0,
};

const s32 gColossoSceneEventEffect[] = {
	SC_MOVE(PIXELS(1088), 0, PIXELS(184)),
	SC_SYNC,
	SC_MOVE(PIXELS(1130), 0, PIXELS(184)),
	SC_SYNC,
	SC_CALL(ColossoLogRollingStage_SetSceneEventValues),
	SC_LABEL(1),
	SC_SET(SW_Y, PIXELS(1)),
	SC_WAIT(10),
	SC_SET(SW_Y, 0),
	SC_WAIT(10),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

const u8 Korosseo_PortraitPaletteOffsets[] = {
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	0, 32, 32, 32, 96, 64, 64, 64, 128, 0,
#else
	0, 32, 32, 32, 96, 64, 64, 160, 128, 0,
#endif
};

const s16 gColossoModeScript2[] = {
	0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
	30, 0x7fff, 30, -1,
};

const s16 gColossoModeScript3B[] = {
	0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
	6, 0x7fff, 6, 0x1000, 240, 6, 0x7fff, 6,
	0x1000, 368, 6, 0x7fff, 6, 0x1000, 224, 6,
	0x7fff, 6, 0x2000, 0, 30, 0x1000, 352, 6,
	0x7fff, 6, 0x1000, 208, 6, 0x7fff, 6, 0x1000,
	336, 6, 0x7fff, 6, 0x1000, 192, 6, 0x7fff,
	6, 0x1000, 320, 6, 0x7fff, 6, -1,
};

char gColossoHexChars[20] = "0123456789ABCDEF";

s32 gColossoRandomEffect[] = {
	SC_ADD(SW_Y, -0x4000),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_FREE,
};

s32 gColossoLinkedEffect[] = {
	SC_SET(SW_HOOK, (s32)ColossoLogRollingStage_SpawnRandomSceneEffect),
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

u32 StagedActor_DirectionSteps[] = {
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

struct SceneEntrance gKorosseoMarutaEntrances[] = {
	{ 0, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 136, 0, 392, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gKorosseoMarutaExits[] = {
	SCENE_EXITS(145),
	SCENE_EXIT(10, 145, 1),
	SCENE_EXIT(11, 140, 66),
	SCENE_EXIT(4, 138, 99),
	SCENE_EXIT(5, 138, 99),
	SCENE_EXITS_END,
};

struct ScenePlacement gKorosseoMarutaPlacements[] = {
	{ 0x0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(152), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(216), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(712), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1240), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1272), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1304), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(536), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(568), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(600), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(632), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(664), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(536), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(568), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(600), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(632), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(664), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(720), 0, PIXELS(216), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x126, CONDITION_ALWAYS, 7,
		PIXELS(1080), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(1208), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(600), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(856), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1080), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1304), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1512), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1512), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x118, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x118, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x118, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

s32 gColossoSceneTaskStatus = 0;

s32 gColossoSceneTaskState = 0;

struct SceneEvent gKorosseoMarutaEvents[] = {
	{ 0x202, 90, CONDITION_ALWAYS, EVENT_SCRIPT(EV90_SCRIPT) },
	{ 0x202, 91, CONDITION_ALWAYS, EVENT_SCRIPT(EV91_SCRIPT) },
	{ EVENT_TOUCH, 11, 0x363, EVENT_SCRIPT(EV11_TOUCH) },
	{ 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(0x8c), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8c), 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_PSYNERGY) },
	{ 0x202, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(0x8c), 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_PSYNERGY) },
	{ 0x202, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EV23_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(0x8c), 15, 0x214, EVENT_SCRIPT(ColossoLogRollingStage_ActivateClearObstacleActors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 16, 0x215, EVENT_SCRIPT(ColossoLogRollingStage_ActivateClearObstacleActors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 17, 0x216, EVENT_SCRIPT(ColossoLogRollingStage_ActivateClearObstacleActors) },
	{ EVENT_TOUCH, 15, 0x367, EVENT_SCRIPT(EV15_TOUCH) },
	{ EVENT_TOUCH, 16, IF_FLAG_SET(0x367), EVENT_SCRIPT(EV16_TOUCH) },
	{ EVENT_TOUCH, 50, 0x210, EVENT_SCRIPT(EV50_TOUCH) },
	{ SEARCH(SEARCH_CHEST), 101, 0x36a, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ SEARCH(SEARCH_CHEST), 102, 0x36b, FIND_ITEM(ITEM_NUT) },
	{ 0x8413, 103, 0x36c, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ 0x50001815, 9, 0x362, EVENT_SCRIPT(EV09_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(0xc), 29, 0x360, EVENT_SCRIPT(EV29_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x91), 28, 0x361, EVENT_SCRIPT(EV28_PSYNERGY) },
	{ 0x50008e15, 30, 0x363, EVENT_SCRIPT(EV30_SCRIPT) },
	{ EVENT_RAISED, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EV04_RAISED) },
	{ EVENT_RAISED, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_RAISED) },
	{ EVENT_TALK, 32, CONDITION_ALWAYS, EVENT_SCRIPT(RunPartyCountInteraction) },
	{ EVENT_TALK, 33, CONDITION_ALWAYS, EVENT_SCRIPT(EV33_TALK) },
	{ EVENT_TALK, 34, CONDITION_ALWAYS, EVENT_SCRIPT(EV34_TALK) },
	{ EVENT_TALK, 35, CONDITION_ALWAYS, EVENT_SCRIPT(EV35_TALK) },
	{ EVENT_TALK, 36, CONDITION_ALWAYS, EVENT_SCRIPT(EV36_TALK) },
	{ EVENT_TALK, 37, CONDITION_ALWAYS, EVENT_SCRIPT(EV37_TALK) },
	{ EVENT_TALK, 38, CONDITION_ALWAYS, EVENT_SCRIPT(EV38_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 32, CONDITION_ALWAYS, MSGNO(MsgKorosseoKawaThisYearsFinalsAreIncredibleThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 33, CONDITION_ALWAYS, MSGNO(MsgKorosseoMarutaTheStreetVendorsCornOnThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 34, CONDITION_ALWAYS, MSGNO(MsgKorosseoMarutaICantSeeAnythingWhenIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 35, CONDITION_ALWAYS, MSGNO(MsgKorosseoMarutaItsNotVeryExcitingWorkBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 36, CONDITION_ALWAYS, MSGNO(MsgKorosseoMarutaStandingForLongPeriodsOfTime) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 37, CONDITION_ALWAYS, MSGNO(MsgKorosseoMarutaIWasUpAllNightTrying) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 38, CONDITION_ALWAYS, MSGNO(MsgKorosseoMarutaSomeTouristToldMeILook) },
	{ EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_RAISED) },
	{ SCENE_EVENTS_END },
};

s32 KorosseoMaruta_Actor8Action[] = {
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(10),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

s32 KorosseoMaruta_LeaderActionA[] = {
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(6),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(12),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

s32 KorosseoMaruta_LeaderActionB[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_Y, 3072),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 3072),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 3072),
	SC_WAIT(16),
	SC_ADD(SW_SCALE_Y, -3072),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -3072),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -3072),
	SC_WAIT(32),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

s32 KorosseoMaruta_LeaderActionC[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_Y, 3072),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 3072),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 3072),
	SC_WAIT(26),
	SC_ADD(SW_SCALE_Y, -3072),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -3072),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -3072),
	SC_WAIT(52),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

s32 gColossoMultiPhaseData[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_END,
};

s32 gColossoParticleKinds[] = {
	SC_CALL(ColossoLogRollingStage_AdvanceParticleMotion),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s16 Korosseo_PortraitSlot = -1;

s16 gColossoModeScriptDefault[] = {
#if !EDITION_INTERNATIONAL
	0x4000, -188, 2048, 0x1000, 256, 1, 0x2000, 16,
#else
	0x4000, -188, 2048, 0x1000, 384, 1, 0x2000, 16,
#endif
	1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
	0x3000, 2048, 20, 0x7fff, 60, -1,
};

s16 gColossoModeScript4[] = {
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

s16 gColossoModeScript3A[] = {
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
	SC_CALL(ColossoLogRollingStage_RaiseLinkedSceneEffect),
	SC_WAIT(3),
	SC_ARG(10),
	SC_WAIT(150),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ARG(1),
	SC_END,
};

s32 KorosseoMaruta_PlaceScript[] = {
	SC_ARG(2),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(6),
	SC_CALL(ColossoLogRollingStage_RaiseLinkedSceneEffect),
	SC_WAIT(3),
	SC_ARG(28),
	SC_WAIT(150),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ARG(1),
	SC_END,
};
