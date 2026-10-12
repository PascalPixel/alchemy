#include "unit/battle/effect/btlcue.h"
#include "unit/battle/effect/zonedata.h"
#include "vram_tab.h"
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
#include "unit/field/common/effect/move.h"
#include "unit/graphics/display/scroll_initialize_hblank_dma.h"
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
#include "scene.h"
#include "btltype.h"
#include "game_state.h"
#include "fieldrun.h"
#include "btlsys.h"
#include "battle.h"
#include "battle_unit.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "iowrite.h"
#include "irq.h"
#include "callback_scheduler.h"
#include "flag_ids.h"
#include "iwram_call.h"
#include "eventwrk.h"
#include "resource_ids.h"
#include "battle_effect_runtime.h"
#include "resource.h"
#include "field_scene.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "fieldobj.h"
#include "object_dispatch.h"
#include "field_event.h"
#include "event_runtime.h"
#include "field_sprite.h"
#include "battle_effect_counters.h"
#include "heap_state.h"
#include "keystate.h"
#include "ownerval.h"
#include "party_state.h"
#include "sound_ids.h"
#include "fixed_math.h"
#include "itemgift.h"
#include "btlcalc.h"
#include "disptran.h"
#include "text/msg_ids.h"
#include "object_lookup.h"
#include "objdisp.h"
#include "effect_runtime.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "layout_guard.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "vram_block.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "btlpty.h"
#include "b5_context.h"
#include "edition.h"
#include "facing_object.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "fieldcoo.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_scene.h"
#include "effect_slot.h"
#include "object_efx.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"

// イベントのワーク (gEventWork) のこのファイルでの見え方。共通の EventWork に欄のない 0xcb6 以降も持つ
struct FieldWork
{
	u8				   unknown_000[0x10];
	s32				   view;
	struct FieldActor *actors[86];
	s16				   action;
	s16				   input;
	s16				   exit;
	s16				   menu;
	s16				   examine;
	s16				   start;
	u16				   descriptor;
	s16				   tile;
	s16				   scene;
	s16				   command;
	s16				   item;
	s16				   event;
	s16				   count;
	s16				   survived;
	s16				   quantities[9];
	s16				   item_argument;
	u8				   unknown_19c[2];
	s16				   mode;
	u8				   unknown_1a0[8];
	s32				   unknown_1a8;
	u8				   unknown_1ac[4];
	s32				   unknown_1b0;
	s32				   unknown_1b4;
	u8				   unknown_1b8[4];
	u8				  *cells;
	s32				   transition;
	u16				   unknown_1c4;
	u16				   transition_active;
	s32				   transition_frames;
	u8				   unknown_1cc[0xaea];
	u16				   busy;
	s16				   effect_active;	// 効果が動いていれば 0 以外
	s16				   effect_frames;	// 残りのフレーム
	s16				   effect_center_x; // 効果の中心 (ドット)
	s16				   effect_center_y;
	s16				   unknown_cc0;
	u8				   unknown_cc2[4];
	s8				   resolving_action; // エナジーの演出中
	s8				   unknown_cc7;
	s16				   queued_sound;
};

// 演出スロット 24 個を動かす
static __inline__ void UpdateAllEffectSlots(void)
{
	struct EffectSlot *slot;
	s32				   n;

	slot = ((struct BattleFxScene *)gEffectWork)->slots;
	n = 23;
	do
	{
		n -= 1;
		EffectSlot_Update(slot);
		slot++;
	} while (n >= 0);
}

//--------------------------------------------------------------------------
// kind に応じた演出の処理を呼ぶ
// kind : 0 対象を指定せずに実行、1 指定の対象 (second_object_id) に実行、2 終わり、3 種類ごとの処理
//--------------------------------------------------------------------------
void Field_DispatchTypeHandler(s32 kind)
{
	if (kind == 0)
		BattleFx_Run();
	else if (kind == 1)
		BattleFx_DispatchRequestKind();
	else if (kind == 2)
		BattleFx_ClearChildValueOnMismatch();
	else if (kind == 3)
		FieldEvent_RunTypeHandler();
}

//--------------------------------------------------------------------------
// effect.c
// フィールド効果：種類ごとの振り分けと、効果のイベント処理
//--------------------------------------------------------------------------

#define HEAP_SLOT_FX 56 // エナジー演出のワーク (gWorkSlot[56] は gEffectWork)

void _call_via_r3(s32, s32);

//--------------------------------------------------------------------------
// 効果の種類 (animation) ごとに本体を走らせる
//--------------------------------------------------------------------------
void BattleFx_Run(void)
{
	struct BattleFxScene *scene;
	struct BattleRuntime *runtime;
	s32					  kind;
	s32					  target_id;
	s32					  obj_id;

	scene = *(gWorkSlot + HEAP_SLOT_FX);
	runtime = *(gWorkSlot + HEAP_SLOT_EVENT);
	kind = scene->animation;
	target_id = scene->second_object_id;

	switch (kind)
	{
	case 1:
		BattleFx_RunItemBreakSequence();
		return;
	case 7:
		RunBattleEffect07();
		return;
	case 11:
		RunBattleEffect11();
		return;
	case 4:
		RunBattleEffect04();
		return;
	case 5:
		RunBattleEffect05();
		return;
	case 14:
		RunBattleEffect14();
		return;
	case 6:
		BattleFx_RunOrbitingParticles();
		return;
	case 3:
		RunBattleEffect03();
		return;
	case 12:
		BattleFx_RunBurstParticles();
		return;
	case 13:
		RunBattleEffect13();
		return;
	case 9: // 前に止めた対象を戻し、近くの対象を止める
		if (gGameState.paused_object != -1)
		{
			BattleFx_ResumeObject(gGameState.paused_object);
			gGameState.paused_object = -1;
		}

		obj_id = BattleEffect_SelectNearbyTargetObject(gGameState.selected_actor, kind);
		obj_id = BattleFx_FilterObjectIdByFlags(obj_id);
		if (BattleFx_FindDescriptorWithOverride(obj_id) != 0)
		{
			BattleFx_SetupObjectPair(gGameState.selected_actor, obj_id);
			BattleFx_MarkChildAndRunFallbackTransition(obj_id);
			BattleEffect_PauseObject(obj_id);
			gGameState.paused_object = obj_id;
		}
		else
		{
			BattleEffect_RunFallbackObjectTransition();
		}
		return;
	case 2: // 波と色の効果 (種類 8 の効果が動いていれば先に止める)
		if (*(s16 *)&runtime->unknown_cb8[0] != 0)
			ResetSceneTransitionEffect();
		RunSceneTransitionEffect(scene->first_object_id, target_id);
		return;
	case 8:
		RunBattleEffect08();
		return;
	case 10:
		BattleFx_RunFlashingCallbackSequence();
		return;
	case 15:
		BattleFx_RunEffect15();
		return;
	case 16:
		RunBattleEffect16();
		return;
	}
}

//--------------------------------------------------------------------------
// 対象が決まっているときの振り分け
//--------------------------------------------------------------------------
void BattleFx_DispatchRequestKind(void)
{
	struct BattleFxScene *scene = *(gWorkSlot + HEAP_SLOT_FX);
	struct BattleRuntime *runtime = *(gWorkSlot + HEAP_SLOT_EVENT);
	s32					  kind = scene->animation;
	s32					  target_id = scene->second_object_id;

	scene->enabled = 0;
	switch (kind)
	{
	case 2:
		if (*(s16 *)&runtime->unknown_cb8[0] != 0)
			ResetSceneTransitionEffect();
		if (gGameState.paused_object != scene->second_object_id)
			((struct ObjectRuntime *)scene->child)->movement_state = 1;
		RunSceneTransitionEffect(scene->first_object_id, target_id);
		break;
	case 1: // 方向キーで対象を押す
		BattleFx_SteerPushedTarget(target_id);
		break;
	case 7:
		BattleEffect_RunTargetedItemBreak(target_id);
		break;
	case 11:
		BattleFx_SteerLiftedTarget(target_id);
		break;
	case 4:
		BattleFx_CallEffect04(target_id);
		break;
	case 5:
		BattleFx_CallEffect05(target_id);
		break;
	case 6:
		BattleFx_StartOrbitingParticles(target_id);
		break;
	case 12:
		BattleFx_RunBurstParticleMainObject(target_id);
		break;
	case 9:
		if (gGameState.paused_object != -1)
		{
			BattleFx_ResumeObject(gGameState.paused_object);
			gGameState.paused_object = -1;
		}
		BattleEffect_PauseObject(target_id);
		gGameState.paused_object = target_id;
		BattleFx_MarkChildAndRunFallbackTransition(target_id);
		break;
	case 3:
		BattleFx_CallEffect03AndStop(target_id);
		break;
	case 14:
		BattleFx_CallEffect14(target_id);
		break;
	case 13:
		BattleFx_RunEffect13Hook(target_id);
		break;
	case 8:
		RunBattleEffect08();
		break;
	case 10:
		BattleFx_RunFlashingCallbackSequence();
		break;
	case 15:
		BattleFx_RunEffect15();
		break;
	case 16:
		RunBattleEffect16();
		break;
	}
}

//--------------------------------------------------------------------------
// 種類 2 の後始末：効果を終え、止めていない対象の movement_state を戻す
//--------------------------------------------------------------------------
void BattleFx_ClearChildValueOnMismatch(void)
{
	struct BattleFxScene *scene = gEffectWork;

	if (scene->animation == 2)
	{
		BattleFx_FinishSceneAndReleaseHeapBlock();
		if (gGameState.paused_object != scene->second_object_id)
		{
			((struct ObjectRuntime *)scene->child)->movement_state = 0;
		}
	}
}

//--------------------------------------------------------------------------
// 種類 8, 10, 16 の追加処理
//--------------------------------------------------------------------------
void FieldEvent_RunTypeHandler(void)
{
	u32 kind;

	kind = ((struct BattleFxScene *)gEffectWork)->animation;
	switch (kind)
	{
	case 8:
		ResetSceneTransitionEffect();
		return;
	case 10:
		MapEvent_RunTileTriggerSequence();
		return;
	case 16:
		FieldEvent_ShowStatusMessage();
		return;
	}
}

//--------------------------------------------------------------------------
// 効果に結びついたイベントを実行する
// event  : オフセット 8 の値が 0x10000 未満ならメッセージ番号、それ以上なら処理の番地
// param  : 処理へ渡す値 (使った者など)
// target : 対象
// フラグ 322 が立っていれば「何も起こらない」を出す
//--------------------------------------------------------------------------
s32 BattleFx_RunEventAction(void *event, s32 param, s32 target)
{
	s32 script;

	if (event != NULL)
	{
		script = ((struct EffectDescriptor *)event)->result;
		if (script != 0)
		{
			if (script < 0x10000)
			{
				Battle_Reset();
				Event_SetMessageId(((struct EffectDescriptor *)event)->result);
				BattleEv_RunWait(target, 0);
				BattleFx_FinishAction();
			}
			else
			{
				_call_via_r3(param, target);
			}
		}
		if (GameFlag_TestFar(322) != 0)
		{
			Battle_Reset();
			UiText_ShowPositionedMessageAndWaitFar((s32)MsgNothingHappens, 1);
			BattleFx_FinishAction();
		}
	}
	return 0;
}

//--------------------------------------------------------------------------
// 各パーツのパレットをフレーム数 % 6 にする (毎フレーム呼ぶと色が巡る)
//--------------------------------------------------------------------------
void ObjectGroup_ApplyRandomChildValues(void *owner)
{
	s32			  kind;
	void		 *anim;
	s32			  initial_count;
	s32			  cnt;
	void		**entry;
	void		 *part;
	volatile s32 *frame_count;
	s32			  value;

	kind = ((struct ObjectRuntime *)owner)->animation_kind;
	if (kind == 1)
	{
		anim = ((struct ObjectRuntime *)owner)->animation;
		if (anim != 0 && (((struct AnimationObject *)anim)->display_flags & kind) == 0)
		{
			initial_count = ((struct AnimationObject *)anim)->count;
			if (initial_count != 0)
			{
				frame_count = (volatile s32 *)&gFrameCount;
				entry = (void **)((struct AnimationObject *)anim)->entries;
				cnt = initial_count;
				do
				{
					part = *entry++;
					value = (u32)*frame_count % 6;
					cnt--;
					((struct AnimationEntry *)part)->param = value;
				} while (cnt != 0);
			}
			((struct AnimationObject *)anim)->dirty = 1;
		}
	}
}

//--------------------------------------------------------------------------
// オブジェクトを向き angle へ magnitude だけずらした位置に置く
//--------------------------------------------------------------------------
void Motion_SetTargetPositionFromMagnitudeAngle(
	struct ObjectRuntime *object, s32 magnitude, s32 angle)
{
	s32 pos[3];

	if (object != 0)
	{
		pos[0] = object->x;
		pos[1] = object->y;
		pos[2] = object->z;
		Vector_AddPolarOffset(magnitude, angle, pos);
		Object_SetMoveTargetFar(object, pos[0], pos[1], pos[2]);
	}
}

// 以下では物体を関数ごとの構造体で渡す

s32 ResourceTable_CountFreeBlocks(void)
{
	u8 *marker = ResourceBlockOwners;
	s32 free_count = 0;
	s32 remaining = 0x200;

	do
	{
		if (*marker++ == 0xff)
		{
			free_count++;
		}
		remaining--;
	} while (remaining != 0);
	return free_count;
}

void *Object_ReplaceResourceEntry(void *src, void *alt)
{
	void			   *ret;
	struct FieldSprite *obj;

	obj = src;
	ret = NULL;
	if (obj != NULL)
	{
		if (alt == NULL)
		{
			obj->unknown_1d_0 = 1;
		}
		else
		{
			Resource_ResetEntry(obj->vram_block);
			obj->vram_block = ((struct FieldSprite *)alt)->vram_block;
			obj->unknown_1d_0 = 1;
			obj = alt;
		}
		ret = obj;
	}
	return ret;
}

//--------------------------------------------------------------------------
// arcupdat.c
// エナジー演出：演出用の物体、ほかの物体の一括設定、ふくらむ弧の動き
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 演出用の物体を出す
// アニメのない物体は消して NULL。高さは使う者に合わせ、優先度は 0
//--------------------------------------------------------------------------
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z)
{
	struct BattleFxScene *work = gEffectWork;
	struct ObjectRuntime *obj;

	obj = Object_CreateFar(kind, x, y, z);
	if (obj != NULL)
	{
		if (obj->animation_kind == 0)
		{
			ObjectDispatch_ReleaseFar(obj);
			return NULL;
		}
		obj->terrain_height = ((struct ObjectRuntime *)work->main_object)->terrain_height;
		obj->flags = 4;
		obj->unknown_23 = 4;
		((struct AnimationObject *)obj->animation)->part[0].priority = 0;
		ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
	}
	return obj;
}

//--------------------------------------------------------------------------
// 主人公と except 以外のすべての物体に、動きの状態と値を設定する
//--------------------------------------------------------------------------
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *except, s32 state, s32 value)
{
	s16					 *player;
	struct ObjectRuntime *obj;
	s32					  id;

	id = 0;
	player = (s16 *)gPlayerObjectId;
	do
	{
		obj = ObjectTable_Get(id);
		if (id != *player && obj != NULL && obj != except)
		{
			obj->movement_state = state;
			ObjectDispatch_ApplyValueToChildrenFar(obj, value);
		}
		id++;
	} while (id <= OBJ_LAST);
}

//--------------------------------------------------------------------------
// 弧 A：32 フレームでふくらんでしぼみ、毎フレーム 1 ドット上がる
// z は物体の 9 ドット先から、小さいほど 5 ドットまで離れる
//--------------------------------------------------------------------------
void BattleFx_UpdateScaledArcObjectA(struct FieldActor *obj)
{
	struct FieldActor *link;
	s32				   v;

	link = obj->linked_object;
	v = ++obj->timer;
	if (v > 31)
	{
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
		return;
	}
	v = Trig_Sin(v << 10);
	obj->scale_x = v;
	obj->scale_y = v;
	obj->x.fixed = link->x.fixed;
	obj->y.fixed += 0x10000;
	obj->z.fixed = link->z.fixed + (0x10000 - v) * 5 + 0x90000;
}

//--------------------------------------------------------------------------
// 弧 B：A の上下を反転したもの。z は物体の 16 ドット先から、小さいほど 5 ドットまで寄る
//--------------------------------------------------------------------------
void BattleFx_UpdateScaledArcObjectB(struct FieldActor *obj)
{
	struct FieldActor *link;
	s32				   v;

	link = obj->linked_object;
	v = ++obj->timer;
	if (v > 31)
	{
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
		return;
	}
	v = Trig_Sin(v << 10);
	obj->scale_x = v;
	obj->scale_y = -v;
	obj->x.fixed = link->x.fixed;
	obj->y.fixed += 0x10000;
	obj->z.fixed = link->z.fixed - (0x10000 - v) * 5 + 0x100000;
}

//--------------------------------------------------------------------------
// actsetup.c
// エナジー演出：ワークの準備、使う者と対象の記録、ふくらむ弧の物体
//--------------------------------------------------------------------------

// スプライト：OAM の三つの属性と管理情報
struct ArcSprite
{
	u8					   pad00[4];
	u16					   y : 8;
	u16					   affine : 1;
	u16					   double_size : 1;
	u16					   mode : 2;
	u16					   mosaic : 1;
	u16					   color : 1; // 1 = 256 色
	u16					   shape : 2;
	u16					   x : 9;
	u16					   unused : 3;
	u16					   hflip : 1;
	u16					   vflip : 1;
	u16					   size : 2;
	u16					   tile : 10;
	u16					   priority : 2;
	u16					   palette : 4;
	u8					   pad0a[18];
	u8					   slot; // VRAM ブロック
	u8					   active : 1;
	u8					   display_flags : 7;
	u8					   pad1e[8];
	u8					   flags;
	u8					   pad27;
	struct AnimationEntry *child;
};

//--------------------------------------------------------------------------
// 物体の位置に二つの弧を出す (B は上下を反転)
// 弧は 32 フレームでふくらんでしぼみ、物体についていく
//--------------------------------------------------------------------------
void BattleFx_SpawnScaledArcObjects(struct FieldActor *linked)
{
	struct BattleFxScene *scene = gEffectWork;
	struct FieldActor	 *source = scene->main_object;
	struct FieldActor	 *objs[2];
	struct FieldActor	 *obj;
	struct ArcSprite	 *spr;
	s32					  i;

	for (i = 0; i <= 1; i++)
	{
		obj = (struct FieldActor *)Object_CreateFar(
			26, linked->x.fixed, linked->y.fixed, linked->z.fixed);
		objs[i] = obj;
		if (obj == 0)
			continue;
		obj->terrain_height = linked->terrain_height;
		spr = obj->sprite;
		obj->motion_flags = 0;
		obj->timer = 0;
		obj->linked_object = linked;
		obj->scale_x = obj->scale_y = 0x1999; // 0.1 倍
		if (spr == 0)
			continue;
		// 演出のタイルを使う 16 色の横長スプライト
		AnimationObjects_SelectAnimationFar((struct AnimationObject *)spr, 0);
		spr->flags = 0;
		Resource_ResetEntry(spr->slot);
		spr->slot = scene->tile_slot;
		spr->active = 1;
		spr->tile = (gVramBlockCache[spr->slot].offset >> 5) & 0x3ff;
		spr->color = 0;
		spr->shape = 1;
		spr->size = 2;
		spr->child->frame = 0;
	}
	// B の弧は最前面、A の弧は使う者と同じ優先度
	objs[0]->update = BattleFx_UpdateScaledArcObjectB;
	objs[0]->sprite->priority = 0;
	objs[1]->update = BattleFx_UpdateScaledArcObjectA;
	objs[1]->sprite->priority = source->sprite->priority;
	objs[1]->priority_flags = 2;
}

//--------------------------------------------------------------------------
// 2 フレームごとに色 7 と 0 を切り替え、16 フレームごとに弧を出す
//--------------------------------------------------------------------------
void BattleFx_FlickerObjectAndTick(s32 obj)
{
	if ((gFrameCount & 2) != 0)
	{
		Animation_ApplyChildValuesFar((struct DispatchObject *)obj, 7);
	}
	else
	{
		Animation_ApplyChildValuesFar((struct DispatchObject *)obj, 0);
	}
	if ((gFrameCount & 15) == 0)
	{
		BattleFx_SpawnScaledArcObjects(obj);
	}
}

//--------------------------------------------------------------------------
// 2 フレームごとに色 0-5 を順に回し、16 フレームごとに弧を出す
//--------------------------------------------------------------------------
void BattleFx_CycleObjectValueByCounter(s32 obj)
{
	if ((gFrameCount & 1) != 0)
	{
		s32 col = (gFrameCount >> 1) % 6;

		Animation_ApplyChildValuesFar((struct DispatchObject *)obj, col);
	}
	if ((gFrameCount & 15) == 0)
	{
		BattleFx_SpawnScaledArcObjects(obj);
	}
}

//--------------------------------------------------------------------------
// 演出スロット 24 個を動かす
//--------------------------------------------------------------------------
void BattleFx_UpdateAllEffectSlots(void)
{
	UpdateAllEffectSlots();
}

//--------------------------------------------------------------------------
// エナジー演出のワークを用意し、わざの演出を準備する
// 演出中にもう一度呼ばれたら、わざだけを入れ替える
//--------------------------------------------------------------------------
void BattleFx_LoadActionEffectResources(s32 act_no, s32 mode)
{
	struct FieldWork	 *ev = *(gWorkSlot + HEAP_SLOT_EVENT);
	struct MapScrollWork *map = *(gWorkSlot + 8);
	struct BattleFxScene *work;
	s32					  running;

	running = ev->resolving_action;
	if (running == 0)
	{
		volatile u32 fill;

		work = Runtime_AllocateHeapBlock(HEAP_SLOT_FX, sizeof *work);
		fill = running;
		Dma_Set((const void *)&fill, work,
			DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | (sizeof *work / 4), REG_DMA3);
	}
	else
	{
		work = *(gWorkSlot + HEAP_SLOT_FX);
	}
	work->action = act_no;
	work->animation = BtlActGet(act_no)->type_0c;
	running = ev->resolving_action;
	if (running != 0)
		return;
	work->free_blocks = 512 - ResourceTable_CountFreeBlocks();
	work->mode = mode;
	work->visible = 1;
	work->enabled = 1;
	work->active = 1;
	work->message_mode = 1;
	// マップのゆれの大きさと減り方を取っておき、演出の後で戻す
	work->saved_x = map->shake_x;
	work->saved_y = map->shake_y;
	work->saved_z = map->shake_decay;
	if (gGameState.map == (s32)&SceneId_MakyuriIriguchi)
		work->lit = 1;
	if (gGameState.map == (s32)&SceneId_MakyuriHeya2)
		work->lit = 1;
	BattleFx_SetupObjectPair(gGameState.selected_actor, -1);
	if (work->animation != 8)
		ev->unknown_cc0 = running;
	work->tile_slot = Resource_FindFreeEntry();
	VramBlock_LoadCached((s16)work->tile_slot, 256, BattleFx_SparkTiles);
	Scheduler_AddOrUpdateCallback(BattleFx_UpdateAllEffectSlots, 0xc80);
}

//--------------------------------------------------------------------------
// 使う者と対象の物体を記録する
// 対象がなければ、使う者の向きに 16 ドット先を演出の位置にする
//--------------------------------------------------------------------------
void BattleFx_SetupObjectPair(s32 user_id, s32 tgt_id)
{
	struct ScriptMotionObject *user;
	struct ScriptMotionObject *tgt;
	s32						   dir;
	struct BattleFxScene	  *work;

	work = gEffectWork;
	work->first_object_id = user_id;
	user = ObjectTable_Get((s16)user_id);
	work->second_object_id = tgt_id;
	work->main_object = user;
	tgt = ObjectTable_Get((s16)tgt_id);
	dir = (user->facing + 0x2000) & 0xc000; // 四方向にまるめる
	work->child = tgt;
	work->angle = dir;
	if (tgt != NULL)
	{
		// 対象の動きと色を、あとで戻すために取っておく
		work->saved_callback = (void *)tgt->hook;
		work->saved_script = (const s32 *)tgt->script;
		{
			struct AnimationObject *anim = ((struct ObjectRuntime *)tgt)->animation;
			u8						pal = anim->entries[0]->param;

			work->saved_palette = pal;
		}
		work->x = tgt->x;
		work->z = tgt->z;
		work->y = tgt->y;
		return;
	}
	work->x = user->x;
	work->z = user->z;
	work->y = user->y;
	Vector_AddPolarOffset(0x100000, dir, &work->x);
}

//--------------------------------------------------------------------------
// 使う者の動きを止めて、色を戻す
//--------------------------------------------------------------------------
void EffectRuntime_StopCurrentObject(void)
{
	struct BattleFxScene	  *work = gEffectWork;
	struct ScriptMotionObject *user = work->main_object;

	user->hook = NULL;
	Animation_ApplyChildValuesFar(user, 0);
	WaitFrames(1);
}

struct BattleEffectPosition
{
	u8	reserved_00[4];
	s32 x;
	s32 y;
	s32 z;
};

#define BATTLE_OBJECT_SLOTS ((struct ScriptMotionObject *)gObjectSlots)

void BattleEffect_CleanupSceneObjects(void)
{
	struct BattleFxScene	   **scene_cell;
	struct BattleFxScene		*scene;
	struct FieldWork			*runtime;
	struct BattleEffectPosition *position;
	struct EffectSlot			*scene_object;
	s32							 remaining;

	scene_cell = (struct BattleFxScene **)&gEffectWork;
	scene = *scene_cell;
	runtime = *(struct FieldWork **)((u8 *)scene_cell - 116);
	position = *(struct BattleEffectPosition **)((u8 *)scene_cell - 192);
	for (remaining = 0; remaining < 24; remaining++)
	{
		scene_object = &scene->slots[remaining];
		if (scene_object->active != 0)
			BattleFx_ClearOwnedSlot(scene_object);
	}

	if (runtime->resolving_action == 0)
	{
		s32 waited = 0;
		void (*first_active_update)(void) = (void (*)(void))BattleFx_UpdateScaledArcObjectB;
		void (*second_active_update)(void) = (void (*)(void))BattleFx_UpdateScaledArcObjectA;
		s32 active;

		do
		{
			struct ScriptMotionObject *slot = BATTLE_OBJECT_SLOTS;

			active = 0;
			remaining = 0;
			while (remaining <= 63)
			{
				void (*update)(void) = (void (*)(void))slot->hook;

				if (update == first_active_update || update == second_active_update)
				{
					active = 1;
					break;
				}
				remaining++;
				slot++;
			}

			if (active != 0)
			{
				waited++;
				WaitFrames(1);
			}
		} while (active != 0 && waited <= 29);

		runtime->unknown_cc7 = 0;
		Scheduler_RemoveCallback(BattleFx_UpdateAllEffectSlots);
		Resource_ResetEntry((s16)scene->tile_slot);
		position->x = scene->saved_x;
		position->y = scene->saved_y;
		position->z = scene->saved_z;
		if (scene->animation != 8)
			runtime->unknown_cc0 = 1;
		BattleFx_PlayQueuedSound();
		Runtime_ReleaseHeapBlock(0x38);
	}
}

void EventObject_Initialize(void)
{
	s32					  event_value;
	s32					  event_index;
	struct FieldActor	 *event_object;
	struct BattleFxScene *event_state;
	void				 *render_state;

	event_state = *(void **)Ram_EffectWork;
	render_state = *(void **)Ram_EventWork;
	event_object = event_state->main_object;
	event_value = (s16)event_state->action;
	event_index = event_value;
	ObjectDispatch_ApplyArgumentToChildrenFar(event_object, 0x14);
	event_object->target_x = event_object->x.fixed;
	event_object->target_y = event_object->y.fixed;
	event_object->target_z = event_object->z.fixed;
	event_object->velocity_x = 0;
	event_object->velocity_y = 0;
	event_object->velocity_z = 0;
	if (event_state->visible != 0)
	{
		AudioCommand_PlayFar(212);
		event_object->update = (void *)BattleFx_FlickerObjectAndTick;
	}
	if (event_state->active != 0)
	{
		ObjectGroup_SetActionForOthers(event_object, 1, 0);
		UiWork_PushValueSlotFar(event_index, 4);
		if ((s8)event_state->mode != 0)
		{
			UiText_ShowPositionedMessageAndWaitFar(MsgAbilityAnnounce, event_state->message_mode);
		}
		else
		{
			UiText_ShowPositionedMessageAndWaitFar(MsgAbilityAnnounce, event_state->message_mode);
		}
		ObjectGroup_SetActionForOthers(event_object, 0, 0x10);
	}
	if (GameFlag_TestFar(0x140) != 0)
	{
		if (event_state->visible != 0)
		{
			event_object->update = (void *)BattleFx_CycleObjectValueByCounter;
		}
		ObjectDispatch_ApplyArgumentToChildrenFar(event_object, 0x15);
	}
	else
	{
		EffectRuntime_StopCurrentObject();
	}
	((struct FieldWork *)render_state)->unknown_cc7 = 1;
}

void BattleEffect_InitializeSharedScene(void)
{
	u8						   *scene;
	struct BattleEffectBuffers *work;
	s32							no;

	scene = *(gWorkSlot + HEAP_SLOT_EVENT);
	work = *(gWorkSlot + 32);
	Dma_Set(&work->target[0x2a0], scene + 0x776, 0x84000150, REG_DMA3);
	if (((struct FieldWork *)scene)->effect_active == 0)
		Dma_Set(work->target, scene + 0x236, 0x84000150, REG_DMA3);
	Dma_Set(work->target, work->current, 0x84000000 | (sizeof work->current / 4), REG_DMA3);

	no = gFrameCount & 7;
	if (GameFlag_TestFar(0x148))
		no = 0;
	if (GameFlag_TestFar(0x149))
		no = 1;
	if (GameFlag_TestFar(0x14a))
		no = 2;
	if (GameFlag_TestFar(0x14b))
		no = 3;
	if (GameFlag_TestFar(0x14c))
		no = 4;
	if (GameFlag_TestFar(0x14d))
		no = 5;
	if (GameFlag_TestFar(0x14e))
		no = 6;
	if (GameFlag_TestFar(0x14f))
		no = 7;
	BattleFx_ApplyColorToTargetBuffer(BattleFx_TargetColors[no], 1);
	BattleFx_StartBufferInterpolation(8);
}

void BattleFx_PrepareBufferInterpolation(void)
{
	s32 value;
	u8 *state;

	state = (u8 *)gEventWork;
	value = (s32)(state + 0x236);
	BattleFx_ApplyColorToTargetBuffer(value, 2);
	if (((struct FieldWork *)state)->effect_active != 0)
	{
		BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
	}
	else
	{
		BattleFx_ApplyColorToTargetBuffer(value, 1);
	}
	BattleFx_StartBufferInterpolation(8);
}

void Camera_WorldToScreen(struct EffectVector *position)
{
	struct EventRuntime **data = (struct EventRuntime **)&gEventWork;

	if ((*data)->mode_19e == 3)
	{
		struct EffectVector result;

		Render_ProjectPoint(position, &result);
		position->x = result.x << 16;
		position->z = result.y << 16;
		position->y = 0;
	}
	else
	{
		struct MapScrollWork *state = *(struct MapScrollWork **)((u8 *)data - 76);
		s32					  x = state->view_x & 0xffff0000;
		s32					  z = state->view_y & 0xffff0000;

		position->x -= x;
		position->z -= position->y;
		position->z -= z;
		position->y = 0;
	}
}

// 以下では座標を関数ごとの構造体で渡す
