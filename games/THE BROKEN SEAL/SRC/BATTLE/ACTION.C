//--------------------------------------------------------------------------
// ACTION.C
// フィールドでのエナジー使用と、マップのマーカー物体の配置
//--------------------------------------------------------------------------

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "FIELDRUN.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "BTLSYS.H"
#include "DMA.H"
#include "GAME_STATE.H"
#include "SCRIPT_OBJECT_RUNTIME.H"
#include "OBJECT_RUNTIME.H"
#include "IO_REG.H"
#include "BTLCALC.H"
#include "HEAP_STATE.H"
#include "text/MSG_IDS.H"

// イベントワークのうち、ここで使う部分
struct BattleCommandRuntime
{
	u8	pad000[0x170];
	s16 result_code; // 出口の要求 (999 = リターン)
	u8	pad172[0x2c];
	s16 battle_mode; // マップのモード (3 = ワールドマップ)
	u8	pad1a0[0xb26];
	u8	resolving_action; // エナジーの演出中
};

extern struct BattleRuntime *gEventWork;

void *ObjectTable_Get(s32);
void  GameFlag_ClearBitFar(s32);
void  UiWork_PushValueSlotFar(s32, s32);
void  UiText_ShowPositionedMessageAndWaitFar(s32, s32);
void  UiWork_FinalizePendingCoreFar(void);

s32	 BattleFx_FindMatchingEvent(s32, s32, void *);
void GameFlag_SetBitFar(s32);
void BattleFx_LoadActionEffectResources(s32, s32);
void BattleFx_SetupObjectPair(s32, s32);
void EventObject_Initialize(void);

void BattleFx_DispatchRequestKind(void);
void BattleFx_Run(void);
void EffectRuntime_StopCurrentObject(void);
void BattleEffect_CleanupSceneObjects(void);

s32 Map_GetTerrainHeightFar(s32, s32, s32);

//--------------------------------------------------------------------------
// エナジーの実行
//--------------------------------------------------------------------------
#define ACT_NO(c) ((c) & 0x3ff)		   // わざ番号
#define ACT_UNIT(c) (((c) >> 10) & 15) // 使う者
#define ACT_PACKED 0x2000			   // EPを使わずに実行する
#define UNIT_NONE 15
#define MIKATA_MAX 7 // 仲間の番号の上限

#define ENA_IMAJIN 144 // イマジン
#define ENA_RITAN 149  // リターン

#define EXIT_RITAN 999 // リターンによる出口

#define MODE_WORLD_MAP 3 // ワールドマップ

#define FLG_ENA_NONE 0x17e // エナジーが効かない
#define FLG_RITAN_NG 0x144 // リターンが打ち消される
#define FLG_ITEM_USE 0x145 // 道具から使った
#define FLG_ENA_EFX 0x140  // 標準の演出を行う
#define FLG_ENA_TGT 0x141  // 対象に向けて演出する

// エナジーのイベントの段階
#define ENA_EV(phase) ((phase) << 28 | EVENT_PSYNERGY)
#define ENA_EV_ANY ENA_EV(7) // 段階を問わない

// 使う者とわざの名前を入れたメッセージを出して待つ
#define ENA_MES(who, act, m)                                               \
	(UiWork_PushValueSlotFar((who), 1), UiWork_PushValueSlotFar((act), 4), \
		UiText_ShowPositionedMessageAndWaitFar((s32)(m), 1))

//--------------------------------------------------------------------------
// フィールドでエナジーを使う
// code : わざ番号と使う者
//--------------------------------------------------------------------------
s32 BattleCommand_ExecuteSelectedAction(u32 code)
{
	s32							 act_no = ACT_NO(code);
	struct BattleCommandRuntime *work = (struct BattleCommandRuntime *)gEventWork;
	const struct SceneEvent		*pre;
	const struct SceneEvent		*post;
	const struct SceneEvent		*mid;
	s32							 aid;
	s32							 tid;
	s32							 ritan;
	s32							 group;
	s32							 cost;
	s32							 no;

	group = BtlActGet(act_no)->type_0c;
	aid = ACT_UNIT(code);
	ObjectTable_Get(gGameState.selected_actor);
	ritan = 0;
	Battle_InitializeRenderObject();
	GameFlag_ClearBitFar(FLG_ITEM_USE);
	if (aid == UNIT_NONE)
		aid = 0;

	// 効かないとき
	if (GameFlag_TestFar(FLG_ENA_NONE))
	{
		ENA_MES(aid, act_no, MsgNothingHappened);
		return 0;
	}
	if (work->battle_mode == MODE_WORLD_MAP && act_no == ENA_IMAJIN)
	{
		ENA_MES(aid, ENA_IMAJIN, MsgNothingHappened);
		return 0;
	}

	// リターン：確かめてから戻り先へ
	if (act_no == ENA_RITAN)
	{
		if (GameFlag_TestFar(FLG_RITAN_NG))
		{
			ENA_MES(aid, ENA_RITAN, MsgDoesNotWorkHere);
			return 0;
		}
		UiWork_PushValueSlotFar(ENA_RITAN, 4);
		UiText_ShowPositionedMessageAndWaitFar((s32)MsgUseAbilityConfirm, 13);
		no = Object_CallSpawnRoutineAtOrigin(1); // 0 = はい
		UiWork_FinalizePendingCoreFar();
		if (no != 0)
			return 0;
		{
			struct GameState *st = &gGameState;
			s32				  scene, ent;
			scene = (u16)st->retreat_scene;
			st->scene = scene;
			ent = (u16)st->retreat_entrance;
			st->entrance = ent;
		}
		work->result_code = EXIT_RITAN;
		ritan = 1;
	}
	if (code & ACT_PACKED)
		return BattleFx_ExecutePackedAbilityEffect(code);

	// 仲間ならEPを払う
	if (aid <= MIKATA_MAX)
	{
		cost = BtlActGet(act_no)->pp_cost;
		if (UnitGetFar(aid)->ep < cost)
		{
			ENA_MES(aid, act_no, MsgNotEnoughPp);
			if (ritan)
				work->result_code = 0;
			return 0;
		}
		UnitEpAddFar(aid, -cost);
	}

	// マップのイベントを探し、演出の方法を決める
	pre = (const struct SceneEvent *)BattleFx_FindMatchingEvent(ENA_EV(1), group, &tid);
	post = (const struct SceneEvent *)BattleFx_FindMatchingEvent(ENA_EV(0), group, &tid);
	mid = (const struct SceneEvent *)BattleFx_FindMatchingEvent(ENA_EV(5), group, &tid);
	tid = -1;
	GameFlag_SetBitFar(FLG_ENA_EFX);
	GameFlag_SetBitFar(FLG_ENA_TGT);
	if (pre || post || mid)
	{
		tid = BattleEffect_SelectNearbyTargetObject(gGameState.selected_actor, group);
		if (post && (post->trigger & 0x400)) // イベントが演出を受け持つ
		{
			GameFlag_ClearBitFar(FLG_ENA_EFX);
			GameFlag_ClearBitFar(FLG_ENA_TGT);
		}
	}
	else
		GameFlag_ClearBitFar(FLG_ENA_TGT);

	// 演出
	if (work->battle_mode == MODE_WORLD_MAP)
		BattleEffect_ClearOutOfBoundsObjects();
	BattleFx_LoadActionEffectResources(act_no, 0);
	work->resolving_action = 1;
	BattleFx_SetupObjectPair(gGameState.selected_actor, tid);
	EventObject_Initialize();

	BattleFx_RunEventAction(pre, aid, tid);
	if (GameFlag_TestFar(FLG_ENA_EFX))
	{
		if (GameFlag_TestFar(FLG_ENA_TGT))
			BattleFx_DispatchRequestKind();
		else
			BattleFx_Run();
	}
	EffectRuntime_StopCurrentObject();
	BattleFx_RunEventAction(post, aid, tid);
	if (GameFlag_TestFar(FLG_ENA_EFX))
		BattleFx_ClearChildValueOnMismatch();
	GameFlag_ClearBitFar(FLG_ENA_EFX);
	GameFlag_ClearBitFar(FLG_ENA_TGT);
	work->resolving_action = 0;
	BattleEffect_CleanupSceneObjects();
	if (work->battle_mode == MODE_WORLD_MAP)
		BattleEffect_ClearAllObjects();
	return 0;
}

//--------------------------------------------------------------------------
// このエナジーに応えるイベントがマップにあるか
//--------------------------------------------------------------------------
s32 BattleFx_HasMatchingEvent5(s32 group)
{
	s32 tid;
	s32 ev = BattleFx_FindMatchingEvent(ENA_EV_ANY, (u16)group, &tid);
	return (u32)((-ev) | ev) >> 31;
}

//--------------------------------------------------------------------------
// この道具に応えるイベントが目の前にあるか
//--------------------------------------------------------------------------
u32 BattleFx_HasTrigger(u16 item)
{
	s32 ev;

	ev = Event_FindFacingTrigger(item);
	return (u32)((0 - ev) | ev) >> 31;
}

//--------------------------------------------------------------------------
// 物体を地面の高さに合わせる
//--------------------------------------------------------------------------
void ObjectMotion_SnapToTerrain(void *object)
{
	struct ObjectRuntime *obj = object;
	s32					  height;

	height = Map_GetTerrainHeightFar(0, obj->x, obj->z);
	obj->y = height;
	obj->terrain_height = height;
}

//--------------------------------------------------------------------------
// マップのマーカー物体
//--------------------------------------------------------------------------
#define MARKER_MAX 10

// マップデータの先頭
struct MarkerMap
{
	u8	unknown_00[0x10];
	u8 *markers; // (列, 行, 番号) の並び、255, 255 で終わる
};

struct MarkerSlot
{
	struct ScriptObjectRuntime *object;
	u8							id; // イベントのトリガー番号
	u8							unknown_05;
	u8							column; // 16 ドットのマス
	u8							row;
};

// イベントワークのうち、マーカーの記録
struct MarkerWork
{
	u8				  unknown_000[0x11c];
	struct MarkerSlot slots[MARKER_MAX];
};

struct ScriptObjectRuntime *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlagsFar(struct ScriptObjectRuntime *object, s32 value);
void ObjectDispatch_ReleaseFar(struct ScriptObjectRuntime *object);
void ObjectDispatch_ApplyArgumentToChildrenFar(struct ScriptObjectRuntime *object, s32 mode);
void ObjectDispatch_InitFromTable6Far(struct ScriptObjectRuntime *object);

//--------------------------------------------------------------------------
// マーカーの位置に、対応するイベントの物体を置く
// 番号 100-239 のマーカーだけを扱い、最大 10 個まで記録する
//--------------------------------------------------------------------------
void Battle_PlaceMapMarkers(void)
{
	u32							id;
	struct MarkerMap		   *map = gMapWork[0];
	struct MarkerWork		   *work = gMapWork[HEAP_SLOT_EVENT - 8];
	s32							count = 0;
	u8						   *list = map->markers;
	struct MarkerSlot		   *slot = work->slots;
	volatile u32				fill;
	const struct SceneEvent	   *event;
	struct ScriptObjectRuntime *obj;
	u32							column;
	u32							row;

	fill = 0;
	Dma_Set((void *)&fill, slot,
		DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | (sizeof(struct MarkerSlot) * MARKER_MAX / 4),
		REG_DMA3);
	if (list == NULL)
		return;

	column = *list++;
	row = *list++;
	while (column != 255 || row != 255)
	{
		id = *list++;
		if (id < 100 || id > 239)
			goto next;
		event = gOverlayArea.events();
		for (; event->control != -1; event++)
		{
			if (event->trigger != id)
				continue;
			if ((event->control & 0x1ff) == 19)
			{
				// マスの中心に物体 20 を置く
				obj = Object_CreateFar(20, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
				if (obj == NULL)
					continue;
				ObjectMotion_SnapToTerrain(obj);
				ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
				if (GameFlag_TestFar(event->condition))
				{
					if ((event->value & 0xfff00000) == 0x500000)
					{
						ObjectDispatch_ReleaseFar(obj);
						continue;
					}
					ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
				}
				ObjectDispatch_InitFromTable6Far(obj);
				obj->home_x = obj->x / 0x10000;
				obj->home_z = obj->z / 0x10000;
				obj->unknown_23 = 1;
				obj->flags_59 = 1;
				slot->id = event->trigger;
				slot->object = obj;
				slot->column = obj->x / 0x100000;
				slot->row = obj->z / 0x100000;
				slot++;
				count++;
				if (count >= MARKER_MAX)
					return;
			}
			else if ((event->control & 0x1ff) == 3)
			{
				// フラグが立っていなければ、物体 28 を置く
				if ((event->value & 0xfff00000) != 0x300000)
					continue;
				if (GameFlag_TestFar(event->condition))
					continue;
				obj = Object_CreateFar(28, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
				if (obj == NULL)
					continue;
				ObjectMotion_SnapToTerrain(obj);
				ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
				ObjectDispatch_InitFromTable6Far(obj);
				ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
				obj->home_x = obj->x / 0x10000;
				obj->home_z = obj->z / 0x10000;
				obj->flags_59 = 1;
				obj->unknown_23 = 1;
				slot->object = obj;
				slot->id = event->trigger;
				slot->column = obj->x / 0x100000;
				slot->row = obj->z / 0x100000;
				slot++;
				count++;
				if (count >= MARKER_MAX)
					return;
			}
			else
			{
				continue;
			}
			break;
		}
	next:
		column = *list++;
		row = *list++;
	}
}
