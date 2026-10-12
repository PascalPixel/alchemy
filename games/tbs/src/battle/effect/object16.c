#include "unit/battle/effect/btlcue.h"
#include "unit/battle/effect/zonedata.h"
#include "vram_tab.h"
#include "unit/battle/effect/object.h"
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

// FieldActor と同じ物体。位置をひとつのベクトルとして渡すための見え方
struct Object
{
	u8						pad0[6];
	u16						angle;
	struct EffectVector		pos;
	u8						pad1[14];
	u8						kind;
	u8						pad2[0x2d];
	struct AnimationObject *sprite;
};

#define TSUBU_MAX 24 // 降らせる粒の数 (scene->slots)

// RunSceneTransitionEffect が 0x298 バイト確保して初期化する
struct WaveFxWork
{
	u16 lines[2][162]; // ラインごとのずれ (2 面を交互に書く)
	u16 phase;		   // 波の位相
	u8	page;		   // 書き込む面
	s8	red;		   // 色相から求めた色 (赤・緑・青、各 4〜11)
	s8	green;
	s8	blue;
	u16 hue;	   // 色相 (0〜359 度)
	u16 source_id; // 使い手
	u16 target_id; // 対象
	u8	delay;	   // 開始までの待ち
	u8	counter;   // 粒を出す周期 (0〜60)
};

void RunSceneTransitionEffect(s32 x, s32 y)
{
	u32				   __umodsi3(u32 numerator, u32 denominator);
	struct WaveFxWork *work;
	volatile u32	   zero;

	work = Runtime_AllocateBlock(22, 0x298);
	BattleEffect_InitializeSharedScene();
	zero = 0;
	Dma_Set((const void *)&zero, work, 0x850000a6, REG_DMA3);
	work->hue = __umodsi3(gFrameCount << 1, 360);
	BattleFx_AdvanceHueCycle();
	BattleFx_ApplyColorToTargetBuffer(
		(work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
	BattleFx_StartBufferInterpolation(8);
	work->source_id = x;
	work->target_id = y;
	work->delay = 8;
	Ui_FillBank15PaletteGrey();
	Scheduler_AddOrUpdateCallback((void (*)(void))BattleFx_UpdateWaveScene, 0xc80);
}

//--------------------------------------------------------------------------
// finish_scene_and_release_heap_block.c
// フィールド効果：ラインごとの波と色の変化、対象から使い手へ流れる粒
//--------------------------------------------------------------------------

struct WaveFxSprite
{
	u32 unknown_00[2];
	u8	unknown_08;
	u32 unknown_09_0 : 2;
	u32 mode : 2;
	u32 unknown_09_4 : 4;
	u8	unknown_0a[0x1c];
	u8	palette;
};

//--------------------------------------------------------------------------
// 効果を止めて使い手の色を戻し、作業領域を返す
//--------------------------------------------------------------------------
void BattleFx_FinishSceneAndReleaseHeapBlock(void)
{
	void *work;

	work = *(void **)((u32)&gBattleBgFxWork);
	Ui_SetBank15PaletteAndClearRenderMode();
	Scheduler_RemoveCallback(BattleFx_UpdateWaveScene);
	Animation_ApplyChildPalette(Object_GetById(((struct WaveFxWork *)work)->source_id), 1);
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
	struct FieldActor		 *src;
	struct FieldActor		 *dst;
	struct FieldActor		 *obj;
	struct WaveFxSprite		 *sprite;
	struct EffectVector		  from;
	struct EffectVector		  to;
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
			(work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
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
			from.x = src->x.fixed;
			meta = Resource_GetMetadataRecordFar(
				(s16)BattleAction_FindDescriptor(work->source_id)->sprite);
			from.y = src->y.fixed + ((s8)meta->box_x << 16) - (2 << 16);
			from.z = src->z.fixed;
			to.x = dst->x.fixed;
			meta = Resource_GetMetadataRecordFar(
				(s16)BattleAction_FindDescriptor(work->target_id)->sprite);
			to.y = dst->y.fixed + ((s8)meta->box_x << 16) - (2 << 16);
			to.z = dst->z.fixed;
			obj = Object_CreateFar(281, to.x, to.y, to.z);
			if (obj != 0)
			{
				sprite = obj->sprite;
				obj->motion_flags = 0;
				obj->speed = 0xa3d7; // 0.64 倍
				obj->acceleration = 0xa3d7;
				obj->facing = ArcTan2(from.z - to.z, from.x - to.x);
				obj->update = BattleFx_SetCallbackWhenTargetUnset;
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

#define GREY28 0x739c // 灰色 (赤・緑・青とも 28)

//--------------------------------------------------------------------------
// BG0 の横スクロールを走査線ごとに書き換える HBlank DMA を張りなおす
//--------------------------------------------------------------------------
void BattleFx_ArmBg0HBlankDma(void)
{
	struct WaveFxWork *work = gBattleBgFxWork;
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
	struct WaveFxWork *work = gBattleBgFxWork;
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
	angle = Fixed_Remainder(angle, 360 << 16);
	if (angle < 60 << 16)
		return Iwram_RatioMulQ14(60 << 16, Iwram_MulQ16(high, angle));
	if (angle >= 60 << 16 && angle < 180 << 16)
		return high;
	if (angle >= 180 << 16 && angle < 240 << 16)
		return Iwram_RatioMulQ14(60 << 16, Iwram_MulQ16(high, (240 << 16) - angle));
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
s32 BattleFx_GetCycledTableWord(u32 unused)
{
	return BattleFx_CyclePatternWords[(gKeysHeld >> 4) & 15]; // 右・左・上・下の 4 ビット
}

//--------------------------------------------------------------------------
// fragment.c
// 演出：アイテムがこわれる演出の本体の動き
//--------------------------------------------------------------------------

// FieldActor と同じ物体。FieldActor に欄のない 0x5e を読むための見え方
struct ItemBreakFragmentObject
{
	u8	reserved_00[72];
	s32 gravity;
	u8	reserved_4c[9];
	u8	motion_flags;
	u8	reserved_56[8];
	u16 unknown_5e;
};

//--------------------------------------------------------------------------
// こわれたアイテムの本体の update
// 向く相手がいれば、そちらへ 1 回に 0x1000（22.5 度）まで曲がる
// そして、まわりにかけらの粒をひとつ出す
// src : 本体のオブジェクト
//--------------------------------------------------------------------------
void BattleFx_UpdateItemBreakFragment(struct FieldActor *src)
{
	struct FieldActor			   *tgt;
	struct EffectVector				pos;
	struct ItemBreakFragmentObject *obj;
	s32								turn;
	s32								dist;

	tgt = src->linked_object;
	if (tgt != 0)
	{
		s32 dx = tgt->x.fixed - src->x.fixed;
		s32 dz = tgt->z.fixed - src->z.fixed;

		if (dx != 0 || dz != 0)
		{
			turn = (s16)(ArcTan2(dz, dx) - src->facing);
			if (turn > 0x1000)
				turn = 0x1000;
			if (turn < -0x1000)
				turn = -0x1000;
			src->facing += turn;
		}
		src->steering_flags = 0;
	}

	// 粒は本体から y を 8〜24 ずらし、ランダムな向きに半径 0〜48 散らした位置に出す
	pos.x = src->x.fixed;
	pos.y = src->y.fixed - (Random16() << 4) - 0x80000;
	pos.z = src->z.fixed;
	dist = Random16() * 3;
	dist <<= 4;
	Vector_AddPolarOffset(dist, Random16(), &pos);

	obj = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
	if (obj != 0)
	{
		obj->motion_flags = 2;
		obj->gravity = 0x1999;
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
		obj->unknown_5e = 12;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}
}

//--------------------------------------------------------------------------
// fieldfx.c
// フィールド効果：方向キーで対象を押す処理、出現と消滅の粒、範囲の見張り
//--------------------------------------------------------------------------

#define CELL_MASK 0xfff00000 // 16 ドットのマス

//--------------------------------------------------------------------------
// 対象のそばに押すオブジェクトを出し、方向キーの向きへ対象を 16 ドット押す
// A, B, L, R で取りやめる。押せないときは 16 フレームごとにエラー音
// 押す先が使い手と重なるときは、使い手の位置しだいで押せないか、使い手も動かす
//--------------------------------------------------------------------------
void BattleFx_SteerPushedTarget(s32 target_id)
{
	struct EffectVector	  position;
	struct EffectVector	  beyond;
	struct BattleFxScene *work;
	struct FieldActor	 *user;
	struct FieldActor	 *target;
	struct FieldActor	 *pusher;
	struct FieldActor	 *hit;
	s32					  x;
	s32					  z;
	s32					  angle;
	s32					  moved;
	s32					  dir;
	s32					  keys;
	s32					  cell_x;
	s32					  cell_z;
	struct EffectVector	 *p;

	work = gEffectWork;
	user = work->main_object;
	target = work->child;
	angle = work->angle + 0x8000; // work->angle の反対向き
	moved = 0;
	if (target == NULL)
		return;
	BattleEffect_InitializeSharedScene();
	user->linked_object = target;
	ObjectDispatch_InitializeFar((struct DispatchObject *)user, BattleFx_SourceHoldScript);
	pusher = BattleFx_StartItemBreak(user);
	if (pusher == NULL)
	{
		BattleFx_PrepareBufferInterpolation();
		return;
	}

	// 押すオブジェクトは対象の 16 ドット上、angle の向きへ 16 ドットの位置
	pusher->linked_object = target;
	p = &position;
	p->x = target->x.fixed;
	p->y = target->y.fixed + (16 << 16);
	p->z = target->z.fixed;
	Vector_AddPolarOffset(16 << 16, angle, p);
	Object_SetMoveTargetFar(pusher, p->x, p->y, p->z);
	BattleFx_SnapScaleToFull((struct MotionObject *)pusher);
	pusher->speed = 4 << 16;
	pusher->acceleration = 0x8000;
	pusher->motion_flags = 4;
	target->update = ObjectGroup_ApplyRandomChildValues; // 押す間は色を巡らせる
	target->speed = 0x6666;
	target->acceleration = 0x3333;
	target->steering_flags = moved;
	target->terrain_id = 2;
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
			position.x = target->x.fixed;
			position.y = target->y.fixed + (16 << 16);
			position.z = target->z.fixed;
			Vector_AddPolarOffset(16 << 16, angle, &position);
			Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
			ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 1);
			pusher->velocity_x = 0;
			pusher->velocity_y = 0;
			pusher->velocity_z = 0;
			continue;
		}

		// 押すオブジェクトを押す向きへ 2 ドット寄せ、対象の 16 ドット先と 32 ドット先を調べる
		position.x = target->x.fixed;
		position.y = target->y.fixed + (16 << 16);
		position.z = target->z.fixed;
		Vector_AddPolarOffset(16 << 16, angle, &position);
		Vector_AddPolarOffset(2 << 16, dir, &position);
		Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
		Script_WaitForEventTimeoutFar(pusher);
		position.x = target->x.fixed;
		position.y = target->y.fixed;
		position.z = target->z.fixed;
		Vector_AddPolarOffset(16 << 16, dir, &position);
		beyond.x = target->x.fixed;
		beyond.y = target->y.fixed;
		beyond.z = target->z.fixed;
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
		cell_x = user->x.fixed & CELL_MASK;
		cell_z = user->z.fixed & CELL_MASK;
		if (cell_x == (p->x & CELL_MASK) && cell_z == (p->z & CELL_MASK))
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		if (cell_x != (beyond.x & CELL_MASK) || cell_z != (beyond.z & CELL_MASK))
			goto move;
		if (Map_GetCellAttributeLowNibbleFar(user->terrain_id, beyond.x, beyond.z) != 0)
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
		target->paused = 0;
		target->speed = 0x3333;
		target->acceleration = 0x3333;
		Object_SetMoveTargetFar(target, position.x, position.y, position.z);
		pusher->motion_flags = 0;
		pusher->speed = 0x3333;
		pusher->acceleration = 0x3333;
		Vector_AddPolarOffset(16 << 16, dir, &position);
		Object_SetMoveTargetFar(pusher, position.x, position.y + (16 << 16), position.z);
		if (moved == 1)
		{
			Object_GetById(work->first_object_id)->steering_flags &= 0xfe;
			user->speed = 0x3333;
			user->acceleration = 0x3333;
			Object_SetMoveTargetFar(user, beyond.x, beyond.y, beyond.z);
		}
		Script_WaitForEventTimeoutFar(target);
		target->x.fixed = x;
		target->z.fixed = z;
		target->velocity_x = 0;
		target->velocity_z = 0;
		break;
	}

	// 対象の色、スクリプト、毎フレームの処理を元に戻し、押すオブジェクトを消す
	Animation_ApplyChildValuesFar(target, work->saved_palette);
	ObjectDispatch_InitializeFar((struct DispatchObject *)target, work->saved_script);
	target->update = work->saved_callback;
	EffectRuntime_StopCurrentObject();
	if (moved == 1)
		(Object_GetById(work->first_object_id))->steering_flags |= 1;
	BattleFx_PrepareBufferInterpolation();
	UpdateRisingParticleBurst(pusher);
}

//--------------------------------------------------------------------------
// 粒の動き：元の位置から 30 ドット先へ飛び、また元の位置へ戻って消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *slot)
{
	struct EffectVector position;
	s8				   *state_ptr = &slot->state;
	s32					state;

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
		Script_WaitForEventTimeoutFar(obj);
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
	struct ObjectRuntime *object = gObjectSlots;
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
	struct FieldWork	 *work;
	struct ObjectRuntime *actor;
	s16					 *timer;
	s32					  dx;
	s32					  dz;

	work = (struct FieldWork *)gEventWork;
	actor = ObjectTable_Get(gGameState.selected_actor);
	if (work->unknown_cc0 != 0) // 時間を減らすか
	{
		timer = &work->effect_frames;
		if (*timer != 0)
			(*timer)--;
	}

	// x は 0xd105 (約 0.82) 倍して比べる
	dx = Iwram_MulQ16(work->effect_center_x - actor->x / 0x10000, 0xd105);
	dz = work->effect_center_y - (actor->z - actor->y) / 0x10000;
	if (dx * dx + dz * dz >= 3600 || work->effect_frames == 0)
	{
		u16 *slot = (u16 *)&work->command; // psynergy_request
		u32	 request = 0x2090;

		*slot = request;
	}
}

//--------------------------------------------------------------------------
// markers.c
// 戦闘エフェクト 8 番：選択中のオブジェクトを中心に画面を切りかえ、印を出す
//--------------------------------------------------------------------------
// gEventWork から並ぶワークのポインタ
struct EffectGlobals
{
	struct FieldWork			  *scene; // イベントのワーク
	u8							   unknown_04[12];
	struct DisplayTransitionState *work; // 画面切りかえのワーク
};

//--------------------------------------------------------------------------
// 戦闘エフェクト 8 番
// 選択中のオブジェクトの位置を中心に画面切りかえを始め、
// 合うイベントがあれば動かしてから、まわりに印を出す
//--------------------------------------------------------------------------
void RunBattleEffect08(void)
{
	struct FieldWork			  *scene;
	struct DisplayTransitionState *work;
	struct FieldActor			  *target;
	s32							   arg;
	s32							   ev;
	s32							   frame;

	scene = ((struct EffectGlobals *)&gEventWork)->scene;
	ObjectDispatch_ApplyValueToKind200Children(6);
	DisplayTransition_InitializeBattleEffectState(8);
	work = ((struct EffectGlobals *)&gEventWork)->work;
	target = ObjectTable_Get(gGameState.selected_actor);
	work->centre_x = target->x.fixed;
	work->centre_y = target->z.fixed - target->y.fixed;
	BattleFx_ApplyColorToSourceBuffer(0x10000, 0);
	BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
	BattleFx_StartBufferInterpolation(1);
#if !defined(TBS_EDITION_EN)
	scene->effect_active = 0; // 英語版にはない
#endif
	WaitFrames(1);
	ev = BattleFx_FindMatchingEvent(0x50000005, 8, &arg);
	if (ev != 0)
		BattleFx_RunEventAction(ev, gGameState.selected_actor, arg);
	AudioCommand_PlayFar(131);

	// イベントのワークに中心 (ドット) と時間を入れ、近くに印を出す
	scene->effect_active = 1;
	scene->effect_center_x = work->centre_x / 0x10000;
	scene->effect_center_y = work->centre_y / 0x10000;
	scene->effect_frames = 600;
	scene->unknown_cc0 = 1;
	FieldEffect_SpawnNearbyMarkers();
	frame = 0;
	do
	{
		WaitFrames(1);
		work->value = frame;
		frame++;
	} while (frame <= 18);
	Scheduler_AddOrUpdateCallback(FieldEffect_WatchLeaderDistance, 0xc80);
}

//--------------------------------------------------------------------------
// burstobj.c
// エナジー演出：イマジンの打ち切り、演出 12 の粒、演出 7 で対象を持ち上げる
//--------------------------------------------------------------------------

#define ENA_EV_4 0x40000005 // エナジーのイベント、段階 4
#define FX_IMAJIN 8			// イマジンの演出
#define TSUBU_OBJ 284		// 粒の物体
#define FALL_OBJ 217		// 落ちる物体
#define FALL_LAST 4			// 落ちる物体は 0-4 の 5 個

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
	// gEffectWork はスロット 56。0x74 前がイベントワーク (27)、0x64 前がスロット 31
	struct BattleFxScene		 **cell = &gEffectWork;
	struct BattleFxScene		  *work = *cell;
	struct DisplayTransitionState *scene = *(struct DisplayTransitionState **)((u8 *)cell - 0x64);
	struct FieldWork			  *state = *(struct FieldWork **)((u8 *)cell - 0x74);
	s16							   zero;
	s32							   tid;
	void						  *ev;

	if (state->effect_active != 0)
	{
		AudioCommand_PlayFar(SOUND_SCENE_TRANSITION);
		Scheduler_RemoveCallback(FieldEffect_WatchLeaderDistance);

		zero = 0;
		state->effect_active = zero;
		state->effect_frames = zero;
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
			scene->next_mode = 0;
			scene->duration = 1;
			scene->step = 1;
			WaitFrames(10);
		}
	}
}

//--------------------------------------------------------------------------
// 対象のある演出 12：対象のまわりに粒を出し、対象を足元の優先度にする
//--------------------------------------------------------------------------
void BattleFx_RunBurstParticleMainObject(s32 unused)
{
	struct FieldActor *tgt;

	tgt = ((struct BattleFxScene *)gEffectWork)->child;
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
void BattleEffect_RunTargetedItemBreak(s32 unused)
{
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

// 破片の数
#define HAHEN_MAX 24

//--------------------------------------------------------------------------
// 対象のない演出の本体：演出の位置の 16 ドット上、左右 32 ドットに spawn で
// 物体を出し、それぞれの向きへ 24 ドット動かしてから、破片を 24 個、
// 8-32 ドット先へ散らす
//--------------------------------------------------------------------------
#define DEFINE_ITEM_BREAK_EFFECT(name, spawn)                                          \
	void name(void)                                                                    \
	{                                                                                  \
		struct BattleFxScene  *work;                                                   \
		struct MotionObject	  *obj;                                                    \
		struct MotionObject	  *pair[2];                                                \
		struct ScatterPosition pos;                                                    \
		struct MotionObject	 **walk;                                                   \
		s32					   n;                                                      \
		s32					   dist;                                                   \
                                                                                       \
		work = gEffectWork;                                                            \
		BattleEffect_InitializeSharedScene();                                          \
		pos.x = work->x;                                                               \
		pos.y = work->y + 0x100000;                                                    \
		pos.z = work->z;                                                               \
		pair[0] = spawn(pos.x + 0x200000, pos.y, pos.z, 0x8000);                       \
		pair[1] = spawn(pos.x - 0x200000, pos.y, pos.z, 0);                            \
                                                                                       \
		WaitFrames(15);                                                                \
		walk = pair;                                                                   \
		for (n = 0; n < 2; n++)                                                        \
		{                                                                              \
			obj = *walk++;                                                             \
			if (obj != 0)                                                              \
				Motion_SetTargetPositionFromMagnitudeAngle(obj, 0x180000, obj->angle); \
		}                                                                              \
                                                                                       \
		Script_WaitForEventTimeoutFar(pair[0]);                                        \
		AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);                                      \
		for (n = 0; n < HAHEN_MAX; n++)                                                \
		{                                                                              \
			pos.x = work->x;                                                           \
			pos.y = work->y + 0x100000;                                                \
			pos.z = work->z;                                                           \
			obj = Object_Spawn(285, pos.x, pos.y, pos.z);                              \
			if (obj != 0)                                                              \
			{                                                                          \
				ObjectDispatch_InitializeFar(                                          \
					(struct DispatchObject *)obj, BattleFx_FragmentScript);            \
				obj->speed_limit = Random16() + 0x20000;                               \
				obj->acceleration = 0x20000;                                           \
				obj->motion_flags = 0;                                                 \
				dist = Random16() * 24 + 0x80000;                                      \
				Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());     \
			}                                                                          \
		}                                                                              \
                                                                                       \
		ObjectDispatch_ReleaseFar(pair[0]);                                            \
		ObjectDispatch_ReleaseFar(pair[1]);                                            \
		BattleFx_PrepareBufferInterpolation();                                         \
	}

// 対象のない演出 7
DEFINE_ITEM_BREAK_EFFECT(RunBattleEffect07, BattleFx_SpawnItemBreakMode3)

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
//--------------------------------------------------------------------------
// 粒の動き : 原点のまわりへ散ってから、主オブジェクトのそばへ集まって消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialBurst(struct EffectSlot *slot)
{
	struct BattleFxScene *scene = gEffectWork;
	struct EffectVector	  pos;
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
	struct EffectVector	  pos;
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
void BattleFx_CallEffect04(s32 unused)
{
	RunBattleEffect04();
}

//--------------------------------------------------------------------------
// orbit.c
// フィールドのエナジー演出：4 番（押し出す弾）、6 番（渦を巻いて集まる粒）
//--------------------------------------------------------------------------

#define UE (16 << 16) // 16 ドット上

//--------------------------------------------------------------------------
// obj の 16 ドット上の位置
//--------------------------------------------------------------------------
static __inline__ void PosAbove(struct FieldActor *obj, struct EffectVector *pos)
{
	pos->x = obj->x.fixed;
	pos->y = obj->y.fixed + UE;
	pos->z = obj->z.fixed;
}

// FieldActor と同じ物体。FieldActor に欄のない 0x5e を使う
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
	struct FieldActor	   *tgt;
	s32						ctx;
	struct FieldActor	   *copies[4];
	struct EffectVector		pos;
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
		ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 9);
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
	struct EffectVector pos;
	s16					life;
	s32					cnt;

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
	struct EffectVector pos;
	s16					life;
	s32					cnt;

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
void BattleFx_StartOrbitingParticles(s32 unused)
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
	struct EffectVector	  pos;
	struct EffectVector	 *p;
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
	struct EffectVector	  pos;
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
//--------------------------------------------------------------------------
// 演出 03 を流し、いまのオブジェクトを止める
//--------------------------------------------------------------------------
void BattleFx_CallEffect03AndStop(s32 unused)
{
	RunBattleEffect03();
	EffectRuntime_StopCurrentObject();
}

//--------------------------------------------------------------------------
// 演出 03
// 8 個の粒が渦を巻いて中心へ集まり、中心に現れた 1 個が火花を出してから
// 遠くへ飛び去る
//--------------------------------------------------------------------------
void RunBattleEffect03(void)
{
	struct BattleFxScene *scn = gEffectWork;
	struct FieldActor	 *target = scn->main_object;
	struct FieldActor	 *object;
	struct FieldSprite	 *last;
	u8					  marker;
	s32					  i;

	BattleEffect_InitializeSharedScene();

	// 渦の粒：1 フレームに 1 個ずつ、1/8 周ずらして出す
	last = 0;
	for (i = 0; i < 8; i++)
	{
		object = Object_Spawn(233, target->x.fixed, target->y.fixed + 0x200000, target->z.fixed);
		if (object != 0)
		{
			object->scale_y = 0xb333; // 0.7 倍
			object->scale_x = 0xb333;
			object->update = BattleFx_UpdateShrinkingOrbitObject;
			object->timer = 120;
			object->work = i << 13;
			object->motion_flags = 4;
			last = Object_ReplaceResourceEntry(object->sprite, last);
		}
		WaitFrames(1);
	}

	marker = last->vram_block;
	AudioCommand_PlayFar(130);
	WaitFrames(110);

	// 中心の 1 個
	target = Object_Spawn(233, 0, 0, 0);
	object = target;
	if (target != 0)
	{
		target->scale_y = 0xb333;
		target->scale_x = 0xb333;
		target->x.fixed = scn->x;
		target->y.fixed = scn->y + 0x100000;
		target->z.fixed = scn->z;
		target->motion_flags = 4;
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
		target->update = BattleFx_RunSparkEmitter;
		target->timer = 0;
		if (scn->enabled != 0)
			WaitFrames(128);
		else
			WaitFrames(192);
	}

	// 飛び去る
	if (target != 0)
	{
		target->timer = -1;
		target->speed = 0x50000;
		target->acceleration = 0x6666;
		target->steering_flags = 0;
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

// FieldActor と同じ物体。FieldActor に欄のない 0x5e (phase) を使う
struct Effect05Object
{
	u8					 reserved_00[8];
	s32					 x, y, z;
	s32					 altitude; // 落ちて止まる高さ
	s32					 scale_x, scale_y;
	u8					 reserved_20[8];
	s32					 velocity_y;
	u8					 reserved_2c[4];
	s32					 speed;
	u8					 reserved_34[0x1c];
	struct WaveFxSprite *sprite;
	u8					 reserved_54;
	u8					 flag;
	u8					 reserved_56[8];
	s16					 phase;
	u8					 reserved_60[0xc];
	void				*callback; // 毎フレームの処理
};

struct Effect05State
{
	s32			   angle;
	s32			   x, y, z; // 玉の行き先の足もと
	struct Object *target;
	s32			   initialized;
	u8			   reserved_18[8];
	u8			   high_arc;
	u8			   reserved_21[0x13];
	u8			   variant;
	u8			   reserved_35[0x10];
	u8			   alternate;
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
			Motion_SetTargetPositionFromMagnitudeAngle(p, 32 << 16, Random16());
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

void BattleFx_CallEffect05(s32 unused)
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
	struct EffectVector	   pos;
	struct EffectVector	   start;
	struct EffectVector	   end;
	struct Effect05State  *work = gEffectWork;
	struct Object		  *target = work->target;
	struct Effect05Object *orb;
	struct EffectVector	  *from;
	struct EffectVector	  *to;
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
		work->x = target->pos.x;
		work->z = target->pos.z;
		Vector_AddPolarOffset(16 << 16, work->angle, (struct EffectVector *)&work->x);
		work->y = Map_GetTerrainHeightFar(0, work->x, work->z);
	}
	from = &start;
	from->x = target->pos.x;
	from->y = target->pos.y + (16 << 16);
	from->z = target->pos.z;
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
			struct EffectVector *a = &start;
			struct EffectVector *b = &end;
			s32					 e, t;

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
				ObjectDispatch_ApplyArgumentToChildrenFar(mote, 8);
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
			struct EffectVector *a = &end;
			struct EffectVector *b = &start;
			s32					 e, t;

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

struct FallScene
{
	u8				   reserved_00[20];
	struct FieldActor *child; // 持ち上げている物
};

LAYOUT_SIZE_GUARD(FallPos_Size, struct EffectVector, 0x0c);
LAYOUT_OFFSET_GUARD(FallScene_Child, struct FallScene, child, 0x14);

//--------------------------------------------------------------------------
// 毎フレームの処理：持ち上げている物のまわりに、落ちる粒をひとつ出す
// 高さは物から 8〜24 ドット上、横はでたらめな向きに 0〜48 ドット
//--------------------------------------------------------------------------
void BattleFx_SpawnFallingParticles(void)
{
	struct FieldActor			   *src;
	struct EffectVector				pos;
	struct ItemBreakFragmentObject *obj;
	u32								dist;

	src = ((struct FallScene *)gEffectWork)->child;
	pos.x = src->x.fixed;
	pos.y = src->y.fixed - Random16() * 16 + (24 << 16);
	pos.z = src->z.fixed;

	dist = Random16() * 3;
	Vector_AddPolarOffset(dist * 16, Random16(), &pos);

	obj = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
	if (obj != 0)
	{
		obj->motion_flags = ACTOR_FALLS;
		obj->gravity = 0x1999; // 重力 0.1 ドット
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
		obj->unknown_5e = 12;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}
}

//--------------------------------------------------------------------------
// lift_drop.c
// 対象を 2 つの持ち手で持ち上げ、方向キーで動かして落とす
//--------------------------------------------------------------------------

#define SHARD_MAX 20 // 破片の数

//--------------------------------------------------------------------------
// 足もとを 16 ドット上げて、pos へ動いたときにぶつかるかを調べる
//--------------------------------------------------------------------------
static __inline__ s32 ObjProbeUp(struct FieldActor *obj, struct EffectVector *pos)
{
	s32 hit;

	obj->terrain_height += 16 << 16;
	hit = Map_CheckObjectStepFar(obj, pos);
	obj->terrain_height -= 16 << 16;
	return hit;
}

//--------------------------------------------------------------------------
// 対象を左右の持ち手で 24 ドットの高さまで持ち上げ、方向キーの向きへ
// 16 ドット動かして落とす (A・B・L・R ならその場で落とす)
// 行き先がふさがっていれば、ブザーを鳴らして待つ
//--------------------------------------------------------------------------
void BattleFx_SteerLiftedTarget(s32 id)
{
	struct BattleFxScene *work;
	struct FieldActor	 *src;
	struct FieldActor	 *target;
	struct FieldActor	 *shards[SHARD_MAX];
	struct EffectVector	  pos;
	struct FieldActor	 *hands[2];
	struct FieldActor	 *obj;
	struct FieldActor	 *right;
	s32					  i;
	s32					  angle;
	s32					  dist;
	struct EffectVector	 *p;

	work = gEffectWork;
	src = work->main_object;
	target = work->child;
	if (target == NULL)
		return;
	BattleEffect_InitializeSharedScene();
	src->linked_object = target;
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
	p->x = target->x.fixed;
	p->y = target->y.fixed + (16 << 16);
	p->z = target->z.fixed;
	Object_SetMoveTargetFar(obj, p->x + (16 << 16), p->y, p->z);
	Object_SetMoveTargetFar(right, p->x - (16 << 16), p->y, p->z);
	Script_WaitForEventTimeoutFar(obj);
	Script_WaitForEventTimeoutFar(right);
	obj->x.fixed = p->x + (16 << 16);
	obj->velocity_x = 0;
	right->x.fixed = p->x - (16 << 16);
	right->velocity_x = 0;

	// 地面から 24 ドットの高さまで持ち上げる
	target->update = ObjectGroup_ApplyRandomChildValues;
	Scheduler_AddOrUpdateCallback(BattleFx_SpawnFallingParticles, 0xc80);
	AudioCommand_PlayFar(130);
	target->motion_flags = 4;
	ObjectDispatch_SetSingleChildFlagsFar(target, 0);
	if (hands[0] != NULL && hands[1] != NULL)
	{
		while (target->y.fixed - target->terrain_height <= 24 << 16)
		{
			obj->y.fixed += 0x6000;
			right->y.fixed += 0x6000;
			target->y.fixed += 0x6000;
			WaitFrames(1);
		}
	}
	hands[0]->speed = 4 << 16;
	hands[0]->acceleration = 0x8000;
	hands[1]->speed = 4 << 16;
	hands[1]->acceleration = 0x8000;
	target->speed = 0x6666;
	target->acceleration = 0x3333;
	target->steering_flags = 0;
	target->terrain_id = 2;

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
			pos.x = target->x.fixed;
			pos.y = target->y.fixed + (16 << 16);
			pos.z = target->z.fixed;
			Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
			Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 1);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 1);
			continue;
		}

		// 持ち手をその向きへ 2 ドット寄せ、16 ドット先の地面を調べる
		pos.x = target->x.fixed;
		pos.y = target->y.fixed + (16 << 16);
		pos.z = target->z.fixed;
		Vector_AddPolarOffset(2 << 16, angle, &pos);
		Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
		Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
		Script_WaitForEventTimeoutFar(hands[0]);
		Script_WaitForEventTimeoutFar(hands[1]);
		pos.x = target->x.fixed;
		pos.y = target->terrain_height;
		pos.z = target->z.fixed;
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
			struct FieldActor *second;
			s32				   x;
			s32				   z;

			AudioCommand_PlayFar(175);
			x = p->x;
			z = p->z;
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
			WaitFrames(15);
			target->paused = i;
			target->speed = 0x3333;
			target->acceleration = 0x3333;
			Object_SetMoveTargetFar(target, p->x, p->y, p->z);
			hands[0]->speed = 0x3333;
			hands[0]->acceleration = 0x3333;
			second = hands[1];
			second->speed = 0x3333;
			second->acceleration = 0x3333;
			Object_SetMoveTargetFar(hands[0], p->x + (16 << 16), p->y, p->z);
			Object_SetMoveTargetFar(second, p->x - (16 << 16), p->y, p->z);
			Script_WaitForEventTimeoutFar(target);
			target->x.fixed = x;
			target->z.fixed = z;
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
	p->x = target->x.fixed;
	p->y = target->y.fixed + (16 << 16);
	p->z = target->z.fixed;
	{
		struct FieldActor **walk = shards;

		// 破片 : 速さ 2〜3 ドット、8〜32 ドット先のでたらめな方向へ
		for (i = 0; i < SHARD_MAX; i++)
		{
			obj = Object_Spawn(0x11d, p->x, p->y, p->z);
			*walk++ = obj;
			if (obj != NULL)
			{
				ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_FragmentScript);
				obj->speed = Random16() + (2 << 16);
				obj->acceleration = 2 << 16;
				obj->motion_flags = 0;
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
	target->update = work->saved_callback;
	target->motion_flags = 3;
	target->velocity_y = 10 << 16;
	target->bounce = 0x3333;
	target->terrain_id = 0;
	src->update = 0;
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

DEFINE_ITEM_BREAK_EFFECT(RunBattleEffect11, BattleFx_SpawnItemBreakMode1)

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

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../../field/common/effect/move.c"

// FieldActor と同じ物体。0x64 を符号なし、0x66 を符号つきで読む (FieldActor の型ではバイトが変わる)
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

// ここに共通ソースが入る（ROM の並びのとおり）
#define EFFECT_SPAWN_RESIDENT
#include "../../field/common/effect/spawn.c"
#undef EFFECT_SPAWN_RESIDENT

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

void BattleFx_CallEffect14(s32 unused)
{
	RunBattleEffect14();
}

//--------------------------------------------------------------------------
// radial.c
// フィールドのエナジー演出：14 番（運ばれる玉と、まわりを回る粒）ほか
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// エナジー 14 番 : 玉を使う者から scene の位置（4 ドット上）まで 10 フレームで運び、
// 0.25 倍から大きくする。scene->enabled がなければ、かけられた物を縮め、
// 玉のまわりに 6 フレームおきに粒を 16 個出して回らせる
//--------------------------------------------------------------------------
void RunBattleEffect14(void)
{
	struct BattleFxScene *scene = gEffectWork;
	struct FieldActor	 *src = scene->main_object;
	struct FieldActor	 *tgt = scene->child;
	struct EffectVector	  pos;
	struct EffectVector	  from;
	struct EffectVector	  to;
	struct FieldActor	 *tama;
	s32					  step;
	struct FieldActor	 *particle;
	s32					  steps = 11;

	step = 0;
	from.x = src->x.fixed;
	from.y = src->y.fixed;
	from.z = src->z.fixed;
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
		tama->x.fixed = from.x + step * (to.x - from.x) / 10;
		tama->y.fixed = from.y + step * (to.y - from.y) / 10;
		tama->z.fixed = from.z + step * (to.z - from.z) / 10;
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
			tgt->update = BattleFx_ShrinkObjectScaleUntilHalf;

		// 粒は玉の 4 ドット上から 0.8 ドットずつ高く、3〜8 ドット離して出す
		step = 0;
		do
		{
			s32 dist;

			pos.x = tama->x.fixed;
			pos.y = tama->y.fixed + step * 0xcccc + (4 << 16);
			pos.z = tama->z.fixed;
			dist = Random16() * 5 + (3 << 16);
			Vector_AddPolarOffset(dist, Random16(), &pos);
			particle = Object_Spawn(0xf9, pos.x, pos.y, pos.z);
			if (particle != 0)
			{
				particle->update = BattleFx_CircleAnchor;
				particle->linked_object = tama;
				particle->timer = 0;
				particle->work = 0;
				particle->facing = Random16();
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
	if (state == 2)
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
	if (state == 3)
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
void BattleFx_MarkChildAndRunFallbackTransition(s32 unused)
{
	struct FieldActor *tgt = ((struct BattleFxScene *)gEffectWork)->child;

	tgt->paused = 1;
	BattleEffect_RunFallbackObjectTransition();
}

//--------------------------------------------------------------------------
// fallback.c
// フィールド効果：対象のない場所へ飛ばして散らす
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 使い手から効果の位置へオブジェクトを 10 フレームで飛ばし、16 個の粒にして散らす
//--------------------------------------------------------------------------
void BattleEffect_RunFallbackObjectTransition(void)
{
	struct BattleFxScene *scene = gEffectWork;
	struct MotionObject	 *user = scene->main_object;
	struct MotionObject	 *obj;
	struct EffectSlot	 *slot;
	struct EffectVector	  pos;
	struct EffectVector	  from;
	struct EffectVector	  to;
	s32					  step;
	s32					  i;

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
// 今の演出定義の flags が 0xff なら、対象なし (-1) を返す
// id : 対象オブジェクト番号
//--------------------------------------------------------------------------
s32 BattleFx_FilterObjectIdByFlags(s32 id)
{
	if (BattleFx_GetFlags(id) == 0xff)
		return -1;
	return id;
}

//--------------------------------------------------------------------------
// run_effect13_hook.c
// エナジー演出 13：オブジェクト 215 を目標まで伸ばし、押し下げて戻す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 対象への演出 (BattleFx_DispatchRequestKind から)
//--------------------------------------------------------------------------
void BattleFx_RunEffect13Hook(s32 unused)
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

//--------------------------------------------------------------------------
// 粒の動き : 主オブジェクトの画面位置 (z + 8 ドット) から、0xc000 の向きの ±45 度へ
// 120 ドット飛ぶ。状態 1 は着いたら飛びなおし、状態 2 は着いたら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialLaunch(struct EffectSlot *slot)
{
	struct EffectVector pos;
	struct FieldActor  *src;
	s32					state;
	u32					r;

	src = ((struct BattleFxScene *)gEffectWork)->main_object;
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

//--------------------------------------------------------------------------
// 戦闘エフェクト 15 番
// 主オブジェクトを回して縮めながら、粒を 1 フレームに 1 つずつ 24 個飛ばす
//--------------------------------------------------------------------------
void BattleFx_RunEffect15(void)
{
	struct BattleFxScene *scene = gEffectWork;
	struct FieldActor	 *obj = scene->main_object;
	struct EffectVector	  pos;
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

void BattleFx_UpdateDescendingParticlePositiveArc(struct FieldActor *particle)
{
	struct FieldActor *source;
	s32				   y;
	s32				   limit;
	s32				   scale;

	limit = particle->terrain_height + 0xA0000;
	source = particle->linked_object;
	if (gGameState.map == (s32)&SceneId_Clear)
		limit = particle->terrain_height + 0x40000;

	y = particle->y.fixed;
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
	particle->x.fixed = source->x.fixed;
	particle->y.fixed = y - 0x20000;
	particle->z.fixed = source->z.fixed + ((0x10000 - scale) * 5) + 0x90000;
}

void BattleFx_UpdateDescendingParticleNegativeArc(struct FieldActor *particle)
{
	struct FieldActor *source;
	s32				   y;
	s32				   limit;
	s32				   scale;

	limit = particle->terrain_height + 0xA0000;
	source = particle->linked_object;
	if (gGameState.map == (s32)&SceneId_Clear)
		limit = particle->terrain_height + 0x40000;

	y = particle->y.fixed;
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
	particle->x.fixed = source->x.fixed;
	particle->y.fixed = y - 0x20000;
	particle->z.fixed = source->z.fixed - ((0x10000 - scale) * 5) + 0x100000;
}

struct DescendingArcSprite
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
	u8					   padding0c[16];
	u8					   resource;
	u8					   active : 1;
	u8					   flags1d_1 : 7;
	u8					   padding1e[8];
	u8					   animation;
	u8					   padding27;
	struct AnimationEntry *frame;
};

void BattleFx_SpawnDescendingArcParticles(struct FieldActor *arc)
{
	struct FieldActor		   *source = arc;
	struct BattleFxScene	   *scene = gEffectWork;
	struct FieldActor		   *leader = scene->main_object;
	struct FieldActor		   *objects[2];
	struct FieldActor		   *object;
	struct DescendingArcSprite *sprite;
	s32							i;
	u8							zero;

	for (i = 0; i <= 1; i++)
	{
		object = Object_CreateFar(26, source->x.fixed, source->y.fixed + 0x400000, source->z.fixed);
		objects[i] = object;
		if (object == NULL)
			continue;
		object->terrain_height = source->terrain_height;
		sprite = object->sprite;
		object->motion_flags = 0;
		object->timer = 0;
		zero = 0;
		object->linked_object = source;
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
		sprite->frame->frame = zero;
	}
	objects[0]->update = (void *)BattleFx_UpdateDescendingParticleNegativeArc;
	((struct DescendingArcSprite *)objects[0]->sprite)->attr.byte.variant = 0;
	objects[1]->update = (void *)BattleFx_UpdateDescendingParticlePositiveArc;
	((struct DescendingArcSprite *)objects[1]->sprite)->attr.byte.variant =
		((struct DescendingArcSprite *)leader->sprite)->attr.byte.variant;
}

//--------------------------------------------------------------------------
// arcstate.c
// エナジー演出 16 (プルーフ)：使う者の動きと、効果が切れたときのメッセージ
//--------------------------------------------------------------------------

#define ENA_PURUFU 150 // プルーフ
#define ITEM_HANE 236  // せいなるはね

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
		UiText_ShowPositionedMessageAndWaitFar((s32)MsgAbilityWoreOff, 1);
		return;
	}
	UiWork_PushValueSlotFar(ITEM_HANE, 2);
	UiText_ShowPositionedMessageAndWaitFar((s32)MsgItemWoreOff, 1);
}

void RunBattleEffect16(void)
{
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
