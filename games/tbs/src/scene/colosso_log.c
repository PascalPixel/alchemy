#include "game_state.h"
#include "io_write_queue.h"
#include "resource_ids.h"
#include "scene_ids.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/colosso_log.h"
#include "unit/scene/common/colosso_solo.h"
#include "unit/scene/common/colosso_marker.h"
#include "unit/scene/common/colosso_setup.h"
#include "unit/scene/common/colosso_occupant.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/menu/menu.h"
#include "unit/sound/sound.h"
#include "unit/runtime/runtime.h"
#include "unit/scene/common/colosso_item_icon.h"
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
#include "field_service.h"
#include "flag_ids.h"
#include "text/msg_ids.h"
#include "battle_unit.h"
#include "resource.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "field_obj.h"
#include "heap_state.h"

// 前方宣言
static void FieldScene_RunMultiPhaseActorSequence(s32 a0);
static void Colosso_RunGreetScene(s32 a0);
static void ColossoLogRollingStage_RunScriptedTransition(s32 mode);
static void ColossoLogRollingStage_NoopSceneHook(void);
static void ColossoLogRollingStage_WaitForBalanceState(void);
static s32 ColossoLogRollingStage_PositionActiveActor(s32 first_handle, s32 second_handle);
static void ColossoLogRollingStage_PushStagedActor(void);

typedef struct SceneControl
{
	s16 enabled;
	s16 active;
	s16 sceneVariant;
	s16 phase;
	s16 timer;
} SceneControl;

typedef struct LogSceneRecord
{
	u8	pad00[6];
	u16 facing;
	s32 x;
	s32 y;
	s32 z;
	u8	pad14[14];
	u8	state;
	u8	pad23;
	s32 motion24;
	u8	pad28[4];
	s32 motion2c;
	s32 rateX;
	s32 rateZ;
	u8	pad38[33];
	u8	flags;
} LogSceneRecord;

typedef struct Position3
{
	s32 x;
	s32 y;
	s32 z;
} Position3;

typedef struct StageObstacleActor
{
	u8	unk00[6];
	u16 attributes;
	s32 x;
	s32 y;
	s32 z;
	u8	unk14[15];
	u8	directionAndKind;
	u8	unk24[12];
	s32 moveRateX;
	s32 moveRateZ;
	u8	unk38[29];
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
	s32 scaleX;
	s32 scaleZ;
	u8	pad20[0x10];
	s32 moveRateX;
	s32 moveRateZ;
} PrimaryStageObject;

typedef struct SecondaryStageObject
{
	u8	filler00[8];
	s32 x;
	u8	filler0C[4];
	s32 z;
	u8	filler14[0x1c];
	s32 moveRateX;
	s32 moveRateZ;
	u8	filler38[0x1d];
	u8	state;
} SecondaryStageObject;

typedef struct StageMotionEffect
{
	u8	filler00[0x30];
	s32 moveRateX;
	s32 moveRateZ;
	u8	filler38[0x1d];
	u8	state;
} StageMotionEffect;

typedef struct PartyInteractionRecord
{
	u8	padding00[10];
	s16 x;
	u8	padding0c[6];
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

// コロッセオ共通 (scene/common/colosso_*.c)

u8 *ColossoLogRollingStage_GetScriptData(void)
{
	return (u8 *)gColossoLogEntrances;
}

s32 ColossoLogRollingStage_GetMessageData(void)
{
	return 0;
}

u8 *ColossoLogRollingStage_GetActorData(void)
{
	return (u8 *)gColossoLogExits;
}

u8 *ColossoLogRollingStage_GetEffectData(void)
{
	return (u8 *)gColossoLogPlacements;
}

static void ColossoLogRollingStage_SceneTask(void)
{
	struct EventWork  *work = gEventWork;
	struct FieldActor *actor = Object_GetById(gGameState.selectedActor);
	s32				   row = actor->z.fixed >> 20;
	s32				   i;
	s32				   kind;

	if (gColossoSceneTaskState == 0)
	{
		gColossoSceneTaskStatus = (gColossoSceneTaskStatus + 1) & 3;
		for (i = 18; i <= 22; i++)
		{
			kind = gColossoLog_LogPattern[gColossoSceneTaskStatus][i - 18];
			Engine_ActorSetAnimation(i, kind);
			Engine_ActorSetAnimation(i + 5, kind + 8);
			Engine_MapCopyCellAttributes(32, 11, 1, 2, (i - 18) * 2 + 33, 11);
			if (kind != 7)
			{
				Engine_MapCopyCellAttributes(74, 12, 1, 1, (i - 18) * 2 + 33, 11);
			}
		}
		Engine_ActorSetAnimation(28, gColossoLog_LogPattern[gColossoSceneTaskStatus][5]);
	}
	else
	{
		for (i = 18; i <= 22; i++)
		{
			kind = gColossoLog_LogPattern[gColossoSceneTaskStatus][i - 18];
			if ((u32)(actor->x.fixed - (i << 21) + 0x31ffff) <= 0x13fffe)
			{
				if (row == 11 && kind == 4)
					work->raisedTrigger = kind;
				if (row == 12 && kind == 5)
					work->raisedTrigger = kind;
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
	FieldScheduler_AddCallback(ColossoLogRollingStage_SceneTask, 0xC80);
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
	s32					id = ((struct GameState *)table)->selectedActor;
	StageObstacleActor *subject =
		(StageObstacleActor *)((struct EventWork *)workspace)->viewCenter;
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

		Map_CopyCellAttributes(47, 61, 1, 4, 49, 61);
	}
}

static void EV09_SCRIPT(void)
{
	PrimaryStageObject *object;

	object = Object_GetById(9);
	object->scaleX = 0x10000;
	object->scaleZ = 0x10000;

	object = Object_GetById(11);
	object->moveRateZ = 0x6666;
	object->moveRateX = 0xCCCC;
	Engine_ObjectSetPosition(object, object->x, 0x200000, object->y);

	object = Object_GetById(10);
	object->moveRateZ = 0x6666;
	object->moveRateX = 0xCCCC;
	Engine_ObjectSetPosition(object, object->x, 0x40000, object->y);

	Engine_GameFlagSet(0x362);
	Map_CopyCellAttributes(15, 12, 1, 1, 13, 12);
	Map_CopyCellAttributes(14, 12, 1, 1, 9, 12);
}

static void EV15_TOUCH(void)
{

	s32				   actor;
	s32				   lowered;
	struct FieldActor *log;
	struct FieldActor *rider;
	u8				  *flags;

	actor = gGameState.selectedActor;
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
		log->motionFlags = lowered;
		log->acceleration = 0x6666;
		log->speed = 0xcccc;
		Engine_ObjectSetPosition(log, log->x.fixed, 0x200000, log->z.fixed);
		log = Object_GetById(10);
		log->motionFlags = lowered;
		log->acceleration = 0x6666;
		log->speed = 0xcccc;
		FieldObject_SetPosition(log, log->x.fixed, 0x40000, log->z.fixed);
		rider = Object_GetById(actor);
		flags = &rider->motionFlags;
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
		rider->terrainHeight = rider->y.fixed;
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
	object->moveRateZ = 0x6666;
	object->moveRateX = 0xcccc;
	Engine_ObjectSetPosition(object, object->x, 0x40000, object->z);

	object = Object_GetById(10);
	object->state = 0;
	object->moveRateZ = 0x6666;
	object->moveRateX = 0xcccc;
	Engine_ObjectSetPosition(object, object->x, 0x200000, object->z);

	Engine_ActorWaitForMove(10);
	{
		Map_CopyCellAttributes(0, 25, 1, 1, 9, 12);
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
			log->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			log->motionFlags = 0;
			log->acceleration = 0x6666;
			log->speed = 0xcccc;
			FieldObject_SetPosition(log, log->x.fixed, 0x40000, log->z.fixed);
			upper = Object_GetById(11);
			upper->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			upper->acceleration = 0x6666;
			upper->speed = 0xcccc;
			Engine_ObjectSetPosition(upper, upper->x.fixed, 0x200000, upper->z.fixed);
			lower = Object_GetById(10);
			lower->acceleration = 0x6666;
			lower->speed = 0xcccc;
			Engine_ObjectSetPosition(lower, lower->x.fixed, 0x40000, lower->z.fixed);
			Engine_GameFlagSet(0x368);
			Map_CopyCellAttributes(15, 12, 1, 1, 13, z);
			Map_CopyCellAttributes(1, 25, 1, 1, x, z);
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
	FieldGameFlag_SetByte(880, x);
	Map_CopyCellAttributes(18, 10, 3, 1, 18, 11);
	Map_CopyCellAttributes(17, 11, 1, 1, x, 11);
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
			actor->directionAndKind = 2;
			actor->state = 0;
			x = actor->x >> 20;
			z = actor->z >> 20;
			Map_CopyCellAttributes(83, 13, 1, 1, x, z);
			x2 = actor->x >> 20;
			z2 = actor->z >> 20;
			Map_CopyCellAttributes(83, 13, 1, 1, x2, z2 + 52);
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
	s32					messageId;

	table = (s16 *)&gGameState;
	actor = Object_GetById(*(s32 *)&table[250]);
	x = actor->x >> 20;
	messageId = 23;
	z = actor->z >> 20;
	if (x == 81 && z == 12)
	{
		if ((actor->attributes & 0xE000) == 0x4000)
		{
			messageId = 253;
		}
		SetMapCellCollision(0, x << 20, z << 20, messageId);
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

	rider = Object_GetById(gGameState.selectedActor);
	log = Object_GetById(31);
	steps = 0;
	angle = gColossoEarlySequenceData[rider->facing >> 13];
	actor = gGameState.selectedActor;
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
			if (ColossoLogRollingStage_CheckPathClearance(next_x, z) == 0)
			{
				steps++;
				x = next_x;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		if (!(gKeysHeld & KEY_RIGHT))
		{
			return;
		}
		mode = 3;
		step = 8;
		for (;;)
		{
			next_x = x + 0x100000;
			if (ColossoLogRollingStage_CheckPathClearance(next_x, z) == 0)
			{
				steps++;
				x = next_x;
			}
			else
			{
				break;
			}
		}
	}
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
	ObjectDispatch_InitFromTable4WithArgument(work->viewCenter, log);
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
		log->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
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
	effect->moveRateZ = move_rate;
	effect->moveRateX = move_rate;
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
	actor->moveRateZ = 0x10000;
	actor->moveRateX = 0x20000;
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
	actor->moveRateZ = 0x10000;
	actor->moveRateX = 0x20000;
	Object_SetMode(actor, 5);
	z = actor->z & 0xFFF00000;
	Engine_ObjectSetPosition(actor, actor->x, actor->y, z + 0xC0000);
	Object_CommitPosition(actor);
	Engine_ActorSetAttachedEffect(*slot, 258);
	FieldBattleFx_RunRisingObjectSequence(*slot, 6, 0);
}

static void ColossoLogRollingStage_NoopSceneEventHook(void)
{
}

static void ColossoLog_RunCompetitorTalk(s32 a0)
{
	struct EventWork *work;
	s32				  owner;
	s32				  lines;
	s32				  entrance;

	work = gEventWork;
	owner = gGameState.selectedActor;
	entrance = gGameState.entrance;
	if (entrance == 2)
	{
		Engine_EventBegin();
		lines = a0 * 3;
		Engine_EventSetMessage(lines + MsgColossoDidntThinkBattles);
		Engine_EventOpenMessage(a0, 0);
		if (Engine_EventChooseYesNo(owner, 0) == 0)
		{
			Engine_EventSetMessage(lines + MsgColossoDidntThinkBattles + 1);
			Engine_EventShowMessage(a0, 0);
			work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
			work->transitionFrames = 15;
			Engine_EventCloseScreen();
			Engine_EventWaitForScreen();
			Colosso_SelectSoloCompetitor(a0);
			Engine_EventOpenScreen();
			Engine_EventWaitForScreen();
		}
		else
		{
			Engine_EventSetMessage(lines + MsgColossoDidntThinkBattles + 2);
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
	Party_SetFields1ceAnd1d0((s32)&SceneId_ColossoLog, 4);
	Event_SetDefeatDestination((s32)&SceneId_ColossoLog, 5);
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
	return (s32)gColossoLogEvents;
}

s32 StageSetup_BuildAndDispatch(void)
{
	struct FieldActor *actor;
	s32				   i;
	s32				   height;
	s32				   column;

	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_GameFlagSet(324);
	FieldScheduler_AddCallback(ColossoLogRollingStage_ShowActorPositionMessage, 3200);
	Map_CopyCellAttributes(74, 60, 8, 6, 120, 60);

	Engine_ActorSetSpriteFlags(Object_GetById(9), 0);

	actor = Object_GetById(10);
	Engine_ActorSetSpriteFlags(actor, 0);
	actor->motionFlags = 0;
	actor->y.fixed = 32 << 16;

	actor = Object_GetById(11);
	Engine_ActorSetSpriteFlags(actor, 0);
	actor->motionFlags = 0;
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
		actor->scaleX = 0x18000;
		actor->scaleY = 0x18000;
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
		Map_CopyCellAttributes(15, 12, 1, 1, 13, 12);
		Map_CopyCellAttributes(1, 25, 1, 1, 9, 12);
		actor = Object_GetById(12);
		Engine_ActorSetSpriteFlags(actor, 0);
		actor->motionFlags = 0;
		actor->y.fixed = 2 << 16;
		actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		actor = Object_GetById(10);
		actor->y.fixed = 4 << 16;
		actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		actor = Object_GetById(11);
		actor->y.fixed = 32 << 16;
	}

	column = FieldGameFlag_GetByte(880);
	if (column == 0)
	{
		column = 19;
	}
	actor = Object_GetById(13);
	actor->x.fixed = (column << 20) + (8 << 16);
	actor->motionFlags = 0;
	actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	Map_CopyCellAttributes(18, 10, 3, 1, 18, 11);
	Map_CopyCellAttributes(17, 11, 1, 1, column, 11);

	for (i = 15; i <= 17; i++)
	{
		actor = Object_GetById(i);
		height = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
		if (actor->y.fixed == 0 && height == 0)
		{
			actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			actor->motionFlags = height;
			Map_CopyCellAttributes(83, 13, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
			Map_CopyCellAttributes(83, 13, 1, 1, actor->x.fixed >> 20, (actor->z.fixed >> 20) + 52);
		}
	}

	if (Engine_GameFlagIsSet(865) != 0)
	{
		for (i = 18; i <= 22; i++)
		{
			actor = Object_GetById(i);
			actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			Object_SetMode(actor, 2);
			actor = Object_GetById(i + 5);
			actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			actor->motionFlags = 0;
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
			actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			actor = Object_GetById(i + 5);
			actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			actor->motionFlags = 0;
			actor->y.fixed = 32 << 16;
		}
		FieldScheduler_AddCallback(ColossoLogRollingStage_SceneTask, 3200);
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
		actor->motionFlags = 0;
		actor->x.fixed = 1130 << 16;
		actor->z.fixed = 184 << 16;
		Engine_ActorSetSpriteFlags(actor, 0);
		Object_SetMode(actor, 3);
		Engine_ObjectSetScript(actor, gColossoLog_Record30Script);
	}
	else
	{
		Map_SetLayerEntryFlag(2);
	}

	if (Engine_GameFlagIsSet(873) != 0)
	{
		actor = Object_GetById(31);
		Object_SetMode(actor, 8);
		actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		Map_CopyCellAttributes(86, 10, 1, 2, 84, 10);
		Map_CopyCellAttributes(86, 9, 1, 1, 84, 12);
	}
	else
	{
		actor = Object_GetById(31);
		Map_CopyCellAttributes(85, 9, 1, 4, actor->x.fixed >> 20, 9);
		Map_CopyCellAttributes(85, 9, 1, 4, actor->x.fixed >> 20, 61);
	}

	actor = Object_GetById(9);
	actor->motionFlags = 0;
	actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	actor = Object_GetById(10);
	actor->motionFlags = 0;
	actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	actor = Object_GetById(11);
	actor->motionFlags = 0;
	actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	Engine_ActorSetAnimation(8, 9);
	gGameState.movementMode = 0;
	Colosso_ShowItemIcon(39, 3);
	Colosso_ShowItemIcon(40, 17);
	Engine_ActorSetChildValue(8, 2);

	switch (gGameState.entrance)
	{
	case 1:
		FieldScene_BuildSceneDescriptorAndInstallTask(0, 8, 6, 1512 << 16, 192 << 16, 39, 40);
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
			Colosso_SelectSoloCompetitor(0);
			SceneActor_PlacePartyAtSavedTiles();
			Object_LinkObjectAndSetCallback(1, 0);
			ColossoLogRollingStage_RunScriptedTransition(3);
		}
		Object_LinkObjectAndSetCallback(1, 0);
		Object_LinkObjectAndSetCallback(2, 0);
		Object_LinkObjectAndSetCallback(3, 0);
		SceneState_InitControlWhenFlag109Clear((s32)&ResourceId_RivalPathC);
		break;

	case 2:
		FieldScheduler_AddCallback(SceneState_RaiseTriggerOnPsynergyTarget, 3200);
		Engine_ActorDestroy(39);
		Engine_ActorDestroy(40);
		if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0)
		{
			break;
		}
		SceneActor_PlacePartyAtSavedTiles();
		Colosso_SelectSoloCompetitor(1);
		ColossoLogRollingStage_RunScriptedTransition(0);
		break;

	case 3:
		if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0)
		{
			break;
		}
		Colosso_RunGreetScene(32);
		Colosso_ClearSavedActorPositions();
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
	particle->x += particle->velocityX << 8;
	particle->y += particle->velocityY << 8;
	particle->scaleX += 0x666;
	particle->scaleY += 0x666;
	particle->velocityX += 5;
	particle->velocityY -= 1;
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
	particle->scaleX = 0x6666;
	particle->scaleY = 0x6666;
	{
		s32 t = 0x40000;
		particle->x = x + t;
		t += y;
		particle->y = t;
		particle->anchorY = t;
	}
	particle->velocityX = 25;
	particle->velocityY = 128;
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
	Engine_ActorEnableActionCallback(8, gColossoLog_Actor8Action);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x5e00000, 0xc00000);
	record = Object_GetById(0);
	{
		u16 shown = 0;

		record->facing = shown;
	}
	Engine_ActorEnableActionCallback(0, gColossoLog_LeaderActionA);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 35);
	Actor_SetSpeed(1, 0x10000, 0x8000);
	Actor_SetSpeed(IVAN, 0x10000, 0x8000);
	Actor_SetSpeed(MIA, 0x10000, 0x8000);
	Actor_SetPosition(GARET, 0x5b80000, 0xb80000);
	Actor_SetPosition(IVAN, 0x5b80000, 0xc80000);
	Actor_SetPosition(MIA, 0x5a80000, 0xc00000);
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
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	ColorBuffer_ApplyTarget(0x10001, 1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventSetMessage(MsgColossoIsaac);
	Engine_EventWait(60);
	data_table_addr = (s32)gColossoMultiPhaseData;
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, data_table_addr);
	record = Object_GetById(0);
	record->scaleX = 0x10000;
	record = Object_GetById(0);
	record->scaleY = 0x10000;
	Engine_ActorSetAnimationAndWait(0, 36);
	record = Object_GetById(0);
	record->x.fixed += 0x30000;
	Engine_EventWait(10);
	record = Object_GetById(0);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(0, gColossoLog_LeaderActionB);
	Event_ShowMessage(GARET, 0);
	Engine_EventWait(20);
	Actor_WalkToAndWait(1, 0x5e0, 176);
	Actor_FaceDirection(GARET, 0x4000, 10);
	Engine_ActorShowEmote(1, 0x100, 20);
	Event_ShowMessage(IVAN, 0);
	Object_LinkObjectAndSetCallback(1, 2);
	Engine_EventWait(30);
	Actor_WalkToAndWait(IVAN, 0x5d0, 176);
	Actor_WalkTo(GARET, 0x5f0, 184);
	Actor_WalkToAndWait(IVAN, 0x5e0, 176);
	Engine_ActorSetAnimation(GARET, 1);
	Actor_FaceDirection(GARET, 0x6000, 0);
	Actor_FaceDirection(IVAN, 0x4000, 10);
	Engine_ActorRunRepeatedMotion(IVAN, 2);
	Engine_EventWait(10);
	Object_LinkObjectAndSetCallback(2, 1);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(1, 4);
	Engine_EventWait(30);
	Event_ShowMessage(MIA, 0);
	Object_LinkObjectAndSetCallback(1, 3);
	Object_LinkObjectAndSetCallback(2, 3);
	Actor_WalkToAndWait(MIA, 0x5d0, 184);
	Object_LinkObjectAndSetCallback(2, 0);
	Object_LinkObjectAndSetCallback(1, 0);
	Actor_ShowEmote(GARET, 0x102, 60);
	Object_LinkObjectAndSetCallback(2, 1);
	Object_LinkObjectAndSetCallback(1, 2);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Engine_EventWait(10);
	Object_LinkObjectAndSetCallback(2, 3);
	Object_LinkObjectAndSetCallback(1, 3);
	Engine_EventWait(20);
	Event_ShowMessage(MIA, 0);
	Engine_ActorEnableActionCallback(0, gColossoLog_LeaderActionC);
	Engine_ActorRunRepeatedMotion(GARET, 2);
	Object_LinkObjectAndSetCallback(1, 0);
	Engine_EventWait(20);
	Event_ShowMessage(GARET, 0);
	Engine_ActorRunRepeatedMotion(IVAN, 2);
	Object_LinkObjectAndSetCallback(2, 0);
	Engine_EventWait(20);
	Event_ShowMessage(IVAN, 0);
	Engine_ActorRunRepeatedMotion(MIA, 2);
	Object_LinkObjectAndSetCallback(3, 0);
	Engine_EventWait(20);
	Event_ShowMessage(MIA, 0);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, data_table_addr);
	Engine_EventWait(60);
	callback_target = (s32)ColossoLogRollingStage_SpawnPeriodicParticle;
	gColossoParticleCount = 9;
	Task_AddCallback((void (*)(void))callback_target, 0xc80);
	Engine_EventWait(5);
	Engine_TaskRemoveCallback(callback_target);
	Engine_EventWait(55);
	Actor_ShowEmote(GARET, 0x101, 60);
	Task_AddCallback((void (*)(void))callback_target, 0xc80);
	Engine_EventWait(20);
	Engine_TaskRemoveCallback(callback_target);
	Engine_EventWait(40);
	Actor_ShowEmote(IVAN, 0x101, 60);
	gColossoParticleCount = 9;
	Task_AddCallback((void (*)(void))callback_target, 0xc80);
	Engine_EventWait(35);
	Engine_TaskRemoveCallback(callback_target);
	Engine_EventWait(25);
	Actor_ShowEmote(MIA, 0x102, 60);
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
	Actor_ShowEmote(MIA, 0x108, 60);
	Engine_ActorStartRepeatedMotion(GARET, 3);
	Engine_ActorStartRepeatedMotion(2, 3);
	Engine_ActorRunRepeatedMotion(3, 3);
	Object_LinkObjectAndSetCallback(3, 2);
	Object_LinkObjectAndSetCallback(1, 2);
	gColossoParticleCount = 9;
	Task_AddCallback((void (*)(void))callback_target, 0xc80);
	Engine_ActorSetAnimation(GARET, 3);
	Engine_ActorSetAnimation(IVAN, 3);
	Engine_ActorSetAnimation(MIA, 3);
	Engine_EventWait(60);
	Actor_WalkTo(MIA, 0x5b8, 200);
	Engine_EventWait(5);
	Actor_WalkTo(IVAN, 0x558, 184);
	Engine_EventWait(3);
	Actor_WalkToAndWait(GARET, 0x5e8, 184);
	Actor_WalkTo(GARET, 0x558, 184);
	Engine_ActorWaitForMove(MIA);
	Engine_ActorSetAnimation(3, 1);
	Object_LinkObjectAndSetCallback(3, 0);
	Engine_EventWait(60);
	Actor_WalkToAndWait(MIA, 0x598, 200);
	Actor_WalkTo(MIA, 0x558, 184);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(30);
	Actor_SetPosition(GARET, 0x5e80000, 0xb00000);
	Actor_SetPosition(IVAN, 0x5b80000, 0xc00000);
	Actor_SetPosition(MIA, 0x6180000, 0xc80000);
	Graphics_EnableObjLayerAndCallbacks();
	ColorBuffer_ApplyTarget(0x10000, 2);
	Engine_ColorBufferInterpolate(1);
	Engine_EventSetMessage(MsgColossoIsaacFellAsleep);
	Event_ShowMessage(GARET, 0);
	Event_ShowMessage(IVAN, 0);
	Event_ShowMessage(MIA, 0);
	Engine_EventWait(60);
	Engine_EventEnd();
}

static void Colosso_RunGreetScene(s32 a0)
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
	Engine_EventSetMessage(MsgColossoSiteThirdFinals);
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
		Colosso_FinishSoloRound(scene);
		return;
	}
	Engine_EventBegin();
	state = SceneDialogue_RunFlagGatedPromptInteraction(scene, 1);
	if (state == 0)
	{
		Engine_EventSetMessage(MsgColossoStageCalledScales);
		Engine_CameraSetSpeed(3 << 16, 0x6000);
		Engine_CameraMoveTo(152 << 16, -1, 200 << 16, 1);
		Engine_CameraWaitForMove();
		Engine_EventWait(30);
		Engine_EventShowMessage(scene, 0);
		SceneEffect_SeedMarkerAndInstallTask(104, 68, 0);
		Engine_EventWait(60);
		SceneState_StartMarkerMove(168, 96, 10);
		Engine_EventWait(70);
		Engine_EventShowMessage(scene, 0);
		SceneEffect_RemoveMarkerTask();
		Engine_TaskWait(2);
		pan1 = Object_GetById(10);
		pan1->motionFlags = 0;
		pan1->acceleration = 0x6666;
		pan1->speed = 0xcccc;
		FieldObject_SetPosition(pan1, pan1->x.fixed, 4 << 16, pan1->z.fixed);
		pan2 = Object_GetById(11);
		pan2->motionFlags = 0;
		pan2->acceleration = 0x6666;
		pan2->speed = 0xcccc;
		FieldObject_SetPosition(pan2, pan2->x.fixed, 32 << 16, pan2->z.fixed);
		Object_CommitPosition(pan2);
		Engine_EventWait(45);
		back1 = Object_GetById(10);
		back1->motionFlags = 0;
		back1->acceleration = 0x6666;
		back1->speed = 0xcccc;
		Engine_ObjectSetPosition(back1, back1->x.fixed, 32 << 16, back1->z.fixed);
		back2 = Object_GetById(11);
		back2->motionFlags = 0;
		back2->acceleration = 0x6666;
		back2->speed = 0xcccc;
		Engine_ObjectSetPosition(back2, back2->x.fixed, 4 << 16, back2->z.fixed);
		Object_CommitPosition(back2);
		Engine_EventWait(15);
		Engine_EventShowMessage(scene, 0);
		SceneEffect_SeedMarkerAndInstallTask(104, 68, 0);
		Engine_EventWait(30);
		SceneState_StartMarkerMove(168, 96, 10);
		Engine_EventWait(40);
		SceneState_StartMarkerMove(104, 68, 10);
		Engine_EventWait(70);
		Engine_EventShowMessage(scene, 0);
		SceneEffect_RemoveMarkerTask();
		Engine_TaskWait(2);
		Engine_CameraFollowActor(0, 0);
		Colosso_ShowFollowUpPrompt(scene, 1);
	}
	else if (state == 1)
	{
		Engine_EventSetMessage(MsgColossoObjectiveGetAcross);
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
		Colosso_FinishSoloRound(scene);
		return;
	}
	Engine_EventBegin();
	state = SceneDialogue_RunFlagGatedPromptInteraction(scene, 2);
	if (state == 0)
	{
		Engine_EventSetMessage(MsgColossoSteppingStoneStage);
		Camera_SetSpeed(3 << 16, 0x6000);
		Camera_MoveTo(188 << 17, -1, 152 << 16, 1);
		x = 280;
		Engine_CameraWaitForMove();
		Engine_EventWait(30);
		Event_ShowMessage(scene, 0);
		Colosso_FadeInCompetitor(0, x, 200);
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
		OverlayObject_PlaceWithScale14000(0, x, 152);
		OverlayObject_PlaceWithScale14000(0, 296, 152);
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
		SceneEffect_SeedMarkerAndInstallTask(96, 40, 0);
		SceneState_StartMarkerMove(128, 40, 10);
		Engine_EventWait(30);
		SceneState_StartMarkerMove(160, 40, 10);
		Engine_EventWait(30);
		SceneState_StartMarkerMove(160, 72, 10);
		Engine_EventWait(30);
		Event_ShowMessage(scene, 0);
		SceneEffect_RemoveMarkerTask();
		Colosso_RestoreCompetitor(0);
		Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
		Colosso_ShowFollowUpPrompt(scene, 2);
	}
	else if (state == 1)
	{
		Engine_EventSetMessage(MsgColossoYourGoalInStageSimple);
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
		Colosso_FinishSoloRound(scene);
	}
	else
	{
		Engine_EventBegin();
		state = SceneDialogue_RunFlagGatedPromptInteraction(scene, 3);
		if (state == 0)
		{
			work = gEventWork;
			Engine_EventSetMessage(MsgColossoPlaceCalledWall);
			ColossoLogRollingStage_ResetAndRunSceneTask();
			Engine_CameraSetSpeed(3 << 16, 0x6000);
			Engine_CameraMoveTo(154 << 18, -1, 184 << 16, 1);
			Engine_CameraWaitForMove();
			Engine_EventWait(30);
			Event_ShowMessage(scene, 0);
			ColossoLogRollingStage_StartSceneTask();
			Engine_EventWait(60);
			Engine_EventShowMessage(scene, 0);
			Colosso_FadeInCompetitor(0, 504, 200);
			/* FAKEMATCH: called through a cast to the s32 call type the bytes show, its result
			 * discarded (as call.h's Value3 did, 2026-10-13); Call3 changes argument allocation.
			 * 2026-10-13: the scene layer's Actor_FaceDirection loads r0 before r1
			 * and r2, where native loads it last (differ 2). */
			((s32 (*)())Engine_ActorFaceDirection)(0, 0, 0);
			ColossoLogRollingStage_WaitForSceneTask();
			Actor_SetSpeed(0, 0x18000, 0xc000);
			SceneActor_StartMode5MoveToTile(0, 680, 200);
			if (work->raisedTrigger != 5)
			{
				do
				{
					Engine_TaskWait(1);
					if (++state > 239)
					{
						break;
					}
				} while (work->raisedTrigger != 5);
			}
			EV05_RAISED();
			Actor_FaceDirection(0, 0xc000, 20);
			Actor_ShowEmote(0, 0x103, 60);
			Engine_EventShowMessage(scene, 0);
			Colosso_RestoreCompetitor(0);
			Engine_CameraFollowActor(0, 0);
			Colosso_ShowFollowUpPrompt(scene, 3);
			work->raisedTrigger = 0;
		}
		else if (state == 1)
		{
			Engine_EventSetMessage(MsgColossoOperatorWallsCheer);
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
		Colosso_FinishSoloRound(event);
	}
	else
	{
		Engine_EventBegin();
		result = SceneDialogue_RunFlagGatedPromptInteraction(event, 4);
		if (result == 0)
		{
			Engine_EventSetMessage(MsgColossoPlaceNormallyCalledFreeClimb);
			Camera_SetSpeed(0x30000, 0x6000);
			Camera_MoveTo(0x3580000, -1, 0xa80000, 1);
			Engine_CameraWaitForMove();
			Engine_EventWait(30);
			Event_ShowMessage(event, 0);
			Event_ShowMessage(event, 0);
			Colosso_FadeInCompetitor(0, 0x330, 200);
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
			Colosso_RestoreCompetitor(0);
			Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
			Colosso_ShowFollowUpPrompt(event, 4);
		}
		else if (result == 1)
		{
			Engine_EventSetMessage(MsgColossoClearStageMustAbleChange);
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
		Colosso_FinishSoloRound(event);
	}
	else
	{
		Engine_EventBegin();
		result = SceneDialogue_RunFlagGatedPromptInteraction(event, 5);
		if (result == 0)
		{
			Engine_EventSetMessage(MsgColossoCalledMovingSidewalkStage);
			Camera_SetSpeed(0x30000, 0x6000);
			Camera_MoveTo(0x4380000, -1, 0xa80000, 1);
			Engine_CameraWaitForMove();
			Engine_EventWait(30);
			Event_ShowMessage(event, 0);
			Event_ShowMessage(event, 0);
			Colosso_FadeInCompetitor(0, 0x3d8, 184);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
			ColossoLogRollingStage_PositionAndActivateActor(0, 0x3e0, 184);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
			ColossoLogRollingStage_PositionActor(0, 0x460, 184);
			Engine_EventWait(120);
			Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
			Engine_EventWait(120);
			OverlayObject_ResetMotionFields(0);
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
			OverlayObject_ResetMotionFields(0);
			gGameState.movementMode = 1;
			Colosso_RestoreCompetitor(0);
			Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
			Colosso_ShowFollowUpPrompt(event, 5);
		}
		else if (result == 1)
		{
			Engine_EventSetMessage(MsgColossoInStageMustTryOutpace);
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
		Colosso_FinishSoloRound(actor);
		return;
	}

	Engine_EventBegin();
	state = SceneDialogue_RunFlagGatedPromptInteraction(actor, 6);

	if (state == 0)
	{
		Engine_EventSetMessage(MsgColossoAnotherLogRollingArea);
		Engine_CameraSetSpeed(0x30000, 0x6000);
		Engine_CameraMoveTo(0x5080000, -1, 0x980000, 1);
		Engine_CameraWaitForMove();
		Engine_EventWait(30);
		Engine_EventShowMessage(actor, 0);
		SceneEffect_SeedMarkerAndInstallTask(0xb4, 0x58, 0);
		Engine_EventWait(60);
		Engine_EventShowMessage(actor, 0);
		SceneState_StartMarkerMove(0x20, 0x54, 10);
		Engine_EventWait(30);
		Engine_EventShowMessage(actor, 0);
		SceneState_StartMarkerMove(0x60, 0x54, 30);
		Engine_EventWait(60);
		Engine_EventShowMessage(actor, 0);
		SceneEffect_RemoveMarkerTask();
		Engine_EventWait(2);
		Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
		Colosso_ShowFollowUpPrompt(actor, 6);
	}
	else if (state == 1)
	{
		Engine_EventSetMessage(MsgColossoHereMustFigureOutHow);
		Engine_EventShowMessage(actor, 0);
	}

	FieldScene_RunMiddleSequence(state, actor, 6);
	Engine_EventEnd();
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/colosso_solo.h"
#include "common/item_equip.h"
#include "common/colosso_task.h"

static void ColossoLogRollingStage_InitializeModeTask(u32 mode, u32 parameter)
{
	s32 handler;

	gColosso_ModeTaskMode = mode;
	gColosso_ModeTaskParam = (parameter << 4);

	{
		s32 budget = 0xc80;
		s32 task = (s32)Colosso_UpdateModeTask;
		FieldScheduler_AddCallback(task, budget);
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

	gColosso_ModeTaskTimer = 0;
	gColosso_ModeTaskScript = (s16 *)handler;
	gColosso_ModeMoveTarget = 0;
	gColosso_ModeMoveDuration = 0;
	gColosso_ModeTaskPosition = 0;
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
		Colosso_LoadPortrait(0);
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
	Colosso_LoadPortrait(mode);
	ColossoLogRollingStage_InitializeModeTask(1, 0);
	Engine_EventWait(120);

	while (AudioCommand_GetStateByte() != 0)
	{
		Engine_TaskWait(1);
	}

	Engine_AudioPlayCue(0x121);
	Colosso_LoadPortrait(5);
	ColossoLogRollingStage_InitializeModeTask(2, 0);
	Engine_AudioPlayCue(0xec);
	Engine_EventWait(60);
	ColossoLogRollingStage_InitializeModeTask(2, 1);
	Engine_AudioPlayCue(0xec);
	Engine_EventWait(60);
	Colosso_LoadPortrait(6);
	ColossoLogRollingStage_InitializeModeTask(2, 0);
	Engine_AudioPlayCue(0xec);
	Engine_EventWait(60);
	Colosso_LoadPortrait(7);
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
		Colosso_LoadPortrait(8);
		/* FAKEMATCH: called through a cast to the s32 call type the bytes show, its result
		 * discarded (as call.h's Value2 did, 2026-10-13); Call2 changes argument allocation.
		 * 2026-10-13: called directly, as elsewhere in this file, each of these two
		 * sites loads r0 before r1, where native loads r1 first (differ 4). */
		((s32 (*)())ColossoLogRollingStage_InitializeModeTask)(3, 1);
		Engine_EventWait(-a0 * 60 + 60);
		kind = 0;
	}
	else
	{
		Engine_EventWait(30);
		Engine_AudioPlayCue(a0 + 90);
		Colosso_LoadPortrait(4);
		/* FAKEMATCH: called through the s32 cast, its result discarded, as in the first arm. */
		((s32 (*)())ColossoLogRollingStage_InitializeModeTask)(3, 0);
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
#include "common/colosso_marker.h"
#include "common/colosso_item_icon.h"

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
	u32 randomValue;

	if (source->verticalMotion >= -255 && source->verticalMotion <= 255)
	{
		source->state = 0;
	}
	randomValue = Engine_RandomNext();
	if (randomValue * 100 >> 16 <= 9)
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

static s32 ColossoLogRollingStage_RaiseLinkedSceneEffect(LinkedStageEffect *source)
{

	LinkedStageEffect *effect = Object_GetById(source->linkedEffectSlot);

	Engine_ObjectSetPosition(effect, source->x, source->y + 0x240000, source->z);
	effect->state = 0;
	Engine_ObjectSetScript(effect, gColossoLinkedEffect);
	Engine_AudioPlayCue(83);
	source->linkedEffectSlot = 0;
	return 0;
}

static s32 ColossoLogRollingStage_PositionActiveActor(s32 first_handle, s32 second_handle)
{
	u8				  *work = gColossoWork;
	struct FieldActor *actor;
	s32				   flag;
	s32				   x;
	s32				   z;
	u16				  *cue;
	s16				  *wait;

	flag = Engine_GameFlagIsSet(0x211);
	actor = Object_GetById(gGameState.selectedActor);
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
	Engine_ObjectSetScript(actor, gColossoLog_PlaceScript);
	while (*wait != 0)
	{
		Engine_TaskWait(1);
	}
	if (flag == 0)
	{
		/* FAKEMATCH: the void function is called as returning s32, which loads r1
		 * before r0 as native. 2026-10-13: called by its prototype at both
		 * sites, each loads r0 first (differ 4). */
		((s32 (*)())SceneActor_ApplyValueAndMatchingSlots)(0, first_handle);
		UiWork_PushValueSlot(first_handle, 2);
	}
	else
	{
		/* FAKEMATCH: called as returning s32, as in the first arm. */
		((s32 (*)())SceneActor_ApplyValueAndMatchingSlots)(0, second_handle);
		UiWork_PushValueSlot(second_handle, 2);
	}
	UiWork_PushValueSlot(gGameState.selectedActor, 1);
	Engine_MessageShowCentered(MsgColossoIsaacGotItem, 3);
	ObjectDispatch_WaitForValue16(actor);
	return flag;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/colosso_path_rival.h"
#include "common/colosso_setup.h"
#include "common/colosso_scene_state.h"
#include "common/object_findslot.h"

static void ColossoLogRollingStage_PushStagedActor(void)
{

	LogSceneRecord *subject;
	LogSceneRecord *target;
	LogSceneRecord *blocker;
	u32				   step;
	u32				   direction;
	Position3		   position;
	u32				   data_index = 250;
	s32				   zero;
	s32				   subject_handle;

	subject_handle = *(s32 *)((u8 *)&gGameState + (data_index << 1));
	subject = Object_GetById(subject_handle);

	direction = subject->facing >> 12;

	step = gStagedActor_DirectionSteps[direction];
	position.x = subject->x + (step & 0xffff0000);
	position.y = subject->y;
	step <<= 16;
	position.z = subject->z + step;

	target = SceneData_FindSlotAtPosition(&position, subject);
	if (target == 0)
	{
		return;
	}

	step = gStagedActor_DirectionSteps[direction];
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

	step = gStagedActor_DirectionSteps[direction];
	position.x = target->x + (step & 0xffff0000);
	position.y = target->y;
	step <<= 16;
	position.z = target->z + step;

	if (FieldObject_CheckMovementCollision(target, &position) > 0)
	{
		return;
	}

	Object_SetMode(subject, 8);
	Engine_TaskWait(15);

	target->rateX = 0x3333;
	target->rateZ = 0x3333;
	Engine_ObjectSetPosition(target, position.x, position.y, position.z);

	subject->rateX = 0x3333;
	subject->rateZ = 0x3333;
	Engine_ObjectSetPosition(subject, position.x, position.y, position.z);

	Engine_AudioPlayCue(0xee);
	Object_CommitPosition(target);
	Engine_AudioPlayCue(0x120);

	target->x = position.x;
	target->z = position.z;
	target->motion24 = zero;
	target->motion2c = zero;

	Object_SetMode(subject, 1);
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/colosso_occupant.h"

const s32 gColossoLog_Record30Script[] = {
	SC_SET(SW_Y, PIXELS(1)),
	SC_WAIT(10),
	SC_SET(SW_Y, 0),
	SC_WAIT(10),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const s8 gColossoLog_LogPattern[][6] = {
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

const u8 gColosso_PortraitPaletteOffsets[] = {
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

u32 gStagedActor_DirectionSteps[] = {
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

struct SceneEntrance gColossoLogEntrances[] = {
	{ 0, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 136, 0, 392, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gColossoLogExits[] = {
	SCENE_EXITS(145),
	SCENE_EXIT(10, 145, 1),
	SCENE_EXIT(11, 140, 66),
	SCENE_EXIT(4, 138, 99),
	SCENE_EXIT(5, 138, 99),
	SCENE_EXITS_END,
};

struct ScenePlacement gColossoLogPlacements[] = {
	{ 0x0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ GARET, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ IVAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ MIA, CONDITION_ALWAYS, ACTOR_STAND,
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

struct SceneEvent gColossoLogEvents[] = {
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
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 32, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 33, CONDITION_ALWAYS, MSGNO(MsgColossoLogTheStreetVendorsCornOnThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 34, CONDITION_ALWAYS, MSGNO(MsgColossoLogICantSeeAnythingWhenIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 35, CONDITION_ALWAYS, MSGNO(MsgColossoLogItsNotVeryExcitingWorkBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 36, CONDITION_ALWAYS, MSGNO(MsgColossoLogStandingForLongPeriodsOfTime) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 37, CONDITION_ALWAYS, MSGNO(MsgColossoLogIWasUpAllNightTrying) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 38, CONDITION_ALWAYS, MSGNO(MsgColossoLogSomeTouristToldMeILook) },
	{ EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_RAISED) },
	{ SCENE_EVENTS_END },
};

s32 gColossoLog_Actor8Action[] = {
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

s32 gColossoLog_LeaderActionA[] = {
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

s32 gColossoLog_LeaderActionB[] = {
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

s32 gColossoLog_LeaderActionC[] = {
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

s16 gColosso_PortraitSlot = -1;

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

s16 gColosso_MarkerSlot = -1;

s32 gColosso_RivalFinishScript[] = {
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

s32 gColossoLog_PlaceScript[] = {
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
