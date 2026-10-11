//--------------------------------------------------------------------------
// nearobj.c
// 向いている方の近くにあるオブジェクトを選ぶ
//--------------------------------------------------------------------------

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
#include "object_runtime.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "object_lookup.h"
#include "scene.h"
#include "effect_runtime.h"
#include "global_cells.h"
#include "battle_effect_runtime.h"
#include "object_effect.h"
#include "btlsys.h"
#include "fieldrun.h"
#include "item.h"
#include "game_state.h"
#include "battle_unit.h"
#include "motobj.h"
#include "animspr.h"
#include "text/msg_ids.h"
#include "dma.h"
#include "script_object_runtime.h"
#include "io_reg.h"
#include "btlcalc.h"
#include "heap_state.h"

void AudioCommand_PlayFar(s32 cue);

u16 ArcTan2(s32 deltaZ, s32 deltaX);
s32			Object_GetById(u32);

#define OBJ_LAST 66 // 調べるオブジェクト番号の最後

//--------------------------------------------------------------------------
// mode に対する探す距離 (ドット)
// BattleFx_TargetRangeByMode は mode と距離の組の並びで、-1 で終わる。なければ 16
//--------------------------------------------------------------------------
s32 BattleFx_MapKeyThroughTable(s32 mode)
{
	s16 *p = BattleFx_TargetRangeByMode;
	s32	 range = 16;
	s32	 key = *p;

	while (key != -1)
	{
		++p;
		if (mode == key)
		{
			range = *p;
			break;
		}
		++p;
		key = *p;
	}
	return range;
}

//--------------------------------------------------------------------------
// id のオブジェクトにいちばん近いオブジェクトを選ぶ (なければ -1)
// 高さの差 48 ドット未満、距離 32 ドット未満のもの。11 ドットより遠ければ
// 向きの ±0x2fff の中にあるものだけ。unknown_59 の 8 は除き、4 は 10/13 の距離とみる
//--------------------------------------------------------------------------
s32 BattleEffect_SelectNearbyObject(s32 id)
{
	struct ObjectRuntime *src;
	struct ObjectRuntime *obj;
	s32					  best;
	s32					  best_dist;
	s32					  i;
	s32					  dx;
	s32					  dy;
	s32					  dz;
	s32					  xx;
	s32					  yy;
	s32					  zz;
	s32					  sq;
	s32					  dist;
	s32					  angle;

	best = -1;
	best_dist = 32;
	src = ObjectTable_Get(id);
	if (src == 0)
		goto done;

	for (i = 0; i <= OBJ_LAST; i++)
	{
		if (i == id)
			continue;

		obj = ObjectTable_Get(i);
		if (obj == 0 || (obj->unknown_59 & 8) != 0)
			continue;

		if (obj->y - src->y >= 0)
		{
			if (obj->y - src->y >= 48 << 16)
				continue;
		}
		else if (src->y - obj->y >= 48 << 16)
		{
			continue;
		}

		// ドット単位の距離
		dx = (obj->x - src->x) / 0x10000;
		dy = (obj->y - src->y) / 0x10000;
		dz = (obj->z - src->z) / 0x10000;

		xx = dx * dx;
		yy = dy * dy;
		zz = dz * dz;
		sq = xx + yy + zz;
		dist = Iwram_Sqrt(sq);
		if ((obj->unknown_59 & 4) != 0)
			dist = dist * 10 / 13;
		if (dist >= best_dist)
			continue;

		angle = ArcTan2(obj->z - src->z, obj->x - src->x);
		if (dist > 11)
		{
			s32 diff = (s16)(angle - src->angle);

			if (diff < -0x2fff)
				continue;
			if (diff > 0x2fff)
				continue;
		}

		best = i;
		best_dist = dist;
	}

done:
	return best;
}

//--------------------------------------------------------------------------
// id のオブジェクトの前にいちばん近いオブジェクトを選ぶ (なければ -1)
// 向きは 90 度単位に丸め、距離は x と z だけで測る。
// mode : 探す距離と、高さの差・向きの幅を決める
//--------------------------------------------------------------------------
s32 BattleEffect_SelectNearbyTargetObject(s32 id, s32 mode)
{
	struct ObjectRuntime *src;
	struct ObjectRuntime *obj;
	s32					  best;
	s32					  best_dist;
	s32					  dir;
	s32					  i;
	s32					  dy_max;
	s32					  dx;
	s32					  dz;
	s32					  cz;
	s32					  dist;
	s32					  angle;
	s32					  width;

	best = -1;
	best_dist = BattleFx_MapKeyThroughTable(mode);
	src = (struct ObjectRuntime *)ObjectTable_Get(id);
	if (src == 0)
		return best;

	dir = (src->angle + 0x2000) & 0xc000;
	for (i = 0; i <= OBJ_LAST; i++)
	{
		if (i == id)
			continue;

		obj = (struct ObjectRuntime *)ObjectTable_Get(i);
		if (obj == 0 || (obj->unknown_59 & 8) != 0)
			continue;

		// 高さの差 : ふつうは 8、mode 13 は 48、5 は 64、2 は 16 ドットまで
		dy_max = 8 << 16;
		if (mode == 13)
			dy_max = 48 << 16;
		if (mode == 5)
			dy_max = 64 << 16;
		if (mode == 2)
			dy_max = 16 << 16;

		{
			s32 dy = obj->y - src->y;
			if (dy >= 0)
			{
				if (dy > dy_max)
					continue;
			}
			else if (src->y - obj->y > dy_max)
			{
				continue;
			}
		}

		// ドット単位の距離 (0 へ向けて切り捨て)
		dx = obj->x - src->x;
		if (dx < 0)
			dx += 0xffff;
		dx >>= 16;

		dz = obj->z - src->z;
		if (dz < 0)
			dz += 0xffff;
		cz = dz >> 16;

		dist = Iwram_Sqrt(dx * dx + cz * cz);
		if ((obj->unknown_59 & 0x10) != 0)
			dist = dist * 2 / 3;
		if (dist >= best_dist)
			continue;

		// 向きの幅 : 19 ドットまでは ±0x1800、それより遠ければ ±0x1000、mode 2 は ±0x2000
		angle = ArcTan2(obj->z - src->z, obj->x - src->x);
		width = 0x1800;
		if (dist > 19)
			width = 0x1000;
		if (mode == 2)
			width = 0x2000;

		if (dist > 11)
		{
			s32 diff = (s16)(angle - dir);
			if (diff < 0)
				diff = -diff;
			if (diff >= width)
				continue;
		}

		best = i;
		best_dist = dist;
	}

	return best;
}

//--------------------------------------------------------------------------
// target のオブジェクトがいれば音を鳴らして mode 4 にし、12 フレームのちに
// 粒の発生を始める
//--------------------------------------------------------------------------
s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 effect, s32 target, s32 mode)
{
	extern void ObjectDispatch_ApplyArgumentToChildrenFar(struct ParticleEffectObject *, s32);
	s32 obj;
	s32 ret;

	obj = Object_GetById(target);
	ret = 0;
	if (obj != 0)
	{
		AudioCommand_PlayFar(124);
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 4);
		WaitFrames(12);
		ret = BattleFx_StartRandomParticleEmitter(effect, mode);
	}
	return ret;
}

struct BtlAct		  *BtlActGet(s32 action_id);
struct ItemDefinition *Item_GetDirectFar(s32 item);

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
#define gFrameCount (*(u32 (*))&gFrameCount)
#endif
#define ObjectTable_Get ((void *(*)(u32))ObjectTable_Get)
s32	  GetFocusedObjectCollision(void);

extern struct BattleRuntime *gEventWork;

struct ItemCommandRuntime
{
	u8	unknown_000[0x170];
	s16 result_code;
	u8	unknown_172[0xb54];
	u8	resolving_action;
};

typedef s32 (*BattleItemCallback)(s32 item, s32 actor, s32 slot);

s32	 Party_CountActiveOwnersFar(void);
void GameFlag_ClearBitFar(s32 flag);
void GameFlag_SetBitFar(s32 flag);
void UiWork_PushValueSlotFar(s32 value, s32 mode);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void UiWork_FinalizePendingCoreFar(void);
void Battle_Reset(void);
void Event_SetMessageId(s32 effect_id);

void BattleFx_FinishAction(void);

s32	 BattleFx_LoadActionEffectResources(s32 action_id, s32 mode);
void BattleFx_Run(void);
void BattleEffect_CleanupSceneObjects(void);
u8	 Inventory_RemoveFar(s32 actor, s32 slot);

void BattleFx_SetupObjectPair(s32, s32);
void EffectRuntime_StopCurrentObject(void);

void Animation_ApplyChildPalette(struct MotionObject *obj, s32 palette)
{
	struct AnimationObject *state;
	struct AnimationEntry **p;
	struct AnimationEntry  *current;
	s32						cnt;
	s32						replacement;
	u8						n;

	if ((obj->record_storage_kind & 15) == 1)
	{
		state = obj->records;
		replacement = palette - 1;
		if (palette == 0)
			replacement = Animation_ChildPaletteCycle[(gFrameCount >> 1) & 7];

		n = state->count;
		if (n != 0)
		{
			p = state->entries;
			cnt = n;
			do
			{
				current = *p++;
				if (current != 0 && current->script != 0 && current->param != 15)
					current->param = replacement;
				cnt--;
			} while (cnt != 0);
		}
		state->dirty = 1;
	}
}

void Battle_ResetEffectCounter(void)
{
	struct BattleRuntime *runtime = gEventWork;

	runtime->unknown_cb6 = 0;
	if (*(s16 *)runtime->unknown_cb8 != 0)
	{
		BattleFx_ExecutePackedAbilityEffect(0x2090);
	}
}

s32 Event_FindFacingTrigger(s32 source)
{
	struct FieldStepWork	*runtime = (struct FieldStepWork *)gEventWork;
	const struct SceneEvent *trigger = runtime->events;
	s32 facing = ((struct MotionObject *)ObjectTable_Get(gGameState.selected_actor))->angle;
	s32 selected = BattleEffect_SelectNearbyObject(gGameState.selected_actor);
	s32 collision;

	source &= 0x1ff;
	collision = GetFocusedObjectCollision();

	while (trigger->control != -1)
	{
		s32 high_value = trigger->trigger & 0xf000;
		s16 has_facing = trigger->trigger & 0x0800;
		s32 low_value = trigger->trigger & 0xff;

		if ((trigger->control & 0x0f) == 4 && GameFlag_IsConditionActive(trigger->condition) != 0
			&& (has_facing == 0 || (u16)(high_value - facing + 0x17ff) <= 0x2ffe))
		{
			u32 flags = trigger->control;
			s32 owner = ((const u8 *)&trigger->control)[1];

			if (source == 0 || owner == source)
			{
				if ((flags & 0x10) != 0)
				{
					if (low_value == selected)
						return (s32)trigger;
				}
				else if (low_value == collision)
				{
					return (s32)trigger;
				}
			}
		}
		trigger++;
	}
	return 0;
}

s32 BattleCommand_ExecuteSelectedItem(s32 arg, s32 slot)
{
	s32						   result;
	s32						   item_id;
	s32						   actor;
	struct BtlUnit			  *obj;
	const struct SceneEvent	  *event;
	s32						   j;
	s32						   i;
	s32						   best;
	s32						   matches;
	s32						   count;
	struct ItemCommandRuntime *runtime;

	result = -1;
	item_id = arg & 0x3ff;
	actor = (arg >> 10) & 0xf;
	{
		best = 0;
		count = Party_CountActiveOwnersFar();

		if (actor == 15)
		{
			actor = 0;
			for (i = 0; i < count; i++)
			{
				obj = UnitGetFar(gGameState.active_owners[i]);
				matches = 0;
				for (j = 0; j < 15; j++)
				{
					if ((obj->inventory[j] & 0x1ff) == item_id)
						matches++;
				}

				if (best < matches)
				{
					best = matches;
					actor = gGameState.active_owners[i];
				}
			}
		}
		else
		{
			obj = UnitGetFar(actor);
			for (j = 0; j < 15; j++)
			{
				if ((obj->inventory[j] & 0x1ff) == item_id)
					best++;
			}
		}

		if (best == 0)
		{
			UiText_ShowPositionedMessageAndWaitFar((s32)MsgNothingHappens, 1);
			return -1;
		}
	}

	event = (const struct SceneEvent *)Event_FindFacingTrigger(item_id);
	if (event != 0 && event->value != 0)
	{
		GameFlag_ClearBitFar(0x143);
		GameFlag_ClearBitFar(0x142);
		if (!(event->trigger & 0x400))
		{
			UiWork_PushValueSlotFar(actor, 1);
			UiWork_PushValueSlotFar(item_id, 2);
			UiText_ShowPositionedMessageAndWaitFar((s32)MsgActorUsesItem, 1);
		}
		if ((s32)event->value < 0x10000)
		{
			s32 objref = BattleEffect_SelectNearbyObject(gGameState.selected_actor);
			Battle_Reset();
			Event_SetMessageId(event->value);
			BattleEv_RunWait(objref, 0);
			BattleFx_FinishAction();
		}
		else
		{
			((BattleItemCallback)event->value)(item_id, actor, slot);
		}
		result = 0;
	}
	else
	{
		s32 action_id;

		GameFlag_ClearBitFar(0x143);
		GameFlag_SetBitFar(0x142);
		action_id = Item_GetDirectFar(item_id)->action_id;
		runtime = (struct ItemCommandRuntime *)gEventWork;

		if (action_id != 0)
		{
			GameFlag_SetBitFar(0x145);
			GameFlag_ClearBitFar(0x142);

			if (action_id == 149 && !GameFlag_TestFar(0x144))
			{
				UiWork_PushValueSlotFar(item_id, 2);
				UiText_ShowPositionedMessageAndWaitFar((s32)MsgUseItemQuestion, 13);
				/* FAKEMATCH: needed to match: the owner index doubles as the declined flag, which ranks it above best in global allocation (r6, best r7) */
				i = Object_CallSpawnRoutineAtOrigin(1);
				UiWork_FinalizePendingCoreFar();
				if (i != 0)
					return 0;

				gGameState.scene = gGameState.retreat_scene;
				gGameState.entrance = gGameState.retreat_entrance;
				runtime->result_code = 999;
			}

			UiWork_PushValueSlotFar(actor, 1);
			UiWork_PushValueSlotFar(item_id, 2);
			UiText_ShowPositionedMessageAndWaitFar((s32)MsgActorUsesItem, 1);
			BattleFx_LoadActionEffectResources(action_id, 0);
			runtime->resolving_action = 1;
			BattleFx_Run();
			runtime->resolving_action = 0;
			BattleEffect_CleanupSceneObjects();

			if (Item_GetDirectFar(item_id)->use_type & 1)
				GameFlag_SetBitFar(0x143);
		}
	}

	if (GameFlag_TestFar(0x142))
		UiText_ShowPositionedMessageAndWaitFar((s32)MsgNothingHappens, 1);
	if (GameFlag_TestFar(0x143))
		Inventory_RemoveFar(actor, slot);
	return result;
}

s32 BattleFx_FindMatchingEvent(s32 requested_flags, s32 group, void *result)
{
	struct FieldStepWork	*runtime = (struct FieldStepWork *)gEventWork;
	const struct SceneEvent *event = runtime->events;
	s32 reference = ((struct MotionObject *)ObjectTable_Get(gGameState.selected_actor))->angle;
	s32 selected = BattleEffect_SelectNearbyTargetObject(gGameState.selected_actor, group);
	s32 alternate;
	s32 ignore_flags = 0;

	*(s32 *)result = selected;
	alternate = GetFocusedObjectCollision();
	if (requested_flags == 0x70000005)
		ignore_flags = 1;

	while (event->control != -1)
	{
		s32 high_value = event->trigger & 0xf000;
		s16 has_reference = event->trigger & 0x0800;
		s32 low_value = event->trigger & 0xff;

		if ((event->control & 0x0f) == 5 && GameFlag_IsConditionActive(event->condition) != 0
			&& (has_reference == 0 || (u16)(high_value - reference + 0x17ff) <= 0x2ffe)
			&& BtlActGet(((const u8 *)&event->control)[1])->type_0c == group
			&& (ignore_flags || (event->control & 0x7000000f) == requested_flags))
		{
			if ((event->control & 0x80) != 0)
				return (s32)event;
			if ((event->control & 0x10) != 0)
			{
				if (low_value == selected)
					return (s32)event;
			}
			else if (low_value == alternate)
			{
				return (s32)event;
			}
		}
		event++;
	}
	return 0;
}

s32 BattleFx_ExecutePackedAbilityEffect(s32 packed)
{
	s32	  output;
	s32	  index;
	s32	  mode;
	u8	  object;
	void *first;
	void *second;

	index = packed & 0x3FF;
	mode = ((u32)packed >> 10) & 0xF;
	object = BtlActGet(index)->type_0c;
	// The object is looked up and the result is not used.
	ObjectTable_Get(gGameState.selected_actor);
	first = (void *)BattleFx_FindMatchingEvent(0x30000005, object, &output);
	second = (void *)BattleFx_FindMatchingEvent(0x20000005, object, &output);
	BattleFx_LoadActionEffectResources(index, 0);
	BattleFx_SetupObjectPair(gGameState.selected_actor, output);
	BattleFx_RunEventAction(first, mode, output);
	FieldEvent_RunTypeHandler();
	EffectRuntime_StopCurrentObject();
	BattleFx_RunEventAction(second, mode, output);
	BattleEffect_CleanupSceneObjects();
	return 0;
}

#undef gFrameCount
#undef ObjectTable_Get

//--------------------------------------------------------------------------
// action.c
// フィールドでのエナジー使用と、マップのマーカー物体の配置
//--------------------------------------------------------------------------
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
#define ObjectTable_Get ((void *(*)(s32))ObjectTable_Get)
void  GameFlag_ClearBitFar(s32);
void  UiWork_PushValueSlotFar(s32, s32);
void  UiText_ShowPositionedMessageAndWaitFar(s32, s32);

s32	 BattleFx_FindMatchingEvent(s32, s32, void *);
void GameFlag_SetBitFar(s32);
#define BattleFx_LoadActionEffectResources ((void (*)(s32, s32))BattleFx_LoadActionEffectResources)
void EventObject_Initialize(void);

void BattleFx_DispatchRequestKind(void);

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
#define Object_CreateFar ((struct ScriptObjectRuntime *(*)(s32 kind, s32 x, s32 y, s32 z))Object_CreateFar)
void ObjectDispatch_SetSingleChildFlagsFar(struct ScriptObjectRuntime *object, s32 value);
void ObjectDispatch_ReleaseFar(struct ScriptObjectRuntime *object);
void ObjectDispatch_InitFromTable6Far(struct ScriptObjectRuntime *object);

//--------------------------------------------------------------------------
// マーカーの位置に、対応するイベントの物体を置く
// 番号 100-239 のマーカーだけを扱い、最大 10 個まで記録する
//--------------------------------------------------------------------------
void Battle_PlaceMapMarkers(void)
{
	void ObjectDispatch_ApplyArgumentToChildrenFar(struct ScriptObjectRuntime *object, s32 mode);
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

#undef ObjectTable_Get
#undef BattleFx_LoadActionEffectResources
#undef Object_CreateFar
