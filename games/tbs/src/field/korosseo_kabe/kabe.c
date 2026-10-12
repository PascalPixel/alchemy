#include "game_state.h"
#include "io_write_queue.h"
#include "scene_ids.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/field/korosseo_kabe/kabe.h"
#include "unit/field/common/korosseo/saved.h"
#include "unit/field/common/korosseo/solo_competitor.h"
#include "unit/field/common/korosseo/raised.h"
#include "unit/field/common/korosseo/solo_round.h"
#include "unit/field/common/korosseo/cheer.h"
#include "unit/field/common/korosseo/compete.h"
#include "unit/field/common/korosseo/markslot.h"
#include "unit/field/common/korosseo/markmove.h"
#include "unit/field/common/korosseo/occupant.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field2.h"
#include "unit/field/common/script/field3.h"
#include "unit/field/common/script/field5.h"
#include "unit/field/common/script/field10.h"
#include "unit/game/character/chara.h"
#include "unit/game/character/chara2.h"
#include "unit/game/character/chara3.h"
#include "unit/game/character/chara4.h"
#include "unit/game/character/chara5.h"
#include "unit/game/character/chara6.h"
#include "unit/graphics/window/window.h"
#include "unit/graphics/window/window7.h"
#include "unit/graphics/window/window9.h"
#include "unit/menu/item/menu.h"
#include "unit/menu/item/menu2.h"
#include "unit/menu/item/menu3.h"
#include "unit/menu/item/menu4.h"
#include "unit/sound/sound.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/field/common/korosseo/item_icon.h"
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
#include "fx_scene.h"
#include "mem.h"
#include "resource.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "call.h"
#include "resource_ids.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "battle_unit.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fieldobj.h"
#include "global_cells.h"
#include "heap_state.h"

// 前方宣言
static void Korosseo_RunGreetScene(s32 a0);
static void KorosseoKabe_RunScriptedTransition(s32 mode);
static void FieldScene_RunLateSequence(s32 a0);
static void SceneState_EmptyHook(void);
static void SceneState_WaitUntilStatusNine(void);
static s32 FieldScene_RunFlag211ApproachScene(s32 handleA, s32 handleB);
static void SceneState_InitControlRecordAndStartTask(s32 resource);
static void StagedActor_PushActorAhead(void);

#define WORK_SLOT_STAGE 59

typedef struct MovedObject
{
	s32 f00;
	s32 f04;
	s32 f08;
	s32 f0c;
	s32 f10;
	u8	f14[28];
	s32 f30;
	s32 f34;
} MovedObject;

typedef struct PartyInteractionRecord
{
	u8	padding_00[10];
	s16 x;
	u8	padding_0c[6];
	s16 y;
} PartyInteractionRecord;

typedef struct KabeSceneRecord
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
} KabeSceneRecord;

typedef struct Position3
{
	s32 x;
	s32 y;
	s32 z;
} Position3;

struct StageControl
{
	s16 status;
	s16 f2;
	s16 f4;
	s16 f6;
	s16 f8;
};

static inline void InitializeActorZero(void)
{
	Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
	Engine_ActorSetSpeed(actorId, 0x10000, 0x8000);
}

// 戦闘開始の種類 (battle/party/party.c と同じ)
// 別ファイルの関数: 呼び出し側は宣言なし (暗黙の int) で呼んでいる

const struct SceneEntrance *Scene_GetEntrances(void)
{
	return gKorosseoKabeEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return gKorosseoKabeExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	return gKorosseoKabePlacements;
}

static void SceneData_NoOpHandler(void)
{
}

static void FieldScene_RunSingleStep(void)
{
	StagedActor_PushActorAhead();
}

static void EV11_PSYNERGY(void)
{
	struct FieldActor *work;
	s32				   v0;
	s32				   v1;

	work = Object_GetById(11);
	if ((work->x.fixed >> 20) == 36)
	{
		Engine_GameFlagSet(0x335);
		v0 = 0x23;
		v1 = 0x4D;
		Engine_MapCopyCellAttributes(0x23, 0x4E, 1, 1, v0, v1);
	}
	else
	{
		Engine_GameFlagClear(0x335);
		v0 = 0x23;
		v1 = 0x4D;
		Engine_MapCopyCellAttributes(0x22, 0x4D, 1, 1, v0, v1);
	}
}

static void FieldScene_RunTwoStepSequence(void)
{
	StagedActor_PushActorAhead();
	EV11_PSYNERGY();
}

static void SceneState_StoreSlotTileXToWork832To848(void)
{

	s32	 fifth = 14;
	s32	 sixth = 11;
	s32 *record;
	s32	 value;

	Engine_MapCopyCellAttributes(100, 11, 12, 4, fifth, sixth);

	record = (s32 *)Object_GetById(12);
	value = record[2] >> 20;
	GameFlag_SetByte(832, value);
	Engine_MapCopyCellAttributes(71, 16, 1, 1, value, 16);

	record = (s32 *)Object_GetById(13);
	value = record[2] >> 20;
	GameFlag_SetByte(840, value);
	Engine_MapCopyCellAttributes(71, 16, 1, 1, value, 16);

	record = (s32 *)Object_GetById(14);
	value = record[2] >> 20;
	GameFlag_SetByte(848, value);
	Engine_MapCopyCellAttributes(71, 16, 1, 1, value, 16);
}

static void EV18_SCRIPT(void)
{
	u8 *record = gEffectWork;

	((struct BattleFxScene *)record)->child_mode = 1;
}

static void EV18_PSYNERGY(void)
{
	struct FieldActor *rec2;
	s32				   frames;
	s32				   spin;
	s32				   frames2;
	s32				   spin2;
	u8				  *p6;

	rec2 = Object_GetById(30);
	p6 = (u8 *)rec2->sprite;
	GameFlag_Set(0x330);
	rec2->acceleration = 0x1999;
	rec2->speed = 0x13333;
	Engine_AudioPlayCue(227);
	FieldObject_SetPosition(rec2, 0x1500000, 0xa0000, 0x1080000);
	for (spin = 0, frames = 9; frames >= 0; frames--)
	{
		((struct FieldSprite *)p6)->rotation -= spin;
		Engine_TaskWait(1);
		spin += 36;
	}
	FieldObject_SetPosition(rec2, 0x14a0000, -0x100000, 0x1080000);
	for (spin2 = 0x168, frames2 = 21; frames2 >= 0; frames2--)
	{
		((struct FieldSprite *)p6)->rotation -= spin2;
		Engine_TaskWait(1);
		spin2 += 36;
	}
	Object_CommitPosition(rec2);
	Engine_EventWait(2);
	Engine_AudioPlayCue(240);
	{
		s32 shown = 0;

		((struct FieldSprite *)p6)->rotation = shown;
	}
	Engine_ActorSetAnimation(30, 4);
	rec2->x.fixed = 0x1500000;
	rec2->y.fixed = -0x80000;
	rec2->z.fixed = 0x1080000;
	rec2->velocity_y = 0;
	rec2->velocity_x = 0;
	Map_CopyCellAttributes(19, 16, 1, 1, 20, 16);
	Map_CopyCellAttributes(20, 80, 1, 1, 21, 80);
}

static void EV20_SCRIPT(void)
{
	u8 *p;

	Engine_GameFlagSet(0x331);
	p = (u8 *)Object_GetById(20) + 85;
	*p = 0;
	{
		s32 p5 = 44;
		s32 p6 = 17;

		Engine_MapCopyCellAttributes(46, 17, 1, 1, p5, p6);
	}
}

static void FieldScene_SetFlag332AndDrawTiles(void)
{
	u8 *slot;

	Engine_GameFlagSet(0x332);
	slot = (u8 *)Object_GetById(21) + 85;
	*slot = 0;
	{
		s32 v5 = 50;
		s32 v6 = 17;

		Engine_MapCopyCellAttributes(46, 17, 1, 1, v5, v6);
	}
}

static void EV19_PSYNERGY(void)
{
	Engine_GameFlagSet(0x333);
	{
		s32 width = 32;
		s32 height = 77;

		Engine_MapCopyCellAttributes(32, 37, 1, 4, width, height);
	}
}

static void KorosseoKabe_RunSelectedActorRisingEffect(void)
{
	BattleFx_RunRisingObjectSequence(gGameState.selected_actor, 6, 0);
}

static void KorosseoKabe_CheckLeaderAhead(void)
{
	Engine_LeaderCheckAhead();
}

static void SceneActor_MovePairByTileOffset(s32 a0, s32 a1, s32 a2)
{

	MovedObject *p;
	MovedObject *q;
	s32			 x;
	s32			 y;

	p = (MovedObject *)Object_GetById(gGameState.selected_actor);
	q = (MovedObject *)Object_GetById(a0);
	Engine_EventBegin();
	{
		x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
		y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

		p->f30 = 0x10000;
		p->f34 = 0x8000;
		Engine_ObjectSetPosition(p, x, p->f0c, y);
	}
	Object_SetMode(p, 27);
	{
		x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
		y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

		q->f30 = 0x10000;
		q->f34 = 0x8000;
		Engine_ObjectSetPosition(q, x, q->f0c, y);
	}
	if (a1 < 0 || a2 < 0)
	{
		Object_SetMode(q, 4);
	}
	else
	{
		Object_SetMode(q, 3);
	}
	Engine_AudioPlayCue(226);
	Object_CommitPosition(p);
	Engine_AudioPlayCue(288);
	Object_SetMode(q, 2);
	Engine_EventEnd();
}

static void EV22_SCRIPT(void)
{
	struct FieldActor *leader = Object_GetById(gGameState.selected_actor);
	s32				   x = leader->x.fixed >> 20;
	s32				   push = 0;
	s32				   block = 32;

	if (leader->z.fixed >> 20 > 12)
	{
		block = 33;
	}
	if (Object_GetById(block)->x.fixed >> 20 != x)
	{
		return;
	}
	if (x > 51)
	{
		if (gKeysHeld & KEY_LEFT)
		{
			push = -64;
		}
	}
	else if (gKeysHeld & KEY_RIGHT)
	{
		push = 64;
	}
	if (push != 0)
	{
		SceneActor_MovePairByTileOffset(block, push, 0);
		Engine_MapCopyCellAttributes(120, 10, 5, 6, 48, 10);
		x = Object_GetById(32)->x.fixed >> 20;
		Engine_MapCopyCellAttributes(52, 28, 1, 3, x, 10);
		x = Object_GetById(33)->x.fixed >> 20;
		Engine_MapCopyCellAttributes(52, 28, 1, 3, x, 13);
	}
}

static void KorosseoKabe_RollLogToCell(s32 id, s32 column, s32 row)
{
	s32				   pusher = gGameState.selected_actor;
	struct FieldActor *leader = Object_GetById(pusher);
	struct FieldActor *log = Object_GetById(id);
	s32				   heading = (leader->facing + 0x1000) & 0xe000;
	s32				   along_x = (log->x.fixed >> 20) != column / 2;
	s32				   dx;
	s32				   dz;

	column <<= 19;
	row <<= 19;
	if (along_x)
	{
		dx = (column - log->x.fixed) / 2;
		dz = 0;
	}
	else
	{
		dx = 0;
		dz = (row - log->z.fixed) / 2;
	}
	Engine_EventBegin();
	Engine_ActorSetAnimation(pusher, 8);
	Engine_EventWait(6);
	log->speed = 0x8000;
	log->acceleration = 0x3333;
	Object_SetMode(log, KorosseoKabe_RollLogScript[heading / 0x4000]);
	Engine_ObjectSetPosition(log, column, 0, row);
	Engine_EventWait(6);
	Engine_ActorSetAnimation(pusher, 2);
	ObjectDispatch_InitFromTable4WithArgument(
		*(struct DispatchObject **)((u8 *)Runtime_AllocateBlock(27, 0xccc) + 480), (s32)log);
	Engine_ActorSetSpeed(pusher, 0x8000, 0x3333);
	Object_SetMode(leader, 2);
	Engine_ObjectSetPosition(leader, leader->x.fixed + dx, 0, leader->z.fixed + dz);
	Engine_AudioPlayCue(239);
	Object_CommitPosition(leader);
	Object_SetMode(leader, 1);
	Object_CommitPosition(log);
	Engine_AudioPlayCue(288);
	Engine_AudioPlayCue(213);
	Object_SetMode(log, 1);
	Engine_EventWait(15);
	Engine_EventEnd();
}

static void SceneState_ApplyRectsForActors15To17(void)
{
	s32 field;

	Engine_MapCopyCellAttributes(100, 11, 12, 4, 14, 11);

	field = Object_GetById(15)->x.fixed >> 20;
	Engine_MapCopyCellAttributes(13, 28, 1, 4, field, 11);

	field = Object_GetById(16)->x.fixed >> 20;
	Engine_MapCopyCellAttributes(13, 28, 1, 4, field, 11);

	field = Object_GetById(17)->z.fixed >> 20;
	Engine_MapCopyCellAttributes(13, 28, 4, 1, 18, field);
}

static void EV24_TOUCH(void)
{
	KorosseoKabe_RollLogToCell(15, 29, 26);
	SceneState_ApplyRectsForActors15To17();
}

static void EV25_TOUCH(void)
{
	KorosseoKabe_RollLogToCell(15, 33, 26);
	SceneState_ApplyRectsForActors15To17();
}

static void EV28_TOUCH(void)
{
	KorosseoKabe_RollLogToCell(16, 45, 26);
	SceneState_ApplyRectsForActors15To17();
}

static void EV27_TOUCH(void)
{
	KorosseoKabe_RollLogToCell(16, 49, 26);
	SceneState_ApplyRectsForActors15To17();
}

static void FieldScene_RunStep17At40By23(void)
{
	KorosseoKabe_RollLogToCell(17, 40, 23);
	SceneState_ApplyRectsForActors15To17();
}

static void FieldScene_RunStep17At40By25(void)
{
	KorosseoKabe_RollLogToCell(17, 40, 25);
	SceneState_ApplyRectsForActors15To17();
}

static void FieldScene_RunSupplementalSequenceOne(void)
{
	s32				   i;
	struct FieldActor *rec8;
	struct FieldActor *rec7;
	s32				   xa;
	s32				   xb;
	s32				   xd;
	s32				   ya;
	s32				   yb;
	s32				   yd;
	struct FieldActor *record;

	rec8 = Object_GetById(gGameState.selected_actor);
	for (i = 22; i <= 25; i++)
	{
		rec7 = Object_GetById(i);
		rec7->paused = 0;
		xa = rec7->x.fixed;
		xb = rec8->x.fixed;
		xd = xa - xb;
		if (xd >= 0)
		{
			if (xd > 0x9ffff)
			{
				continue;
			}
		}
		else
		{
			xb -= xa;
			if (xb > 0x9ffff)
			{
				continue;
			}
		}
		ya = rec7->z.fixed;
		yb = rec8->z.fixed;
		yd = ya - yb;
		if (yd >= 0)
		{
			if (yd > 0x9ffff)
			{
				continue;
			}
		}
		else
		{
			yb -= ya;
			if (yb > 0x9ffff)
			{
				continue;
			}
		}
		if (GameFlag_IsSet(0x104) != 0)
		{
			yb = rec7->z.fixed;
		}
		else
		{
			yb = rec8->z.fixed;
			yd = rec7->velocity_z;
			yb += yd;
		}
		rec8->z.fixed = yb;
	}
	if (KorosseoKabe_SpectatorTimer != 0 && rec7->target_x == 0x80000000)
	{
		if (KorosseoKabe_SpectatorPhase == 0)
		{
			Map_CopyCellAttributes(58, 28, 7, 1, 58, 13);
		}
		else
		{
			Map_CopyCellAttributes(58, 10, 1, 1, 58, 11);
		}
	}
	else
	{
		Map_CopyCellAttributes(57, 11, 1, 1, 58, 11);
		Map_CopyCellAttributes(58, 14, 7, 1, 58, 13);
	}
	if (KorosseoKabe_SpectatorTimer == 0)
	{
		KorosseoKabe_SpectatorPhase ^= 1;
		if (KorosseoKabe_SpectatorPhase != 0)
		{
			record = Object_GetById(22);
			FieldObject_SetPosition(record, 0x3a80000, 0, 0xb80000);
			record = Object_GetById(23);
			FieldObject_SetPosition(record, 0x3c80000, 0, 0xf80000);
			record = Object_GetById(24);
			FieldObject_SetPosition(record, 0x3e80000, 0, 0xb80000);
			record = Object_GetById(25);
			FieldObject_SetPosition(record, 0x4080000, 0, 0xf80000);
			Engine_ActorSetAnimation(31, 11);
		}
		else
		{
			record = Object_GetById(22);
			FieldObject_SetPosition(record, 0x3a80000, 0, 0xd80000);
			record = Object_GetById(23);
			FieldObject_SetPosition(record, 0x3c80000, 0, 0xd80000);
			record = Object_GetById(24);
			FieldObject_SetPosition(record, 0x3e80000, 0, 0xd80000);
			record = Object_GetById(25);
			FieldObject_SetPosition(record, 0x4080000, 0, 0xd80000);
			Engine_ActorSetAnimation(31, 10);
		}
	}
	KorosseoKabe_SpectatorTimer++;
	if ((u32)KorosseoKabe_SpectatorTimer > 119)
	{
		if (GameFlag_IsSet(0x104) == 0)
		{
			KorosseoKabe_SpectatorTimer = 0;
		}
	}
}

static void FieldScene_PlaceSpectatorRow(void)
{
	KorosseoKabe_SpectatorTimer = 0;
	KorosseoKabe_SpectatorPhase = 0;
	Engine_TaskRemoveCallback(FieldScene_RunSupplementalSequenceOne);
	Actor_SetPosition(22, 0x3a80000, 0xd80000);
	Actor_SetPosition(23, 0x3c80000, 0xd80000);
	Actor_SetPosition(24, 0x3e80000, 0xd80000);
	Actor_SetPosition(25, 0x4080000, 0xd80000);
	Engine_ActorSetAnimation(31, 10);
}

static void KorosseoKabe_StartSupplementalSceneTask(void)
{
	Engine_TaskAddCallback(FieldScene_RunSupplementalSequenceOne, 0xC85);
	Engine_GameFlagClear(0x104);
}

static void SceneState_WaitForStatusWords(void)
{
	s32 cnt;

	Engine_TaskWait(10);

	cnt = 0;
	while (KorosseoKabe_SpectatorPhase != 0 || KorosseoKabe_SpectatorTimer != 75)
	{
		Engine_TaskWait(1);
		cnt++;
		if (cnt >= 600)
		{
			return;
		}
	}
}

static void EV31_PSYNERGY(void)
{
	BattleEffect_PauseObject(31);
	Engine_GameFlagSet(820);

	if (KorosseoKabe_SpectatorPhase != 0)
	{
		KorosseoKabe_SpectatorTimer = 0;
	}

	Engine_TaskWait(30);
	Engine_TaskWait(1);

	Engine_TaskRemoveCallback(FieldScene_RunSupplementalSequenceOne);

	Engine_MapCopyCellAttributes(58, 28, 7, 1, 58, 13);
	Engine_MapCopyCellAttributes(57, 11, 1, 1, 58, 11);
}

static void EV50_TOUCH(void)
{
	s32 rec8;
	s32 v5;
	s32 base;

	base = 2;
	SceneState_EmptyHook();
	Engine_EventBegin();
	rec8 = FieldScene_RunFlag211ApproachScene(77, 89);
	SceneState_WaitUntilStatusNine();
	v5 = 9;
	do
	{
		Object_RefreshSelectorById(8);
		v5 = (v5 - 1);
	} while (v5 >= 0);
	Actor_SetSpeed(8, 0x10000, 0x8000);
	Actor_WalkTo(8, 88, 0x100);
	Actor_SetSpeed(0, 0x10000, 0x8000);
	Actor_WalkToAndWait(0, 120, 0x100);
	Engine_ActorSetAnimation(8, 1);
	Engine_ActorFaceEachOther(0, 8, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimation(8, 3);
	Engine_ActorSetAnimationAndWait(0, 3);
	Engine_EventWait(20);
	Actor_SetSpeed(0, 0x20000, 0x10000);
	Actor_SetSpeed(8, 0x20000, 0x10000);
	Actor_WalkTo(0, 112, 0x100);
	Engine_ActorWalkToAndWait(8, 96, 0x100);
	Engine_ActorSetAnimation(0, 16);
	Engine_ActorSetAnimation(8, 9);
	Engine_EventWait(10);
	/* FAKEMATCH: base is set at the top of the function, so 2 - rec8 + 1 is not folded into 3 - rec8. */
	BattleFx_SetWeightedResult(72, base - rec8 + 1);
	Battle_SetStart(3);
	Party_SetFields1ceAnd1d0((s32)&SceneId_KorosseoKabe, 4);
	Event_SetDefeatDestination((s32)&SceneId_KorosseoKabe, 5);
	Engine_GameFlagSet(0x11a);
}

const struct SceneEvent *Scene_GetEvents(void)
{
	return gKorosseoKabeEvents;
}

static void FieldScene_RunPairedEntranceWalk(s32 a0)
{

	u32 i;
	s32 record;

	Engine_ActorDestroy(40);
	Engine_ActorDestroy(41);
	Owner_RefreshActiveRatios(1);
	Engine_EventBegin();
	Actor_SetPosition(8, 0x580000, 0x1000000);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x780000, 0x1000000);
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
	Camera_MoveTo(0x680000, 0, 0xc00000, 0);
	FieldScene_RunLateSequence(a0);
	Engine_EventEnd();
}

s32 KorosseoKabe_ApplyEntryState(void)
{
	struct FieldActor *actor;
	s32				   x;
	s32				   i;
	s32				   col;
	s32				   k;
	s32				   pos;
	s32				   row;
	s32				   zero;

	gEventWork->start_transition = 0;
	Engine_GameFlagSet(0x144);
	Map_CopyCellAttributes(14, 11, 12, 4, 100, 11);
	Map_CopyCellAttributes(48, 10, 5, 6, 120, 10);
	for (i = 26; i <= 30; i++)
	{
		actor = Object_GetById(i);
		Object_SetMode(actor, 4);
		actor->motion_flags = 0;
		actor->y.fixed = 0;
		actor->priority_flags = 2;
	}
	Object_GetById(18)->priority_flags = 2;
	if (Engine_GameFlagIsSet(0x330))
	{
		actor = Object_GetById(30);
		actor->x.fixed = 0x1500000;
		actor->y.fixed = -0x80000;
		actor->z.fixed = 0x1080000;
		Map_CopyCellAttributes(19, 16, 1, 1, 20, 16);
		Map_CopyCellAttributes(20, 80, 1, 1, 21, 80);
	}
	else
	{
		actor = Object_GetById(30);
		Object_SetMode(actor, 3);
		actor->y.fixed = 0x100000;
	}
	Object_GetById(11)->priority_flags = 2;
	zero = 0;
	if (Engine_GameFlagIsSet(0x335))
	{
		Map_CopyCellAttributes(35, 78, 1, 1, 35, 77);
	}
	if (Engine_GameFlagIsSet(0x333))
	{
		Engine_ActorSetAnimation(19, 4);
		Map_CopyCellAttributes(32, 37, 1, 4, 32, 77);
	}
	if (Engine_GameFlagIsSet(0x331))
	{
		Object_GetById(20)->motion_flags = zero;
		Object_GetById(20)->priority_flags = 2;
		Engine_ActorSetAnimation(20, 5);
		Map_CopyCellAttributes(46, 17, 1, 1, 44, 17);
	}
	if (Engine_GameFlagIsSet(0x332))
	{
		Object_GetById(21)->motion_flags = zero;
		Object_GetById(21)->priority_flags = 2;
		Engine_ActorSetAnimation(21, 5);
		Map_CopyCellAttributes(46, 17, 1, 1, 50, 17);
	}
	actor = Object_GetById(32);
	col = actor->x.fixed >> 20;
	actor->motion_flags = zero;
	actor->priority_flags = 2;
	Engine_MapCopyCellAttributes(52, 28, 1, 3, col, 10);
	actor = Object_GetById(33);
	col = actor->x.fixed >> 20;
	actor->motion_flags = zero;
	actor->priority_flags = 2;
	Engine_MapCopyCellAttributes(52, 28, 1, 3, col, 13);

	x = GameFlag_GetByte(0x340);
	if (x == 0)
	{
		x = 73;
	}
	actor = Object_GetById(12);
	actor->x.fixed = (x << 20) + 0x80000;
	actor->motion_flags = zero;
	actor->priority_flags = 2;
	Engine_MapCopyCellAttributes(71, 16, 1, 1, x, 16);
	x = GameFlag_GetByte(0x348);
	if (x == 0)
	{
		x = 76;
	}
	actor = Object_GetById(13);
	actor->x.fixed = (x << 20) + 0x80000;
	actor->motion_flags = zero;
	actor->priority_flags = 2;
	Engine_MapCopyCellAttributes(71, 16, 1, 1, x, 16);
	x = GameFlag_GetByte(0x350);
	if (x == 0)
	{
		x = 79;
	}
	actor = Object_GetById(14);
	actor->x.fixed = (x << 20) + 0x80000;
	actor->motion_flags = zero;
	actor->priority_flags = 2;
	Engine_MapCopyCellAttributes(71, 16, 1, 1, x, 16);

	SceneState_ApplyRectsForActors15To17();
	Engine_ActorSetAnimation(31, 10);
	if (Engine_GameFlagIsSet(0x334))
	{
		for (k = 22, row = 13, pos = 58; k <= 25; k++, pos += 2)
		{
			actor = Object_GetById(k);
			actor->priority_flags = 2;
			Object_SetMode(actor, 4);
			Engine_MapCopyCellAttributes(56, 13, 1, 1, pos, row);
		}
		Engine_ActorSetAnimation(31, 10);
		BattleEffect_PauseObject(31);
	}
	else
	{
		for (k = 22; k <= 25; k++)
		{
			actor = Object_GetById(k);
			actor->priority_flags = 2;
			Object_SetMode(actor, 4);
			actor->speed = 0x8000;
			actor->acceleration = 0x3333;
		}
		Engine_TaskAddCallback(FieldScene_RunSupplementalSequenceOne, 0xc85);
		Scheduler_SetCallbackMask((u32)FieldScene_RunSupplementalSequenceOne, 1);
	}
	Engine_ActorSetAnimation(8, 9);
	gGameState.movement_mode = 0;
	Korosseo_ShowItemIcon(41, 89);
	Korosseo_ShowItemIcon(40, 77);
	Engine_ActorSetChildValue(8, 1);
	switch (gGameState.entrance)
	{
	case 1:
		FieldScene_BuildDescriptorAndInstallTask(0, 8, 5, 0x680000, 0x1000000, 40, 41);
		Map_CopyCellsLayered(127, 0, 1, 2, 79, 6);
		Engine_ActorDestroy(34);
		Engine_ActorDestroy(35);
		Engine_ActorDestroy(36);
		Engine_ActorDestroy(37);
		Engine_ActorDestroy(38);
		Engine_ActorDestroy(39);
		if (!Engine_GameFlagIsSet(0x109))
		{
			Engine_AudioPlayCue(17);
			Korosseo_SelectSoloCompetitor(0);
			SceneActor_PlacePartyAtSavedTiles();
			KorosseoKabe_RunScriptedTransition(2);
		}
		Object_LinkObjectAndSetCallback(1, 0);
		Object_LinkObjectAndSetCallback(2, 0);
		Object_LinkObjectAndSetCallback(3, 0);
		SceneState_InitControlRecordAndStartTask((s32)&ResourceId_RivalPathB);
		break;
	case 2:
		Engine_TaskAddCallback(SceneState_RaiseTriggerOnPsynergyTarget, 0xc80);
		Engine_ActorDestroy(40);
		Engine_ActorDestroy(41);
		if (!Engine_GameFlagIsSet(0x109))
		{
			SceneActor_PlacePartyAtSavedTiles();
			Korosseo_SelectSoloCompetitor(1);
			KorosseoKabe_RunScriptedTransition(0);
		}
		break;
	case 3:
		if (!Engine_GameFlagIsSet(0x109))
		{
			Korosseo_RunGreetScene(34);
			Korosseo_ClearSavedActorPositions();
		}
		break;
	case 4:
		FieldScene_RunPairedEntranceWalk(2);
		Engine_EventRequestExit(4);
		break;
	case 5:
		FieldScene_RunPairedEntranceWalk(-2);
		Engine_EventRequestExit(5);
		break;
	}
	return 0;
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
	Engine_EventSetMessage((s32)MsgKorosseoSiteSecondFinals);
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

static void EV35_TALK(s32 speaker)
{
	s32				   result;
	struct FieldActor *actor;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(speaker);
	}
	else
	{
		Engine_EventBegin();
		result = SceneDialogue_RunFlagGatedPromptInteraction(speaker, 1);
		if (result == 0)
		{
			Engine_EventSetMessage((s32)MsgKorosseoStageDubbedMini);
			Engine_CameraSetSpeed(0x30000, 0x6000);
			Engine_CameraMoveTo(0x4c80000, -1, 0xb80000, 1);
			Engine_CameraWaitForMove();
			Engine_EventShowMessage(speaker, 0);
			Korosseo_FadeInCompetitor(ACTOR_PARTY_LEADER, 0x4f8, 168);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
			OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 0x508, 184);
			OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 0x508, 216);
			OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 0x4c8, 216);
			Engine_EventShowMessage(speaker, 0);
			OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 0x4c8, 248);
			OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 0x4a8, 248);
			Engine_EventWait(3);
			actor = Object_GetById(ACTOR_PARTY_LEADER);
			actor->velocity_y = 0x40000;
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 28);
			Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
			Engine_EventWait(30);
			Engine_EventShowMessage(speaker, 0);
			Korosseo_RestoreCompetitor(ACTOR_PARTY_LEADER);
			Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
			Korosseo_ShowFollowUpPrompt(speaker, 1);
		}
		else if (result == 1)
		{
			Engine_EventSetMessage((s32)MsgKorosseoMainThingStage);
			Engine_EventShowMessage(speaker, 0);
		}
		FieldScene_RunMiddleSequence(result, speaker, 1);
		Engine_EventEnd();
	}
}

static void EV36_TALK(s32 a0)
{
	u32 i;
	s32 rec;
	s32 record;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(a0);
	}
	else
	{
		Engine_EventBegin();
		rec = SceneDialogue_RunFlagGatedPromptInteraction(a0, 2);
		if (rec == 0)
		{
			Engine_EventSetMessage((s32)MsgKorosseoShiftingFloorStage);
			FieldScene_PlaceSpectatorRow();
			Camera_SetSpeed(0x30000, 0x6000);
			Camera_MoveTo(0x3d80000, -1, 0xe80000, 1);
			Engine_CameraWaitForMove();
			/* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
			Value2(Engine_EventShowMessage, a0, 0);
			KorosseoKabe_StartSupplementalSceneTask();
			Event_ShowMessage(a0, 0);
			Korosseo_FadeInCompetitor(0, 0x438, 0x108);
			Engine_EventWait(15);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
			OverlayObject_PlaceWithScale14000(0, 0x438, 216);
			OverlayObject_PlaceWithScale14000(0, 0x428, 216);
			SceneState_WaitForStatusWords();
			Engine_LeaderCheckAhead();
			Camera_MoveTo(-1, -1, -1, 0);
			Event_ShowMessage(a0, 0);
			Korosseo_RestoreCompetitor(0);
			Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
			Korosseo_ShowFollowUpPrompt(a0, 2);
		}
		else
		{
			if (rec == 1)
			{
				Engine_EventSetMessage((s32)MsgKorosseoOperatorLiftsWillCheerFor);
				Event_ShowMessage(a0, 0);
			}
		}
		FieldScene_RunMiddleSequence(rec, a0, 2);
		Engine_EventEnd();
	}
}

static void EV37_TALK(s32 a0)
{
	u32 i;
	s32 rec2;
	s32 record;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(a0);
	}
	else
	{
		Engine_EventBegin();
		rec2 = SceneDialogue_RunFlagGatedPromptInteraction(a0, 3);
		if (rec2 != 0)
		{
		}
		else
		{
			Engine_EventSetMessage((s32)MsgKorosseoLogRollingStage);
			Camera_SetSpeed(0x30000, 0x6000);
			Camera_MoveTo(0x2f00000, -1, 0xc00000, 1);
			Engine_CameraWaitForMove();
			Engine_EventWait(60);
			Camera_SetSpeed(0x10000, 0x2000);
			Camera_MoveTo(0x2f00000, -1, 0xe00000, 1);
			Engine_CameraWaitForMove();
			Event_ShowMessage(a0, 0);
			Korosseo_FadeInCompetitor(0, 0x358, 0x108);
			Engine_EventWait(10);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
			OverlayObject_PlaceWithScale14000(0, 0x358, 0x108);
			OverlayObject_PlaceWithScale14000(0, 0x358, 232);
			Event_ShowMessage(a0, 0);
			OverlayObject_PlaceWithScale14000(0, 0x348, 232);
			Engine_EventWait(10);
			/* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
			Value3(SceneActor_MovePairByTileOffset, 33, -64, 0);
			Camera_MoveTo(0x2f00000, -1, 0xd80000, 1);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
			Engine_EventWait(10);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
			Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x2f8, 232);
			Engine_EventWait(10);
			Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 30);
			Event_ShowMessage(a0, 0);
			Korosseo_RestoreCompetitor(0);
			Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
			Actor_SetPosition(33, 0x3480000, 0xe80000);
			Korosseo_ShowFollowUpPrompt(a0, 3);
			goto L_020016b0;
		}
		if (rec2 == 1)
		{
			Engine_EventSetMessage((s32)MsgKorosseoHereYourObjectiveRideLogs);
			Event_ShowMessage(a0, 0);
		}
	L_020016b0:;
		FieldScene_RunMiddleSequence(rec2, a0, 3);
		Engine_EventEnd();
	}
}

static void EV38_TALK(s32 speaker)
{
	s32				   z;
	struct FieldActor *actor;
	s32				   result;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(speaker);
		return;
	}
	Engine_EventBegin();
	result = SceneDialogue_RunFlagGatedPromptInteraction(speaker, 4);
	if (result == 0)
	{
		Engine_EventSetMessage((s32)MsgKorosseoFansJustCallWall);
		Engine_CameraSetSpeed(0x30000, 0x6000);
		Camera_MoveTo(0x2180000, -1, 0xf00000, 1);
		Engine_CameraWaitForMove();
		Engine_EventWait(45);
		Camera_SetSpeed(0x10000, 0x2000);
		Camera_MoveTo(0x2180000, -1, 0xc00000, 1);
		z = 264;
		Engine_CameraWaitForMove();
		Engine_EventShowMessage(speaker, 0);
		Korosseo_FadeInCompetitor(ACTOR_PARTY_LEADER, 632, z);
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 616, z);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
		battle_owner_69();
		Camera_SetSpeed(0x4000, 0x800);
		Camera_MoveTo(0x2180000, -1, 0xa00000, 1);
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 10);
		actor = Object_GetById(ACTOR_PARTY_LEADER);
		Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed + 0x400000, actor->z.fixed);
		Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
		battle_owner_69();
		Camera_MoveTo(-1, -1, -1, 0);
		Event_ShowMessage(speaker, 0);
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
		OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 488, 248);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 20);
		BattleFx_RunRisingObjectSequence(ACTOR_PARTY_LEADER, 6, 0);
		Engine_CameraMoveTo(0x2180000, -1, 0xa00000, 1);
		Engine_EventShowMessage(speaker, 0);
		Korosseo_RestoreCompetitor(ACTOR_PARTY_LEADER);
		Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
		Korosseo_ShowFollowUpPrompt(speaker, 4);
	}
	else if (result == 1)
	{
		Engine_EventSetMessage((s32)MsgKorosseoScalingWallQuickly);
		Engine_EventShowMessage(speaker, 0);
	}
	FieldScene_RunMiddleSequence(result, speaker, 4);
	Engine_EventEnd();
}

static void KorosseoKabe_PushBlockToCell(s32 id, s32 column, s32 row)
{
	s32				   pusher = gGameState.selected_actor;
	struct FieldActor *leader = Object_GetById(pusher);
	struct FieldActor *block = Object_GetById(id);
	s32				   along_x = (block->x.fixed >> 20) != column / 2;
	s32				   dx;
	s32				   dz;

	column <<= 16;
	row <<= 16;
	if (along_x)
	{
		dx = (column - block->x.fixed) / 2;
		dz = 0;
	}
	else
	{
		dx = 0;
		dz = (row - block->z.fixed) / 2;
	}
	Engine_ActorSetAnimation(pusher, 8);
	Engine_EventWait(6);
	block->speed = 0x8000;
	block->acceleration = 0x3333;
	Engine_AudioPlayCue(239);
	Object_SetMode(block, 3);
	Engine_ObjectSetPosition(block, column, 0, row);
	Engine_EventWait(6);
	Engine_ActorSetAnimation(pusher, 2);
	Engine_ActorSetSpeed(pusher, 0x8000, 0x3333);
	Object_SetMode(leader, 2);
	Engine_ObjectSetPosition(leader, leader->x.fixed + dx, 0, leader->z.fixed + dz);
	Object_CommitPosition(leader);
	Object_SetMode(leader, 1);
	Object_CommitPosition(block);
	Object_SetMode(block, 1);
	Engine_AudioPlayCue(288);
	Engine_AudioPlayCue(213);
	Engine_EventWait(15);
}

static void EV39_TALK(s32 speaker)
{
	s32 result;

	if (gGameState.entrance == 2)
	{
		Korosseo_FinishSoloRound(speaker);
		return;
	}
	Engine_EventBegin();
	result = SceneDialogue_RunFlagGatedPromptInteraction(speaker, 5);
	if (result == 0)
	{
		Engine_EventSetMessage((s32)MsgKorosseoPlaceCalledBoard);
		Engine_CameraSetSpeed(0x20000, 0x4000);
		Camera_MoveTo(0x1480000, -1, 0x1080000, 1);
		Engine_CameraWaitForMove();
		Engine_EventWait(30);
		Camera_SetSpeed(0x18000, 0x3000);
		Camera_MoveTo(0x1380000, -1, 0xb00000, 1);
		Engine_CameraWaitForMove();
		Engine_EventShowMessage(speaker, 0);
		Korosseo_FadeInCompetitor(ACTOR_PARTY_LEADER, 408, 264);
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
		OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 408, 216);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 10);
		Engine_EventShowMessage(speaker, 0);
		KorosseoKabe_PushBlockToCell(16, 360, 208);
		Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 264, 45);
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
		OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 376, 216);
		OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 376, 248);
		OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 312, 248);
		Engine_EventShowMessage(speaker, 0);
		Korosseo_RestoreCompetitor(ACTOR_PARTY_LEADER);
		Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
		Actor_SetPosition(16, 0x1880000, 0xd00000);
		Korosseo_ShowFollowUpPrompt(speaker, 5);
	}
	else if (result == 1)
	{
		Engine_EventSetMessage((s32)MsgKorosseoFocusRollingLogs);
		Engine_EventShowMessage(speaker, 0);
	}
	FieldScene_RunMiddleSequence(result, speaker, 5);
	Engine_EventEnd();
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korosseo/saved.c"
#include "../common/korosseo/solo_competitor.c"
#include "../common/korosseo/raised.c"
#include "../common/korosseo/solo_round.c"
#include "../common/korosseo/cheer.c"
#include "../common/item/addequip.c"
#include "../common/korosseo/korosseo.c"

static void KorosseoKabe_InitializeModeTask(u32 mode, u32 param)
{
	s32 handler;

	Korosseo_ModeTaskMode = mode;
	Korosseo_ModeTaskParam = (param << 4);

	{
		s32 budget = 0xc80;
		Engine_TaskAddCallback(Korosseo_UpdateModeTask, budget);
	}

	handler = (s32)KorosseoKabe_ModeRecordDefault;
	if (mode == 2)
	{
		handler = (s32)KorosseoKabe_ModeRecordTwo;
	}
	if (mode == 4)
	{
		handler = (s32)KorosseoKabe_ModeRecordFour;
	}
	if (mode == 3)
	{
		if (param != 0)
		{
			handler = (s32)KorosseoKabe_ModeRecordThreeAlt;
		}
		else
		{
			handler = (s32)KorosseoKabe_ModeRecordThree;
		}
	}

	Korosseo_ModeTaskTimer = 0;
	Korosseo_ModeTaskScript = (s16 *)handler;
	Korosseo_ModeMoveTarget = 0;
	Korosseo_ModeMoveDuration = 0;
	Korosseo_ModeTaskPosition = 0;
}

static void KorosseoKabe_RunScriptedTransition(s32 mode)
{
	if (mode == 0)
	{
		Engine_EventBegin();
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		Engine_EventWait(30);
		Engine_AudioPlayCue(0x59);
		Korosseo_LoadPortrait(0);
		KorosseoKabe_InitializeModeTask(1, 0);
		Engine_EventWait(120);
		Engine_EventEnd();
		return;
	}

	Engine_AudioPlayCue(0xf7);
	Engine_EventBegin();
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	{
		s16 *base = KorosseoKabe_ModeRecordDefault;
		*(s16 *)((u8 *)base + 30) = (mode * 60);
	}
	Engine_EventWait(30);
	Engine_AudioPlayCue(mode + 0x5a);
	Korosseo_LoadPortrait(mode);
	KorosseoKabe_InitializeModeTask(1, 0);
	Engine_EventWait(120);

	goto check_transition;
wait_transition:
	Engine_TaskWait(1);
check_transition:
	if (AudioCommand_GetStateByte() != 0)
		goto wait_transition;

	Engine_AudioPlayCue(0x121);
	Korosseo_LoadPortrait(5);
	KorosseoKabe_InitializeModeTask(2, 0);
	Engine_AudioPlayCue(0xec);
	Engine_EventWait(60);
	KorosseoKabe_InitializeModeTask(2, 1);
	Engine_AudioPlayCue(0xec);
	Engine_EventWait(60);
	Korosseo_LoadPortrait(6);
	KorosseoKabe_InitializeModeTask(2, 0);
	Engine_AudioPlayCue(0xec);
	Engine_EventWait(60);
	Korosseo_LoadPortrait(7);
	KorosseoKabe_InitializeModeTask(4, 0);
	Engine_AudioPlayCue(0xed);
	Audio_PlayCueFromEventWork();
	Engine_EventEnd();
	Engine_GameFlagSet(0x123);
}

static void FieldScene_RunLateSequence(s32 a0)
{

	s32 kind;

	Engine_AudioPlayCue(247);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	KorosseoKabe_ModeRecordThree[13] = a0 * 60;
	STORE_CONST(KorosseoKabe_ModeRecordThreeAlt[13]) = (a0 < 0 ? -a0 : a0) * 60;
	if (a0 < 0)
	{
		Engine_EventWait(30);
		Engine_AudioPlayCue(86);
		Korosseo_LoadPortrait(8);
		/* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
		Value2(KorosseoKabe_InitializeModeTask, 3, 1);
		Engine_EventWait(-a0 * 60 + 60);
		kind = 0;
	}
	else
	{
		Engine_EventWait(30);
		Engine_AudioPlayCue(a0 + 90);
		Korosseo_LoadPortrait(4);
		/* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
		Value2(KorosseoKabe_InitializeModeTask, 3, 0);
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
#include "../common/korosseo/markslot.c"
#include "../common/korosseo/marker.c"
#include "../common/korosseo/markmove.c"
#include "../common/korosseo/item_icon.c"

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

static void SceneState_EmptyHook(void)
{
}

static void KorosseoKabe_MarkStageReady(void)
{
	((struct StageControl *)gSceneState)->status = 9;
}

static void SceneState_WaitUntilStatusNine(void)
{
	s16 *status = &((struct StageControl *)gSceneState)->status;

	while (*status != 9)
	{
		Engine_TaskWait(1);
	}
}

static void KorosseoKabe_SpawnRandomSceneEffect(SparkSource *a)
{
	s32 t[3];
	u32 n;

	if (a->f28 >= -255 && a->f28 <= 255)
	{
		a->f55 = 0;
	}
	n = Engine_RandomNext();
	if (n * 100 >> 16 <= 9)
	{
		SparkSource *o;
		s32			 u;
		s32			 w;

		t[0] = a->f08;
		t[1] = a->f0c;
		t[2] = a->f10;
		u = Engine_RandomNext();
		w = Engine_RandomNext();
		Vector_AddPolarOffset(u << 4, w, t);
		{
			s32 x = t[0];
			s32 y = t[1];
			s32 z = t[2];

			o = (SparkSource *)Engine_ObjectCreate(285, x, y, z);
		}
		if (o != 0)
		{
			o->f55 = 0;
			Engine_ActorSetSpriteFlags(o, 0);
			Engine_ObjectSetScript(o, KorosseoKabe_SparkScript);
			Object_SetMode(o, 1);
			Object_SetMode(o, 0);
		}
	}
}

static s32 KorosseoKabe_RaiseLinkedSceneEffect(RaisedEffect *a)
{
	RaisedEffect *o = (RaisedEffect *)Object_GetById(a->f64);

	Engine_ObjectSetPosition(o, a->f08, a->f0c + 0x240000, a->f10);
	o->f55 = 0;
	Engine_ObjectSetScript(o, KorosseoKabe_RaiseScript);
	Engine_AudioPlayCue(83);
	a->f64 = 0;
	return 0;
}

static s32 FieldScene_RunFlag211ApproachScene(s32 handleA, s32 handleB)
{
	u8				  *workspace = *(u8 **)((u8 *)gWorkSlot + WORK_SLOT_STAGE * 4);
	u8				  *shared;
	struct FieldActor *record;
	s32				   flag;
	s32				   x;
	s32				   z;
	u16				  *cuep;
	s16				  *waitp;

	flag = Engine_GameFlagIsSet(0x211);

	shared = (u8 *)&gGameState;
	record = Object_GetById(((struct GameState *)shared)->selected_actor);

	if (*(s32 *)(workspace + 232) < record->x.fixed)
	{
		x = *(s32 *)(workspace + 232) + 0xc0000;
	}
	else
	{
		x = *(s32 *)(workspace + 232) - 0xc0000;
	}

	if (flag != 0)
	{
		z = *(s32 *)(workspace + 236) + 0x100000;
		cuep = (u16 *)(workspace + 228);
	}
	else
	{
		z = *(s32 *)(workspace + 236) - 0x100000;
		cuep = (u16 *)(workspace + 226);
	}

	waitp = (s16 *)&record->timer;
	*waitp = *cuep;
	record->acceleration = 0x4000;
	record->speed = 0x10000;

	Engine_ObjectSetPosition(record, x, 0, z);
	Engine_GameFlagSet(0x211);
	Engine_ObjectSetScript(record, KorosseoKabe_ApproachScript);

	while (*waitp != 0)
	{
		Engine_TaskWait(1);
	}

	if (flag == 0)
	{
		/* FAKEMATCH: void functions called as returning s32; as void calls the argument moves are ordered differently. */
		((s32 (*)())SceneActor_ApplyValueAndMatchingSlots)(0, handleA);
		UiWork_PushValueSlot(handleA, 2);
	}
	else
	{
		((s32 (*)())SceneActor_ApplyValueAndMatchingSlots)(0, handleB);
		UiWork_PushValueSlot(handleB, 2);
	}

	shared = (u8 *)&gGameState;
	UiWork_PushValueSlot(((struct GameState *)shared)->selected_actor, 1);
	Engine_MessageShowCentered((s32)MsgKorosseoRobinGotItem, 3);
	ObjectDispatch_WaitForValue16(record);

	return flag;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korosseo/path_rival.c"

void FieldScene_BuildDescriptorAndInstallTask(
	s32 first, s32 second, s32 mode, s32 centre, s32 extra, s32 third, s32 fourth)
{
	u8				  *descriptor;
	struct FieldActor *first_record;
	struct FieldActor *second_record;
	s32				   handle;
	s32				   extent;

	descriptor = Runtime_AllocateBlock(59, 0x7170);
	handle = (s32)Runtime_BumpAllocateAlternatePool(512);

	*(u16 *)(descriptor + 222) = first;
	*(u16 *)(descriptor + 224) = second;
	*(u16 *)(descriptor + 226) = third;
	*(u16 *)(descriptor + 228) = fourth;
	*(u16 *)(descriptor + 230) = mode;
	*(s32 *)(descriptor + 232) = centre;
	*(s32 *)(descriptor + 236) = extra;

	first_record = Object_GetById(first);
	second_record = Object_GetById(second);

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

	Engine_TaskAddCallback(Korosseo_DrawGauge, 0xc76);

	Runtime_BumpFree(handle);
}

static void SceneState_InitControlRecordAndStartTask(s32 resource)
{
	u8					*state = *(u8 **)((u8 *)gWorkSlot + WORK_SLOT_STAGE * 4);
	struct StageControl *m = (struct StageControl *)gSceneState;

	Resource_DecodeType01(Resource_GetTableEntry(resource), state + 240);
	if (Engine_GameFlagIsSet(0x109) == 0)
	{
		m->status = 1;
		m->f2 = 1;
		m->f4 = *(u16 *)(state + 224);
		m->f8 = 0;
		m->f6 = 0;
	}
	Engine_TaskAddCallback(Korosseo_UpdatePathRival, 0xc85);
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korosseo/scene_state.c"
#include "../common/object/findslot.c"

static void StagedActor_PushActorAhead(void)
{

	KabeSceneRecord *subject;
	KabeSceneRecord *target;
	KabeSceneRecord *blocker;
	u32				 step;
	u32				 dir;
	Position3		 pos;
	u32				 idx = 250;
	s32				 zero;
	s32				 handle;

	handle = *(s32 *)((u8 *)&gGameState + (idx << 1));
	subject = (KabeSceneRecord *)Object_GetById(handle);

	dir = subject->facing >> 12;

	step = KorosseoKabe_DirectionSteps[dir];
	pos.x = subject->x + (step & 0xffff0000);
	pos.y = subject->y;
	step <<= 16;
	pos.z = subject->z + step;

	target = SceneData_FindSlotAtPosition((s32 *)&pos, subject);
	if (target == 0)
	{
		return;
	}

	step = KorosseoKabe_DirectionSteps[dir];
	pos.x = target->x + (step & 0xffff0000);
	pos.y = target->y;
	step <<= 16;
	pos.z = target->z + step;

	blocker = SceneData_FindSlotAtPosition((s32 *)&pos, target);
	if (blocker != 0 && (blocker->flags & 1) != 0)
	{
		return;
	}

	pos.x = target->x;
	pos.y = target->y + 0x100000;
	pos.z = target->z;

	blocker = SceneData_FindSlotAtPosition((s32 *)&pos, target);
	if (blocker != 0 && (blocker->flags & 1) != 0)
	{
		return;
	}

	target->state = 2;
	zero = 0;

	step = KorosseoKabe_DirectionSteps[dir];
	pos.x = target->x + (step & 0xffff0000);
	pos.y = target->y;
	step <<= 16;
	pos.z = target->z + step;

	if (Object_CheckMovementCollision(target, &pos) > 0)
	{
		return;
	}

	Object_SetMode(subject, 8);
	Engine_TaskWait(15);

	target->rate_x = 0x3333;
	target->rate_z = 0x3333;
	Engine_ObjectSetPosition(target, pos.x, pos.y, pos.z);

	subject->rate_x = 0x3333;
	subject->rate_z = 0x3333;
	Engine_ObjectSetPosition(subject, pos.x, pos.y, pos.z);

	Engine_AudioPlayCue(0xee);
	Object_CommitPosition(target);
	Engine_AudioPlayCue(0x120);

	target->x = pos.x;
	target->z = pos.z;
	target->motion_24 = zero;
	target->motion_2c = zero;

	Object_SetMode(subject, 1);
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korosseo/occupant.c"

const u8 KorosseoKabe_RollLogScript[] = {2, 2, 3, 3};

const u8 Korosseo_PortraitPaletteOffsets[] = {
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	0, 32, 32, 32, 96, 64, 64, 64, 128, 0,
#else
	0, 32, 32, 32, 96, 64, 64, 160, 128, 0,
#endif
};

const s16 KorosseoKabe_ModeRecordTwo[] = {
	0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
	30, 0x7fff, 30, -1,
};

const s16 KorosseoKabe_ModeRecordThreeAlt[] = {
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

s32 KorosseoKabe_SparkScript[] = {
	SC_ADD(SW_Y, -0x4000),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_FREE,
};

s32 KorosseoKabe_RaiseScript[] = {
	SC_SET(SW_HOOK, (s32)KorosseoKabe_SpawnRandomSceneEffect),
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

#define DIRECTION_STEP(x, z) ((u32)(x) << 16 | (u16)(z))

u32 KorosseoKabe_DirectionSteps[] = {
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

struct SceneEntrance gKorosseoKabeEntrances[] = {
	{ 1, CONDITION_ALWAYS, 1272, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 1224, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 384, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gKorosseoKabeExits[] = {
	SCENE_EXITS(144),
	SCENE_EXIT(10, 144, 1),
	SCENE_EXIT(11, 140, 68),
	SCENE_EXIT(4, 140, 31),
	SCENE_EXIT(5, 138, 98),
	SCENE_EXITS_END,
};

struct ScenePlacement gKorosseoKabePlacements[] = {
	{ 0x0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1272), 0, PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(568), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1272), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1224), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1176), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(264), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(392), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x103, CONDITION_ALWAYS, 7,
		PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(520), 0, PIXELS(258), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(712), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(808), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(936), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(968), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(1000), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(1032), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(248), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(264), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(296), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(312), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x103, CONDITION_ALWAYS, 7,
		PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1072), 0, PIXELS(280), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(840), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(840), 0, PIXELS(232), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1224), 0, PIXELS(488), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1208), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(984), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(760), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(536), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(240), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(272), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

s32 KorosseoKabe_SpectatorPhase = 0;

s32 KorosseoKabe_SpectatorTimer = 0;

struct SceneEvent gKorosseoKabeEvents[] = {
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(KorosseoKabe_RunSelectedActorRisingEffect) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(KorosseoKabe_CheckLeaderAhead) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 28, CONDITION_ALWAYS, EVENT_SCRIPT(EV28_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 27, CONDITION_ALWAYS, EVENT_SCRIPT(EV27_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EV24_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_TOUCH) },
	{ 0x202, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_SCRIPT) },
	{ 0x202, 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunTwoStepSequence) },
	{ 0x202, 26, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunTwoStepSequence) },
	{ EVENT_TOUCH, 50, 0x210, EVENT_SCRIPT(EV50_TOUCH) },
	{ SEARCH(SEARCH_CHEST), 101, 0x338, FIND_ITEM(ITEM_NUT) },
	{ 0x8413, 102, 0x339, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ SEARCH(SEARCH_CHEST), 104, 0x33b, FIND_ITEM(ITEM_NUT) },
	{ 0x50001815, 20, 0x331, EVENT_SCRIPT(EV20_SCRIPT) },
	{ 0x50001815, 21, 0x332, EVENT_SCRIPT(FieldScene_SetFlag332AndDrawTiles) },
	{ 0x10008e15, 18, 0x330, EVENT_SCRIPT(EV18_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(0x8e), 18, 0x330, EVENT_SCRIPT(EV18_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0xc), 19, 0x333, EVENT_SCRIPT(EV19_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneData_NoOpHandler) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneData_NoOpHandler) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8c), 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_StoreSlotTileXToWork832To848) },
	{ PSYNERGY_ON_ACTOR(0x8c), 13, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_StoreSlotTileXToWork832To848) },
	{ PSYNERGY_ON_ACTOR(0x8c), 14, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_StoreSlotTileXToWork832To848) },
	{ PSYNERGY_ON_ACTOR(0x91), 31, 0x334, EVENT_SCRIPT(EV31_PSYNERGY) },
	{ EVENT_TALK, 34, CONDITION_ALWAYS, EVENT_SCRIPT(RunPartyCountInteraction) },
	{ EVENT_TALK, 35, CONDITION_ALWAYS, EVENT_SCRIPT(EV35_TALK) },
	{ EVENT_TALK, 36, CONDITION_ALWAYS, EVENT_SCRIPT(EV36_TALK) },
	{ EVENT_TALK, 37, CONDITION_ALWAYS, EVENT_SCRIPT(EV37_TALK) },
	{ EVENT_TALK, 38, CONDITION_ALWAYS, EVENT_SCRIPT(EV38_TALK) },
	{ EVENT_TALK, 39, CONDITION_ALWAYS, EVENT_SCRIPT(EV39_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 34, CONDITION_ALWAYS, MSGNO(MsgKorosseoKawaThisYearsFinalsAreIncredibleThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 35, CONDITION_ALWAYS, MSGNO(MsgKorosseoKabeICantBelieveAnybodyEverGets) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 36, CONDITION_ALWAYS, MSGNO(MsgKorosseoKabeHowDoTheyMakeThatFloor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 37, CONDITION_ALWAYS, MSGNO(MsgKorosseoKabeImAmazedPeopleCanActuallyCross) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 38, CONDITION_ALWAYS, MSGNO(MsgKorosseoKabeBoyThatBitOverThereMust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 39, CONDITION_ALWAYS, MSGNO(MsgKorosseoKabeIHeardBabiGotTheLog) },
	{ EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_RAISED) },
	{ SCENE_EVENTS_END },
};

s16 Korosseo_PortraitSlot = -1;

s16 KorosseoKabe_ModeRecordDefault[] = {
#if !EDITION_INTERNATIONAL
	0x4000, -188, 2048, 0x1000, 256, 1, 0x2000, 16,
#else
	0x4000, -188, 2048, 0x1000, 384, 1, 0x2000, 16,
#endif
	1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
	0x3000, 2048, 20, 0x7fff, 60, -1,
};

s16 KorosseoKabe_ModeRecordFour[] = {
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

s16 KorosseoKabe_ModeRecordThree[] = {
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
	SC_CALL(KorosseoKabe_RaiseLinkedSceneEffect),
	SC_WAIT(3),
	SC_ARG(10),
	SC_WAIT(150),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ARG(1),
	SC_END,
};

s32 KorosseoKabe_ApproachScript[] = {
	SC_ARG(2),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(6),
	SC_CALL(KorosseoKabe_RaiseLinkedSceneEffect),
	SC_WAIT(3),
	SC_ARG(28),
	SC_WAIT(150),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ARG(1),
	SC_END,
};
