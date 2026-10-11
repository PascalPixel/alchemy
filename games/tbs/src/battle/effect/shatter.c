//--------------------------------------------------------------------------
// projectile_volley.c
// バトル効果：BattleFx_RunProjectileVolley の型ごとの入り口
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
#include "b5_context.h"
#include "motobj.h"
#include "field_event.h"
#include "battle_effect_work.h"
#include "iwram_call.h"
#include "fixed_math.h"
#include "canvas.h"
#include "mem.h"
#include "resource_ids.h"
#include "resource.h"
#include "battle_efx.h"
#include "btldisp.h"
#include "effect_step.h"
#include "callback_scheduler.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "heap_state.h"
#include "transform.h"
#include "scene.h"
#include "map_scroll.h"
#include "project.h"
#include "global_cells.h"

void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride);

void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

#define ObjectDispatch_ApplyValueToChildrenFar \
	((s32 (*)(s32, s32))ObjectDispatch_ApplyValueToChildrenFar)

#define ARG(e) ((struct BattleEffectArgument *)(e))

#define STEP_HAYAI 0x30 // 絵のアニメの step（ふだんは 0x10）
#define STEP_FUTSU 0x10

void BattleFx_RunMode0(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 0);
}

void BattleFx_RunMode1(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 1);
}

void BattleFx_RunMode2(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 2);
}

void BattleFx_RunMode3(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 3);
}

void BattleFx_RunMode11(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 11);
}

void BattleFx_RunMode4(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 4);
}

//--------------------------------------------------------------------------
// 型 5 : 使う者を mode 2 にし、アニメを速めているあいだに放つ
//--------------------------------------------------------------------------
void BattleEffect_RunMode5WithAction(void *effect)
{
	s32 obj;

	obj = (s32)GetBattleObjectSlotFar(ARG(effect)->actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_HAYAI);
	BattleFx_RunProjectileVolley(effect, 5);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_FUTSU);
}

//--------------------------------------------------------------------------
// 型 6 : 型 5 と同じく、アニメを速めて放つ
//--------------------------------------------------------------------------
void BattleFx_RunMode6WithAction(void *effect)
{
	s32 obj;

	obj = (s32)GetBattleObjectSlotFar(ARG(effect)->actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_HAYAI);
	BattleFx_RunProjectileVolley(effect, 6);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_FUTSU);
}

void BattleFx_RunMode7(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 7);
}

void BattleFx_RunMode10(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 10);
}

//--------------------------------------------------------------------------
// 型 9 : 型 5 と同じく、アニメを速めて放つ
//--------------------------------------------------------------------------
void BattleFx_RunMode9WithAction(void *effect)
{
	s32 obj;

	obj = (s32)GetBattleObjectSlotFar(ARG(effect)->actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_HAYAI);
	BattleFx_RunProjectileVolley(effect, 9);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_FUTSU);
}

//--------------------------------------------------------------------------

// approach.c

// バトル効果：対象へ飛ぶ弾の一斉射撃と、キャラの移動

//--------------------------------------------------------------------------

void ObjectDispatch_InitFromTable6Far(struct MotionObject *);

extern u8  gBattleFxWork[];
extern u16 ParticleStreams_CellOffsets[];

// mode ごとの表
extern u8 Volley_CometHeights[];
extern u8
	Volley_Modes[]; // 5 バイトずつ：弾の数、届くまでのフレーム、弾の間隔、対象の間隔、火花の数
extern u8  Volley_FlashWidths[];
extern u8  Volley_FlashHeights[];
extern u8  Volley_FlashSteps[];
extern u8  Volley_FlashRises[];
extern u16 Volley_FlashOffsets[];
extern u16 Volley_PelletOffsets[];

// 粒の表：0- 当たりの光、100- 火花、512- 火の玉の尾
#define PTCL ((struct EffectStep *)Ram_MapCellBuffer)
#define PTCL_MAX 1024

#define HI(v) (((s16 *)&(v))[1]) // 16.16 の整数部
#define ABS(v) ((v) < 0 ? -(v) : (v))

// 合成の方法
#define CLIP (GOUSEI_CLIP_X | GOUSEI_CLIP_Y)

void BattleFx_PrepareCanvasEffect(
	struct BattleEffectArgument *effect, s32 kind, s32 side, s32 mode, s32 *x, s32 *y);

struct BlitterPair
{
	DrawRectangle upper;
	DrawRectangle lower;
};

//--------------------------------------------------------------------------
// 使う者から対象へ弾を飛ばし、当たったら光と火花を出す
// mode : 弾の絵と飛び方 (0-11)
//--------------------------------------------------------------------------
void BattleFx_RunProjectileVolley(struct BattleEffectArgument *effect, s32 mode)
{
	struct EffectPosition	 position;
	s32						 screen_x;
	s32						 screen_y;
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 i;
	s32						 j;
	s32						 frame;
	struct BlitterPair		 draw;
	struct BattleCamera		*camera;
	u8						*sheet;
	s32						 se_wait;
	s32						 total;
	s32						 kind;
	s32						 shots;
	s32						 steps;
	struct MotionObject		*user;
	s32						 k;
	s32						 shift;
	s32						 hit;
	s32						 x;

	// ワーク、キャンバス、カメラ
	heap = (void **)gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	camera =
		*(struct BattleCamera **)((u8 *)heap
								  - (HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_CAMERA) * sizeof(void *));
	sheet = heap[2];
	se_wait = 0;
	work->effect = effect;
	kind = effect->variant;
	if (mode == 10)
		BattleFx_BeginCanvasLayer(0);
	else
		BattleFx_BeginCanvasLayer(1);
	if (work->effect->prelude == 1)
		BattleFx_PrepareCanvasEffect(effect, 1, work->effect->side, 2, &screen_x, &screen_y);

	// 128x128 の合成。向こう側なら左右を反転する
	if (mode == 5)
	{
		if (work->effect->side == 1)
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
		else
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP, GOUSEI_HIKAKU);
	}
	else
	{
		if (work->effect->side == 1)
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP | GOUSEI_FLIP_X, GOUSEI_KASAN);
		else
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP, GOUSEI_KASAN);
	}
	draw.upper = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];

	// 弾の絵
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	if (mode == 0 || mode == 5 || mode == 8)
	{
		if (mode == 5)
			kind = 2;
		if (mode == 8)
			kind = 0;
		if (kind == 0)
			Resource_LoadAndDecompress((s32)&ResourceId_CometSheetA, work->sheet + 0x1000, 1, 1);
		else if (kind == 1)
			Resource_LoadAndDecompress((s32)&ResourceId_CometSheetB, work->sheet + 0x1000, 1, 1);
		else
			Resource_LoadAndDecompress((s32)&ResourceId_CometSheetC, work->sheet + 0x1000, 1, 1);
		if (mode == 5)
		{
			const void *pal = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
			{
				Iwram_CopyWords((void *)BG_PLTT, pal, 128);
			}
		}
		Resource_LoadAndDecompress((s32)&ResourceId_WindStreakSheet, work->sheet + 0x2000, 1, 0);
		if (mode == 5)
		{
			const void *pal = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
			{
				Iwram_CopyWords((void *)BG_PLTT, pal, 128);
			}
		}
		work->transfer_mode = 2;
		work->transfer_value = 75;
	}
	else if (mode == 1)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_ShurikenSheet, work, 1, 1);
		REG_BLDCNT = 0;
		work->transfer_mode = 1;
		work->transfer_value = 0;
	}
	else if (mode == 2)
	{
		Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_CometSheetA), 128);
		Resource_LoadAndDecompress((s32)&ResourceId_ProjectileVolleyImage, work, 0, 0);
		work->transfer_mode = 2;
		work->transfer_value = 50;
	}
	else if (mode == 3 || mode == 4 || mode == 11)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_YellowSparkSheet, work, 1, 1);
		work->transfer_mode = 2;
		work->transfer_value = 50;
	}
	else if (mode == 6)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_FireballSheet, work, 1, 1);
		work->transfer_mode = 2;
		work->transfer_value = 50;
	}
	else
	{
		Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work, 1, 1);
		Resource_LoadAndDecompress(
			(s32)&ResourceId_MercuryDjinnSmallSheet, work->sheet + 0x65c0, 1, 0);
		work->transfer_mode = 2;
		work->transfer_value = 50;
	}
	Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 弾の出る位置と速さ
	user = GetBattleObjectSlotFar(work->effect->actor)->object;
	shots = Volley_Modes[mode * 5];
	steps = Volley_Modes[mode * 5 + 1];
	{
		s32 stagger = Volley_Modes[mode * 5 + 2];
		s32 delay = Volley_Modes[mode * 5 + 3];
		s32 height = Battle_GetObjectTableValueFar(work->effect->actor);

		if (work->effect->count * shots > 63) // particles[64] に収める
			work->effect->count = 1;

		for (i = 0; i != work->effect->count; i++)
		{
			struct MotionObject *tgt = GetBattleObjectSlotFar(work->effect->actors[i])->object;
			s32 tgt_height = Battle_GetObjectTableValueFar(work->effect->actors[i]);

			for (j = 0; j != shots; j++)
			{
				struct EffectStep *shot = &work->particles[i * shots + j];

				shot->x = user->x;
				if (mode == 7)
					shot->y = user->y + ((Random16() & 15) << 16) + 0x3a0000;
				else if (mode == 10)
					shot->y = user->y + height / 2;
				else if (mode == 6)
					shot->y = user->y + height / 2;
				else if (mode == 9)
					shot->y = user->y + height / 2 + ((16 - (Random16() & 31)) << 16);
				else if ((u32)(mode - 3) <= 1)
					shot->y = user->y + height / 2 + ((16 - (Random16() & 31)) << 16);
				else if (mode == 11)
					shot->y = user->y + height / 2 + ((32 - (Random16() & 63)) << 16);
				else if (mode == 5)
					shot->y = user->y + height / 2;
				else
					shot->y = user->y + height;
				shot->z = user->z;
				shot->velocity_x = (tgt->x - shot->x) / steps;
				if (mode == 7)
					shot->velocity_y =
						(s32)(tgt->y + ((Random16() & 63) << 16) - shot->y - 0xc0000) / steps;
				else if (mode == 8)
					shot->velocity_y =
						(s32)(tgt->y + ((Random16() & 7) << 16) - shot->y + 0x160000) / steps;
				else if (mode == 9)
					shot->velocity_y =
						(s32)(tgt->y + ((64 - (Random16() & 63)) << 16) - shot->y) / steps;
				else if (mode == 10)
					shot->velocity_y =
						(s32)(tgt->y + ((Random16() & 31) << 16) - shot->y + 0x40000) / steps;
				else if ((u32)(mode - 3) <= 1 || mode == 11 || mode == 5)
					shot->velocity_y = 0;
				else if (mode == 6)
					shot->velocity_y =
						(s32)(tgt->y + tgt_height / 2 + ((Random16() & 15) << 16) - shot->y)
						/ steps;
				else
					shot->velocity_y =
						(s32)(tgt->y + tgt_height - ((Random16() & 15) << 16) - shot->y) / steps;
				shot->velocity_z = (tgt->z - shot->z) / steps;
				shot->variant = 0;
			}
		}
		for (i = 0; i != PTCL_MAX; i++)
			PTCL[i].variant = 0;
		if (mode == 6)
			total = work->effect->count * delay + shots * stagger + 32;
		else
			total = work->effect->count * delay + shots * stagger + 16;

		//------------------------------------------------------------------
		// 毎フレームの動きと描画
		//------------------------------------------------------------------
		for (frame = 0; frame != total; frame++)
		{
			s32 index;

			// 結果の再生を始める
			if (se_wait > 0)
				se_wait--;
			if (mode == 6)
			{
				if (frame == 4)
					AudioCommand_PlayFar(136);
				if (frame == 32)
					BattleEventRuntime_BeginPhaseFar(134);
			}
			else if (mode == 7)
			{
				if (frame == 48)
					BattleEventRuntime_BeginPhaseFar(133);
			}
			else if (mode != 5)
			{
				if (frame == 16)
					BattleEventRuntime_BeginPhaseFar(133);
			}

			// 前置き：ジンの小さな絵が円を描き、70 フレームから上へ去る
			if (work->effect->prelude == 1)
			{
				s32 x = (-Trig_Sin(frame << 11) << 2 >> 16) + screen_x / 2 - 10;
				s32 y = (Trig_Cos(frame << 11) << 1 >> 16) + screen_y - 24;

				if (frame > 69)
					y = y - frame * 2 + 138;
				if (work->effect->side == 1)
					BattleEffect_LoadWork(
						HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP | GOUSEI_FLIP_X, GOUSEI_KASAN);
				else
					BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP, GOUSEI_KASAN);
				draw.lower = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
				if (frame <= 3)
					draw.lower(canvas, work->sheet + 0x65c0, x, y, 20, 40);
				draw.lower(canvas, work->sheet + 0x65c0, x, y, 20, 40);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			}

			// 弾
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			index = 0;
			for (i = 0; i != work->effect->count; i++)
			{
				struct MotionObject *tgt = GetBattleObjectSlotFar(work->effect->actors[i])->object;

				for (j = 0; j != shots; j++, index++)
				{
					if (j * stagger + i * delay < frame)
					{
						struct EffectStep	  *shot = &work->particles[i * shots + j];
						struct EffectPosition *pos = &position;

						// 画面の位置 (キャンバスの横は半分)
						EffectPosition_ApplyBaseAndYOffset((s32 *)shot, pos);
						pos->x >>= 1;
						shot->x += shot->velocity_x;
						shot->y += shot->velocity_y;
						shot->z += shot->velocity_z;
						if (mode == 6)
						{
							// 火の玉の尾を二つ出す
							shift = 0;
							for (k = 512; k != PTCL_MAX; k++)
							{
								struct EffectStep *p = &PTCL[k];

								if (p->variant == 0)
								{
									p->x = position.x << 16;
									p->y = position.y << 16;
									p->velocity_x = ((Random16() & 255) - 128) << 11;
									p->velocity_y = ((Random16() & 255) - 128) << 11;
									p->variant = (Random16() & 7) + 32;
									shift++;
									if (shift == 2)
										break;
								}
							}
						}
						if (shot->variant == 0)
						{
							// 対象の側に入り、対象の x を越えたら当たり
							x = shot->x;
							if (tgt->x < 0)
							{
								hit = x < 0;
								/* FAKEMATCH: conditional cancelling updates end CSE
								 * before the hit test, preserving the native sign
								 * result and duplicated side check. Both updates
								 * disappear; removing both guards loses three
								 * English instructions and changes 77 rows. */
								if (tgt)
								{
									hit ^= x;
									hit ^= x;
								}
								if (!hit)
									goto draw_shot;
							}
							else
							{
								hit = x >= 0;
								/* FAKEMATCH: the same CSE block end keeps the
								 * opposite sign result before its side check;
								 * the cancelling updates emit no instructions. */
								if (tgt)
								{
									hit ^= x;
									hit ^= x;
								}
								if (!hit)
									goto draw_shot;
							}
							if (ABS(x) >= ABS(tgt->x))
							{
								s32				   spawned = 0;
								struct EffectStep *flash = &PTCL[index];

								shot->variant = 1;
								if (mode == 5)
								{
									BattleEventRuntime_BeginPhaseFar(134);
								}
								else if (mode != 6 && se_wait == 0)
								{
									se_wait = 8;
									AudioCommand_PlayFar(132);
								}
								if (mode == 2)
								{
									// はね返る
									s32 kick = ((Random16() & 31) + 32) << 12;

									if (Random16() & 1)
										shot->velocity_y += kick;
									else
										shot->velocity_y -= kick;
									shot->velocity_x = -shot->velocity_x;
								}
								flash->variant = 1;
								flash->x = pos->x;
								flash->y = pos->y;
								flash->z = 0;
								if (mode != 7)
									work->shake_frames = 2;
								ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 8);
								if (mode != 7 && mode != 9 && mode != 10)
								{
									if (mode == 5)
										BattleMotion_ApplyVariantMotionFar(
											work->effect->actors[i], 4);
									else
										BattleMotion_ApplyVariantMotionFar(
											work->effect->actors[i], 5);
								}
								// 火花
								shift = mode == 5 ? 12 : 11;
								for (k = 100; k != 512; k++)
								{
									struct EffectStep *p = &PTCL[k];

									if (p->variant == 0)
									{
										p->x = position.x << 16;
										p->y = position.y << 16;
										p->velocity_x = ((Random16() & 255) - 128) << shift;
										p->velocity_y = ((Random16() & 255) - 128) << shift;
										p->variant = (Random16() & 7) + 16;
										spawned++;
										if (spawned == Volley_Modes[mode * 5 + 4])
											break;
									}
								}
							}
						}
					draw_shot:
						if (mode == 0 || mode == 5 || mode == 8)
						{
							// 絵は上半分だけで、下半分は上下を反転して描く
							if (work->effect->side == 1)
								BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
									CLIP | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
							else
								BattleEffect_LoadWork(
									HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP, GOUSEI_HIKAKU);
							((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
								work->sheet + 0x1000, pos->x - 16,
								pos->y - Volley_CometHeights[kind], 32, Volley_CometHeights[kind]);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
							if (work->effect->side == 1)
								BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
									CLIP | GOUSEI_FLIP, GOUSEI_HIKAKU);
							else
								BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
									CLIP | GOUSEI_FLIP_Y, GOUSEI_HIKAKU);
							((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
								work->sheet + 0x1000, pos->x - 16, pos->y, 32,
								Volley_CometHeights[kind]);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
						}
						else if (mode == 1)
						{
							if (ABS(shot->x) <= ABS(tgt->x))
								draw.upper(canvas, work->sheet + frame % 6 * 768, pos->x - 16,
									pos->y - 12, 32, 24);
						}
						else if (mode == 7 || mode == 9 || mode == 10)
						{
							s32 cell = j & 3;
							draw.upper(canvas, work->sheet + Volley_PelletOffsets[cell], pos->x - 4,
								pos->y - 4, 8, 8);
						}
						else if (mode == 2)
						{
							draw.upper(
								canvas, work->sheet + j % 6 * 128, pos->x - 4, pos->y - 8, 8, 16);
						}
						else if (mode == 3)
						{
							draw.upper(canvas, work->sheet + 96, pos->x - 9, pos->y - 7, 18, 13);
						}
						else if (mode == 4)
						{
							draw.upper(canvas, work, pos->x - 6, pos->y - 4, 12, 8);
						}
						else if (mode == 11)
						{
							draw.upper(canvas, work->sheet + 330, pos->x - 15, pos->y - 12, 29, 23);
						}
						else
						{
							draw.upper(canvas, work, pos->x - 20, pos->y - 32, 40, 64);
						}
					}
				}
			}

			// 当たりの光：12 フレーム。半分の絵を、反転してもう半分描く (奇数は縦横を入れ替えた絵)
			if (mode == 0 || mode == 5 || mode == 8)
			{
				for (i = 0; i != work->effect->count * shots; i++)
				{
					struct EffectStep *flash = &PTCL[i];

					if (flash->variant == 1)
					{
						s32 step = flash->z / 2;

						if (!(i & 1))
						{
							BattleEffect_LoadWork(
								HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP, GOUSEI_HIKAKU);
							((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
								work->sheet + Volley_FlashOffsets[step],
								flash->x - Volley_FlashWidths[step],
								flash->y - Volley_FlashRises[step], Volley_FlashWidths[step],
								Volley_FlashHeights[step]);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
							BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
								CLIP | GOUSEI_FLIP, GOUSEI_HIKAKU);
							((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
								work->sheet + Volley_FlashOffsets[step], flash->x,
								flash->y + Volley_FlashSteps[step] - Volley_FlashRises[step],
								Volley_FlashWidths[step], Volley_FlashHeights[step]);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
						}
						else
						{
							BattleEffect_LoadWork(
								HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP, GOUSEI_HIKAKU);
							((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
								work->sheet + Volley_FlashOffsets[step] + 0x128a,
								flash->x - Volley_FlashRises[step],
								flash->y - Volley_FlashWidths[step], Volley_FlashHeights[step],
								Volley_FlashWidths[step]);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
							BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
								CLIP | GOUSEI_FLIP, GOUSEI_HIKAKU);
							((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
								work->sheet + Volley_FlashOffsets[step] + 0x128a,
								flash->x + Volley_FlashSteps[step] - Volley_FlashRises[step],
								flash->y, Volley_FlashHeights[step], Volley_FlashWidths[step]);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
						}
						flash->z++;
						if (flash->z == 12)
							flash->variant = 0;
					}
				}
			}

			// 火花：落ちて、下 (y 112) ではね返る
			for (i = 100; i != 512; i++)
			{
				struct EffectStep *p = &PTCL[i];

				if (p->variant > 0)
				{
					s32 size = (p->variant >> 3) + 1;

					draw.upper(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
						HI(p->x) - size / 2, HI(p->y) - size, size, size * 2);
					EffectStep_AdvanceWithGravity2D(p, 60, 0x1000);
					if (p->y > 0x700000)
						p->velocity_y = -p->velocity_y / 2;
					p->variant--;
				}
			}

			// 火の玉の尾：上へ昇る
			for (i = 512; i != PTCL_MAX; i++)
			{
				struct EffectStep *p = &PTCL[i];

				if (p->variant > 0)
				{
					s32 size = (p->variant >> 4) + 1;

					draw.upper(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
						HI(p->x) - size / 2, HI(p->y) - size, size, size * 2);
					EffectStep_AdvanceWithGravity2D(p, 60, -0x4000);
					p->variant--;
				}
			}
			Camera_ApplyShake(4, 4);
			ObjectGroup_TickMemberTimers();
			work->transfer_pending = 1;
			WaitFrames(1);
		}
	}

	Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// キャラを、上へはずみをつけて元の位置 (anchor) へ戻す
//--------------------------------------------------------------------------
void BattleFx_InitializeRisingObject(s32 id)
{
	struct BtlObj		*slot = GetBattleObjectSlotFar(id);
	struct MotionObject *obj = slot->object;

	obj->acceleration = 0x20000;
	obj->speed_limit = 0x80000; // 8 ドット/フレーム
	obj->velocity_y = 0x40000;
	obj->vertical_motion_strength = 0xab85;
	obj->vertical_motion_phase = 0;
	obj->auto_face_motion = 0;
	obj->snap_to_target = 1;
	ObjectDispatch_InitFromTable6Far(obj);
	Object_SetMoveTargetFar(obj, slot->anchor_x, 0, slot->anchor_z);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
}

//--------------------------------------------------------------------------
// キャラを、相手までの 8 割の位置へ frames フレームで動かす
//--------------------------------------------------------------------------
void BattleFx_SetApproachMotion(s32 id, s32 tgt_id, s32 frames)
{
	struct BtlObj		*slot = GetBattleObjectSlotFar(id);
	struct BtlObj		*tgt_slot = GetBattleObjectSlotFar(tgt_id);
	struct MotionObject *obj = slot->object;
	struct MotionObject *tgt = tgt_slot->object;
	s32					 rate = 80; // %
	s32					 diff_x = tgt->x - obj->x;
	s32					 start_x = obj->x;
	s32					 dx = rate * diff_x / 100;
	s32					 diff_z = tgt->z - obj->z;
	s32					 start_z = obj->z;
	s32					 dz = rate * diff_z / 100;
	s32					 x = start_x + dx;
	s32					 z = start_z + dz;
	s32					 sx = dx >> 8; // 1/256 ドットで距離を求める
	s32					 sz = dz >> 8;
	s32					 dist;

	dist = Iwram_Sqrt(sx * sx + sz * sz);
	dist = (dist << 8) / frames;
	obj->acceleration = dist;
	obj->speed_limit = dist;
	obj->snap_to_target = 1;
	obj->vertical_motion_strength = 0xab85;
	obj->velocity_y = 0;
	obj->vertical_motion_phase = 0;
	obj->auto_face_motion = 1;
	ObjectDispatch_InitFromTable6Far(obj);
	Object_SetMoveTargetFar(obj, x, 0, z);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
}

//--------------------------------------------------------------------------

// pack_rows.c

// タイル：横 2 バイトを 1 バイトに詰める

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 1 行 40 バイトを 288 行、横に 2 バイトずつ 1 バイトに詰めて写す
// （あとの奇数列のバイトが残る）
// stride : src の 1 行のバイト数（dst はその半分）
//--------------------------------------------------------------------------
void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride)
{
	s32 y = 0;
	s32 h = 288;
	s32 ofs = 0;

	do
	{
		s32 x;

		for (x = 0; x != 40; x++)
		{
			dst[ofs / 2 + x / 2] = src[ofs + x];
		}
		ofs += stride;
		y++;
	} while (y != h);
}

//--------------------------------------------------------------------------

// burst.c

// バトル効果：相手へ寄って当て、輪と粒を飛び散らせる (色の違う四種)

//--------------------------------------------------------------------------

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

#define TSUBU_MAX 64	// 粒の数
#define RING_SIZE 0x3c0 // 輪の一枚 (20x48)
#define Graphics_PackTileRows ((void (*)(void *source, void *destination, s32 width, s32 rows))Graphics_PackTileRows)

//--------------------------------------------------------------------------
// 四種の入口
//--------------------------------------------------------------------------
void BattleFx_RunParticleFieldVariant0(s32 effect)
{
	BattleFx_RunParticleFieldVariant((struct BattleEffectArgument *)effect, 0);
}

void BattleFx_RunParticleFieldVariant1(s32 effect)
{
	BattleFx_RunParticleFieldVariant((struct BattleEffectArgument *)effect, 1);
}

void BattleFx_RunParticleFieldVariant2(s32 effect)
{
	BattleFx_RunParticleFieldVariant((struct BattleEffectArgument *)effect, 2);
}

void BattleFx_RunParticleFieldVariant3(s32 effect)
{
	BattleFx_RunParticleFieldVariant((struct BattleEffectArgument *)effect, 3);
}

//--------------------------------------------------------------------------
// 使う者が最初の対象へ寄り、10 フレーム目に当てる
// 対象の位置に輪を出し、粒を 64 個飛び散らせる。全部で 60 フレーム
// variant : 色 (パレットの元の絵 0-3)
//--------------------------------------------------------------------------
void BattleFx_RunParticleFieldVariant(struct BattleEffectArgument *effect, s32 variant)
{
	/* FAKEMATCH: needed to match: the blitter/camera declarations and independent store order preserve spill slots and loop preheader order */
	void					**heap;
	void					**cursor;
	struct BattleEffectWork	 *work;
	void					 *canvas;
	BattleEffectDrawRectangle draw[2];
	struct BattleCamera		 *camera;
	u8						 *sheet;
	struct EffectStep		 *p;
	struct EffectStep		 *step;
	struct EffectPosition	  origin;
	struct EffectPosition	  screen;
	struct EffectPosition	  pos;
	struct MotionObject		 *tgt;
	s32						  frame;
	s32						  n;
	s32						  size;
	s32						  t;
	s32						  pal;
	u8						 *cells;

	// ワーク、キャンバス、カメラ
	cells = Ram_MapCellBuffer;
	heap = (void **)&gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	camera =
		*(struct BattleCamera **)((u8 *)heap
								  - (HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_CAMERA) * sizeof(void *));
	sheet = heap[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);

	// 上半分と、上下を反転した下半分。向こう側なら左右も反転する
	if (work->effect->side == 0)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP, GOUSEI_HIKAKU);
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP | GOUSEI_FLIP_Y, GOUSEI_HIKAKU);
	}
	else
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP | GOUSEI_FLIP, GOUSEI_HIKAKU);
	}
	draw[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	draw[1] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];

	// 絵と色
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1, 0);
	Graphics_PackTileRows(work, cells, 40, 288);
	Resource_LoadAndDecompress((s32)&ResourceId_BlueBurstSheet, work->sheet, 1, 1);
	switch (variant)
	{
	case 0:
		pal = (s32)&ResourceId_FirePillarSheetB;
		break;
	case 1:
		pal = (s32)&ResourceId_IceBlockSheet;
		break;
	case 2:
		pal = (s32)&ResourceId_PinkBurstSheet;
		break;
	default:
		pal = (s32)&ResourceId_BlastSheet;
		break;
	}
	Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(pal), 128);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 使う者を寄せ、対象の位置に粒を用意する (x は画面の中央の側へ飛ぶ)
	BattleFx_SetApproachMotion(work->effect->actor, work->effect->actors[0], 10);
	tgt = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (n = 0; n != TSUBU_MAX; n++)
	{
		step = &work->particles[n];
		step->x = tgt->x;
		step->y = tgt->y + 0xa0000; // 10 ドット上
		step->z = tgt->z;
		step->velocity_x = (Random16() & 0x1ff) << 11;
		step->velocity_y = ((Random16() & 255) - 64) << 11;
		step->velocity_z = ((Random16() & 255) - 128) << 11;
		if (step->x > 0)
			step->velocity_x = -step->velocity_x;
		step->variant = n / 2 + 16; // 残りのフレーム
	}
	EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &origin);

	for (frame = 0; frame != 60; frame++)
	{
		// 15 フレームのあいだ、使う者の位置に絵を上下二つに分けて描く
		if (frame <= 14)
		{
			EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &screen);
			draw[0](canvas, work, screen.x / 2 - 16, screen.y - 48, 40, 32);
			draw[1](canvas, work, screen.x / 2 - 16, screen.y - 16, 40, 32);
		}
		// 当たり
		if (frame == 10)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
			BattleEventRuntime_BeginPhaseFar(134);
			work->shake_frames = 8;
		}
		// 輪：8 フレーム目から 2 フレームずつ 6 枚
		t = frame - 8;
		if (t >= 0 && t <= 11)
		{
			s32 ring = t / 2;
			draw[0](canvas, cells + ring * RING_SIZE, origin.x / 2 - 16, screen.y - 40, 20, 48);
		}
		// 粒
		if (t >= 0 && t <= 55)
		{
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			for (n = 0; n != TSUBU_MAX; n++)
			{
				p = &work->particles[n];
				size = p->variant;
				if (size > 0)
				{
					EffectPosition_ApplyBaseAndYOffset(&p->x, &pos);
					size >>= 4;
					size += 2;
					pos.x >>= 1;
					draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1], pos.x - size / 2,
						pos.y - size, size, size * 2);
					EffectStep_AdvanceWithGravity3D(p, 60, -0x200);
					p->variant--;
				}
			}
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

#undef Graphics_PackTileRows

//--------------------------------------------------------------------------

// transpose_copy.c

// タイル：バイトの絵を右に 90 度回して写す

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 幅 w・高さ h の src を、幅 h・高さ w の dst に右へ 90 度回して写す
// （src のいちばん上の行が dst のいちばん右の列になる）
//--------------------------------------------------------------------------
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h)
{
	s32		  y;
	s32		  x;
	s32		  col;
	s32		  ofs;
	u8		 *d;
	const u8 *s;

	y = 0;
	if (h != 0)
	{
		ofs = 0;
		col = h;
		do
		{
			x = 0;
			if (w != 0)
			{
				d = (u8 *)(col + (u32)dst - 1);
				s = (const u8 *)(ofs + (u32)src);
				do
				{
					x++;
					*d = *s;
					s++;
					d += h;
				} while (x != w);
			}
			y++;
			ofs += w;
			col--;
		} while (y != h);
	}
}

//--------------------------------------------------------------------------

// djinn_bursts.c

// バトル演出：水星と木星のジンが回りながら弾を放つ

//--------------------------------------------------------------------------

void AudioCommand_PlayFar(s32 cue);
#define MOTE(n) (&((struct EffectStep *)Ram_MapCellBuffer)[n]) // 破片の粒 (最大 512)

//--------------------------------------------------------------------------
// 水星のジン：(96, 60) のまわりを渦を巻いて回りながら、青い炎を 5 発左へ撃つ
// 炎は 5 フレーム進むと 32 個の粒に砕ける
//--------------------------------------------------------------------------
void BattleFx_RunMercuryDjinnFlames(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	u8						*sheet;
	DrawRectangle			 blit[2];
	DrawRectangle			*draw;
	struct EffectStep		*flame;
	struct EffectStep		*mote;
	u8						*cells;
	s32						 i;
	s32						 j;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	draw = blit;
	BattleFx_FetchRectangleBlitters(0, draw);

	// 炎の絵は BlueFlame を転置して写す
	cells = Ram_MapCellBuffer;
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, cells, 1, 1);
	Graphics_TransposeCopy(cells, work->sheet + 0x2a8, 17, 104);
	Graphics_TransposeCopy(cells + 0x6e8, work->sheet + 0x990, 34, 65);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	for (i = 0; i != 8; i++)
		work->particles[i].variant = -1;
	for (i = 0; i != 512; i++)
		MOTE(i)->variant = -1;
	AudioCommand_PlayFar(162);
	frame = 0;
	do
	{
		if (frame == 56)
			BattleEventRuntime_BeginPhaseFar(133);

		// 炎：1 フレームに 12 ドット左へ進み、5 フレーム目で砕ける
		for (i = 0; i != 5; i++)
		{
			flame = &work->particles[i];
			if (flame->variant != -1)
			{
				blit[0](canvas, work->sheet + 0x990, flame->x - 16, flame->y - 17, 65, 34);
				flame->x -= 12;
				flame->variant++;
				if (flame->variant == 5)
				{
					AudioCommand_PlayFar(133);
					work->shake_frames = 4;
					for (j = 0; j != 32; j++)
					{
						s32 dir;
						s32 speed;

						mote = MOTE(i * 32 + j);
						dir = Random16() & 0xffff;
						speed = (Random16() & 0x1ff) + 256;
						mote->x = flame->x << 16;
						mote->y = flame->y << 16;
						mote->velocity_x = (Trig_Sin(dir) * speed) >> 8;
						mote->velocity_y = (Trig_Cos(dir) * speed) >> 7;
						mote->variant = (Random16() & 15) + 32; // 寿命 32〜47 フレーム
					}
				}
			}
		}

		// ジン：32 フレームで 1 周する。半径 64 - 2 × frame (横はその半分) は
		// 32 フレームで 0 になって中心を通り、その後は反対側へ広がる
		if (frame <= 95)
		{
			s32 x = ((Trig_Sin(frame << 11) * (64 - frame * 2)) >> 17) + 96;
			s32 y = ((Trig_Cos(frame << 11) * (64 - frame * 2)) >> 16) + 60;

			draw[1](canvas, work, x - 10, y - 17, 20, 34);
			for (i = 0; i != 5; i++)
			{
				// 決まったフレームに炎を撃ち、その 6 フレーム後に対象のパレットを
				// 6 フレーム 7 番に替えてアニメ 5 にし、動き 6 を与える
				if (frame == MercuryDjinnFlames_LaunchFrames[i])
				{
					struct EffectStep *shot = &work->particles[i];

					if (shot->variant == -1)
					{
						shot->x = x - 8;
						shot->velocity_x = x;
						shot->y = y;
						shot->variant = 0;
						break;
					}
				}
				if (frame == MercuryDjinnFlames_LaunchFrames[i] + 6)
				{
					for (j = 0; j != work->effect->count; j++)
					{
						ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 6);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
					}
				}
			}
		}

		// 粒：寿命に応じて小さくなり、落ちながら減速する
		for (i = 0; i != 256; i++)
		{
			mote = MOTE(i);
			if (mote->variant != -1)
			{
				s32 size = mote->variant / 16 + 2;

				draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
				EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
				mote->variant--;
			}
		}
		Camera_ApplyShake(4, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	} while (frame != 96);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 木星のジン：回りながら、右上から左下へ火の玉を 8 つ落とす
// 火の玉は y が 95 を越えると 32 個の粒に砕ける
//--------------------------------------------------------------------------
void BattleFx_RunJupiterDjinnFall(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	u8						*sheet;
	DrawRectangle			 blit[2];
	DrawRectangle			*draw;
	struct EffectStep		*ball;
	struct EffectStep		*mote;
	s32						 frame;
	s32						 i;
	s32						 j;
	s32						 angle;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	draw = blit;
	BattleFx_FetchRectangleBlitters(0, draw);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet, work->sheet + 0x320, 1, 0);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 火の玉の出だし：x 64〜127、y -80〜-17
	for (i = 0; i != 32; i++)
	{
		ball = &work->particles[i];
		ball->x = (Random16() & 63) + 64;
		ball->y = (Random16() & 63) - 80;
	}
	for (i = 0; i != 512; i++)
		MOTE(i)->variant = -1;
	AudioCommand_PlayFar(171);
	frame = 0;
	angle = 0x8000; // 180°
	do
	{
		if (frame == 56)
			BattleEventRuntime_BeginPhaseFar(133);

		// ジン：1 フレームに 11.25° 回る。半径 64 - 2 × frame (横はその半分) は
		// 32 フレームで 0 になって中心を通り、その後は反対側へ広がる
		if (frame <= 95)
		{
			s32 x = (Trig_Sin(angle) * (64 - frame * 2)) >> 17;
			s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

			blit[0](canvas, work, x + 86, y + 28, 20, 40);
		}

		// 火の玉：4 フレームおきに 1 つずつ、1 フレームに左へ 6、下へ 12 ドット
		for (i = 0; i != 8; i++)
		{
			ball = &work->particles[i];
			if (frame >= i * 4 + 8 && ball->y <= 95)
			{
				blit[0](canvas, work->sheet + 0x320, ball->x - 20, ball->y - 32, 40, 64);
				ball->x -= 6;
				ball->y += 12;
				if (ball->y > 95)
				{
					for (j = 0; j != 32; j++)
					{
						s32 dir;
						s32 speed;

						mote = MOTE(i * 32 + j);
						dir = Random16() & 0xffff;
						speed = (Random16() & 0x1ff) + 256;
						mote->x = ball->x << 16;
						mote->y = ball->y << 16;
						mote->velocity_x = (Trig_Sin(dir) * speed) >> 7;
						mote->velocity_y = (Trig_Cos(dir) * speed) >> 6;
						mote->variant = (Random16() & 15) + 32; // 寿命 32〜47 フレーム
					}
					AudioCommand_PlayFar(133);
					work->shake_frames = 4;
					for (j = 0; j != work->effect->count; j++)
					{
						ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 6);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
					}
				}
			}
		}

		// 粒：寿命に応じて小さくなり、落ちながら減速する
		for (i = 0; i != 512; i++)
		{
			mote = MOTE(i);
			if (mote->variant != -1)
			{
				s32 size = mote->variant / 16 + 1;

				draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
				EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
				mote->variant--;
			}
		}
		Camera_ApplyShake(4, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		angle -= 0x800; // 11.25°
		frame++;
	} while (frame != 96);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------

// load_and_decompress.c

// リソース：パレットつきのリソースを展開する

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// リソース id を dst に展開する。先頭 128 バイトがパレットのとき、
// copy_pal なら BG パレットの先頭に写し、skip_pal ならとばして展開する
//--------------------------------------------------------------------------
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skip_pal, s32 copy_pal)
{
	u8 *res = Resource_GetTableEntry(id);

	if (copy_pal != 0)
	{
		/* FAKEMATCH: the routine is called through a local pointer to its fixed entry; a plain call matches The Broken Seal through its IWRAM call layer but not The Lost Age, whose header calls the entry directly (all six of its editions differ). */
		s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

		copy((void *)0x05000000, res, 128);
	}
	if (skip_pal != 0)
		res += 128;
	Resource_DecodeType01(res, dst);
}

#define EffectPosition_ApplyAlternateStepAndYOffset \
	((void (*)(s16, s32 *))EffectPosition_ApplyAlternateStepAndYOffset)

void BattleFx_RunVenusDjinnColumns(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	DrawRectangle			 draw[2];
	u8						*sheet;
	struct EffectStep		*step;
	struct EffectStep		*mote;
	s32						 i;
	s32						 j;
	s32						 angle;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 11, 2);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
	draw[0] = heap_cache[7];
	draw[1] = heap_cache[8];
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, work->sheet + 0x2f8, 1, 0);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	for (i = 0; i != 32; i++)
	{
		step = &work->particles[i];
		step->x = Random16() & 63;
		step->y = 104;
	}
	for (i = 0; i != 512; i++)
		((struct EffectStep *)Ram_MapCellBuffer)[i].variant = -1;
	AudioCommand_PlayFar(141);
	frame = 0;
	angle = 0x8000;
	do
	{
		if (frame <= 79)
		{
			s32 x = (Trig_Sin(angle) * 24) >> 16;
			s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

			draw[1](canvas, work, x + 22, y + 29, 20, 38);
		}
		if (frame == 56)
			BattleEventRuntime_BeginPhaseFar(133);
		for (i = 0; i != 10; i++)
		{
			step = &work->particles[i];
			if (frame >= i * 4 + 16)
			{
				draw[0](canvas, work->sheet + 0x9e0, step->x - 17, step->y - 32, 34, 65);
				if (frame == i * 4 + 16)
				{
					for (j = 0; j != 16; j++)
					{
						s32 direction;
						s32 speed;

						mote = &((struct EffectStep *)Ram_MapCellBuffer)[i * 32 + j];
						direction = (Random16() & 0x7fff) + 0x4000;
						speed = (Random16() & 0x1ff) + 256;
						mote->x = step->x << 16;
						mote->y = (step->y + 16) << 16;
						mote->velocity_x = (Trig_Sin(direction) * speed) >> 7;
						mote->velocity_y = (Trig_Cos(direction) * speed) >> 6;
						mote->variant = (Random16() & 15) + 32;
					}
					if (i & 1)
						AudioCommand_PlayFar(133);
					work->shake_frames = 4;
					for (j = 0; j != work->effect->count; j++)
					{
						ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 6);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
					}
				}
				step->y -= 12;
			}
		}
		for (i = 0; i != 512; i++)
		{
			mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];
			if (mote->variant != -1)
			{
				s32 size = mote->variant / 16 + 2;

				draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
				EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
				mote->variant--;
			}
		}
		Camera_ApplyShake(4, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		angle -= 0x800;
		frame++;
	} while (frame != 96);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunMarsDjinnPuffs(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	u8						*sheet;
	DrawRectangle			 callbacks[2];
	DrawRectangle			*draw;
	struct EffectStep		*step;
	struct EffectStep		*mote;
	s32						 member;
	s32						 frame;
	s32						 i;
	s32						 curtain_y;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	draw = callbacks;
	BattleFx_FetchRectangleBlitters(0, draw);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet + 0x320, 1, 1);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	member = 0;
	step = work->particles;
	do
	{
		s32 angle = member << 11;

		step->x = (Trig_Sin(angle) * 24) >> 16;
		step->y = ((Trig_Cos(angle) * 4) >> 16) + 52;
		if (member & 1)
			step->x = 32 - step->x;
		else
			step->x += 32;
		step->variant = -(member * 2);
		for (i = 0; i != 16; i++)
		{
			mote = &((struct EffectStep *)Ram_MapCellBuffer)[member * 16 + i];
			mote->x = (((Random16() & 15) + step->x) - 8) << 16;
			mote->y = ((Random16() & 7) + 96) << 16;
			mote->velocity_x = ((Random16() & 127) - 64) << 11;
			mote->velocity_y = ((Random16() & 127) - 64) << 10;
			mote->z = Random16() & 0xffff;
			mote->velocity_z = Random16() & 0xffff;
		}
		step++;
		member++;
	} while (member != 9);

	AudioCommand_PlayFar(0x88);
	frame = 0;
	curtain_y = -172;
	do
	{
		if (frame == 56)
			BattleEventRuntime_BeginPhaseFar(0x85);
		if (frame <= 23)
			callbacks[0](canvas, work->sheet + 0x320 + (frame / 4) * 0x640, 40, 20, 40, 40);
		if (frame == 20)
			Iwram_CopyWords(
				(void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
		if (frame >= 20 && frame <= 31)
		{
			if (frame > 23)
				draw[1](canvas, work->sheet, 146 - frame * 4, curtain_y, 20, 40);
			else
				callbacks[0](canvas, work->sheet, 50, 20, 20, 40);
		}
		if (frame == 32)
		{
			AudioCommand_PlayFar(0x91);
			work->shake_frames = 8;
			Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1, 1);
		}
		if (frame > 31)
		{
			member = 0;
			step = work->particles;
			do
			{
				if (step->variant >= 0 && step->variant <= 47)
				{
					s32 cell = step->variant / 8;
					u32 width;

					callbacks[0](canvas, work->sheet + PuffArc_CellSourceOffsets[cell],
						step->x - ((width = PuffArc_CellWidths[cell]) >> 1),
						step->y + PuffArc_CellBiasY[cell], width, PuffArc_CellHeights[cell]);
				}
				member++;
				step->variant++;
				step++;
			} while (member != 9);
		}
		step = (struct EffectStep *)Ram_MapCellBuffer;
		member = 0;
		do
		{
			if (frame >= (member / 16) * 2 + 40)
			{
				s32 size;
				s32 x;
				s32 angle;

				size = (member & 1) + 3;
				x = HI(step->x) + ((Trig_Sin(step->z) * 4) >> 16);
				draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1], x - ((u32)size >> 1),
					HI(step->y) - size, size, size * 2);
				EffectStep_AdvanceWithGravity2D(step, 64, -0x2000);
				angle = step->z;
				step->z = angle + 0x800;
				if (step->z > 0xffff)
					step->z = angle + 0x800 - 0xffff;
			}
			member++;
			step++;
		} while (member != 144);

		if (frame == 38)
		{
			for (member = 0; member != work->effect->count; member++)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, 16);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[member], 6);
			}
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		curtain_y += 8;
		frame++;
	} while (frame != 112);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

extern struct EffectStep gMapCellBuffer[];
#define PARTICLE_COUNT 64

void BattleFx_PrepareCanvasEffect(
	struct BattleEffectArgument *effect, s32 kind, s32 side, s32 anchor, s32 *out_x, s32 *out_y);

void BattleFx_RunParticleReveal(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 spawn[3];
	s32						 screen_y;
	s32						 screen_x;
	DrawRectangleFn			 routine[2];
	struct EffectStep		*p;
	s32						 i;
	s32						 frame;
	s32						 rise;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	BattleFx_PrepareCanvasEffect(effect, 1, work->effect->side, 2, &screen_x, &screen_y);
	BattleFx_FetchRectangleBlitters(work->effect->side, routine);
	Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1, 1);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], spawn);

	for (i = 0; i != PARTICLE_COUNT; i++)
	{
		s32 angle;
		s32 speed;

		angle = (Random16() & 0x7fff) + 0x4000;
		speed = (Random16() & 0x1ff) + 0x80;
		gMapCellBuffer[i].x = ((spawn[0] / 2 + (Random16() & 0xf)) - 8) << 16;
		gMapCellBuffer[i].y = (spawn[1] + 8) << 16;
		gMapCellBuffer[i].velocity_x = (Trig_Sin(angle) * speed) >> 9;
		gMapCellBuffer[i].velocity_y = (Trig_Cos(angle) * speed) >> 6;
		gMapCellBuffer[i].z = Random16() & 0x7f;
		gMapCellBuffer[i].velocity_z = Random16() & 0x7f;
		gMapCellBuffer[i].variant = (Random16() & 0xf) + 32;
	}

	for (frame = 0; frame != 64; frame++)
	{
		if (frame > 47)
			REG_BLDALPHA = (64 - frame) | 0x1000;
		if (frame == 1)
		{
			Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet + 0x400, 1, 1);
			Resource_LoadAndDecompress(
				(s32)&ResourceId_MercuryDjinnSmallSheet, work->sheet + 0x65c0, 1, 0);
		}

		if (work->effect->prelude == 1)
		{
			s32 orbit;
			s32 x;
			s32 y;

			orbit = frame << 11;
			x = ((-Trig_Sin(orbit) << 2) >> 16) + screen_x / 2 - 10;
			y = ((Trig_Cos(orbit) << 1) >> 16) + screen_y - 22;
			if (frame > 69)
				y = (y - frame * 2) + 138;
			routine[1](canvas, work->sheet + 0x65c0, x, y, 20, 40);
			if (frame <= 3)
				routine[1](canvas, work->sheet + 0x65c0, x, y, 20, 40);
		}

		for (i = 0, p = gMapCellBuffer; i != PARTICLE_COUNT; i++, p++)
		{
			if (frame >= i / 4 + 4)
			{
				s32 cell;
				s32 w;
				s32 h;

				cell = (p->z / 128) & 3;
				routine[i & 1](canvas, work->sheet + 0x400 + ParticleReveal_CellSourceOffsets[cell],
					HI(p->x) - (w = ParticleReveal_CellWidths[cell]) / 2,
					HI(p->y) - (h = ParticleReveal_CellHeights[cell]) / 2, w, h);
				EffectStep_AdvanceWithGravity3D(p, 63, 0x1000);
			}
		}

		if (frame == 8)
		{
			work->shake_frames = frame;
			BattleEventRuntime_BeginPhaseFar(0x86);
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 16);
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 3);
		}

		rise = frame * 4;
		if (rise > 32)
			rise = 32;
		if (work->effect->side == 0)
		{
			for (i = 0; i != 5; i++)
				routine[0](canvas, work, (i << 5) - (frame / 4 & 31), 120 - rise, 32, 32);
		}
		else
		{
			for (i = 0; i != 5; i++)
				routine[0](canvas, work, ((i << 5) + (frame / 4 & 31)) - 32, 120 - rise, 32, 32);
		}

		Camera_ApplyShake(4, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunVenusDjinnRockToss(struct BattleEffectArgument *effect)
{
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 i;
	s32						 frame;
	DrawRectangle			 draw[2];
	struct BattleCamera		*camera;
	struct MotionObject		*target;
	struct MotionObject		*actor;
	struct EffectPosition	 position;
	s32						 screen_x;
	s32						 screen_y;
	s32						 j;

	work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	BattleFx_PrepareCanvasEffect(effect, 0, work->effect->side, 2, &screen_x, &screen_y);
	REG_BLDALPHA = 0x1010;
	if (work->effect->side == 1)
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 2);
	else
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = (BattleEffectDrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	Resource_LoadAndDecompress((s32)&ResourceId_PortalSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet, work->sheet + 0x65c0, 1, 1);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	actor = GetBattleObjectSlotFar(work->effect->actor)->object;
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (i = 0; i != 8; i++)
	{
		struct EffectStep *rock = &work->particles[i];

		rock->x = actor->x;
		rock->y = 0x420000;
		rock->z = actor->z;
		rock->velocity_x = (i * 0x500000) >> 5;
		rock->velocity_y = (s32)(((Random16() & 127) - 64) << 16) >> 6;
		rock->velocity_z = (s32)(((Random16() & 255) - 127) << 16) >> 5;
		if (rock->x > 0)
			rock->velocity_x = -rock->velocity_x;
		rock->variant = 1;
	}
	for (frame = 0; frame != 96; frame++)
	{
		if (frame > 16)
			BattleFx_StepPaletteToResource((s32)&ResourceId_PortalSheet);
		if (work->effect->prelude == 1)
		{
			s32 angle = frame << 11;
			s32 x = ((-Trig_Sin(angle) << 2) >> 16) + screen_x / 2 - 10;
			s32 y = ((Trig_Cos(angle) << 1) >> 16) + screen_y - 22;

			if (frame > 16)
				y = y - frame * 2 + 32;
			if (work->effect->side == 1)
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 3);
			else
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
			if (frame <= 3)
				((BattleEffectDrawRectangle)((union HeapState *)gWorkSlot)
						->slots[HEAP_SLOT_BLITTER_ALTERNATE])(
					canvas, work->sheet + 0x65c0, x, y, 20, 40);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			draw[0](canvas, work->sheet + 0x65c0, x, y, 20, 40);
		}
		if ((frame & 1) == 0)
		{
			for (i = 0; i != 32; i++)
			{
				struct EffectStep *chip = &work->particles[32 + i];

				chip->velocity_x = Random16() % 6 + 3;
				chip->velocity_y = RockToss_ChipFlips[Random16() & 3];
			}
		}
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		for (i = 0; i != 6; i++)
		{
			struct EffectStep *rock = &work->particles[i];

			if (rock->variant == 1)
			{
				if (frame > i * 2)
				{
					s32 x;
					s32 y;

					EffectPosition_ApplyBaseAndYOffset(&rock->x, &position);
					position.x >>= 1;
					x = position.x - 12;
					y = position.y - 24;
					draw[0](canvas, work, x, y, 24, 48);
					if ((frame & 3) <= 1)
						draw[0](canvas, work->sheet + RockToss_ChipCells[1], x + RockToss_ChipX[1],
							y + RockToss_ChipY[1], RockToss_ChipWidths[1], RockToss_ChipHeights[1]);
					else
						draw[0](canvas, work->sheet + RockToss_ChipCells[2], x + RockToss_ChipX[2],
							y + RockToss_ChipY[2], RockToss_ChipWidths[2], RockToss_ChipHeights[2]);
					for (j = 0; j != 4; j++)
					{
						struct EffectStep *chip = &work->particles[32 + i * 4 + j];
						s32				   chip_x;
						s32				   chip_y;

						BattleEffect_LoadWork(
							HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, chip->velocity_y, 2);
						draw[1] = (BattleEffectDrawRectangle)((union HeapState *)gWorkSlot)
									  ->slots[HEAP_SLOT_BLITTER_ALTERNATE];
						if (chip->velocity_y & 4)
							chip_x = x - RockToss_ChipWidths[chip->velocity_x]
									 - RockToss_ChipX[chip->velocity_x] + 24;
						else
							chip_x = x + RockToss_ChipX[chip->velocity_x];
						if (chip->velocity_y & 8)
							chip_y = y - RockToss_ChipHeights[chip->velocity_x]
									 - RockToss_ChipY[chip->velocity_x] + 48;
						else
							chip_y = y + RockToss_ChipY[chip->velocity_x];
						draw[1](canvas, work->sheet + RockToss_ChipCells[chip->velocity_x], chip_x,
							chip_y, RockToss_ChipWidths[chip->velocity_x],
							RockToss_ChipHeights[chip->velocity_x]);
						Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
					}
					rock->x += rock->velocity_x;
					rock->y += rock->velocity_y;
					rock->z += rock->velocity_z;
				}
				if (frame > i * 2 + 16)
				{
					rock->velocity_x += (target->x - rock->x) >> 8;
					rock->velocity_y += (0x140000 - rock->y) >> 8;
					rock->velocity_z += (target->z - rock->z) >> 8;
					if (frame < i * 2 + 85)
					{
						rock->velocity_x = rock->velocity_x * 60 / 64;
						rock->velocity_y = rock->velocity_y * 60 / 64;
						rock->velocity_z = rock->velocity_z * 60 / 64;
					}
					if (rock->y < 0x140000)
					{
						work->shake_frames = 8;
						rock->variant = 0;
						AudioCommand_PlayFar(134);
						ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
					}
				}
			}
		}
		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

#undef Camera_ApplyShake

#define gBattleFxWork (*(struct BattleEffectWork **)&gBattleFxWork)

void Camera_ApplyShake(s32 random_mask, u32 shake_range)
{
	s32						 display_y;
	s32						 offset_x;
	s32						*remaining_frames;
	s32						 restored_position;
	struct BattleEffectWork *work;
	s32						 offset_y;
	s32						 half_range;
	s32						 random_x;
	struct BgScroll			*display_position;
	struct Projection		*display_center;
	struct BgScroll			*disp_p;
	struct Projection		*center_p;

	work = gBattleFxWork;
	remaining_frames = &work->shake_frames;
	if (*remaining_frames > 0)
	{
		random_x = (random_mask - 1) & Random16();
		half_range = (s32)(shake_range + (shake_range >> 0x1F)) >> 1;
		offset_y = ((shake_range - 1) & Random16()) - half_range;
		display_position = gBgScroll;
		offset_x = random_x - half_range;
		display_y = offset_y + 0x20;
		display_position[1].x = offset_x;
		display_position[1].y = display_y;
		display_center = &gProjection;
		display_center->center_x = 0x78 - offset_x;
		display_center->center_y = 0x78 - offset_y;
		*remaining_frames -= 1;
		return;
	}
	restored_position = work->saved_bg1_x;
	disp_p = gBgScroll;
	disp_p[1].x = restored_position;
	restored_position = work->saved_bg1_y;
	disp_p[1].y = restored_position;
	center_p = &gProjection;
	center_p->center_x = 0x78;
	center_p->center_y = 0x78;
}

#undef gBattleFxWork
#define gWorkSlot (*(u8(*)[]) & gWorkSlot)

extern s16 Mode12_Points[][2];
extern u8  Mode12_Animations[];
extern u8  Mode12_SmokeFlips[];

struct Point
{
	s32 x;
	s32 y;
	s32 z;
};

#define PARTICLES ((struct EffectStep *)(Ram_MapCellBuffer + 0xc58))
#define SLOT(n) (((DrawRectangle *)gWorkSlot)[n])

void BattleFx_InitializeMode12(struct BattleEffectArgument *efx)
{
	u8						 ramp[32];
	s32						 vec[3];
	struct Point			 pts[11];
	s32						 pos[3];
	s32						 scale[3];
	void				   **slots;
	s32						*ctrl;
	struct BattleEffectWork *work;
	void					*dst;
	u8						*aux;
	s32						 frame;
	DrawRectangle			 blit[2];
	s32						 size;
	s32						 count;
	s32						 i;
	s32						 j;
	s32						 n;
	struct EffectStep *p;
	DrawRectangle	  *cache;
	s32				   rings = 0x2710;

	slots = (void **)(gWorkSlot + 44 * 4);
	ctrl = slots[0];
	work = slots[-5];
	dst = slots[-4];
	aux = slots[-3];
	work->effect = efx;
	BattleFx_BeginCanvasLayer(0x2000);
	*(u16 *)0x04000020 = 0x100;
	Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesC, work->sheet + 0x1800, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, aux, 0, 0);

	{
		s32 offset;
		for (i = 0, offset = 0; i != 8; i++)
		{
			u8 *src;
			for (j = 0, src = aux; j != 0x302; j++)
			{
				s32 v = *src++;

				if (v > 64 - i * 7)
				{
					v = 64 - i * 7;
				}
				if (v < 0)
				{
					v = 0;
				}
				(work->sheet + 10000)[offset + j] = v;
			}
			offset += j;
		}
	}

	BattlePres_ConfigureEffectDisplay();
	*(u16 *)0x05000000 = 0;
	*(u16 *)0x05000002 = 0;
	*(u16 *)0x0400000c = 0x2784;
	work->scroll_timer = 0;
	work->scroll_interval = 2;
	work->scroll_step_x = 1;
	work->scroll_step_y = 0;
	ctrl[4] = 1;
	Scheduler_AddOrUpdateCallback((s32)BattleFx_AdvanceScrollOnInterval, 0x480);
	Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	work->transfer_mode = 0;
	BattleEffect_WipeCanvas(0, 0);
	BattleBackground_LoadFar(1, (s32)&ResourceId_DuskCloudsBackdrop, 0);
	gProjection.center_y = 240;
	BattleEffect_WipeCanvas(0, 1);
	*(u16 *)0x04000020 = 0x80;
	BattleEffect_LoadWork(46, 7, 7, 3, 3);
	cache = (DrawRectangle *)gWorkSlot;
	blit[0] = cache[46];
	*(u16 *)0x04000000 = 0x7741;
	*(u16 *)0x04000020 = 0x80;
	*(u16 *)0x04000052 = 0x100f;
	*(u16 *)0x04000050 = 0x3f44;
	work->transfer_mode = 2;
	work->transfer_value = 75;

	for (i = 0; i != 16; i++)
	{
		ramp[i] = i * 8 + 3;
		ramp[31 - i] = i * 8 + 3;
		if (ramp[i] > 63)
		{
			ramp[i] = 63;
		}
		if (ramp[31 - i] > 63)
		{
			ramp[31 - i] = 63;
		}
	}
	*(u16 *)0x0400000c = 0x784;

	for (frame = 0; frame != 170; frame++)
	{
		size = 2;
		count = frame * 2;
		if (frame == 16)
		{
			AudioCommand_PlayFar(140);
		}
		if (frame == 132)
		{
			AudioCommand_PlayFar(131);
		}
		if (frame == 151)
		{
			AudioCommand_PlayFar(145);
		}
		if (gKeysRepeat & (KEY_A | KEY_B))
		{
			break;
		}
		Graphics_UpdatePhasePalette(frame * 2, 0xaaab, 0x5555, 0);
		if (frame == 150)
		{
			work->transfer_mode = 1;
			work->transfer_value = 0x1a1a1a1a;
		}
		else
		{
			work->transfer_mode = 2;
			work->transfer_value = 75;
		}
		Camera_StoreSceneParameters(0x1fe0000, Iwram_RatioMulQ14(0x1fe0000, 0xc000), 0x7fff0000);
		Render_ResetTransformState();
		if (frame > 128)
		{
			s32 a = frame - 128;
			if (a > 22)
			{
				a = 20;
			}
			pos[0] = 0;
			pos[1] = -a << 17;
			pos[2] = 0x2000000;
			size = (a >> 2) + 2;
			if (size > 8)
			{
				size = 8;
			}
			SceneTransform_ApplyPosition(pos);
			SceneTransform_ApplyPitch(-0x1000);
			SceneTransform_ApplyRoll(0x8000);
			SceneTransform_ApplyPitch(0x8000);
			SceneTransform_ApplyYaw(a << 12);
			if (frame > 150)
			{
				scale[0] = frame * 0x1800 - 0xd9400;
				scale[1] = scale[0];
				scale[2] = scale[0];
				size = 5;
			}
			else
			{
				scale[0] = 0x10000 - a * 3 * 0x400;
				scale[1] = scale[0];
				scale[2] = scale[0];
			}
			SceneTransform_ApplyScale(scale);
		}
		else
		{
			pos[0] = 0;
			pos[1] = 0;
			pos[2] = 0x1000000;
			SceneTransform_ApplyPosition(pos);
			SceneTransform_ApplyPitch(-0x1000);
			SceneTransform_ApplyRoll(frame << 8);
			SceneTransform_ApplyPitch(frame << 8);
		}

		if (frame <= 149)
		{
			for (i = 0; i != 6; i++)
			{
				vec[0] = (Mode12_Points[i][0] - 96) << 16;
				vec[1] = 0;
				vec[2] = (Mode12_Points[i][1] - 96) << 16;
				EffectPosition_ApplyBaseAndYOffset(vec, (struct EffectPosition *)&pts[i]);
				pts[i].x = (pts[i].x >> 17) + 64;
				pts[i].y = (pts[i].y >> 16) + 60;
			}
			for (i = 0; i != 3; i++)
			{
				n = count * 2 - i * 48 - 256;
				if (n > 48)
				{
					n = 48;
				}
				if (n >= 0)
				{
					for (j = 0; j != n; j++)
					{
						s32 x = pts[i * 2].x
								+ Iwram_MulQ16(j * (pts[i * 2 + 1].x - pts[i * 2].x), 0x555);
						s32 y = pts[i * 2].y
								+ Iwram_MulQ16(j * (pts[i * 2 + 1].y - pts[i * 2].y), 0x555);
						blit[0](dst, aux + ParticleStreams_CellOffsets[size - 1], x - size / 2,
							y - size, size, size * 2);
					}
				}
			}
		}

		if (frame <= 179)
		{
			DrawRectangle blit47;
			s32			  lvl = 0;

			if (frame > 155)
			{
				lvl = frame - 156;
			}
			if (lvl > 7)
			{
				lvl = 7;
			}
			if (frame <= 139)
			{
				BattleEffect_LoadWork(47, 7, 7, 3, 3);
				blit47 = SLOT(47);
			}
			else
			{
				BattleEffect_LoadWork(47, 7, 7, 3, 2);
				blit47 = SLOT(47);
			}
			n = count;
			if (n > 128)
			{
				n = 128;
			}
			for (i = 0; i != n; i++)
			{
				s32 s = size + 1;
				vec[0] = Trig_Sin(i << 9) * 96;
				vec[2] = -(Trig_Cos(i << 9) * 96);
				EffectPosition_ApplyBaseAndYOffset(vec, (struct EffectPosition *)&pts[0]);
				pts[0].x = (pts[0].x >> 17) + 64;
				pts[0].y = (pts[0].y >> 16) + 60;
				blit47(dst,
					work->sheet + (lvl * 0x302 + ParticleStreams_CellOffsets[s - 1]) + rings,
					pts[0].x - s / 2, pts[0].y - s, s, s * 2);
			}
			Runtime_ReleaseHeapBlock(47);
		}

		if ((u32)(frame - 151) <= 16)
		{
			s32 w = 0;
			s32 h = (frame - 151) * 20 + 44;
			if (frame > 151)
			{
				w = frame - 152;
			}
			if (w > 15)
			{
				w = 15;
			}
			for (i = 1; i != 10; i++)
			{
				if (w + i <= 15)
				{
					blit[0](dst, &ramp[w + i], w + 48 + i, 16 - i, 32 - w * 2 - i * 2, 1);
				}
			}
			for (i = 0; i != h; i++)
			{
				s32 k = 32;
				/* FAKEMATCH: loop.c matches this 32 with the smoke calls' 32 (life 19, savings 2: 12 * 2 * 19 = 456 >= 450 insns) and hoists it out of the frame loop, so it loses its register and later reloads rotate; the native builds it in this loop's preheader. 2026-10-11: loop.c's threshold is 12 after an earlier move (savings 2 x life 19 x 12 = 456 >= 448 insns); a 96 local declared before the ramp loops lowers that to 432 and defeats the hoist, but the asm pair's two codeless preheader insns also raise i's live length to 1292, which orders n (28 refs/106) before i (195 refs) and gives n r9; without them i takes r9 and a slot at [sp, #60] appears (differ 699). */
				asm("" : "+r"(k));
				blit[0](dst, ramp + w, w + 48, i + 16, k - w * 2, 1);
			}
			BattleEffect_LoadWork(47, 7, 7, 3, 2);
			SLOT(47)(dst, work, 32, (frame - 151) * 20 - 12, 32, 96);
			Runtime_ReleaseHeapBlock(47);
			BattleEffect_LoadWork(47, 7, 7, 7, 2);
			SLOT(47)(dst, work, 64, (frame - 151) * 20 - 12, 32, 96);
			Runtime_ReleaseHeapBlock(47);
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	ctrl[4] = 0;
	Scheduler_RemoveCallback((u32)BattleFx_AdvanceScrollOnInterval);
	BattleEffect_SetupBlendedDisplay();
	BattleFx_SelectLivingTargets(work->effect);
	BattleFx_SpawnObjects(9, 0x173, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, Ram_MapCellBuffer, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_VioletMoteSheet, work->sheet + 0x6000, 1, 0);
	Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_CrystalSheet), 128);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesB, aux, 0, 0);
	*(u16 *)0x04000052 = 0x1010;
	{
		s32 cam_x;
		s32 cam_y;
		s32 vel_x;
		s32 vel_y;
		s32 cx;
		s32 cy;

		cam_x = 0x500000;
		cam_y = 0x400000;
		vel_x = 0;
		vel_y = 0;
		for (i = 0; i != 910; i++)
		{
			PARTICLES[i].variant = 0;
		}
		for (i = 0; i != 16; i++)
		{
			work->particles[i].x = (Random16() & 31) - 16;
			work->particles[i].y = Random16() & 63;
			work->particles[i].variant = -(Random16() & 15);
		}
		work->transfer_mode = 2;
		work->transfer_value = 50;
		AudioCommand_PlayFar(145);
		cx = 60;
		cy = 112;

		for (frame = 0; frame != 192; frame++)
		{
			s32 d;
			if (frame == 20)
			{
				Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetB, work, 1, 0);
			}
			d = frame - 80;
			if ((u32)d <= 59 && !(frame & 7))
			{
				AudioCommand_PlayFar(134);
			}
			if (frame == 140)
			{
				BattleEventRuntime_BeginPhaseFar(134);
			}
			for (j = 0; j != 1; j++)
			{
				s32 start = j * 128 + 8;
				if (frame >= start && frame < j * 128 + 17)
				{
					if (frame >= j * 128 + 9 && frame < j * 128 + 12)
					{
						blit[0](dst, work->sheet, 36, 0, 48, cy);
					}
					if (frame >= j * 128 + 12 && frame < start + 8)
					{
						blit[0](dst, work->sheet + 0x1500, 36, 0, 48, 112);
					}
					if (frame == start + 2)
					{
						s32 n = 0;
						for (i = 0, p = PARTICLES; i != 910; i++, p++)
						{
							if (p->variant == 0)
							{
								s32 speed = 0x3ff;
								s32 angle;
								speed &= Random16();
								angle = (Random16() & 0x7fff) - 0x4000;
								p->x = cx << 16;
								p->y = cy << 16;
								speed += 32;
								p->velocity_x = (Trig_Sin(angle) * speed) >> 7;
								p->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
								p->variant = (Random16() & 7) + 32;
								if (++n == 512)
								{
									break;
								}
							}
						}
						work->shake_frames = 8;
					}
				}
			}
			if (frame == 10)
			{
				for (i = 0; i != work->effect->count; i++)
				{
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 8);
					BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
				}
			}
			if ((u32)d <= 63)
			{
				s32 ox;
				s32 oy;
				s32 sx;
				for (i = 0; i != 6; i++)
				{
					if (frame == i * 8 + 80)
					{
						Iwram_FillWords(dst, 0x4000, 0x10101010);
					}
				}
				for (i = 0; i != 30; i++)
				{
					if (frame >= i * 2 + 80)
					{
						if (frame < i * 2 + 82)
						{
							s32 rx = (Random16() & 3) - 3;
							s32 ry = (Random16() & 3) + 32;
							BattleEffect_LoadWork(47, 7, 7, 3, 2);
							SLOT(47)(dst, work->sheet + i % 3 * 0x1440, rx, ry, 72, 72);
							Runtime_ReleaseHeapBlock(47);
						}
						if (frame == i * 2 + 80)
						{
							s32 n = 0;
							for (j = 0, p = PARTICLES; j != 910; j++, p++)
							{
								if (p->variant == 0)
								{
									s32 speed = 0x3ff;
									s32 angle;
									speed &= Random16();
									angle = Random16() & 0xffff;
									p->x = 0x100000;
									p->y = 0x500000;
									speed += 32;
									p->velocity_x = (Trig_Sin(angle) * speed) >> 7;
									p->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
									p->variant = (Random16() & 7) + 32;
									if (++n == 16)
									{
										break;
									}
								}
							}
							work->shake_frames = 8;
							for (j = 0; j != work->effect->count; j++)
							{
								ObjectGroup_UpdateMembers(work->effect->actors[j],
									Mode12_Animations[Random16() & 3], 5, j, 4);
								BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 4);
							}
						}
					}
				}
				{
					s32 r = 7;
					s32 angle;
					r &= Random16();
					angle = Random16() & 0xffff;
					ox = (Trig_Sin(angle) * (r + 8)) >> 16;
					sx = ox + 72;
					oy = 32 - ((Trig_Cos(angle) * (r + 8)) >> 16);
				}
				BattleEffect_LoadWork(47, 7, 7, 3, 2);
				SLOT(47)(dst, work->sheet + 0x6000, ox + 60, oy - 24, 12, 24);
				Runtime_ReleaseHeapBlock(47);
				BattleEffect_LoadWork(47, 7, 7, 7, 2);
				SLOT(47)(dst, work->sheet + 0x6000, sx, oy - 24, 12, 24);
				Runtime_ReleaseHeapBlock(47);
				BattleEffect_LoadWork(47, 7, 7, 11, 2);
				SLOT(47)(dst, work->sheet + 0x6000, ox + 60, oy, 12, 24);
				Runtime_ReleaseHeapBlock(47);
				BattleEffect_LoadWork(47, 7, 7, 15, 2);
				SLOT(47)(dst, work->sheet + 0x6000, sx, oy, 12, 24);
				Runtime_ReleaseHeapBlock(47);
				j = 0;
				for (i = 0, p = PARTICLES; i != 910; i++, p++)
				{
					if (p->variant == 0)
					{
						s32 speed = 63;
						s32 angle;
						speed &= Random16();
						angle = Random16() & 0xffff;
						p->x = sx << 16;
						p->y = oy << 16;
						speed += 64;
						p->velocity_x = (Trig_Sin(angle) * speed) >> 7;
						p->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
						p->variant = (Random16() & 7) + 16;
						if (++j == 4)
						{
							break;
						}
					}
				}
			}

			BattleEffect_LoadWork(47, 7, 7, 15, 2);
			{
				DrawRectangle	   draw = SLOT(47);
				struct EffectStep *q;
				for (i = 0, q = PARTICLES; i != 910; i++, q++)
				{
					if (q->variant > 0)
					{
						q->variant--;
						EffectStep_AdvanceWithGravity2D(q, 60, 0);
						if (q->y > 0x780000)
						{
							q->velocity_y = -q->velocity_y / 2;
						}
						else if ((u32)q->x <= 0x7effff && q->y >= 0)
						{
							s32 x = q->x >> 16;
							s32 y = q->y >> 16;
							s32 s = q->variant / 16 + 1;
							draw(dst, aux + ParticleStreams_CellOffsets[s - 1], x - s / 2, y - s, s,
								s * 2);
						}
					}
				}
			}
			Runtime_ReleaseHeapBlock(47);

			if (frame > 15)
			{
				if (frame == 32)
				{
					vel_x = 0x40000;
					vel_y = -0x4000;
				}
				if (frame > 31)
				{
					cam_x += vel_x;
					cam_y += vel_y;
					vel_x = vel_x * 60 / 64;
					vel_y = vel_y * 60 / 64;
				}
				BattleFx_PlaceFormationObjects(0, cam_x, cam_y);
			}
			if ((u32)(frame - 16) <= 127)
			{
				s32 base;
				for (i = 0, base = cam_x >> 17; i != 3; i++)
				{
					s32 k = i & 3;
					s32 angle = Random16() & 0xffff;
					s32 x =
						((Trig_Sin(angle) << 3) >> 16) + base - (BattleFx_GlintCellWidths[k] >> 1);
					s32 y = ((Trig_Cos(angle) * 40) >> 16) - (BattleFx_GlintCellHeights[k] >> 1);
					BattleEffect_LoadWork(47, 7, 7, 3 | Mode12_SmokeFlips[Random16() & 3], 2);
					SLOT(47)(dst, Ram_MapCellBuffer + BattleFx_GlintCellOffsets[k], x, y + 56,
						BattleFx_GlintCellWidths[k], BattleFx_GlintCellHeights[k]);
					Runtime_ReleaseHeapBlock(47);
				}
			}
			if (frame <= 31)
			{
				Camera_ApplyShake(8, 16);
			}
			else
			{
				Camera_ApplyShake(4, 4);
			}
			ObjectGroup_TickMemberTimers();
			work->transfer_pending = 1;
			WaitFrames(1);
		}

		Runtime_ReleaseHeapBlock(46);
		Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
		BattleEffect_RunImpactBurst(0, cam_x, cam_y);
	}
	for (i = 0; i != 9; i++)
	{
		ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
	}
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunShatteringRocks(struct BattleEffectArgument *object)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangleFn			 blit;
	s32						 rows;
	struct EffectPosition	 first;
	struct EffectPosition	 final;
	struct EffectStep		*rock;
	struct EffectStep		*shard;
	s32						 frame;
	s32						 i;
	s32						 j;
	s32						 cell;
	u32						 width;
	u32						 height;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = object;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100;
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
	BattleEffect_LoadWork(46, 7, 7, 3, 1);
	blit = heap_cache[7];
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[work->effect->count - 1], &final);
	first.x += (final.x - first.x) / 2;
	REG_BG2X = (64 - first.x) << 8;
	work->transfer_mode = 1;
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	rows = ShatterRocks_RockCounts[work->effect->variant];
	for (i = 0; i != rows; i++)
	{
		rock = &work->particles[i];
		rock->y = -0x400000;
		rock->velocity_y = 0;
	}
	for (i = 0; i != rows; i++)
	{
		for (j = 0; j != 21; j++)
		{
			struct EffectStep *seed = &((struct EffectStep *)Ram_MapCellBuffer)[i * 21 + j];

			seed->x = (ShatterRocks_ShardOffsets[j * 2] + ShatterRocks_RockX[i]) << 16;
			seed->y = ShatterRocks_ShardOffsets[j * 2 + 1] << 16;
			seed->velocity_x = ((Random16() % 96) - 48) << 10;
			seed->velocity_y = -((Random16() & 127) + 32) << 11;
			seed->z = 32;
			seed->variant = 0;
		}
	}
	for (frame = 0; frame != ShatterRocks_DropFrames[rows - 1] + 80; frame++)
	{
		if (frame == ShatterRocks_DropFrames[rows - 1] + 48)
			BattleEventRuntime_BeginPhaseFar(132);
		for (i = 0; i != rows; i++)
		{
			rock = &work->particles[i];
			if (frame == ShatterRocks_DropFrames[i] + 18)
			{
				AudioCommand_PlayFar(134);
				work->shake_frames = 4;
			}
			if (frame >= ShatterRocks_DropFrames[i] + 18)
			{
				for (j = 0; j != 21; j++)
				{
					shard = &((struct EffectStep *)Ram_MapCellBuffer)[i * 21 + j];
					cell = (j % 5) * 3 + shard->variant / 96 % 3;
					blit(canvas, work->sheet + ShatterRocks_ShardCells[cell] + 0x83c,
						HI(shard->x) - ((width = ShatterRocks_ShardWidths[cell]) >> 1),
						HI(shard->y) - ((height = ShatterRocks_ShardHeights[cell]) >> 1), width,
						height);
					EffectStep_AdvanceWithGravity2D(shard, 64, 0x4000);
					shard->variant += shard->z;
					if (shard->z > 1 && (frame & 1))
						shard->z--;
				}
			}
			if (frame < ShatterRocks_DropFrames[i] + 18)
			{
				if (frame >= ShatterRocks_DropFrames[i])
					blit(canvas, work, ShatterRocks_RockX[i] + 47, HI(rock->y), 34, 62);
				rock->y += rock->velocity_y;
				if (frame > ShatterRocks_DropFrames[i])
					rock->velocity_y += 0x10000;
				if (rock->y > 0x320000)
					rock->y = 0x320000;
			}
			if (frame == ShatterRocks_DropFrames[i] + 18)
			{
				for (j = 0; j != work->effect->count; j++)
					ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 8);
			}
		}
		Camera_ApplyShake(2, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(46);
	BattleFx_EndCanvasLayer();
}

#undef gWorkSlot
