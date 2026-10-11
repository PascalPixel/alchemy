//--------------------------------------------------------------------------
// effect.c
// フィールド効果：種類ごとの振り分けと、効果のイベント処理
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
#include "battle_effect_runtime.h"
#include "heap_state.h"
#include "animspr.h"
#include "object_runtime.h"
#include "game_state.h"
#include "fx_scene.h"
#include "types.h"
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
void Battle_Reset();
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
