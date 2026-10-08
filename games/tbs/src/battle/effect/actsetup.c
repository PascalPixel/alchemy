//--------------------------------------------------------------------------
// actsetup.c
// エナジー演出：ワークの準備、使う者と対象の記録、ふくらむ弧の物体
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
#include "resource.h"
#include "vram_block.h"
#include "mem.h"
#include "animspr.h"
#include "object_runtime.h"
#include "script_motion.h"
#include "objdisp.h"
#include "fieldobj.h"
#include "btlsys.h"
#include "types.h"
#include "game_state.h"
#include "fx_scene.h"
#include "callback_scheduler.h"
#include "effect_slot.h"
#include "global_cells.h"
#include "dma.h"
#include "scene_ids.h"
#include "object_lookup.h"
#include "system.h"
#include "io_reg.h"
#include "heap_state.h"
#include "map_scroll.h"

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

extern struct BattleFxScene *gEffectWork;
struct FieldActor			*Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);

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

void Vector_AddPolarOffset(s32, s32, void *);

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
