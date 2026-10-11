//--------------------------------------------------------------------------
// dispatch_type_handler.c
// フィールド：演出処理の呼び分け
//--------------------------------------------------------------------------

#include "types.h"
#include "battle.h"
#include "field.h"
#include "debug.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "battle_effect_runtime.h"
#include "heap_state.h"
#include "animspr.h"
#include "object_runtime.h"
#include "game_state.h"
#include "fx_scene.h"
#include "scene.h"
#include "global_cells.h"
#include "btlsys.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "field_sprite.h"
#include "fieldobj.h"
#include "objdisp.h"
#include "object_efx.h"
#include "fixed_math.h"
#include "field_event.h"
#include "vram_block.h"
#include "mem.h"
#include "script_motion.h"
#include "callback_scheduler.h"
#include "effect_slot.h"
#include "dma.h"
#include "scene_ids.h"
#include "object_lookup.h"
#include "io_reg.h"
#include "map_scroll.h"
#include "event_runtime.h"
#include "ram_buffer.h"
#include "frame.h"
#include "metadata_lookup.h"
#include "menu_list.h"
#include "window.h"
#include "iwram_call.h"
#include "tbs_edition.h"
#include "keystate.h"
#include "edition.h"
#include "motobj.h"
#include "sound_ids.h"
#include "disptran.h"
#include "object_dispatch.h"
#include "map.h"
#include "object_effect.h"
#include "field_effect.h"
#include "fixed_point_position.h"
#include "ui.h"
#include "io_write_queue.h"
#include "party_state.h"
#include "resource_ids.h"

void BattleFx_DispatchRequestKind(void);
void BattleFx_Run(void);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);

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

void BattleFx_CycleObjectValueByCounter(s32 obj);
void BattleFx_FlickerObjectAndTick(s32 obj);

#define gFrameCount (*(volatile s32 *)&gFrameCount)

#define BattleFx_MarkChildAndRunFallbackTransition \
	((void (*)(s32))BattleFx_MarkChildAndRunFallbackTransition)
#define BattleEffect_RunTargetedItemBreak ((void (*)(s32))BattleEffect_RunTargetedItemBreak)
#define BattleFx_CallEffect04 ((void (*)(s32))BattleFx_CallEffect04)
#define BattleFx_CallEffect05 ((void (*)(s32))BattleFx_CallEffect05)
#define BattleFx_StartOrbitingParticles ((void (*)(s32))BattleFx_StartOrbitingParticles)
#define BattleFx_RunBurstParticleMainObject ((void (*)(s32))BattleFx_RunBurstParticleMainObject)
#define BattleFx_CallEffect03AndStop ((void (*)(s32))BattleFx_CallEffect03AndStop)
#define BattleFx_RunEffect13Hook ((void (*)(s32))BattleFx_RunEffect13Hook)

#define FX_SLOT 56 // gWorkSlot[56] は gEffectWork

void BattleEffect_RunFallbackObjectTransition(void);

extern struct BattleFxScene *gEffectWork;
void						 RunSceneTransitionEffect(s32 source_id, s32 target_id);
void						 RunBattleEffect04(void);
void						 RunBattleEffect14(void);
void						 BattleEffect_PauseObject(s32 obj_id);

void Event_SetMessageId(s32);
void BattleFx_FinishAction();
void _call_via_r3(s32, s32);
void UiText_ShowPositionedMessageAndWaitFar(s32, s32);
void Object_SetMoveTargetFar(struct ObjectRuntime *, s32, s32, s32);

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

	scene = *(gWorkSlot + FX_SLOT);
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
	struct BattleFxScene *scene = *(gWorkSlot + FX_SLOT);
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

	kind = gEffectWork->animation;
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
	void Battle_Reset();
	s32 script;

	if (event != NULL)
	{
		script = FIELD_AT_OFFSET(event, s32 *, 8);
		if (script != 0)
		{
			if (script < 0x10000)
			{
				Battle_Reset();
				Event_SetMessageId(FIELD_AT_OFFSET(event, s32 *, 8));
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
				frame_count = &gFrameCount;
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
	void Vector_AddPolarOffset(s32, s32, void *);
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

s32 ResourceTable_CountFreeBlocks(void)
{
    u8 *marker = ResourceBlockOwners;
    s32 free_count = 0;
    s32 remaining = 0x200;

    do {
        if (*marker++ == 0xff) {
            free_count++;
        }
        remaining--;
    } while (remaining != 0);
    return free_count;
}

void *Object_ReplaceResourceEntry(void *src, void *alt)
{
    void *ret;
    struct FieldSprite *obj;

    obj = src;
    ret = NULL;
    if (obj != NULL) {
        if (alt == NULL) {
            obj->unknown_1d_0 = 1;
        } else {
            Resource_ResetEntry(obj->vram_block);
            obj->vram_block = (u8)((struct FieldSprite *)alt)->vram_block;
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
void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

#define ObjectDispatch_ApplyValueToChildrenFar \
	((void (*)(struct ObjectRuntime *, s32))ObjectDispatch_ApplyValueToChildrenFar)

#define OBJ_LAST 66 // 調べる物体の番号の上限
void						 ObjectDispatch_ReleaseFar(struct ObjectRuntime *);
void						 ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *, s32);

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
	v = (s16)++obj->timer;
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
	v = (s16)++obj->timer;
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

#undef gFrameCount

#define HEAP_SLOT_FX 56 // エナジー演出のワーク

// スプライトの子
struct ArcChild
{
	u8 pad00[22];
	u8 frame;
};

// スプライト：OAM の三つの属性と管理情報
struct ArcSprite
{
	u8				 pad00[4];
	u16				 y : 8;
	u16				 affine : 1;
	u16				 double_size : 1;
	u16				 mode : 2;
	u16				 mosaic : 1;
	u16				 color : 1; // 1 = 256 色
	u16				 shape : 2;
	u16				 x : 9;
	u16				 unused : 3;
	u16				 hflip : 1;
	u16				 vflip : 1;
	u16				 size : 2;
	u16				 tile : 10;
	u16				 priority : 2;
	u16				 palette : 4;
	u8				 pad0a[18];
	u8				 slot; // VRAM ブロック
	u8				 active : 1;
	u8				 display_flags : 7;
	u8				 pad1e[8];
	u8				 flags;
	u8				 pad27;
	struct ArcChild *child;
};

// 弧の物体
struct ArcObject
{
	u8				  pad00[8];
	s32				  x; // 16.16 固定小数点のドット
	s32				  y;
	s32				  z;
	s32				  terrain_height;
	s32				  scale_x; // 0x10000 = 等倍
	s32				  scale_y;
	u8				  pad20[0x23 - 0x20];
	u8				  priority_flags;
	u8				  pad24[0x50 - 0x24];
	struct ArcSprite *sprite;
	u8				  pad54[1];
	u8				  motion_flags;
	u8				  pad56[0x64 - 0x56];
	u16				  counter; // 出てからのフレーム
	u8				  pad66[2];
	struct ArcObject *linked_object; // ついていく物体
	void (*update)(struct FieldActor *);
};

extern volatile u32 gFrameCount;
void				Animation_ApplyChildValuesFar(struct DispatchObject *object, u32 value);

// イベントワークのうち、ここで使う部分
struct BattleSceneState
{
	u8	padding000[0xcc0];
	u16 unknown_cc0;
	u8	padding0cc2[4];
	s8	running; // エナジーの演出中
};

void BattleFx_SpawnScaledArcObjects(struct FieldActor *link);

//--------------------------------------------------------------------------
// 物体の位置に二つの弧を出す (B は上下を反転)
// 弧は 32 フレームでふくらんでしぼみ、物体についていく
//--------------------------------------------------------------------------
void BattleFx_SpawnScaledArcObjects(struct FieldActor *linked)
{
	/* FAKEMATCH: needed to match: scalar coordinates and packed OAM preserve alias order and registers */
	struct ArcObject	 *link = (struct ArcObject *)linked;
	struct BattleFxScene *scene = gEffectWork;
	struct ArcObject	 *source = (struct ArcObject *)scene->main_object;
	struct ArcObject	 *objs[2];
	struct ArcObject	 *obj;
	struct ArcSprite	 *spr;
	s32					  i;

	for (i = 0; i <= 1; i++)
	{
		obj = (struct ArcObject *)Object_CreateFar(26, link->x, link->y, link->z);
		objs[i] = obj;
		if (obj == 0)
			continue;
		obj->terrain_height = link->terrain_height;
		spr = obj->sprite;
		obj->motion_flags = 0;
		obj->counter = 0;
		obj->linked_object = link;
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
	s32 p;
	s32 n;

	p = (s32)gEffectWork->slots;
	n = 23;
	do
	{
		n -= 1;
		EffectSlot_Update((struct EffectSlot *)p);
		p += sizeof(struct EffectSlot);
	} while (n >= 0);
}

//--------------------------------------------------------------------------
// エナジー演出のワークを用意し、わざの演出を準備する
// 演出中にもう一度呼ばれたら、わざだけを入れ替える
//--------------------------------------------------------------------------
void BattleFx_LoadActionEffectResources(s32 act_no, s32 mode)
{
	struct BattleSceneState *ev = *(gWorkSlot + HEAP_SLOT_EVENT);
	struct MapScrollWork	*map = *(gWorkSlot + 8);
	struct BattleFxScene	*work;
	s32						 running;

	running = ev->running;
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
	running = ev->running;
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
	void Vector_AddPolarOffset(s32, s32, void *);
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
	Animation_ApplyChildValuesFar((struct DispatchObject *)user, 0);
	WaitFrames(1);
}

void AudioCommand_PlayFar(s32 cue);

#define BattleFx_UpdateScaledArcObjectB ((void (*)(void))BattleFx_UpdateScaledArcObjectB)
#define BattleFx_UpdateScaledArcObjectA ((void (*)(void))BattleFx_UpdateScaledArcObjectA)
#define ObjectGroup_SetActionForOthers ((void (*)(void *, s32, s32))ObjectGroup_SetActionForOthers)

#define gObjectSlots ((u8(*)) & gObjectSlots)

struct BattleEffectRuntime
{
	u8	reserved_000[0xcc0];
	s16 restore_requested;
	u8	reserved_cc2[4];
	s8	teardown_blocked;
	s8	teardown_state;
};

struct BattleEffectPosition
{
	u8	reserved_00[4];
	s32 x;
	s32 y;
	s32 z;
};

#define BATTLE_OBJECT_SLOTS (*(struct ScriptMotionObject **)gObjectSlots)
void BattleFx_ClearOwnedSlot(struct EffectSlot *object);
void BattleFx_PlayQueuedSound(void);
#define CALLBACK_1 (u32) BattleFx_FlickerObjectAndTick
#define CALLBACK_2 (u32) BattleFx_CycleObjectValueByCounter

s32 __umodsi3(s32, s32);

void BattleEffect_CleanupSceneObjects(void)
{
	struct BattleFxScene	   **scene_cell;
	struct BattleFxScene		*scene;
	struct BattleEffectRuntime	*runtime;
	struct BattleEffectPosition *position;
	struct EffectSlot			*scene_object;
	s32							 remaining;

	scene_cell = &gEffectWork;
	scene = *scene_cell;
	runtime = *(struct BattleEffectRuntime **)((u8 *)scene_cell - 116);
	position = *(struct BattleEffectPosition **)((u8 *)scene_cell - 192);
	for (remaining = 0; remaining < 24; remaining++)
	{
		scene_object = &scene->slots[remaining];
		if (scene_object->active != 0)
			BattleFx_ClearOwnedSlot(scene_object);
	}

	if (runtime->teardown_blocked == 0)
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

		runtime->teardown_state = 0;
		Scheduler_RemoveCallback(BattleFx_UpdateAllEffectSlots);
		Resource_ResetEntry((s16)scene->tile_slot);
		position->x = scene->saved_x;
		position->y = scene->saved_y;
		position->z = scene->saved_z;
		if (scene->animation != 8)
			runtime->restore_requested = 1;
		BattleFx_PlayQueuedSound();
		Runtime_ReleaseHeapBlock(0x38);
	}
}

void EventObject_Initialize(void)
{
	s32					  zero;
	s32					  event_value;
	s32					  event_index;
	void				 *event_object;
	struct BattleFxScene *event_state;
	void				 *render_state;

	event_state = *(void **)Ram_EffectWork;
	render_state = *(void **)Ram_EventWork;
	zero = 0;
	event_object = event_state->main_object;
	event_value = (s16)event_state->action;
	event_index = event_value;
	ObjectDispatch_ApplyArgumentToChildrenFar(event_object, 0x14);
	FIELD_AT_OFFSET(event_object, u32 *, 0x38) = *(s32 *)((u8 *)(event_object) + 8);
	FIELD_AT_OFFSET(event_object, s32 *, 0x3C) = FIELD_AT_OFFSET(event_object, s32 *, 0xC);
	FIELD_AT_OFFSET(event_object, s32 *, 0x40) = FIELD_AT_OFFSET(event_object, s32 *, 0x10);
	FIELD_AT_OFFSET(event_object, u32 *, 0x24) = 0;
	FIELD_AT_OFFSET(event_object, u32 *, 0x28) = 0;
	*(s32 *)((u8 *)(event_object) + 0x2C) = 0;
	if (event_state->visible != 0)
	{
		AudioCommand_PlayFar(212);
		FIELD_AT_OFFSET(event_object, s32 *, 0x6C) = CALLBACK_1;
	}
	if (event_state->active != 0)
	{
		ObjectGroup_SetActionForOthers(event_object, 1, 0);
		UiWork_PushValueSlotFar(event_index, 4);
		if ((s8)event_state->mode != 0)
		{
			UiText_ShowPositionedMessageAndWaitFar(
				(void *)MsgAbilityAnnounce, event_state->message_mode);
		}
		else
		{
			UiText_ShowPositionedMessageAndWaitFar(
				(void *)MsgAbilityAnnounce, event_state->message_mode);
		}
		ObjectGroup_SetActionForOthers(event_object, 0, 0x10);
	}
	if (GameFlag_TestFar(0x140) != 0)
	{
		if (event_state->visible != 0)
		{
			FIELD_AT_OFFSET(event_object, s32 *, 0x6C) = CALLBACK_2;
		}
		ObjectDispatch_ApplyArgumentToChildrenFar(event_object, 0x15);
	}
	else
	{
		EffectRuntime_StopCurrentObject();
	}
	FIELD_AT_OFFSET(render_state, s8 *, 0xCC7) = 1;
}

void BattleEffect_InitializeSharedScene(void)
{
	u8						   *scene;
	struct BattleEffectBuffers *work;
	s32							no;

	scene = *(gWorkSlot + HEAP_SLOT_EVENT);
	work = *(gWorkSlot + 32);
	Dma_Set(&work->target[0x2a0], scene + 0x776, 0x84000150, REG_DMA3);
	if (*(s16 *)(scene + 0xcb8) == 0)
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
	if (FIELD_AT_OFFSET(state, s16 *, 0xCB8) != 0)
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

void RunSceneTransitionEffect(s32 x, s32 y)
{
	u8			*work;
	volatile u32 zero;

	work = Runtime_AllocateBlock(22, 0x298);
	BattleEffect_InitializeSharedScene();
	zero = 0;
	Dma_Set((const void *)&zero, work, 0x850000a6, REG_DMA3);
	*(u16 *)(work + 0x28e) = __umodsi3(gFrameCount << 1, 360);
	BattleFx_AdvanceHueCycle();
	BattleFx_ApplyColorToTargetBuffer(
		((s8)work[0x28d] << 10) | ((s8)work[0x28c] << 5) | (s8)work[0x28b] | 0x200000, 1);
	BattleFx_StartBufferInterpolation(8);
	*(u16 *)(work + 0x290) = x;
	*(u16 *)(work + 0x292) = y;
	work[0x294] = 8;
	Ui_FillBank15PaletteGrey();
	Scheduler_AddOrUpdateCallback((void (*)(void))BattleFx_UpdateWaveScene, 0xc80);
}

//--------------------------------------------------------------------------
// finish_scene_and_release_heap_block.c
// フィールド効果：ラインごとの波と色の変化、対象から使い手へ流れる粒
//--------------------------------------------------------------------------

#define Animation_ApplyChildPalette ((void (*)(struct FxObject *, s32))Animation_ApplyChildPalette)
#define Object_SetMoveTargetFar \
	((void (*)(struct FxObject *, s32, s32, s32))Object_SetMoveTargetFar)
#define BattleFx_SetCallbackWhenTargetUnset ((void (*)(void))BattleFx_SetCallbackWhenTargetUnset)

#undef FIELD_AT_OFFSET
#undef FIELD_AT_OFFSET
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct FxVector
{
	s32 x;
	s32 y;
	s32 z;
};

struct FxSprite
{
	u32 unknown_00[2];
	u8	unknown_08;
	u32 unknown_09_0 : 2;
	u32 mode : 2;
	u32 unknown_09_4 : 4;
	u8	unknown_0a[0x1c];
	u8	palette;
};

struct FxObject
{
	u16				 unknown_00[3];
	u16				 angle;
	s32				 x;
	s32				 y;
	s32				 z;
	u32				 unknown_14[7];
	s32				 scale_x; // 0x10000 で等倍
	s32				 scale_y;
	u32				 unknown_38[6];
	struct FxSprite *sprite;
	u32				 unknown_54 : 8;
	u32				 visible : 8;
	u32				 unknown_56 : 16;
	u32				 unknown_58[5];
	void			*callback;
};

// RunSceneTransitionEffect が 0x298 バイト確保して初期化する
struct WaveFxWork
{
	u16 lines[2][162]; // ラインごとのずれ (2 面を交互に書く)
	u16 phase;		   // 波の位相
	u8	page;		   // 書き込む面
	s8	blue;		   // 色相から求めた色
	s8	green;
	s8	red;
	u16 hue;	   // 色相 (0〜359 度)
	u16 source_id; // 使い手
	u16 target_id; // 対象
	u8	delay;	   // 開始までの待ち
	u8	counter;   // 粒を出す周期 (0〜60)
};

void			 Ui_SetBank15PaletteAndClearRenderMode(void);
s16				*BattleAction_FindDescriptor(s32 id);
s32				 ArcTan2(s32 y, s32 x);

//--------------------------------------------------------------------------
// 効果を止めて使い手の色を戻し、作業領域を返す
//--------------------------------------------------------------------------
void BattleFx_FinishSceneAndReleaseHeapBlock(void)
{
	void *work;

	work = *(void **)((u32)&gBattleBgFxWork);
	Ui_SetBank15PaletteAndClearRenderMode();
	Scheduler_RemoveCallback(BattleFx_UpdateWaveScene);
	Animation_ApplyChildPalette(Object_GetById(FIELD_AT_OFFSET(work, u16, 0x290)), 1); // source_id
	BattleFx_PrepareBufferInterpolation();
	Runtime_ReleaseHeapBlock(22);
}

//--------------------------------------------------------------------------
// 毎フレームの処理 (スケジューラから呼ばれる)
//--------------------------------------------------------------------------
void BattleFx_UpdateWaveScene(void)
{
	struct WaveFxWork		 *work = gBattleBgFxWork;
	u16						 *line;
	u32						  i;
	struct FxObject			 *src;
	struct FxObject			 *dst;
	struct FxObject			 *obj;
	struct FxSprite			 *sprite;
	struct FxVector			  from;
	struct FxVector			  to;
	struct AnimationMetadata *meta;

	if (work->delay != 0)
	{
		work->delay--;
		return;
	}

	// 160 ライン分のずれを ±4 ドットの正弦波で作り、面を切り替える
	line = work->lines[work->page ^ 1];
	for (i = 0; i < 160; i++)
		*line++ = Trig_Sin(__udivsi3((work->phase + i * 8) << 16, 160)) >> 14;
	work->phase += 4;
	work->page ^= 1;
	if (work->page != 0)
	{
		BattleFx_ApplyColorToTargetBuffer(
			(work->red << 10) | (work->green << 5) | work->blue | 0x200000, 1);
		BattleFx_StartBufferInterpolation(1);
		BattleFx_AdvanceHueCycle();
	}
	Animation_ApplyChildPalette(Object_GetById(work->source_id), 0);

	// 周期の 0, 8, 16 で、対象から使い手へ向かう粒を出す
	if (work->counter == 0 || work->counter == 8 || work->counter == 16)
	{
		src = Object_GetById(work->source_id);
		dst = Object_GetById(work->target_id);
		if (src != 0 && dst != 0)
		{
			// メタデータの box_x を高さに使い、そこから 2 ドット下
			from.x = src->x;
			meta = Resource_GetMetadataRecordFar(*BattleAction_FindDescriptor(work->source_id));
			from.y = src->y + ((s8)meta->box_x << 16) - (2 << 16);
			from.z = src->z;
			to.x = dst->x;
			meta = Resource_GetMetadataRecordFar(*BattleAction_FindDescriptor(work->target_id));
			to.y = dst->y + ((s8)meta->box_x << 16) - (2 << 16);
			to.z = dst->z;
			obj = Object_CreateFar(281, to.x, to.y, to.z);
			if (obj != 0)
			{
				sprite = obj->sprite;
				obj->visible = 0;
				obj->scale_x = 0xa3d7; // 0.64 倍
				obj->scale_y = 0xa3d7;
				obj->angle = ArcTan2(from.z - to.z, from.x - to.x);
				obj->callback = BattleFx_SetCallbackWhenTargetUnset;
				sprite->palette = 0;
				sprite->mode = 1;
				Object_SetMoveTargetFar(obj, from.x, from.y, from.z);
			}
		}
	}

	if (work->counter == 0)
		AudioCommand_PlayFar(130);
	if (++work->counter > 60)
		work->counter = 0;
}

//--------------------------------------------------------------------------
// hue.c
// 背景の走査線スクロール DMA、色相の巡回、BG パレット 15 の切り替え
//--------------------------------------------------------------------------

#undef BattleFx_SetCallbackWhenTargetUnset

#define PaletteGlow_UpdateFar ((s32 (*)(s32, s32))PaletteGlow_UpdateFar)

struct WaveFxWork_2
{
	u16 lines[2][162]; // 走査線ごとの BG0 横スクロール (2 面)
	u16 phase;
	u8	page;	// HBlank DMA で送る面
	u8	rgb[3]; // 色相から求めた色 (赤・緑・青、各 4〜11)
	u16 hue;	// 色相 (度)
	u16 source_id;
	u16 target_id;
	u8	delay; // 0 以外のあいだは DMA を張らない
	u8	counter;
};

#define GREY28 0x739c // 灰色 (赤・緑・青とも 28)

//--------------------------------------------------------------------------
// BG0 の横スクロールを走査線ごとに書き換える HBlank DMA を張りなおす
//--------------------------------------------------------------------------
void BattleFx_ArmBg0HBlankDma(void)
{
	struct WaveFxWork_2 *work = gBattleBgFxWork;
	if (!work->delay)
	{
		u16			 *line = work->lines[work->page];
		volatile u16 *dma = REG_DMA0;
		dma[5] &= 0xc5ff; // 開始タイミングとリピートを消す
		dma[5] &= 0x7fff; // 止める
		(void)dma[5];
		// HBlank ごとに 16 ビットを 1 回 (リピート、転送先リロード)
		Dma_Set(line, REG_BG0HOFS, 0xa2600001, (volatile u32 *)dma);
	}
}

//--------------------------------------------------------------------------
// 色相を 4 度すすめ、その色を求める
//--------------------------------------------------------------------------
void BattleFx_AdvanceHueCycle(void)
{
	struct WaveFxWork_2 *work = gBattleBgFxWork;
	s32				   r = 0;
	s32				   g = 0;
	s32				   b = 0;
	s32				   ofs;
	u8				  *pr;
	u8				  *pg;
	u8				  *pb;

	BattleFx_ComputeHueChannels(work->hue << 16, &r, &g, &b);

	// 0〜31 を 4〜11 に縮める (ofs は rgb の位置から 1 ずつ進む)
	ofs = 0x28b;
	pr = (u8 *)work + ofs;
	*pr = (r >> 18) + 4;
	ofs += 1;
	pg = (u8 *)work + ofs;
	*pg = (g >> 18) + 4;
	ofs += 1;
	pb = (u8 *)work + ofs;
	*pb = (b >> 18) + 4;

	work->hue += 4;

	*pr &= 0x1f;
	*pg &= 0x1f;
	*pb &= 0x1f;

	if (work->hue >= 360)
	{
		work->hue = 0;
	}
}

//--------------------------------------------------------------------------
// 色相から赤・緑・青を求める (各 0〜31、16.16 固定小数)
// hue : 色相 (度、16.16 固定小数)
//--------------------------------------------------------------------------
void BattleFx_ComputeHueChannels(s32 hue, s32 *r, s32 *g, s32 *b)
{
	*r = BattleFx_HueChannelRamp(hue + (120 << 16), 0, 31 << 16);
	*g = BattleFx_HueChannelRamp(hue, 0, 31 << 16);
	*b = BattleFx_HueChannelRamp(hue - (120 << 16), 0, 31 << 16);
}

//--------------------------------------------------------------------------
// 色相に対する色の強さ
// 0〜60 度で上がり、60〜180 度は high、180〜240 度で下がり、ほかは low
//--------------------------------------------------------------------------
s32 BattleFx_HueChannelRamp(s32 angle, s32 low, s32 high)
{
	s32 product;

	angle = Fixed_Remainder(angle, 360 << 16);
	if (angle < 60 << 16)
	{
		product = Iwram_MulQ16(high, angle);
	}
	else
	{
		if (angle >= 60 << 16 && angle < 180 << 16)
			return high;
		/* FAKEMATCH: needed to match: the goto places the low return after the divide */
		if (!(angle >= 180 << 16 && angle < 240 << 16))
			goto out;
		product = Iwram_MulQ16(high, (240 << 16) - angle);
	}
	{
		return Iwram_RatioMulQ14(60 << 16, product);
	}
out:
	return low;
}

//--------------------------------------------------------------------------
// 16.16 固定小数の剰余 (divisor が 0 なら 0)
//--------------------------------------------------------------------------
s32 Fixed_Remainder(s32 value, s32 divisor)
{
	s32 quot;

	if (divisor == 0)
		return 0;
	if (divisor & 0xf0000000)
		divisor = -divisor;
	quot = Iwram_RatioMulQ14(divisor, value);
	return value - Iwram_MulQ16(quot & 0xffff0000, divisor); // 商の整数部 × divisor を引く
}

//--------------------------------------------------------------------------
// 移動の目標がなければ、目標なしのスクリプトを始める
//--------------------------------------------------------------------------
void BattleFx_SetCallbackWhenTargetUnset(struct ObjectRuntime *obj)
{
	s32 ty;
	s32 tx;

	tx = obj->target_x;
	if (tx == 0x80000000) // 目標なし
	{
		ty = obj->target_y;
		if ((ty == tx) && (obj->target_z == ty))
		{
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)obj, BattleFx_UntargetedObjectScript);
		}
	}
}

//--------------------------------------------------------------------------
// BG パレット 15 の色 1, 3〜12 を灰色にし、走査線スクロールを始める
//--------------------------------------------------------------------------
void Ui_FillBank15PaletteGrey(void)
{
	volatile u16 *p;

	((struct UiRenderWork *)gWindowWork[0])->mode = 1;
	p = &BG_PLTT_COLOR(15, 1);
	*p = GREY28;
	p += 2;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	Scheduler_AddOrUpdateCallback(BattleFx_ArmBg0HBlankDma, 0x480);
}

//--------------------------------------------------------------------------
// 走査線スクロールを止め、BG パレット 15 を戻す
//--------------------------------------------------------------------------
void Ui_SetBank15PaletteAndClearRenderMode(void)
{
	void *work;

	work = gWindowWork[0];
	Scheduler_RemoveCallback(BattleFx_ArmBg0HBlankDma);
	BG_PLTT_COLOR(15, 1) = 0x7fff;	// 白
	BG_PLTT_COLOR(15, 3) = 0;		// 黒
	BG_PLTT_COLOR(15, 11) = 0x294a; // 灰 (10)
	BG_PLTT_COLOR(15, 12) = 0x5294; // 灰 (20)
	PaletteGlow_UpdateFar(gGameState.palette_glow[0], gGameState.palette_glow[1]);
	((struct UiRenderWork *)work)->mode = 0;
}

//--------------------------------------------------------------------------

// pattern.c

// バトル効果：十字キーの入力を向きに変える

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 押している十字キーの向き（0x10000 で 1 周）を返す
// 右 0、下 0x4000、左 0x8000、上 0xc000
// 何も押していないとき、斜めのときは 0xffff
//--------------------------------------------------------------------------
u16 BattleFx_GetCycledTableWord(void)
{
	return BattleFx_CyclePatternWords[(gKeysHeld >> 4) & 15]; // 右・左・上・下の 4 ビット
}

//--------------------------------------------------------------------------

// fragment.c

// 演出：アイテムがこわれる演出の本体の動き

//--------------------------------------------------------------------------

#if !GAME_TLA
void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);
#endif

struct ItemBreakFragmentPosition
{
	s32 x;
	s32 y;
	s32 z;
};

struct ItemBreakFragmentSource
{
	u8								reserved_00[6];
	u16								angle; // 向き（0x10000 で 1 周）
	s32								x;
	s32								y;
	s32								z;
	u8								reserved_14[70];
	u8								flag_5a;
	u8								reserved_5b[13];
	struct ItemBreakFragmentSource *target; // 向く相手
};

struct ItemBreakFragmentObject
{
	u8	reserved_00[72];
	s32 field_48; // 重力（gravity と同じ位置）
	u8	reserved_4c[9];
	u8	mode_55; // motion_flags と同じ位置
	u8	reserved_56[8];
	u16 field_5e;
};

extern s32 ArcTan2(s32, s32);

extern void Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *);

//--------------------------------------------------------------------------
// こわれたアイテムの本体の update
// 向く相手がいれば、そちらへ 1 回に 0x1000（22.5 度）まで曲がる
// そして、まわりにかけらの粒をひとつ出す
// src : 本体のオブジェクト
//--------------------------------------------------------------------------
void BattleFx_UpdateItemBreakFragment(struct ItemBreakFragmentSource *src)
{
	struct ItemBreakFragmentSource	*tgt;
	struct ItemBreakFragmentPosition pos;
	struct ItemBreakFragmentObject	*obj;
	s32								 turn;
	s32								 dist;

	tgt = src->target;
	if (tgt != 0)
	{
		s32 dx = tgt->x - src->x;
		s32 dz = tgt->z - src->z;

		if (dx != 0 || dz != 0)
		{
#if GAME_TLA
			s32 dir = ArcTan2(dz, dx);
			s32 cur = src->angle;
			s32 lim = 128;

			turn = (s16)(dir - cur);
			lim <<= 5;
			if (turn > lim)
				turn = lim;
#else
			turn = (s16)(ArcTan2(dz, dx) - src->angle);
			if (turn > 0x1000)
				turn = 0x1000;
#endif
			if (turn < -0x1000)
				turn = -0x1000;
			src->angle += turn;
		}
		src->flag_5a = 0;
	}

	// 粒は本体から y を 8〜24 ずらし、ランダムな向きに半径 0〜48 散らした位置に出す
	pos.x = src->x;
	pos.y = src->y - (Random16() << 4) - 0x80000;
	pos.z = src->z;
	dist = Random16() * 3;
	dist <<= 4;
	Vector_AddPolarOffset(dist, Random16(), &pos);

#if GAME_TLA
	obj = Object_Spawn(0x2a1, pos.x, pos.y, pos.z);
#else
	obj = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
#endif
	if (obj != 0)
	{
		obj->mode_55 = 2;
		obj->field_48 = 0x1999;
#if GAME_TLA
		Object_SetMode(obj, 0);
#else
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
#endif
		obj->field_5e = 12;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}
}

//--------------------------------------------------------------------------

// fieldfx.c

// フィールド効果：方向キーで対象を押す処理、出現と消滅の粒、範囲の見張り

//--------------------------------------------------------------------------

#undef gFrameCount
#undef gObjectSlots

#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(void *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

#define BattleFx_UpdateItemBreakFragment ((void (*)(void *))BattleFx_UpdateItemBreakFragment)
#define ObjectGroup_ApplyRandomChildValues ((void (*)(void))ObjectGroup_ApplyRandomChildValues)
#define BattleFx_GetCycledTableWord ((s32 (*)(u32))BattleFx_GetCycledTableWord)

#define gObjectSlots ((u8 *)&gObjectSlots)

#define CELL_MASK 0xfff00000 // 16 ドットのマス

struct Output_08097f80
{
	s32 x;
	s32 y;
	s32 z;
};

extern void AudioCommand_PlayFar(s32);
s32 Script_WaitForEventTimeoutFar();

struct FxObject_2
{
	u8				 unknown_00[8];
	s32				 x;
	s32				 y;
	s32				 z;
	s32				 ground;
	u8				 unknown_18[10];
	u8				 mode;
	u8				 unknown_23;
	s32				 velocity_x;
	s32				 velocity_y;
	s32				 velocity_z;
	s32				 speed;
	s32				 lift;
	u8				 unknown_38[12];
	s32				 gravity;
	u8				 unknown_48[13];
	u8				 flag;
	u8				 unknown_56[4];
	u8				 unknown_5a; // bit 0 は押した後に操作キャラで立てる
	u8				 unknown_5b;
	u8				 unknown_5c[12];
	struct FxObject_2 *linked;
	void			*callback;
};

//--------------------------------------------------------------------------
// 対象のそばに押すオブジェクトを出し、方向キーの向きへ対象を 16 ドット押す
// A, B, L, R で取りやめる。押せないときは 16 フレームごとにエラー音
// 押す先が使い手と重なるときは、使い手の位置しだいで押せないか、使い手も動かす
//--------------------------------------------------------------------------
void BattleFx_SteerPushedTarget(s32 target_id)
{
	s32				 Map_CheckObjectStepFar(struct FxObject_2 *object, struct Output_08097f80 *position);
	struct Output_08097f80	position;
	struct Output_08097f80	beyond;
	struct BattleFxScene   *work;
	struct FxObject_2		   *user;
	struct FxObject_2		   *target;
	struct FxObject_2		   *pusher;
	struct FxObject_2		   *hit;
	s32						x;
	s32						z;
	s32						angle;
	s32						moved;
	s32						dir;
	s32						keys;
	s32						cell_x;
	s32						cell_z;
	struct Output_08097f80 *p;

	work = gEffectWork;
	user = work->main_object;
	target = work->child;
	angle = work->angle + 0x8000; // work->angle の反対向き
	moved = 0;
	if (target == NULL)
		return;
	BattleEffect_InitializeSharedScene();
	user->linked = target;
	ObjectDispatch_InitializeFar((struct DispatchObject *)user, BattleFx_SourceHoldScript);
	pusher = BattleFx_StartItemBreak(user);
	if (pusher == NULL)
	{
		BattleFx_PrepareBufferInterpolation();
		return;
	}

	// 押すオブジェクトは対象の 16 ドット上、angle の向きへ 16 ドットの位置
	pusher->linked = target;
	p = &position;
	p->x = target->x;
	p->y = target->y + (16 << 16);
	p->z = target->z;
	Vector_AddPolarOffset(16 << 16, angle, p);
	Object_SetMoveTargetFar(pusher, p->x, p->y, p->z);
	BattleFx_SnapScaleToFull((struct MotionObject *)pusher);
	pusher->speed = 4 << 16;
	pusher->lift = 0x8000;
	pusher->flag = 4;
	target->callback = ObjectGroup_ApplyRandomChildValues; // 押す間は色を巡らせる
	target->speed = 0x6666;
	target->lift = 0x3333;
	target->unknown_5a = moved;
	target->mode = 2;
	for (;;)
	{
		WaitFrames(1);
		keys = gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS);
		if (keys != 0)
			break;

		// 方向キーが離れていれば押すオブジェクトを元の位置へ戻す
		dir = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
		if (dir == 0xffff)
		{
			position.x = target->x;
			position.y = target->y + (16 << 16);
			position.z = target->z;
			Vector_AddPolarOffset(16 << 16, angle, &position);
			Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
			ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 1);
			pusher->velocity_x = 0;
			pusher->velocity_y = 0;
			pusher->velocity_z = 0;
			continue;
		}

		// 押すオブジェクトを押す向きへ 2 ドット寄せ、対象の 16 ドット先と 32 ドット先を調べる
		position.x = target->x;
		position.y = target->y + (16 << 16);
		position.z = target->z;
		Vector_AddPolarOffset(16 << 16, angle, &position);
		Vector_AddPolarOffset(2 << 16, dir, &position);
		Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
		Script_WaitForEventTimeoutFar(pusher);
		position.x = target->x;
		position.y = target->y;
		position.z = target->z;
		Vector_AddPolarOffset(16 << 16, dir, &position);
		beyond.x = target->x;
		beyond.y = target->y;
		beyond.z = target->z;
		Vector_AddPolarOffset(32 << 16, dir, &beyond);
		if (Map_CheckObjectStepFar(target, &position) > 0)
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		hit = ScriptObject_FindOverlappingEntryFar(target, p);
		if (hit == NULL)
			goto move;
		if (hit != user)
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}

		// 押す先が使い手と重なる：使い手のマスが押す先なら押せない。使い手が 32 ドット先の
		// マスにいれば、そこが通れるときだけ使い手もそこへ動かす
		cell_x = user->x & CELL_MASK;
		cell_z = user->z & CELL_MASK;
		if (cell_x == (p->x & CELL_MASK) && cell_z == (p->z & CELL_MASK))
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		if (cell_x != (beyond.x & CELL_MASK) || cell_z != (beyond.z & CELL_MASK))
			goto move;
		if (Map_GetCellAttributeLowNibbleFar(user->mode, beyond.x, beyond.z) != 0)
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		moved = 1;

		// 押す：押すオブジェクトの動作は angle と押す向きの差 (90° 単位) で選ぶ
	move:
		AudioCommand_PlayFar(175);
		x = position.x;
		z = position.z;
		ObjectDispatch_ApplyArgumentToChildrenFar(
			pusher, BattleFx_QuadrantModes[(u16)(angle - dir) >> 14]);
		WaitFrames(15);
		target->unknown_5b = 0;
		target->speed = 0x3333;
		target->lift = 0x3333;
		Object_SetMoveTargetFar(target, position.x, position.y, position.z);
		pusher->flag = 0;
		pusher->speed = 0x3333;
		pusher->lift = 0x3333;
		Vector_AddPolarOffset(16 << 16, dir, &position);
		Object_SetMoveTargetFar(pusher, position.x, position.y + (16 << 16), position.z);
		if (moved == 1)
		{
			Object_GetById(work->first_object_id)->unknown_5a &= 0xfe;
			user->speed = 0x3333;
			user->lift = 0x3333;
			Object_SetMoveTargetFar(user, beyond.x, beyond.y, beyond.z);
		}
		Script_WaitForEventTimeoutFar(target);
		target->x = x;
		target->z = z;
		target->velocity_x = 0;
		target->velocity_z = 0;
		break;
	}

	// 対象の色、スクリプト、毎フレームの処理を元に戻し、押すオブジェクトを消す
	Animation_ApplyChildValuesFar(target, work->saved_palette);
	ObjectDispatch_InitializeFar((struct DispatchObject *)target, work->saved_script);
	target->callback = work->saved_callback;
	EffectRuntime_StopCurrentObject();
	if (moved == 1)
		Object_GetById(work->first_object_id)->unknown_5a |= 1;
	BattleFx_PrepareBufferInterpolation();
	UpdateRisingParticleBurst(pusher);
}

//--------------------------------------------------------------------------
// 粒の動き：元の位置から 30 ドット先へ飛び、また元の位置へ戻って消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *slot)
{
	struct Output_08097f80 position;
	s8					  *state_ptr = &slot->state;
	s32					   state;

next_state:
	state = *state_ptr;
	if (state == 0)
	{
		u32 angle;

		position.x = slot->origin_x;
		position.z = slot->origin_z;
		angle = Random16();
		Vector_AddPolarOffset(30 << 16, (u16)angle, &position);
		slot->target_x = position.x;
		slot->target_z = position.z;
		slot->acceleration = 4 << 16;
		slot->max_speed = 4 << 16;
		slot->flag42 = state;
		(*state_ptr)++;
		return;
	}

	if (state == 1)
	{
		if (BattleFx_HasReachedTarget(slot) == 0)
		{
			(*state_ptr)++;
			goto next_state;
		}
		return;
	}

	if (state == 2)
	{
		slot->target_x = slot->origin_x;
		slot->target_z = slot->origin_z;
		{
			u32 value = 0x400;

			slot->max_turn_step = value;
		}
		slot->flag42 = 1;
		(*state_ptr)++;
		return;
	}

	if (state == 3 && BattleFx_HasReachedTarget(slot) == 0)
		BattleFx_ClearOwnedSlot(slot);
}

//--------------------------------------------------------------------------
// 使い手のそばにオブジェクトを出し、30 フレーム見せてから粒にして消す
//--------------------------------------------------------------------------
void BattleFx_RunItemBreakSequence(void)
{
	s32					  work[3];
	struct BattleFxScene *scene;
	void				 *object;

	scene = gEffectWork;
	object = scene->main_object;

	/* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling */
	do
	{
		BattleEffect_InitializeSharedScene();
	} while (0);
	object = BattleFx_StartItemBreak(object);
	BattleFx_SnapScaleToFull(object);
	if (object != 0)
	{
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 4);
		WaitFrames(30);
	}
	BattleFx_PrepareBufferInterpolation();
	UpdateRisingParticleBurst(object);
}

//--------------------------------------------------------------------------
// object の 16 ドット上にオブジェクト 215 を出し、向いている側
// (90° 単位) へ 16 ドット動かす。周りに粒 8 個を散らす
// 戻り値：出したオブジェクト (出せなければ 0)
//--------------------------------------------------------------------------
void *BattleFx_StartItemBreak(void *object)
{
	struct MotionObject *src = object;
	struct MotionObject *parent;
	struct MotionObject *child;
	s32					 angle;
	s32					 dist;
	s32					 speed;
	s32					 i;
	u32					 spread;
	u32					 jitter;

	angle = (src->angle + 0x2000) & 0xc000; // いちばん近い 90° の向き
	parent = Object_Spawn(215, src->x, src->y + (16 << 16), src->z);
	if (parent == 0)
		return 0;
	parent->scale_y = 0x4000; // 0.25 倍から
	parent->scale_x = 0x4000;
	parent->update = BattleFx_UpdateItemBreakFragment;
	parent->speed_limit = 2 << 16;
	parent->acceleration = 2 << 16;
	parent->motion_flags = 0;
	ObjectDispatch_ApplyArgumentToChildrenFar(parent, 3);
	Motion_SetTargetPositionFromMagnitudeAngle(parent, 16 << 16, angle);

	// 粒：向きの ±45° へ 8〜32 ドット、速さ 1〜2 ドット
	for (i = 7; i >= 0; i--)
	{
		child = Object_Spawn(285, src->x, src->y + (16 << 16), src->z);
		if (child != 0)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)child, BattleFx_FragmentScript);
			speed = Random16() + 0x10000;
			child->acceleration = 1 << 16;
			child->speed_limit = speed;
			child->motion_flags = 2;
			child->vertical_motion_strength = 0x51e;
			spread = Random16();
			child->velocity_y = spread - Random16();
			dist = Random16() * 24 + (8 << 16);
			jitter = Random16();
			Motion_SetTargetPositionFromMagnitudeAngle(
				child, dist, ((jitter - Random16()) >> 3) + src->angle);
		}
	}
	AudioCommand_PlayFar(SOUND_ITEM_BREAK);
	return parent;
}

//--------------------------------------------------------------------------
// 拡大率を 1/16 刻みで等倍まで上げ、位置を確定する
//--------------------------------------------------------------------------
void BattleFx_SnapScaleToFull(struct MotionObject *obj)
{
	s32 next;
	s32 scale;

	if (obj != NULL)
	{
		scale = obj->scale_x;
		if (scale <= 0xffff)
		{
			do
			{
				next = scale + 0x1000;
				scale = next;
			} while (next <= 0xffff);
			obj->scale_x = next;
			obj->scale_y = next;
		}
		Script_WaitForEventTimeoutFar();
	}
}

//--------------------------------------------------------------------------
// 31 フレームかけて回りながら上り、縮んでから粒 8 個になって消える
//--------------------------------------------------------------------------
void UpdateRisingParticleBurst(void *object)
{
	struct MotionObject *src = object;
	struct MotionObject *child;
	s32					 i;
	s32					 dist;
	u32					 spread;

	AudioCommand_PlayFar(154);
	for (i = 30; i >= 0; i--)
	{
		src->y += 1 << 16;
		src->angle += 0x2000; // 45°
		src->scale_x += -0x800;
		src->scale_y += -0x800;
		WaitFrames(1);
	}
	for (i = 7; i >= 0; i--)
	{
		child = Object_Spawn(285, src->x, src->y, src->z);
		if (child != 0)
		{
			s32 speed;
			/* FAKEMATCH: needed to match: a block-local 1.0 keeps 0x10000 in one register for both uses */
			s32 unit = 0x10000;

			ObjectDispatch_InitializeFar((struct DispatchObject *)child, BattleFx_FragmentScript);
			speed = Random16();
			child->acceleration = unit;
			child->speed_limit = speed + unit;
			child->motion_flags = 2;
			child->vertical_motion_strength = 0xa3d;
			spread = Random16();
			child->velocity_y = spread - Random16();
			dist = Random16() * 24;
			dist += 8 << 16;
			Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
		}
	}
	AudioCommand_PlayFar(131);
	ObjectDispatch_ReleaseFar(src);
}

//--------------------------------------------------------------------------
// 64 個のオブジェクトのうち、最初のパーツが絵 200 のものへ value を渡す
//--------------------------------------------------------------------------
void ObjectDispatch_ApplyValueToKind200Children(s32 value)
{
	struct ObjectRuntime *object = *(struct ObjectRuntime **)gObjectSlots;
	s32					  cnt = 63;

	do
	{
		if (object->script != NULL)
		{
			u8 kind = object->animation_kind;

			if (kind == 1)
			{
				struct AnimationObject *anim = object->animation;
				struct AnimationEntry  *entry = anim->entries[0];

				if (entry->anim_id == 200)
				{
					entry->param = value;
					anim->dirty = kind;
				}
			}
		}
		cnt--;
		object++;
	} while (cnt >= 0);
}

//--------------------------------------------------------------------------
// 効果の中心から操作キャラが 60 ドット離れるか、残り時間が尽きたら終了を頼む
// (中心と時間は RunBattleEffect08 が gEventWork の 0xcb8〜 に置く)
//--------------------------------------------------------------------------
void FieldEffect_WatchLeaderDistance(void)
{
	u8					 *work;
	struct ObjectRuntime *actor;
	s16					 *timer;
	s32					  dx;
	s32					  dz;

	work = gEventWork;
	actor = ObjectTable_Get(gGameState.selected_actor);
	if (*(s16 *)(work + 0xcc0) != 0) // 時間を減らすか
	{
		timer = (s16 *)(work + 0xcba); // 残りフレーム
		if (*timer != 0)
			(*timer)--;
	}

	// x は 0xd105 (約 0.82) 倍して比べる
	dx = Iwram_MulQ16(*(s16 *)(work + 0xcbc) - actor->x / 0x10000, 0xd105);
	dz = *(s16 *)(work + 0xcbe) - (actor->z - actor->y) / 0x10000;
	if (dx * dx + dz * dz >= 3600 || *(s16 *)(work + 0xcba) == 0)
	{
		u16 *slot = (u16 *)(work + 382); // psynergy_request
		u32	 request = 0x2090;

		*slot = request;
	}
}

//--------------------------------------------------------------------------
// markers.c
// 戦闘エフェクト 8 番：選択中のオブジェクトを中心に画面を切りかえ、印を出す
//--------------------------------------------------------------------------

#define ObjectTable_Get ((struct EffectTarget * (*)(s32)) ObjectTable_Get)

#define BattleFx_RunEventAction ((void (*)(s32, s32, s32))BattleFx_RunEventAction)

// イベントのワークと画面切りかえのワークを兼ねる
struct EffectState
{
	u8	unknown_000[0x52a];
	u16 frame;	// DisplayTransitionState の value
	s32 x;		// 同じく centre_x (16.16 固定小数)
	s32 height; // 同じく centre_y
	u8	unknown_534[0xcb8 - 0x534];
	u16 active;
	u16 duration;
	s16 center_x; // 中心 (ドット)
	s16 center_y;
	u16 step;
};
struct EffectGlobals
{
	struct EffectState *scene; // イベントのワーク
	u8					unknown_04[12];
	struct EffectState *work; // 画面切りかえのワーク
};
struct EffectTarget
{
	u8	unknown_00[8];
	s32 x; // 16.16 固定小数
	s32 y;
	s32 z;
};
void WaitFrames(s32);
s32	 BattleFx_FindMatchingEvent(s32, s32, s32 *);
void AudioCommand_PlayFar(s32);

//--------------------------------------------------------------------------
// 戦闘エフェクト 8 番
// 選択中のオブジェクトの位置を中心に画面切りかえを始め、
// 合うイベントがあれば動かしてから、まわりに印を出す
//--------------------------------------------------------------------------
void RunBattleEffect08(void)
{
	void BattleFx_ApplyColorToSourceBuffer(s32, s32);
	struct EffectState	*scene;
	struct EffectState	*work;
	struct EffectTarget *target;
	s32					 arg;
	s32					 ev;
	s32					 frame;

	scene = ((struct EffectGlobals *)&gEventWork)->scene;
	ObjectDispatch_ApplyValueToKind200Children(6);
	DisplayTransition_InitializeBattleEffectState(8);
	work = ((struct EffectGlobals *)&gEventWork)->work;
	target = ObjectTable_Get(gGameState.selected_actor);
	work->x = target->x;
	work->height = target->z - target->y;
	BattleFx_ApplyColorToSourceBuffer(0x10000, 0);
	BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
	BattleFx_StartBufferInterpolation(1);
#if !defined(TBS_EDITION_EN)
	scene->active = 0; // 英語版にはない
#endif
	WaitFrames(1);
	ev = BattleFx_FindMatchingEvent(0x50000005, 8, &arg);
	if (ev != 0)
		BattleFx_RunEventAction(ev, gGameState.selected_actor, arg);
	AudioCommand_PlayFar(131);

	// イベントのワークに中心 (ドット) と時間を入れ、近くに印を出す
	scene->active = 1;
	scene->center_x = work->x / 0x10000;
	scene->center_y = work->height / 0x10000;
	scene->duration = 600;
	scene->step = 1;
	FieldEffect_SpawnNearbyMarkers();
	frame = 0;
	do
	{
		WaitFrames(1);
		work->frame = frame;
		frame++;
	} while (frame <= 18);
	Scheduler_AddOrUpdateCallback(FieldEffect_WatchLeaderDistance, 0xc80);
}

//--------------------------------------------------------------------------

// burstobj.c

// エナジー演出：イマジンの打ち切り、演出 12 の粒、演出 7 で対象を持ち上げる

//--------------------------------------------------------------------------

#undef BattleEffect_RunTargetedItemBreak
#undef BattleFx_CallEffect03AndStop
#undef BattleFx_CallEffect04
#undef BattleFx_RunBurstParticleMainObject
#undef BattleFx_RunEventAction
#undef BattleFx_StartOrbitingParticles
#undef Object_SetMoveTargetFar

#define BattleFx_ApplyColorToTargetBuffer ((void (*)(u32, s32))BattleFx_ApplyColorToTargetBuffer)

#define ENA_EV_4 0x40000005 // エナジーのイベント、段階 4
#define FX_IMAJIN 8			// イマジンの演出
#define TSUBU_MAX 24		// 粒の数
#define TSUBU_OBJ 284		// 粒の物体
#define FALL_OBJ 217		// 落ちる物体
#define FALL_LAST 4			// 落ちる物体は 0-4 の 5 個

// イベントワークのうち、イマジンの状態
typedef struct
{
	u8	reserved000[0xcb8];
	s16 active;			  // イマジンの効果中
	s16 transition_timer; // 残りのフレーム
} SceneTransitionState;

// ワークのスロット 31 のうち、ここで使う部分
typedef struct
{
	u8 reserved000[0x53c];
	u8 transition_status;
	u8 transition_mode;
	u8 transition_phase;
} SceneTransitionScene;

struct BurstParticleVector
{
	s32 values[3]; // x, y, z
};

// 粒のスプライトの属性
struct BurstParticleVisual
{
	u8 reserved_00[5];
	u8 flags_a_low : 5;
	u8 flip : 1;
	u8 flags_a_high : 2;
	u8 reserved_06;
	u8 flags_b_low : 6;
	u8 flags_b_high : 2;
	union
	{
		struct
		{
			u16 shape : 10;
			u16 unused : 6;
		} bits;
		struct
		{
			u8 lo;
			u8 high_low : 4;
			u8 palette : 4;
		} bytes;
	} attributes;
};

struct BurstParticleVisualGroup
{
	struct BurstParticleVisual primary;
	struct BurstParticleVisual child;
};

//--------------------------------------------------------------------------
// イマジンの効果中なら打ち切る
// 見張りをやめ、色を黒から 30 フレームで戻し、イマジンの段階 4 のイベントを実行する
//--------------------------------------------------------------------------
void ResetSceneTransitionEffect(void)
{
	void						 BattleFx_ApplyColorToSourceBuffer(u32 battle_value, s32 enabled);
	// gEffectWork はスロット 56。0x74 前がイベントワーク (27)、0x64 前がスロット 31
	struct BattleFxScene **cell = &gEffectWork;
	struct BattleFxScene  *work = *cell;
	SceneTransitionScene  *scene = *(SceneTransitionScene **)((u8 *)cell - 0x64);
	SceneTransitionState  *state = *(SceneTransitionState **)((u8 *)cell - 0x74);
	s16					   zero;
	s32					   tid;
	void				  *ev;

	if (state->active != 0)
	{
		AudioCommand_PlayFar(SOUND_SCENE_TRANSITION);
		Scheduler_RemoveCallback(FieldEffect_WatchLeaderDistance);

		zero = 0;
		state->active = zero;
		state->transition_timer = zero;
		ObjectDispatch_ApplyValueToKind200Children(0);

		BattleFx_ApplyColorToTargetBuffer(PALETTE_TRANSFORM_PLAIN, 1);
		BattleFx_StartBufferInterpolation(1);
		BattleFx_ApplyColorToSourceBuffer(0, 0);
		BattleFx_ApplyColorToTargetBuffer(PALETTE_TRANSFORM_PLAIN, 0);
		BattleFx_StartBufferInterpolation(30);
		WaitFrames(1);

		ev = BattleFx_FindMatchingEvent(ENA_EV_4, FX_IMAJIN, &tid);
		if (ev != NULL)
			BattleFx_RunEventAction(ev, gGameState.selected_actor, tid);

		if (work->child_option == 0)
		{
			scene->transition_phase = 0;
			scene->transition_status = 1;
			scene->transition_mode = 1;
			WaitFrames(10);
		}
	}
}

//--------------------------------------------------------------------------
// 対象のある演出 12：対象のまわりに粒を出し、対象を足元の優先度にする
//--------------------------------------------------------------------------
void BattleFx_RunBurstParticleMainObject(void)
{
	struct FieldActor *tgt;

	tgt = gEffectWork->child;
	if (tgt != NULL)
	{
		BattleEffect_SpawnBurstParticleField();
		ObjectDispatch_ApplyArgumentToChildrenFar(tgt, 2);
		tgt->collision_flags = 0;
		ObjectDispatch_SetSingleChildFlagsFar(tgt, 0);
		tgt->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
		WaitFrames(10);
		AudioCommand_PlayFar(SOUND_RECOVERY);
		WaitFrames(40);
		BattleFx_PrepareBufferInterpolation();
	}
}

//--------------------------------------------------------------------------
// 対象のない演出 12：粒を出したあと、まわり 4-10 ドットに落ちる物体を 5 個出す
//--------------------------------------------------------------------------
void BattleFx_RunBurstParticles(void)
{
	struct BattleFxScene	   *work = gEffectWork;
	struct BurstParticleVector	pos;
	struct BurstParticleVector *p;
	s32							n;

	BattleEffect_SpawnBurstParticleField();
	AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
	p = &pos;
	n = FALL_LAST;
	do
	{
		struct FieldActor *obj;
		s32				   dist;

		p->values[0] = work->x;
		p->values[2] = work->z;
		dist = (Random16() * 6) + 0x40000;
		Vector_AddPolarOffset(dist, Random16(), p);
		p->values[1] = work->y;
		obj = Object_Spawn(FALL_OBJ, p->values[0], p->values[1], p->values[2]);
		if (obj != 0)
		{
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)obj, BattleFx_BurstParticleObjectScript);
			obj->motion_flags = ACTOR_FALLS;
		}
		WaitFrames(((Random16() * 2) >> 16) + 2);
		n--;
	} while (n >= 0);
	WaitFrames(30);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// 使う者の前から粒を 24 個、2 フレームおきに出し、演出の位置のまわりへ飛ばす
// 出る位置：向き 0x4000 は 10 ドット上、0xc000 は 24 ドット上、ほかは 10 ドット上の 10 ドット前
//--------------------------------------------------------------------------
void BattleEffect_SpawnBurstParticleField(void)
{
	struct BattleFxScene *work;
	struct FieldActor	 *user;
	s32					  pos[3];
	s32					  n;

	work = gEffectWork;
	user = work->main_object;
	BattleEffect_InitializeSharedScene();

	for (n = 0; n < TSUBU_MAX; n++)
	{
		struct FieldActor		   *obj;
		struct BurstParticleVisual *visual;
		struct BurstParticleVisual *child;
		s32							dist;

		if (work->angle == 0x4000)
		{
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + 0xa0000;
			pos[2] = user->z.fixed;
		}
		else if (work->angle == 0xc000)
		{
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + 0x180000;
			pos[2] = user->z.fixed;
		}
		else
		{
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + 0xa0000;
			pos[2] = user->z.fixed;
			Vector_AddPolarOffset(0xa0000, work->angle, pos);
		}

		// 子のスプライトに、向きと形と色を写す
		obj = Object_Spawn(TSUBU_OBJ, pos[0], pos[1], pos[2]);
		visual = &((struct BurstParticleVisualGroup *)obj->sprite)->primary;
		child = &((struct BurstParticleVisualGroup *)obj->sprite)->child;
		child->flip = visual->flip;
		child->flags_a_high = visual->flags_a_high;
		child->flags_b_high = visual->flags_b_high;
		child->attributes.bits.shape = visual->attributes.bits.shape;
		child->attributes.bytes.palette = visual->attributes.bytes.palette;

		if (obj != 0)
		{
			obj->scale_y = 0xb333; // 0.7 倍
			obj->scale_x = 0xb333;
			obj->acceleration = 0x18000;
			obj->speed = 0x18000;
			obj->motion_flags = 0;
			Animation_ApplyChildValuesFar(obj, 11);
			ObjectDispatch_ApplyArgumentToChildrenFar(obj, 7);
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)obj, BattleFx_CommonParticleScript + 1);
			ObjectDispatch_SetSingleChildFlagsFar(obj, 1);

			// 行き先：演出の位置から 4-10 ドット
			pos[0] = work->x;
			pos[1] = work->y;
			pos[2] = work->z;
			if (work->angle == 0xc000)
				Vector_AddPolarOffset(0xe0000, work->angle, pos);
			dist = Random16() * 6 + 0x40000;
			Vector_AddPolarOffset(dist, Random16(), pos);
			Object_SetMoveTargetFar(obj, pos[0], pos[1], pos[2]);
		}
		AudioCommand_PlayFar(131);
		WaitFrames(2);
	}
	WaitFrames(8);
}

//--------------------------------------------------------------------------
// 対象のある演出 7
// 左右 32 ドットに物体を出して 14 ドット動かし、対象といっしょに 32 ドット持ち上げる
//--------------------------------------------------------------------------
void BattleEffect_RunTargetedItemBreak(void)
{
	void Script_WaitForEventTimeoutFar(void *object);
	struct BattleFxScene  *work;
	struct DispatchObject *user;
	struct FieldActor	  *tgt;
	struct MotionObject	  *pair[2];
	s32					   pos[3];
	s32					   x, y, z;
	s32					   n;
	s32					   start_y;

	work = gEffectWork;
	tgt = work->child;
	user = work->main_object;
	if (tgt == NULL)
		return;

	BattleEffect_InitializeSharedScene();
	user->argument = (s32)tgt;
	ObjectDispatch_InitializeFar(user, BattleFx_CommonParticleScript + 3);

	x = work->x;
	pos[0] = x;
	y = work->y + 0x100000; // 16 ドット上
	pos[1] = y;
	z = work->z;
	pos[2] = z;
	pair[0] = BattleFx_SpawnItemBreakMode3(x + 0x200000, y, z, 0x8000);
	pair[1] = BattleFx_SpawnItemBreakMode3(pos[0] - 0x200000, pos[1], pos[2], 0);

	WaitFrames(15);
	for (n = 0; n < 2; n++)
	{
		struct MotionObject *obj = pair[n];
		if (obj != NULL)
			Motion_SetTargetPositionFromMagnitudeAngle(obj, 0xe0000, obj->angle);
	}

	Script_WaitForEventTimeoutFar(pair[0]);
	tgt->update = (void (*)(union FieldObject *))ObjectGroup_ApplyRandomChildValues;
	AudioCommand_PlayFar(130);
	tgt->motion_flags = 4;

	// 1/4 ドットずつ 32 ドット上げる
	start_y = tgt->y.fixed;
	if (pair[0] != NULL && pair[1] != NULL && start_y <= start_y + 0x200000)
	{
		do
		{
			pair[0]->y += 0x4000;
			pair[1]->y += 0x4000;
			tgt->y.fixed += 0x4000;
			WaitFrames(1);
		} while (tgt->y.fixed <= start_y + 0x200000);
	}

	UpdateRisingParticleBurst(pair[0]);
	UpdateRisingParticleBurst(pair[1]);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// anchor.c
// エナジー演出 7：左右の二つの物体と、飛び散る破片
//--------------------------------------------------------------------------

void						 AudioCommand_PlayFar(s32 sound);
extern const s32			 BattleFx_FragmentScript[];

#define HAHEN_MAX 24 // 破片の数

//--------------------------------------------------------------------------
// 対象のない演出 7
// 演出の位置の左右 32 ドットに物体を出して動かし、破片を 24 個散らす
//--------------------------------------------------------------------------
void RunBattleEffect07(void)
{
	void						 ObjectDispatch_ReleaseFar(void *object);
	void						 Script_WaitForEventTimeoutFar(void *object);
	struct BattleFxScene  *work;
	struct MotionObject	  *obj;
	struct MotionObject	  *pair[2];
	struct ScatterPosition pos;
	struct MotionObject	 **walk;
	s32					   n;
	s32					   dist;

	work = gEffectWork;
	BattleEffect_InitializeSharedScene();
	pos.x = work->x;
	pos.y = work->y + 0x100000; // 16 ドット上
	pos.z = work->z;
	pair[0] = BattleFx_SpawnItemBreakMode3(pos.x + 0x200000, pos.y, pos.z, 0x8000);
	pair[1] = BattleFx_SpawnItemBreakMode3(pos.x - 0x200000, pos.y, pos.z, 0);

	// それぞれの向きへ 24 ドット動かす
	WaitFrames(15);
	walk = pair;
	for (n = 0; n < 2; n++)
	{
		obj = *walk++;
		if (obj != 0)
			Motion_SetTargetPositionFromMagnitudeAngle(obj, 0x180000, obj->angle);
	}

	// 破片：8-32 ドット先へ散る
	Script_WaitForEventTimeoutFar(pair[0]);
	AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
	for (n = 0; n < HAHEN_MAX; n++)
	{
		pos.x = work->x;
		pos.y = work->y + 0x100000;
		pos.z = work->z;
		obj = Object_Spawn(285, pos.x, pos.y, pos.z);
		if (obj != 0)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_FragmentScript);
			obj->speed_limit = Random16() + 0x20000;
			obj->acceleration = 0x20000;
			obj->motion_flags = 0;
			dist = Random16() * 24 + 0x80000;
			Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
		}
	}

	ObjectDispatch_ReleaseFar(pair[0]);
	ObjectDispatch_ReleaseFar(pair[1]);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// 物体 215 を出し、回しながら 1/4 から等倍まで 24 フレームで大きくする
// angle : 大きくなったあとの向き
//--------------------------------------------------------------------------
struct MotionObject *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle)
{
	struct MotionObject *obj;
	s32					 scale;

	AudioCommand_PlayFar(SOUND_ITEM_BREAK);
	obj = Object_Spawn(215, x, y, z);
	if (obj != NULL)
	{
		obj->scale_x = obj->scale_y = 0x4000;
		obj->acceleration = obj->speed_limit = 0x30000; // 3 ドット/フレーム
		((struct FieldSprite *)obj->records)->priority = 0;
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 3);
		scale = obj->scale_x;
		if (scale < 0x10000)
		{
			do
			{
				scale += 0x800;
				obj->scale_y = scale;
				obj->scale_x = scale;
				obj->angle += 0x2000; // 1/8 回転
				WaitFrames(1);
				scale = obj->scale_x;
			} while (scale <= 0xffff);
		}
		obj->angle = angle;
	}
	return obj;
}

//--------------------------------------------------------------------------
// impact.c
// 散って集まる粒の動きと、重い衝撃の破片
//--------------------------------------------------------------------------

#undef Motion_SetTargetPositionFromMagnitudeAngle

#define Camera_WorldToScreen ((void (*)(struct EffectPosition *))Camera_WorldToScreen)

struct EffectPosition
{
	s32 x; // 16.16 固定小数
	s32 y;
	s32 z;
};

void BattleFx_ClearOwnedSlot(struct EffectSlot *effect);

//--------------------------------------------------------------------------
// 粒の動き : 原点のまわりへ散ってから、主オブジェクトのそばへ集まって消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialBurst(struct EffectSlot *slot)
{
	struct BattleFxScene *scene = gEffectWork;
	struct EffectPosition pos;
	s32					  state;

again:
	state = slot->state;
	if (state == 0)
	{
		// 原点から 25 ドット、でたらめな方向へ
		pos.x = slot->origin_x;
		pos.z = slot->origin_z;
		Vector_AddPolarOffset(25 << 16, (u16)Random16(), &pos);
		slot->target_x = pos.x;
		slot->target_z = pos.z;
		slot->acceleration = 3 << 16;
		slot->max_speed = 3 << 16;
		slot->flag42 = 0;
		slot->state++;
		return;
	}

	if (state == 1)
	{
		// 着いたらすぐ次へ
		if (BattleFx_HasReachedTarget(slot) == 0)
		{
			slot->state++;
			goto again;
		}
		return;
	}

	if (state == 2)
	{
		// 主オブジェクトの 16 ドット上から scene->angle へ 8 ドット、
		// 画面座標にして 4 ドットの揺らぎを足す
		struct MotionObject *obj = scene->main_object;

		pos.x = obj->x;
		pos.y = obj->y + (16 << 16);
		pos.z = obj->z;
		Vector_AddPolarOffset(8 << 16, scene->angle, &pos);
		Camera_WorldToScreen(&pos);
		Vector_AddPolarOffset(4 << 16, Random16(), &pos);
		slot->target_x = pos.x;
		slot->target_z = pos.z;
		slot->max_turn_step = 0x800;
		slot->flag42 = 1;
		slot->state++;
		return;
	}

	if (state == 3 && BattleFx_HasReachedTarget(slot) == 0)
		BattleFx_ClearOwnedSlot(slot);
}

//--------------------------------------------------------------------------
// 重い衝撃 : y を 32 ドット下げた位置に 0x11b を置き、破片を 12 個まき散らす
// src : 衝撃の位置になるオブジェクト
//--------------------------------------------------------------------------
s32 SpawnHeavyImpactEffect(struct MotionObject *src)
{
	struct EffectPosition pos;
	struct ObjectRuntime *obj;
	struct ObjectRuntime *frag;
	s32					  i;

	AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
	pos.x = src->x;
	pos.y = src->y;
	pos.z = src->z;
	obj = Object_Spawn(0x11b, pos.x, pos.y - (32 << 16), pos.z);
	if (obj != NULL)
	{
		obj->flags = 0;
		obj->unknown_5e = 20;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}

	// 破片 : 速さ 1〜2 ドット、8〜32 ドット先のでたらめな方向へ
	for (i = 0; i < 12; i++)
	{
		frag = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
		if (frag != NULL)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)frag, BattleFx_FragmentScript);
			frag->speed_limit = Random16() + (1 << 16);
			frag->acceleration = 1 << 16;
			frag->flags = 0;
			Motion_SetTargetPositionFromMagnitudeAngle(
				frag, Random16() * 24 + (8 << 16), Random16());
		}
	}
	return 0;
}

//--------------------------------------------------------------------------
// 戦闘エフェクト 4 番を呼ぶ
//--------------------------------------------------------------------------
void BattleFx_CallEffect04(void)
{
	RunBattleEffect04();
}

//--------------------------------------------------------------------------
// orbit.c
// フィールドのエナジー演出：4 番（押し出す弾）、6 番（渦を巻いて集まる粒）
//--------------------------------------------------------------------------

#undef Camera_WorldToScreen

void Audio_PlayCue(s32 cue);

#define ObjectGroup_SetChildValueUnlessFifteenFar \
	((void (*)(s32, s32))ObjectGroup_SetChildValueUnlessFifteenFar)
#define ScriptObject_CheckOverlapFar \
	((s32 (*)(struct FieldActor *, struct BurstPosition *))ScriptObject_CheckOverlapFar)
#define Object_ReplaceResourceEntry                                                     \
	((struct AnimationObject * (*)(struct AnimationObject *, struct AnimationObject *)) \
			Object_ReplaceResourceEntry)
#undef Camera_WorldToScreen
#define Camera_WorldToScreen ((void (*)(struct BurstPosition *))Camera_WorldToScreen)

struct BurstPosition
{
	s32 x, y, z; // 16.16 ドット
};

#define UE (16 << 16) // 16 ドット上

//--------------------------------------------------------------------------
// obj の 16 ドット上の位置
//--------------------------------------------------------------------------
static __inline__ void PosAbove(struct FieldActor *obj, struct BurstPosition *pos)
{
	pos->x = obj->x.fixed;
	pos->y = obj->y.fixed + UE;
	pos->z = obj->z.fixed;
}

struct SparkObject
{
	u8	unknown_00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	unknown_14[4];
	s32 scale_x;
	s32 scale_y;
	u8	unknown_20[24];
	s32 anchor_x; // ゆれの中心（FieldActor の target_x の位置）
	u8	unknown_3c[12];
	s32 gravity; // FieldActor と同じ位置
	u8	unknown_4c[9];
	u8	motion_flags; // FieldActor と同じ位置
	u8	unknown_56[8];
	u16 unknown_5e;
	u8	unknown_60[4];
	s16 phase; // ゆれの位相（128 フレームで 1 周）
	u8	unknown_66[6];
	void (*update)(struct SparkObject *);
};

//--------------------------------------------------------------------------
// エナジー 4 番 : 使う者の前に弾を出して大きくし、残像 3 つと前へ飛ばす
// 当たった物（scene->child）は 16 ドット押し出す
//--------------------------------------------------------------------------
void RunBattleEffect04(void)
{
	s32	 Map_CheckObjectStepFar(struct FieldActor *object, struct BurstPosition *pos);
	struct FieldActor	   *tgt;
	s32						ctx;
	struct FieldActor	   *copies[4];
	struct BurstPosition	pos;
	struct FieldActor	   *obj;
	struct FieldActor	   *copy;
	struct FieldActor	   *src;
	struct BattleFxScene   *scene;
	struct EffectSlot	   *slot;
	struct AnimationObject *res;
	s32						ev;
	s32						scale;
	s32						i;
	u8						res_slot;

	scene = gEffectWork;
	tgt = scene->child;
	BattleEffect_InitializeSharedScene();
	AudioCommand_PlayFar(130);
	slot = scene->slots;

	// 使う者の頭上から、光を 2 フレームおきに 12 個はじけさせる
	for (i = 0; i <= 11; i++)
	{
		src = scene->main_object;
		PosAbove(src, &pos);
		Camera_WorldToScreen(&pos);
		EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
		EffectSlot_SetCallback(slot, BattleFx_UpdateRadialBurst);
		EffectSlot_SetObjectMode(slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar((s32)slot->object, 9);
		slot->scale_y = 0xb333; // 0.7 倍
		slot->scale_x = 0xb333;
		WaitFrames(2);
		slot++;
	}

	// 弾：使う者の向きに 8 ドット先で、0.25 倍から等倍まで大きくする
	src = scene->main_object;
	pos.x = src->x.fixed;
	pos.y = src->y.fixed + UE;
	pos.z = src->z.fixed;
	Vector_AddPolarOffset(8 << 16, scene->angle, &pos);
	obj = Object_Spawn(0xd7, pos.x, pos.y, pos.z);
	if (obj == NULL)
	{
		BattleFx_PrepareBufferInterpolation();
		return;
	}
	obj->scale_y = 0x4000;
	obj->scale_x = 0x4000;
	obj->facing = scene->angle;
	obj->speed = 4 << 16;
	obj->acceleration = 4 << 16;
	obj->motion_flags = 0;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 5);
	Animation_ApplyChildValuesFar(obj, 3);
	scale = obj->scale_x;
	if (scale < 0x10000)
	{
		do
		{
			scale += 0x500;
			obj->scale_y = scale;
			obj->scale_x = scale;
			WaitFrames(1);
			scale = obj->scale_x;
		} while (scale <= 0xffff);
	}
	WaitFrames(3);

	// 残像 3 つ。絵の資源はひとつを分けあう
	res = NULL;
	for (i = 2; i >= 0; i--)
	{
		copy = copies[i] = Object_Spawn(0xd7, obj->x.fixed, obj->y.fixed, obj->z.fixed);
		if (copy != NULL)
		{
			copy->scale_y = 0xf000;
			copy->scale_x = 0xf000;
			copy->facing = scene->angle;
			copy->speed = 4 << 16;
			copy->acceleration = 4 << 16;
			copy->motion_flags = 0;
			ObjectDispatch_ApplyArgumentToChildrenFar(copy, 5);
			Animation_ApplyChildValuesFar(copy, 2);
			res = Object_ReplaceResourceEntry((struct AnimationObject *)copy->sprite, res);
		}
	}
	res_slot = res->slot;

	// 行き先：scene->enabled なら使う者の 56 ドット先、ほかは scene の位置
	if (scene->enabled != 0)
	{
		src = scene->main_object;
		pos.x = src->x.fixed;
		pos.y = src->y.fixed + UE;
		pos.z = src->z.fixed;
		Vector_AddPolarOffset(56 << 16, scene->angle, &pos);
	}
	else
	{
		pos.x = scene->x;
		pos.y = scene->y + UE;
		pos.z = scene->z;
	}
	Object_SetMoveTargetFar(obj, pos.x, pos.y, pos.z);
	ObjectDispatch_InitializeFar(
		(struct DispatchObject *)obj, BattleFx_BurstParticleObjectScript + 4);
	for (i = 0; i < 3; i++)
	{
		copy = copies[i];
		if (copy != NULL)
		{
			WaitFrames(3);
			Object_SetMoveTargetFar(copy, pos.x, pos.y, pos.z);
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)copy, BattleFx_CommonParticleScript + 1);
		}
	}

	// 弾のスクリプトが終わるまで、60 フレームまで待つ
	i = 0;
	if (((struct ScriptMotionObject *)obj)->script != NULL)
	{
	wait_script:
		WaitFrames(1);
		i++;
		if (i <= 59 && ((struct ScriptMotionObject *)obj)->script != NULL)
			goto wait_script;
	}

	// 当たった物を、ぶつからなければ 16 ドット押し出す（child_option なら跳ねる）
	if (tgt != NULL && scene->child_mode == 0)
	{
		if (scene->child_option != 0)
			tgt->velocity_y = 8 << 16;
		pos.x = tgt->x.fixed;
		pos.y = tgt->y.fixed;
		pos.z = tgt->z.fixed;
		Vector_AddPolarOffset(16 << 16, scene->angle, &pos);
		if (Map_CheckObjectStepFar(tgt, &pos) == 0 && ScriptObject_CheckOverlapFar(tgt, &pos) == 0)
		{
			tgt->acceleration = 1 << 16;
			tgt->speed = 1 << 16;
			Object_SetMoveTargetFar(tgt, pos.x, pos.y, pos.z);
		}
	}

	// 応じるマップのイベントがあれば動かす
	ev = BattleFx_FindMatchingEvent(0x50000005, 4, &ctx);
	if (ev != 0)
		BattleFx_RunEventAction((void *)ev, gGameState.selected_actor, ctx);
	WaitFrames(10);
	BattleFx_PrepareBufferInterpolation();
	WaitFrames(20);
	if (res_slot != 96)
		Resource_ResetEntry(res_slot);
}

//--------------------------------------------------------------------------
// 毎フレームの処理：8 フレームごとに 2 フレームだけアニメ 2 にして光らせる
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleMain(struct FieldActor *particle)
{
	s32 frame = gFrameCount & 7;
	if (frame == 0)
	{
		Animation_ApplyChildValuesFar(particle, 2);
	}
	else if (frame == 2)
	{
		Animation_ApplyChildValuesFar(particle, 0);
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理：1/16 ずつ縮み、1/16 以下で消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleFade(struct FieldActor *particle)
{
	s32 scale;

	if (particle != NULL)
	{
		scale = particle->scale_x - 0x1000;
		particle->scale_y -= 0x1000;
		particle->scale_x = scale;
		if (scale <= 0x1000)
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)particle, BattleFx_CommonParticleScript);
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理：target のまわりを左まわりに渦を巻いて寄る
// 半径は残りフレーム × 2 ドット、1 フレームに 0x800 まわる
// work は始めの向き、timer は残りフレーム。0 で縮んで消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleLeft(struct FieldActor *particle)
{
	struct BurstPosition pos;
	s16					 life;
	s32					 cnt;

	if (particle != NULL)
	{
		cnt = (u16)particle->timer - 1;
		particle->timer = cnt;
		life = cnt;
		if (life != 0)
		{
			pos.x = particle->target_x;
			pos.y = particle->target_y;
			pos.z = particle->target_z;
			Vector_AddPolarOffset(life << 17, (s16)particle->work + (life << 11), &pos);
			particle->x.fixed = pos.x;
			particle->y.fixed = pos.y;
			particle->z.fixed = pos.z;
		}
		else
		{
			particle->update = (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleFade;
		}
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理：左まわりと同じで、右まわり
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleRight(struct FieldActor *particle)
{
	struct BurstPosition pos;
	s16					 life;
	s32					 cnt;

	if (particle != NULL)
	{
		cnt = (u16)particle->timer - 1;
		particle->timer = cnt;
		life = cnt;
		if (life != 0)
		{
			pos.x = particle->target_x;
			pos.y = particle->target_y;
			pos.z = particle->target_z;
			Vector_AddPolarOffset(life << 17, (s16)particle->work - (life << 11), &pos);
			particle->x.fixed = pos.x;
			particle->y.fixed = pos.y;
			particle->z.fixed = pos.z;
		}
		else
		{
			particle->update = (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleFade;
		}
	}
}

//--------------------------------------------------------------------------
// エナジー 6 番をかけられた物（scene->child）から始める
// child_mode があれば、物は変えずに粒だけ出す
//--------------------------------------------------------------------------
void BattleFx_StartOrbitingParticles(void)
{
	struct BattleFxScene *scene = gEffectWork;
	struct FieldActor	 *tgt = scene->child;

	if (tgt != 0)
	{
		if (scene->child_mode != 0)
		{
			scene->enabled = 1;
		}
		tgt->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
		BattleFx_RunOrbitingParticles();
	}
}

//--------------------------------------------------------------------------
// エナジー 6 番 : 粒 16 個が scene の位置へ渦を巻いて集まり、
// 物を 16 回光らせる。child_option がなければ物を mode 2 にする
//--------------------------------------------------------------------------
void BattleFx_RunOrbitingParticles(void)
{
	s32					  ctx;
	struct BurstPosition  pos;
	struct BurstPosition *p;
	struct BattleFxScene *scene;
	struct FieldActor	 *tgt;
	struct FieldActor	 *particle;
	void				 *ev;
	s32					  cnt;

	scene = gEffectWork;
	tgt = scene->child;
	BattleEffect_InitializeSharedScene();
	AudioCommand_PlayFar(115);

	// 3 フレームおきに粒を出す。大きさ 0.5〜1 倍、まわる向きはでたらめ
	// 中心は scene の位置から 2〜6 ドットずらす
	p = &pos;
	cnt = 15;
	do
	{
		particle = Object_Spawn(0xe8, 0, 0, 0);
		if (particle != NULL)
		{
			u32 scale;
			s32 dist;

			scale = (Random16() >> 1) + 0x8000;
			particle->scale_y = scale;
			particle->scale_x = scale;
			if ((Random16() & 1) != 0)
				particle->update =
					(void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleLeft;
			else
				particle->update =
					(void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleRight;

			particle->facing = Random16();
			particle->timer = 60;
			particle->work = Random16();
			Animation_ApplyChildValuesFar(particle, 9);

			p->x = scene->x;
			p->y = scene->y;
			p->z = scene->z;
			dist = (Random16() << 2) + (2 << 16);
			Vector_AddPolarOffset(dist, Random16(), p);
			particle->target_x = p->x;
			particle->target_y = p->y;
			particle->target_z = p->z;
		}

		WaitFrames(3);
		cnt--;
	} while (cnt >= 0);

	WaitFrames(10);
	AudioCommand_PlayFar(115);
	WaitFrames(50);

	if (tgt != NULL && scene->enabled == 0)
	{
		AudioCommand_PlayFar(212);

		// 5 フレームごとに 1 フレーム光る
		cnt = 15;
		do
		{
			Animation_ApplyChildValuesFar(tgt, 7);
			WaitFrames(1);
			Animation_ApplyChildValuesFar(tgt, 0);
			WaitFrames(4);
			cnt--;
		} while (cnt >= 0);

		if (scene->child_option == 0)
		{
			AudioCommand_PlayFar(220);
			ObjectDispatch_ApplyArgumentToChildrenFar(tgt, 2);
		}

		tgt->update = (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleMain;
		ev = BattleFx_FindMatchingEvent(0x50000005, 6, &ctx);
		if (ev != NULL)
		{
			BattleFx_RunEventAction(ev, gGameState.selected_actor, ctx);
		}
		WaitFrames(20);
	}

	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// 毎フレームの処理：anchor_x を中心に左右 4 ドット、128 フレームで 1 往復ゆれる
//--------------------------------------------------------------------------
void BattleFx_SwayParticle(struct SparkObject *obj)
{
	s16 *phase;

	phase = &obj->phase;
	obj->x = obj->anchor_x + Iwram_MulQ16(4 << 16, Trig_Sin(*phase << 9));
	(*phase)++;
	*phase = (*phase + 128) % 128;
}

//--------------------------------------------------------------------------
// 毎フレームの処理：scene の位置の 10 ドット上を中心に、渦を巻いて寄る
// 半径は残りフレームと同じドット数。timer が 0 で消える
//--------------------------------------------------------------------------
void BattleFx_UpdateShrinkingOrbitObject(struct FieldActor *obj)
{
	struct BattleFxScene *scene = gEffectWork;
	struct BurstPosition  pos;
	s16					  life;
	s32					  cnt;

	if (obj != NULL)
	{
		cnt = (u16)obj->timer - 1;
		obj->timer = cnt;
		life = cnt;
		if (life != 0)
		{
			pos.x = scene->x;
			pos.y = scene->y + (10 << 16);
			pos.z = scene->z;
			Vector_AddPolarOffset(life << 16, (s16)obj->work + (life << 11), &pos);
			obj->x.fixed = pos.x;
			obj->y.fixed = pos.y;
			obj->z.fixed = pos.z;
		}
		else
		{
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)obj, BattleFx_CommonParticleScript);
		}
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理：scene の位置の 16 ドット上で左右 6 ドット（64 フレーム）ゆれ、
// 3 フレームごとに、ゆれながらゆっくり落ちる粒を出す
// phase が -1 なら、その場から動かない
//--------------------------------------------------------------------------
void BattleFx_RunSparkEmitter(struct SparkObject *obj)
{
	struct BattleFxScene *scene;
	s16					 *phase;
	struct SparkObject	 *spark;
	s32					  pos[3];

	phase = &obj->phase;
	scene = gEffectWork;
	if (*phase != -1)
	{
		obj->x = scene->x + Iwram_MulQ16(6 << 16, Trig_Sin(*phase << 10));
		obj->y = scene->y + UE;
		obj->z = scene->z;
		(*phase)++;
		*phase = (*phase + 64) % 64;
	}
	if (gFrameCount % 3 == 0)
	{
		// 2 ドット上から、でたらめな向きに 0〜6 ドットずらす
		pos[0] = obj->x;
		pos[1] = obj->y + (2 << 16);
		pos[2] = obj->z;
		Vector_AddPolarOffset(Random16() * 6, Random16(), pos);
		spark = Object_Spawn(0x11d, pos[0], pos[1], pos[2]);
		if (spark != 0)
		{
			spark->update = BattleFx_SwayParticle;
			spark->scale_x = spark->scale_y = 0x9999; // 0.6 倍
			spark->motion_flags = ACTOR_FALLS;
			spark->gravity = 458;
			spark->phase = Random16() >> 9;
			spark->anchor_x = spark->x;
			Animation_ApplyChildValuesFar(spark, 9);
			spark->unknown_5e = 72;
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)spark, BattleFx_CommonParticleScript);
		}
	}
}

//--------------------------------------------------------------------------
// call_effect_03_and_stop.c
// 演出 03：渦を巻いて集まる 8 個の粒と、火花を出して飛び去る 1 個
//--------------------------------------------------------------------------

#undef Object_ReplaceResourceEntry

#undef Motion_SetTargetPositionFromMagnitudeAngle
#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(struct BattleEffect03Object *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

#define BattleFx_UpdateShrinkingOrbitObject ((void (*)(void))BattleFx_UpdateShrinkingOrbitObject)
#define BattleFx_RunSparkEmitter ((void (*)(void))BattleFx_RunSparkEmitter)
#undef Object_ReplaceResourceEntry
#define Object_ReplaceResourceEntry                                         \
	((struct BattleEffect03Link * (*)(void *, struct BattleEffect03Link *)) \
			Object_ReplaceResourceEntry)

//--------------------------------------------------------------------------
// 演出 03 を流し、いまのオブジェクトを止める
//--------------------------------------------------------------------------
void BattleFx_CallEffect03AndStop(void)
{
	RunBattleEffect03();
	EffectRuntime_StopCurrentObject();
}

struct BattleEffect03Object
{
	u8	  reserved_00[8];
	s32	  x;
	s32	  y;
	s32	  z;
	u8	  reserved_14[4];
	s32	  scale_x;
	s32	  scale_y;
	u8	  reserved_20[16];
	s32	  velocity_x;
	s32	  velocity_y;
	u8	  reserved_38[24];
	void *visual;
	u8	  reserved_54;
	u8	  mode;
	u8	  reserved_56[4];
	u8	  unknown_5a;
	u8	  reserved_5b[9];
	s16	  angle; // 渦の粒では残りフレーム（BattleFx_UpdateShrinkingOrbitObject）
	s16	  phase; // 渦の粒では最初の角度
	u8	  reserved_68[4];
	void *callback;
};

struct BattleEffect03Link
{
	u8 reserved_00[28];
	u8 marker;
};

//--------------------------------------------------------------------------
// 演出 03
// 8 個の粒が渦を巻いて中心へ集まり、中心に現れた 1 個が火花を出してから
// 遠くへ飛び去る
//--------------------------------------------------------------------------
void RunBattleEffect03(void)
{
	void						 ObjectDispatch_ReleaseFar(void *);
	void						 Script_WaitForEventTimeoutFar(void *);
	struct BattleFxScene		*scn = gEffectWork;
	struct BattleEffect03Object *target = scn->main_object;
	struct BattleEffect03Object *object;
	struct BattleEffect03Link	*last;
	u8							 marker;
	s32							 i;

	BattleEffect_InitializeSharedScene();

	// 渦の粒：1 フレームに 1 個ずつ、1/8 周ずらして出す
	last = 0;
	for (i = 0; i < 8; i++)
	{
		object = Object_Spawn(233, target->x, target->y + 0x200000, target->z);
		if (object != 0)
		{
			object->scale_y = 0xb333; // 0.7 倍
			object->scale_x = 0xb333;
			object->callback = BattleFx_UpdateShrinkingOrbitObject;
			object->angle = 120;
			object->phase = i << 13;
			object->mode = 4;
			last = Object_ReplaceResourceEntry(object->visual, last);
		}
		WaitFrames(1);
	}

	marker = last->marker;
	AudioCommand_PlayFar(130);
	WaitFrames(110);

	// 中心の 1 個
	target = Object_Spawn(233, 0, 0, 0);
	object = target;
	if (target != 0)
	{
		target->scale_y = 0xb333;
		target->scale_x = 0xb333;
		target->x = scn->x;
		target->y = scn->y + 0x100000;
		target->z = scn->z;
		target->mode = 4;
		Animation_ApplyChildValuesFar(target, 7);
	}

	AudioCommand_PlayFar(131);
	WaitFrames(12);
	if (target != 0)
	{
		// 2 フレームごとに子の値を切り替える（4 回に 1 回は 10）
		for (i = 0; i < 30; i++)
		{
			/* FAKEMATCH: needed to match: the ROM keeps object as a second register copy of the spark (r5 beside r6); this redundant reassignment stops gcse copy-propagating object = target away, and loop then drops it (differ 1 without it). */
			target = object;
			if (i & 3)
				Animation_ApplyChildValuesFar(object, 9);
			else
				Animation_ApplyChildValuesFar(object, 10);
			WaitFrames(2);
		}
	}

	// 火花
	Animation_ApplyChildValuesFar(object, 0);
	AudioCommand_PlayFar(84);
	if (object != 0)
	{
		target->callback = BattleFx_RunSparkEmitter;
		target->angle = 0;
		if (scn->enabled != 0)
			WaitFrames(128);
		else
			WaitFrames(192);
	}

	// 飛び去る
	if (target != 0)
	{
		target->angle = -1;
		target->velocity_x = 0x50000;
		target->velocity_y = 0x6666;
		target->unknown_5a = 0;
		Motion_SetTargetPositionFromMagnitudeAngle(target, 0xc00000, 0xe800);
		Script_WaitForEventTimeoutFar(target);
		ObjectDispatch_ReleaseFar(target);
	}
	if (marker != 0x60)
		Resource_ResetEntry(marker);
	if (scn->finish_callback != 0)
		scn->finish_callback();
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------

// orb.c

// 姿を隠すあいだの足もとの確認と、戦闘エフェクト 5 番 (光の玉と降る粒)

//--------------------------------------------------------------------------

#undef BattleFx_CallEffect05
#undef Motion_SetTargetPositionFromMagnitudeAngle
#undef ObjectTable_Get
#undef ScriptObject_CheckOverlapFar

extern u8 gMapCellBuffer[];

#define CLOAK_TILE 0xfb	 // 隠れていられるタイルの属性
#define CLOAK_END 0x2092 // 隠れていられないときの psynergy_request

//--------------------------------------------------------------------------
// 毎フレームの処理 : 選択中のオブジェクトの足もとのタイルが
// CLOAK_TILE でなければ、CLOAK_END を要求する
// イベントの mode が 3 なら 32 ドット四方の gMapBlocks、
// ほかは 16 ドット四方の地形のマップを見る
//--------------------------------------------------------------------------
void CheckObjectMapTile(void)
{
	u32					  slot;
	struct EventRuntime	 *ev;
	struct ObjectRuntime *obj;
	u8					 *tile;
	s32					  x;
	s32					  y;

	slot = (u32)&gEventWork;
	ev = gEventWork;
	obj = ObjectTable_Get(gGameState.selected_actor);
	tile = (u8 *)*(struct MapState **)(slot - 76); // gMapWork[0]

	if (ev->mode_19e == 3)
	{
		u32 tx;
		u32 ty;

		x = obj->x;
		if (x < 0)
			x += 0x1fffff;
		tx = (x >> 21) & 31;

		y = obj->z;
		if (y < 0)
			y += 0x1fffff;
		ty = (y >> 21) & 31;

		tile = (u8 *)gMapBlocks + ((tx + (ty << 5)) << 2);
	}
	else
	{
		if (obj->terrain_id <= 2)
		{
			tile = (u8 *)((struct MapState *)tile)->layers[obj->terrain_id].cells;
		}
		else
			tile = (u8 *)gMapCellBuffer;

		x = obj->x;
		if (x < 0)
			x += 0xfffff;
		{
			u32 tx = x >> 20;

			y = obj->z;
			if (y < 0)
				y += 0xfffff;

			tile = (u8 *)(tile + ((tx + ((y >> 20) << 7)) << 2));
		}
	}

	if (tile[2] != CLOAK_TILE)
		((struct EventWork *)ev)->psynergy_request = CLOAK_END;
}

//--------------------------------------------------------------------------
// 姿を戻す : 確認を止め、点滅させて cloaked を消す
//--------------------------------------------------------------------------
void MapEvent_RunTileTriggerSequence(void)
{
	struct ObjectRuntime   *obj;
	struct AnimationObject *anim;
	struct AnimationEntry  *entry;
	u32						i;

	obj = ObjectTable_Get(gGameState.selected_actor);
	anim = obj->animation;
	entry = anim->entries[0];

	AudioCommand_PlayFar(154);
	Scheduler_RemoveCallback(CheckObjectMapTile);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
	((struct ScriptMotionObject *)obj)->hook = 0;

	for (i = 0; i < 5; ++i)
	{
		entry->param = 7;
		anim->dirty = 1;
		anim->flags = 2;
		WaitFrames(2);
		anim->dirty = 1;
		anim->flags = 0;
		WaitFrames(2);
	}

	for (i = 0; i < 5; ++i)
	{
		entry->param = 7;
		anim->dirty = 1;
		anim->flags = 0;
		WaitFrames(2);
		entry->param = 0;
		anim->dirty = 1;
		WaitFrames(2);
	}

	anim->flags = 1;
	gGameState.cloaked = 0;
}

//--------------------------------------------------------------------------
// 隠れているなら、足もとの確認を始める
//--------------------------------------------------------------------------
void BattleFx_ScheduleCloakedMapCheck(void)
{
	if (gGameState.cloaked != 0)
	{
		Scheduler_AddOrUpdateCallback((SchedulerCallback)CheckObjectMapTile, 0xc80);
	}
}

//--------------------------------------------------------------------------
// 姿を隠す : 10 回点滅させて cloaked を立て、足もとを確かめる
// その場で隠れられなければ、すぐに姿を戻す
//--------------------------------------------------------------------------
void BattleFx_RunFlashingCallbackSequence(void)
{
	struct EventWork	   *ev = gEventWork;
	struct ObjectRuntime   *obj = ObjectTable_Get(gGameState.selected_actor);
	struct AnimationObject *anim = obj->animation;
	struct AnimationEntry  *entry = anim->entries[0];
	u32						n;
	void (*check)(void);

	AudioCommand_PlayFar(130);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
	((struct ScriptMotionObject *)obj)->hook = 0;
	n = 0;
	do
	{
		entry->param = 7;
		anim->dirty = 1;
		anim->flags = 2;
		WaitFrames(2);
		anim->dirty = 1;
		anim->flags = 0;
		WaitFrames(2);
		n++;
	} while (n <= 9);
	n = 0;
	entry->param = n;
	anim->flags = 2;
	anim->dirty = 1;
	check = CheckObjectMapTile;
	Scheduler_AddOrUpdateCallback(check, 0xc80);
	gGameState.cloaked = 1;
	check();
	if (ev->psynergy_request == CLOAK_END)
	{
		MapEvent_RunTileTriggerSequence();
		ev->psynergy_request = n;
	}
}

//--------------------------------------------------------------------------
// 戦闘エフェクト 5 番
//--------------------------------------------------------------------------
struct Effect05Point
{
	s32 x, y, z; // 16.16 固定小数
};

struct Effect05Target
{
	u8					 reserved_00[8];
	struct Effect05Point position;
};

struct Effect05Sprite
{
	u32 unknown_00[2];
	u8	unknown_08;
	u32 unknown_09_0 : 2;
	u32 mode : 2;
	u32 unknown_09_4 : 4;
};

struct Effect05Object
{
	u8					   reserved_00[8];
	s32					   x, y, z;
	s32					   altitude; // 落ちて止まる高さ
	s32					   scale_x, scale_y;
	u8					   reserved_20[8];
	s32					   velocity_y;
	u8					   reserved_2c[4];
	s32					   speed;
	u8					   reserved_34[0x1c];
	struct Effect05Sprite *sprite;
	u8					   reserved_54;
	u8					   flag;
	u8					   reserved_56[8];
	s16					   phase;
	u8					   reserved_60[0xc];
	void				  *callback; // 毎フレームの処理
};

struct Effect05State
{
	s32					   angle;
	s32					   x, y, z; // 玉の行き先の足もと
	struct Effect05Target *target;
	s32					   initialized;
	u8					   reserved_18[8];
	u8					   high_arc;
	u8					   reserved_21[0x13];
	u8					   variant;
	u8					   reserved_35[0x10];
	u8					   alternate;
};

//--------------------------------------------------------------------------
// 毎フレームの処理 : 落ちる粒が altitude に着いたら、小さな粒を 3 つ散らす
//--------------------------------------------------------------------------
void BattleFx_SpawnRandomAngleTriplet(struct Effect05Object *obj)
{
	s32					   i;
	struct Effect05Object *p;
	s32					   phase = 2;
	s32					   phase2;
	s16					  *pp;

	if (obj->y <= obj->altitude)
	{
		obj->phase = phase;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
		p = NULL;
		obj->callback = p;

		// 半分の大きさで、上へ 1 ドットの速さで跳ね、32 ドット先のでたらめな方向へ
		for (i = 0; i <= 2; i++)
		{
			p = Object_Spawn(0xf0, obj->x, obj->y, obj->z);
			if (p == NULL)
			{
				break;
			}
			p->scale_y = 0x8000;
			p->scale_x = 0x8000;
			p->flag = 2;
			p->velocity_y = 1 << 16;
			p->speed = Random16() + 0x13333;
			Motion_SetTargetPositionFromMagnitudeAngle(
				(struct ObjectRuntime *)p, 32 << 16, Random16());
			pp = &p->phase;
			phase2 = 6;
			*pp = phase2;
			ObjectDispatch_InitializeFar((struct DispatchObject *)p, BattleFx_CommonParticleScript);
		}
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : 左右にゆれながらゆっくり落ち、地面に着いたら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateDriftingFallObject(struct ObjectRuntime *obj)
{
	s32 r;

	obj->y -= 0x4ccc;
	r = Random16();
	obj->x += r - Random16();
	if (obj->y <= obj->terrain_height)
	{
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}
}

void BattleFx_CallEffect05(void)
{
	RunBattleEffect05();
}

//--------------------------------------------------------------------------
// 0〜10 段で start から end へ
//--------------------------------------------------------------------------
static __inline__ s32 Lerp10(s32 start, s32 end, s32 step)
{
	return start + step * (end - start) / 10;
}

//--------------------------------------------------------------------------
// 光の玉を対象の 16 ドット上から行き先の 32 ドット上 (variant なら 80 ドット上) へ
// 10 フレームで運びながら大きくし、粒を降らせてから戻す
// 行き先は、まだ決まっていなければ対象から angle の向きへ 16 ドット
//--------------------------------------------------------------------------
void RunBattleEffect05(void)
{
	void						 Animation_ApplyChildValuesFar(void *, s32);
	void						 ObjectDispatch_ReleaseFar(void *);
	void						 Vector_AddPolarOffset(s32, s32, void *);
	struct Effect05Point   pos;
	struct Effect05Point   start;
	struct Effect05Point   end;
	struct Effect05State  *work = gEffectWork;
	struct Effect05Target *target = work->target;
	struct Effect05Object *orb;
	struct Effect05Point  *from;
	struct Effect05Point  *to;
	s32					   i;
	s32					   steps = 11;
	s32					   count;
	s32					   y;
	struct Effect05Object *mote;

	orb = Object_Spawn(0xef, i = 0, 0, 0);
	if (orb == 0)
		return;
	BattleEffect_InitializeSharedScene();
	AudioCommand_PlayFar(138);
	if (work->initialized == 0)
	{
		work->x = target->position.x;
		work->z = target->position.z;
		Vector_AddPolarOffset(16 << 16, work->angle, (struct Effect05Point *)&work->x);
		work->y = Map_GetTerrainHeightFar(0, work->x, work->z);
	}
	from = &start;
	from->x = target->position.x;
	from->y = target->position.y + (16 << 16);
	from->z = target->position.z;
	to = &end;
	to->x = work->x;
	y = work->y;
	to->y = y + (32 << 16);
	to->z = work->z;
	if ((s8)work->variant != 0)
		to->y = y + (80 << 16);

	// 運びながら 1/4 から等倍まで大きくする
	for (; i < steps; i++)
	{
		s32 scale;

		{
			struct Effect05Point *a = &start;
			struct Effect05Point *b = &end;
			s32					  e, t;

			e = b->x;
			t = a->x;
			t += i * (e - t) / 10;
			orb->x = t;
			e = b->y;
			t = a->y;
			t += i * (e - t) / 10;
			orb->y = t;
			e = b->z;
			t = a->z;
			t += i * (e - t) / 10;
			orb->z = t;
		}
		scale = Lerp10(0x4000, 0x10000, i);
		orb->scale_x = scale;
		orb->scale_y = scale;
		WaitFrames(1);
	}
	WaitFrames(10);

	if ((s8)work->alternate == 0)
	{
		// 玉のまわり 3〜8 ドットから粒を 6 フレームおきに落とす (最後は玉の真下)
		count = 10;
		if ((s8)work->high_arc == 0)
			count = 24;
		for (i = 0; i < count; i++)
		{
			pos.x = orb->x;
			pos.y = orb->y;
			pos.z = orb->z;
			Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
			if (i == count - 1)
			{
				WaitFrames(25);
				pos.x = orb->x;
				pos.y = orb->y;
				pos.z = orb->z;
			}
			mote = Object_Spawn(0xf0, pos.x, pos.y, pos.z);
			if (mote != 0)
			{
				mote->altitude = pos.y - (32 << 16);
				mote->callback = (void *)BattleFx_SpawnRandomAngleTriplet;
				mote->flag = 2;
			}
			AudioCommand_PlayFar(132);
			WaitFrames(6);
		}
		WaitFrames(10);
	}
	else
	{
		// 玉のまわり 3〜8 ドットから、ゆれて落ちる粒を 6 フレームおきに出す
		if ((s8)work->high_arc != 0)
			count = 10;
		else
			count = 30;
		for (i = 0; i < count; i++)
		{
			pos.x = orb->x;
			pos.y = orb->y;
			pos.z = orb->z;
			Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
			mote = Object_Spawn(0x11c, pos.x, pos.y, pos.z);
			if (mote != 0)
			{
				mote->callback = (void *)BattleFx_UpdateDriftingFallObject;
				mote->flag = 0;
				mote->sprite->mode = 2;
				ObjectDispatch_ApplyArgumentToChildrenFar((struct ObjectRuntime *)mote, 8);
				Animation_ApplyChildValuesFar(mote, 7);
			}
			WaitFrames(6);
		}
		WaitFrames(70);
	}

	// 等倍から 1/4 まで縮めながら、対象の上へ戻す
	i = 0;
	do
	{
		s32 scale;

		{
			struct Effect05Point *a = &end;
			struct Effect05Point *b = &start;
			s32					  e, t;

			e = b->x;
			t = a->x;
			t += i * (e - t) / 10;
			orb->x = t;
			e = b->y;
			t = a->y;
			t += i * (e - t) / 10;
			orb->y = t;
			e = b->z;
			t = a->z;
			t += i * (e - t) / 10;
			orb->z = t;
		}
		scale = Lerp10(0x10000, 0x4000, i);
		orb->scale_x = scale;
		orb->scale_y = scale;
		WaitFrames(1);
		i++;
	} while (i < steps);
	ObjectDispatch_ReleaseFar(orb);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// particles.c
// バトル効果：持ち上げた物のまわりに落ちる粒
//--------------------------------------------------------------------------

struct FallPos
{
	s32 x;
	s32 y;
	s32 z;
};

struct FallSource
{
	u8	reserved_00[8];
	s32 x; // 16.16 ドット
	s32 y;
	s32 z;
};

struct FallScene
{
	u8				   reserved_00[20];
	struct FallSource *child; // 持ち上げている物
};

struct FallParticle
{
	u8	reserved_00[72];
	s32 gravity; // FieldActor と同じ位置
	u8	reserved_4c[9];
	u8	motion_flags; // FieldActor と同じ位置
	u8	reserved_56[8];
	u16 unknown_5e;
};

LAYOUT_SIZE_GUARD(FallPos_Size, struct FallPos, 0x0c);
LAYOUT_OFFSET_GUARD(FallSource_X, struct FallSource, x, 0x08);
LAYOUT_OFFSET_GUARD(FallScene_Child, struct FallScene, child, 0x14);
LAYOUT_OFFSET_GUARD(FallParticle_Gravity, struct FallParticle, gravity, 0x48);
LAYOUT_OFFSET_GUARD(FallParticle_MotionFlags, struct FallParticle, motion_flags, 0x55);
LAYOUT_OFFSET_GUARD(FallParticle_Unknown5e, struct FallParticle, unknown_5e, 0x5e);

//--------------------------------------------------------------------------
// 毎フレームの処理：持ち上げている物のまわりに、落ちる粒をひとつ出す
// 高さは物から 8〜24 ドット上、横はでたらめな向きに 0〜48 ドット
//--------------------------------------------------------------------------
void BattleFx_SpawnFallingParticles(void)
{
	void				 Vector_AddPolarOffset(s32, s32, void *);
	struct FallSource	*src;
	struct FallPos		 pos;
	struct FallParticle *obj;
	u32					 dist;

	src = ((struct FallScene *)gEffectWork)->child;
	pos.x = src->x;
	pos.y = src->y - Random16() * 16 + (24 << 16);
	pos.z = src->z;

	dist = Random16() * 3;
	Vector_AddPolarOffset(dist * 16, Random16(), &pos);

	obj = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
	if (obj != 0)
	{
		obj->motion_flags = ACTOR_FALLS;
		obj->gravity = 0x1999; // 重力 0.1 ドット
		ObjectDispatch_ApplyArgumentToChildrenFar((struct FieldActor *)obj, 0);
		obj->unknown_5e = 12;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}
}

//--------------------------------------------------------------------------
// lift_drop.c
// 対象を 2 つの持ち手で持ち上げ、方向キーで動かして落とす
//--------------------------------------------------------------------------

#define Random16 ((s32 (*)(void))Random16)

#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(struct FxObject_3 *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

#define BattleFx_SpawnItemBreakMode1 \
	((struct FxObject_3 * (*)(s32, s32, s32, s32)) BattleFx_SpawnItemBreakMode1)
#define Object_SetMoveTargetFar \
	((void (*)(struct FxObject_3 *, s32, s32, s32))Object_SetMoveTargetFar)
#define ScriptObject_CheckOverlapFar \
	((s32 (*)(struct FxObject_3 *, struct FxPosition *))ScriptObject_CheckOverlapFar)
#define BattleFx_SpawnRadialParticleRing \
	((void (*)(struct FxObject_3 *))BattleFx_SpawnRadialParticleRing)

struct FxPosition
{
	s32 x; // 16.16 固定小数
	s32 y;
	s32 z;
};

struct FxObject_3
{
	u8				 unknown_00[8];
	s32				 x; // 座標 (16.16 固定小数、y が高さ)
	s32				 y;
	s32				 z;
	s32				 ground; // 足もとの地面の高さ
	u8				 unknown_18[10];
	u8				 mode;
	u8				 unknown_23;
	s32				 velocity_x;
	s32				 velocity_y;
	s32				 velocity_z;
	s32				 speed;
	s32				 lift;
	u8				 unknown_38[12];
	s32				 gravity;
	u8				 unknown_48[13];
	u8				 flag;
	u8				 unknown_56[4];
	u8				 unknown_5a;
	u8				 unknown_5b;
	u8				 unknown_5c[12];
	struct FxObject_3 *linked;
	void			*callback;
};

void			 WaitFrames(s32 frames);

#define SHARD_MAX 20 // 破片の数

//--------------------------------------------------------------------------
// 足もとを 16 ドット上げて、pos へ動いたときにぶつかるかを調べる
//--------------------------------------------------------------------------
static __inline__ s32 ObjProbeUp(struct FxObject_3 *obj, struct FxPosition *pos)
{
	s32				 Map_CheckObjectStepFar(struct FxObject_3 *object, struct FxPosition *position);
	s32 hit;

	obj->ground += 16 << 16;
	hit = Map_CheckObjectStepFar(obj, pos);
	obj->ground -= 16 << 16;
	return hit;
}

//--------------------------------------------------------------------------
// 対象を左右の持ち手で 24 ドットの高さまで持ち上げ、方向キーの向きへ
// 16 ドット動かして落とす (A・B・L・R ならその場で落とす)
// 行き先がふさがっていれば、ブザーを鳴らして待つ
//--------------------------------------------------------------------------
void BattleFx_SteerLiftedTarget(s32 id)
{
	void			 Animation_ApplyChildValuesFar(void *object, s32 value);
	void			 ObjectDispatch_ReleaseFar(void *object);
	void			 Script_WaitForEventTimeoutFar(void *object);
	void			 Vector_AddPolarOffset(s32 magnitude, s32 angle, void *position);
	struct BattleFxScene *work;
	struct FxObject_3		 *src;
	struct FxObject_3		 *target;
	struct FxObject_3		 *shards[SHARD_MAX];
	struct FxPosition	  pos;
	struct FxObject_3		 *hands[2];
	struct FxObject_3		 *obj;
	struct FxObject_3		 *right;
	s32					  i;
	s32					  angle;
	s32					  dist;
	struct FxPosition	 *p;

	work = gEffectWork;
	src = work->main_object;
	target = work->child;
	if (target == NULL)
		return;
	BattleEffect_InitializeSharedScene();
	src->linked = target;
	ObjectDispatch_InitializeFar((struct DispatchObject *)src, BattleFx_SourceHoldScript);

	// 持ち手を左右 32 ドットに出し、対象の左右 16 ドットへ動かす
	/* FAKEMATCH: needed to match: the position is reached through this pointer everywhere but in the steering loop's first part, which names the position itself */
	p = &pos;
	p->x = work->x;
	p->y = work->y + (16 << 16);
	p->z = work->z;
	obj = BattleFx_SpawnItemBreakMode1(p->x + (32 << 16), p->y, p->z, 0x8000);
	hands[0] = obj;
	right = BattleFx_SpawnItemBreakMode1(p->x - (32 << 16), p->y, p->z, 0);
	hands[1] = right;
	if (hands[0] == NULL || right == NULL)
	{
		BattleFx_PrepareBufferInterpolation();
		return;
	}
	WaitFrames(15);
	p->x = target->x;
	p->y = target->y + (16 << 16);
	p->z = target->z;
	Object_SetMoveTargetFar(obj, p->x + (16 << 16), p->y, p->z);
	Object_SetMoveTargetFar(right, p->x - (16 << 16), p->y, p->z);
	Script_WaitForEventTimeoutFar(obj);
	Script_WaitForEventTimeoutFar(right);
	obj->x = p->x + (16 << 16);
	obj->velocity_x = 0;
	right->x = p->x - (16 << 16);
	right->velocity_x = 0;

	// 地面から 24 ドットの高さまで持ち上げる
	target->callback = ObjectGroup_ApplyRandomChildValues;
	Scheduler_AddOrUpdateCallback(BattleFx_SpawnFallingParticles, 0xc80);
	AudioCommand_PlayFar(130);
	target->flag = 4;
	ObjectDispatch_SetSingleChildFlagsFar(target, 0);
	if (hands[0] != NULL && hands[1] != NULL)
	{
		while (target->y - target->ground <= 24 << 16)
		{
			obj->y += 0x6000;
			right->y += 0x6000;
			target->y += 0x6000;
			WaitFrames(1);
		}
	}
	hands[0]->speed = 4 << 16;
	hands[0]->lift = 0x8000;
	hands[1]->speed = 4 << 16;
	hands[1]->lift = 0x8000;
	target->speed = 0x6666;
	target->lift = 0x3333;
	target->unknown_5a = 0;
	target->mode = 2;

	//------------------------------------------------------------------
	// 方向キーで動かす
	//------------------------------------------------------------------
	for (;;)
	{
		WaitFrames(1);
		if ((gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
			break;

		// 方向キーなし : 持ち手を対象の左右に戻す
		angle = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
		if (angle == 0xffff)
		{
			pos.x = target->x;
			pos.y = target->y + (16 << 16);
			pos.z = target->z;
			Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
			Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 1);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 1);
			continue;
		}

		// 持ち手をその向きへ 2 ドット寄せ、16 ドット先の地面を調べる
		pos.x = target->x;
		pos.y = target->y + (16 << 16);
		pos.z = target->z;
		Vector_AddPolarOffset(2 << 16, angle, &pos);
		Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
		Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
		Script_WaitForEventTimeoutFar(hands[0]);
		Script_WaitForEventTimeoutFar(hands[1]);
		pos.x = target->x;
		pos.y = target->ground;
		pos.z = target->z;
		Vector_AddPolarOffset(16 << 16, angle, &pos);
		i = ScriptObject_CheckOverlapFar(target, &pos);
		/* FAKEMATCH: needed to match: entering the blocked path from both tests keeps it ahead of the drop path, as the ROM lays them out */
		if (i != 0)
			goto blocked;
		if (ObjProbeUp(target, p) > 0)
		{
		blocked: // ふさがっている : 16 フレームごとにブザー
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		// 空いている : 対象をそこへ動かす
		{
			struct FxObject_3 *second;
			s32				 x;
			s32				 z;

			AudioCommand_PlayFar(175);
			x = p->x;
			z = p->z;
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
			WaitFrames(15);
			target->unknown_5b = i;
			target->speed = 0x3333;
			target->lift = 0x3333;
			Object_SetMoveTargetFar(target, p->x, p->y, p->z);
			hands[0]->speed = 0x3333;
			hands[0]->lift = 0x3333;
			second = hands[1];
			second->speed = 0x3333;
			second->lift = 0x3333;
			Object_SetMoveTargetFar(hands[0], p->x + (16 << 16), p->y, p->z);
			Object_SetMoveTargetFar(second, p->x - (16 << 16), p->y, p->z);
			Script_WaitForEventTimeoutFar(target);
			target->x = x;
			target->z = z;
			target->velocity_x = i;
			target->velocity_z = i;
			WaitFrames(10);
			break;
		}
	}

	//------------------------------------------------------------------
	// 対象の位置から破片を飛ばして持ち手を消し、対象を跳ねさせて落とす
	//------------------------------------------------------------------
	ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
	ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
	Scheduler_RemoveCallback(BattleFx_SpawnFallingParticles);
	AudioCommand_PlayFar(135);
	WaitFrames(15);
	AudioCommand_PlayFar(135);
	WaitFrames(15);
	p->x = target->x;
	p->y = target->y + (16 << 16);
	p->z = target->z;
	{
		struct FxObject_3 **walk = shards;

		// 破片 : 速さ 2〜3 ドット、8〜32 ドット先のでたらめな方向へ
		for (i = 0; i < SHARD_MAX; i++)
		{
			obj = Object_Spawn(0x11d, p->x, p->y, p->z);
			*walk++ = obj;
			if (obj != NULL)
			{
				ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_FragmentScript);
				obj->speed = Random16() + (2 << 16);
				obj->lift = 2 << 16;
				obj->flag = 0;
				dist = Random16() * 24 + (8 << 16);
				Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
			}
		}
	}
	AudioCommand_PlayFar(131);
	ObjectDispatch_ReleaseFar(hands[0]);
	ObjectDispatch_ReleaseFar(hands[1]);

	// 対象の見た目と動きを戻し、上へ 10 ドットの速さで跳ねさせる
	Animation_ApplyChildValuesFar(target, work->saved_palette);
	ObjectDispatch_InitializeFar((struct DispatchObject *)target, work->saved_script);
	target->callback = work->saved_callback;
	target->flag = 3;
	target->velocity_y = 10 << 16;
	target->gravity = 0x3333;
	target->mode = 0;
	src->callback = 0;
	Animation_ApplyChildValuesFar(src, 0);
	if (work->child_option != 0)
	{
		// 上がりきって落ちきるまで (それぞれ最長 90 フレーム) 待ち、輪を出す
		for (i = 0; i < 90 && target->velocity_y >= 0; i++)
			WaitFrames(1);
		WaitFrames(1);
		for (i = 0; i < 90 && target->velocity_y < 0; i++)
			WaitFrames(1);
		BattleFx_SpawnRadialParticleRing(target);
		BattleFx_PrepareBufferInterpolation();
		WaitFrames(30);
	}
	else
	{
		BattleFx_PrepareBufferInterpolation();
	}
}

#undef BattleFx_SpawnItemBreakMode1
#undef Motion_SetTargetPositionFromMagnitudeAngle
#undef Random16

#undef Motion_SetTargetPositionFromMagnitudeAngle
#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(void *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

void RunBattleEffect11(void)
{
	void						 ObjectDispatch_ReleaseFar(void *object);
	void						 Script_WaitForEventTimeoutFar(void *object);
	struct BattleFxScene  *scene;
	struct MotionObject	  *obj;
	struct MotionObject	  *anchors[2];
	struct ScatterPosition position;
	struct MotionObject	 **walk;
	s32					   index;
	s32					   magnitude;

	scene = gEffectWork;
	BattleEffect_InitializeSharedScene();
	position.x = scene->x;
	position.y = scene->y + 0x100000;
	position.z = scene->z;
	anchors[0] =
		BattleFx_SpawnItemBreakMode1(position.x + 0x200000, position.y, position.z, 0x8000);
	anchors[1] = BattleFx_SpawnItemBreakMode1(position.x - 0x200000, position.y, position.z, 0);

	WaitFrames(15);
	walk = anchors;
	for (index = 0; index < 2; index++)
	{
		obj = *walk++;
		if (obj != 0)
			Motion_SetTargetPositionFromMagnitudeAngle(obj, 0x180000, obj->angle);
	}

	Script_WaitForEventTimeoutFar(anchors[0]);
	AudioCommand_PlayFar(134);
	for (index = 0; index < 24; index++)
	{
		position.x = scene->x;
		position.y = scene->y + 0x100000;
		position.z = scene->z;
		obj = Object_Spawn(0x11d, position.x, position.y, position.z);
		if (obj != 0)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_FragmentScript);
			obj->speed_limit = Random16() + 0x20000;
			obj->acceleration = 0x20000;
			obj->motion_flags = 0;
			magnitude = Random16() * 24 + 0x80000;
			Motion_SetTargetPositionFromMagnitudeAngle(obj, magnitude, Random16());
		}
	}

	ObjectDispatch_ReleaseFar(anchors[0]);
	ObjectDispatch_ReleaseFar(anchors[1]);
	BattleFx_PrepareBufferInterpolation();
}

struct MotionObject *BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle)
{
	struct MotionObject *obj;
	s32					 scale;

	AudioCommand_PlayFar(SOUND_ITEM_BREAK);
	obj = Object_Spawn(215, x, y, z);
	if (obj != NULL)
	{
		obj->scale_y = 0x4000;
		obj->scale_x = 0x4000;
		obj->speed_limit = 0x30000;
		obj->acceleration = 0x30000;
		obj->auto_face_motion = 0;
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
		scale = obj->scale_x;
		if (scale < 0x10000)
		{
			do
			{
				scale += 0x800;
				obj->scale_y = scale;
				obj->scale_x = scale;
				obj->angle += 0x2000;
				WaitFrames(1);
				scale = obj->scale_x;
			} while (scale <= 0xffff);
		}
		obj->angle = angle;
	}
	return obj;
}

void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(union FieldObject *object)
{
	object->effect.x += object->effect.velocity_x;
	object->effect.y += object->effect.velocity_y;
	object->effect.z += object->effect.velocity_z;
	object->effect.scale_x += object->effect.scale_rate_x;
	object->effect.scale_y += object->effect.scale_rate_y;
	object->effect.sprite->rotation += object->effect.spin;
}

//--------------------------------------------------------------------------

// circle.c

// バトル演出：輪に散る粒子と、中心のまわりを回るオブジェクト

//--------------------------------------------------------------------------

#undef BattleFx_MarkChildAndRunFallbackTransition
#undef BattleFx_SpawnRadialParticleRing
#undef Camera_WorldToScreen
#undef ObjectDispatch_ApplyValueToChildrenFar
#undef ObjectGroup_SetChildValueUnlessFifteenFar
#undef Random16

#define Object_SetPositionAndResetMotionFar \
	((void (*)(void *, s32, s32, s32))Object_SetPositionAndResetMotionFar)

extern const s32 BattleFx_CommonParticleScript[];

struct OrbitEffect
{
	u8					unknown_00[6];
	u16					angle; // 進む向き・回る角度
	s32					x;
	s32					y;
	s32					z;
	u8					unknown_14[20];
	s32					scale;
	u8					unknown_2c[28];
	s32					speed;
	u8					unknown_4c[24];
	u16					timer;	// 経過フレーム
	s16					pause;	// 向きを大きく変える残りフレーム
	struct OrbitEffect *anchor; // 回る中心
	void (*update)(struct OrbitEffect *);
};

//--------------------------------------------------------------------------
// 粒子の毎フレーム処理
// 速度で進み、x と z の速度を少しずつ落とし、拡大率と回転を変える
//--------------------------------------------------------------------------
void BattleFx_UpdateParticleMotionAndScale(union FieldObject *object)
{
	struct FieldEffect *efx = &object->effect;

	efx->x += efx->velocity_x;
	efx->y += efx->velocity_y;
	efx->z += efx->velocity_z;
	efx->velocity_x -= efx->velocity_x / 18;
	efx->velocity_z -= efx->velocity_z / 16;
	efx->scale_x += efx->scale_rate_x;
	efx->scale_y += efx->scale_rate_y;
	efx->sprite->rotation += efx->spin;
}

//--------------------------------------------------------------------------
// origin の位置から、水平に輪を描いて広がる粒子を 17 個出す
// x 方向の速度は z 方向の 1.5 倍
//--------------------------------------------------------------------------
void BattleFx_SpawnRadialParticleRing(struct MotionObject *origin)
{
	struct EffectOptions opt;
	s32					 vel[3];
	s32					 vz;
	s32					 angle;
	u32					 i;

	opt.palette = 0;
	opt.update = BattleFx_UpdateParticleMotionAndScale;
	opt.start_scale_x = 0xcccc; // 0.8 倍
	opt.start_scale_y = 0xcccc;
	i = 0;
	do
	{
		angle = i << 12; // 1/16 周ずつ
		vel[0] = Trig_Cos(angle) * 3 / 2;
		vel[1] = 0;
		vz = Trig_Sin(angle);
		vel[2] = vz;
		Effect_SpawnResident(origin->x, origin->y, origin->z, vel[0], vel[1], vz, 0x01090001, &opt);
		i++;
	} while (i <= 16);
}

//--------------------------------------------------------------------------
// 中心を離れてさまよう（毎フレーム）
// 1 フレームに 2〜3 ドット進み、ときどき向きを大きく変える
// 101 フレームで共通の粒子スクリプトへ移る
//--------------------------------------------------------------------------
void BattleFx_WanderAroundAnchor(struct OrbitEffect *efx)
{
	s32 len;
	s32 angle;
	s32 dx;
	s32 dz;

	len = Random16() + 0x20000;
	angle = efx->angle;
	dx = Iwram_MulQ16(len, Trig_Cos(angle));
	dz = Iwram_MulQ16(len, Trig_Sin(angle));
	efx->x += dx;
	efx->z += dz;
	efx->angle += 0xfff0;
	if (efx->pause != 0)
	{
		efx->pause--;
		efx->angle += 0x800;
	}
	else if ((Random16() << 5) >> 16 == 0) // 1/32 の確率
	{
		efx->pause = ((Random16() << 4) >> 16) + 8; // 8〜23 フレーム
	}
	if (++efx->timer == 101)
		ObjectDispatch_InitializeFar((struct DispatchObject *)efx, BattleFx_CommonParticleScript);
}

//--------------------------------------------------------------------------
// 中心のまわりを半径 8 ドットで回る（毎フレーム）
// 121 フレーム回ったら、さまよう動きに切り替える
//--------------------------------------------------------------------------
void BattleFx_CircleAnchor(struct OrbitEffect *efx)
{
	struct OrbitEffect *anchor;
	s32					angle;
	s32					r;
	s32					dx;
	s32					dz;

	anchor = efx->anchor;
	r = 0x80000;
	angle = efx->angle;
	dx = Iwram_MulQ16(r, Trig_Cos(angle));
	dz = Iwram_MulQ16(r, Trig_Sin(angle));
	efx->x = anchor->x + dx;
	efx->z = anchor->z + dz;
	efx->angle += 0x800;
	if (++efx->timer == 121)
	{
		efx->update = BattleFx_WanderAroundAnchor;
		efx->timer = 0;
		efx->pause = 0;
		efx->speed = 0x1999;
		efx->scale = 0x30000;
		efx->angle = Random16();
	}
}

//--------------------------------------------------------------------------
// 毎フレーム縮め、0.5 倍を切ったら位置を戻して処理を外す
//--------------------------------------------------------------------------
void BattleFx_ShrinkObjectScaleUntilHalf(struct MotionObject *obj)
{
	s32 scale;

	scale = obj->scale_x - 0x80;
	obj->scale_y = scale;
	obj->scale_x = scale;
	if (scale < 0x8000)
	{
		Object_SetPositionAndResetMotionFar(obj, 0, 0, 0);
		((struct ScriptMotionObject *)obj)->hook = 0;
	}
}

void BattleFx_CallEffect14(void)
{
	RunBattleEffect14();
}

//--------------------------------------------------------------------------
// radial.c
// フィールドのエナジー演出：14 番（運ばれる玉と、まわりを回る粒）ほか
//--------------------------------------------------------------------------

#define BattleFx_ShrinkObjectScaleUntilHalf ((void (*)(void))BattleFx_ShrinkObjectScaleUntilHalf)
#define BattleFx_CircleAnchor ((void (*)(void))BattleFx_CircleAnchor)

struct EffectParticle
{
	u8					   reserved_00[6];
	s16					   angle; // 0x10000 で 1 周
	s32					   x;	  // 16.16 ドット
	s32					   y;
	s32					   z;
	u8					   reserved_14[4];
	s32					   scale_x; // 0x10000 で等倍
	s32					   scale_y;
	u8					   reserved_20[0x64 - 0x20];
	s16					   timer;  // BattleFx_CircleAnchor が数える
	s16					   pause;  // 同じく
	struct EffectParticle *anchor; // まわる中心
	void (*callback)(void);
};

//--------------------------------------------------------------------------
// エナジー 14 番 : 玉を使う者から scene の位置（4 ドット上）まで 10 フレームで運び、
// 0.25 倍から大きくする。scene->enabled がなければ、かけられた物を縮め、
// 玉のまわりに 6 フレームおきに粒を 16 個出して回らせる
//--------------------------------------------------------------------------
void RunBattleEffect14(void)
{
	void				   ObjectDispatch_ApplyArgumentToChildrenFar(void *, s32);
	void				   ObjectDispatch_ReleaseFar(void *);
	void				  *Object_Spawn(s32, s32, s32, s32);
	struct BattleFxScene  *scene = gEffectWork;
	struct EffectParticle *src = scene->main_object;
	struct EffectParticle *tgt = scene->child;
	struct EffectVector	   pos;
	struct EffectVector	   from;
	struct EffectVector	   to;
	struct EffectParticle *tama;
	s32					   step;
	struct EffectParticle *particle;
	s32					   steps = 11;

	step = 0;
	from.x = src->x;
	from.y = src->y;
	from.z = src->z;
	to.x = scene->x;
	to.y = scene->y - (4 << 16);
	to.z = scene->z;
	tama = Object_Spawn(0xda, 0, 0, 0);
	if (tama == 0)
	{
		return;
	}
	BattleEffect_InitializeSharedScene();
	ObjectDispatch_ApplyArgumentToChildrenFar(tama, 2);
	do
	{
		s32 scale;
		tama->x = from.x + step * (to.x - from.x) / 10;
		tama->y = from.y + step * (to.y - from.y) / 10;
		tama->z = from.z + step * (to.z - from.z) / 10;
		scale = 0x4000 + step * 0x10ccc / 10; // 0.25 倍から 1.3 倍へ
		tama->scale_x = scale;
		tama->scale_y = scale;
		WaitFrames(1);
		step++;
	} while (step < steps);
	tama->scale_x = 0x1b333; // 横 1.7 倍
	tama->scale_y = 0x14ccc; // 縦 1.3 倍
	AudioCommand_PlayFar(163);
	WaitFrames(20);
	if (scene->enabled == 0)
	{
		if (tgt != 0)
			tgt->callback = BattleFx_ShrinkObjectScaleUntilHalf;

		// 粒は玉の 4 ドット上から 0.8 ドットずつ高く、3〜8 ドット離して出す
		step = 0;
		do
		{
			s32 dist;

			pos.x = tama->x;
			pos.y = tama->y + step * 0xcccc + (4 << 16);
			pos.z = tama->z;
			dist = Random16() * 5 + (3 << 16);
			Vector_AddPolarOffset(dist, Random16(), &pos);
			particle = Object_Spawn(0xf9, pos.x, pos.y, pos.z);
			if (particle != 0)
			{
				particle->callback = BattleFx_CircleAnchor;
				particle->anchor = tama;
				particle->timer = 0;
				particle->pause = 0;
				particle->angle = Random16();
			}
			WaitFrames(6);
			step++;
		} while (step <= 15);
		WaitFrames(20);
		WaitFrames(120);
	}
	ObjectDispatch_ApplyArgumentToChildrenFar(tama, 1);
	WaitFrames(30);
	AudioCommand_PlayFar(136);
	WaitFrames(20);
	ObjectDispatch_ReleaseFar(tama);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// スロットの粒の動き（毎フレーム）
// state 0 : 出た所から 40〜70 ドット離れた所へ向かう
// state 1 : 着くまで待ち、着いたら state 2 へ
// state 2 : scene の位置の 8 ドット上（画面の位置）のまわり 4 ドットへ向かう
// state 3 : 着いたらスロットを空ける
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialCamera(struct EffectSlot *effect)
{
	struct BattleFxScene *scene;
	struct EffectVector	  pos;
	s8					 *st;
	s16					  angle;
	s32					  state;

	scene = gEffectWork;
	st = &effect->state;
top:
	state = *st;
	if (state == 0)
	{
		pos.x = effect->origin_x;
		pos.z = effect->origin_z;
		angle = Random16();
		Vector_AddPolarOffset(Random16() * 30 + (40 << 16), (u16)angle, &pos);
		effect->target_x = pos.x;
		effect->target_z = pos.z;
		effect->acceleration = 4 << 16;
		effect->max_speed = 4 << 16;
		effect->flag42 = state;
		goto advance;
	}
	if (state == 1)
	{
		if (BattleFx_HasReachedTarget(effect) != 0)
			return;
		*st = *st + 1;
		goto top;
	}
	else if (state == 2)
	{
		pos.x = scene->x;
		pos.y = scene->y + (8 << 16);
		pos.z = scene->z;
		Camera_WorldToScreen(&pos);
		Vector_AddPolarOffset(4 << 16, Random16(), &pos);
		effect->target_x = pos.x;
		effect->target_z = pos.z;
		effect->max_turn_step = 0x1000;
		effect->flag42 = 1;
	advance:
		*st = *st + 1;
		return;
	}
	else if (state == 3)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
			BattleFx_ClearOwnedSlot(effect);
		return;
	}
	else
	{
		return;
	}
}

//--------------------------------------------------------------------------
// かけられた物（scene->child）に paused を立て、
// BattleEffect_RunFallbackObjectTransition の演出を動かす
//--------------------------------------------------------------------------
void BattleFx_MarkChildAndRunFallbackTransition(void)
{
	struct FieldActor *tgt = gEffectWork->child;

	tgt->paused = 1;
	BattleEffect_RunFallbackObjectTransition();
}

//--------------------------------------------------------------------------
// fallback.c
// フィールド効果：対象のない場所へ飛ばして散らす
//--------------------------------------------------------------------------

#define Camera_WorldToScreen ((void (*)(s32 *))Camera_WorldToScreen)
#define ObjectGroup_SetChildValueUnlessFifteenFar \
	((void (*)(struct MotionObject *, s32))ObjectGroup_SetChildValueUnlessFifteenFar)

struct FallbackPosition
{
	s32 x;
	s32 y;
	s32 z;
};
u32	 Random16(void);

//--------------------------------------------------------------------------
// 使い手から効果の位置へオブジェクトを 10 フレームで飛ばし、16 個の粒にして散らす
//--------------------------------------------------------------------------
void BattleEffect_RunFallbackObjectTransition(void)
{
	void					ObjectDispatch_ApplyArgumentToChildrenFar(void *, s32);
	void					ObjectDispatch_ReleaseFar(void *);
	void *Object_Spawn(s32, s32, s32, s32);
	struct BattleFxScene   *scene = gEffectWork;
	struct MotionObject	   *user = scene->main_object;
	struct MotionObject	   *obj;
	struct EffectSlot	   *slot;
	struct FallbackPosition pos;
	struct FallbackPosition from;
	struct FallbackPosition to;
	s32						step;
	s32						i;

	scene->y = user->y;
	obj = Object_Spawn(250, 0, 0, 0);
	step = 0;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
	if (obj == 0)
		return;

	// 使い手の 16 ドット上から、効果の位置の 8 ドット上へ
	BattleEffect_InitializeSharedScene();
	from.x = user->x;
	from.y = user->y + (16 << 16);
	from.z = user->z;
	to.x = scene->x;
	to.y = scene->y + (8 << 16);
	to.z = scene->z;
	for (;;)
	{
		s32 steps = 11;
		s32 scale;

		obj->x = from.x + (to.x - from.x) * step / 10;
		obj->y = from.y + (to.y - from.y) * step / 10;
		obj->z = from.z + (to.z - from.z) * step / 10;
		scale = BattleFx_Interpolate(0x10000, 0x4000, step); // 0.25 倍から等倍へ
		obj->scale_x = scale;
		obj->scale_y = scale;
		WaitFrames(1);
		step++;
		if (step >= steps)
			break;
	}

	WaitFrames(5);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
	WaitFrames(10);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
	WaitFrames(10);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
	WaitFrames(10);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_HIGH);

	// その周り 4 ドットに粒を置き、外へ飛ばしてから効果の位置へ戻す
	slot = &scene->slots[0];
	for (i = 0; i < 16; i++)
	{
		pos.x = obj->x;
		pos.y = obj->y + (8 << 16);
		pos.z = obj->z;
		Camera_WorldToScreen(&pos.x);
		Vector_AddPolarOffset(4 << 16, Random16(), &pos.x);
		EffectSlot_Initialize(slot, 285, pos.x, pos.z);
		EffectSlot_SetCallback(slot, BattleFx_UpdateRadialCamera);
		ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 7);
		slot++;
	}

	pos.x = obj->x;
	pos.y = obj->y + (8 << 16);
	pos.z = obj->z;
	WaitFrames(8);
	ObjectDispatch_ReleaseFar(obj);
	WaitFrames(4);
	WaitFrames(30);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// object_pause.c
// オブジェクトを止めて表の値をでたらめに当て続け、もとの処理に戻す
//--------------------------------------------------------------------------

#define ObjectDispatch_ApplyValueToChildrenFar \
	((void (*)(void *, s32))ObjectDispatch_ApplyValueToChildrenFar)

//--------------------------------------------------------------------------
// 止めているあいだの毎フレームの処理 : 表の 8 つからでたらめに選んで当てる
//--------------------------------------------------------------------------
void BattleEffect_SetRandomTableValueOnObject(void *obj)
{
	s8 *table = BattleFx_RandomChildValues;
	s32 r = Random16();
	Animation_ApplyChildValuesFar(obj, table[(u32)(r * 8) >> 16]);
}

//--------------------------------------------------------------------------
// id のオブジェクトを止める
// 毎フレームの処理と、動いていれば最初のアニメーションの param を
// gGameState に覚えておく
//--------------------------------------------------------------------------
void BattleEffect_PauseObject(s32 id)
{
	struct FieldActor	  *obj;
	struct AnimationEntry *entry;

	obj = ObjectTable_Get(id);
	if (obj != NULL)
	{
		gGameState.paused_update = (u32)obj->update;
		gGameState.paused_pose = 0;
		if (obj->active == 1)
		{
			entry = ((struct AnimationObject *)obj->sprite)->entries[0];
			if (entry != NULL)
			{
				gGameState.paused_pose = entry->param;
			}
		}
		obj->update = (void (*)(union FieldObject *))BattleEffect_SetRandomTableValueOnObject;
		obj->paused = 1;
		ObjectDispatch_ApplyValueToChildrenFar(obj, 0);
	}
}

//--------------------------------------------------------------------------
// id のオブジェクトを、覚えておいた毎フレームの処理と param に戻す
//--------------------------------------------------------------------------
void BattleFx_ResumeObject(s32 id)
{
	struct FieldActor *obj = ObjectTable_Get(id);
	if (obj != 0)
	{
		if (obj->update == (void (*)(union FieldObject *))BattleEffect_SetRandomTableValueOnObject)
		{
			struct GameState *state = &gGameState;
			obj->update = (void (*)(union FieldObject *))state->paused_update;
			state->paused_update = 0;
			Animation_ApplyChildValuesFar(obj, state->paused_pose);
		}
		obj->paused = 0;
		ObjectDispatch_ApplyValueToChildrenFar(obj, 16);
	}
}

//--------------------------------------------------------------------------

// filter_object_id.c

// バトル演出：演出の対象オブジェクトのしぼりこみ

//--------------------------------------------------------------------------

s32 BattleFx_GetFlags();

//--------------------------------------------------------------------------
// 今の演出定義の flags が 0xff なら、対象なし (-1) を返す
// id : 対象オブジェクト番号
//--------------------------------------------------------------------------
s32 BattleFx_FilterObjectIdByFlags(s32 id)
{
	if (BattleFx_GetFlags() == 0xff)
		return -1;
	return id;
}

#undef BattleFx_RunEffect13Hook
#undef Camera_WorldToScreen
#undef ObjectGroup_SetChildValueUnlessFifteenFar
#undef Object_SetPositionAndResetMotionFar

//--------------------------------------------------------------------------
// run_effect13_hook.c
// エナジー演出 13：オブジェクト 215 を目標まで伸ばし、押し下げて戻す
//--------------------------------------------------------------------------

#define Object_SetPositionAndResetMotionFar \
	((void (*)(struct MotionObject *, s32, s32, s32))Object_SetPositionAndResetMotionFar)

//--------------------------------------------------------------------------
// 対象への演出 (BattleFx_DispatchRequestKind から)
//--------------------------------------------------------------------------
void BattleFx_RunEffect13Hook(void)
{
	RunBattleEffect13();
}

//--------------------------------------------------------------------------
// オブジェクト 215 を使う者の 16 ドット上から目標へ 10 段で運びながら
// 1/4 倍から等倍にし、20 ドット押し下げて child を (-9, -9) へ移し、
// 19.5 ドット上げてから縮めて戻す
// 目標は scene->enabled なら使う者の前 32 ドット、ほかは scene の位置で、
// どちらも 32 ドット上
//--------------------------------------------------------------------------
void RunBattleEffect13(void)
{
	void Animation_ApplyChildValuesFar(void *, s32);
	void ObjectDispatch_ApplyArgumentToChildrenFar(void *object, s32 animation);
	void Vector_AddPolarOffset(s32, s32, void *);
	struct BattleFxScene	 *scene = gEffectWork;
	struct MotionObject		 *actor = scene->main_object;
	struct MotionObject		 *child = scene->child;
	struct MotionObject		 *object;
	struct FixedPointPosition spawn;
	struct FixedPointPosition origin;
	struct FixedPointPosition target;
	s32						  step;
	s32						  i;
	s32						  steps = 11;

	origin.x = actor->x;
	origin.y = actor->y + (16 << 16);
	origin.z = actor->z;
	if (scene->enabled)
	{
		target.x = actor->x;
		target.y = actor->y + (32 << 16);
		target.z = actor->z;
		Vector_AddPolarOffset(32 << 16, scene->angle, &target.x);
	}
	else
	{
		target.x = scene->x;
		target.y = scene->y + (32 << 16);
		target.z = scene->z;
	}
	spawn.x = scene->x;
	spawn.y = scene->y + (32 << 16);
	spawn.z = scene->z;
	object = Object_Spawn(215, spawn.x, spawn.y, spawn.z);
	if (object == 0)
		return;
	BattleEffect_InitializeSharedScene();
	AudioCommand_PlayFar(138);
	object->angle = actor->angle;
	object->speed_limit = 0x14ccc;
	object->motion_flags = 0;
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
	Animation_ApplyChildValuesFar(object, 1);

	// 目標へ伸ばす
	step = 0;
	do
	{
		s32 scale;

		object->x = origin.x + (target.x - origin.x) * step / 10;
		object->y = origin.y + (target.y - origin.y) * step / 10;
		object->z = origin.z + (target.z - origin.z) * step / 10;
		scale = BattleFx_Interpolate(0x10000, 0x4000, step);
		object->scale_x = scale;
		object->scale_y = scale;
		WaitFrames(1);
		step++;
	} while (step < steps);
	WaitFrames(10);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 6);
	WaitFrames(15);

	// 2 ドットずつ 10 フレーム押し下げる
	for (i = 0; i < 10; i++)
	{
		object->y -= 2 << 16;
		WaitFrames(1);
	}
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
	AudioCommand_PlayFar(132);
	if (child != 0)
		Object_SetPositionAndResetMotionFar(child, -(9 << 16), child->y, -(9 << 16));
	WaitFrames(20);

	// 1.5 ドットずつ 13 フレーム上げる
	for (i = 0; i < 13; i++)
	{
		object->y += 0x18000;
		WaitFrames(1);
	}
	WaitFrames(10);
	AudioCommand_PlayFar(114);

	// 使う者のところへ縮めて戻す
	step = 0;
	do
	{
		s32 scale;

		object->x = target.x + (origin.x - target.x) * step / 10;
		object->y = target.y + (origin.y - target.y) * step / 10;
		object->z = target.z + (origin.z - target.z) * step / 10;
		scale = BattleFx_Interpolate(0x4000, 0x10000, step);
		object->scale_x = scale;
		object->scale_y = scale;
		WaitFrames(1);
		step++;
	} while (step < steps);
	ObjectDispatch_ReleaseFar(object);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// launch.c
// 戦闘エフェクト 15 番：回って縮む主オブジェクトと、扇状に飛び出す粒
//--------------------------------------------------------------------------

#define Camera_WorldToScreen ((void (*)(struct Output *))Camera_WorldToScreen)
#define EffectSlot_Initialize ((void (*)(void *, s32, s32, s32))EffectSlot_Initialize)
#define ObjectGroup_SetChildValueUnlessFifteenFar \
	((void (*)(void *, s32))ObjectGroup_SetChildValueUnlessFifteenFar)

#define SLOT_MAX 24 // エフェクト枠の数 (粒の数)

//--------------------------------------------------------------------------
// 毎フレームの更新 : 0x1000 + timer × 0x50 ずつ回し、timer × 0x50 が 0x1000 に
// 達するまで timer を増やして速める
//--------------------------------------------------------------------------
void BattleFx_UpdateSpinAngle(struct FieldActor *obj)
{
	u32 step;

	step = obj->timer * 0x50;
	obj->facing = obj->facing + step + 0x1000;
	if (step < 0x1000)
		obj->timer = obj->timer + 1;
}

//--------------------------------------------------------------------------
// 毎フレームの更新 : 45 度ずつ回りながら縮み、y を 1 ドットずつ増やす
// 拡大率が 0x3000 を下回ったら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateDescendingObject(struct FieldActor *obj)
{
	u32 scale;

	obj->scale_y -= 0x280;
	scale = obj->scale_x - 0x280;
	obj->facing += 0x2000;
	obj->y.fixed += 1 << 16;
	obj->scale_x = scale;
	if ((s32)scale < 0x3000)
		obj->active = 0;
}

struct Output
{
	s32 x; // 16.16 固定小数
	s32 y;
	s32 z;
};
#ifndef MODERN // modern GCC keeps the headers' volatile declaration
#endif

//--------------------------------------------------------------------------
// 粒の動き : 主オブジェクトの画面位置 (z + 8 ドット) から、0xc000 の向きの ±45 度へ
// 120 ドット飛ぶ。状態 1 は着いたら飛びなおし、状態 2 は着いたら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialLaunch(struct EffectSlot *slot)
{
	void			   Vector_AddPolarOffset(s32, s32, void *);
	struct Output	   pos;
	struct FieldActor *src;
	s32				   state;
	u32				   r;

	src = gEffectWork->main_object;
	state = slot->state;

	if (state == 0)
	{
		pos.x = src->x.fixed;
		pos.y = src->y.fixed;
		pos.z = src->z.fixed;
		Camera_WorldToScreen(&pos);

		slot->x = pos.x;
		slot->z = pos.z + (8 << 16);
		slot->origin_z = slot->z;
		slot->origin_x = slot->x;
		pos.z = slot->origin_z;
		pos.x = slot->origin_x;

		r = Random16();
		Vector_AddPolarOffset(
			120 << 16, ((r << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000, &pos);

		slot->target_x = pos.x;
		slot->target_z = pos.z;
		slot->acceleration = 5 << 16;
		slot->max_speed = 5 << 16;
		slot->flag42 = state;
		slot->state++;

		if ((gFrameCount & 2) != 0)
			AudioCommand_PlayFar(246);
	}
	else if (state == 1)
	{
		if (BattleFx_HasReachedTarget(slot) == 0)
			slot->state--;
	}
	else if (state == 2)
	{
		if (BattleFx_HasReachedTarget(slot) == 0)
			BattleFx_ClearOwnedSlot(slot);
	}
}
void ObjectMotion_ArmCallback(s32, s32, s32);

//--------------------------------------------------------------------------
// 戦闘エフェクト 15 番
// 主オブジェクトを回して縮めながら、粒を 1 フレームに 1 つずつ 24 個飛ばす
//--------------------------------------------------------------------------
void BattleFx_RunEffect15(void)
{
	void Battle_Reset(void);
	void				  Animation_ApplyChildValuesFar(void *, s32);
	void				  ObjectDispatch_ApplyArgumentToChildrenFar(void *, s32);
	void				  ObjectDispatch_SetSingleChildFlagsFar(void *, s32);
	struct BattleFxScene *scene = gEffectWork;
	struct FieldActor	 *obj = scene->main_object;
	struct Output		  pos;
	struct EffectSlot	 *slot;
	u8					  last;
	u32					  i;

	Battle_Reset();
	Motion_CamBounds(-1, -1, -1, 0);
	BattleEffect_InitializeSharedScene();
	WaitFrames(10);
	ObjectMotion_ArmCallback(scene->first_object_id, 0x4000, 0);
	WaitFrames(30);
	obj->update = (void (*)(union FieldObject *))ObjectGroup_ApplyRandomChildValues;
	AudioCommand_PlayFar(131);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 28);
	WaitFrames(40);

	// 回りはじめる
	AudioCommand_PlayFar(220);
	Animation_ApplyChildValuesFar(obj, 0);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 3);
	obj->update = (void (*)(union FieldObject *))BattleFx_UpdateSpinAngle;
	obj->timer = 0;
	WaitFrames(70);

	// 縮みはじめ、その画面位置から粒を飛ばす
	ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
	obj->motion_flags = 0;
	obj->update = (void (*)(union FieldObject *))BattleFx_UpdateDescendingObject;
	obj->target_x = ACTOR_NO_TARGET;
	pos.x = obj->x.fixed;
	pos.y = obj->y.fixed;
	pos.z = obj->z.fixed;
	Camera_WorldToScreen(&pos);
	i = 0;
	slot = scene->slots;
	do
	{
		s32 scale;
		EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
		EffectSlot_SetCallback(slot, BattleFx_UpdateRadialLaunch);
		EffectSlot_SetObjectMode(slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, (Random16() * 7) >> 16);
		scale = (Random16() >> 1) + 0x13333; // 1.2〜1.7 倍
		slot->scale_y = scale;
		slot->scale_x = scale;
		i++;
		WaitFrames(1);
		slot++;
	} while (i < SLOT_MAX);
	WaitFrames(70);

	// 飛んでいる粒を、着いたら消える状態にする
	i = 0;
	last = 2;
	do
	{
		if (scene->slots[i].active != 0)
			scene->slots[i].state = last;
		i++;
	} while (i < SLOT_MAX);
	WaitFrames(40);
	BattleFx_PrepareBufferInterpolation();
	WaitFrames(10);
}

struct ArcParticle
{
	u8					unknown_00[8];
	s32					x;
	s32					y;
	s32					z;
	s32					floor;
	s32					scale_x;
	s32					scale_y;
	u8					unknown_20[0x48];
	struct ArcParticle *source;
};

void BattleFx_UpdateDescendingParticlePositiveArc(struct ArcParticle *particle)
{
	struct ArcParticle *source;
	s32					y;
	s32					limit;
	s32					scale;

	limit = particle->floor + 0xA0000;
	source = particle->source;
	if (gGameState.map == (s32)&SceneId_Clear)
		limit = particle->floor + 0x40000;

	y = particle->y;
	if (y <= limit)
	{
		ObjectDispatch_ReleaseFar(particle);
		return;
	}

	scale = particle->scale_x + 0xC00;
	if (scale > 0x10000)
		scale = 0x10000;
	particle->scale_x = scale;
	particle->scale_y = scale;
	particle->x = source->x;
	particle->y = y - 0x20000;
	particle->z = source->z + ((0x10000 - scale) * 5) + 0x90000;
}

void BattleFx_UpdateDescendingParticleNegativeArc(struct ArcParticle *particle)
{
	struct ArcParticle *source;
	s32					y;
	s32					limit;
	s32					scale;

	limit = particle->floor + 0xA0000;
	source = particle->source;
	if (gGameState.map == (s32)&SceneId_Clear)
		limit = particle->floor + 0x40000;

	y = particle->y;
	if (y <= limit)
	{
		ObjectDispatch_ReleaseFar(particle);
		return;
	}

	scale = particle->scale_x + 0xC00;
	if (scale > 0x10000)
		scale = 0x10000;
	particle->scale_x = scale;
	particle->scale_y = -scale;
	particle->x = source->x;
	particle->y = y - 0x20000;
	particle->z = source->z - ((0x10000 - scale) * 5) + 0x100000;
}

struct ArcParticle;

struct ArcFrame
{
	u8 padding00[22];
	u8 visible;
};

struct ArcSprite_2
{
	u8 padding00[5];
	u8 flags05_0 : 5;
	u8 flags05_5 : 1;
	u8 layer : 2;
	u8 padding06;
	u8 flags07_0 : 6;
	u8 size : 2;
	union
	{
		struct
		{
			u16 tile : 10;
			u16 flags08_10 : 6;
		} half;
		struct
		{
			u8 tile_low;
			u8 flags09_0 : 2;
			u8 variant : 2;
			u8 flags09_4 : 4;
		} byte;
	} attr;
	u8				 padding0c[16];
	u8				 resource;
	u8				 active : 1;
	u8				 flags1d_1 : 7;
	u8				 padding1e[8];
	u8				 animation;
	u8				 padding27;
	struct ArcFrame *frame;
};

struct ArcObject_2
{
	u8				  padding00[8];
	s32				  x;
	s32				  y;
	s32				  z;
	s32				  unknown_14;
	s32				  scale_x;
	s32				  scale_y;
	u8				  padding20[0x30];
	struct ArcSprite_2 *sprite;
	u8				  padding54;
	u8				  unknown_55;
	u8				  padding56[0x0e];
	u16				  unknown_64;
	u8				  padding66[2];
	struct ArcObject_2 *source;
	void			 *callback;
};

void BattleFx_SpawnDescendingArcParticles(struct ArcObject_2 *source)
{
	struct BattleFxScene *scene = gEffectWork;
	struct ArcObject_2	 *leader = scene->main_object;
	struct ArcObject_2	 *objects[2];
	struct ArcObject_2	 *object;
	struct ArcSprite_2	 *sprite;
	s32					  i;
	u8					  zero;

	for (i = 0; i <= 1; i++)
	{
		object = Object_CreateFar(26, source->x, source->y + 0x400000, source->z);
		objects[i] = object;
		if (object == NULL)
			continue;
		object->unknown_14 = source->unknown_14;
		sprite = object->sprite;
		object->unknown_55 = 0;
		object->unknown_64 = 0;
		zero = 0;
		object->source = source;
		object->scale_y = 0x6666;
		object->scale_x = 0x6666;
		if (sprite == NULL)
			continue;
		AnimationObjects_SelectAnimationFar(sprite, 0);
		sprite->animation = zero;
		Resource_ResetEntry(sprite->resource);
		sprite->resource = scene->arc_tile_slot;
		sprite->active = 1;
		sprite->attr.half.tile = gVramBlockCache[sprite->resource].offset >> 5;
		sprite->flags05_5 = 0;
		sprite->layer = 1;
		sprite->size = 2;
		sprite->frame->visible = zero;
	}
	objects[0]->callback = (void *)BattleFx_UpdateDescendingParticleNegativeArc;
	objects[0]->sprite->attr.byte.variant = 0;
	objects[1]->callback = (void *)BattleFx_UpdateDescendingParticlePositiveArc;
	objects[1]->sprite->attr.byte.variant = leader->sprite->attr.byte.variant;
}

//--------------------------------------------------------------------------
// arcstate.c
// エナジー演出 16 (プルーフ)：使う者の動きと、効果が切れたときのメッセージ
//--------------------------------------------------------------------------

void UiText_ShowPositionedMessageAndWaitFar(s32 no, s32 flags);

#define BattleFx_SpawnDescendingArcParticles \
	((void (*)(void *))BattleFx_SpawnDescendingArcParticles)
#define UiText_ShowPositionedMessageAndWaitFar \
	((void (*)(void *, s32))UiText_ShowPositionedMessageAndWaitFar)

#define ENA_PURUFU 150 // プルーフ
#define ITEM_HANE 236  // せいなるはね

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
#endif

void UiWork_PushValueSlotFar(s32 arg0, s32 arg1);

//--------------------------------------------------------------------------
// 演出の終わり：使う者の表示の flags を 5 フレームの周期で 3 と 1 に切り替える
//--------------------------------------------------------------------------
void BattleFx_UpdateEffect16State(void)
{
	struct ArcEffectObject *obj;
	struct AnimationObject *rec;
	struct AnimationEntry  *entry;
	s32						phase;

	obj = ObjectTable_Get(gGameState.selected_actor);
	rec = obj->records;
	entry = rec->entries[0];
	phase = gFrameCount % 5;
	if (phase == 0)
	{
		rec->dirty = 1;
		rec->flags = 3;
	}
	else if (phase == 2)
	{
		entry->param = 0; // 色を戻す
		rec->dirty = 1;
		rec->flags = 1;
	}
}

//--------------------------------------------------------------------------
// 演出中の使う者：5 フレームごと (ワールドマップでは 7) に降る弧の粒を出し、
// phase 1 では回る
//--------------------------------------------------------------------------
void BattleFx_UpdatePairedArcSpawner(struct ArcEffectObject *obj)
{
	s16 phase;
	s16 cnt;

	phase = obj->phase;
	cnt = obj->step++;

	if (gGameState.map == (s32)&SceneId_Clear)
	{
		if (__modsi3(cnt, 7) == 0)
			BattleFx_SpawnDescendingArcParticles(obj);
	}
	else if (__modsi3(cnt, 5) == 0)
	{
		BattleFx_SpawnDescendingArcParticles(obj);
	}

	if (phase == 1)
		obj->angle += 0xc00;
}

//--------------------------------------------------------------------------
// プルーフ (せいなるはね) の効果が切れた
//--------------------------------------------------------------------------
void FieldEvent_ShowStatusMessage(void)
{
	gGameState.avoid_distance = 0;
	if (gGameState.avoid_by_item == 0)
	{
		UiWork_PushValueSlotFar(ENA_PURUFU, 4);
		UiText_ShowPositionedMessageAndWaitFar(MsgAbilityWoreOff, 1);
		return;
	}
	UiWork_PushValueSlotFar(ITEM_HANE, 2);
	UiText_ShowPositionedMessageAndWaitFar(MsgItemWoreOff, 1);
}

#undef EffectSlot_Initialize
#undef UiText_ShowPositionedMessageAndWaitFar

#define BattleFx_UpdatePairedArcSpawner ((void (*)(void))BattleFx_UpdatePairedArcSpawner)

void Object_ApplyProjectedPlacementFar(
	struct AnimationObject *sprite, s32 *position, s32 *scale, s32 mode);
s32 ArcTan2(s32 x, s32 y);

void RunBattleEffect16(void)
{
	void					Animation_ApplyChildValuesFar(void *, s32);
	void					ObjectDispatch_SetSingleChildFlagsFar(void *, s32);
	struct BattleFxScene   *scene;
	struct ArcEffectObject *object;
	struct AnimationObject *group;
	struct AnimationEntry  *entry;
	u32						angle;
	s32						slot;
	s32						count;

	scene = gEffectWork;
	object = scene->main_object;
	group = object->records;
	entry = group->entries[0];
	angle = object->angle;
	slot = Resource_FindFreeEntry();
	{
		s16 *tile_slot = &scene->arc_tile_slot;
		s32	 zero = 0;

		*tile_slot = slot;
		VramBlock_LoadCached((s16)slot, 0x100, BattleFx_ArcSparkTiles);
		gGameState.avoid_distance = 0x09600000;
		gGameState.avoid_by_item = GameFlag_TestFar(0x145);
		Animation_ApplyChildValuesFar(object, zero);
		object->callback = BattleFx_UpdatePairedArcSpawner;
		object->phase = zero;
		object->step = zero;
	}
	AudioCommand_PlayFar(0x8c);
	WaitFrames(15);
	object->phase = 1;
	WaitFrames(10);
	for (count = 0; count < 20; count++)
	{
		entry->param = 7;
		group->dirty = 1;
		WaitFrames(2);
		group->dirty = 1;
		entry->param = 0;
		group->flags = 1;
		WaitFrames(3);
	}
	object->callback = NULL;
	object->angle = angle;
	Scheduler_AddOrUpdateCallback(BattleFx_UpdateEffect16State, 0xc80);
	WaitFrames(15);
	AudioCommand_PlayFar(0xae);
	WaitFrames(55);
	Scheduler_RemoveCallback(BattleFx_UpdateEffect16State);
	if (gGameState.cloaked != 0)
		ObjectDispatch_SetSingleChildFlagsFar(object, 2);
	else
		ObjectDispatch_SetSingleChildFlagsFar(object, 1);
	Animation_ApplyChildValuesFar(object, 0);
	Resource_ResetEntry(scene->arc_tile_slot);
	UiText_ShowPositionedMessageAndWaitFar((s32)MsgMonstersAttackLess, 1);
}

void EffectSlot_Update(struct EffectSlot *effect)
{
	if (effect->active != 0)
	{
		effect->age++;
		if (effect->callback_delay != 0)
			effect->callback_delay--;
		else if (effect->callback != 0)
			effect->callback(effect);
		if (effect->active != 0)
		{
			if (effect->update_motion != 0)
				EffectSlot_UpdateMotion(effect);
			if (effect->render != 0)
				BattleFx_DrawScaledObject(effect);
		}
	}
}

void BattleFx_DrawScaledObject(struct EffectSlot *object)
{
	struct AnimationObject *sprite;
	s32						offset;
	s32						scale[2];
	s32						position[4];

	sprite = object->object;
	offset = 0;
	if (object->flags & 4)
		offset = 0x1fc0000 - object->z;
	scale[0] = Iwram_MulQ16(object->scale_x, sprite->scale);
	scale[1] = Iwram_MulQ16(object->scale_y, sprite->scale);
	position[0] = object->x;
	position[1] = offset;
	position[2] = object->z + offset;
	position[3] = 0;
	if (object->x > -0x200000 && object->x < 0x1100000 && object->z > -0x200000
		&& object->z < 0xe00000)
		Object_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}

void EffectSlot_UpdateMotion(struct EffectSlot *effect)
{
	s32 dx;
	s32 dz;
	s32 distance;
	s32 ix;
	s32 iz;
	s32 speed;
	s16 angle;
	s32 heading;
	s32 facing;
	s16 turn;

	if (effect->target_x == EFFECT_NO_TARGET)
		return;
	dx = effect->target_x - effect->x;
	dz = effect->target_z - effect->z;
	if (effect->stop_at_target != 0)
	{
		ix = dx / 0x10000;
		iz = dz / 0x10000;
		distance = Iwram_Sqrt(ix * ix + iz * iz) << 16;
		if (distance < 0x800000)
			distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
		if (distance <= 0x80000)
		{
			EffectSlot_SetPosition(effect, effect->target_x, effect->target_z);
			return;
		}
	}
	angle = ArcTan2(dz, dx);
	if (effect->flag42 != 0)
	{
		heading = effect->heading;
		turn = angle - heading;
		if ((turn >= 0 ? turn : -turn) >= effect->max_turn_step)
		{
			if (turn < 0)
			{
				if (-turn > effect->max_turn_step)
					turn = -effect->max_turn_step;
			}
			else if (turn > effect->max_turn_step)
			{
				turn = effect->max_turn_step;
			}
			angle = turn + heading;
		}
	}
	facing = (u16)angle;
	effect->heading = facing;
	speed = effect->speed + effect->acceleration;
	if (speed > effect->max_speed)
		speed = effect->max_speed;
	angle = facing;
	effect->speed = speed;
	effect->x += Iwram_MulQ16(Trig_Cos(angle), speed);
	effect->z += Iwram_MulQ16(Trig_Sin(angle), speed);
}

u32 BattleFx_HasReachedTarget(struct EffectSlot *effect)
{
	u32 value;

	if (effect->stop_at_target == 0)
	{
		return 0;
	}
	value = effect->target_x ^ 0x80000000;
	return ((0u - value) | value) >> 31;
}

void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z)
{
	effect->target_x = EFFECT_NO_TARGET;
	effect->target_z = EFFECT_NO_TARGET;
	effect->x = x;
	effect->z = z;
	effect->speed = 0;
}

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode)
{
	AnimationObjects_SelectAnimationFar(effect->object, mode);
}

void EffectSlot_SetCallback(struct EffectSlot *effect, EffectCallback callback)
{
	effect->callback = callback;
	effect->callback_delay = 0;
	effect->age = 0;
	effect->state = 0;
}

void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z)
{
	s32			 unit;
	s8			*object;
	volatile u32 zero;

	zero = 0;
	Dma_Set((const void *)&zero, effect, 0x85000000 | (sizeof *effect / 4), REG_DMA3);
	object = ResourceObject_CreateFar(kind);
	effect->object = object;
	if (object != NULL)
		object[9] &= ~0x0c;
	EffectSlot_SetPosition(effect, x, z);
	/* FAKEMATCH: needed to match: 1.0 is held in a local, set before max_speed, so the object reload is scheduled ahead of the three unit stores */
	unit = 0x10000;
	effect->max_speed = 0x20000;
	effect->acceleration = effect->scale_y = effect->scale_x = unit;
	effect->origin_x = x;
	effect->origin_z = z;
	((s8 *)effect->object)[38] = 0;
	effect->stop_at_target = 1;
	effect->flag42 = 1;
	effect->update_motion = 1;
	effect->render = 1;
	effect->active = 1;
	effect->random_value = Random16();
	effect->flags = 4;
	EffectSlot_SetObjectMode(effect, 1);
}

void BattleFx_ClearOwnedSlot(struct EffectSlot *slot)
{
	volatile u32 zero;
	if (slot->object)
		ResourceObject_ReleaseFar((struct ResourceObjectWork *)slot->object);
	zero = 0;
	Dma_Set(&zero, slot, 0x85000000 | (sizeof *slot / 4), REG_DMA3);
}

struct FxWindowEntry
{
	u32 a;
	u32 b;
	u32 attr;
};

struct FxWindowWork
{
	u16					 tiles;
	u16					 window_slot;
	s32					 x;
	s32					 y;
	u8					 padding0c[6];
	u16					 unknown_12;
	u8					 padding14[4];
	s32					 unknown_18;
	s32					 window;
	struct FxWindowEntry entries[67];
};

struct FxObject_4
{
	u8	padding00[8];
	s32 x;
	u8	padding0c[6];
	s16 y;
};

void BattleFx_SetupResourcesAndWindow(void)
{
	u32						  *oam;
	volatile u32			   fill;
	u32						  *pal;
	struct FxWindowWork		  *work;
	struct FxObject_4			  *object;
	s32						   attr;
	u32						   i;
	u32						   tile;
	u32						   attr2;

	pal = (u32 *)Runtime_BumpAllocateAlternatePool(32);
	work = (struct FxWindowWork *)gMapCellBuffer;
	oam = (u32 *)work->entries;
	object = Object_GetById(gGameState.selected_actor);
	work->tiles = Resource_FindFreeEntry();
	fill = 0;
	Dma_Set((const void *)&fill, pal, 0x85000020, REG_DMA3);
	pal[0] = 255;
	pal[1] = 0x1ff;
	pal[2] = 272;
	pal[8] = 68;
	pal[9] = 324;
	pal[10] = 272;
	pal[16] = 119;
	pal[17] = 0x177;
	pal[18] = 272;
	pal[24] = 0xff0;
	pal[25] = 0xffff;
	pal[26] = 0x1ffff;
	pal[27] = 0x11ff0;
	pal[28] = 0x1100;
	attr = VramBlock_LoadCached(work->tiles, 128, pal) | 0x400;
	for (i = 0; i < 66; i++)
	{
		*oam++ = 0;
		*oam++ = 0;
		*oam++ = attr;
	}
	MemFree(pal);
	if (GameFlag_TestFar(284))
	{
		work->x = 240 << 15;
		work->y = 160 << 15;
	}
	else
	{
		work->x = (((object->x - 0x10000000) >> 16) * 240 / 4096) << 16;
		work->y = ((object->y * 160) / 4096) << 16;
	}
	work->window_slot = Resource_LoadFixedBlockBIntoFreeSlotFar();
	tile = gVramBlockCache[work->window_slot].offset;
	work->window = UiWindow_CreateFar(0, 0, 0, 0, 2);
	work->unknown_12 = 0xffff;
	work->unknown_18 = 0x10000;
	*oam++ = 0;
	*oam++ = 0x40000000;
	attr2 = 0x400;
	attr2 |= tile >> 5;
	*oam = attr2;
}

//--------------------------------------------------------------------------
// cleanup_resources_and_window.c
// ワールドマップ：地名の印・カーソル・地名ウインドウ
//--------------------------------------------------------------------------

void RenderOutput_PrepareForRedrawFar(s32 window);
#define ObjectTable_Get ((struct MapObject * (*)(s32)) ObjectTable_Get)

extern void UiWork_FinalizeFar(u32 arg0, u32 arg1);

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
// キーを離すとカーソルが近くの地名へ寄る
#define WORLD_MAP_CURSOR_SNAPS 1
#define WORLD_MAP_NAME_RANGE 64 // 地名を出す距離の 2 乗
#define WORLD_MAP_NAME_LEFT 3	// 地名ウインドウを印から左へずらす量
#else
#define WORLD_MAP_CURSOR_SNAPS 0
#define WORLD_MAP_NAME_RANGE 100
#define WORLD_MAP_NAME_LEFT 1
#endif

#define FLAG_NOT_ON_MAP 284 // いまいる場所が地図にのっていない

// OBJ の属性（2 ワード目からが OAM と同じ並び）
struct MapMarker
{
	struct MapMarker *link;
	u16				  y : 8;
	u16				  affine : 2;
	u16				  blend_mode : 2; // 1 で半透明
	u16				  mosaic : 1;
	u16				  full_color : 1;
	u16				  shape : 2;
	u16				  x : 9;
	u16				  affine_index : 5;
	u16				  size : 2;
	u16				  tile : 10;
	u16				  priority : 2;
	u16				  palette : 4;
	u16				  padding0a;
};

// 16.16 固定小数（整数部がドット）
union MapFixed
{
	s32 value;
	struct
	{
		u16 fraction;
		s16 whole;
	} part;
};

// ワールドマップの作業領域（gMapCellBuffer に置く）
struct WorldMapWork
{
	u16				 vram_block; // 印の OBJ キャラクタの VRAM ブロック
	u16				 padding02;	 // もうひとつの VRAM ブロック（終わりに返す）
	union MapFixed	 x;			 // カーソル（ドット）
	union MapFixed	 z;
	u8				 padding0c[6];
	s16				 shown; // いま地名ウインドウに出している印（-1 はなし）
	u8				 padding14[4];
	s32				 speed; // カーソルの速さ
	s32				 window;
	struct MapMarker markers[67];
};

struct MapObject
{
	u8			   padding00[8];
	s32			   x;
	u8			   padding0c[4];
	union MapFixed z;
};

//--------------------------------------------------------------------------
// ワールドマップを閉じる：VRAM ブロックを返し、地名ウインドウを消す
//--------------------------------------------------------------------------
void BattleFx_CleanupResourcesAndWindow(void)
{
	struct WorldMapWork *work = (struct WorldMapWork *)gMapCellBuffer;

	Resource_ResetEntry(work->vram_block);
	Resource_ResetEntry(work->padding02);
	UiWork_FinalizeFar(work->window, 2);
}
#ifndef MODERN // modern GCC keeps the headers' volatile declaration
#endif
s32		   ArcTan2(s32 dz, s32 dx);

//--------------------------------------------------------------------------
// ワールドマップの毎フレーム処理（スケジューラのコールバック）
// フラグの立った場所に印を出し、十字キーでカーソルを動かし、
// カーソルにいちばん近い印の地名をウインドウに出す
// L・R を押している間はカーソルを現在地に合わせる
//--------------------------------------------------------------------------
void Map_UpdateWorldMapMarkers(void)
{
	void				 Vector_AddPolarOffset(s32 magnitude, s32 angle, void *position);
	s32					 leader;
	const s32			*place;
	s32					 tile_base;
	s32					 blend;
	s32					 best;
	s32					 best_dist;
	s32					 cursor_x;
	s32					 best_x;
	s32					 best_y;
	s32					 mes;
	s32					 best_mes;
	s32					 height;
	s32					 width;
	s32					 pos[3];
	struct WorldMapWork *work;
	struct MapObject	*obj;
	s32					 cursor_y;
	s32					 tile;
	s32					 mode;
	s32					 flag;
	s32					 i;
	struct MapMarker	*marker;
	s32					 x;
	s32					 y;
	s32					 angle;

#if WORLD_MAP_CURSOR_SNAPS
	leader = gGameState.selected_actor;
	work = (struct WorldMapWork *)Ram_MapCellBuffer;
	marker = work->markers;
	place = WorldMap_PlaceMarkers;
	tile_base = gVramBlockCache[work->vram_block].offset >> 5;
#else
	work = (struct WorldMapWork *)Ram_MapCellBuffer;
	leader = gGameState.selected_actor;
	place = WorldMap_PlaceMarkers;
	tile_base = gVramBlockCache[work->vram_block].offset >> 5;
	marker = work->markers;
#endif
	best = -1;
	best_dist = WORLD_MAP_NAME_RANGE;
	blend = WorldMap_MarkerBlendCycle[(gFrameCount >> 1) & 31];

	//------------------------------------------------------------------
	// カーソル
	//------------------------------------------------------------------
	if (!GameFlag_TestFar(FLAG_NOT_ON_MAP) && (gKeysHeld & KEYS_SHOULDERS))
	{
		// L・R：現在地
		obj = ObjectTable_Get(gGameState.selected_actor);
		if (obj == NULL)
			goto markers;
		cursor_x = ((obj->x - 0x10000000) >> 16) * 240 / 4096;
		cursor_y = obj->z.part.whole * 160 / 4096;
	}
	else
	{
		// 十字キー：押し続けると速くなる
		angle = WorldMap_CursorDirectionAngles[(gKeysHeld >> 4) & 15];
		if (angle != 0xffff)
		{
			pos[0] = work->x.value;
			pos[1] = 0;
			pos[2] = work->z.value;
			Vector_AddPolarOffset(work->speed, angle, pos);
			if (pos[0] < 0x100000) // x は 16〜239 ドット
				pos[0] = 0x100000;
			if (pos[0] > 0xef0000)
				pos[0] = 0xef0000;
			if (pos[2] < 0) // y は 0〜141 ドット
				pos[2] = 0;
			if (pos[2] > 0x8d0000)
				pos[2] = 0x8d0000;
			work->x.value = pos[0];
			work->z.value = pos[2];
			if (work->speed < 0x60000)
				work->speed += 0x2000;
		}
		else
		{
			work->speed = 0x10000;
		}
		cursor_x = work->x.part.whole;
		cursor_y = work->z.part.whole;
	}

	//------------------------------------------------------------------
	// 印：0 番は現在地、11〜65 番は WorldMap_PlaceMarkers の場所
	// 表は（フラグ, 地名）の組で、フラグの上位 16bit はキャラクタ番号
	//------------------------------------------------------------------
markers:
	for (i = 0; i <= 65; i++)
	{
		if (i != leader && i <= 10)
			continue;
		if (i == 0)
		{
			if (GameFlag_TestFar(FLAG_NOT_ON_MAP))
				continue;
			tile = 0;
			flag = leader;
			mode = 0;
		}
		else
		{
			flag = *place++;
			mes = *place++;
			if (flag == 0)
				continue;
#if WORLD_MAP_CURSOR_SNAPS
			if (flag == -1)
				break;
			tile = flag >> 16;
			mode = 1;
#else
			tile = flag >> 16;
			mode = 1;
			if (flag == -1)
				break;
#endif
		}
		if (!GameFlag_TestFar(flag))
			continue;
		obj = ObjectTable_Get(i);
		if (obj == NULL)
			continue;

		// ワールドの 4096 ドット四方を 240x160 に縮める
		x = ((obj->x - 0x10000000) >> 16) * 240 / 4096;
		y = obj->z.part.whole * 160 / 4096;
		marker->blend_mode = mode;
		marker->tile = tile_base + tile;
		marker->x = x - 1;
		marker->y = y - 1;
		width = x - cursor_x;
		height = y - cursor_y;
		if (width * width + height * height < best_dist)
		{
			best_mes = mes;
			best = i;
			best_dist = width * width + height * height;
			best_x = x;
			best_y = y;
		}
		if (i != 0 || (gFrameCount & 15) <= 7) // 現在地は 8 フレームごとに点滅
			Runtime_PushSlotEntry(marker++, y);
	}
#if WORLD_MAP_CURSOR_SNAPS
	// キーを離していれば、カーソルをいちばん近い印へ寄せる
	if (gKeysHeld == 0)
	{
		if (best != -1)
		{
			if (best_dist <= 1)
			{
				work->x.value = best_x << 16;
				work->z.value = best_y << 16;
				work->speed = 0x10000;
			}
			else
			{
				if (work->speed < 0x20000)
					work->speed = 0x20000;
				angle = ArcTan2(best_y - cursor_y, best_x - cursor_x);
				pos[0] = work->x.value;
				pos[1] = 0;
				pos[2] = work->z.value;
				Vector_AddPolarOffset(work->speed, (u16)angle, pos);
				work->x.value = pos[0];
				work->z.value = pos[2];
				if (work->speed < 0x60000)
					work->speed += 0x2000;
			}
			cursor_x = work->x.part.whole;
			cursor_y = work->z.part.whole;
		}
		else
		{
			work->speed = 0x10000;
		}
	}
#endif

	// いちばん近い印を囲む枠（点滅）
	if (best != -1 && (gFrameCount & 15) <= 7)
	{
		marker->blend_mode = 0;
		marker->tile = tile_base + 3;
		marker->x = best_x - 2;
		marker->y = best_y - 2;
		Runtime_PushSlotEntry(marker, best_y);
	}

	// カーソル
	marker = &work->markers[66];
	marker->affine_index = 0;
	marker->x = cursor_x - 17;
	marker->y = cursor_y + 1;
	Runtime_PushSlotEntry(marker, 246);

	//------------------------------------------------------------------
	// 地名ウインドウ：印が変わったときだけ書き直す
	//------------------------------------------------------------------
	if (work->shown != best)
	{
		work->shown = best;
		RenderOutput_PrepareForRedrawFar(work->window);
		if (best != -1)
		{
			if (best == 0)
				best_mes = (s32)MsgPresentLocation;
			else
				best_mes = Scene_GetAreaNameIndex(best_mes, 1) + (s32)MsgDebugEntryName;
			UiText_MeasureResourceEntriesFar(best_mes, &width, &height);
			cursor_x = best_x;
			cursor_y = best_y;
			cursor_x -= WORLD_MAP_NAME_LEFT;
			cursor_y -= 11;
			if (cursor_x + width > 240)
			{
				// 右へはみ出すなら、印の左上へ回す
				cursor_x = 232 - width;
				cursor_y = best_y - 20;
			}
			if (cursor_y < 0)
				cursor_y = 0;
			UiText_DrawResourceFar(best_mes, work->window, cursor_x, cursor_y);
		}
	}

	// 印の半透明を明滅させる
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}

#undef ObjectTable_Get

void Event_FinishScreenTransition(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void BattleFx_ScheduleRatioTransition(s32, s32);

void Map_ShowWorldMap(s32 picture)
{
	/* FAKEMATCH: needed to match: one-pass loops order display-control setup and the spilled layer load. */
	struct MapState			*map = WORK_SLOT(8);
	struct EventWork		*field = Runtime_AllocateBlock(27, 0xccc);
	struct ObjectSystemWork *menu = WORK_SLOT(HEAP_SLOT_MENU_CONTROL);
	s32					 resource = (s32)&ResourceId_WorldMapPicture;
	struct MapAnimation *layer;
	u8					 saved_flags[16];
	s32					 i;
	u8					*graphics;
	s32					 saved_status;
	s16					 blend;

	if (field->mode != 3)
		return;
	field->view_center->paused = 1;
	saved_status = field->transition_frames;
	field->transition_frames = 6;
	Event_FinishScreenTransition();
	Event_WaitTransitionFrames();
	layer = map->anim;
	for (i = 0; i < 16; i++)
	{
		saved_flags[i] = layer->paused;
		layer->paused = 1;
		layer++;
	}
	menu->suspended = 1;
	WaitFrames(1);
	blend = REG_BLDALPHA;
	graphics = Resource_GetTableEntry(resource);
	Dma_Set(graphics, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | (448 / 4), REG_DMA3);
	BG_PLTT[0] = 0;
	Resource_DecodeType01(graphics + 448, gMapCellBuffer);
	Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00, DMA_ENABLE32 | DMA_32BIT | (240 * 160 / 4),
		REG_DMA3);
	{
		/* FAKEMATCH: needed to match: a word temporary keeps the halfword values as word constants; direct stores load them by halfword from the pool. */
		s32 v = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) | 2;
		REG_BG1CNT = v;
		v = DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
		REG_DISPCNT = v;
	}
	BattleFx_SetupResourcesAndWindow();
	Scheduler_AddOrUpdateCallback(Map_UpdateWorldMapMarkers, 0xc80);
	if (GameFlag_TestFar(284))
		UiText_ShowPositionedMessageAndWaitFar((s32)MsgNotOnMap, 1);
	do
	{
		WaitFrames(1);
	} while ((gKeysRepeat & (KEY_A | KEY_B)) == 0);
	Scheduler_RemoveCallback(Map_UpdateWorldMapMarkers);
	BattleFx_CleanupResourcesAndWindow();
	{
		s32 v;
		/* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling. */
		do
		{
			v = DISPCNT_OBJ_1D_MAP;
		} while (0);
		REG_DISPCNT = v;
	}
	Map_LoadAreaGraphicsFar();
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, (u16)blend, 0x20000);
	/* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling. */
	do
	{
		layer = map->anim;
	} while (0);
	for (i = 0; i < 16; i++)
	{
		layer->paused = saved_flags[i];
		layer++;
	}
	menu->suspended = 0;
	Event_StartScreenTransition();
	Event_WaitTransitionFrames();
	field->transition_frames = saved_status;
	field->view_center->paused = 0;
}

void BattleFx_UpdateObjectVisibilityBounds(void)
{
	struct ObjectRuntime *object;
	s32					  x;
	s32					  y;
	s32					  left;
	s32					  right;
	s32					  top;
	s32					  bottom;
	u32					  id;

	// the selected actor is looked up and the result discarded, as the ROM does
	Object_GetById(gGameState.selected_actor);
	object = (struct ObjectRuntime *)gEventWork->view_center;
	x = object->x;
	left = x - (320 << 16);
	right = x + (320 << 16);
	y = object->z;
	top = y - (600 << 16);
	bottom = y + (400 << 16);

	id = 8;
	do
	{
		object = ObjectTable_Get(id);

		if (object != 0)
		{
			s32 ox = object->x;
			s32 oy = object->z;

			if (ox < left || ox > right || oy < top || oy > bottom)
			{
				object->animation_kind = 0;
			}
			else
			{
				object->animation_kind = 1;
			}
		}
		id++;
	} while (id <= 65);
}

void BattleFx_RunVisibilityTransition(void)
{
	struct EventWork *field = Runtime_AllocateBlock(27, 0xccc);

	if (field->mode == 3)
	{
		Scheduler_EnableUnmaskedOverlayCallbacks();
		BattleFx_UpdateObjectVisibilityBounds();
		Battle_InitializeRenderObject();
		BattleFx_ScheduleRatioTransition(0x9d89, 6);
		if (gKeysHeld & KEY_L)
		{
			do
			{
				WaitFrames(1);
			} while (gKeysHeld & KEY_L);
		}
		BattleFx_ScheduleRatioTransition(0x10000, 6);
		Scheduler_DisableOverlayCallbacks();
	}
}
