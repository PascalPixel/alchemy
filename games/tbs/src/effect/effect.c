#include "unit/effect/koukatbl.h"
#include "unit/effect/effect.h"
#include "unit/object/object.h"
#include "unit/object/psyfx.h"
#include "unit/runtime/runtime.h"
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
#include "canvas.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "menu_list.h"
#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "scene.h"
#include "global_cells.h"
#include "dma.h"
#include "resource_ids.h"
#include "battle_efx.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "btldisp.h"
#include "ram_buffer.h"
#include "resource.h"
#include "b5_context.h"
#include "motobj.h"
#include "iwram_call.h"
#include "transform.h"
#include "btlwork.h"
#include "project.h"
#include "field_event.h"
#include "keystate.h"
#include "btltype.h"
#include "sound_ids.h"
#include "btlsys.h"
#include "battle_session.h"
#include "animspr.h"
#include "battle_unit.h"
#include "fixed_point_position.h"
#include "irq.h"
#include "unit/effect/koukabit.h"
#include "call.h"

void BattlePres_ConfigureEffectDisplay(void)
{
	REG_BLDCNT = 0;
	REG_BLDALPHA = 0x100e;
	REG_WIN0H = 0x00f0;
	REG_WIN0V = 0x1088;
	REG_WIN1H = 0x00f0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;
	IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
	WaitFrames(1);
}

void BattleFx_AdvanceScrollOnInterval(void)
{
	struct BattleEffectWork *work = gBattleFxWork[0];
	u32						*counter = (u32 *)&work->scroll_timer;

	(*counter)++;
	if (*counter == work->scroll_interval)
	{
		gBgScroll[1].x += work->scroll_step_x;
		gBgScroll[1].y += work->scroll_step_y;
		*counter = 0;
	}
}

void Camera_AdvanceBg2Reference(void)
{
	s32						 count;
	struct BattleEffectWork *work;

	work = gBattleFxWork[0];
	count = work->scroll_timer + 1;
	work->scroll_timer = count;
	if (count == work->scroll_interval)
	{
		REG_BG2X = work->bg2x;
		REG_BG2Y = work->bg2y;
		work->bg2x += work->scroll_step_x;
		work->bg2y += work->scroll_step_y;
		work->scroll_timer = 0;
	}
}

void BattleFx_ArmWin0HBlankDma(void)
{
	volatile u16 *channel = REG_DMA0;
	channel[5] &= 0xc5ff;
	channel[5] &= 0x7fff;
	(void)channel[5];
	Dma_Set(gMapCellBuffer, (void *)0x04000040, 0xa2600001, (volatile u32 *)channel);
}

void BattleEffect_RunFallingParticles(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	u8						*graphics;
	BlitterPair				 draw;
	struct EffectStep		*shard;
	struct EffectStep		*puff;
	u16						*line;
	s32						 i;
	s32						 k;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	graphics = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0x2001);
	REG_BG2PA = 0x100;
	Resource_LoadAndDecompress((s32)&ResourceId_IceShardSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, graphics, 0, 0);
	BattlePres_ConfigureEffectDisplay();
	REG_BLDCNT = 0x3f44;
	REG_WININ = 0x3337;

	{
		s32 y;
		s32 slant;

		for (i = 0, y = -128, slant = -16; i != 32; y -= 64, i++, slant -= 8)
		{
			s32 x;

			shard = &work->particles[i];
			x = (Random16() & 63) + (Random16() & 7) + 24;
			if (work->effect->side == 1)
				x = x + slant + 24;
			else
				x = x - slant + 80;
			shard->y = y;
			shard->x = x << 3;
			shard->variant = -1;
		}
	}
	for (i = 0; i != 32; i++)
		work->particles[32 + i].variant = -1;

	if (work->effect->side == 0)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 2, 2);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 2, 3);
	}
	else
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 6, 2);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 6, 3);
	}
	draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER_ALTERNATE];

	if (work->effect->side == 0)
	{
		line = (u16 *)gMapCellBuffer;
		for (i = 0; i != 160; i++)
		{
			if (i >= 8 && i <= 103)
				line[i] = (0xf0 - i) | ((0x70 - i) << 8);
			else if (i <= 135)
				line[i] = 0x888;
			else
				line[i] = 0x100;
		}
	}
	else
	{
		line = (u16 *)gMapCellBuffer;
		for (i = 0; i != 160; i++)
		{
			if (i >= 8 && i <= 95)
				line[i] = (i - 8 + 160) | ((i + 0x18) << 8);
			else if (i <= 135)
				line[i] = 0x78f8;
			else
				line[i] = 0x100;
		}
	}

	Scheduler_AddOrUpdateCallback(BattleFx_ArmWin0HBlankDma, 0x480);
	work->transfer_mode = 2;
	if (work->effect->variant == 1)
		work->transfer_value = 75;
	else
		work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != FallingShards_Counts[work->effect->variant * 2 + 1]; frame++)
	{
		if (frame == FallingShards_Counts[work->effect->variant * 2 + 1] - 16)
			BattleEventRuntime_BeginPhaseFar(132);
		for (i = 0; i != FallingShards_Counts[work->effect->variant * 2]; i++)
		{
			shard = &work->particles[i];
			if (shard->variant == -1)
			{
				s32 x = shard->x / 8;
				s32 y = shard->y / 8;

				draw[work->effect->variant == 2 ? 1 : 0](canvas, work, x, y, 32, 32);
				if (shard->y <= 0x27f)
				{
					if (work->effect->side == 0)
						shard->x -= 64;
					else
						shard->x += 64;
					shard->y += 64;
				}
				else
				{
					if ((i & 3) == 0)
						AudioCommand_PlayFar(115);
					work->shake_frames = 2;
					shard->variant = 0;
					for (k = 0; k != work->effect->count; k++)
						ObjectGroup_UpdateMembers(work->effect->actors[k], 9, 5, k, 8);
				}
			}
			if (shard->variant != -1)
			{
				s32 x = shard->x / 8;
				s32 y = shard->y / 8;

				if (shard->variant >= 1 && shard->variant <= 14)
				{
					draw[work->effect->variant == 2 ? 1 : 0](
						canvas, work->sheet + 0x400 + ((shard->variant / 3) << 10), x, y, 32, 32);
				}
				if (shard->variant >= 9 && shard->variant <= 11)
				{
					for (k = 0; k != 32; k++)
					{
						puff = &work->particles[32 + k];
						if (puff->variant == -1)
						{
							puff->variant = 18;
							puff->x = (((Random16() & 31) + shard->x / 8) << 3) + 8;
							puff->y = ((Random16() & 15) + shard->y / 8 - 15) << 3;
							break;
						}
					}
				}
				if (shard->variant <= 14)
					shard->variant++;
			}
		}
		for (i = 0; i != 32; i++)
		{
			puff = &work->particles[32 + i];
			if (puff->variant != -1)
			{
				if (puff->variant <= 17)
				{
					s32 image = puff->variant / 2;
					s32 x = puff->x / 8 - (BattleFx_PuffSizes[image] >> 1);
					s32 y = puff->y / 8 - (BattleFx_PuffSizes[image] >> 1);

					draw[work->effect->variant == 2 ? 1 : 0](canvas,
						graphics + BattleFx_PuffCells[image], x, y, BattleFx_PuffSizes[image],
						BattleFx_PuffSizes[image]);
				}
				if (puff->variant > -1)
					puff->variant--;
			}
		}
		ObjectGroup_TickMemberTimers();
		Camera_ApplyShake(4, 4);
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Scheduler_RemoveCallback(BattleFx_ArmWin0HBlankDma);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
	BattlePres_ConfigureEffectDisplay();
}

struct BoltSparkBuffer
{
	u16				  lines[160];
	struct EffectStep sparks[512];
};

#define gBoltSparks ((struct BoltSparkBuffer *)Ram_MapCellBuffer)

void BattleEffect_RunFallingBolts(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	DrawRectangle			 draw_spark;
	DrawRectangle			 draw_bolt;
	struct EffectStep		*bolt;
	struct EffectStep		*spark;
	u16						*line;
	s32						 i;
	s32						 k;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0x2001);
	REG_BG2PA = 0x100;
	Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet, work->sheet + 0x604, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, work->sheet, 0, 0);
	BattlePres_ConfigureEffectDisplay();
	REG_BLDCNT = 0x3f44;
	REG_WININ = 0x3337;

	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 2, 2);
	draw_bolt = heap_cache[7];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 2, 3);
	draw_spark = heap_cache[8];

	for (i = 0; i != 512; i++)
		gBoltSparks->sparks[i].variant = -1;

	for (i = 0; i != 64; i++)
	{
		s32 x;
		s32 y;

		bolt = &work->particles[i];
		x = Random16() & 63;
		y = -(i * FallingBolts_Counts[work->effect->variant * 4 + 2] + 16);
		if (work->effect->side == 1)
			x = x + y / 2 - 48;
		else
			x = x - y / 2 + 72;
		bolt->x = x << 3;
		bolt->y = y << 3;
		bolt->variant = -1;
	}

	if (work->effect->side == 0)
	{
		line = (u16 *)gMapCellBuffer;
		for (i = 0; i != 160; i++)
		{
			if (i >= 8 && i <= 103)
				line[i] = ((0x34 - i / 2) << 8) | (0xb4 - i / 2);
			else if (i <= 135)
				line[i] = 0x80;
			else
				line[i] = 0x100;
		}
	}
	else
	{
		line = (u16 *)gMapCellBuffer;
		for (i = 0; i != 160; i++)
		{
			if (i >= 8 && i <= 103)
				line[i] = ((i / 2 + 60) << 8) | (i / 2 + 188);
			else if (i <= 135)
				line[i] = 0x70f0;
			else
				line[i] = 0x100;
		}
	}

	Scheduler_AddOrUpdateCallback(BattleFx_ArmWin0HBlankDma, 0x480);
	if (work->effect->variant == 0)
	{
		work->transfer_mode = 1;
		work->transfer_value = 0;
	}
	else if (work->effect->variant == 1)
	{
		work->transfer_mode = 2;
		work->transfer_value = 50;
	}
	else
	{
		work->transfer_mode = 2;
		work->transfer_value = 75;
	}
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != FallingBolts_Counts[work->effect->variant * 4 + 3]; frame++)
	{
		if (frame == FallingBolts_Counts[work->effect->variant * 4 + 3] - 64)
			BattleEventRuntime_BeginPhaseFar(132);
		for (i = 0; i != FallingBolts_Counts[work->effect->variant * 4]; i++)
		{
			s32 x;
			s32 y;

			bolt = &work->particles[i];
			x = bolt->x / 8;
			y = bolt->y / 8;
			if (bolt->variant == -1)
			{
				draw_bolt(canvas, work->sheet + 0x604, x, y, 24, 24);
				if (bolt->y <= 0x27f)
				{
					if (work->effect->side == 0)
						bolt->x -= 32;
					else
						bolt->x += 32;
					bolt->y += 64;
				}
				else
				{
					bolt->variant = 0;
					for (k = 0; k != FallingBolts_Counts[work->effect->variant * 4 + 1]; k++)
					{
						spark = &gBoltSparks
									 ->sparks[i * FallingBolts_Counts[work->effect->variant * 4 + 1]
											  + k];
						spark->x = (x + 12) << 16;
						spark->y = y << 16;
						spark->velocity_x = ((Random16() & 255) - 128) << 9;
						if (work->effect->variant == 2)
							spark->velocity_y = ((Random16() & 0x1ff) - 0x180) << 10;
						else
							spark->velocity_y = ((Random16() & 255) - 255) << 10;
						spark->variant = (Random16() & 15) + 16;
					}
					if ((i & 3) == 0)
						AudioCommand_PlayFar(132);
					for (k = 0; k != work->effect->count; k++)
						ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 2);
				}
			}
			else
			{
				if (bolt->variant >= 0 && bolt->variant <= 3)
					draw_bolt(canvas, work->sheet + 0x844, x, y, 24, 24);
				else if (bolt->variant <= 7)
					draw_bolt(canvas, work->sheet + 0xa84, x - 9, y - 9, 42, 42);
				if (bolt->variant <= 14)
					bolt->variant++;
			}
		}
		for (i = 0; i != 512; i++)
		{
			spark = &gBoltSparks->sparks[i];
			if (spark->variant != -1)
			{
				s32 size = spark->variant + 1;

				if (size > 6)
					size = 6;
				draw_spark(canvas, work->sheet + BattleFx6_FlareCells[size - 1],
					((s16 *)&spark->x)[1] - size, ((s16 *)&spark->y)[1] - size, size * 2, size * 2);
				EffectStep_AdvanceWithGravity2D(spark, 60, 0x2000);
				spark->variant--;
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattleFx_ArmWin0HBlankDma);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
	BattlePres_ConfigureEffectDisplay();
}

//--------------------------------------------------------------------------
// run_five_mode_mode0.c
// バトル演出：使う者から対象ごとに 3 本の筋を飛ばして当てる (色の違う五種)
//--------------------------------------------------------------------------
// FiveMode_Records は mode ごとに 4 バイト
#define REC(mode, n) FiveMode_Records[(mode) * 4 + (n)]
#define REC_ANIME 0	 // 0 でなければ筋の絵を 3 枚で回す
#define REC_HIT 1	 // 0 でなければ当たりの音とパレット
#define REC_MOTION 2 // 当たったときののけぞり (-1 ならなし)
#define REC_FURUE 3	 // 0 でなければ当たった対象を震わせる

#define SUJI_MAX 3 // 対象ひとりあたりの筋の数

//--------------------------------------------------------------------------
// mode ごとの入口 (色)
//--------------------------------------------------------------------------
void BattleFx_RunFiveModeMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunFiveMode(effect, 0);
}

void BattleFx_RunFiveModeMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunFiveMode(effect, 1);
}

void BattleFx_RunFiveModeMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RunFiveMode(effect, 2);
}

void BattleFx_RunFiveModeMode4(struct BattleEffectArgument *effect)
{
	BattleFx_RunFiveMode(effect, 4);
}

void BattleFx_RunFiveModeMode3(struct BattleEffectArgument *effect)
{
	BattleFx_RunFiveMode(effect, 3);
}

void BattleFx_RunFiveModeMode1B(struct BattleEffectArgument *effect)
{
	BattleFx_RunFiveMode(effect, 1);
}

//--------------------------------------------------------------------------
// 使う者の 20 ドット上から、対象ごとに 6 フレームおきに 3 本の筋を出し、
// 24 フレームで対象へ届かせる。対象は 32 フレームおきに順に始まる
// 筋の絵は 20x40 で、上半分を上下に反転して 20x80 に描く
// mode : 色 (BG パレットの元の絵 0〜4) と FiveMode_Records の行
//--------------------------------------------------------------------------
void BattleFx_RunFiveMode(struct BattleEffectArgument *effect, s32 mode)
{
	struct EffectPosition	 position;
	void				   **slots;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 k;
	DrawRectangle			 draw_lower;
	DrawRectangle			 draw_upper;
	struct BattleCamera		*camera;
	struct MotionObject		*source;
	struct MotionObject		*target;
	s32						 resource;
	s32						 frame;
	s32						 j;
	s32						 streaks = SUJI_MAX;
	s32						 lift = 20 << 16;

	slots = gBattleFxWork;
	cursor = slots;
	work = *cursor++;
	canvas = *cursor;
	camera = slots[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);

	// 上半分は上下を反転して描く。向こう側なら左右も反転する
	if (work->effect->side == 0)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP | GOUSEI_FLIP_Y, GOUSEI_HIKAKU);
		draw_upper = slots[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP, GOUSEI_HIKAKU);
		draw_lower = slots[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	}
	else
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP | GOUSEI_FLIP, GOUSEI_HIKAKU);
		draw_upper = slots[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
		draw_lower = slots[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	}

	// 筋の絵と、mode で選ぶ色
	Resource_LoadAndDecompress((s32)&ResourceId_FiveModeImage, work->sheet, 0, 0);
	switch (mode)
	{
	case 0:
		resource = (s32)&ResourceId_EmberStreakSheet;
		break;
	case 1:
		resource = (s32)&ResourceId_LimePalette;
		break;
	case 2:
		resource = (s32)&ResourceId_VioletPaletteD;
		break;
	case 3:
		resource = (s32)&ResourceId_RedPaletteB;
		break;
	case 4:
	default:
		resource = (s32)&ResourceId_CyanPalette;
		break;
	}
	Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
	work->transfer_mode = 2;   // 転送のたびに半分へ減らして残像にする
	work->transfer_value = 50; // 1/2
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	source = GetBattleObjectSlotFar(work->effect->actor)->object;
	{
		s32 total = streaks * work->effect->count * 6 + 48;

		// 筋は使う者の 20 ドット上から、24 フレームで対象の 20 ドット上へ
		for (k = 0; k != work->effect->count; k++)
		{
			target = GetBattleObjectSlotFar(work->effect->actors[k])->object;
			for (j = 0; j != streaks; j++)
			{
				struct EffectStep *streak = &work->particles[k * streaks + j];

				streak->x = source->x;
				streak->y = source->y + lift;
				streak->z = source->z;
				streak->velocity_x = (target->x - streak->x) / 24;
				streak->velocity_y = (target->y + lift - streak->y) / 24;
				streak->velocity_z = (target->z - streak->z) / 24;
				streak->variant = 0; // 出てからのフレーム数
			}
		}

		for (frame = 0; frame != total; frame++)
		{
			s32 *row = work->bg2_x;

			// 160 ライン分の BG2X を sin で揺らす
			for (k = 0; k != 160; k++)
				*row++ = (0x80000 - Trig_Sin((frame + k) << 12) * 2) >> 10;
			// 終わりの 16 フレームで半透明を薄くする
			if (frame > total - 16)
				REG_BLDALPHA = (total - frame) | 0x1000;
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			for (k = 0; k != work->effect->count; k++)
			{
				if (frame >= k * 32)
				{
					for (j = 0; j != streaks; j++)
					{
						if (frame >= k * 32 + j * 6)
						{
							struct EffectStep *streak = &work->particles[k * streaks + j];
							s32				   step;

							// 絵は 8 フレームごとに次へ (6 枚目で止まる)
							EffectPosition_ApplyBaseAndYOffset((s32 *)streak, &position);
							position.x >>= 1;
							step = streak->variant / 8;
							if (step > 5)
								step = 5;
							if (REC(mode, REC_ANIME))
							{
								u8 *cell = &work->sheet[step * 800 + frame / 2 % 3 * 4800];

								draw_upper(canvas, cell, position.x - 10, position.y - 40, 20, 40);
								draw_lower(canvas, cell, position.x - 10, position.y, 20, 40);
							}
							else
							{
								u8 *cell = &work->sheet[step * 800 + 2 * 4800];

								draw_upper(canvas, cell, position.x - 10, position.y - 40, 20, 40);
								draw_lower(canvas, cell, position.x - 10, position.y, 20, 40);
							}
							EffectStep_AdvanceWithGravity3D(streak, 64, 0);
							streak->variant++;
						}
					}

					// 当たりの 6 フレーム後から 32 フレームのあいだ、対象を左右に震わせる
					if (REC(mode, REC_FURUE) && frame >= k * 32 + 30 && frame < k * 32 + 62)
					{
						struct MotionObject *struck =
							GetBattleObjectSlotFar(work->effect->actors[k])->object;

						struck->x += FiveMode_Tremble[(frame - k * 32 - 30) % 8] << 16;
						if (struck->x > 0)
							struck->x += 0x8000;
						else
							struck->x -= 0x8000;
						ObjectGroup_UpdateMembers(work->effect->actors[k], -1, 5, -1, 0);
					}

					// 当たり：音を鳴らし、パレット 7 にして 8 フレーム後に戻す (2 回)
					if (REC(mode, REC_HIT))
					{
						if (frame == k * 32 + 24)
						{
							AudioCommand_PlayFar(133);
							if (k == 0)
								BattleEventRuntime_BeginPhaseFar(-1); // 最初の当たりで結果へ進む
							ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 8);
						}
						if (frame == k * 32 + 40)
							ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 8);
					}
					if (REC(mode, REC_MOTION) != -1 && frame == k * 32 + 24)
					{
						work->shake_frames = 4;
						BattleMotion_ApplyVariantMotionFar(
							work->effect->actors[k], REC(mode, REC_MOTION));
					}
				}
			}
			Camera_ApplyShake(8, 8);
			ObjectGroup_TickMemberTimers();
			work->transfer_pending = 1;
			WaitFrames(1);
		}
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunParticlePoolMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunParticlePool(effect, 1);
}

void BattleFx_RunParticlePoolMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunParticlePool(effect, 0);
}

void BattleFx_RunParticlePool(struct BattleEffectArgument *effect, s32 mode)
{
	s32						 point[3];
	struct EffectPosition	 position;
	struct BattleEffectWork *work;
	void					*canvas;
	BlitterPair				 draw;
	struct BattleCamera		*camera;
	u8						*sheet;
	struct MotionObject		*source;
	struct EffectStep		*mote;
	s32						 resource;
	s32						 frame;
	s32						 i;

	work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	sheet = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
	work->effect = effect;
	if (mode == 0)
		BattleFx_BeginCanvasLayer(0);
	else
		BattleFx_BeginCanvasLayer(1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 11, 2);
	draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER_ALTERNATE];
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	if (mode == 0)
		resource = (s32)&ResourceId_OrangePaletteA;
	else
		resource = (s32)&ResourceId_EarthWallSheet;
	Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry(resource), 128);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	source = GetBattleObjectSlotFar(work->effect->actor)->object;
	for (i = 0; i != 256; i++)
	{
		s32 speed;
		s32 angle;

		mote = &PARTICLES[i];

		speed = 0x3ff;
		speed &= Random16();
		angle = Random16() & 0xffff;
		mote->x = source->x;
		mote->y = source->y + 0x50000;
		mote->z = source->z;
		mote->velocity_x = Trig_Sin(angle) * (speed + 32) >> 8;
		mote->velocity_y = ((Random16() & 255) - 32) << 9;
		mote->velocity_z = -(Trig_Cos(angle) * (speed + 32) << 1) >> 8;
		mote->variant = (Random16() & 31) + 48;
		if (mode == 0)
		{
			mote->velocity_x /= 2;
			mote->velocity_z /= 2;
		}
	}

	for (frame = 0; frame != 128; frame++)
	{
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		for (i = 0; i != 128; i++)
		{
			mote = &PARTICLES[i];
			if (frame >= i / 32 * 8 && mote->variant >= 0)
			{
				s32 size;

				point[0] = mote->x + Trig_Sin((i * 4 + mote->variant) << 10) * 16;
				point[1] = mote->y;
				point[2] = mote->z;
				EffectPosition_ApplyBaseAndYOffset(point, &position);
				position.x >>= 1;
				if (position.depth <= 313)
					position.depth = 314;
				if (position.depth > 634)
					position.depth = 634;
				size = 6 - (position.depth - 314) / 64;
				draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					position.x - size / 2, position.y - size, size, size * 2);
				EffectStep_AdvanceWithGravity3D(mote, 62, 0x400);
				if (mode == 1)
				{
					if (source->x < 0)
						mote->velocity_x += 0x2000;
					else
						mote->velocity_x -= 0x2000;
				}
				mote->variant--;
			}
		}
		if (mode == 1)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame == i * 8 + 48)
				{
					BattleEventRuntime_BeginPhaseFar(-1);
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 8);
				}
			}
		}
		else
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame == i * 8 + 48)
				{
					AudioCommand_PlayFar(126);
					BattleEventRuntime_BeginPhaseFar(-1);
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, -1, i, 8);
				}
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

#define gSparks ((struct EffectStep *)(Ram_MapCellBuffer + 0x3800))

void BattleFx_RunTwelveModeMode6(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 6);
}

void BattleFx_RunTwelveModeMode3(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 3);
}

void BattleFx_RunTwelveModeMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 1);
}

void BattleFx_RunTwelveModeMode10(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 0xA);
}

void BattleFx_RunTwelveModeMode5(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 5);
}

void BattleFx_RunTwelveModeMode9(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 9);
}

void BattleFx_RunTwelveModeMode4(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 4);
}

void BattleFx_RunTwelveModeMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 0);
}

void BattleFx_RunTwelveModeMode8(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 8);
}

void BattleFx_RunTwelveModeMode7(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 7);
}

void BattleFx_RunTwelveModeMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 2);
}

void BattleFx_RunTwelveModeMode11(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 0xB);
}

void BattleFx_RunTwelveMode(struct BattleEffectArgument *effect, s32 mode)
{
	struct EffectPosition	 ground;
	struct EffectPosition	 target_position;
	struct EffectPosition	 alternate;
	struct EffectPosition	 position;
	BlitterPair				 draw;
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 k;
	u8						*sheet;
	struct BattleCamera		*camera;
	struct MotionObject		*target;
	struct MotionObject		*source;
	s32						 resource;
	s32						 i;
	s32						 wisp_sheet = 0x3200;
	DrawRectangle			*lower = &gBlitters[HEAP_SLOT_BLITTER_ALTERNATE];

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	camera = heap_cache[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
	work->effect = effect;
	if (mode == 8)
		BattleFx_BeginCanvasLayer(0);
	else
		BattleFx_BeginCanvasLayer(1);
	REG_BLDALPHA = 0x1010;
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
	if (TwelveMode_Records[mode * 7] == 0)
		Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage, work->sheet + 0xc80, 0, 0);
	else
		Resource_LoadAndDecompress((s32)&ResourceId_TwelveModeImage, work->sheet + 0xc80, 0, 0);
	switch (TwelveMode_Records[mode * 7 + 3])
	{
	case 0:
		resource = (s32)&ResourceId_BlueBeamSheet;
		break;
	case 1:
		resource = (s32)&ResourceId_LimePalette;
		break;
	case 2:
		resource = (s32)&ResourceId_PinkPalette;
		break;
	case 3:
		resource = (s32)&ResourceId_EmberStreakSheet;
		break;
	case 4:
		resource = (s32)&ResourceId_MarsDjinnSheet;
		break;
	case 5:
	default:
		resource = (s32)&ResourceId_LightningBoltSheet;
		break;
	}
	Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry(resource), 128);
	Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet + 0x3200, 1, 0);

	for (k = 0; k != 1; k++)
	{
		target = GetBattleObjectSlotFar(work->effect->actors[k])->object;
		for (i = 0; i != 64; i++)
		{
			struct EffectStep *step = &work->particles[i];

			EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[k], &ground);
			step->x = (ground.x / 2) << 16;
			step->y = 0x500000;
			step->z = 0;
			step->velocity_x = ((Random16() & 255) - 128) << 9;
			step->velocity_y = ((Random16() & 255) - 128) << 9;
			step->velocity_z = 0;
			step->variant = -1;
		}
		for (i = 0; i != 128; i++)
		{
			struct EffectStep *step = &PARTICLES[k * 128 + i];

			step->x = target->x;
			step->y = 0x140000;
			step->z = target->z;
			step->velocity_x = ((Random16() & 255) - 128) << 11;
			step->velocity_y = ((Random16() & 255) - 128) << 11;
			step->velocity_z = ((Random16() & 255) - 128) << 11;
			step->variant = -1;
		}
		for (i = 0; i != 512; i++)
		{
			gSparks[i].x = target->x;
			gSparks[i].y = 0x140000;
			gSparks[i].z = target->z;
			if (TwelveMode_Records[mode * 7] == 1)
			{
				gSparks[i].velocity_x = ((Random16() & 255) - 128) << 11;
				gSparks[i].velocity_y = ((Random16() & 255) - 128) << 11;
				gSparks[i].velocity_z = ((Random16() & 255) - 128) << 11;
			}
			else
			{
				if (TwelveMode_Records[mode * 7 + 2] & 16)
				{
					gSparks[i].velocity_x = (Random16() & 0x1ff) << 11;
					gSparks[i].velocity_y = ((Random16() & 0x1ff) - 256) << 11;
					gSparks[i].velocity_z = ((Random16() & 0x1ff) - 256) << 11;
				}
				else
				{
					gSparks[i].velocity_x = (Random16() & 255) << 11;
					gSparks[i].velocity_y = ((Random16() & 255) - 128) << 11;
					gSparks[i].velocity_z = ((Random16() & 255) - 128) << 11;
				}
				if (gSparks[i].x > 0)
					gSparks[i].velocity_x = -gSparks[i].velocity_x;
			}
			gSparks[i].variant = -1;
		}
	}

	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &target_position);
	EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &alternate);
	target_position.y += (alternate.y - target_position.y) / 2;

	for (frame = 0; frame != TwelveMode_Records[mode * 7 + 6]; frame++)
	{
		s32 span = TwelveMode_Records[mode * 7 + 1] * TwelveMode_Records[mode * 7 + 4];
		s32 length = span * 4;
		s32 glint = 0;

		if (TwelveMode_Records[mode * 7] == 0)
		{
			if (frame < span * 6)
			{
				s32 picture = frame / TwelveMode_Records[mode * 7 + 4] % 6;

				if (work->effect->side == 1)
				{
					BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 2);
					gBlitters[HEAP_SLOT_BLITTER](canvas,
						work->sheet + TwelveMode_StrikeOffsets[picture],
						target_position.x / 2 - (TwelveMode_StrikeReach[picture] / 2)
							- TwelveMode_StrikeWidths[picture] + 8,
						target_position.y - (TwelveMode_StrikeHeights[picture] / 2),
						TwelveMode_StrikeWidths[picture], TwelveMode_StrikeHeights[picture]);
				}
				else
				{
					BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
					gBlitters[HEAP_SLOT_BLITTER](canvas,
						work->sheet + TwelveMode_StrikeOffsets[picture],
						target_position.x / 2 + (TwelveMode_StrikeReach[picture] / 2) - 8,
						target_position.y - (TwelveMode_StrikeHeights[picture] / 2),
						TwelveMode_StrikeWidths[picture], TwelveMode_StrikeHeights[picture]);
				}
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
				if (frame % (TwelveMode_Records[mode * 7 + 4] * 6)
					== TwelveMode_Records[mode * 7 + 4] * 4)
				{
					if (mode == 8)
					{
						BattleEventRuntime_BeginPhaseFar(134);
					}
					else
					{
						AudioCommand_PlayFar(133);
						BattleEventRuntime_BeginPhaseFar(133);
					}
					if (TwelveMode_Records[mode * 7 + 2] & 16)
					{
						work->shake_frames = 8;
						ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 12);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
						for (i = 0; i != 512; i++)
							gSparks[i].variant = (Random16() & 15) + 15;
					}
					else
					{
						work->shake_frames = 4;
						ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
						for (i = 0; i != 32; i++)
						{
							struct EffectStep *step =
								&gSparks[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 32 + i];

							step->variant = (Random16() & 15) + 7;
						}
					}
					for (i = 0; i != 8; i++)
					{
						struct EffectStep *step =
							&work->particles[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 16
											 + i];

						step->variant = 0;
					}
				}
			}
			if (frame >= 12 && frame <= 31)
				glint = 1;
		}
		else if (frame < length + 4)
		{
			s32 stage;

			if (frame < length)
			{
				s32 picture = frame / TwelveMode_Records[mode * 7 + 4];

				while (picture > 4)
					picture -= 4;
				stage = TwelveMode_ColumnStages[picture];
			}
			else
			{
				stage = 3;
			}
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, TwelveMode_Records[mode * 7 + 5]);
			gBlitters[HEAP_SLOT_BLITTER](
				canvas, work->sheet + stage * 864 + 0xc80, target_position.x / 2 - 18, 56, 18, 48);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, TwelveMode_Records[mode * 7 + 5]);
			gBlitters[HEAP_SLOT_BLITTER](
				canvas, work->sheet + stage * 864 + 0xc80, target_position.x / 2, 56, 18, 48);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			if (frame % (TwelveMode_Records[mode * 7 + 4] * 4)
				== TwelveMode_Records[mode * 7 + 4] * 3)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
				work->shake_frames = 4;
				if (frame
					> (TwelveMode_Records[mode * 7 + 1] * 4 - 4) * TwelveMode_Records[mode * 7 + 4])
					BattleEventRuntime_BeginPhaseFar(133);
				else
					AudioCommand_PlayFar(133);
				for (i = 0; i != 64; i++)
				{
					struct EffectStep *step =
						&gSparks[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 64 + i];

					step->variant = (Random16() & 15) + 7;
				}
				for (i = 0; i != 8; i++)
				{
					struct EffectStep *step =
						&work->particles[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 16 + i];

					step->variant = 0;
				}
				for (i = 0; i != 16; i++)
					PARTICLES[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 16 + i].variant = 0;
			}
			if (stage == 3)
				glint = 1;
		}

		if ((TwelveMode_Records[mode * 7 + 2] & 4) && glint)
		{
			for (i = 0; i != 3; i++)
			{
				s32 image = frame & 3;
				s32 angle = Random16() & 0xffff;
				s32 range = 31;
				s32 radius = Random16() & range;
				s32 x;
				s32 y;

				x = target_position.x / 2 + (Trig_Sin(angle) * (radius + 4) >> 17)
					- (BattleFx_GlintCellWidths[image] / 2);
				y = target_position.y - (Trig_Cos(angle) * (radius + 4) >> 17)
					- (BattleFx_GlintCellHeights[image] / 2);
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
					TwelveMode_GlintDrawFlags[Random16() & 3] | 3, 3);
				(*lower)(canvas, work->sheet + BattleFx_GlintCellOffsets[image], x, y,
					BattleFx_GlintCellWidths[image], BattleFx_GlintCellHeights[image]);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			}
		}

		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
		draw[0] = gBlitters[HEAP_SLOT_BLITTER];
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
		draw[1] = *lower;
		for (k = 0; k != 1; k++)
		{
			source = GetBattleObjectSlotFar(work->effect->actor)->object;
			if (TwelveMode_Records[mode * 7 + 2] & 1)
			{
				for (i = 0; i != 64; i++)
				{
					struct EffectStep *step = &work->particles[i];

					if (step->variant >= 0 && step->variant <= 23)
					{
						s32 picture = step->variant / 4;

						draw[i & 1](canvas, work->sheet + picture * 1152 + wisp_sheet,
							HI(step->x) - 12, HI(step->y) - 24, 24, 48);
						EffectStep_AdvanceWithGravity2D(step, 62, -0x400);
						step->variant++;
					}
				}
			}
			if (TwelveMode_Records[mode * 7 + 2] & 2)
			{
				s32 size = 3;

				if (mode == 11)
					size = 8;
				if (frame == 55)
					ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
				if (frame == 90)
					ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
				for (i = 0; i != 64; i++)
				{
					struct EffectStep *step = &PARTICLES[i];

					if (step->variant >= 0)
					{
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &position);
						position.x >>= 1;
						draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
							position.x - ((u32)size >> 1), position.y - size, size, size * 2);
						EffectStep_AdvanceWithGravity3D(step, 60, 0);
						step->variant++;
						if (step->variant > 10)
						{
							s32 dx = (source->x - step->x) >> 8;
							s32 dy = (source->y - step->y + 0x140000) >> 8;
							s32 dz = (source->z - step->z) >> 8;

							step->velocity_x += dx;
							step->velocity_y += dy;
							step->velocity_z += dz;
							if ((dx >= -0xfff && dx <= 0xfff) && (dz >= -0xfff && dz <= 0xfff))
								step->variant = -1;
						}
					}
				}
			}
			if (TwelveMode_Records[mode * 7 + 2] & 8)
			{
				for (i = 0; i != 128; i++)
				{
					struct EffectStep *step = &gSparks[i];
					s32				   size = step->variant;

					if (size >= 0)
					{
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &position);
						position.x >>= 1;
						EffectStep_AdvanceWithGravity3D(step, 60, 0);
						size >>= 3;
						size++;
						draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
							position.x - size / 2, position.y - size, size, size * 2);
						step->variant--;
					}
				}
			}
		}
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		if (TwelveMode_Records[mode * 7 + 2] & 16)
			Camera_ApplyShake(8, 8);
		else
			Camera_ApplyShake(2, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// flipping_burst.c
// バトル演出：逆さに跳ぶふたりと閃光、剣の雨、柱とらせんの粒、割れて飛ぶ絵
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 使い手を最初の対象との間の 9/10 の位置に置き、ふたりで跳んで逆さになる
// 54 フレーム目に対象の色を変えてふたりを跳ね返らせ、閃光を輪に広げながら昇らせる
//--------------------------------------------------------------------------
void BattleFx_RunFlippingBurst(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	/* FAKEMATCH: frame local: an unused second blitter word fills the
	   frame's 8-byte draw slot, as this file's two-blitter effects declare
	   the pair. Measured (2026-10-13): with draw[1] the frame is 80 bytes
	   for 84 (differ 14, stack offsets only). */
	BlitterPair				 draw;
	struct BtlObj			*first;
	struct BtlObj			*second;
	struct MotionObject		*user;
	struct MotionObject		*target;
	s32						 x;
	s32						 z;
	s32						 user_height;
	s32						 target_height;
	s32						 point[3];
	struct EffectPosition	 screen;
	s32						 frame;
	s32						 percent;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1, 1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	draw[0] = heap_cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	first = GetBattleObjectSlotFar(work->effect->actor);
	second = GetBattleObjectSlotFar(work->effect->actors[0]);
	user = first->object;
	target = second->object;
	percent = 90;
	x = user->x + percent * (target->x - user->x) / 100;
	z = user->z + percent * (target->z - user->z) / 100;
	user_height = Battle_GetObjectTableValueFar(work->effect->actor);
	target_height = Battle_GetObjectTableValueFar(work->effect->actors[0]);
	ObjectDispatch_InitFromTable6Far(user);
	Object_SetMoveTargetFar(user, x, 0, z);
	ObjectDispatch_ApplyArgumentToChildrenFar(user, 2);
	user->snap_to_target = 1;
	user->auto_face_motion = 1;
	user->acceleration = 2 << 16;
	user->speed_limit = 8 << 16;
	WaitFrames(20);
	for (frame = 0; frame != 96; frame++)
	{
		struct BattleCamera *camera;

		camera = gCameraWork;
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

		// 0：ふたりで跳ぶ、11：逆さにして体の高さだけ持ち上げる
		if (frame == 0)
		{
			target->velocity_y = 15 << 16;
			target->vertical_motion_strength = 0x91eb;
			user->velocity_y = 15 << 16;
			user->vertical_motion_strength = 0x91eb;
		}
		if (frame == 11)
		{
			target->scale_y = -target->scale_y;
			user->scale_y = -user->scale_y;
			user->y += user_height;
			target->y += target_height;
		}

		// 54：対象の色を変え、ふたりを跳ね返らせる
		if (frame == 54)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 10);
			target->velocity_y = 8 << 16;
			target->vertical_motion_strength = 0xab85;
			user->velocity_y = 5 << 16;
			user->vertical_motion_strength = 0x7851;
			user->acceleration = 1 << 16;
			user->speed_limit = 2 << 16;
			user->auto_face_motion = 0;
			ObjectDispatch_InitFromTable6Far(user);
			Object_SetMoveTargetFar(user, 0, 0, user->z);
		}
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		point[0] = user->x;
		point[1] = user->y;
		point[2] = user->z;
		EffectPosition_ApplyBaseAndYOffset(point, &screen);
		screen.x >>= 1;

		// 54〜55：大きな閃光、56〜67：16 個の閃光が半径を広げながら昇る
		if (frame == 54 || frame == 55)
			draw[0](canvas, work, screen.x - 16, screen.y - 16, 32, 64);
		if (frame >= 56 && frame <= 67)
		{
			s32 i;

			for (i = 0; i != 16; i++)
			{
				s32 angle;
				s32 rx;
				s32 ry;

				angle = i << 12; // 22.5° ずつ
				rx = screen.x + ((Trig_Sin(angle) * (frame - 46)) >> 16);
				ry = ((Trig_Cos(angle) * (frame - 46)) >> 16) - frame + 100;
				draw[0](canvas, work->sheet + (((frame - 56) / 2) << 11), rx - 16, ry, 32, 64);
			}
		}

		// 64：向きと高さを元に戻す
		if (frame == 64)
		{
			target->scale_y = -target->scale_y;
			user->scale_y = -user->scale_y;
			user->y -= user_height;
			target->y -= target_height;
			ObjectDispatch_ApplyArgumentToChildrenFar(user, 0);
		}
		if (frame == 54)
			BattleEventRuntime_BeginPhaseFar(134);
		if (frame == 0)
		{
			AudioCommand_PlayFar(136);
			work->shake_frames = 6;
		}
		if (frame == 53)
			work->shake_frames = 6;
		Camera_ApplyShake(16, 16);
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 剣の雨：16 本の剣が 4 フレームおきに降り、中央 (x = 64) へ寄りながら落ちて刺さる
// 刺さった剣は下 4 ドットを描かない。画面は 16 フレームで現れ、104 から消える
//--------------------------------------------------------------------------
void BattleFx_RunBladeRain(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 pos;
	BlitterPair				 draw;
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			*blit;
	struct EffectStep		*blade;
	s32						 frame;
	s32						 i;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 等倍
	REG_BLDALPHA = 0x1000;

	// [0] そのまま、[1] 左右反転 (どちらも 0 以外の画素だけ描く)
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	draw[0] = (DrawRectangle)heap_cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_NURI);
	draw[1] = (DrawRectangle)heap_cache[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	blit = draw;
	Resource_LoadAndDecompress((s32)&ResourceId_SwordSlashSheet, work->sheet, 1, 1);
	work->transfer_mode = 1; // 転送のたびにキャンバスを 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &pos);
	REG_BG2X = (64 - pos.x) << 8;

	// 剣：x 16〜111、中央から遠いほど横の速さ (2 ドット単位) を大きくする
	for (i = 0; i != 64; i++)
	{
		s32 column;

		blade = &work->particles[i];
		column = Random16() % 96 + 16;
		blade->x = column;
		blade->y = (24 - i / 4) << 16;
		if (column <= 43)
			blade->velocity_x = 3;
		else if (column <= 51)
			blade->velocity_x = 2;
		else if (column <= 59)
			blade->velocity_x = 1;
		else if (column <= 67)
			blade->velocity_x = 0;
		else if (column <= 75)
			blade->velocity_x = -1;
		else if (column <= 83)
			blade->velocity_x = -2;
		else
			blade->velocity_x = -3;
		blade->velocity_x <<= 17;
		blade->velocity_y = 8 << 16;
		blade->x <<= 16;
	}
	AudioCommand_PlayFar(212);
	for (frame = 0; frame != 120; frame++)
	{
		if (frame <= 16)
		{
			REG_BLDALPHA = frame | 0x1000;
			if (frame == 16)
				REG_BLDCNT = 0;
		}
		if (frame > 103)
		{
			REG_BLDALPHA = (120 - frame) | 0x1000;
			if (frame == 104)
				REG_BLDCNT = 0x3f44;
		}

		// 絵は横の速さで選び、左へ進む剣は左右反転して描く
		for (i = 15; i != -1; i--)
		{
			s32 drift;
			s32 speed;
			s32 cell;

			blade = &work->particles[i];
			drift = blade->velocity_x;
			speed = drift;
			if (drift < 0)
				speed = -drift;
			cell = speed >> 17;
			if (frame < i * 4 + 25)
			{
				u32 wide;
				u32 high;

				blit[(u32)drift >> 31](canvas, work->sheet + BladeRain_CellSourceOffsets[cell],
					(blade->x >> 16) - ((wide = BladeRain_CellWidths[cell]) >> 1),
					(blade->y >> 16) - ((high = BladeRain_CellHeights[cell]) >> 1), wide, high);
				if (frame >= i * 4 + 16)
				{
					blade->x += blade->velocity_x;
					blade->y += blade->velocity_y;
				}
			}
			else
			{
				u32 wide;
				u32 high;

				blit[(u32)drift >> 31](canvas, work->sheet + BladeRain_CellSourceOffsets[cell],
					(blade->x >> 16) - ((wide = BladeRain_CellWidths[cell]) >> 1),
					(blade->y >> 16) - ((high = BladeRain_CellHeights[cell]) >> 1), wide, high - 4);
			}
		}

		// 24〜84 フレームの 4 フレームごとに対象のパレットを 2 フレーム 7 番に替えて
		// アニメ 5 にし、画面を揺らし、8 フレームごとに音を鳴らす
		if (frame >= 23 && frame <= 87 && (frame & 3) == 0)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 2);
			work->shake_frames = 1;
			if ((frame & 7) == 0)
				AudioCommand_PlayFar(133);
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

// PARTICLES : らせんの粒 (128 個)

//--------------------------------------------------------------------------
// 使い手の足元に左右対称の柱 (IceTile の絵) を立て、周りにらせんを描いて昇る粒を出す
// effect->kind	   : パレット (0 YellowPaletteA、1 IceTile、2 RedPaletteA、他 VioletPaletteA)
// effect->variant : 1 足して 1〜4 に収め、粒の大きさにする
//--------------------------------------------------------------------------
void BattleFx_RunIceFleckBurst(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 ground;
	struct EffectPosition	 pos;
	s32						 point[3];
	struct BattleEffectWork *work;
	void					*canvas;
	BlitterPair				 draw;
	u8						*sheet;
	struct BattleCamera		*camera;
	s32						 frame;
	void					*palette;
	s32						 i;

	work =
		(struct BattleEffectWork *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
	canvas = (void *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
	sheet = (u8 *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x60e);
	camera = gCameraWork;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	work->fade_frames = 24;
	work->fade_step = 0;
	REG_BLDALPHA = 0x100c;
	REG_BG2PA = 0x100; // 等倍
	Resource_LoadAndDecompress((s32)&ResourceId_IceTileSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
	switch (work->effect->kind)
	{
	case 0:
		palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
		break;
	case 1:
		palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
		break;
	case 2:
		palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
		break;
	default:
		palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
		break;
	}
	Iwram_CopyWords((void *)BG_PLTT, palette, 128);

	// 粒：x は角度、z は半径 (1/16 ドット単位)、i フレーム待ってから昇る
	for (i = 0; i != 128; i++)
	{
		PARTICLES[i].y = 0;
		PARTICLES[i].x = Random16() & 0xffff;
		PARTICLES[i].z = (Random16() & 0x1ff) + i * 2;
		PARTICLES[i].variant = -i;
	}
	/* FAKEMATCH: the cancelling pointer updates retain work's allocation
	 * priority without operand asm. Without them, work and the cell index
	 * exchange r9/r11 and two spill slots swap (420 instructions, 50 rows).
	 * The adjacent scheduling asm is unnecessary with these updates. */
	work++;
	work--;
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
	draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
	work->effect->variant++;
	if (work->effect->variant <= 0)
		work->effect->variant = 1;
	if (work->effect->variant > 4)
		work->effect->variant = 4;
	AudioCommand_PlayFar(212);

	for (frame = 0; frame != 56; frame++)
	{
		EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &ground);
		REG_BG2X = (64 - ground.x) << 8;
		if (frame > 49)
			REG_BLDALPHA = (112 - frame * 2) | 0x1000;
		if (frame == 16)
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, -1, 0, 20);

		// 柱：左半分 (x = 47) と左右反転した右半分 (x = 64) を比較合成で描く
		// 柱の絵 (17×64) は 2 フレームごとに 4 枚、光 (24×43) は 4 フレームごとに 3 枚
		if (frame <= 55)
		{
			s32 n;
			u8 *glow;

			i = frame / 2;
			n = i % 4;
			BattleEffect_LoadWork(
				HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
			draw[1] = ((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE];
			draw[1](canvas, work->sheet + n * 1088, 47, ground.y - 64, 17, 64);
			n = frame / 4 % 3;
			glow = work->sheet + n * 1032 + 0x1100;
			draw[1](canvas, glow, 40, ground.y - 36, 24, 43);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
				GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
			draw[1] = ((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE];
			n = i % 4;
			draw[1](canvas, work->sheet + n * 1088, 64, ground.y - 64, 17, 64);
			draw[1](canvas, glow, 64, ground.y - 36, 24, 43);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		}
		GetBattleObjectSlotFar(work->effect->actor);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

		// 粒 (最初の 32 個)：1 フレームに 0x400 回り、5 ドット昇り、半径を 4 ドット広げる
		for (i = 0; i != 32; i++)
		{
			struct EffectStep *fleck = &PARTICLES[i];

			if (fleck->variant >= 0)
			{
				s32 size;

				point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
				point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
				point[1] = fleck->y;
				fleck->x += 0x400;
				fleck->y += 5 << 16;
				fleck->z += 64;
				EffectPosition_ApplyBaseAndYOffset(point, &pos);
				pos.x /= 2;
				size = (i & 1) + work->effect->variant;
				draw[0](canvas, sheet + BattleFx6_FlareCells[size - 1], pos.x - size, pos.y - size,
					size * 2, size * 2);
			}
			fleck->variant++;
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
	/* FAKEMATCH: cancelling frame updates preserve the allocation lifetimes
	 * without the two trailing asm statements. Deleting both swaps the
	 * ground and camera-position pointers at sp+8/sp+12 (420 instructions,
	 * 18 differing rows); one dead increment leaves the same swap. */
	frame ^= 1;
	frame ^= 1;
}

// 割れて飛ぶ 33 片：始まりの位置、大きさ、絵の位置
// 256×120 の 8bpp タイル並び (横 32 タイル) の点 (x, y) に色 2 を置く
#define PLOT(x, y) \
	(*(Ram_MapCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 + (y) / 8 * 2048) = 2)

//--------------------------------------------------------------------------
// 中心 (96, 60) から灰色の円が 1 フレームに半径 32 ずつ広がり、
// TileAnimationMask の絵を 33 片に割って飛ばしながら、パレット 6〜13 を
// 白から戦闘背景の色へ戻す。51 フレームからは閃光 32 個が昇る
//--------------------------------------------------------------------------
void BattleEffect_RunTileAndPaletteAnimation(struct BattleEffectArgument *effect)
{
	/* FAKEMATCH: needed to match: never read: the native frame keeps 12 unused bytes at sp+44..55, exactly this struct 2026-10-11: without it only the frame shrinks (differ 4). */
	struct EffectPosition		 position;
	struct BattleEffectWork		*work;
	void						*canvas;
	s32							 frame;
	BlitterPair					 draw;
	struct BtlWork				*battle;
	struct BattleBackgroundView *view;
	void					   **slots;
	void					   **cache;
	s32							 rmax;
	struct EffectStep			*piece;
	s32							 r;
	s32							 i;
	s32							 px;
	s32							 j;
	s32							 k;
	s32							 d;
	s32							 py;
	s32							 x1;
	s32							 size;
	s32							 left;
	s32							 left2;
	s32							 y1;
	s32							 x2;
	s32							 y2;
	u8							*start_y;
	u8							*start_x;
	s32							 cx;
	s32							 cy;

	work = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(*work));
	size = 0x4000;
	canvas = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, size);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x302);
	slots = gWorkSlot + HEAP_SLOT_BATTLE;
	battle = slots[0];
	view = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
	work->effect = effect;

	// BG2 を 128×128 のキャンバスにする (BattleFx_BeginCanvasLayer と同じ設定)
	BattleFx_ClearActorTimers();
	view->second_mode = 1;
	gBgScroll[1].y = 32;
	BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 0);
	REG_BG2CNT = 0x784;
	BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
	view->second_mode = 0;
	REG_BG2X = 0;
	REG_BG2Y = 0xfffff000;
	REG_BG2PA = 0x80;
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	REG_BG2PD = 0x100;
	REG_WIN0H = 0xf0;
	REG_WIN0V = 0x1088;
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;

	i = 0;
	for (k = 0; k != 16; k++)
	{
		for (j = 0; j != 8; j++, i += 2)
		{
			s16 entry;

			entry = ((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
			*(volatile s16 *)(0x06003800 + i) = entry;
		}
	}

	Iwram_ClearWords(canvas, size);
	Iwram_ClearWords((void *)0x06004000, size);
	IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
	REG_BLDALPHA = 0x1010;
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_TileAnimationMask, work, 1, 1);
	work->transfer_mode = 1; // 転送のたびにキャンバスを 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	cache = gWorkSlot;
	draw[0] = cache[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(
		HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw[1] = cache[HEAP_SLOT_BLITTER_ALTERNATE];

	// 片：中心 (32, 60) から外へ、距離の 1/4 の速さで飛ぶ
	start_y = BattlePieceStartY;
	start_x = BattlePieceStartX;
	r = 0;
	piece = work->particles;
	for (i = 0; i != 33; i++)
	{
		s32 x, y, vx, vy;

		x = *start_x++ << 16;
		y = *start_y++ << 16;
		piece->x = x;
		piece->y = y;
		vx = (x - 0x200000) >> 2;
		vy = (y - 0x3c0000) >> 2;
		piece->velocity_x = vx;
		piece->velocity_y = vy;
		piece++;
	}

	// VRAM 0x06008000 の絵を写してから色 1 で埋め、パレット 6〜13 を白にする
	Iwram_CopyWords(Ram_MapCellBuffer, (void *)0x06008000, 0x7800);
	Iwram_FillWords(Ram_MapCellBuffer, 0x7800, 0x01010101);
	view->busy = 1;
	work->saved_bg1_x = gBgScroll[1].x;
	work->saved_bg1_y = gBgScroll[1].y;
	gBgScroll[1].x = 0;
	IO_WRITE_QUEUE_PUSH(0x0400000a, 0x1f81, 0x20000);
	Iwram_FillWords((void *)0x050000c0, 0x100, 0x7fff7fff);
	AudioCommand_PlayFar(212);
	ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 3, 0, 30);

	for (frame = 0; frame != 128; frame++)
	{
		if (frame == 2)
			AudioCommand_PlayFar(212);
		if (frame == 3)
			AudioCommand_PlayFar(212);
		if (frame == 28)
			ObjectGroup_UpdateMembers(work->effect->actors[0], -1, 3, -1, 0);
		if (frame == 32)
			AudioCommand_PlayFar(149);
		if (frame == 5)
			AudioCommand_PlayFar(145);
		if (frame == 5)
		{
			gBgScroll[1].x = work->saved_bg1_x;
			BattleBackground_LoadFar(1, battle->background, -1);
		}

		// 8 フレームから、パレット 6〜13 の各色を 1 段ずつ戦闘背景の色へ近づける
		if (frame > 7)
		{
			u16 *pal;

			pal = (u16 *)0x050000c0;
			for (i = 0; i != 128; i++)
			{
				s32 red = *pal & 0x1f;
				s32 green = (*pal >> 5) & 0x1f;
				s32 blue = (*pal >> 10) & 0x1f;
				s32 goal_red = battle->palette[i] & 0x1f;
				s32 goal_green = (battle->palette[i] >> 5) & 0x1f;
				s32 goal_blue = (battle->palette[i] >> 10) & 0x1f;

				if (red < goal_red)
					red++;
				else if (red > goal_red)
					red--;
				if (green < goal_green)
					green++;
				else if (green > goal_green)
					green--;
				if (blue < goal_blue)
					blue++;
				else if (blue > goal_blue)
					blue--;
				*pal++ = (blue << 10) | (green << 5) | red;
			}
		}

		if (frame == 4)
			Iwram_FillWords((void *)0x06008000, 0x7800, 0x02020202);

		// 0〜3 フレーム：円を半径 32 ずつ広げ、色 2 の灰色を明るくしていく
		if (frame <= 3)
		{
			s32 v;

			v = frame * 4 + 8;
			rmax = frame * 32;
			BG_PLTT[2] = (v << 10) | (v << 5) | v;
			cx = 96;
			cy = 60;
			for (; r != rmax; r++)
			{
				// 中点円の描き方で、8 分の 1 ずつ対称に点を置く
				px = r;
				py = 0;
				d = r;
				while (px >= py)
				{
					x2 = px + cx;
					x1 = cx - px;
					left = x1;
					y1 = cy - py;
					y2 = py + cy;
					if (y1 < 0)
						y1 = 0;
					if (y2 > 119)
						y2 = 119;
					if (x1 < 0)
						x1 = 0;
					if (x2 > 255)
						x2 = 255;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);
					x1 = left + 1;
					x2 = px + 97;
					if (x1 < 0)
						x1 = 0;
					if (x2 > 255)
						x2 = 255;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);

					x2 = py + cx;
					x1 = cx - py;
					left2 = x1;
					y2 = px + cy;
					y1 = cy - px;
					if (x1 < 0)
						x1 = 0;
					if (x2 > 255)
						x2 = 255;
					if (y1 < 0)
						y1 = 0;
					if (y2 > 119)
						y2 = 119;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);
					x1 = left2 + 1;
					x2 = py + 97;
					if (x1 < 0)
						x1 = 0;
					if (x2 > 255)
						x2 = 255;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);

					d -= py * 2 + 1;
					if (d < 0)
					{
						d += px * 2 - 2;
						px--;
					}
					py++;
				}
			}
			Iwram_CopyWords((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
		}

		// 50 フレームまで片を描き、4 フレームから落とす
		if (frame <= 50)
		{
			for (i = 0; i != 33; i++)
			{
				piece = &work->particles[i];
				draw[0](canvas, (u8 *)work + BattlePieceOffset[i], HI(piece->x), HI(piece->y),
					BattlePieceWidth[i], BattlePieceHeight[i]);
				if (frame > 3)
					EffectStep_AdvanceWithGravity2D(piece, 64, 0x4000);
			}
		}

		// 8〜50 フレーム：色 1 を黒から白へ
		if (frame >= 8 && frame <= 50)
		{
			s32 v;

			v = frame - 8;
			if (v > 31)
				v = 31;
			BG_PLTT[1] = (v << 10) | (v << 5) | v;
		}

		// 51 フレーム：閃光の絵と色を用意し、x 32〜63、y 80〜111 に 32 個を置く
		if (frame == 51)
		{
			Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work, 1, 0);
			for (i = 1; i != 64; i++)
			{
				s32 v;

				v = i / 2;
				if (v < 0)
					v = 0;
				BG_PLTT[i] = (v << 10) | ((v / 2) << 5) | v;
			}
			REG_BLDCNT = 0x3f44;
			for (i = 0; i != 32; i++)
			{
				piece = &work->particles[i];
				piece->x = ((Random16() & 31) + 32) << 16;
				piece->y = ((Random16() & 31) + 80) << 16;
				piece->velocity_x = ((Random16() & 0x1ff) - 0x100) << 12;
				piece->velocity_y = 0;
				piece->variant = 0;
			}
			work->transfer_mode = 2;
			work->transfer_value = 50; // 転送のたびに半分へ減らして残像にする
		}

		// 閃光：i/4 フレームずつ遅れて出て、4 フレームごとに絵を進めながら昇る
		if (frame > 52)
		{
			for (i = 0; i != 32; i++)
			{
				piece = &work->particles[i];
				if (frame >= i / 4 + 52 && piece->variant <= 39)
				{
					s32 cell;

					cell = piece->variant / 4;
					if (cell > 5)
						cell = 5;
					draw[1](canvas, (u8 *)work + (cell << 11), HI(piece->x) - 16, HI(piece->y) - 32,
						32, 64);
					EffectStep_AdvanceWithGravity2D(piece, 60, -0x1000);
					piece->variant++;
				}
			}
		}

		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	// 背景と表示を元に戻す
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	ObjectGroup_UpdateMembers(work->effect->actors[0], -1, 1, -1, 0);
	gBgScroll[1].x = work->saved_bg1_x;
	gBgScroll[1].y = 32;
	BattlePresentation_ConfigurePaletteFadeFar(2, battle->background, 0);
	WaitFrames(1);
	IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}

// 光の筋 1 本。PARTICLES と同じ 28 バイトの置き場を別の意味で読む
struct Streak
{
	s32 head;
	s32 tail;
	s32 unused_08;
	s32 pitch;
	s32 yaw;
	s32 roll;
	s32 unused_18;
};

void BattleFx_RunGatheringBurst(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 ground;
	struct BattleEffectWork *work;
	void					*canvas;
	BlitterPair				 draw;
	u8						*sheet;
	void					*palette;
	s32						 frame;
	s32						 i;

	work = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
	canvas = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
	sheet = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x60e);
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	work->fade_frames = 24;
	work->fade_step = 0;
	REG_BLDALPHA = 0x100c;
	REG_BG2PA = 0x100;
	Resource_LoadAndDecompress((s32)&ResourceId_CyanSparkSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
	switch (work->effect->kind)
	{
	case 0:
		palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
		break;
	case 1:
		palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
		break;
	case 2:
		palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
		break;
	default:
		palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
		break;
	}
	{
		Iwram_CopyWords((void *)0x05000000, palette, 128);
	}

	for (i = 0; i != 128; i++)
	{
		PARTICLES[i].y = 0x800000;
		PARTICLES[i].x = Random16() & 0xffff;
		PARTICLES[i].z = (Random16() & 0x1ff) + 0x400;
		PARTICLES[i].variant = -i;
	}
	for (i = 0; i != 64; i++)
	{
		work->particles[i].x = Random16() & 0xffff;
		work->particles[i].y = (Random16() & 31) + 16;
		work->particles[i].variant = (i & 15) + 16;
	}
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 3);
	draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	AudioCommand_PlayFar(140);

	for (frame = 0; frame != 56; frame++)
	{
		EffectPosition_ApplyStepAndYOffset(effect->actor, &ground);
		REG_BG2X = (64 - ground.x) << 8;
		if (frame > 49)
			REG_BLDALPHA = (0x70 - frame * 2) | 0x1000;
		if (frame == 26)
		{
			AudioCommand_PlayFar(212);
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, -1, 0, 20);
		}
		if (frame >= 28 && frame <= 48)
			draw[0](
				canvas, work->sheet + (frame - 28) / 3 * 2304 + 0x1400, 40, ground.y - 24, 48, 48);
		if (frame >= 0 && frame <= 14)
		{
			s32			   picture = frame / 3 % 5;
			DrawRectangle *slot = &((DrawRectangle *)gWorkSlot)[47];

			for (i = 0; i != 4; i++)
			{
				BattleEffect_LoadWork(
					HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CornerSparks_DrawFlags[i] | 3, 2);
				draw[1] = *slot;
				draw[1](canvas, work->sheet + (picture << 10), CornerSparks_X[i] + 32,
					ground.y + CornerSparks_Y[i] - 32, 32, 32);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			}
		}
		if (frame >= 0)
		{
			for (i = 0; i != 64; i++)
			{
				struct EffectStep *mote = &work->particles[i];

				if (mote->variant >= 0 && mote->y > 0)
				{
					s32 size = (mote->variant >> 3) + 1;
					s32 x = (mote->y * Trig_Sin(mote->x) >> 16) + 64;
					s32 y = (mote->y * Trig_Cos(mote->x) >> 16) + ground.y;

					if (size <= 0)
						size = 1;
					draw[0](canvas, sheet + BattleFx6_FlareCells[size - 1], x - size, y - size,
						size * 2, size * 2);
					mote->y -= 2;
					mote->variant--;
				}
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}

void BattlePresentation_DrawStreaks(void)
{
	struct BattleEffectWork *work = gBattleFxWork[0];
	s32						 frame;
	s32						 i;
	struct Streak			*streak;
	struct Vector3			 point;
	struct EffectPosition	 head;
	struct EffectPosition	 tail;

	frame = (work->frame)++;
	if (frame == 0)
	{
		for (i = 0; i != 256; i++)
		{
			struct Streak *seed = &((struct Streak *)Ram_MapCellBuffer)[i];
			s32			   r = Random16() & 15;

			seed->head = r + 48;
			seed->tail = r + 40;
			seed->pitch = Random16() & 0xffff;
			seed->yaw = Random16() & 0xffff;
			seed->roll = Random16() & 0xffff;
		}
	}
	point.y = 0;
	point.z = 0;
	i = 0;
	for (; i != 64; i++)
	{
		streak = &((struct Streak *)Ram_MapCellBuffer)[i];
		if (frame > i / 4 && streak->head > 0)
		{
			s32 fade;

			Render_ResetTransformState();
			SceneTransform_ApplyRoll(streak->roll);
			SceneTransform_ApplyPitch(streak->pitch);
			SceneTransform_ApplyYaw(streak->yaw);
			point.x = streak->head;
			EffectPosition_ApplyBaseAndYOffset((s32 *)&point, &head);
			head.x += 64;
			head.y += 80;
			point.x = streak->tail;
			EffectPosition_ApplyBaseAndYOffset((s32 *)&point, &tail);
			tail.x += 64;
			tail.y += 80;
			streak->tail -= 4;
			streak->head -= 4;
			if (streak->tail < 0)
				streak->tail = 0;
			fade = -streak->tail / 2;
			BattleFx_DrawClippedCanvasLine(tail.x - 1, tail.y, head.x - 1, head.y, fade + 48);
			BattleFx_DrawClippedCanvasLine(tail.x, tail.y - 1, head.x, head.y - 1, fade + 48);
			BattleFx_DrawClippedCanvasLine(tail.x, tail.y, head.x, head.y, fade + 56);
		}
	}
	work->transfer_pending = 1;
}

void BattlePresentation_PrepareScene(s32 kind)
{
	struct BattleEffectWork *work;
	void					*palette;
	s32						 id;

	work = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
	BattleFx_BeginCanvasLayer(0);
	work->fade_frames = 24;
	REG_BG2PA = 0x100;
	REG_BLDALPHA = 0x1010;
	switch (kind)
	{
	case 0:
		id = (s32)&ResourceId_YellowPaletteB;
		break;
	case 1:
		id = (s32)&ResourceId_CrystalSheet;
		break;
	case 2:
		id = (s32)&ResourceId_EmberStreakSheet;
		break;
	case 3:
		id = (s32)&ResourceId_VioletPaletteD;
		break;
	case 4:
	default:
		id = (s32)&ResourceId_DustPuffSheet;
		break;
	}
	palette = Resource_GetTableEntry(id);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	work->frame = 0;
	work->transfer_mode = 3;
	work->transfer_value = 0x06060606;
	Scheduler_AddOrUpdateCallback(BattlePresentation_DrawStreaks, 0xC80);
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
}

void BattleFx_ScheduleCallbacksAndReleaseBlocks(void)
{
	Scheduler_RemoveCallback(BattlePresentation_DrawStreaks);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	{
		Iwram_ClearWords((void *)0x06004000, 0x4000);
	}
	Scheduler_RemoveCallback(Palette_StepFadeTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}

void BattleFx_RunTwoResourceMode0(struct BattleEffectArgument *efx)
{
	BattleFx_RunTwoResource(efx, 0);
}

void BattleFx_RunTwoResourceMode1(struct BattleEffectArgument *efx)
{
	BattleFx_RunTwoResource(efx, 1);
}

void BattleFx_RunTwoResource(struct BattleEffectArgument *efx, s32 mode)
{
	struct BattleEffectWork *work;
	void					*canvas;
	struct MotionObject		*object;
	struct EffectPosition	 position;
	/* FAKEMATCH: needed to match: only draw[0] is used: the second word fills four bytes of the frame */
	BlitterPair				 draw;
	s32						 frames;
	s32						 frame;
	s32						 cell;
	void				   **heap;
	void				   **p;

	heap = gBattleFxWork;
	p = heap;
	work = *p++;
	canvas = *p;
	work->effect = efx;
	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100;
	if (work->effect->side == 1)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 1);
		draw[0] = (DrawRectangle)heap[7];
	}
	else
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 1);
		draw[0] = (DrawRectangle)heap[7];
	}
	Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet, Ram_MapCellBuffer, 1, 0);
	if (mode == 0)
	{
		void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
		Iwram_CopyWords((void *)0x05000000, palette, 128);
	}
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &position);
	if (work->effect->side == 0)
		REG_BG2X = (16 - position.x) << 8;
	else
		REG_BG2X = (112 - position.x) << 8;
	frames = 74;
	if (mode != 1)
		frames = 48;
	for (frame = 0; frame != frames; frame++)
	{
		cell = frame / 4;
		if (cell <= 5)
		{
			if (cell <= 3)
				draw[0](canvas, work->sheet + TwoResource_CellSourceOffsets[cell],
					TwoResource_CellX[cell + work->effect->side * 6],
					TwoResource_CellBiasY[cell] + 32, TwoResource_CellWidths[cell],
					TwoResource_CellHeights[cell]);
			else
				draw[0](canvas, Ram_MapCellBuffer + TwoResource_CellSourceOffsets[cell],
					TwoResource_CellX[cell + work->effect->side * 6],
					TwoResource_CellBiasY[cell] + 32, TwoResource_CellWidths[cell],
					TwoResource_CellHeights[cell]);
		}
		if (frame == 8)
		{
			if (mode == 0)
			{
				BattleEventRuntime_BeginPhaseFar(0x85);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
			}
			else
			{
				AudioCommand_PlayFar(0x86);
				ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
			}
			work->shake_frames = 8;
		}
		if (mode == 1)
		{
			if (frame == 13)
			{
				object->velocity_y = 0xc0000;
				object->vertical_motion_strength = 0x7851;
				object->vertical_motion_phase = 0x4000;
			}
			if (frame == 65)
			{
				work->shake_frames = 4;
				BattleEventRuntime_BeginPhaseFar(0x86);
			}
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunSpiderWeb(struct BattleEffectArgument *efx)
{
	struct EffectPosition	 first;
	struct EffectPosition	 last;
	void					*canvas;
	struct BattleEffectWork *work;
	s32						 size;
	s32						 step;

	/* FAKEMATCH: the frame counter is pinned to r9, as the reference keeps it. 2026-10-11: as a plain local its priority (20 refs/232 insns) ties size and outranks step (13/240), so it takes sl and step r9 (differ 30, one register swap); the reference needs step ranked above the counter, which no plain shape gives (for/while/u32, declaration order, a bound variable, early init, argument use, 19k permuter candidates). */
	register s32 frame __asm__("r9");
	s32			 i;

	/* FAKEMATCH: needed to match: size and step first hold the slot table and blend register addresses, sharing their registers 2026-10-11: plain slot reads and REG_BLDALPHA differ 56; the reads alone 40, the register alone 50. */
	size = (s32)&Ram_WorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	canvas = *(void **)size;
	work = *(struct BattleEffectWork **)(size - 4);
	work->effect = efx;
	BattleFx_BeginCanvasLayer(2);
	step = 0x04000052;
	REG_BG2PA = 0x100;
	*(u16 *)step = 0x1000;
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[work->effect->count - 1], &last);
	first.x += (last.x - first.x) / 2;
	REG_BG2X = (64 - first.x) << 8;
	Resource_LoadAndDecompress((s32)&ResourceId_SpiderWebSheet, work->sheet, 1, 1);
	work->transfer_mode = 1;
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(143);
	frame = 0;
	step = 1;
	size = 32;
	do
	{
		if (frame <= 8)
			REG_BLDALPHA = (frame << 1) | 0x1000;
		if (frame > 53)
			REG_BLDALPHA = (0x7c - (frame << 1)) | 0x1000;
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, step);
		((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 33, 41, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, step);
		((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 64, 41, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 11, step);
		((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 33, 72, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 15, step);
		((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 64, 72, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		if (frame == 32)
			BattleEventRuntime_BeginPhaseFar(143);
		for (i = 0; i != work->effect->count; i++)
		{
			if (frame == 10)
				ObjectGroup_UpdateMembers(work->effect->actors[i], 7, -1, i, 8);
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = step;
		WaitFrames(1);
		frame++;
	} while (frame != 63);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

void BattleEffect_WipeCanvas(s32 mode, s32 value)
{
	struct BattleEffectWork *work;
	u8						*canvas;
	u8						 delay[128];
	s32						 x;
	s32						 y;
	s32						 pos;

	canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	for (x = 0; x != 128; x++)
		delay[x] = Random16() & 0x3f;

	if (mode == 1)
	{
		s32 front;
		s32 speed;

		front = 0;
		speed = 1;
		x = 0;
		do
		{
			front += speed;
			speed++;
			for (; x != front; x++)
			{
				for (y = 0; y != 128; y++)
				{
					pos = x - delay[y];
					if (pos >= 0)
						if (pos <= 127)
							canvas[(((y / 8) * 16 + pos / 8) * 8 + (y & 7)) * 8 + (pos & 7)] =
								1 - value;
				}
			}
			work->transfer_pending = 1;
			WaitFrames(1);
		} while (front <= 256);
	}
	else
	{
		s32 front;
		s32 speed;

		front = 0;
		speed = 1;
		y = 0;
		do
		{
			front += speed / 2;
			speed += 4;
			for (; y != front; y++)
			{
				for (x = 0; x != 128; x++)
				{
					pos = y - delay[x];
					if (pos >= 0)
						if (pos <= 127)
							canvas[(((pos / 8) * 16 + x / 8) * 8 + (pos & 7)) * 8 + (x & 7)] =
								1 - value;
				}
			}
			work->transfer_pending = 1;
			WaitFrames(1);
		} while (front <= 191);
	}
}

void BattlePresentation_ProcessPendingGraphicsTransfer(void)
{
	void				   **heap_cache = gBattleFxWork;
	struct BattleEffectWork *work = heap_cache[0];
	void					*source;
	s32						*counter;
	s32						 next_counter;

	if (work->transfer_pending == 1)
	{
		source = heap_cache[1];
		switch (work->transfer_mode)
		{
		case 0:
			Iwram_CopyWords((void *)0x06004000, source, 0x4000);
			break;
		case 1:
			Iwram_CopyWords((void *)0x06004000, source, 0x4000);
			Iwram_FillWords(source, 0x4000, work->transfer_value);
			break;
		case 2:
			if (work->transfer_value == 50)
			{
				ColorBuffer_BackupAndHalve(source, (void *)0x06004000, 0x4000);
			}
			else
			{
				ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
			}
			break;
		case 3:
			ColorBuffer_BackupAndDarken(source, work->transfer_value, (void *)0x06004000, 0x4000);
			break;
		case 4:
			ColorBuffer_BackupAndBrighten(source, work->transfer_value, (void *)0x06004000, 0x4000);
			break;
		}
		work->transfer_pending = 0;
		counter = &work->frames_since_transfer;
		next_counter = 1;
	}
	else
	{
		counter = &work->frames_since_transfer;
		next_counter = *counter + 1;
	}
	*counter = next_counter;
}

void BattleFx_FlushPendingGraphicsTransfer(void)
{
	void				   **heap_cache;
	struct BattleEffectWork *work;
	void					*source;
	s32						 transfer_mode;

	heap_cache = gBattleFxWork;
	work = heap_cache[0];
	source = Ram_MapCellBuffer;
	if (work->transfer_pending != 1)
		return;

	transfer_mode = work->transfer_mode;
	switch (transfer_mode)
	{
	case 0:
		Iwram_CopyWords((void *)0x06008000, source, 0x7800);
		break;
	case 1:
		Iwram_CopyWords((void *)0x06008000, source, 0x7800);
		Iwram_FillWords(source, 0x7800, work->transfer_value);
		break;
	case 2:
		if (work->transfer_value == 50)
		{
			ColorBuffer_BackupAndHalve(source, (void *)0x06008000, 0x7800);
		}
		else
		{
			ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06008000, 0x7800);
		}
		break;
	case 3:
		ColorBuffer_BackupAndDarken(source, work->transfer_value, (void *)0x06008000, 0x7800);
		break;
	}

	work->transfer_pending = 0;
}

#define FADE_STEP 1092

void Graphics_ApplyWindowBlendRegisters(void)
{
	struct BattleEffectWork *work = gBattleFxWork[0];

	REG_WIN0H = work->win0h;
	REG_WIN0V = work->win0v;
	REG_WIN1H = work->win1h;
	REG_WIN1V = work->win1v;
	REG_WININ = work->winin;
	REG_WINOUT = work->winout;
	REG_DISPCNT = work->dispcnt;
	REG_BLDCNT = work->bldcnt;
	REG_BLDALPHA = work->bldalpha;
}

void Display_ApplyBg2Reference(void)
{
	struct BattleEffectWork *work = gBattleFxWork[0];

	REG_BG2X = work->bg2x;
	REG_BG2Y = work->bg2y;
}

void Palette_StepFadeTransfer(void)
{
	struct BtlWork			*battle = WORK_SLOT(HEAP_SLOT_BATTLE);
	struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);

	if (work->fade_frames > 0)
	{
		s32 step = ++work->fade_step;

		Graphics_ScaleRgb555ClampedFar(
			battle->palette, (void *)&BG_PLTT_COLOR(6, 0), 0x10000 - step * FADE_STEP, 128);
		work->fade_frames--;
	}
}

void BattleFx_ClearActorTimers(void)
{
	Iwram_ClearWords((*(struct BattleEffectWork **)gBattleFxWork)->actor_timers,
		sizeof((*(struct BattleEffectWork **)gBattleFxWork)->actor_timers));
}

void ObjectGroup_TickMemberTimers(void)
{
	struct BattleEffectWork *work;
	s32						 i;

	work = gBattleFxWork[0];
	i = 0;
	do
	{
		if (work->actor_timers[i] != 0)
		{
			if ((work->actor_timers[i] = work->actor_timers[i] - 1) == 0)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 0, -1, -1, 0);
			}
		}
		i += 1;
	} while (i != 8);
}

//--------------------------------------------------------------------------
// canvas.c
// バトル演出：BG2 のキャンバス面を開く・閉じる
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// キャンバスは 128x128 の 8bpp（0x4000 バイト）で、BG2 のアフィン面に
// キャラクタブロック 1 から並べる。横は 2 倍に伸ばして 256 ドットで見せる
//
// REG_DISPCNT（モード 1・OBJ 1 次元）
//   0x7741 : BG0〜2・OBJ・WIN0/1
//   0x7341 : BG0・BG1・OBJ・WIN0/1（BG2 を消す）
//   0x7541 : BG0・BG2・OBJ・WIN0/1（BG1 を消す）
//   0x1741 : BG0〜2・OBJ
//   0x1341 : BG0・BG1・OBJ
//   0x1541 : BG0・BG2・OBJ
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// キャンバス面を開く（背景パレットをフェードさせ、BG2 をキャンバスにする）
// bg_control : REG_BG2CNT に足す値
//--------------------------------------------------------------------------
void BattleFx_BeginCanvasLayer(s32 bg_control)
{
	struct BattleEffectWork		*work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	struct BtlWork				*btl = *(gWorkSlot + HEAP_SLOT_BATTLE);
	void						*canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	struct BattleBackgroundView *display = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	s32							 row;
	s32							 col;
	s32							 ofs;

	BattleFx_ClearActorTimers();
	display->second_mode = 1;
	WaitFrames(1);
	REG_BLDCNT = 0;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7741, 0x20000);
	gBgScroll[1].y = 32;
	WaitFrames(1);

	// 背景パレットのフェードを始める
	BattlePresentation_ConfigurePaletteFadeFar(1, btl->background, 0);
	work->fade_frames = 0;
	work->fade_step = 0;
	Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);

	// BG2 を消している間に設定を変える
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
	WaitFrames(1);
	REG_BG2CNT = bg_control | 0x784;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
	BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
	display->second_mode = 0;
	WaitFrames(1);

	REG_BLDCNT = 0x3f44;   // BG2 を半透明にして下の面と重ねる
	REG_BLDALPHA = 0x100e; // 14:16
	REG_BG2X = 0;
	REG_BG2Y = -0x1000; // 16 ライン下げる
	REG_BG2PA = 0x80;	// 横 2 倍
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	REG_BG2PD = 0x100;
	REG_WIN0H = 0xf0;	// 0〜240
	REG_WIN0V = 0x1088; // 16〜136
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7741, 0x20000);

	// スクリーン：16x16 キャラクタを順に並べる（1 キャラ 1 バイトなので 2 個ずつ書く）
	ofs = 0;
	for (row = 0; row != 16; row++)
	{
		for (col = 0; col != 8; col++, ofs += 2)
		{
			s32 chr;
			s16 pair;

			chr = row * 16 + col * 2;
			pair = ((chr + 1) << 8) | chr;
			*(volatile s16 *)((u8 *)BG_CANVAS_MAP + ofs) = pair;
		}
	}

	Iwram_ClearWords(canvas, 0x4000);
	Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
	work->shake_frames = 0;
	work->saved_bg1_x = gBgScroll[1].x;
	work->saved_bg1_y = gBgScroll[1].y;
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// キャンバス面を開く（ウインドウなしで始め、fade_frames を 24 にする版）
// bg_control : REG_BG2CNT に足す値
//--------------------------------------------------------------------------
void BattleFx_OpenCanvasLayer(s32 bg_control)
{
	struct BattleEffectWork		*work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	struct BtlWork				*btl = *(gWorkSlot + HEAP_SLOT_BATTLE);
	void						*canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	struct BattleBackgroundView *display = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	s32							 row;
	s32							 col;
	s32							 ofs;

	BattleFx_ClearActorTimers();
	display->second_mode = 1;
	WaitFrames(1);
	REG_BLDCNT = 0;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x1741, 0x20000);
	gBgScroll[1].y = 32;
	WaitFrames(1);
	BattlePresentation_ConfigurePaletteFadeFar(1, btl->background, 128);
	work->fade_frames = 24;
	work->fade_step = 0;
	Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x1341, 0x20000);
	WaitFrames(1);
	REG_BG2CNT = bg_control | 0x784;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x1341, 0x20000);
	BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
	display->second_mode = 0;
	WaitFrames(1);

	REG_BLDCNT = 0x3f44;
	REG_BLDALPHA = 0x100e;
	REG_BG2X = 0;
	REG_BG2Y = -0x1000;
	REG_BG2PA = 0x80;
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	REG_BG2PD = 0x100;
	REG_WIN0H = 0xf0;
	REG_WIN0V = 0x1088;
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7741, 0x20000);
	WaitFrames(1);

	ofs = 0;
	for (row = 0; row != 16; row++)
	{
		for (col = 0; col != 8; col++, ofs += 2)
		{
			s32 chr;
			s16 pair;

			chr = row * 16 + col * 2;
			pair = ((chr + 1) << 8) | chr;
			*(volatile s16 *)((u8 *)BG_CANVAS_MAP + ofs) = pair;
		}
	}

	Iwram_ClearWords(canvas, 0x4000);
	Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// キャンバス面を開き、BG2 を 256x256 の折り返しありにする
// スクリーンの各行は 16 キャラのキャンバスと、16 キャラの 0 番
// bg_control : REG_BG2CNT に足す値
//--------------------------------------------------------------------------
void BattleFx_BeginTiledCanvas(s32 bg_control)
{
	s32 row;
	s32 col;
	s32 ofs;

	BattleFx_BeginCanvasLayer(bg_control);
	REG_BG2CNT = bg_control | 0x6784;
	ofs = 0;
	for (row = 0; row != 16; row++)
	{
		for (col = 0; col != 8; col++, ofs += 2)
		{
			s32 chr;
			s16 pair;

			chr = row * 16 + col * 2;
			pair = ((chr + 1) << 8) | chr;
			*(volatile s16 *)((u8 *)BG_CANVAS_MAP + ofs) = pair;
		}
		for (col = 0; col != 8; col++, ofs += 2)
		{
			*(volatile u16 *)((u8 *)BG_CANVAS_MAP + ofs) = 0;
		}
	}
}

//--------------------------------------------------------------------------
// キャンバス面を閉じ、背景パレットのレベルを 8 フレームで 21 から 0 へ戻す
//--------------------------------------------------------------------------
void BattleFx_EndCanvasLayer(void)
{
	struct BattleEffectWork *work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	struct BtlWork			*btl = *(gWorkSlot + HEAP_SLOT_BATTLE);
	s32						 i;

	AudioCommand_PlayFar(289);
	gBgScroll[1].x = work->saved_bg1_x;
	gBgScroll[1].y = work->saved_bg1_y;
	gProjection.center_x = 120;
	gProjection.center_y = 120;
	REG_BG2CNT = 0x787;
	Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
	Scheduler_RemoveCallback(Palette_StepFadeTransfer);
	gBgScroll[1].y = 32;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
	REG_BLDCNT = 0;
	WaitFrames(1);

	BattlePresentation_ConfigurePaletteFadeFar(2, btl->background, 7);
	WaitFrames(1);
	for (i = 0; i != 8; i++)
	{
		BattlePresentation_SetPaletteLevelFar(btl->background, 21 - i * 3);
		WaitFrames(1);
	}

	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7541, 0x20000);
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// BG1 とウインドウを消して、背景パレットのフェードを設定する
//--------------------------------------------------------------------------
void BattleFx_SetTransitionFlagAndDisplay(void)
{
	struct BtlWork *btl;
	s32			   *flag;

	flag = &((struct BattleBackgroundView *)Ram_WorkSlot[HEAP_SLOT_BATTLE_BACKGROUND])->second_mode;
	btl = Ram_WorkSlot[HEAP_SLOT_BATTLE];
	*flag = 1;
	QueueIoWriteDelay2((u32)&REG_DISPCNT, 0x1541);
	WaitFrames(1);
	BattlePresentation_ConfigurePaletteFadeFar(2, btl->background, 0);
	WaitFrames(1);
}

void BattleFx_SetupCanvasTileMap(void)
{
	u8						   **cells = (u8 **)&BtlWk;
	void						*canvas = cells[31];
	struct BtlWork				*screen = (struct BtlWork *)cells[0];
	struct BattleBackgroundView *layer = (struct BattleBackgroundView *)cells[35];
	s32							 row;
	s32							 col;
	s32							 off;

	BattleFx_ClearActorTimers();
	BattlePresentation_ConfigurePaletteFadeFar(2, screen->background, 0);
	gBgScroll[1].y = 32;
	layer->second_mode = 0;
	Iwram_ClearWords((void *)0x06003fc0, 64);
	Iwram_FillWords((void *)0x0600f900, 0x200, -1);
	off = 0;
	for (row = 0; row != 16; row++)
	{
		for (col = 0; col != 32; col++, off += 2)
		{
			if (col > 15)
				*(volatile u16 *)(0x0600fb00 + off) = 0xff;
			else
				*(volatile u16 *)(0x0600fb00 + off) = row * 16 + 0x100 + col;
		}
	}
	REG_DISPCNT = 0x7741;
	REG_BG1CNT = 0x1f81;
	REG_BLDCNT = 0x3f42;
	REG_WIN0H = 0xf0;
	REG_WIN0V = 0x1088;
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;
	REG_BLDALPHA = 0x100e;
	Iwram_ClearWords(canvas, 0x4000);
	Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// canvas_draw_clipped_line.c
// バトル演出：キャンバスへ線を引く（y をキャンバス内に収める）
//--------------------------------------------------------------------------

#define ABS(v) ((v) < 0 ? -(v) : (v))

//--------------------------------------------------------------------------
// 128x128 の 8bpp キャンバス（横 16 タイル）に線を引く
// 既にある色番号より大きいときだけ書き込む
// x0, y0 : 始点（ドット）
// x1, y1 : 終点（ドット）
// color  : 色番号
//--------------------------------------------------------------------------
void BattleFx_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color)
{
	s32 dx = x1 - x0;
	s32 dy = y1 - y0;
	s32 frac = 0x80; // 8bit 小数の 0.5
	u8 *canvas = ((u8 **)gBattleFxWork)[1];
	s32 step;
	s32 i;
	s32 x;
	s32 y;
	s32 tmp;
	u32 ofs;

	if (y0 < 0)
		y0 = 0;
	if (y0 > 127)
		y0 = 127;
	if (y1 < 0)
		y1 = 0;
	if (y1 > 127)
		y1 = 127;

	if (ABS(dx) < ABS(dy))
	{
		// 縦長：y を 1 ドットずつ進め、x は小数で追う
		if (dy < 0)
		{
			tmp = x0;
			x0 = x1;
			x1 = tmp;
			tmp = y0;
			y0 = y1;
			y1 = tmp;
			dx = x1 - x0;
			dy = y1 - y0;
		}
		step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
		x = x0;
		for (i = y0; i != y1; i++)
		{
			ofs = (((((u32)i >> 3) << 4) + ((u32)x >> 3)) << 3) + (i & 7);
			ofs = (ofs << 3) + (x & 7);
			if (canvas[ofs] < color)
				canvas[ofs] = color;
			frac += step;
			if (frac & 0x100)
			{
				if (dx > 0)
					x++;
				else
					x--;
				frac &= ~0x100;
			}
		}
	}
	else
	{
		// 横長：x を 1 ドットずつ進め、y は小数で追う
		if (dx < 0)
		{
			tmp = x0;
			x0 = x1;
			x1 = tmp;
			tmp = y0;
			y0 = y1;
			y1 = tmp;
			dx = x1 - x0;
			dy = y1 - y0;
		}
		step = (ABS(y1 - y0) << 8) / ABS(x1 - x0);
		y = y0;
		for (i = x0; i != x1; i++)
		{
			ofs = (((((u32)y >> 3) << 4) + ((u32)i >> 3)) << 3) + (y & 7);
			ofs = (ofs << 3) + (i & 7);
			if (canvas[ofs] < color)
				canvas[ofs] = color;
			frac += step;
			if (frac & 0x100)
			{
				if (dy > 0)
					y++;
				else
					y--;
				frac &= ~0x100;
			}
		}
	}
}

//--------------------------------------------------------------------------
// 炎になって落ちる跳躍
//--------------------------------------------------------------------------
// 破片 1 つ。PARTICLES と同じ 28 バイトの置き場で、速さの位置が struct EffectStep と違う
struct Chip
{
	s32 x; // 画面座標 (16.16 固定小数)
	s32 y;
	s32 velocity_x;
	s32 unused_0c;
	s32 velocity_y;
	s32 unused_14;
	s32 life; // 残りフレーム
};

#define gChips ((struct Chip *)Ram_MapCellBuffer)

//--------------------------------------------------------------------------
// 使い手が光って跳び上がり、炎の玉になって最初の対象へ落ち、破片を散らす
// effect : side が落ちる向きを、actors[0] が落ちる先を決める
//--------------------------------------------------------------------------
void BattleFx_RunLeapingStrike(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 pos;
	BlitterPair				 draw;
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	u8						*sprites;
	struct MotionObject		*target;
	s32						 dx;
	struct MotionObject		*actor;
	u8						*palette;
	s32						 frame;
	s32						 i;

	heap = gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	sprites = heap[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_EFFECT];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), sprites);
	palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
	Iwram_CopyWords((void *)BG_PLTT, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
	draw[1] = heap[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (i = 0; i != 1024; i++)
		gChips[i].life = 0;
	actor = GetBattleObjectSlotFar(work->effect->actor)->object;
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	if (actor->x > 0)
		dx = -(15 << 16);
	else
		dx = 15 << 16;

	for (frame = 0; frame != 88; frame++)
	{
		struct BattleCamera *camera = gCameraWork;

		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame > 17 || frame == 0)
		{
			EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &pos);
			pos.x /= 2;
		}

		// 光 : 2〜3 は使い手の上に、4〜15 は 16 方向へ広がる
		if (frame >= 2 && frame <= 3)
			draw[0](canvas, work, pos.x - 16, pos.y - 64, 32, 64);
		if (frame >= 4 && frame <= 15)
		{
			for (i = 0; i != 16; i++)
			{
				s32 x = pos.x + (frame * Trig_Sin(i << 12) >> 16);
				s32 y = pos.y + (frame * Trig_Cos(i << 12) >> 16) - frame;

				draw[0](canvas, work->sheet + ((frame - 4) / 2 << 11), x - 16, y - 64, 32, 64);
			}
		}
		// 跳び上がる
		if (frame == 4)
		{
			actor->velocity_y = 20 << 16;
			actor->acceleration = 1 << 16;
			actor->speed_limit = 3 << 16;
			actor->vertical_motion_strength = 0xab85;
			actor->auto_face_motion = 0;
			actor->snap_to_target = 0;
			Object_SetMoveTargetFar(actor, actor->x * 3, 0, actor->z);
			ObjectDispatch_ApplyArgumentToChildrenFar(actor, 2);
			work->shake_frames = frame;
			AudioCommand_PlayFar(136);
		}
		// 炎の玉の絵に切りかえ、対象と同じ奥行きにそろえる
		if (frame == 16)
		{
			palette = Resource_GetTableEntry((s32)&ResourceId_FlameballSheet);
			Iwram_CopyWords((void *)BG_PLTT, palette, 128);
			palette += 128;
			Resource_DecodeType01(palette, work);
			actor->vertical_motion_strength = 0;
			actor->velocity_x = 0;
			actor->velocity_y = 0;
			actor->z = target->z;
			ObjectDispatch_InitFromTable6Far(actor);
		}
		// 1 フレームに横へ 15、下へ 8 ドット落ち、地面に着いたら破片を 256 個散らす
		if (frame > 17)
		{
			if (actor->y > 0)
			{
				actor->x += dx;
				actor->y -= 8 << 16;
				if (work->effect->side == 0)
				{
					draw[0](canvas, work, pos.x - 20, pos.y - 52, 40, 64);
					pos.x -= 8;
				}
				else
				{
					draw[1](canvas, work, pos.x - 26, pos.y - 52, 40, 64);
					pos.y += 8;
				}
			}
			if (actor->y < 0)
			{
				actor->y = 0;
				for (i = 0; i != 256; i++)
				{
					struct Chip *chip = &gChips[i];
					s32			 speed;
					s32			 angle;

					speed = 0x3ff;
					speed &= Random16();
					angle = Random16() & 0xffff;
					chip->x = pos.x << 16;
					chip->y = (pos.y - 24) << 16;
					chip->velocity_x = Trig_Sin(angle) * (speed + 32) >> 6;
					chip->velocity_y = -(Trig_Cos(angle) * (speed + 32) << 1) >> 6;
					chip->life = (Random16() & 7) + 32;
				}
				work->shake_frames = 8;
				BattleEventRuntime_BeginPhaseFar(145);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
				ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
			}
		}

		// 破片 : 1 フレームに 56/64 へ減速して落ち、y = 112 で跳ね返る
		for (i = 0; i != 256; i++)
		{
			struct Chip *chip = &gChips[i];
			s32			 y;

			if (chip->life > 0)
			{
				chip->x += chip->velocity_x;
				y = chip->y + chip->velocity_y;
				chip->life--;
				chip->y = y;
				chip->velocity_x = chip->velocity_x * 56 / 64;
				chip->velocity_y = chip->velocity_y * 56 / 64 + 0x2000;
				if (chip->y > 112 << 16)
				{
					chip->velocity_y = -chip->velocity_y / 2;
				}
				else if ((chip->x >= 0 && chip->x < 127 << 16) && chip->y >= 0)
				{
					s32 size = chip->life / 8 + 1;

					draw[i & 1](canvas, sprites + ParticleStreams_CellOffsets[size - 1],
						(chip->x >> 16) - size / 2, (chip->y >> 16) - size, size, size * 2);
				}
			}
		}
		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 集まるどくろ
//--------------------------------------------------------------------------
// PARTICLES : 対象ひとりに 64 個
//--------------------------------------------------------------------------
// 対象ごとに 32 フレームずつずらして、まわり 64 ドットに散ったどくろを
// 4 フレームおきに 8 つ出し、回りながら対象へ寄せる
//--------------------------------------------------------------------------
void BattleFx_RunClosingSkulls(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 pos;
	s32						 point[3];
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	BlitterPair				 draw;
	struct BattleCamera		*camera;
	u8						*palette;
	s32						 i;
	s32						 k;

	heap = gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	palette = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
	Iwram_CopyWords((void *)BG_PLTT, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
	Iwram_CopyWords((void *)BG_PLTT, palette, 128);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
	draw[1] = heap[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
	work->transfer_mode = 3;
	work->transfer_value = 0x04040404;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (i = 0; i != 512; i++)
	{
		PARTICLES[i].x = ((Random16() & 255) - 127) << 15;
		PARTICLES[i].y = ((Random16() & 255) - 127) << 15;
		PARTICLES[i].z = ((Random16() & 255) - 127) << 15;
	}
	AudioCommand_PlayFar(142);

	for (frame = 0; frame != work->effect->count * 32 + 96; frame++)
	{
		s32 *row;

		camera = gCameraWork;
		if (frame == 96)
			BattleEventRuntime_BeginPhaseFar(0);

		// 背景を走査線ごとに横へ波打たせる
		row = work->bg2_x;
		if (work->effect->side == 0)
		{
			for (i = 0; i != 160; i++)
				*row++ = (0x60000 - Trig_Sin((frame + i) << 11) * 6) >> 10;
		}
		else
		{
			for (i = 0; i != 160; i++)
				*row++ = Trig_Sin((frame + i) << 11) * 6 >> 10;
		}

		for (k = 0; k != work->effect->count; k++)
		{
			struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[k])->object;

			// 対象の 20 ドット上を中心に、毎フレーム回す
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			point[0] = target->x;
			point[1] = 20 << 16;
			point[2] = target->z;
			SceneTransform_ApplyPosition(point);
			if (frame > k * 32)
			{
				SceneTransform_ApplyPitch(frame << 9);
				if (frame == k * 32 + 32)
					ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 32);

				// どくろは中心までの距離の 1/距離ずつ寄る
				for (i = 0; i != 8; i++)
				{
					struct EffectStep *skull = &PARTICLES[k * 64 + i];

					if (frame > (k * 8 + i) * 4)
					{
						s32 x = (skull->x >> 8) * (skull->x >> 8);
						s32 y = (skull->y >> 8) * (skull->y >> 8);
						s32 z = (skull->z >> 8) * (skull->z >> 8);
						s32 dist = Iwram_Sqrt(x + y + z) >> 8;

						if (dist != 0)
						{
							EffectPosition_ApplyBaseAndYOffset((s32 *)skull, &pos);
							pos.x >>= 1;
							draw[0](
								canvas, work->sheet + i % 3 * 576, pos.x - 12, pos.y - 12, 24, 24);
							skull->x -= skull->x / dist;
							skull->y -= skull->y / dist;
							skull->z -= skull->z / dist;
							skull->variant++;
						}
					}
				}
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 回る渦 : 対象ごとに 16 フレームずつずらして、40 ドット上で
// 半径 16 ドットの渦を 4 つ回す (60 フレーム)
//--------------------------------------------------------------------------
void BattleFx_RunMemberOrbit(void *effect)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	void					*palette;
	BlitterPair				 draw;
	s32						 point[3];
	struct EffectPosition	 screen;
	s32						 member;
	s32						 ofs_x;
	s32						 frame;

	heap = gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100;
	palette = Resource_GetTableEntry((s32)&ResourceId_SpiralSheet);
	Iwram_CopyWords((void *)BG_PLTT, palette, 128);
	Resource_DecodeType01((u8 *)palette + 128, work);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 15, 2);
	draw[1] = heap[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	if (work->effect->side == 1)
	{
		REG_BG2X = -0x6800;
		ofs_x = -112;
	}
	else
	{
		ofs_x = 0;
	}
	for (frame = 0; frame != (work->effect->count * 16) + 48; frame++)
	{
		struct BattleCamera *camera;
		s32					*row;
		s32					 i;

		// 背景を走査線ごとに横へ波打たせる
		camera = gCameraWork;
		row = work->bg2_x;
		if (work->effect->side == 0)
		{
			s32 angle;
			s32 base;

			for (i = 0, base = 0x80000, angle = frame << 10; i != 160; i++)
			{
				*row++ = (base - (Trig_Sin(angle) << 3)) >> 10;
				angle += 1024;
			}
		}
		else
		{
			s32 angle;

			for (i = 0, angle = frame << 10; i != 160; i++)
			{
				*row++ = ((Trig_Sin(angle) << 3) >> 10) - 0x7000;
				angle += 1024;
			}
		}
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		member = 0;
		if (work->effect->count != 0)
		{
			while (member != work->effect->count)
			{
				struct MotionObject *obj;

				obj = GetBattleObjectSlotFar(work->effect->actors[member])->object;
				if (frame > member * 16 && frame < (member * 16) + 60)
				{
					s32 spin;

					if (frame == (member * 16) + 32)
					{
						ObjectGroup_UpdateMembers(work->effect->actors[member], 0, 5, -1, 0);
					}
					point[0] = obj->x;
					point[1] = 40 << 16;
					point[2] = obj->z;
					EffectPosition_ApplyBaseAndYOffset(point, &screen);

					// 渦 (32×32) : 4 コマ、16 フレームごとに描画関数を入れかえる
					for (i = 0; i != 4; i++)
					{
						s32 x;
						s32 y;
						s32 half;

						spin = (frame << 9) + (i << 14);
						x = (screen.x + ((Trig_Sin(spin) << 4) >> 16)) + ofs_x;
						y = screen.y + ((Trig_Cos(spin) << 4) >> 16);
						half = frame / 16;
						draw[half & 1](canvas, work->sheet + (((frame / 4) - (half * 4)) << 10),
							x - 16, y - 16, 32, 32);
					}
				}
				member++;
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 散って集まる光 (mode が色を決める)
//--------------------------------------------------------------------------
void BattleFx_RunMemberBurstMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunMemberBurst(effect, 0);
}

void BattleFx_RunMemberBurstMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunMemberBurst(effect, 1);
}

void BattleFx_RunMemberBurstMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RunMemberBurst(effect, 2);
}

//--------------------------------------------------------------------------
// 対象ごとに光の粒を使い手から散らし、30 フレームのちから対象の体の中ほどへ
// 引き寄せ、32 ドット以内に入ったら消す
// mode : 0 は PinkBurstSheet、1 は MarsDjinnSheet、2 は MercuryDjinnSheet の色
//--------------------------------------------------------------------------
void BattleFx_RunMemberBurst(struct BattleEffectArgument *effect, s32 mode)
{
	struct EffectPosition	 pos;
	BlitterPair				 draw;
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 k;
	struct BattleCamera		*camera;
	s32						 pal;
	s32						 i;

	heap = gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	camera = heap[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_MemberBurstImage), work);
	if (mode == 0)
		pal = (s32)&ResourceId_PinkBurstSheet;
	else if (mode == 1)
		pal = (s32)&ResourceId_MarsDjinnSheet;
	else
		pal = (s32)&ResourceId_MercuryDjinnSheet;
	Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(pal), 128);
	BattleFx_FetchRectangleBlitters(work->effect->side, draw);

	// 粒 : 使い手の体の高さから、どの向きにも 2 ドットまでの速さで散る
	for (i = 0; i != 1024; i++)
		PARTICLES[i].variant = -1;
	for (k = 0; k != work->effect->count; k++)
	{
		struct MotionObject *src = GetBattleObjectSlotFar(work->effect->actor)->object;
		s32					 height = Battle_GetObjectTableValueFar(work->effect->actor);

		for (i = 0; i != 128; i++)
		{
			struct EffectStep *mote = &PARTICLES[k * 128 + i];

			mote->x = src->x;
			mote->y = height;
			mote->z = src->z;
			mote->velocity_x = ((Random16() & 255) - 128) << 10;
			mote->velocity_y = ((Random16() & 255) - 128) << 10;
			mote->velocity_z = ((Random16() & 255) - 128) << 10;
			mote->variant = 0;
		}
	}

	Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(146);

	for (frame = 0; frame != MemberBurst_Counts[mode * 2 + 1] + work->effect->count * 20; frame++)
	{
		s32 *row;

		if (frame == 80)
		{
			if (mode == 0)
				BattleEventRuntime_BeginPhaseFar(134);
			else
				BattleEventRuntime_BeginPhaseFar(133);
		}
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

		// 背景を走査線ごとに横へ波打たせる
		row = work->bg2_x;
		for (i = 0; i != 160; i++)
			*row++ = (0x100000 - Trig_Sin((frame + i) << 10) * 16) >> 10;

		for (k = 0; k != work->effect->count; k++)
		{
			struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[k])->object;
			s32					 half = Battle_GetObjectTableValueFar(work->effect->actors[k]) / 2;

			if (frame == k * 20 + 71)
			{
				if (mode == 0)
					AudioCommand_PlayFar(134);
				else
					AudioCommand_PlayFar(133);
			}
			if (frame == k * 20 + 70)
				ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 26);
			if (frame > k * 20)
			{
				for (i = 0; i != MemberBurst_Counts[mode * 2]; i++)
				{
					struct EffectStep *mote = &PARTICLES[k * 128 + i];

					if (frame > (k * 10 + i) * 2 && mote->variant >= 0)
					{
						EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &pos);
						pos.x >>= 1;
						draw[0](canvas, work->sheet + i % 3 * 640, pos.x - 10, pos.y - 16, 20, 32);
						EffectStep_AdvanceWithGravity3D(mote, 62, 0);
						if (frame > k * 20 + i + 30)
						{
							s32 dx = (target->x - mote->x) >> 9;
							s32 dy = (target->y + half - mote->y) >> 9;
							s32 dz = (target->z - mote->z) >> 9;

							mote->velocity_x += dx;
							mote->velocity_y += dy;
							mote->velocity_z += dz;
							if ((dx >= -0xfff && dx <= 0xfff) && (dz >= -0xfff && dz <= 0xfff))
								mote->variant = -1;
						}
					}
				}
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// earthwal.c
// バトル演出：キャンバス描画ルーチンの準備と、48 フレームの壁の演出
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 128×128 のキャンバスへ描く 2 つのルーチンを組み立てる
// flip : 0 以外なら左右反転して描く
// out  : [0] 比較合成 (大きい色番号を残す)、[1] 加算合成 (63 で飽和)
//--------------------------------------------------------------------------
void BattleFx_FetchRectangleBlitters(s32 flip, BlitterPair out)
{
	if (flip == 0)
	{
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
		out[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
		out[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	}
	else
	{
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
		out[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
			GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_KASAN);
		out[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	}
}

//--------------------------------------------------------------------------
// 3 つの入口：mode を決めて本体を呼ぶ
//--------------------------------------------------------------------------
void BattleFx_RunFortyEightFrameMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunFortyEightFrameEffect(effect, 1);
}

void BattleFx_RunFortyEightFrameMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunFortyEightFrameEffect(effect, 0);
}

void BattleFx_RunFortyEightFrameMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RunFortyEightFrameEffect(effect, 2);
}

//--------------------------------------------------------------------------
// 最初の対象の足元に、左右対称の壁 (EarthWall の絵) を立てる
// mode : 0 等倍、32 フレーム小さく揺らす
//        1 横 1.25 倍、8 フレーム大きく揺らす
//        2 横 1.25 倍、揺らさず Fireball のパレットで描く
//--------------------------------------------------------------------------
void BattleFx_RunFortyEightFrameEffect(struct BattleEffectArgument *effect, s32 mode)
{
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	u8						*palette;
	s32						 x;
	struct EffectPosition	 pos;
	BlitterPair				 draw;
	void				   **slots;
	s32						 frame;
	s32						 cell;

	cursor = gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);

	// 絵は EarthWall、パレットは MarsDjinn (mode 2 は Fireball) で上書きする
	palette = Resource_GetTableEntry((s32)&ResourceId_EarthWallSheet);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	if (mode == 2)
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_FireballSheet);
		Iwram_CopyWords((void *)0x05000000, palette, 128);
	}

	// キャンバスの x = 64 が対象の位置にくるよう BG2 をずらす
	EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &pos);
	if (mode == 0)
	{
		REG_BG2PA = 0x100; // 等倍
		x = 64 - pos.x;
		REG_BG2X = x << 8;
	}
	else
	{
		REG_BG2PA = 0xcc; // 0.8 (横 1.25 倍に表示)
		x = (-pos.x * 4) / 5 + 64;
		REG_BG2X = x << 8;
	}

	// 左半分はそのまま、右半分は左右反転して比較合成で描く
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	slots = gWorkSlot;
	draw[0] = slots[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
	draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE];
	work->transfer_mode = 2;
	work->transfer_value = 50; // 転送のたびに半分へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	if (mode == 2)
	{
		work->shake_frames = 0;
		AudioCommand_PlayFar(212);
	}
	else if (mode == 1)
	{
		work->shake_frames = 8;
		AudioCommand_PlayFar(212);
	}
	else
	{
		work->shake_frames = 32;
	}

	for (frame = 0; frame != 48; frame++)
	{
		if (frame == 0)
		{
			if (mode == 2)
				ObjectGroup_UpdateMembers(work->effect->actors[0], 7, -1, 0, 32);
			else
				ObjectGroup_UpdateMembers(work->effect->actors[0], 10, -1, 0, 32);
		}
		if (frame == 24)
			BattleEventRuntime_BeginPhaseFar(0);
		if (frame == 8 && mode == 0)
			AudioCommand_PlayFar(126);

		// 4 フレームごとに絵 0, 1, 2 と進め、その後は 2 と 1 を交互に (27 フレームまで)
		if (frame <= 31)
		{
			cell = frame / 4;
			if (cell > 2)
				cell = (cell & 1) + 1;
			if (frame <= 27)
			{
				draw[0](canvas, work->sheet + EarthWall_CellSourceOffsets[cell],
					64 - EarthWall_CellWidths[cell], (pos.y - EarthWall_CellHeights[cell]) + 8,
					EarthWall_CellWidths[cell], EarthWall_CellHeights[cell]);
				draw[1](canvas, work->sheet + EarthWall_CellSourceOffsets[cell], 64,
					(pos.y - EarthWall_CellHeights[cell]) + 8, EarthWall_CellWidths[cell],
					EarthWall_CellHeights[cell]);
			}
		}
		if (mode == 0)
			Camera_ApplyShake(2, 2);
		else
			Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// member_beam.c
// 戦闘エフェクト：ジンが現れ、光の粒が降り、空から光の柱が対象に落ちる
//--------------------------------------------------------------------------
// work->sheet の中の絵
#define PIC_MOTE 0x2710	 // 粒 (CrescentMoonSheet、24×24)
#define PIC_DJINN 0x65c0 // ジン (40×40)

//--------------------------------------------------------------------------
// mode 0 : ヴィーナスのジン
//--------------------------------------------------------------------------
void BattleFx_RunTwoModeAMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunMemberBeam(effect, 0);
}

//--------------------------------------------------------------------------
// mode 1 : マーキュリーのジン
//--------------------------------------------------------------------------
void BattleFx_RunTwoModeAMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunMemberBeam(effect, 1);
}

//--------------------------------------------------------------------------
// prelude が 1 ならジンを出して揺らしながら上へ動かし、対象ごとに
// 16 フレームずつずらして粒を 8 つ降らせ、72 フレーム目から光の柱を落とす
// mode : 0 はヴィーナス、1 はマーキュリーのジンの絵
//--------------------------------------------------------------------------
void BattleFx_RunMemberBeam(struct BattleEffectArgument *effect, s32 mode)
{
	struct EffectPosition	 one;
	struct EffectPosition	 pos;
	s32						 ground[3];
	s32						 point[3];
	s32						 djinn_x;
	s32						 djinn_y;
	BlitterPair				 draw;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 ofs_x;
	u8						*palette;
	s32						 i;
	s32						 k;

	cursor = gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	if (work->effect->prelude == 1)
		BattleFx_PrepareCanvasEffect(
			effect, mode, work->effect->side ^ work->effect->prelude, 0, &djinn_x, &djinn_y);
	palette = Resource_GetTableEntry((s32)&ResourceId_EarthWallSheet);
	Iwram_CopyWords((void *)BG_PLTT, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	palette = Resource_GetTableEntry((s32)&ResourceId_CrescentMoonSheet);
	Iwram_CopyWords((void *)BG_PLTT, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work->sheet + PIC_MOTE);
	if (mode == 0)
		palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet);
	else
		palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
	Iwram_CopyWords((void *)BG_PLTT, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work->sheet + PIC_DJINN);

	// 粒 : 80 ドットの高さから、揺れの位相はでたらめ
	for (i = 0; i != 512; i++)
	{
		PARTICLES[i].x = 0;
		PARTICLES[i].y = 80 << 16;
		PARTICLES[i].z = (Random16() | -32) << 14;
		PARTICLES[i].variant = Random16() & 255;
	}

	// 対象がひとりなら、その画面位置が x = 64 に来るよう BG2 をずらす
	REG_BG2PA = 0x100;
	if (work->effect->count == 1)
	{
		EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &one);
		ofs_x = 64 - one.x;
	}
	else
	{
		if (work->effect->side == 1)
			ofs_x = -112;
		else
			ofs_x = 0;
	}
	REG_BG2X = ofs_x << 8;
	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != work->effect->count * 16 + 116; frame++)
	{
		struct BattleCamera *camera = gCameraWork;

		if (frame == 64)
			AudioCommand_PlayFar(212);
		if (frame == 80)
			BattleEventRuntime_BeginPhaseFar(0);

		// ジン : 左右に 20 ドット揺れ、32 フレームのちから 2 ドットずつ上がる
		k = work->effect->prelude;
		if (k == 1)
		{
			s32 angle = frame << 11;
			s32 x = (-Trig_Sin(angle) * 20 >> 16) + djinn_x + ofs_x - 20;
			s32 y = (Trig_Cos(angle) * 4 >> 16) + djinn_y - 24;

			BattleFx_FetchRectangleBlitters(work->effect->side ^ k, draw);
			if (frame > 32)
				y = y - frame * 2 + 64;
			draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
			if (frame <= 3)
				draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		}
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
		draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
		draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];

		// 粒 : 8 フレームおきに出て、左右に 16 ドット揺れながら
		// 1 ドットずつ降り、高さ 8 ドットで消える
		for (k = 0; k != work->effect->count; k++)
		{
			struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[k])->object;
			s32					 start = k * 16;

			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			point[0] = target->x;
			point[1] = 0;
			point[2] = target->z;
			SceneTransform_ApplyPosition(point);
			if (frame > start)
			{
				for (i = 0; i != 8; i++)
				{
					struct EffectStep *mote = &PARTICLES[k * 64 + i];

					if (frame > i * 8 + start && mote->y > 8 << 16)
					{
						s32 sway;

						EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &pos);
						pos.x += ofs_x;
						sway = Trig_Sin(mote->variant << 10) * 16 >> 16;
						if (i & 1)
							pos.x -= sway;
						else
							pos.x += sway;
						draw[i & 1](canvas,
							work->sheet + MemberBeam_MotePictures[mote->variant / 8 & 7] * 576
								+ PIC_MOTE,
							pos.x - 12, pos.y - 12, 24, 24);
						mote->y -= 1 << 16;
						mote->variant++;
					}
				}
			}
		}

		// 光の柱 : 画面の上から対象の足もとまで、左右対称に描く
		// 幅は 6 ドット、88 フレーム目から 3 フレームに 1 ドットずつ細る
		for (i = 0; i != work->effect->count; i++)
		{
			s32 start = i * 16;

			if (frame >= start + 72)
			{
				struct MotionObject *target =
					GetBattleObjectSlotFar(work->effect->actors[i])->object;

				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				if (frame == start + 72)
					ObjectGroup_UpdateMembers(work->effect->actors[i], 1, -1, -1, 0);
				if (frame == start + 88)
					ObjectGroup_UpdateMembers(work->effect->actors[i], 0, -1, -1, 0);
				ground[0] = target->x;
				ground[1] = 0;
				ground[2] = target->z;
				EffectPosition_ApplyBaseAndYOffset(ground, &pos);
				pos.x += ofs_x;
				if (frame < start + 104)
				{
					s32 pic = frame / 4;
					s32 width = 6;

					if (frame > start + 88)
						width = 6 - (frame - start - 88) / 3;
					if (pic > 2)
						pic = (pic & 1) + 1;
					// 足もとの光
					if (frame < start + 100)
					{
						draw[0](canvas, work->sheet + MemberBeam_HeadOffsets[pic],
							pos.x - MemberBeam_HeadWidths[pic],
							pos.y - MemberBeam_HeadHeights[pic] + 8, MemberBeam_HeadWidths[pic],
							MemberBeam_HeadHeights[pic]);
						draw[1](canvas, work->sheet + MemberBeam_HeadOffsets[pic], pos.x,
							pos.y - MemberBeam_HeadHeights[pic] + 8, MemberBeam_HeadWidths[pic],
							MemberBeam_HeadHeights[pic]);
					}
					// 柱 (1 行ずつ)
					for (k = 0; k != pos.y; k++)
					{
						draw[0](canvas, work->sheet + 5, pos.x - width, k, width, 1);
						draw[1](canvas, work->sheet + 5, pos.x, k, width, 1);
					}
				}
			}
		}
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunSevenModeMode4(struct BattleEffectArgument *effect)
{
	BattleFx_RunSevenMode(effect, 4);
}

void BattleFx_RunSevenModeMode5(struct BattleEffectArgument *effect)
{
	BattleFx_RunSevenMode(effect, 5);
}

void BattleFx_RunSevenModeMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunSevenMode(effect, 0);
}

void BattleFx_RunSevenModeMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunSevenMode(effect, 1);
}

void BattleFx_RunSevenModeMode6(struct BattleEffectArgument *effect)
{
	BattleFx_RunSevenMode(effect, 6);
}

void BattleFx_RunSevenModeMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RunSevenMode(effect, 2);
}

void BattleFx_RunSevenModeMode3(struct BattleEffectArgument *effect)
{
	BattleFx_RunSevenMode(effect, 3);
}

extern s8  SevenMode_Records[];
extern u8  SevenMode_ShardWidths[];
extern u8  SevenMode_ShardHeights[];
extern u16 SevenMode_ShardOffsets[];

#define gBursts ((struct EffectStep *)(Ram_MapCellBuffer + 0x1c00))

void BattleFx_RunSevenMode(struct BattleEffectArgument *effect, s32 mode)
{
	struct EffectPosition	 position;
	BlitterPair				 draw;
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 k;
	s32						 total;
	struct MotionObject		*source;
	struct BattleCamera		*camera;
	u8						*palette;
	s32						 i;
	s32						 height;

	heap_cache = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	camera = heap_cache[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BLDALPHA = 0x1010;
	palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
	Iwram_CopyWords((void *)BG_PLTT, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	palette = Resource_GetTableEntry((s32)&ResourceId_SmokeSheet);
	Iwram_CopyWords((void *)BG_PLTT, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work->sheet + 0x3e80);
	if (mode == 0)
		palette = Resource_GetTableEntry((s32)&ResourceId_BluePalette);
	else if (mode == 1)
		palette = Resource_GetTableEntry((s32)&ResourceId_SpiderWebSheet);
	else if (mode == 2)
		palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
	else if (mode == 3)
		palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
	else if (mode == 4)
		palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
	else if (mode == 6)
		palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
	else
		palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
	Iwram_CopyWords((void *)BG_PLTT, palette, 128);

	for (i = 0; i != 1024; i++)
		PARTICLES[i].variant = -1;
	source = GetBattleObjectSlotFar(work->effect->actor)->object;
	height = 0x40000;
	for (k = 0; k != work->effect->count; k++)
	{
		struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[k])->object;

		for (i = 0; i != 16; i++)
		{
			struct EffectStep *shot = &PARTICLES[k * 16 + i];

			shot->x = source->x;
			shot->y = height;
			shot->z = source->z;
			shot->velocity_x = (target->x - source->x) >> 4;
			shot->velocity_y = 0x40000;
			shot->velocity_z = (target->z - source->z) >> 4;
			shot->variant = 0;
		}
	}
	for (i = 0; i != 256; i++)
		gBursts[i].variant = -1;

	if (effect->side == 0)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
		draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		if (SevenMode_Records[mode * 2] == 0)
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
			draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
		}
		else
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
			draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
		}
	}
	else
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 2);
		draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		if (SevenMode_Records[mode * 2] == 0)
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
			draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
		}
		else
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
			draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
		}
	}
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	if (SevenMode_Records[mode * 2] == 0)
		total = work->effect->count * 8 + 72;
	else
		total = work->effect->count * 8 + 56;
	AudioCommand_PlayFar(103);

	for (frame = 0; frame != total; frame++)
	{
		struct EffectStep *burst;

		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		k = 0;
		if (work->effect->count != 0)
		{
			do
			{
				if (frame >= k * 8)
				{
					struct EffectStep *shot = &PARTICLES[k * 16];

					if (frame == k * 8 + 17)
					{
						ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 16);
						BattleEventRuntime_BeginPhaseFar(133);
					}
					/* FAKEMATCH: needed to match: restores the CSE block boundary after the hit block (k << 3 is recomputed). Without the block: differ 449; without both of this function's empty blocks: differ 337. */
					do
					{
					} while (0);
					if (shot->variant >= 0)
					{
						s32 step = (frame - k * 8) / 3;

						if (step > 9)
							step = 9;
						EffectPosition_ApplyBaseAndYOffset((s32 *)shot, &position);
						position.x >>= 1;
						if (step > 4)
							draw[0](canvas, work->sheet + step * 768, position.x - 16,
								position.y - 12, 32, 24);
						else
							draw[0](canvas, work->sheet + step * 768, position.x - 12,
								position.y - 16, 24, 32);
						if (shot->variant == 0)
							EffectStep_AdvanceWithGravity3D(shot, 63, -0x8000);
						if (shot->y < 0)
						{
							s32 count;
							s8 *records = SevenMode_Records;
							s32 row;

							shot->y = 0;
							shot->variant = 1;
							/* FAKEMATCH: needed to match: native loads the table address before the row (r0, r1) 2026-10-11: SevenMode_Records[mode * 2] in the test differs 135, as a conditional expression too. */
							row = mode * 2;
							if (records[row] == 0)
								count = 4;
							else
								count = 16;
							for (i = 0; i != count; i++)
							{
								struct EffectStep *burst;
								s32				   group = k * 4;

								/* FAKEMATCH: needed to match: restores the CSE block boundary between group's set and group * 8. Without the block: differ 171; as gBursts + group * 8 + i without it: differ 177. */
								do
								{
								} while (0);
								burst = &gBursts[group * 8 + i];

								burst->x = shot->x;
								burst->y = shot->y;
								burst->z = shot->z;
								if (SevenMode_Records[mode * 2] == 0)
								{
									burst->velocity_x = ((Random16() & 63) - 32) << 11;
									burst->velocity_y = 0;
									burst->velocity_z = ((Random16() & 63) - 32) << 11;
								}
								else
								{
									burst->velocity_x = ((Random16() & 63) - 32) << 13;
									burst->velocity_y = ((Random16() & 31) + 32) << 12;
									burst->velocity_z = ((Random16() & 63) - 32) << 13;
								}
								burst->variant = 0;
							}
						}
					}
				}
				k++;
			} while (k != work->effect->count);
		}
		for (i = 0, burst = gBursts; i != 256; burst++, i++)
		{

			if ((u32)burst->variant <= 44 && burst->y >= 0)
			{
				EffectPosition_ApplyBaseAndYOffset((s32 *)burst, &position);
				position.x >>= 1;
				if (SevenMode_Records[mode * 2] == 0)
				{
					draw[1](canvas, work->sheet + burst->variant / 8 * 1152 + 0x3e80,
						position.x - 12, position.y - 24, 24, 48);
				}
				else
				{
					s32 picture = burst->variant / 5;
					s32 flip;

					if (i & 1)
						picture += 9;
					flip = effect->side;
					draw[burst->velocity_x > 0 ? flip ^ 1 : flip](canvas,
						work->sheet + SevenMode_ShardOffsets[picture] + 0x1e00,
						position.x - SevenMode_ShardWidths[picture] / 2,
						position.y - SevenMode_ShardHeights[picture] / 2,
						SevenMode_ShardWidths[picture], SevenMode_ShardHeights[picture]);
				}
				if (SevenMode_Records[mode * 2] == 0)
					EffectStep_AdvanceWithGravity3D(burst, 62, 0x800);
				else
					EffectStep_AdvanceWithGravity3D(burst, 62, -0x8000);
				burst->variant++;
			}
		}
		Camera_ApplyShake(2, 2);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

#define PIXEL_BUFFER Ram_MapCellBuffer

void BattleFx_RunCounterReveal(struct BattleEffectArgument *object)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	u8						*sheet;
	void					*cells;
	BlitterPair				 rectangle;
	struct MotionObject		*member_object;
	/* FAKEMATCH: needed to match: the second position is never used: it gives the frame its native 76 bytes 2026-10-11: a single position shrinks only the frame (differ 4). */
	struct EffectPosition pos[2];
	s32					  curve[2];
	s32					  draw_enabled;
	s32					  idx_a;
	s32					  idx_b;
	s32					  zero_val;
	s32					  frame;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	/* FAKEMATCH: needed to match: the zero is held in sl through the setup calls. Storing the literal 0 without the local: differ 66. */
	zero_val = 0;
	work->effect = object;
	BattleFx_BeginTiledCanvas(0);
	REG_BG2PA = 0x100;
	REG_BLDALPHA = 0x1010;
	sheet = Resource_GetTableEntry((s32)&ResourceId_CounterRevealSheet);
	Iwram_CopyWords(BG_PLTT, sheet, 128);
	sheet = sheet + 128;
	Resource_DecodeType01(sheet, work);
	cells = Ram_MapCellBuffer;
	sheet = Resource_GetTableEntry((s32)&ResourceId_RedCrescentSheetA);
	sheet = sheet + 128;
	Resource_DecodeType01(sheet, cells);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 1);
	rectangle[0] = (DrawRectangle)heap_cache[7];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 1);
	rectangle[1] = (DrawRectangle)heap_cache[8];
	Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
	work->transfer_mode = 1;
	work->transfer_value = zero_val;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	draw_enabled = 1;
	if (work->effect->side == 1)
	{
		curve[0] = -0x500000;
	}
	else
	{
		curve[0] = 0x700000;
	}
	curve[1] = -0x200000;
	for (frame = 0; frame != 132; frame++)
	{
		s32	 spin;
		s32	 screen_x;
		s32	 screen_y;
		s32	 amp;
		s32	 row_base;
		s32	 angle;
		s32	 i;
		s32 *scanline;

		spin = frame << 9;
		screen_x = (curve[0] >> 16) + ((Trig_Sin(spin) << 4) >> 16) + 48;
		screen_y = (curve[1] >> 16) + ((Trig_Cos(spin) << 2) >> 16) + 16;
		if (frame == 88)
		{
			AudioCommand_PlayFar(134);
		}
		if (frame == 32)
		{
			if (work->effect->side == 1)
			{
				curve[0] = -0x200000;
			}
			else
			{
				curve[0] = 0x480000;
			}
			curve[1] = 0x180000;
			draw_enabled = 0;
		}
		if (frame == 33)
		{
			REG_BLDALPHA = 0x1010;
			draw_enabled = 1;
		}
		if (frame == 64)
		{
			EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], pos);
			if (work->effect->side == 1)
			{
				curve[0] = (pos[0].x - 128) << 16;
			}
			else
			{
				curve[0] = (pos[0].x - 64) << 16;
			}
			curve[1] = 0;
			draw_enabled = 0;
		}
		if (frame == 65)
		{
			REG_BLDALPHA = 0x1010;
			draw_enabled = 1;
		}
		scanline = work->bg2_x;
		amp = 0;
		if (frame <= 31)
		{
			if (frame > 15)
			{
				amp = (frame * 2) - 32;
				REG_BLDALPHA = (31 - frame) | 0x1000;
			}
		}
		else if (frame <= 63)
		{
			if (frame > 47)
			{
				amp = (frame * 2) - 96;
				REG_BLDALPHA = (63 - frame) | 0x1000;
			}
		}
		if (amp < 0)
		{
			amp = 0;
		}

		i = 0;
		row_base = (6 - screen_x) << 8;
		angle = frame << 11;
		for (; i != 160; i++)
		{
			*scanline++ = row_base - ((Trig_Sin(angle) * amp) >> 10);
			angle += 0x800;
		}
		if (draw_enabled != 0)
		{
			if (work->effect->side == 0)
			{
				idx_a = 0;
				idx_b = 0;
			}
			else
			{
				idx_a = 1;
				/* FAKEMATCH: needed to match: i is 160 here, so this is 0: a literal 0 drops a stack slot and shrinks the frame (differ 452). */
				idx_b = i >> 31;
			}
			if (frame <= 87)
			{
				rectangle[work->effect->side](canvas, work, CounterReveal_PanelX[idx_a * 7],
					CounterReveal_PanelY[idx_b * 7] + screen_y, 57, 98);
			}
			else
			{
				if (frame <= 91)
				{
					rectangle[work->effect->side](canvas, work, CounterReveal_PanelX[idx_a * 7],
						CounterReveal_PanelY[idx_b * 7] + screen_y, 57, 98);
				}
				rectangle[work->effect->side](canvas, work->sheet + 0x15d2,
					CounterReveal_PanelX[idx_a * 7 + 1],
					CounterReveal_PanelY[idx_b * 7 + 1] + screen_y, 99, 69);
				if ((frame - 88) <= 1U)
				{
					Iwram_FillWords(canvas, 0x4000, 0x3F3F3F3F);
				}
				if ((frame - 90) <= 1U)
				{
					rectangle[work->effect->side](canvas, work->sheet + 0x3081,
						CounterReveal_PanelX[idx_a * 7 + 2],
						CounterReveal_PanelY[idx_b * 7 + 2] + screen_y, 128, 91);
				}
				if ((frame - 92) <= 1U)
				{
					rectangle[work->effect->side](canvas, (void *)gMapCellBuffer,
						CounterReveal_PanelX[idx_a * 7 + 3],
						CounterReveal_PanelY[idx_b * 7 + 3] + screen_y, 128, 91);
				}
				if ((frame - 94) <= 1U)
				{
					rectangle[work->effect->side](canvas, (u8 *)gMapCellBuffer + 0x2d80,
						CounterReveal_PanelX[idx_a * 7 + 4],
						CounterReveal_PanelY[idx_b * 7 + 4] + screen_y, 128, 59);
				}
				if ((frame - 96) <= 1U)
				{
					rectangle[work->effect->side](canvas, (u8 *)gMapCellBuffer + 0x4b00,
						CounterReveal_PanelX[idx_a * 7 + 5],
						CounterReveal_PanelY[idx_b * 7 + 5] + screen_y, 122, 29);
				}
				if ((frame - 98) <= 1U)
				{
					rectangle[work->effect->side](canvas, (u8 *)gMapCellBuffer + 0x58d2,
						CounterReveal_PanelX[idx_a * 7 + 6],
						CounterReveal_PanelY[idx_b * 7 + 6] + screen_y, 76, 25);
				}
			}
		}
		if (frame == 88)
		{
			member_object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
			member_object->velocity_y = 0x10000;
			member_object->acceleration = 0x20000;
			member_object->speed_limit = 0x20000;
			member_object->vertical_motion_strength = 0;
			member_object->auto_face_motion = 0;
			member_object->snap_to_target = 0;
			Object_SetMoveTargetFar(member_object, member_object->x << 1, 0, member_object->z);
			ObjectGroup_UpdateMembers(work->effect->actors[0], -1, 5, -1, 0);
		}
		if (frame == 120)
		{
			GetBattleObjectSlotFar(work->effect->actors[0])->object->vertical_motion_strength =
				0xAB85;
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
	BattleBackground_LoadFar(1, BtlWk->background, 24);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void Graphics_ConvertBackgroundToBlueRamp(void)
{
	s32	 i, x, y, row, end, speed;
	s32	 red, green, blue;
	u16	 color;
	u8	*delays, *p, *pixel;
	u16 *palette;
	u32	 delay_mask;
	palette = (u16 *)0x05000040;
	i = 0;
	do
	{
		s32 half;
		half = i / 2;
		*palette++ = (i << 10) | (half << 5) | half;
		i++;
	} while (i != 32);
	delays = *(u8 **)&gWorkSlot[HEAP_SLOT_BATTLE_SHEET];
	end = 0;
	speed = 16;
	Iwram_CopyWords(PIXEL_BUFFER, (void *)0x06008000, 0x7800);
	delay_mask = 63;
	p = delays;
	do
	{
		*p++ = Random16() & delay_mask;
	} while (p != delays + 256);
	row = 0;
	i = 0;
	do
	{
		end += speed / 4;
		speed++;
		while (row != end)
		{
			x = 0;
			do
			{
				y = row - delays[x];
				if (y >= 0)
				{
					if (y <= 119)
					{
						pixel = PIXEL_BUFFER
								+ ((x & 7) + ((x / 8) << 6) + ((y & 7) << 3) + ((y / 8) << 11));
						color = ((s16 *)0x05000000)[*pixel];
						red = color & 31;
						green = (color >> 5) & 31;
						blue = (color >> 10) & 31;
						if (red < green)
							red = green;
						if (red < blue)
							red = blue;
						*pixel = 63 - red;
					}
				}
				x++;
			} while (x != 256);
			row++;
		}
		Iwram_CopyWords((void *)0x06008000, PIXEL_BUFFER, 0x7800);
		WaitFrames(1);
		if (end > 248)
			break;
		i++;
	} while (i != 27);
	palette = (u16 *)0x050000c0;
	i = 0;
	do
	{
		s32 half;
		half = i / 2;
		*palette++ = (i << 10) | (half << 5) | half;
		i++;
	} while (i != 32);
	i = 0;
	do
	{
		PIXEL_BUFFER[i] += 64;
		i++;
	} while (i != 0x7800);
	Iwram_CopyWords((void *)0x06008000, PIXEL_BUFFER, 0x7800);
	WaitFrames(1);
}

void BattleFx_RunSpinningTriangle(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 anchor;
	struct Vector3			 vector;
	struct EffectPosition	 screen;
	struct Vector3			 scale;
	s32						 origin_x;
	s32						 origin_y;
	BlitterPair				 draw;
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 member;
	struct BattleCamera		*camera;
	s32						 shift;
	u8						*graphics;
	u8						*palette;
	struct MotionObject		*object;
	s32						 tick;
	struct EffectStep		*point;
	s32						 size;
	s32						 j;
	s32						 k;

	heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	graphics = heap_cache[2];
	camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	if (work->effect->prelude == 1)
		BattleFx_PrepareCanvasEffect(effect, 3, work->effect->side, 0, &origin_x, &origin_y);
	REG_BG2PA = 0x100;
	palette = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
	Resource_DecodeType01(
		Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD), work->sheet + 0x1000);
	palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
	palette += 128;
	Resource_DecodeType01(palette, work->sheet + 0x2000);
	work->transfer_mode = 3;
	work->transfer_value = 0x04040404;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
	shift = 64 - anchor.x;
	REG_BG2X = shift << 8;
	AudioCommand_PlayFar(142);

	for (frame = 0; frame != work->effect->count * 20 + 72; frame++)
	{
		if (frame == 64)
			BattleEventRuntime_BeginPhaseFar(0);
		Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
		if (work->effect->prelude == 1)
		{
			s32 x;
			s32 y;

			x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin_x + shift - 20;
			y = ((Trig_Cos(frame << 11) << 2) >> 16) + origin_y - 24;
			BattleFx_FetchRectangleBlitters(work->effect->side, draw);
			if (frame > 32)
				y = y - frame * 2 + 64;
			draw[0](canvas, work->sheet + 0x2000, x, y, 40, 40);
			if (frame <= 3)
				draw[1](canvas, work->sheet + 0x2000, x, y, 40, 40);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		}
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
		draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
		draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
		if (frame > 16 && (frame & 15) == 0)
			work->transfer_value += 0x01010101;
		for (member = 0; member != 1; member++)
		{
			tick = frame - member * 8;
			object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
			if (tick >= 0 && tick < 96)
			{
				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				vector.x = object->x;
				vector.y = object->y;
				vector.z = object->z;
				EffectPosition_ApplyBaseAndYOffset((s32 *)&vector, &screen);
				screen.x = anchor.x + shift;
				screen.y -= 24;
				if (tick <= 67)
				{
					for (j = 0; j != 3; j++)
					{
						point = &work->particles[member * 32 + j];
						Render_ResetTransformState();
						if (tick <= 63)
						{
							scale.x = 0x2a000 - (tick * 3 << 9);
							scale.y = 0x2a000 - (tick * 3 << 9);
							scale.z = 0x2a000 - (tick * 3 << 9);
							SceneTransform_ApplyScale(&scale);
							SceneTransform_ApplyRoll((64 - tick) << 9);
							SceneTransform_ApplyYaw((64 - tick) << 9);
						}
						SceneTransform_ApplyRoll(j * 0x5555);
						EffectPosition_ApplyBaseAndYOffset(
							(s32 *)SpinningTriangle_Vertex, (struct EffectPosition *)&vector);
						point->velocity_x = vector.x + screen.x;
						point->velocity_y = vector.y + screen.y + 16;
					}
					for (j = 0; j != 3; j++)
					{
						struct EffectStep *from = &work->particles[j + member * 32];
						struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

						size = 5 - tick / 16;
						for (k = 0; k != 24; k++)
						{
							s32 x = from->velocity_x + k * (to->velocity_x - from->velocity_x) / 24;
							s32 y = from->velocity_y + k * (to->velocity_y - from->velocity_y) / 24;

							draw[0](canvas, work->sheet + BattleFx6_FlareCells[size - 1] + 0x1000,
								x - size, y - size, size * 2, size * 2);
						}
					}
				}
				if (tick > 63)
				{
					draw[0](canvas, work, screen.x - 24, screen.y - 24, 24, 48);
					draw[1](canvas, work, screen.x, screen.y - 24, 24, 48);
				}
			}
		}
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunTriangleStrike(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 anchor;
	struct Vector3			 vector;
	struct EffectPosition	 screen;
	struct Vector3			 scale;
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	BlitterPair				 draw;
	s32						 member;
	struct BattleCamera		*camera;
	s32						 shift;
	u8						*graphics;
	u8						*palette;
	struct MotionObject		*object;
	s32						 tick;
	struct EffectStep		*point;
	s32						 size;
	s32						 j;
	s32						 k;

	heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	graphics = heap_cache[2];
	camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100;
	palette = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
	palette = Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD);
	Resource_DecodeType01(palette, work->sheet + 0x1000);
	work->transfer_mode = 3;
	work->transfer_value = 0x04040404;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
	shift = 64 - anchor.x;
	REG_BG2X = shift << 8;
	AudioCommand_PlayFar(142);

	for (frame = 0; frame != work->effect->count * 20 + 72; frame++)
	{
		if (frame == 64)
			BattleEventRuntime_BeginPhaseFar(0);
		if (frame == 46)
			BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0], 16, 0);
		Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
		draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
		draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
		if (frame > 16 && (frame & 15) == 0)
			work->transfer_value += 0x01010101;
		for (member = 0; member != 1; member++)
		{
			tick = frame - member * 8;
			object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
			if (tick >= 0 && tick < 96)
			{
				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				vector.x = object->x;
				vector.y = object->y;
				vector.z = object->z;
				EffectPosition_ApplyBaseAndYOffset((s32 *)&vector, &screen);
				screen.x = anchor.x + shift;
				screen.y -= 24;
				if (tick <= 67)
				{
					for (j = 0; j != 3; j++)
					{
						point = &work->particles[member * 32 + j];
						Render_ResetTransformState();
						if (tick <= 63)
						{
							scale.x = 0x2a000 - (tick * 3 << 9);
							scale.y = 0x2a000 - (tick * 3 << 9);
							scale.z = 0x2a000 - (tick * 3 << 9);
							SceneTransform_ApplyScale(&scale);
							SceneTransform_ApplyRoll((64 - tick) << 9);
							SceneTransform_ApplyYaw((64 - tick) << 9);
						}
						SceneTransform_ApplyRoll(j * 0x5555);
						EffectPosition_ApplyBaseAndYOffset(
							(s32 *)TriangleStrike_Vertex, (struct EffectPosition *)&vector);
						point->velocity_x = vector.x + screen.x;
						point->velocity_y = vector.y + screen.y + 16;
					}
					for (j = 0; j != 3; j++)
					{
						struct EffectStep *from = &work->particles[j + member * 32];
						struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

						size = 5 - tick / 16;
						for (k = 0; k != 24; k++)
						{
							s32 x = from->velocity_x + k * (to->velocity_x - from->velocity_x) / 24;
							s32 y = from->velocity_y + k * (to->velocity_y - from->velocity_y) / 24;

							draw[0](canvas, work->sheet + BattleFx6_FlareCells[size - 1] + 0x1000,
								x - size, y - size, size * 2, size * 2);
						}
					}
				}
				if (tick > 63)
				{
					draw[0](canvas, work, screen.x - 24, screen.y - 24, 24, 48);
					draw[1](canvas, work, screen.x, screen.y - 24, 24, 48);
				}
			}
		}
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunRingBolts(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 pos;
	struct EffectPosition	 base;
	struct Vector3			 scale;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	/* FAKEMATCH: frame local: only draw[0] is used; the unused second blitter word fills
	   four bytes of the frame. Measured (2026-10-14): declared as one DrawRectangle, the
	   frame is 112 bytes for 116 (differ 48, frame offsets only). */
	BlitterPair			 draw;
	s32					 i;
	u8					*graphics;
	struct BattleCamera *camera;
	struct MotionObject *caster;
	struct MotionObject *source;
	struct MotionObject *target;
	struct EffectStep	*point;
	struct EffectStep	*step;
	s32					 start;
	s32					 peak;
	s32					 size;
	s32					 j;
	s32					 k;
	struct EffectStep	*trails;

	work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	graphics = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
	camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	caster = GetBattleObjectSlotFar(effect->actor)->object;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
	Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
	ObjectDispatch_ApplyArgumentToChildrenFar(caster, 2);
	ObjectDispatch_ApplyValueToChildrenFar(caster, 48);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	draw[0] = (DrawRectangle) * (gWorkSlot + HEAP_SLOT_BLITTER);
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	source = GetBattleObjectSlotFar(work->effect->actor)->object;
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (i = 0, point = work->particles; i != 3; i++)
	{
		point->x = source->x;
		point->y = source->y + 0x280000;
		point->z = source->z;
		if (i == 0)
			point->velocity_x = (target->x - point->x) / 12;
		else
			point->velocity_x = (target->x * 2 - point->x) / 12;
		point->velocity_y = (target->y - point->y + 0x280000) / 12;
		point->velocity_z = (target->z - point->z) / 12;
		point->variant = 0;
		point++;
	}

	trails = gMapCellBuffer;
	for (frame = 0; frame != 60; frame++)
	{
		if (frame <= 47)
		{
			struct BattleCamera *view = *(gWorkSlot + HEAP_SLOT_CAMERA);
			s32					 speed;

			if (frame <= 39)
				speed = 128;
			else
				speed = 0x300 - frame * 16;
			if (work->effect->side == 0)
				view->yaw -= speed;
			else
				view->yaw += speed;
		}
		for (i = 0; i != 3; i++)
		{
			start = i * 12;
			if (frame >= start)
			{
				step = &work->particles[i];
				size = (frame - start) / 4 + 2;
				if (size > 10)
					size = 10;
				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				SceneTransform_ApplyPosition(step);
				peak = 0;
				for (j = 0; j != 10; j++)
				{
					s32 depth;

					Graphics_SaveTransferWorkOnce();
					SceneTransform_ApplyRoll((frame - start) << 10);
					SceneTransform_ApplyYaw(0x4000);
					scale.x = ((frame - start) << 12) + 0x1000;
					if (scale.x > 0x10000)
						scale.x = 0x10000;
					scale.y = scale.x;
					scale.z = scale.x;
					SceneTransform_ApplyScale(&scale);
					SceneTransform_ApplyRoll(j * 0x199a);
					depth =
						EffectPosition_ApplyBaseAndYOffset((s32 *)&RingBolts_Points[j & 1], &pos);
					if (peak < depth)
						peak = depth;
					pos.x >>= 1;
					trails[i * 10 + j].velocity_x = pos.x + base.x;
					trails[i * 10 + j].velocity_y = pos.y + base.y;
					trails[i * 10 + j].velocity_x = pos.x;
					trails[i * 10 + j].velocity_y = pos.y;
					Graphics_RestoreTransferWork();
				}
				if (peak <= 399999)
				{
					for (j = 0; j != 10; j++)
					{
						struct EffectStep *from =
							&((struct EffectStep *)gMapCellBuffer)[j + i * 10];
						struct EffectStep *to =
							&((struct EffectStep *)gMapCellBuffer)[(j + 1) % 10 + i * 10];

						for (k = 0; k != 16; k++)
						{
							s32 x = from->velocity_x + (to->velocity_x - from->velocity_x) * k / 16;
							s32 y = from->velocity_y + (to->velocity_y - from->velocity_y) * k / 16;

							draw[0](canvas, graphics + ParticleStreams_CellOffsets[size - 1],
								x - size / 2, y - size, size, size * 2);
						}
					}
				}
				step->x += step->velocity_x;
				step->y += step->velocity_y;
				step->z += step->velocity_z;
				if (frame == start + i + 10)
				{
					target->acceleration = 0x20000;
					target->speed_limit = 0x80000;
					target->velocity_y = 0x50000;
					target->vertical_motion_strength = 0xab85;
					target->auto_face_motion = 0;
					ObjectDispatch_InitFromTable6Far(target);
					if (target->x < 0)
						Object_SetMoveTargetFar(target, target->x - 0x280000, 0, target->z);
					else
						Object_SetMoveTargetFar(target, target->x + 0x280000, 0, target->z);
					if (i == 2)
					{
						BattleEventRuntime_BeginPhaseFar(134);
					}
					else
					{
						AudioCommand_PlayFar(134);
						ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
					}
					work->shake_frames = 4;
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

void BattleFx_RunSpinningStars(struct BattleEffectArgument *effect)
{
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	/* FAKEMATCH: frame local: only draw[0] is used; the unused second blitter word fills
	   four bytes of the frame. Measured (2026-10-14): declared as one DrawRectangle, the
	   frame is 60 bytes for 64 (differ 40, frame offsets only). */
	BlitterPair			  draw;
	s32					  i;
	u8					 *graphics;
	struct BattleCamera	 *camera;
	struct MotionObject	 *source;
	struct MotionObject	 *target;
	struct EffectStep	 *point;
	struct EffectStep	 *step;
	struct EffectPosition pos;
	s32					  size;
	s32					  j;
	s32					  k;
	struct EffectStep	 *trails;

	work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	graphics = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
	camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
	Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	work->transfer_mode = 2;
	work->transfer_value = 50;
	draw[0] = (DrawRectangle) * (gWorkSlot + HEAP_SLOT_BLITTER);
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	source = GetBattleObjectSlotFar(work->effect->actor)->object;
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (i = 0, point = work->particles; i != 8; i++)
	{
		s32 delay = i * 8;

		point->x = source->x / 2;
		point->y = source->y + 0x780000;
		point->z = source->z;
		point->velocity_x = (target->x + ((s32)((Random16() & 0x7f) - 64) << 16) - point->x) / 12;
		point->velocity_y = (target->y - point->y + 0x140000) / 12;
		point->velocity_z = (target->z - point->z) / 12;
		point->variant = (Random16() & 15) + delay;
		point++;
	}

	size = 2;
	trails = gMapCellBuffer;
	for (frame = 0; frame != 128; frame++)
	{
		Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
		if (frame == 96)
			BattleEventRuntime_BeginPhaseFar(134);
		for (i = 0; i != 8; i++)
		{
			step = &work->particles[i];
			if (frame >= step->variant)
			{
				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				EffectPosition_ApplyBaseAndYOffset((s32 *)step, &pos);
				pos.x >>= 1;
				if (pos.x >= -8 && pos.x <= 127)
				{
					if (pos.y <= 127)
					{
						if (pos.y >= -8)
						{
							for (j = 0; j != 10; j++)
							{
								trails[i * 10 + j].velocity_x =
									pos.x
									+ ((Trig_Sin(j * 0x199a - ((frame - step->variant) << 11))
										   * SpinningStars_Radii[j & 1])
											/ 2
										>> 16);
								trails[i * 10 + j].velocity_y =
									pos.y
									- (Trig_Cos(j * 0x199a - ((frame - step->variant) << 11))
											* SpinningStars_Radii[j & 1]
										>> 16);
							}
							for (j = 0; j != 10; j++)
							{
								struct EffectStep *from = &trails[j + i * 10];
								struct EffectStep *to = &trails[i * 10 + (j + 1) % 10];

								for (k = 0; k != 12; k++)
								{
									s32 x = from->velocity_x
											+ (to->velocity_x - from->velocity_x) * k / 12;
									s32 y = from->velocity_y
											+ (to->velocity_y - from->velocity_y) * k / 12;

									draw[0](canvas,
										graphics + ParticleStreams_CellOffsets[size - 1],
										x - size / 2, y - size, size, size * 2);
								}
							}
						}
					}
				}
				if (step->y <= 0x1dffff)
				{
					step->velocity_y = -step->velocity_y;
					step->velocity_x /= 2;
					step->velocity_z /= 2;
					work->shake_frames = 4;
					AudioCommand_PlayFar(134);
					for (j = 0; j != work->effect->count; j++)
						ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 8);
				}
				step->x += step->velocity_x;
				step->y += step->velocity_y;
				step->z += step->velocity_z;
			}
		}
		Camera_ApplyShake(4, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

// 倍率 {x, y} (Q16)
typedef struct Scale
{
	s32 x;
	s32 y;
} Scale;

extern const struct Scale BattleFx_StarsUnitScale;
extern u8				  WhirlwindComets_GridX[];
extern u8				  WhirlwindComets_GridY[];
extern u16				  WhirlwindComets_Frames[];
extern u8				  WhirlwindComets_CellWidths[];
extern u8				  WhirlwindComets_CellHeights[];
extern u8				  WhirlwindComets_CellBiasY[];
extern u16				  WhirlwindComets_CellOffsets[];

void BattleFx_RunWhirlwindComets(struct BattleEffectArgument *argument)
{
	void				   **cells;
	void					*draw_destination;
	void					*view;
	struct BattleEffectWork *runtime;
	BlitterPair				 draw;
	s32						 frame;
	u8						*graphics;
	struct Point2D			 wave;
	struct Point2D			 drift;
	struct MotionObject		*object;
	struct BtlObj			*context;
	struct EffectStep		*point;
	struct EffectStep		*spark;
	struct EffectStep		*mark;
	u8						*tbl;
	u8						*src;
	s32						 index;
	s32						 slot;
	s32						 size;
	s32						 span;
	s32						 dist;
	s32						 alive;
	s32						 width;
	s32						 heading[8];
	s32						 radius[8];
	s32						 spin[8];
	struct Point2D			 home[8];
	s32						 base_angle[8];
	struct Vector3			 work;
	struct EffectPosition	 screen;
	struct Vector3			 shake;
	struct Placement		 place;
	struct Scale			 scale;

	cells = &gBattleFxWork[1];
	draw_destination = cells[0];
	view = *(void **)((u8 *)cells - 112);
	runtime = cells[-1];
	graphics = cells[1];
	runtime->effect = argument;

	BattleFx_BeginCanvasLayer(0);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = (DrawRectangle)cells[6];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
	draw[1] = (DrawRectangle)cells[7];

	tbl = Resource_GetTableEntry((s32)&ResourceId_WhirlwindSheet);
	Iwram_CopyWords((void *)0x05000000, tbl, 0x80);
	tbl += 0x80;
	Resource_DecodeType01(tbl, runtime);
	Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);

	runtime->transfer_mode = 2;
	runtime->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	wave.x = 0x800000;
	wave.y = 0x280000;
	drift.x = 0;
	drift.y = 0;

	for (index = 0; index != runtime->effect->count; index++)
	{
		context = GetBattleObjectSlotFar(runtime->effect->actors[index]);
		object = context->object;
		home[index].x = object->x;
		home[index].y = object->z;
		base_angle[index] = object->angle;
		heading[index] = (u16)ArcTan2(object->x, object->z);
		radius[index] =
			Iwram_Sqrt((object->x >> 8) * (object->x >> 8) + (object->z >> 8) * (object->z >> 8))
			>> 7;
		spin[index] = 0;
		object->vertical_motion_strength = 0;
	}

	index = 0;
	do
	{
		point = &runtime->particles[index];
		point->z = 120;
		point->y = 0;
		Random16();
		point->velocity_y = 0;
		point->velocity_x = 0;
		point->variant = Random16() & 63;
		index++;
	} while (index != 64);

	width = 48;
	frame = 0;
	do
	{
		if ((gKeysRepeat & (KEY_A | KEY_B)) != 0)
		{
			if (frame <= 159)
				frame = 160;
			else if (frame <= 394)
				frame = 395;
		}

		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(view, (u8 *)view + 12);

		if (frame == 16)
			AudioCommand_PlayFar(141);
		if (frame == 256)
			AudioCommand_PlayFar(140);
		if (frame == 334)
			AudioCommand_PlayFar(212);
		if (frame == 347)
			AudioCommand_PlayFar(212);
		if (frame == 359)
			AudioCommand_PlayFar(212);
		if (frame == 372)
			AudioCommand_PlayFar(212);

		for (index = 0; index != runtime->effect->count; index++)
		{
			if (frame > index * 16)
			{
				context = GetBattleObjectSlotFar(runtime->effect->actors[index]);
				object = context->object;
				object->x = (radius[index] * Trig_Sin(heading[index])) >> 1;
				object->z = (radius[index] * Trig_Cos(heading[index])) >> 1;

				if (frame <= 159)
				{
					if (frame > index * 16 + 16)
						spin[index] += 48;
					if (radius[index] <= 31)
						object->y += 0x60000;
					else
						object->y += 0x8000;
					if (object->y > 0x7c0000)
						object->y = 0x7c0000;
				}

				if (frame <= 490)
				{
					heading[index] += spin[index];
					if (heading[index] > 0x10000)
						heading[index] -= 0x10000;
					object->angle += spin[index] / 4;
				}

				if (frame == 395)
					object->vertical_motion_strength = 0xab85;

				slot = 0;
				do
				{
					if (frame == WhirlwindComets_Frames[slot])
					{
						object->velocity_y = 0;
						ObjectGroup_UpdateMembers(runtime->effect->actors[index], 7, 5, index, 8);
					}
					slot++;
				} while (slot != 4);

				if (radius[index] > 16)
					radius[index] -= 2;
			}
		}

		if ((u32)(frame - 16) <= 143)
		{
			size = frame * 2 - 32;
			if (size > 48)
				size = 48;
			draw[0](draw_destination, &runtime->sheet[frame / 4 % 3 * 3072 + (width - size) * 48],
				32, 112 - size, width, size);
		}

		if ((u32)(frame - 48) <= 111)
		{
			size = frame * 2 - 96;
			if (size > 64)
				size = 64;
			draw[0](draw_destination, &runtime->sheet[frame / 4 % 3 * 3072 + (64 - size) * 48], 32,
				64 - size, width, size);
		}

		if ((u32)(frame - 160) <= 239)
		{
			src = (u8 *)runtime + frame / 4 % 3 * 3072;
			draw[0](draw_destination, src, 8, 0, width, 64);
			draw[0](draw_destination, src, 8, 64, width, 64);
		}

		if (frame <= 159)
		{
			index = 0;
			point = runtime->particles;
			do
			{
				if (point[index].variant == 0)
				{
					work.x = point[index].z * Trig_Sin(point[index].velocity_y);
					work.y = point[index].y;
					work.z = point[index].z * Trig_Cos(point[index].velocity_y);
					EffectPosition_ApplyBaseAndYOffset(&work.x, &screen);
					screen.x >>= 1;
					if (point[index].y <= 0x3fffff)
					{
						draw[0](draw_destination, (u8 *)runtime + 0x2400, screen.x - 12,
							screen.y - 12, 16, 16);
					}
					if (point[index].z > 24)
						point[index].z -= 4;
					point[index].velocity_y += point[index].velocity_x * 2;
					if (point[index].velocity_y > 0x10000)
						point[index].velocity_y -= 0x10000;
					point[index].velocity_x += 50;
					if (point[index].velocity_x > 4096)
						point[index].velocity_x = 4096;
					point[index].velocity_y += 2048;
					point[index].y += point[index].velocity_y * 2;
					if (point[index].y > 0x300000)
					{
						point[index].y = 0;
						point[index].z = 100;
						point[index].velocity_y = 0;
						point[index].velocity_x = 0;
					}
				}
				else
				{
					point[index].variant--;
				}
				index++;
			} while (index != 24);
		}

		if (frame == 160)
		{
			s32 speed;
			s32 angle;

			BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
			BattlePresentation_ConfigurePaletteFadeFar(1, (s32)&ResourceId_VioletSkyBackdrop, 8);
			BattleFx_SelectLivingTargets(runtime->effect);
			BattleFx_SpawnObjects(9, 376, 2);

			tbl = Resource_GetTableEntry((s32)&ResourceId_VioletCometSheet);
			Iwram_CopyWords((void *)0x05000000, tbl, 0x80);
			tbl += 0x80;
			Resource_DecodeType01(tbl, (u8 *)runtime + 0x3600);

			index = 0;
			do
			{
				PARTICLES[index].variant = 0;
				index++;
			} while (index != 512);

			for (index = 0; index != runtime->effect->count; index++)
			{
				context = GetBattleObjectSlotFar(runtime->effect->actors[index]);
				object = context->object;
				object->y = ((Random16() & 15) + 8) << 16;
			}

			gProjection.center_x = 72;

			index = 0;
			do
			{
				speed = Random16() & 127;
				angle = (Random16() & 0x7fff) + 0x9fff;
				runtime->particles[index].x = Trig_Sin(angle) * speed;
				runtime->particles[index].y = Trig_Cos(angle) * speed;
				runtime->particles[index].z = (Random16() % 200 - 100) << 16;
				runtime->particles[index].variant = 0;
				index++;
			} while (index != 32);

			for (index = 0; index != runtime->effect->count; index++)
			{
				radius[index] = 16;
				spin[index] = 6000;
			}
		}

		if (frame > 159)
		{
			scale = BattleFx_StarsUnitScale;
			size = 16;
			if (frame - 160 <= 64)
				size = 32;

			wave.x -= (Trig_Sin((frame - 160) << 7) * 32) >> 6;
			wave.y += (Trig_Sin((frame - 160) << 9) * size) >> 6;
			wave.x += drift.x;
			wave.y += drift.y;
			drift.x /= 2;
			drift.y /= 2;

			scale.x = 0x10000;
			scale.y = 0x10000;
			place.unknown_0c = 0;
			index = 0;
			do
			{
				place.x = (WhirlwindComets_GridX[index] << 16) + wave.x + 0x280000;
				place.y = 0xff0000 + index * 0x400000;
				place.z = ((WhirlwindComets_GridY[index] + index * 64) << 16) + wave.y;
				Object_ApplyProjectedPlacementFar(runtime->objects[index], &place, &scale, 0);
				index++;
			} while (index != 9);

			if (frame > 255)
			{
				s32 threshold;

				shake.z = 0x1000000;
				alive = 0;
				shake.x = 0;
				shake.y = 0;
				Render_ResetTransformState();
				SceneTransform_ApplyPosition(&shake);

				if (frame <= 329)
				{
					index = 0;
					do
					{
						s32 xx;
						s32 yy;
						s32 zz;

						point = &runtime->particles[index];
						xx = (point->x >> 8) * (point->x >> 8);
						yy = (point->y >> 8) * (point->y >> 8);
						zz = (point->z >> 8) * (point->z >> 8);
						dist = Iwram_Sqrt(xx + yy + zz) >> 9;
						if (dist != 0)
						{
							EffectPosition_ApplyBaseAndYOffset(&point->x, &screen);
							screen.x = (screen.x >> 17) + (wave.x >> 17) + 32;
							screen.y = (screen.y >> 16) + (wave.y >> 16) - 4;
							screen.depth >>= 16;
							if (screen.depth <= 169)
								screen.depth = 170;
							if (screen.depth > 350)
								screen.depth = 350;
							span = 6 - (screen.depth - 170) / 36;
							draw[1](draw_destination,
								graphics + ParticleStreams_CellOffsets[span - 1],
								screen.x - span / 2, screen.y - span, span, span * 2);
							point->x -= point->x / dist;
							point->y -= point->y / dist;
							point->z -= point->z / dist;
						}
						else
						{
							alive++;
						}
						index++;
					} while (index != 32);

					if (alive > 0)
					{
						span = alive / 10 + 1;
						draw[1](draw_destination, graphics + ParticleStreams_CellOffsets[span - 1],
							(wave.x >> 17) - span / 2 + 32, (wave.y >> 16) - span - 4, span,
							span * 2);
					}
				}

				slot = 0;
				do
				{
					if (frame == WhirlwindComets_Frames[slot])
					{
						s32 speed;
						s32 angle;

						index = 0;
						do
						{
							speed = (Random16() & 127) + 16;
							angle = Random16() & 0xffff;
							spark = &PARTICLES[slot * 128 + index];
							spark->velocity_x = (Trig_Sin(angle) * speed) >> 6;
							spark->velocity_y = -(Trig_Cos(angle) * speed) >> 6;
							spark->velocity_y = ((Random16() & 255) - 128) << 10;
							spark->x = 0;
							spark->y = 0;
							spark->z = 0;
							spark->variant = (Random16() & 15) + 64;
							index++;
						} while (index != 96);
					}

					if (frame >= WhirlwindComets_Frames[0])
					{
						spark = PARTICLES;
						index = 0;
						do
						{
							if (spark[index].variant > 0)
							{
								EffectPosition_ApplyBaseAndYOffset(&spark[index].x, &screen);
								screen.x = (screen.x >> 17) + 32;
								screen.y = (screen.y >> 16) + 56;
								screen.depth >>= 16;
								if (screen.depth <= 169)
									screen.depth = 170;
								if (screen.depth > 350)
									screen.depth = 350;
								span = 3 - (screen.depth - 170) / 90;
								draw[0](draw_destination,
									graphics + ParticleStreams_CellOffsets[span - 1],
									screen.x - span / 2, screen.y - span, span, span * 2);
								spark[index].x += spark[index].velocity_x;
								spark[index].y += spark[index].velocity_y;
								spark[index].z += spark[index].velocity_y;
								spark[index].variant--;
							}
							index++;
						} while (index != 512);
					}
					slot++;
				} while (slot != 4);

				index = 0;
				do
				{
					mark = &runtime->particles[index + 60];
					threshold = index * 12 + 330;
					if (frame == threshold)
					{
						mark->x = (wave.x >> 17) + 32;
						mark->y = (wave.y >> 16) - 4;
						mark->velocity_x = (wave.x >> 17) + 32;
						mark->velocity_y = (wave.y >> 16) - 4;

						slot = 0;
						do
						{
							runtime->particles[slot + 32].variant = 4;
							slot++;
						} while (slot != 28);

						drift.x = 0x80000;
						drift.y = -0x20000;
						runtime->shake_frames = 8;
					}

					if (frame >= threshold)
					{
						s32 size;

						size = (frame - threshold) / 2;
						if (size > 2)
							size = 2;
						draw[1](draw_destination, (u8 *)runtime + WhirlwindComets_CellOffsets[size],
							mark->x, mark->y - WhirlwindComets_CellBiasY[size],
							WhirlwindComets_CellWidths[size], WhirlwindComets_CellHeights[size]);
						mark->x -= 8;
						mark->y += 2;

						if (frame < threshold + 8)
						{
							SceneTransform_ApplyRoll(-2048);
							SceneTransform_ApplyYaw(-4096);

							slot = 0;
							spark = &runtime->particles[32];
							do
							{
								struct EffectStep *step;

								step = &spark[slot];
								work.x = 0;
								work.y = step->variant * Trig_Cos(slot * 2340);
								work.z = step->variant * Trig_Sin(slot * 2340);
								step->variant += 2;
								EffectPosition_ApplyBaseAndYOffset(&work.x, &screen);
								screen.x =
									(screen.x >> 17) + runtime->particles[index + 60].velocity_x;
								screen.y =
									(screen.y >> 16) + runtime->particles[index + 60].velocity_y;
								screen.depth >>= 16;
								if (screen.depth <= 169)
									screen.depth = 170;
								if (screen.depth > 350)
									screen.depth = 350;
								span = 3 - (screen.depth - 170) / 90;
								draw[0](draw_destination,
									graphics + ParticleStreams_CellOffsets[span - 1],
									screen.x - span / 2, screen.y - span, span, span * 2);
								slot++;
							} while (slot != 28);
						}
					}
					index++;
				} while (index != 4);
			}
		}

		ObjectGroup_TickMemberTimers();
		runtime->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	} while (frame != 400);

	BattleEventRuntime_BeginPhaseFar(134);

	for (index = 0; index != runtime->effect->count; index++)
	{
		struct MotionObject *obj;

		context = GetBattleObjectSlotFar(runtime->effect->actors[index]);
		obj = context->object;
		obj->x = home[index].x;
		obj->z = home[index].y;
		obj->angle = base_angle[index];
	}

	gProjection.center_x = 120;

	BattleEffect_SetupBlendedDisplay();

	index = 0;
	do
	{
		ResourceObject_ReleaseFar(runtime->objects[index]);
		index++;
	} while (index != 9);

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// runtime_draw_render_animation_mode1.c
// バトル演出：粒が昇る演出 (embers.c) の二つの入口
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// mode 1 : 使う者の側から出し、LightningBolt のパレットで描く
//--------------------------------------------------------------------------
void BattleFx_RenderAnimationMode1(s32 effect)
{
	BattleEffect_RunPaletteParticles((struct BattleEffectArgument *)effect, 1);
}

//--------------------------------------------------------------------------
// mode 0 : 8 体の隊列を出して動かし、最後にその位置で爆発を起こす
//--------------------------------------------------------------------------
void BattleFx_RenderAnimationMode0(s32 effect)
{
	BattleEffect_RunPaletteParticles((struct BattleEffectArgument *)effect, 0);
}

//--------------------------------------------------------------------------
// embers.c
// バトル演出：粒を下から昇らせる演出と、粒を噴き上げて 3 本の柱を流す演出
//--------------------------------------------------------------------------
// PARTICLES : 昇る粒 (最大 512)
//--------------------------------------------------------------------------
// 208 フレームの演出。49〜159 フレームに A か B を押すと 160 フレームへ飛ばす
// mode : 0 なら 8 体の隊列を出して動かし、最後にその位置で ImpactBurst を起こす
//        1 なら effect->side で決まる位置から火花と閃光を出し、
//        LightningBolt のパレットで描く
// 粒は 4 種類 (キャンバス座標、y は下向きが正)：
//   PARTICLES[0..511]    昇る粒：y = 112 から上へ加速して昇る。33〜103 フレームは
//                     1 フレームに 1 つ、104〜175 フレームは 8 つずつ出す
//   particles[40..63] 火花：41〜127 の奇数フレームに 1 つずつ、斜め上へ飛ぶ
//   particles[0..23]  閃光：129〜175 フレームに 1 つずつ、横へ飛ぶ
//   particles[24..39] 筋：上から斜めに落ち、y が 55 を越えたら絵を繰り返す
//--------------------------------------------------------------------------
void BattleEffect_RunPaletteParticles(struct BattleEffectArgument *effect, s32 mode)
{
	struct EffectPosition	 origin;
	void				   **cursor;
	void					*canvas;
	struct BattleCamera		*camera;
	struct BattleEffectWork *work;
	BlitterPair				 draw;
	s32						 frame;
	u8						*sheet;
	struct Point2D			 focus;
	s32						 dir;
	u8						*palette;
	s32						 i;

	// cursor は gWorkSlot[HEAP_SLOT_BATTLE_CANVAS] (キャンバス) を指す
	cursor = &gBattleFxWork[1];
	canvas = cursor[0];
	camera = cursor[-28]; // [12] カメラ
	work = cursor[-1];	  // [39] 作業領域
	sheet = cursor[1];	  // [41] 粒の絵
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	REG_BG2CNT = 0x784;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw[0] = cursor[6];
	BattleEffect_LoadWork(
		HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
	draw[1] = cursor[7];

	// 閃光は FlashBurst、筋は EmberStreak (sheet + 0x3000) の絵
	palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
	palette += 128;
	Resource_DecodeType01(palette, work->sheet + 0x3000);
	Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), sheet);
	if (mode == 1)
		Iwram_CopyWords(
			(void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet), 128);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// mode 1 で side が 1 なら左右を逆にする
	if (mode == 0)
		dir = 1;
	else if (work->effect->side == 1)
		dir = -1;
	else
		dir = 1;
	if (mode == 1)
	{
		EffectPosition_ApplyStepAndYOffset(work->effect->actor, &origin);
		origin.x /= 2;
		origin.y = 66;
		if (work->effect->side == 1)
			origin.x = 76;
		else
			origin.x = 44;
	}

	// 隊列の中心は (212, -60) から
	focus.x = 212 << 16;
	focus.y = -(60 << 16);
	for (i = 0; i != 64; i++)
		work->particles[i].variant = -1;
	for (i = 0; i != 16; i++)
	{
		struct EffectStep *streak = &work->particles[24 + i];

		if (dir == 1)
			streak->x = (Random16() & 127) + 128;
		else
			streak->x = (Random16() & 127) - 128;
		streak->y = (Random16() & 7) - 72;
		streak->variant = -(Random16() & 31); // 落ち始めるまでの待ち
	}
	for (i = 0; i != 512; i++)
		PARTICLES[i].variant = -1;
	if (mode == 0)
	{
		BattleFx_SelectLivingTargets(work->effect);
		WaitFrames(1);
		BattleFx_SpawnObjects(8, 377, 2);
	}

	for (frame = 0; frame != 208; frame++)
	{
		// A か B で 160 フレームへ
		if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 48 && frame <= 159)
		{
			if (mode == 0)
			{
				AnimationObjects_SelectAnimationFar(work->objects[0], 8);
				AnimationObjects_SelectAnimationFar(work->objects[1], 9);
				AnimationObjects_SelectAnimationFar(work->objects[3], 10);
				AnimationObjects_SelectAnimationFar(work->objects[4], 11);
			}
			for (i = 0; i != work->effect->count; i++)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 10, 5, -1, 0);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
			}
			frame = 160;
		}
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame == 178)
			BattleEventRuntime_BeginPhaseFar(134);

		// 128 から残像を半分に、160〜175 はキャンバスを塗りつぶしてパレットを明るくし、
		// 176 からは転送のたびに画素値を 2 ずつ下げて消す
		if (frame == 128)
			work->transfer_value = 50;
		if (frame == 176)
		{
			work->transfer_mode = 3;
			work->transfer_value = 0x02020202;
			palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
			Iwram_CopyWords((void *)0x05000000, palette, 128);
		}
		else if (frame >= 160 && frame <= 175)
		{
			work->transfer_mode = 1;
			work->transfer_value = 0x10101010;
			if (frame > 173)
				work->transfer_value = 0x3f3f3f3f;
			Palette_BrightenBgEntries(2, 2, 2);
		}

		//------------------------------------------------------------------
		// 新しい粒を出す
		//------------------------------------------------------------------
		// 昇る粒：y = 112 から上向きに出す
		if (frame >= 33 && frame <= 175)
		{
			s32 spawned = 0;
			s32 limit = 1;

			if (frame > 103)
				limit = 8;
			for (i = 0; i != 512; i++)
			{
				struct EffectStep *grain = &PARTICLES[i];

				if (grain->variant == -1)
				{
					grain->x = ((Random16() & 255) - 32) << 16;
					grain->y = 112 << 16;
					grain->velocity_x = ((Random16() & 127) + PaletteParticles_DropSpeeds[i & 3])
										<< 9;
					grain->velocity_y = -((Random16() & 127) + PaletteParticles_DropSpeeds[i & 3])
										<< 11;
					grain->variant = 0;
					spawned++;
					if (spawned == limit)
						break;
				}
			}
		}
		// 火花：奇数フレームに 1 つ、斜め上へ出す
		if (frame >= 41 && frame <= 127)
		{
			s32 spawned = 0;

			if (frame & 1)
			{
				for (i = 0; i != 24; i++)
				{
					struct EffectStep *spark = &work->particles[40 + i];

					if (spark->variant == -1)
					{
						s32 speed = (Random16() & 255) + 128;
						s32 angle = (Random16() & 0x1fff) + 20000;

						if (mode == 0)
						{
							spark->x = ((Random16() & 7) + 78) << 16;
							spark->y = 70 << 16;
						}
						else
						{
							spark->x = ((Random16() & 7) + origin.x - 8) << 16;
							spark->y = origin.y << 16;
						}
						spark->velocity_x = speed * Trig_Sin(angle) >> 9;
						spark->velocity_y = speed * Trig_Cos(angle) >> 9;
						spawned++;
						spark->variant = 0;
						if (spawned == 1)
							break;
					}
				}
			}
		}
		if (frame == 48)
			AudioCommand_PlayFar(141);
		if (frame == 128)
			AudioCommand_PlayFar(145);
		// 閃光：1 フレームに 1 つ、横へ出す
		if (frame >= 129 && frame <= 175)
		{
			s32 spawned = 0;

			for (i = 0; i != 24; i++)
			{
				struct EffectStep *flash = &work->particles[i];

				if (flash->variant == -1)
				{
					s32 speed = (Random16() & 255) + 128;
					s32 angle = (Random16() & 0x1fff) - 20000;

					if (mode == 0)
					{
						flash->x = 68 << 16;
						flash->y = 64 << 16;
					}
					else
					{
						flash->x = origin.x << 16;
						flash->y = origin.y << 16;
					}
					flash->velocity_x = speed * Trig_Sin(angle) >> 6;
					flash->velocity_y = speed * Trig_Cos(angle) >> 6;
					flash->variant = 0;
					spawned++;
					if (spawned == 1)
						break;
				}
			}
		}

		//------------------------------------------------------------------
		// 粒を描いて動かす
		//------------------------------------------------------------------
		// 閃光：4 フレームごとに絵を進め、24 フレームで消える
		if (frame <= 175)
		{
			for (i = 0; i != 24; i++)
			{
				struct EffectStep *flash = &work->particles[i];

				if (flash->variant >= 0)
				{
					draw[0](canvas, work->sheet + ((flash->variant >> 2) << 11), HI(flash->x) - 16,
						HI(flash->y) - 32, 32, 64);
					flash->x += flash->velocity_x * dir;
					flash->y += flash->velocity_y;
					flash->variant++;
					if (flash->variant == 24)
						flash->variant = -1;
				}
			}
		}

		// 火花：1×2 の点、上へ加速して 48 フレームで消える
		for (i = 0; i != 24; i++)
		{
			struct EffectStep *spark = &work->particles[40 + i];
			s32				   size = 1;

			if (spark->variant >= 0)
			{
				draw[1](canvas, PaletteParticles_SparkPixels, HI(spark->x), HI(spark->y) - size,
					size, size * 2);
				spark->x += spark->velocity_x * dir;
				spark->y += spark->velocity_y;
				spark->velocity_y += -0x400;
				spark->variant++;
				if (spark->variant == 48)
					spark->variant = -1;
			}
		}

		// 昇る粒：128 フレームまでは上へ加速し、その後は上昇の速さを保ったまま
		// x の速度を毎フレーム減らして左へ曲げる (dir = -1 なら右、奇数番が強い)。
		// 256 フレームで消える
		if (frame <= 175)
		{
			for (i = 0; i != 512; i++)
			{
				struct EffectStep *grain = &PARTICLES[i];

				if (grain->variant >= 0)
				{
					u8	size = PaletteParticles_DropSizes[i & 3];
					s32 height = size * 2;

					draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
						HI(grain->x) - size / 2, HI(grain->y) - size, size, height);
					grain->x += grain->velocity_x * dir;
					grain->y += grain->velocity_y;
					if (frame > 128)
					{
						if (i & 1)
							grain->velocity_x += -0x8000;
						else
							grain->velocity_x += -0x2000;
					}
					else
					{
						grain->velocity_y += -0x400;
					}
					grain->variant++;
					if (grain->variant == 256)
						grain->variant = -1;
				}
			}
		}

		//------------------------------------------------------------------
		// 揺れと隊列
		//------------------------------------------------------------------
		if (frame == 128)
			work->shake_frames = 48;
		if (mode == 0 && frame == 48)
			work->shake_frames = 8;

		// 40〜47 フレームに隊列を 1 フレームあたり左へ 8、下へ 16 ドット動かす
		if (frame >= 40 && frame <= 47)
		{
			focus.x += -(8 << 16);
			focus.y += 16 << 16;
		}
		if (mode == 0)
		{
			if (frame == 128)
			{
				AnimationObjects_SelectAnimationFar(work->objects[0], 8);
				AnimationObjects_SelectAnimationFar(work->objects[1], 9);
				AnimationObjects_SelectAnimationFar(work->objects[3], 10);
				AnimationObjects_SelectAnimationFar(work->objects[4], 11);
			}
			if (frame == 176)
			{
				AnimationObjects_SelectAnimationFar(work->objects[0], 0);
				AnimationObjects_SelectAnimationFar(work->objects[1], 1);
				AnimationObjects_SelectAnimationFar(work->objects[3], 3);
				AnimationObjects_SelectAnimationFar(work->objects[4], 4);
			}
			BattleFx_PlaceFormationObjects(3, focus.x, focus.y);
		}
		if (frame == 138)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 10, 5, -1, 0);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
			}
		}

		// 筋：待ちの後、1 フレームに 6 ドットずつ斜めに落ち、y = 55 を越えたら
		// 2 フレームごとに 6 枚の絵を繰り返す
		if (frame <= 175)
		{
			for (i = 0; i != 16; i++)
			{
				struct EffectStep *streak = &work->particles[24 + i];

				if (streak->y > 55)
				{
					if (streak->variant >= 0 && streak->variant <= 11)
					{
						s32 cel = streak->variant / 2;

						draw[0](canvas, work->sheet + PaletteParticles_StreakOffsets[cel] + 0x3000,
							streak->x - PaletteParticles_StreakWidths[cel] / 2,
							streak->y + PaletteParticles_StreakDrops[cel],
							PaletteParticles_StreakWidths[cel],
							PaletteParticles_StreakHeights[cel]);
					}
					streak->variant++;
					if (streak->variant == 12)
						streak->variant = 0;
				}
				else if (streak->variant == 0)
				{
					s32 size = 10;

					streak->x -= dir * 6;
					streak->y += 6;
					if (frame <= 47 && streak->y > 55)
						AudioCommand_PlayFar(136);
					draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
						streak->x - size / 2, streak->y + 30, size, size * 2);
				}
				else
				{
					streak->variant++;
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
	if (mode == 0)
	{
		BattleEffect_RunImpactBurst(3, focus.x, focus.y);
		for (i = 0; i != 8; i++)
			ResourceObject_ReleaseFar(work->objects[i]);
	}
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 124 フレームの演出。33〜97 フレームに A か B を押すと 98 フレームへ飛ばす
// 12 体の隊列を右から入れ、28 フレームと 32〜79 フレームに粒を噴き上げる
// 粒は重力で落ち、80 フレームを過ぎると上へ加速して昇る
// 32〜79 フレームには 3 本の柱 (FirePillarA の絵) を上へ流す
//--------------------------------------------------------------------------
void BattleEffect_RunEmberColumns(struct BattleEffectArgument *effect)
{
	BlitterPair				 draw;
	void				   **cursor;
	void					*canvas;
	struct BattleEffectWork *work;
	u8						*graphics;
	u8						*palette;

	// cursor は gWorkSlot[HEAP_SLOT_BATTLE_CANVAS] (キャンバス) を指す
	cursor = &gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	canvas = cursor[0];
	work = cursor[-1];	  // [39] 作業領域
	graphics = cursor[1]; // [41] 粒の絵
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	BattleFx_FetchRectangleBlitters(0, draw);

	// 下の帯は WaterSpray、柱は FirePillarA (sheet + 0x6e4) の絵
	palette = Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	palette = Resource_GetTableEntry((s32)&ResourceId_FirePillarSheetA);
	palette += 128;
	Resource_DecodeType01(palette, work->sheet + 0x6e4);
	Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	{
		struct Point2D	   focus;
		struct Point2D	   velocity;
		s32				   rise;
		s32				   frame;
		s32				   k;
		struct EffectStep *point;
		struct EffectStep *spark;
		struct EffectStep *sparks;
		s32				  *life;

		// 隊列は (256, 88) から左へ
		rise = 0;
		focus.x = 256 << 16;
		focus.y = 88 << 16;
		velocity.x = -(16 << 16);
		velocity.y = -(4 << 16);
		for (k = 0; k != 64; k++)
			work->particles[k].variant = -1;
		for (k = 0; k != 16; k++)
		{
			point = &work->particles[24 + k];
			point->x = Random16() & 127;
			point->y = (Random16() & 7) + 56;
			point->variant = -(Random16() & 15);
		}
		life = &gMapCellBuffer->variant;
		for (k = 0; k != 1024; k++)
		{
			*life = -1;
			life += 7; // sizeof (struct EffectStep) / 4
		}
		sparks = gMapCellBuffer;
		BattleFx_SelectLivingTargets(work->effect);
		WaitFrames(1);
		BattleFx_SpawnObjects(12, 380, 2);

		for (frame = 0; frame != 124; frame++)
		{
			if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 32 && frame <= 97)
				frame = 98;
			if (frame == 120)
				BattleEventRuntime_BeginPhaseFar(134);
			if (frame <= 15)
				rise += 2;

			// 隊列：速度を毎フレーム x 58/64、y 56/64 に減らし、x = 120 より左では右へ押し戻す
			if (frame <= 99)
			{
				focus.x += velocity.x;
				focus.y += velocity.y;
				velocity.x = velocity.x * 58 / 64;
				velocity.y = velocity.y * 56 / 64;
				if (focus.x < 120 << 16)
					velocity.x += 0x8000;
			}
			BattleFx_PlaceFormationObjects(1, focus.x, focus.y);

			// 粒：(32, 96) のまわりから上向きに、28 フレームに 256 個、
			// 32〜79 フレームには 1 フレームに 16 個ずつ噴き上げる
			if (frame == 28)
			{
				for (k = 0; k != 256; k++)
				{
					spark = &sparks[k];
					if (spark->variant == -1)
					{
						s32 radius = Random16() & 63;
						s32 angle = Random16() & 0xffff;

						spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
						spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
						spark->velocity_x = ((Random16() & 63) - 32) << 14;
						spark->velocity_y = (-(Random16() & 63) - 8) << 13;
						spark->variant = 0;
					}
				}
			}
			if (frame >= 32 && frame < 80)
			{
				s32 spawned = 0;

				for (k = 0; k != 1024; k++)
				{
					spark = &sparks[k];
					if (spark->variant == -1)
					{
						s32 radius = Random16() & 63;
						s32 angle = Random16() & 0xffff;

						spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
						spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
						spark->velocity_x = ((Random16() & 63) - 32) << 14;
						spark->velocity_y = (-(Random16() & 63) - 8) << 13;
						spark->variant = 0;
						spawned++;
						if (spawned == 16)
							break;
					}
				}
			}
			if (frame == 0)
				AudioCommand_PlayFar(164);
			if (frame == 32)
				AudioCommand_PlayFar(145);
			if (frame == 80)
				AudioCommand_PlayFar(144);

			// 柱：104 ドットの絵を 1 フレームに 16 ドットずつ上へ流す
			if (frame >= 32 && frame < 80)
			{
				for (k = 0; k != 3; k++)
				{
					s32 span = (frame * 16 - 256 + k * 25) % 104;

					draw[0](canvas, work->sheet + 0x6e4, EmberColumns_Columns[k * 2] - 17,
						EmberColumns_Columns[k * 2 + 1] - span - 104, 34, 104);
					draw[0](canvas, work->sheet + 0x6e4, EmberColumns_Columns[k * 2] - 17,
						EmberColumns_Columns[k * 2 + 1] - span, 34, span);
				}
			}

			// 下の帯：32×32 の絵を 5 枚、16 フレームで 32 ドットせり上がる
			if (frame <= 95)
			{
				for (k = 0; k != 5; k++)
					draw[0](canvas, work, k * 32 + ((frame / 4) & 31) - 32, 120 - rise, 32, 32);
			}

			// 粒：68 フレームを過ぎると大きくし、落ちているものは加算合成で描く。
			// 80 フレームまでは EmberColumns_Gravity で下へ、その後は上へ加速し、
			// 速度は毎フレーム 62/64 に減らす。落ちながら y が 104 を越えたら消す
			for (k = 0; k != 1024; k++)
			{
				spark = &sparks[k];
				if (spark->variant >= 0)
				{
					s32 size = k % 3 + 2;

					if (spark->velocity_y > 0)
						size += 2;
					if (frame > 68 && size <= 5)
						size = 6;
					if (frame > 70 && size <= 6)
						size = 7;
					if (frame > 72 && size <= 7)
						size = 8;
					if (frame > 74 && size <= 8)
						size = 9;
					if (frame > 76)
						size = 10;
					draw[spark->velocity_y > 0](canvas,
						graphics + ParticleStreams_CellOffsets[size - 1], HI(spark->x) - size / 2,
						HI(spark->y) - size, size, size * 2);
					spark->x += spark->velocity_x;
					spark->y += spark->velocity_y;
					if (frame > 80)
						spark->velocity_y += -0x8000;
					else
						spark->velocity_y += EmberColumns_Gravity[k & 3];
					spark->velocity_x = spark->velocity_x * 62 / 64;
					spark->velocity_y = spark->velocity_y * 62 / 64;
					spark->variant++;
					if (spark->velocity_y > 0 && HI(spark->y) > 104)
						spark->variant = -1;
				}
			}

			// 対象：36・48・60・72 フレームにパレット 7 で跳ねさせ、6 フレーム後にパレットを戻す
			if (frame <= 79)
			{
				for (k = 0; k != work->effect->count; k++)
				{
					if (frame > 29)
					{
						s32 phase = frame % 12;

						if (phase == 0)
						{
							struct MotionObject *obj =
								GetBattleObjectSlotFar(work->effect->actors[k])->object;

							ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, -1, 0);
							obj->velocity_y = 0x48000;
							obj->vertical_motion_strength = 0xab85;
						}
						if (phase == 6)
							ObjectGroup_UpdateMembers(work->effect->actors[k], 0, 5, -1, 0);
					}
				}
			}
			work->transfer_pending = 1;
			WaitFrames(1);
		}

		Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleEffect_RunImpactBurst(1, focus.x, focus.y);
		for (k = 0; k != 12; k++)
			ResourceObject_ReleaseFar(work->objects[k]);
	}
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunVortexMotes(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 pos;
	struct BattleEffectWork *work;
	void					*canvas;
	BlitterPair				 draw;
	u8						*palette;
	struct MotionObject		*source;
	struct MotionObject		*object;
	struct EffectStep		*point;
	struct EffectStep		*seat;
	struct EffectStep		*target;
	struct BattleCamera		*camera;
	s32						 frame;
	s32						 y;
	s32						 i;
	s8						*flags;

	flags = (s8 *)gMapCellBuffer;
	work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = (DrawRectangle) * (gWorkSlot + HEAP_SLOT_BLITTER);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 15, 2);
	REG_BLDALPHA = 0x0f0f;
	draw[1] = (DrawRectangle) * (gWorkSlot + HEAP_SLOT_BLITTER_ALTERNATE);
	source = GetBattleObjectSlotFar(work->effect->actor)->object;
	y = source->y + Battle_GetObjectTableValueFar(work->effect->actor);
	for (i = 0; i != 30; i++)
	{
		point = &work->particles[i];
		point->x = source->x;
		point->y = y;
		point->z = source->z;
		point->velocity_x = ((s32)((Random16() & 255) - 127) << 16) >> 5;
		point->velocity_y = ((s32)((Random16() & 127) - 16) << 16) >> 6;
		point->velocity_z = ((s32)((Random16() & 255) - 127) << 16) >> 5;
		point->variant = -1;
		flags[i] = 0;
	}
	for (i = 0; i != work->effect->count; i++)
	{
		seat = &work->particles[32 + i];
		object = GetBattleObjectSlotFar(work->effect->actors[i])->object;
		seat->x = object->x;
		seat->y = 0;
		seat->z = object->z;
	}
	work->camera_delta = 0;
	work->camera_phase = 0;
	Scheduler_AddOrUpdateCallback(Camera_ApplyPhasedDelta, 0x480);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(164);

	for (frame = 0; frame != VortexMotes_Counts[work->effect->variant * 2 + 1]; frame++)
	{
		camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
		if (frame >= 17 && frame < 64)
			work->camera_delta = 0x180;
		else
			work->camera_delta = 0;
		if (frame == VortexMotes_Counts[work->effect->variant * 2 + 1] - 16)
			BattleEventRuntime_BeginPhaseFar(132);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		for (i = 0; i != VortexMotes_Counts[work->effect->variant * 2]; i++)
		{
			point = &work->particles[i];
			if (frame > i * 2 && flags[i] == 0)
			{
				EffectPosition_ApplyBaseAndYOffset((s32 *)point, &pos);
				pos.x >>= 1;
				if (pos.depth < 160)
					pos.depth = 160;
				if (pos.depth > 0x31f)
					pos.depth = 0x31f;
				draw[0](canvas, work->sheet + 0xc00, pos.x - 6, pos.y - 12, 12, 24);
				point->x += point->velocity_x;
				point->y += point->velocity_y;
				point->z += point->velocity_z;
			}
			if (frame > i * 2 + 48 && flags[i] == 0)
			{
				target = &work->particles[32 + i % work->effect->count];
				point->velocity_x += (target->x - point->x) >> 9;
				point->velocity_y += (target->y - point->y) >> 9;
				point->velocity_z += (target->z - point->z) >> 9;
				if (frame < i * 2 + 85)
				{
					point->velocity_x = point->velocity_x * 60 / 64;
					point->velocity_y = point->velocity_y * 60 / 64;
					point->velocity_z = point->velocity_z * 60 / 64;
				}
				if (point->y < 0)
				{
					flags[i] = 1;
					point->variant = 0;
					point->x = pos.x;
					point->y = pos.y + (Random16() & 31) - 16;
					ObjectGroup_UpdateMembers(work->effect->actors[i % work->effect->count], 7, 5,
						i % work->effect->count, 4);
					BattleMotion_ApplyVariantMotionFar(
						work->effect->actors[i % work->effect->count], 0);
					work->shake_frames = 4;
					AudioCommand_PlayFar(132);
				}
			}
			if (point->variant >= 0 && point->variant < 16)
			{
				draw[0](canvas, work->sheet + ((point->variant / 2) % 3 << 10), point->x - 16,
					point->y - 56, 16, 64);
				draw[1](canvas, work->sheet + ((point->variant / 2) % 3 << 10), point->x,
					point->y - 56, 16, 64);
				point->variant++;
			}
		}
		Camera_ApplyShake(work->effect->variant * 2 + 2, work->effect->variant * 2 + 2);
		if (work->camera_phase == 0)
			work->camera_phase = 1;
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(Camera_ApplyPhasedDelta);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunTornado(struct BattleEffectArgument *effect)
{
	BlitterPair				 draw;
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 wave;
	u8						*palette;
	struct EffectStep		*point;
	s32						 frame;
	s32						 x;
	s32						 y;
	s32						 segment;
	s32						 cell;
	s32						 i;
	s32						 j;

	heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BLDALPHA = 0x1010;
	palette = Resource_GetTableEntry((s32)&ResourceId_TornadoSheet);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = (DrawRectangle)heap_cache[7];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
	draw[1] = (DrawRectangle)heap_cache[8];
	for (j = 0; j != 16; j++)
	{
		point = &work->particles[j];
		point->x = Random16() & 31;
		point->y = (Random16() & 63) + 16;
		point->variant = -(Random16() & 15);
	}
	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	REG_BLDALPHA = 0x1000;
	WaitFrames(1);
	AudioCommand_PlayFar(141);

	for (frame = 0; frame != 80; frame++)
	{
		wave = Trig_Sin(frame << 10) << 4;
		if (frame == 32)
			BattleEventRuntime_BeginPhaseFar(133);
		for (i = 0; i != 7; i++)
		{
			if (frame == i * 8 + 16)
				Iwram_FillWords(canvas, 0x4000, 0x08080808);
		}
		if (work->effect->side == 1)
			wave += 0x200000;
		else
			wave -= 0x200000;
		if (frame <= 16)
			REG_BLDALPHA = frame | 0x1000;
		if (frame > 63)
			REG_BLDALPHA = (0x4f - frame) | 0x1000;
		for (i = 0; i != Tornado_Shapes[work->effect->variant * 3]; i++)
		{
			x = ((Tornado_Shapes[work->effect->variant * 3 + 1]
						 * Trig_Sin((frame << 11) + i * 0x4000)
					 + wave)
					>> 16)
				+ 40;
			y = (Trig_Cos((frame << 11) + i * 0x4000) * 2) >> 16;
			segment = (frame / 2) % 3;
			draw[0](canvas, work->sheet + segment * 0xa00 + 0xc56, x, y + 16, 40, 32);
			draw[0](canvas, work->sheet + segment * 0x500 + 0x2a56, x, y + 48, 40, 32);
			draw[0](canvas, work->sheet + segment * 0xa00 + 0x1156, x, y + 80, 40, 32);
			for (j = 0; j != 4; j++)
			{
				point = &work->particles[i * 4 + j];
				if (point->variant >= 0)
				{
					cell = point->variant / 2 + (j / 2) * 3;
					draw[j & 1](canvas, work->sheet + BattleFx_GlintCellOffsets[cell], point->x + x,
						point->y + y, BattleFx_GlintCellWidths[cell],
						BattleFx_GlintCellHeights[cell]);
				}
				point->variant++;
				if (point->variant == 6)
				{
					point->x = Random16() & 31;
					point->y = (Random16() & 63) + 16;
					point->variant = 0;
				}
			}
		}
		for (j = 0; j != work->effect->count; j++)
		{
			for (i = 0; i != 7; i++)
			{
				if (frame == i * 8 + j * 3 + 16)
				{
					ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 4);
					BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
				}
			}
		}
		work->shake_frames = 1;
		Camera_ApplyShake(Tornado_Shapes[work->effect->variant * 3 + 2],
			Tornado_Shapes[work->effect->variant * 3 + 2] * 2);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// crystal.c
// バトル演出：降ってきて砕ける結晶
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 結晶が降ってきて地面で砕ける
// Crystal_Counts は variant ごとに（結晶の数, 全体のフレーム数）
// 結晶 : particles[]（z は砕けたら 1、variant は降り始めるフレーム）
// 破片 : gMapCellBuffer に結晶 1 個あたり 16 個（z は回転の速さ）
//--------------------------------------------------------------------------
void BattleFx_RunFallingCrystals(struct BattleEffectArgument *effect)
{
	BlitterPair				 draw;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	struct EffectStep		*seed;
	struct EffectStep		*cry;
	struct EffectStep		*shards;
	struct EffectStep		*shard;
	s32						 x;
	s32						 cell;
	s32						 i;
	s32						 j;

	shards = gMapCellBuffer;
	cursor = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 1);
	BattleFx_FetchRectangleBlitters(work->effect->side, draw);
	work->transfer_mode = 2; // 残像 1/2
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 結晶を画面の上に並べ、8 フレームずつずらして降らせる
	for (j = 0; j != Crystal_Counts[work->effect->variant * 2]; j++)
	{
		seed = &work->particles[j];
		x = Random16() & 31;
		seed->y = -0x400000;
		if (work->effect->side == 1)
		{
			x = ((Random16() & 31) + 80) << 16;
			seed->velocity_x = (Random16() & 63) << 12;
		}
		else
		{
			x = ((Random16() & 31) + 8) << 16;
			seed->velocity_x = -(Random16() & 63) << 12;
		}
		seed->x = x - seed->velocity_x * 18;
		seed->velocity_y = 0;
		seed->z = 0;
		seed->variant = j * 8;
	}

	for (frame = 0; frame != Crystal_Counts[work->effect->variant * 2 + 1]; frame++)
	{
		if (work->effect->variant == 2 && frame <= 103)
		{
			// カメラを横に回す（96 フレーム目から遅くする）
			struct BattleCamera *camera = gCameraWork;
			s32					 speed;

			if (frame <= 95)
				speed = 192;
			else
				speed = 2496 - frame * 24;
			if (work->effect->side == 0)
				camera->yaw -= speed;
			else
				camera->yaw += speed;
		}
		if (frame == Crystal_Counts[work->effect->variant * 2 + 1] - 80)
			BattleEventRuntime_BeginPhaseFar(134);
		if (frame == Crystal_Counts[work->effect->variant * 2 + 1] - 8)
		{
			// 最後の 8 フレームは暗くしていく
			work->transfer_mode = 3;
			work->transfer_value = 0x06060606;
		}
		if (frame <= Crystal_Counts[work->effect->variant * 2 + 1] - 8)
		{
			for (i = 0; i != Crystal_Counts[work->effect->variant * 2]; i++)
			{
				cry = &work->particles[i];
				if (cry->z == 1)
				{
					// 砕けた結晶：破片を回しながら飛ばす
					for (j = 0; j != 16; j++)
					{
						cell = (j % 5) * 3 + (shards[i * 16 + j].variant / 96) % 3;
						shard = &shards[i * 16 + j];
						draw[j <= 2](canvas, work->sheet + Crystal_ShardOffsets[cell] + 0x800,
							HI(shard->x) - Crystal_ShardWidths[cell] / 2,
							HI(shard->y) - Crystal_ShardHeights[cell] / 2,
							Crystal_ShardWidths[cell], Crystal_ShardHeights[cell]);
						EffectStep_AdvanceWithGravity2D(shard, 64, 0x2000);
						shard->variant += shard->z;
						if (shard->z > 1 && (frame & 1))
							shard->z--;
					}
				}
				else if (frame >= cry->variant)
				{
					// 落ちている結晶（32x64）
					draw[i & 1](canvas, work, HI(cry->x) - 16, HI(cry->y), 32, 64);
					EffectStep_AdvanceWithGravity2D(cry, 64, 0x10000);
					if (cry->y > 0x380000)
					{
						// 地面（y = 56）に着いたら砕く
						cry->z = 1;
						cry->y = 0x380000;
						for (j = 0; j != 16; j++)
						{
							shard = &shards[i * 16 + j];
							shard->x = ((Crystal_ShardStarts[j * 2] - 40) << 16) + cry->x;
							shard->y = Crystal_ShardStarts[j * 2 + 1] << 16;
							shard->velocity_x = ((Random16() & 127) - 64) << 11;
							shard->velocity_y = -(Random16() & 127) << 11;
							if (i & 1)
							{
								shard->velocity_x *= 2;
								shard->velocity_y *= 2;
							}
							shard->z = 32;
							shard->variant = 0;
						}
						work->shake_frames = 8;
						AudioCommand_PlayFar(144);
						for (j = 0; j != work->effect->count; j++)
							ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 4);
					}
				}
			}
		}
		Camera_ApplyShake(work->effect->variant * 2 + 4, work->effect->variant * 4 + 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void Graphics_UpdatePhasePalette(s32 frame, s32 red_phase, s32 green_phase, s32 blue_phase)
{
	u16	 palette[64];
	u16 *output;
	s32	 phase;
	s32	 red_offset;
	s32	 green_offset;
	s32	 blue_offset;
	s32	 index;

	phase = frame * 0x400;
	red_offset = (Trig_Sin(phase + red_phase) * 16) >> 15;
	green_offset = (Trig_Sin(phase + green_phase) * 16) >> 15;
	blue_offset = (Trig_Sin(phase + blue_phase) * 16) >> 15;

	palette[0] = 0;
	index = 1;
	output = &palette[1];
	do
	{
		s32 red;
		s32 green;
		s32 blue;

		red = (index + red_offset) / 2;
		green = (index + green_offset) / 2;
		blue = (index + blue_offset) / 2;

		if (red < 0)
			red = 0;
		if (red > 31)
			red = 31;
		if (green < 0)
			green = 0;
		if (green > 31)
			green = 31;
		if (blue < 0)
			blue = 0;
		if (blue > 31)
			blue = 31;

		*output++ = (blue << 10) | (green << 5) | red;
		index++;
	} while (index != 64);

	Iwram_CopyWords((void *)0x05000002, palette, sizeof(palette));
}

//--------------------------------------------------------------------------
// lightning.c
// 戦闘エフェクト：稲妻の柱と飛び散る火花
//--------------------------------------------------------------------------

#define SPARK_MAX 1024 // 火花の数

//--------------------------------------------------------------------------
// 稲妻の柱を 8 フレームおきに落とし、そのたびに地面から火花を散らす
// effect : variant が柱の本数と火花の数を、side が柱の並びを決める
//--------------------------------------------------------------------------
void BattleFx_RunLightningPillars(struct BattleEffectArgument *effect)
{
	BlitterPair				 draw;
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 i;
	u8						*sprites;
	s32						 count;
	s32						 frame;
	s32						 k;
	s32						 start;
	s32						*var;
	struct EffectStep		*spark;
	struct EffectStep		*sparks;

	heap = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	sprites = heap[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_EFFECT];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BLDALPHA = 0x1010;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
	draw[0] = (DrawRectangle)heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
	draw[1] = (DrawRectangle)heap[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
	if (work->effect->variant != 2)
	{
		void *palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteB);
		Iwram_CopyWords((void *)BG_PLTT, palette, 128);
	}

	// 火花をすべて止める
	var = &((struct EffectStep *)gMapCellBuffer)->variant;
	for (k = 0; k != SPARK_MAX; k++)
	{
		*var = 0;
		var += 7;
	}
	sparks = gMapCellBuffer;
	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	count = LightningPillar_Counts[work->effect->variant];

	for (frame = 0; frame != count * 8 - count + 48; frame++)
	{
		// variant 2 : 最初の 64 フレームはカメラを横へ流す
		if (work->effect->variant == 2 && frame <= 63)
		{
			struct CameraWork *camera = (struct CameraWork *)gCameraWork;
			s32				   speed;

			if (frame <= 55)
				speed = 256;
			else
				speed = 704 - frame * 8;
			if (work->effect->side == 1)
				camera->scroll -= speed;
			else
				camera->scroll += speed;
		}
		if (frame == 32)
			BattleEventRuntime_BeginPhaseFar(134);

		//------------------------------------------------------------------
		// 柱 i は 8i フレーム目に落ち、+1 で 1 コマ目、+2〜3 で 2 コマ目を描く
		//------------------------------------------------------------------
		for (i = 0; i != count; i++)
		{
			start = i * 8;
			if (frame == start)
			{
				AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
				Iwram_FillWords(canvas, 0x4000, 0x10101010);
			}
			if (frame >= start && frame < start + 9)
			{
				if (frame >= start + 1 && frame < start + 2)
					draw[0](canvas, work, LightningPillar_Columns[i + work->effect->side * 7] - 24,
						0, 48, 112);
				if (frame >= start + 2 && frame < start + 4)
					draw[0](canvas, work->sheet + 0x1500,
						LightningPillar_Columns[i + work->effect->side * 7] - 24, 0, 48, 112);

				// 柱の根もと (y = 104) から上向き ±90 度へ火花を散らし、画面を揺らす
				if (frame == start + 2)
				{
					s32 n = 0;

					for (k = 0; k != SPARK_MAX; k++)
					{
						spark = &sparks[k];
						if (spark->variant == 0)
						{
							s32 speed;
							s32 angle;

							speed = (Random16() & 0x3ff) + 32;
							angle = (Random16() & 0x7fff) - 0x4000;

							spark->x = LightningPillar_Columns[i + work->effect->side * 7] << 16;
							spark->y = 104 << 16;
							spark->velocity_x = (Trig_Sin(angle) * speed) >> 7;
							spark->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
							spark->variant = (Random16() & 7) + 32;
							n++;
							if (n == LightningPillar_Sparks[work->effect->variant * 2])
								break;
						}
					}
					work->shake_frames = LightningPillar_Sparks[work->effect->variant * 2 + 1];
				}
			}
			if (frame == start + 4)
			{
				for (k = 0; k != work->effect->count; k++)
				{
					BattleMotion_ApplyVariantMotionFar(work->effect->actors[k], 1);
					ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 8);
				}
			}
		}

		// 火花 : 地面で半分の速さで跳ね返り、残りが少ないほど小さく描く
		for (k = 0; k != SPARK_MAX; k++)
		{
			spark = &sparks[k];
			if (spark->variant > 0)
			{
				spark->variant--;
				EffectStep_AdvanceWithGravity2D(spark, 60, 0x1000);
				if (spark->y > 104 << 16)
				{
					spark->velocity_y = -spark->velocity_y / 2;
				}
				else
				{
					s32 x = spark->x >> 16;
					s32 y;
					s32 size;

					if (spark->x >= 0 && x <= 119 && spark->y >= 0)
					{
						y = spark->y >> 16;
						size = spark->variant / 8 + 1;
						draw[k & 1](canvas, sprites + ParticleStreams_CellOffsets[size - 1],
							x - size / 2, y - size, size, size * 2);
					}
				}
			}
		}
		Camera_ApplyShake(8, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// run_two_mode_b_mode0.c
// バトル演出：火花の群れ (sparks.c) の二つの入口
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// kind 0 : キャンバスを優先度 1 で開き、(60, 48) から出す
//--------------------------------------------------------------------------
void BattleFx_RunTwoModeBMode0(s32 effect)
{
	BattleFx_RunSparkGroups(effect, 0);
}

//--------------------------------------------------------------------------
// kind 1 : キャンバスを優先度 0 で開き、(60, 64) から OrangePaletteB で出す
//--------------------------------------------------------------------------
void BattleFx_RunTwoModeBMode1(s32 effect)
{
	BattleFx_RunSparkGroups(effect, 1);
}

void BattleFx_RunSparkGroups(struct BattleEffectArgument *effect, s32 kind)
{
	struct EffectPosition	 pos;
	BlitterPair				 draw;
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 i;
	s32						 frame;
	u8						*graphics;
	s32						 origin_x;
	s32						 origin_y;
	s32						 j;
	s32						 start;
	struct EffectStep		*point;
	struct EffectStep		*spark;
	struct EffectStep		*sparks;

	heap_cache = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	graphics = heap_cache[2];
	work->effect = effect;
	if (kind == 0)
	{
		BattleFx_BeginCanvasLayer(1);
		origin_x = 60;
		origin_y = 48;
	}
	else if (kind == 1)
	{
		BattleFx_BeginCanvasLayer(0);
		origin_x = 60;
		origin_y = 64;
	}
	else
	{
		BattleFx_BeginCanvasLayer(0);
		EffectPosition_ApplyStepAndYOffset(work->effect->actor, &pos);
		origin_x = pos.x / 2;
		origin_y = pos.y + 48;
	}
	REG_BLDALPHA = 0x1010;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
	draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
	if (kind == 1)
	{
		Iwram_CopyWords(
			(void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
	}
	else if (kind == 2)
	{
		Iwram_CopyWords(
			(void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet), 128);
	}
	sparks = gMapCellBuffer;

	for (i = 0; i != SparkGroups_Shapes[work->effect->variant * 5 + 1]; i++)
	{
		for (j = 0; j != 16; j++)
		{
			s32 radius;
			s32 angle;

			point = &work->particles[i * 16 + j];
			radius = j * 2;
			angle = Random16() & 0xffff;
			point->x = Trig_Sin(angle) * radius;
			point->y = -(Trig_Cos(angle) * radius);
			point->variant = j / 2 + 25;
		}
		for (j = 0; j != SparkGroups_Shapes[work->effect->variant * 5]; j++)
		{
			s32 speed;
			s32 angle;

			spark = &sparks[i * SparkGroups_Shapes[work->effect->variant * 5]] + j;
			speed = (Random16() & 0x3ff) + 32;
			angle = Random16() & 0xffff;
			if (work->effect->side == 1)
				spark->x = (origin_x - SparkGroups_Shapes[work->effect->variant * 5 + i + 2] + 28)
						   << 16;
			else
				spark->x = (origin_x + SparkGroups_Shapes[work->effect->variant * 5 + i + 2] - 28)
						   << 16;
			spark->y = origin_y << 16;
			spark->velocity_x = (Trig_Sin(angle) * speed) >> 6;
			spark->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 6;
			spark->variant = (Random16() & 7) + 32;
		}
	}
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != SparkGroups_Shapes[work->effect->variant * 5 + 1] * 8 + 56; frame++)
	{
		struct CameraWork *camera = *(struct CameraWork **)(gWorkSlot + HEAP_SLOT_CAMERA);

		if (work->effect->variant == 2 && frame <= 51)
		{
			if (work->effect->side == 0)
				camera->scroll += 256;
			else
				camera->scroll -= 256;
		}
		/* FAKEMATCH: needed to match: this fill calls the fixed entry itself; through Iwram_FillWords a spilled invariant ahead of the frame loop is built in r2 where the ROM uses r1 (3 of 776 instructions differ). */
		if (work->effect->variant == 3 && frame == 4)
			Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
		if (kind == 1 || kind == 2)
		{
			if (frame == 2)
				BattleEventRuntime_BeginPhaseFar(145);
		}
		else
		{
			if (frame == 2)
				AudioCommand_PlayFar(145);
			if (frame == 24)
				BattleEventRuntime_BeginPhaseFar(134);
		}
		for (i = 0; i != SparkGroups_Shapes[work->effect->variant * 5 + 1]; i++)
		{
			start = i * 8;
			if (frame == start)
				work->shake_frames = 12;
			if (frame >= start && frame < start + 2)
			{
				if (work->effect->side == 1)
					draw[0](canvas, work,
						origin_x - SparkGroups_Shapes[work->effect->variant * 5 + i + 2] + 12,
						origin_y - 32, 32, 64);
				else
					draw[0](canvas, work,
						origin_x + SparkGroups_Shapes[work->effect->variant * 5 + i + 2] - 44,
						origin_y - 32, 32, 64);
			}
			if (frame >= start)
			{
				for (j = 0; j != 12; j++)
				{
					s32 x;
					s32 y;

					point = &work->particles[i * 16] + j;
					y = ((s16 *)&point->y)[1] + origin_y;
					if (work->effect->side == 1)
						x = ((s16 *)&point->x)[1] + origin_x
							- SparkGroups_Shapes[work->effect->variant * 5 + i + 2] + 28;
					else
						x = ((s16 *)&point->x)[1] + origin_x
							+ SparkGroups_Shapes[work->effect->variant * 5 + i + 2] - 28;
					if (point->variant >= 0 && point->variant < 18)
						draw[0](canvas,
							work->sheet + (SparkGroups_FlashCells[point->variant / 3] << 11),
							x - 16, y - 32, 32, 64);
					if (point->variant > 0)
						point->variant--;
					else
						point->variant = -1;
				}
			}
			if (frame > start + 5)
			{
				s32 gravity;

				if (kind == 2)
					gravity = -0x1000;
				else
					gravity = 0x1000;
				for (j = 0; j != SparkGroups_Shapes[work->effect->variant * 5]; j++)
				{
					spark = &sparks[i * SparkGroups_Shapes[work->effect->variant * 5]] + j;
					if (spark->variant > 0)
					{
						EffectStep_AdvanceWithGravity2D(spark, 60, gravity);
						spark->variant--;
						if (spark->y > 0x6c0000)
						{
							spark->velocity_y = -spark->velocity_y / 2;
						}
						else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0)
						{
							s32 y = spark->y >> 16;
							s32 x = spark->x >> 16;
							s32 size = spark->variant / 5 + 1;

							draw[j & 1](canvas, graphics + ParticleStreams_CellOffsets[size - 1],
								x - size / 2, y - size, size, size * 2);
						}
					}
				}
			}
			for (j = 0; j != work->effect->count; j++)
			{
				if (frame == start + 6)
				{
					ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 10);
					BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 4);
				}
			}
		}
		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunFirePillars(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 pillar;
	u8						*sheet;
	struct EffectPosition	 screen;
	BlitterPair				 draw;
	struct MotionObject		*object;
	struct BattleCamera		*camera;
	s32						 i;

	heap_cache = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BLDALPHA = 0x1010;
	Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	if (work->effect->variant == 0)
		Iwram_CopyWords(
			(s32)BG_PLTT, (s32)Resource_GetTableEntry((s32)&ResourceId_RedPaletteB), 128);
	else if (work->effect->variant == 2)
		Iwram_CopyWords(
			(s32)BG_PLTT, (s32)Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
	draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];

	for (i = 0; i != 1024; i++)
		PARTICLES[i].variant = 0;

	object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (i = 0; i != 4; i++)
	{
		struct EffectStep *base = &work->particles[i];
		s32				   x;

		x = ((Random16() & 15) + 72) << 16;
		base->y = 0;
		base->x = x;
		base->z = FirePillars_Depths[work->effect->variant][i] << 16;
		if (object->x < 0)
			base->x = -x;
	}

	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != 96; frame++)
	{
		camera = gCameraWork;
		if (work->effect->variant == 2 && frame < 64)
		{
			if (work->effect->side == 0)
				camera->yaw = camera->yaw + 192;
			else
				camera->yaw = (s16)camera->yaw - 192;
		}
		if (frame == 16)
			BattleEventRuntime_BeginPhaseFar(134);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

		if (frame < 64)
		{
			for (pillar = 0; pillar != FirePillars_Counts[work->effect->variant]; pillar++)
			{
				EffectPosition_ApplyBaseAndYOffset((s32 *)&work->particles[pillar], &screen);
				screen.x /= 2;
				screen.y -= 8;
				if (frame == FirePillars_StartFrames[pillar])
					AudioCommand_PlayFar(145);
				if (frame >= FirePillars_StartFrames[pillar] + 4)
				{
					s32 scroll = (frame * 16 + pillar * 25) % 104;

					draw[pillar & 1](canvas, work, screen.x - 17, screen.y - scroll - 104, 34, 104);
					draw[pillar & 1](canvas, work, screen.x - 17, screen.y - scroll, 34, scroll);
					if (frame & 1)
					{
						draw[0](canvas, work->sheet + 0xdd0, screen.x - 20, screen.y - 24, 20, 37);
						draw[1](canvas, work->sheet + 0xdd0, screen.x, screen.y - 24, 20, 37);
					}
					else
					{
						draw[0](canvas, work->sheet + 0x10b4, screen.x - 20, screen.y - 24, 20, 37);
						draw[1](canvas, work->sheet + 0x10b4, screen.x, screen.y - 24, 20, 37);
					}
				}
				if (frame == FirePillars_StartFrames[pillar]
					|| frame >= FirePillars_StartFrames[pillar] + 16)
				{
					s32 spawned = 0;

					for (i = 0; i != 1024; i++)
					{
						struct EffectStep *spark = &PARTICLES[i];

						if (spark->variant == 0)
						{
							s32 speed;
							s32 angle;

							speed = (Random16() & 0x3ff) + 32;
							angle = (Random16() & 0x7fff) - 0x4000;
							spark->x = screen.x << 8;
							spark->y = (screen.y << 8) + 0x1000;
							spark->z = (Trig_Sin(angle) * speed) >> 15;
							spark->velocity_y = -(Trig_Cos(angle) * speed * 2) >> 15;
							spawned++;
							if (frame == FirePillars_StartFrames[pillar])
							{
								spark->variant = (Random16() & 7) + 48;
								if (spawned == 200)
									break;
							}
							else
							{
								spark->variant = (Random16() & 7) + 24;
								if (spawned == 4)
									break;
							}
						}
					}
				}
				if (frame == FirePillars_StartFrames[pillar])
				{
					work->shake_frames = 2;
					for (i = 0; i != work->effect->count; i++)
					{
						ObjectGroup_UpdateMembers(work->effect->actors[i], 10, 5, i, 8);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 1);
					}
				}
			}
		}

		for (i = 0; i != 1024; i++)
		{
			struct EffectStep *spark = &PARTICLES[i];
			s32				   life = spark->variant;

			if (life > 0)
			{
				s32 x;
				s32 y;
				s32 py;

				spark->variant = life - 1;
				x = spark->x += spark->z;
				y = spark->y += spark->velocity_y;
				spark->z = spark->z * 60 / 64;
				spark->velocity_y = spark->velocity_y * 60 / 64 - 16;
				py = y / 256;
				if (py > 120)
				{
					spark->velocity_y = -spark->velocity_y / 2;
				}
				else if (x >= 0 && x >> 8 <= 126 && y >= 0)
				{
					s32 px = x >> 8;
					s32 size = (life - 17) / 8;

					if (size <= 0)
						size = 1;
					draw[i & 1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
						px - size / 2, py - size, size, size * 2);
				}
			}
		}
		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// render_mode.c
// バトル演出：粒子の演出の入口
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 演出ごとの入口。粒子の型（色と数）を BattleFx_RenderMode に渡す
// effect : 演出の引数
//--------------------------------------------------------------------------
void BattleFx_RenderMode0Or1(struct BattleEffectArgument *effect)
{
	if (effect->variant == 0)
	{
		BattleFx_RenderMode(effect, 0);
		return;
	}
	BattleFx_RenderMode(effect, 1);
}

void BattleFx_RenderMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 1);
}

void BattleFx_RenderMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 2);
}

void BattleFx_RenderMode3(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 3);
}

void BattleFx_RenderMode4(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 4);
}

void BattleFx_RenderMode5(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 5);
}

void BattleFx_RenderMode7(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 7);
}

void BattleFx_RenderMode6(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 6);
}

#define CONVERGES(mode) ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6 || (mode) == 7)
#define DECAYS(mode) ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6)

enum
{
	RENDER_MODE_TORNADO = 0x2580
};

void BattleFx_RenderMode(struct BattleEffectArgument *effect, u32 mode)
{
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 total;
	s32						 member;
	s32						 count;
	struct EffectPosition	 screen;
	s32						 point[3];
	struct EffectPosition	 base;
	BlitterPair				 draw;
	void					*palette;
	s32						*row;
	s32						 i;

	cursor = gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	count = 16;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
	if (mode == 0)
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
	}
	else if (mode == 1)
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
	}
	else if (mode == 2)
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_CyanPalette);
	}
	else if (mode == 3)
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
	}
	else if (mode == 4)
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
	}
	else if (mode == 5)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1, 0);
		palette = Resource_GetTableEntry((s32)&ResourceId_GlowOrbSheet);
	}
	else if (mode == 7)
	{
		count = 24;
		Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1, 0);
		palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
	}
	else
	{
		count = 32;
		Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 0);
		palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
	}
	Iwram_CopyWords((s32)BG_PLTT, (s32)palette, 128);
	if (mode == 4)
		Resource_LoadAndDecompress((s32)&ResourceId_HeartSheet, work->sheet, 1, 1);
	if (mode == 3)
		Resource_LoadAndDecompress(
			(s32)&ResourceId_TornadoSheet, work->sheet + RENDER_MODE_TORNADO, 1, 0);

	for (i = 0; i != 512; i++)
	{
		struct EffectStep *mote = &PARTICLES[i];

		if (CONVERGES(mode))
		{
			mote->x = ((Random16() & 0xff) - 127) << 15;
			mote->y = ((Random16() & 0xff) - 127) << 14;
			mote->z = ((Random16() & 0xff) - 127) << 15;
		}
		else
		{
			mote->x = ((Random16() & 0xff) - 127) << 13;
			mote->y = ((Random16() & 0xff) - 255) << 13;
			mote->z = ((Random16() & 0xff) - 127) << 13;
		}
		mote->variant = 0;
	}

	if (CONVERGES(mode))
		total = work->effect->count * 8 + 64;
	else
		total = work->effect->count * 8 + 32;
	if (!(mode <= 1 || mode == 3))
		Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(142);

	for (frame = 0; frame != total; frame++)
	{
		struct BattleCamera *camera = gCameraWork;

		if (mode == 7)
		{
			if (frame == total - 46)
				BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0], 16, 0);
			if (frame == total - 32)
			{
				BattleEventRuntime_BeginPhaseFar(134);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
				work->shake_frames = 8;
			}
		}
		else if (frame == total - 32)
		{
			BattleEventRuntime_BeginPhaseFar(133);
		}
		row = work->bg2_x;
		for (i = 0; i != 160; i++)
			*row++ = (0x40000 - (Trig_Sin((frame << 12) + i * 0x800) << 2)) >> 10;

		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *object =
				GetBattleObjectSlotFar(work->effect->actors[member])->object;

			if (mode == 3 && frame > member * 8 && frame < member * 8 + 32)
			{
				EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &base);
				for (i = 0; i != 2; i++)
				{
					s32 angle;
					s32 radius;
					s32 x;
					s32 y;

					angle = Random16() & 0xffff;
					radius = (Random16() & 31) + 4;
					x = base.x / 2 + ((Trig_Sin(angle) * radius) >> 17)
						- (BattleFx_GlintCellWidths[frame & 3] >> 1);
					y = base.y - ((Trig_Cos(angle) * radius) >> 16)
						- (BattleFx_GlintCellHeights[frame & 3] >> 1);
					BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
						3 | RenderMode_GlintFlips[Random16() & 3], 2);
					((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
						work->sheet + BattleFx_GlintCellOffsets[frame & 3] + RENDER_MODE_TORNADO, x,
						y + 16, BattleFx_GlintCellWidths[frame & 3],
						BattleFx_GlintCellHeights[frame & 3]);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				}
			}
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
			draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
			draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			point[0] = object->x;
			point[1] = 160 << 13;
			point[2] = object->z;
			SceneTransform_ApplyPosition(point);
			if (frame > member * 8)
			{
				SceneTransform_ApplyYaw(frame << 9);
				if (mode <= 1 || mode == 4)
					SceneTransform_ApplyPitch(frame << 9);
				for (i = 0; i != count; i++)
				{
					struct EffectStep *mote = &PARTICLES[member * 64 + i];
					s32				   distance;

					if (frame > member * 8 + i)
					{
						s32 xx = (mote->x >> 8) * (mote->x >> 8);
						s32 yy = (mote->y >> 8) * (mote->y >> 8);
						s32 zz = (mote->z >> 8) * (mote->z >> 8);

						distance = Iwram_Sqrt(xx + yy + zz) >> 9;
						if (distance != 0 && mote->variant <= 23)
						{
							s32 cel = mote->variant / 4;

							EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &screen);
							screen.x >>= 1;
							if (mode == 5 || mode == 7)
								draw[1](canvas, work->sheet + cel * 1600, screen.x - 20,
									screen.y - 20, 40, 40);
							else if (mode == 6)
								draw[1](canvas, work->sheet + 0xc00, screen.x - 6, screen.y - 12,
									12, 24);
							else if (mode == 4)
								draw[1](canvas, work->sheet, screen.x - 11, screen.y - 21, 22, 42);
							else
								draw[(i & 3) != 0](canvas, work->sheet + cel * 1152, screen.x - 12,
									screen.y - 24, 24, 48);
							if (DECAYS(mode))
							{
								mote->x -= mote->x / distance;
								mote->y -= mote->y / distance;
								mote->z -= mote->z / distance;
							}
							else
							{
								mote->y += 0x10000;
							}
							mote->variant++;
							if (mote->variant == 24)
							{
								if (DECAYS(mode))
								{
									mote->variant = 0;
								}
								else
								{
									mote->x = ((Random16() & 0xff) - 127) << 13;
									mote->y = ((Random16() & 0xff) - 255) << 12;
									mote->z = ((Random16() & 0xff) - 127) << 13;
								}
							}
						}
					}
				}
			}
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			if (frame == member * 8 + 16)
			{
				s32 delay = total - frame;

				if (delay > 31)
					delay = 31;
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, delay);
			}
		}
		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	if (!(mode <= 1 || mode == 3))
		Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunBouncingSheep(void *object)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 record[3];

	struct EffectStep *one;
	struct EffectStep *sheep;
	s32				   i;
	s32				   frame;
	DrawRectangle	   draw_b;
	DrawRectangle	   draw_a;
	s32				   member;

	sheep = PARTICLES;
	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = object;
	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100;
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_SheepSheet, work->sheet, 1, 1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 1);
	draw_a = (DrawRectangle)heap_cache[7];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 15, 1);
	draw_b = (DrawRectangle)heap_cache[8];
	for (i = 0; i != 32; i++)
	{
		one = &sheep[i];
		one->x = ((Random16() & 63) + 32) << 16;
		one->y = -0x200000;
		one->velocity_y = Random16() & 0;
		one->z = Random16() & 3;
		one->variant = Random16() & 255;
	}
	if (work->effect->side == 1)
		REG_BG2X = -0x7000;
	work->transfer_mode = 1;
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(142);
	for (frame = 0; frame != 148; frame++)
	{
		struct BattleCamera *camera;

		camera = gCameraWork;
		if (frame == 80)
			BattleEventRuntime_BeginPhaseFar(0);
		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *member_object;

			member_object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			record[0] = member_object->x;
			record[1] = 160 << 14;
			record[2] = member_object->z;
			SceneTransform_ApplyPosition(record);
			if (frame == member * 16 + 64)
				ObjectGroup_UpdateMembers(work->effect->actors[member], 0, 5, -1, 0);
		}
		one = sheep;
		for (i = 0; i != 12; i++, one++)
		{
			if (frame > i * 4 && one->y <= 0x7fffff)
			{
				s32 cel;

				cel = (one->variant / 16) & 7;
				if (cel < 4)
					draw_a(canvas, work->sheet + (cel << 10), (one->x >> 16) - 16,
						(one->y >> 16) - 16, 32, 32);
				else
					draw_b(canvas, work->sheet + (cel << 10) - 0x1000, (one->x >> 16) - 16,
						(one->y >> 16) - 16, 32, 32);
				one->y += one->velocity_y;
				one->velocity_y += 0x2000;
				one->variant += one->z;
				if (one->y > 0x5c0000 && one->velocity_y == 0)
				{
					s32 speed = one->velocity_y + 1;
					one->y = 0x5c0000;
					one->z += 4;
					one->velocity_y = -speed / 2;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunTargetMarkers(struct BattleEffectArgument *efx)
{
	void				   **slot;
	struct BattleEffectWork *work;
	void					*canvas;
	/* FAKEMATCH: needed to match: the family's blitter pair: this effect loads only one. A single blitter, as draw[1] or a plain pointer: differ 202. */
	BlitterPair				 draw;
	struct EffectStep		*marker;
	s32						 i;
	s32						 frame;
	s32						 start;
	s32						 cel;
	s32						 y;
	struct EffectPosition	 pos;

	slot = &gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	canvas = slot[0];
	work = slot[-1];
	work->effect = efx;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100;
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_FlameColumnSheet, work->sheet, 1, 1);
	if (work->effect->side == 1)
	{
		REG_BG2X = -0x7000;
	}
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 1);
	work->transfer_mode = 1;
	work->transfer_value = 0;
	draw[0] = (DrawRectangle)slot[6];
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	for (i = 0; i != work->effect->count; i++)
	{
		work->particles[i].variant = Random16() & 63;
	}
	frame = 0;
	while (frame != (work->effect->count << 5) + 32)
	{
		if (frame == 32)
		{
			BattleEventRuntime_BeginPhaseFar(0);
		}
		i = 0;
		if (work->effect->count != 0)
		{
			marker = work->particles;
			do
			{
				start = i << 4;
				if (frame == start)
				{
					AudioCommand_PlayFar(143);
				}
				if (frame >= start && frame < start + 72)
				{
					EffectPosition_ApplyStepAndYOffset(work->effect->actors[i], &pos);
					if (work->effect->side == 1)
					{
						pos.x -= 112;
					}
					y = pos.y;
					pos.y = y - 16;
					draw[0](canvas, work->sheet + 1728, pos.x - 8, y - 20, 16, 20);
					if (frame >= start)
					{
						cel = ((frame - start) + marker->variant) / 6 % 9;
						draw[0](canvas, work->sheet + cel * 192, pos.x - 8, pos.y - 16, 16, 12);
					}
				}
				marker++;
				i++;
			} while (i != work->effect->count);
		}
		work->transfer_pending = 1;
		WaitFrames(1);
		frame += 1;
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunSparkTravel(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 member;
	u8						*sheet;
	DrawRectangle			*draw;
	struct MotionObject		*actor;
	struct MotionObject		*goal;
	s32						 frame;
	s32						 i;
	s32						 position[3];
	s32						 target[3];
	s32						 delta[3];
	s32						 rider[3];
	struct EffectPosition	 screen;
	s32						 point[3];
	struct EffectPosition	 view;
	BlitterPair				 callbacks;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	BattleFx_FetchRectangleBlitters(work->effect->side ^ 1, draw = callbacks);
	actor = GetBattleObjectSlotFar(work->effect->actor)->object;
	goal = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *spark = &PARTICLES[i];
		s32				   heading = Random16() & 0xffff;
		s32				   speed = (Random16() & 0xff) + 128;

		spark->x = 0;
		spark->y = ((Random16() & 31) + 20) << 16;
		spark->z = 0;
		spark->velocity_x = (Trig_Sin(heading) * speed) >> 5;
		spark->velocity_y = 0;
		spark->velocity_z = (Trig_Cos(heading) * speed) >> 5;
		spark->variant = 0;
	}
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	position[0] = actor->x;
	position[1] = 0;
	position[2] = actor->z;
	target[0] = goal->x;
	target[1] = 180 << 15;
	target[2] = 0;
	delta[0] = (target[0] - position[0]) / 40;
	delta[1] = (target[1] - position[1]) / 40;
	delta[2] = (target[2] - position[2]) / 40;
	for (frame = 0; frame != 98; frame++)
	{
		struct BattleCamera *camera = gCameraWork;
		if (frame == 8)
			AudioCommand_PlayFar(212);
		if (frame == 80)
			AudioCommand_PlayFar(142);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame >= 30 && frame < 70)
		{
			position[0] += delta[0];
			position[1] += delta[1];
			position[2] += delta[2];
		}
		SceneTransform_ApplyPosition(position);
		if (frame == 0)
			ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
		if (frame == 24)
			ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
		for (i = 0; i != 32; i++)
		{
			struct EffectStep *spark = &PARTICLES[i];
			if (frame > i && spark->variant == 0)
			{
				s32 size;
				Graphics_SaveTransferWorkOnce();
				switch (i & 3)
				{
				case 0:
					SceneTransform_ApplyYaw(frame * (i * 32 + 256));
					break;
				case 1:
					SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
					break;
				case 2:
					SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
					break;
				case 3:
					SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
					SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
					break;
				}
				EffectPosition_ApplyBaseAndYOffset((s32 *)spark, &screen);
				screen.x >>= 1;
				Graphics_RestoreTransferWork();
				if (screen.depth < 250)
					screen.depth = 250;
				if (screen.depth > 634)
					screen.depth = 634;
				size = 9 - (screen.depth - 250) / 64;
				callbacks[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					screen.x - size / 2, screen.y - size, size, size * 2);
				EffectStep_AdvanceWithGravity3D(spark, 60, 0);
				if (frame > i + 30)
				{
					s32 dx = -spark->x >> 8;
					s32 dy = -spark->y >> 8;
					s32 dz = -spark->z >> 8;

					spark->velocity_x += dx;
					spark->velocity_y += dy;
					spark->velocity_z += dz;
				}
			}
		}
		if (frame > 82)
		{
			rider[0] = 0;
			rider[1] = Trig_Sin(frame << 10) << 2;
			rider[2] = 0;
			EffectPosition_ApplyBaseAndYOffset(rider, &screen);
			screen.x >>= 1;
			draw[1](canvas, work, screen.x - 10, screen.y - 17, 20, 34);
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	for (i = 0; i != 512; i++)
	{
		struct EffectStep *spark = &PARTICLES[i];
		spark->x = ((Random16() & 0xff) - 127) << 15;
		spark->y = ((Random16() & 0x7f) + 64) << 15;
		spark->z = ((Random16() & 0xff) - 127) << 15;
		spark->variant = 0;
	}
	Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sheet, 0, 0);
	for (frame = 0; frame != work->effect->count * 8 + 72; frame++)
	{
		struct BattleCamera *camera = gCameraWork;
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame >= work->effect->count * 8 + 40)
			target[1] += 0x40000;
		point[0] = target[0];
		point[1] = target[1];
		point[2] = target[2] + Trig_Sin(frame << 11) * 40;
		EffectPosition_ApplyBaseAndYOffset(point, &view);
		view.x >>= 1;
		callbacks[0](canvas, work, view.x - 10, view.y - 17, 20, 34);
		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *object =
				GetBattleObjectSlotFar(work->effect->actors[member])->object;
			s32 start = member * 8;
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			position[0] = object->x;
			position[1] = 160 << 14;
			position[2] = object->z;
			SceneTransform_ApplyPosition(position);
			if (frame == start + 30)
				AudioCommand_PlayFar(126);
			if (frame == start + 40)
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, -1, -1, 0);
			if (frame == start + 64)
				ObjectGroup_UpdateMembers(work->effect->actors[member], 0, -1, -1, 0);
			if (frame > start)
			{
				SceneTransform_ApplyYaw((frame - start) << 9);
				for (i = 0; i != 64; i++)
				{
					struct EffectStep *spark = &PARTICLES[member * 64 + i];

					if (frame > start + i / 2)
					{
						s32 xx = (spark->x >> 8) * (spark->x >> 8);
						s32 yy = (spark->y >> 8) * (spark->y >> 8);
						s32 zz = (spark->z >> 8) * (spark->z >> 8);
						s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;
						if (distance != 0)
						{
							s32 cel;

							EffectPosition_ApplyBaseAndYOffset((s32 *)spark, &view);
							view.x >>= 1;
							if (view.depth < 314)
								view.depth = 314;
							if (view.depth > 634)
								view.depth = 634;
							/* FAKEMATCH: needed to match: the size series A draws its sparks at, kept from that effect and never used here. Without the dead store: differ 479. */
							cel = 3 - (view.depth - 314) / 128;
							cel = (i * 4 + frame) % 9;
							{
								u8 *cell = sheet + BattleFx_PuffCells[cel];
								u32 edge = BattleFx_PuffSizes[cel];
								u32 half = edge >> 1;

								draw[1](canvas, cell, view.x - half, view.y - half, edge, edge);
							}
							spark->x -= spark->x / distance;
							spark->y -= spark->y / distance;
							spark->z -= spark->z / distance;
						}
					}
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void Camera_ApplyPhasedDelta(void)
{
	struct BattleEffectWork *work = gBattleFxWork[0];
	struct BattleCamera		*camera = *(struct BattleCamera **)((u8 *)gBattleFxWork - 108);
	s32						*phase = &work->camera_phase;

	if (*phase == 1)
	{
		camera->yaw += work->camera_delta;
		*phase = 0;
	}
	else
	{
		camera->yaw += work->camera_delta / 2;
		if (*phase == 2)
			*phase = 0;
		else
			*phase = 2;
	}
}

//--------------------------------------------------------------------------
// run_no_effect_frames.c
// バトル演出：空の演出の繰り返し
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 何もしない BattleFx_RunNoEffect を cnt 回呼ぶ（フレーム待ちはしない）
// cnt : 呼ぶ回数
//--------------------------------------------------------------------------
void BattleFx_RunNoEffectFrames(s32 cnt)
{
	s32 n;

	n = 0;
	if (cnt != 0)
	{
		do
		{
			n += 1;
			BattleFx_RunNoEffect();
		} while (n != cnt);
	}
}

//--------------------------------------------------------------------------
// dispatch.c
// バトル演出：演出の振り分け・走査線ごとのウインドウ端・生きている対象
//--------------------------------------------------------------------------

typedef void (*Callback)(s32 *);
extern Callback BattleFx_ModeHandlers[];

//--------------------------------------------------------------------------
// 演出の作業領域を取り、種類（arg[0]）ごとの演出を流して返す
// arg : struct BattleEffectArgument
//--------------------------------------------------------------------------
void BattleFx_InitializeMode(s32 *arg)
{
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x302);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BattleEffectWork));
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);

	if (*arg == 0)
	{
		BattleFx_InitializeDefaultMode((struct BattleEffectArgument *)arg);
	}
	else
	{
		switch (*arg)
		{
		case 1:
			BattleEffect_RunCirclingFallingScene(arg);
			break;
		case 2:
			BattleEffect_RunEmberColumns(arg);
			break;
		case 3:
			BattleFx_RunObjectRow(arg);
			break;
		case 4:
			BattleEffect_RunStagedParticles(arg);
			break;
		case 5:
			BattleEffect_RunDitherDissolveScene((struct BattleEffectArgument *)arg);
			break;
		case 6:
			BattleFx_InitializeMode6(arg);
			break;
		case 7:
			BattleFx_RenderAnimationMode0(arg);
			break;
		case 8:
			BattleFx_RunWhirlwindComets((struct BattleEffectArgument *)arg);
			break;
		case 9:
			BattleEffect_RunShockwaveScene((struct BattleEffectArgument *)arg);
			break;
		case 10:
			BattleFx_InitializeMode10(arg);
			break;
		case 11:
			BattleFx_InitializeDefaultMode((struct BattleEffectArgument *)arg);
			break;
		case 12:
			BattleFx_InitializeMode12(arg);
			break;
		}
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}

//--------------------------------------------------------------------------
// 演出の作業領域を取り、BattleFx_ModeHandlers の表から演出を流す
// 種類 0 は演出なしで、variant（arg[6]）を 0 にする
// arg : struct BattleEffectArgument
//--------------------------------------------------------------------------
void BattleFx_DispatchMode(s32 *arg)
{
	s32						 kind;
	struct BattleEffectWork *work;

	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x302);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BattleEffectWork));
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);

	work = gBattleFxWork[0];
	work->effect = (struct BattleEffectArgument *)arg;
	kind = arg[0];
	if (kind == 0)
		arg[6] = 0;
	else
		BattleFx_ModeHandlers[kind - 1](arg);

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}

//--------------------------------------------------------------------------
// ウインドウ 0 の右端を走査線ごとに決め、HBlank DMA で毎ライン書き込む
// （スケジューラのコールバック）
// gMapCellBuffer +0   : 基準の右端（u16）
// gMapCellBuffer +2   : 8〜135 ライン目のずれ（u8 × 128）
// gMapCellBuffer +0x82: 作った WIN0H の表（u16 × 160）
// 表の外のラインは 0xfff1（左端 > 右端）で、ウインドウに入らない
//--------------------------------------------------------------------------
void BattleFx_BuildWindowEdgeTable(void)
{
	u16 *base;
	u16 *tbl;
	u8	*ofs;
	s32	 i;
	s32	 x;

	base = (u16 *)gMapCellBuffer;
	tbl = (u16 *)(Ram_MapCellBuffer + 0x82);
	ofs = Ram_MapCellBuffer + 2;
	for (i = 0; i != 160; i++)
	{
		if (i >= 8 && i <= 135)
		{
			x = *base - ofs[i - 8];
			if (x < 0)
				x = 0;
			if (x > 240)
				x = 240;
			tbl[i] = x;
		}
		else
		{
			tbl[i] = 0xfff1;
		}
	}
	{
		volatile u16 *channel = REG_DMA0;

		// DMA0 のリピートと開始条件を消してから止める
		channel[5] &= 0xc5ff;
		channel[5] &= 0x7fff;
		(void)channel[5];
		// HBlank ごと・リピート・転送先リロードで 1 ハーフワード
		Dma_Set(
			(u8 *)gMapCellBuffer + 0x82, (void *)&REG_WIN0H, 0xa2600001, (volatile u32 *)channel);
	}
}

//--------------------------------------------------------------------------
// 対象の側（敵か味方か）で HP の残っている者を集め、オブジェクトを出す
// 敵の番号は 128 から
//--------------------------------------------------------------------------
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *arg)
{
	s16 list[14];
	s32 cnt;
	s32 i;

	cnt = 0;
	if (arg->actors[0] > 127)
	{
		for (i = 0; i != 6; i++)
		{
			s32 id = i + 128;

			if (UnitGetFar(id)->hp > 0)
				list[cnt++] = id;
		}
	}
	else
	{
		for (i = 0; i != 8; i++)
		{
			if (UnitGetFar(i)->hp > 0)
				list[cnt++] = i;
		}
	}
	list[cnt] = 0xff; // 終わり
	BattleActor_SpawnObjectsForListFar(list, 0);
}

void BattleEffect_SetupBlendedDisplay(void)
{
	void		  **slots = &gWorkSlot[HEAP_SLOT_BATTLE];
	u8			   *buffer = slots[HEAP_SLOT_BATTLE_CANVAS - HEAP_SLOT_BATTLE];
	struct BtlWork *battle = slots[0];
	volatile u32	zero;

	REG_DISPCNT = 1;
	gBgScroll[1].y = 32;
	BattleBackground_LoadFar(1, battle->background, 24);
	zero = 0;
	Dma_Set((const void *)&zero, buffer, 0x85001000, REG_DMA3);
	zero = 0;
	Dma_Set((const void *)&zero, (void *)0x06004000, 0x85001000, REG_DMA3);
	WaitFrames(1);
	REG_BLDALPHA = 0x100e;
	REG_BLDCNT = 0x3f46;
	REG_DISPCNT = 0x7741;
	gProjection.center_y = 120;
}

void ObjectGroup_UpdateMembers(
	s32 set_id, s32 object_value, s32 group_value, s32 state_slot, s32 state_value)
{
	struct BtlObj			*set;
	struct AnimationObject	*group;
	struct BattleEffectWork *state;
	s32						 group_index;

	set = GetBattleObjectSlotFar(set_id);
	state = gBattleFxWork[0];
	group_index = 0;

	while ((group = GetMotionRecordFar(set->object, group_index)) != NULL)
	{
		if (state_slot != -1)
		{
			state->actor_timers[state_slot] = ((u8 *)&state_value)[0];
		}
		if (set->fading == 0)
		{
			if (object_value != -1)
			{
				s32 object_index;

				object_index = 0;
				if (group->count != 0)
				{
					struct AnimationEntry **objects;

					objects = group->entries;
					do
					{
						struct AnimationEntry *object;

						object = *objects++;
						if (object != NULL && object != (struct AnimationEntry *)set->effect_entry
							&& object != (struct AnimationEntry *)set->animation_entry)
						{
							if (object_value == 0)
								object->param = BattleMotion_GetSlotPaletteFar(set_id);
							else
								object->param = object_value;
							object->frame = 0xff;
						}
						object_index++;
					} while (object_index != group->count);
				}
			}
			if (group_value != -1)
				AnimationObjects_SelectAnimationFar(group, group_value);
		}
		group_index++;
	}
}

void BattleFx_InitTilemapAndFlushQueue(void)
{
	BattleFx_BeginTiledCanvas(1);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// initialize_mode10.c
// 戦闘エフェクト：いばらのつる、岩と氷塊、氷の破片
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// いばらのつる : とげを飛ばし、つるを伸ばして対象を持ち上げ、
// 絵を点ごとに消してから対象を落として爆発させる
// effect : 対象の一覧
//--------------------------------------------------------------------------
void BattleEffect_RunDitherDissolveScene(struct BattleEffectArgument *effect)
{
	void				   **cursor;
	struct BattleEffectWork *work;
	u8						*canvas;
	s32						 frame;
	s32						 height;
	s32						 speed;
	s32						 phase;
	s32						 rise;
	s32						 i;
	s32						 j;
	u8						*order;
	s32						 point[3];
	struct EffectPosition	 screen;
	BlitterPair				 draw;

	cursor = gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_SpawnObjects(8, 0x177, 1);

	// 消す順番 : 8 組の 0〜127 をでたらめに並べかえる
	order = Ram_MapCellBuffer;
	for (i = 0; i != 1024; i++)
		order[i] = i & 127;
	for (i = 0; i != 8; i++)
	{
		for (j = 0; j != 128; j++)
		{
			s32 a = Random16() & 127;
			s32 b = Random16() & 127;
			u8 *to = &order[i * 128 + b];
			u8 *from = &order[i * 128 + a];
			u8	t = *to;

			*to = *from;
			*from = t;
		}
	}

	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100;
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_ThornVineSheet, work->sheet, 1, 1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 1);
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 1);
	draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	work->transfer_mode = 1;
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	REG_BG2X = -0x2000; // 左へ 32 ドット (20.8 固定小数)

	// つる (0〜15) : x、根もとの y、伸びる速さ、伸びはじめるまでの待ち
	for (i = 0; i != 16; i++)
	{
		struct EffectStep *vine = &work->particles[i];

		if (i <= 4)
		{
			vine->x = i * 20;
			vine->y = (Random16() & 7) + 104;
		}
		else
		{
			vine->x = i * 20 - 90;
			vine->y = (Random16() & 7) + 108;
		}
		vine->velocity_y = (Random16() & 7) + 4;
		vine->variant = (Random16() & 15) + 16;
	}
	// 光 (16〜31) : 負の variant は出るまでの待ち
	for (i = 0; i != 16; i++)
	{
		struct EffectStep *spark = &work->particles[16 + i];

		spark->x = (Random16() & 63) + 32;
		spark->y = (Random16() & 31) + 64;
		spark->variant = -(Random16() & 7) - 8;
	}
	// とげ (32〜47) : 右上から左へ飛ぶ
	for (i = 0; i != 16; i++)
	{
		struct EffectStep *thorn = &work->particles[32 + i];

		thorn->x = 128 << 16;
		thorn->y = 64 << 16;
		thorn->velocity_x = -((Random16() & 255) + 200) << 9;
		thorn->velocity_y = 0;
		thorn->variant = 0;
	}
	BattleFx_SelectLivingTargets(work->effect);

	height = -(64 << 16);
	speed = 0;
	for (frame = 0; frame != 366; frame++)
	{
		struct BattleCamera *camera = gCameraWork;

		// A・B で、つるの場面を落とす場面まで飛ばす
		if ((gKeysRepeat & (KEY_A | KEY_B)) != 0 && frame > 190 && frame <= 285)
		{
			Iwram_ClearWords(canvas, 0x4000);
			frame = 286;
		}
		if (frame == 224)
			work->transfer_mode = 0;
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame == 31)
		{
			work->shake_frames = 8;
			AudioCommand_PlayFar(157);
			for (i = 0; i != work->effect->count; i++)
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 6);
		}
		if (frame == 72)
			AudioCommand_PlayFar(136);
		if (frame == 140)
			AudioCommand_PlayFar(156);

		// 陣形のオブジェクトを 64 ドットの高さまで落とす
		speed += 0x4000;
		height += speed;
		if (height > 64 << 16)
			height = 64 << 16;
		BattleFx_PlaceFormationObjects(2, 128 << 16, height);
		if (frame >= 48 && frame <= 96)
		{
			s32 pose = (frame - 48) / 24 % 3;

			AnimationObjects_SelectAnimationFar(work->objects[3], ThornVines_ObjectPoses[pose * 2]);
			AnimationObjects_SelectAnimationFar(
				work->objects[4], ThornVines_ObjectPoses[pose * 2 + 1]);
		}

		// とげを 1 フレームに 1 本ずつ飛ばす
		if (frame >= 72 && frame <= 127)
		{
			for (i = 0; i != 16; i++)
			{
				struct EffectStep *thorn = &work->particles[32 + i];

				if (frame >= i + 72 && thorn->y < (104 << 16))
				{
					s32 cel = (frame + i) / 4 % 5;
					s32 x = HI(thorn->x);
					s32 y = thorn->y >> 16;
					u8 *cell = work->sheet + ThornVines_ThornOffsets[cel];
					u32 width = ThornVines_ThornWidths[cel];
					u32 tall;

					x -= width >> 1;
					tall = ThornVines_ThornHeights[cel];
					y -= tall >> 1;
					draw[0](canvas, cell, x, y, width, tall);
					EffectStep_AdvanceWithGravity2D(thorn, 64, 0x1000);
				}
			}
		}
		// 破片 (16〜63) を地面から跳ねあげる
		if (frame == 128)
		{
			for (i = 0; i != 48; i++)
			{
				struct EffectStep *chip = &work->particles[16 + i];

				chip->x = (Random16() % 96) << 16;
				chip->y = ((Random16() & 7) + 88) << 16;
				chip->velocity_x = ((Random16() & 255) - 128) << 11;
				chip->velocity_y = -(Random16() & 255) << 11;
				chip->variant = -(Random16() & 15) - 16;
			}
			REG_BG2X = 0;
		}

		// つるを伸ばす (伸びているあいだは画面を揺らす)
		phase = frame - 128;
		if (phase >= 0 && phase <= 96)
		{
			rise = phase;
			if (rise > 80)
				rise = 80;
			else
				work->shake_frames = 2;
			for (i = 0; i != 10; i++)
			{
				struct EffectStep *vine = &work->particles[i];

				if (rise > vine->variant)
				{
					u8 *sheet = work->sheet;
					s32 top;
					s32 wrap;
					s32 k;

					if (i > 5)
						sheet = work->sheet + 0x6c0;
					top = (rise - vine->variant) * vine->velocity_y;
					for (wrap = top; wrap > 184; wrap -= 64)
						;
					// 先端 (24×8)
					if (wrap <= 119)
						draw[i & 1](canvas, sheet, vine->x, vine->y - wrap - 8, 24, 8);
					// 茎 (24×64 を 3 枚、画面の上と根もとで切る)
					for (k = 0; k != 3; k++)
					{
						s32 y = vine->y - wrap + k * 64;
						s32 skip = 0;
						s32 rows = 64;

						if (y >= -64)
						{
							if (y < 0)
							{
								skip = -y * 24;
								rows = y + 64;
								y = 0;
							}
							if (y + rows > vine->y)
								rows -= y + rows - vine->y;
							draw[i & 1](canvas, sheet + skip + 192, vine->x, y, 24, rows);
						}
					}
					// 葉 (奇数のつるだけ)
					if ((i & 1) != 0)
					{
						s32 y = ((vine->y - top) & 127) - 16;
						s32 kind = i % 3;
						s32 full = ThornVines_LeafSizes[kind];
						s32 rows = full;

						if (y + rows > vine->y)
							rows -= y + rows - vine->y;
						if (rows > 0)
							draw[i & 1](canvas, work->sheet + ThornVines_LeafOffsets[kind],
								vine->x + 8, y, full, rows);
					}
				}
			}
		}
		// 破片 : 落ちるか画面の外へ出たら、フレーム 159 まで跳ねあげなおす
		if (phase >= 0 && phase <= 95)
		{
			for (i = 0; i != 32; i++)
			{
				struct EffectStep *chip = &work->particles[32 + i];

				if (chip->variant >= 0)
				{
					s32 cel = i % 5;

					draw[0](canvas, work->sheet + ThornVines_ChipOffsets[cel], HI(chip->x),
						HI(chip->y), ThornVines_ChipWidths[cel], ThornVines_ChipHeights[cel]);
					chip->x += chip->velocity_x;
					chip->y += chip->velocity_y;
					chip->velocity_y += 0x4000;
					if ((chip->y < 0 || chip->y > 120 << 16) && frame <= 159)
					{
						chip->x = (Random16() % 96) << 16;
						chip->y = ((Random16() & 7) + 88) << 16;
						chip->velocity_x = ((Random16() & 255) - 128) << 11;
						chip->velocity_y = -(Random16() & 255) << 11;
					}
				}
				chip->variant++;
			}
		}

		// BG パレットの 64 色を 4 フレームごとに灰色へ 1 段ずつ近づける
		if (frame >= 224 && frame <= 247 && (frame & 3) == 0)
		{
			u16 *color = (u16 *)BG_PLTT;

			for (i = 0; i != 64; i++)
			{
				s32 red = *color & 31;
				s32 green = (*color >> 5) & 31;
				s32 blue = (*color >> 10) & 31;
				s32 grey = (red + green + blue) / 3;

				if (red > grey)
					red--;
				if (red < grey)
					red++;
				if (green > grey)
					green--;
				if (green < grey)
					green++;
				if (blue > grey)
					blue--;
				if (blue < grey)
					blue++;
				*color++ = (blue << 10) | (green << 5) | red;
			}
		}

		// 対象ごとに 6 つの光を、体のまわりでくり返し光らせる
		if (phase >= 0 && phase <= 172)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				struct MotionObject *object =
					GetBattleObjectSlotFar(work->effect->actors[i])->object;

				point[0] = object->x;
				point[1] = object->y;
				point[2] = object->z;
				EffectPosition_ApplyBaseAndYOffset(point, &screen);
				for (j = 0; j != 6; j++)
				{
					struct EffectStep *spark = &work->particles[16 + i * 6 + j];

					if (spark->variant == 0)
					{
						spark->x = screen.x + (Random16() & 15) - 8;
						spark->y = screen.y + (Random16() & 15) - 40;
					}
					if (spark->variant >= 0 && spark->variant <= 4)
					{
						u8 *cell = work->sheet + ThornVines_SparkOffsets[spark->variant];
						u32 size = ThornVines_SparkSizes[spark->variant];
						u32 half = size >> 1;

						draw[0](canvas, cell, spark->x - half, spark->y - half, size, size);
					}
					spark->variant++;
					if (frame <= 199 && spark->variant == 5)
						spark->variant = -(Random16() & 7);
				}
			}
		}

		// 128×128 の絵を、上から 2 行ずつ順番どおりの点で消していく
		if (frame > 232)
		{
			s32 row = frame * 2 - 496;

			for (i = 0; i != 32; i++)
			{
				for (j = 0; j != 4; j++)
				{
					s32 y = row + i;

					if (y >= 0 && y <= 127)
					{
						s32 x = order[((y & 7) * 32 + i) * 4 + j];

						canvas[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = 0;
					}
				}
			}
			row++;
			for (i = 0; i != 32; i++)
			{
				for (j = 0; j != 4; j++)
				{
					s32 y = row + i;

					if (y >= 0 && y <= 127)
					{
						s32 x = order[((y & 7) * 32 + i) * 4 + j];

						canvas[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = 0;
					}
				}
			}
		}

		// 対象を 8 フレームずつずらして 128 ドットの高さまで持ち上げる
		if (frame >= 161 && frame <= 223)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame > i * 8 + 160)
				{
					struct BtlObj		*slot = GetBattleObjectSlotFar(work->effect->actors[i]);
					struct MotionObject *object = slot->object;
					void				*record;
					s32					 n;

					object->y += 8 << 16;
					if (object->y > 128 << 16)
						object->y = 128 << 16;
					object->vertical_motion_strength = 0;
					for (n = 0; (record = GetMotionRecordFar(slot->object, n)) != 0; n++)
						AnimationObjects_SelectAnimationFar(record, 5);
				}
			}
		}
		// 5 フレームずつずらして 96 ドットから落とし、16 フレーム後にぶつける
		for (i = 0; i != work->effect->count; i++)
		{
			if (frame == i * 5 + 286)
			{
				struct MotionObject *object =
					GetBattleObjectSlotFar(work->effect->actors[i])->object;

				object->y = 96 << 16;
				object->vertical_motion_strength = 0xab85;
			}
			if (frame == i * 5 + 302)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 7, -1, i, 8);
				AudioCommand_PlayFar(134);
				work->shake_frames = 8;
			}
		}

		// 爆発の絵に切りかえ、対象ごとに火の粉を 72 度おきに 10 個出す
		if (frame == 302)
		{
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			Resource_LoadAndDecompress((s32)&ResourceId_YellowRingSheet, work->sheet, 1, 0);
			Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet + 0x1680, 1, 1);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
			draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
			draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
			REG_BLDCNT = 0x3f46;
			REG_BG2PA = 0x80;
			REG_BG2X = 0;
			work->transfer_mode = 2;
			work->transfer_value = 75;
			Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
			for (i = 0; i != work->effect->count; i++)
			{
				struct MotionObject *object =
					GetBattleObjectSlotFar(work->effect->actors[i])->object;

				for (j = 0; j != 10; j++)
				{
					struct EffectStep *ember = &work->particles[i * 10 + j];

					ember->x = object->x;
					ember->y = 20 << 16;
					ember->z = object->z;
					ember->velocity_x = Trig_Sin(j * 0x3334) << 2;
					ember->velocity_y = (Random16() & 0x7fff) + (1 << 16);
					ember->velocity_z = Trig_Cos(j * 0x3334) << 2;
					ember->variant = 0;
				}
			}
		}
		// 爆発 (40×24 を左右に分けて 6 コマ) と、そのあとの火の粉 5 個
		if (frame > 301)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				s32 start = i * 4 + 302;

				if (frame >= start && frame < i * 4 + 314)
				{
					s32					 cel = (frame - start) / 2;
					struct MotionObject *object =
						GetBattleObjectSlotFar(work->effect->actors[i])->object;

					point[0] = object->x;
					point[1] = 0;
					point[2] = object->z;
					EffectPosition_ApplyBaseAndYOffset(point, &screen);
					screen.x >>= 1;
					draw[0](canvas, work->sheet + cel * 480, screen.x - 20, screen.y - 24, 20, 24);
					draw[1](canvas, work->sheet + cel * 480, screen.x, screen.y - 24, 20, 24);
				}
				if (frame >= start + 6)
				{
					for (j = 0; j != 5; j++)
					{
						struct EffectStep *ember = &work->particles[i * 10 + j];

						EffectPosition_ApplyBaseAndYOffset(&ember->x, &screen);
						screen.x >>= 1;
						if (ember->variant >= 0 && ember->variant <= 26)
						{
							s32 cel = 6;
							s32 offset = ThornVines_EmberOffsets[cel];
							u32 size = ThornVines_EmberSizes[cel];

							draw[0](canvas, work->sheet + offset, screen.x - (size >> 1),
								screen.y - (size >> 1), size, size);
						}
						EffectStep_AdvanceWithGravity2D(ember, 60, 0x1000);
						ember->variant++;
					}
				}
			}
		}

		if (frame <= 127)
			Camera_ApplyShake(4, 16);
		else if (frame <= 301)
			Camera_ApplyShake(2, 2);
		else
			Camera_ApplyShake(4, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleEventRuntime_BeginPhaseFar(134);
	BattleEffect_RunImpactBurst(2, 128 << 16, height);
	for (i = 0; i != 8; i++)
		ResourceObject_ReleaseFar(work->objects[i]);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 岩と氷塊
//--------------------------------------------------------------------------

extern const Scale BattleFx10_UnitScale;

#define TRAILS ((struct EffectStep *)((u8 *)gMapCellBuffer + 0xe00)) // PARTICLES[128] から

//--------------------------------------------------------------------------
// 岩と氷塊 : 窓を開いて岩を崩し、大きな氷塊を落としてしぶきを散らしたあと、
// 氷塊を降らせて対象に当て、大岩を崩して破片を跳ねさせる
// effect : 対象の一覧
//--------------------------------------------------------------------------
void BattleFx_InitializeMode10(struct BattleEffectArgument *effect)
{
	s32						 pts[32][2];
	s32						 pos[4];
	void					*canvas;
	struct BattleEffectWork *work;
	s32						 frame;
	DrawRectangle			 draw;
	DrawRectangle			 draw_alt;
	u8						*dots;
	s32						 step;
	s32						 row;
	u32						*heap;
	s32						 count;
	s32						 i;
	s32						 j;
	s32						 ox;
	s32						 oy;
	s32						 y;
	s32						 t;
	struct EffectStep		*p;
	struct EffectStep		*q;
	Scale					 scale;

	heap = (u32 *)&gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	canvas = (void *)heap[0];
	work = (struct BattleEffectWork *)heap[HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_BATTLE_CANVAS];
	dots = (u8 *)heap[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_CANVAS];
	step = 1;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	BattlePres_ConfigureEffectDisplay();
	PLTT_BACKDROP = 0;
	*(u16 *)0x05000002 = 0; // BG パレット 0 の色 1
	work->transfer_mode = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	REG_WININ = 0x2137;
	BattleEffect_WipeCanvas(1, 0);
	REG_WIN0H = 0xf0f0; // 幅 0 の窓
	Resource_LoadAndDecompress((s32)&ResourceId_IceBlockSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, dots, 0, 0);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw = (DrawRectangle)heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_CANVAS];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 1);
	draw_alt = (DrawRectangle)heap[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_CANVAS];
	gProjection.center_y = 240;
	BattleFx_SelectLivingTargets(work->effect);
	WaitFrames(1);
	BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
	BattleFx_SpawnObjects(9, 0x174, 1);
	REG_DISPCNT = 0x7741;
	REG_BG2PA = 0x80;
	REG_BLDALPHA = 0x1010;
	REG_BLDCNT = 0x3f44;
	work->transfer_mode = 2;
	work->transfer_value = 50;

	for (i = 0; i != 64; i++)
	{
		PARTICLES[i].x = (Random16() & 63) + 32;
		PARTICLES[i].y = (Random16() & 31) + 120;
		PARTICLES[i].variant = -1;
	}

	for (i = 0; i != 32; i++)
	{
		pts[i][0] = BattleFx10_Points[i][0];
		pts[i][1] = BattleFx10_Points[i][1];
	}

	AudioCommand_PlayFar(141);

	//------------------------------------------------------------------
	// 前半 : 岩、大きな氷塊、しぶき (A・B で後半へ)
	//------------------------------------------------------------------
	for (frame = 0; frame != 288; frame++)
	{
		count = 16;
		if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 16)
		{
			break;
		}

		// 窓を左から開く : 右端を加速しながら動かし、各行を 0〜63 ドット引っこめる
		if (frame >= 0 && frame < 16)
		{
			u16 *edge = (u16 *)gMapCellBuffer;
			if (frame == 1)
			{
				u8 *inset = Ram_MapCellBuffer + 2;
				s32 n;
				for (n = 0; n != 128; n++)
				{
					inset[n] = Random16() & 63;
				}
				*edge = 0;
				Scheduler_AddOrUpdateCallback(BattleFx_BuildWindowEdgeTable, 0x480);
			}
			*edge += step;
			step += 3;
			if (frame == 15)
			{
				Scheduler_RemoveCallback(BattleFx_BuildWindowEdgeTable);
			}
		}

		// 残っている岩の数
		if (frame > 103)
		{
			count = 0;
		}
		else if (frame > 63)
		{
			count = 6;
		}
		else if (frame > 31)
		{
			count = 10;
		}

		// 揺れ : フレーム 167 まではでたらめ、176〜179 は表のとおり
		if (frame <= 167)
		{
			ox = (Random16() & 3) - 1;
			oy = (Random16() & 3) - 1;
			((struct BgScrollWords *)gBgScroll)->unk04 = ox;
			((struct BgScrollWords *)gBgScroll)->unk06 = oy + 32;
		}
		else
		{
			ox = 0;
			oy = 0;
			((struct BgScrollWords *)gBgScroll)->unk04 = 0;
			((struct BgScrollWords *)gBgScroll)->unk06 = 32;
		}
		if (frame >= 176 && frame < 180)
		{
			ox = -BattleFx10_ShakeOffsets[frame - 176];
			oy = BattleFx10_ShakeOffsets[frame - 176];
			((struct BgScrollWords *)gBgScroll)->unk04 = ox;
			((struct BgScrollWords *)gBgScroll)->unk06 = oy + 32;
		}

		for (i = 0; i != count; i++)
		{
			s32 n = i % 3;
			draw(canvas, work->sheet + BattleFx10_RockCells[n], BattleFx10_Points[i][0] - ox,
				BattleFx10_Points[i][1] - BattleFx10_RockHeights[n] - oy, BattleFx10_RockWidths[n],
				BattleFx10_RockHeights[n]);
		}

		// 3×3 のオブジェクトを 32 ドットおきに並べる
		scale = BattleFx10_UnitScale;
		if (frame == 174)
		{
			for (i = 0; i != 9; i++)
			{
				((struct {
					u8 pad[9];
					u8 tile_hi : 2;
					u8 priority : 2;
					u8 palette : 4;
				} *)work->objects[i])
					->priority = 0;
			}
		}
		if (frame > 208)
		{
			AnimationObjects_SelectAnimationFar(
				work->objects[2], BattleFx10_Animations[(frame / 4) & 3]);
		}

		scale.x = 0x10000;
		scale.y = 0x10000;
		pos[3] = 0;
		pos[1] = 255 << 16;
		for (row = 0; row != 3; row++)
		{
			for (j = 0; j != 3; j++)
			{
				pos[0] = (j * 32 + 144 - ox) << 16;
				pos[2] = (row * 32 + 76 - oy) << 16;
				Object_ApplyProjectedPlacementFar(work->objects[row * 3 + j], pos, &scale, 0);
			}
		}

		// 大きな氷塊 (24×48) : 160〜175 で右上から落ち、208 からゆっくり沈む
		t = frame - 160;
		if (t >= 0 && t < 158)
		{
			s32 u = frame - 208;
			s32 sx = 80;
			s32 sy = 8;
			if (frame <= 175)
			{
				sx = 96 - t;
				sy = t * 4 - 56;
			}
			else if (frame > 208)
			{
				sy = u / 4 + 8;
			}
			draw_alt(canvas, work->sheet + 0xc46, sx, sy, 24, 48);
		}

		if (frame == 32)
		{
			AudioCommand_PlayFar(134);
		}
		if (frame == 64)
		{
			AudioCommand_PlayFar(134);
		}
		if (frame == 104)
		{
			AudioCommand_PlayFar(134);
		}
		if (frame == 176)
		{
			AudioCommand_PlayFar(134);
		}
		if (frame == 226)
		{
			AudioCommand_PlayFar(145);
		}
		Render_ResetTransformState();

		// 岩が崩れるたびに破片を 32 個ずつ跳ねあげる
		if (frame == 32)
		{
			for (i = 0; i != 32; i++)
			{
				PARTICLES[i].x = ((Random16() & 31) + 68) << 16;
				PARTICLES[i].y = ((Random16() & 31) + 8) << 16;
				PARTICLES[i].velocity_x = ((Random16() & 127) - 63) << 11;
				PARTICLES[i].velocity_y = ((-Random16() & 127) - 64) << 11;
				PARTICLES[i].variant = (Random16() & 15) + 32;
			}
		}
		if (frame == 64)
		{
			for (i = 0; i != 32; i++)
			{
				PARTICLES[i + 32].x = (Random16() % 48 + 60) << 16;
				PARTICLES[i + 32].y = ((Random16() & 31) + 52) << 16;
				PARTICLES[i + 32].velocity_x = ((Random16() & 127) - 63) << 12;
				PARTICLES[i + 32].velocity_y = ((-Random16() & 31) - 32) << 13;
				PARTICLES[i + 32].variant = (Random16() & 15) + 32;
			}
		}
		if (frame == 104)
		{
			for (i = 0; i != 32; i++)
			{
				PARTICLES[i].x = ((Random16() & 63) + 52) << 16;
				PARTICLES[i].y = ((Random16() & 31) + 72) << 16;
				PARTICLES[i].velocity_x = ((Random16() & 127) - 63) << 11;
				PARTICLES[i].velocity_y = ((-Random16() & 31) - 32) << 13;
				PARTICLES[i].variant = (Random16() & 15) + 32;
			}
		}

		if (frame >= 32 && frame < 208)
		{
			for (i = 0; i != 64; i++)
			{
				if (PARTICLES[i].variant >= 0)
				{
					s32 n;
					if (frame > 191)
					{
						n = i % 7 + 4;
					}
					else
					{
						n = i & 3;
					}
					draw_alt(canvas, work->sheet + BattleFx10_DebrisCells[n], HI(PARTICLES[i].x),
						HI(PARTICLES[i].y), BattleFx10_DebrisWidths[n],
						BattleFx10_DebrisHeights[n]);
					PARTICLES[i].x += PARTICLES[i].velocity_x;
					PARTICLES[i].y += PARTICLES[i].velocity_y;
					PARTICLES[i].velocity_y += 0x2000;
				}
			}
		}

		// しぶき : (72, 56) から左下へ 128 個、228 からはその跡に煙を残す
		if (frame > 223)
		{
			if (frame == 224)
			{
				for (i = 0; i != 128; i++)
				{
					PARTICLES[i].x = 72 << 16;
					PARTICLES[i].y = 56 << 16;
					PARTICLES[i].velocity_x = (-(Random16() & 127) - 64) << 11;
					PARTICLES[i].velocity_y = ((Random16() & 127) + 16) << 11;
					PARTICLES[i].variant = Random16();
				}
			}
			for (i = 0; i != 128; i++)
			{
				if (frame >= i / 4 + 224)
				{
					s32 n = i % 3;
					if ((i & 1) == 0)
					{
						draw(canvas, work->sheet + BattleFx10_SprayCells[n], HI(PARTICLES[i].x),
							HI(PARTICLES[i].y), BattleFx10_SprayWidths[n],
							BattleFx10_SprayHeights[n]);
					}
					PARTICLES[i].x += PARTICLES[i].velocity_x;
					PARTICLES[i].y += PARTICLES[i].velocity_y;
					if ((PARTICLES[i].x >> 16) < -16 || (PARTICLES[i].y >> 16) > 120)
					{
						PARTICLES[i].x = 72 << 16;
						PARTICLES[i].y = 56 << 16;
					}
					PARTICLES[i].variant++;
				}
			}
			if (frame == 228)
			{
				for (i = 0; i != 128; i++)
				{
					p = &PARTICLES[i];
					q = &TRAILS[i];
					q->x = p->x;
					q->y = p->y;
					q->variant = 0;
				}
			}
			for (i = 0, q = TRAILS, p = PARTICLES; i != 128; i++, q++, p++)
			{
				if (frame >= i + 228)
				{
					s32 n = q->variant / 2 % 9;
					s32 sz = BattleFx_PuffSizes[n];
					draw(canvas, dots + BattleFx_PuffCells[n], HI(q->x) - sz / 2, HI(q->y) - sz / 2,
						sz, sz);
					if (++q->variant == 18)
					{
						q->x = p->x;
						q->y = p->y;
						q->variant = 0;
					}
				}
			}
		}

		work->transfer_pending = 1;
		WaitFrames(1);
	}

	//------------------------------------------------------------------
	// 後半 : 氷塊を降らせて対象に当て、大岩を崩して破片を跳ねさせる
	//------------------------------------------------------------------
	for (i = 0; i != 9; i++)
	{
		ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
	}
	BattleEffect_SetupBlendedDisplay();
	REG_BLDALPHA = 0x1010;
	AudioCommand_PlayFar(289);

	for (i = 0; i != 64; i++)
	{
		work->particles[i].x = ((Random16() & 127) + 64) << 16;
		work->particles[i].z = -(16 << 16);
		work->particles[i].y = (-(Random16() & 127) - 64) << 16;
		work->particles[i].velocity_x = (-(Random16() & 63) - 127) << 12;
		work->particles[i].velocity_y = ((Random16() & 63) + 127) << 12;
		work->particles[i].variant = 0;
	}
	for (i = 0; i != 64; i++)
	{
		PARTICLES[i].x = Random16() & 127;
		PARTICLES[i].y = (Random16() & 63) + i / 2;
		PARTICLES[i].variant = -i / 2;
	}

	for (frame = 0; frame != 146; frame++)
	{
		count = 0;
		if (frame == 96)
		{
			BattleEventRuntime_BeginPhaseFar(134);
		}
		if (frame == 16)
		{
			work->shake_frames = 32;
		}
		// 大岩を 2 フレームに 1 つずつ、16 個まで出す
		if (frame > 16)
		{
			count = (frame - 16) / 2;
			if (count > 16)
			{
				count = 16;
			}
		}
		if (frame >= 9 && frame < 72 && (frame & 3) == 0)
		{
			AudioCommand_PlayFar(132);
		}
		if (frame == 72)
		{
			AudioCommand_PlayFar(145);
		}
		// 右上から斜めに落ちる塊 (40×80)
		if (frame > 64)
		{
			draw(canvas, work->sheet + 0x14f9, (64 - frame) * 7 + 88, frame * 14 - 992, 40, 80);
		}
		if (frame <= 71)
		{
			for (i = 0; i != count; i++)
			{
				s32 n = i % 3;
				draw(canvas, work->sheet + BattleFx10_BoulderCells[n], BattleFx10_Points[i][0] - 56,
					BattleFx10_Points[i][1] - BattleFx10_BoulderHeights[n],
					BattleFx10_BoulderWidths[n], BattleFx10_BoulderHeights[n]);
			}
		}
		// 大岩が崩れ、破片 64 個を岩の位置から跳ねあげる
		if (frame == 72)
		{
			for (i = 0; i != 64; i++)
			{
				work->particles[i].x = (BattleFx10_Points[i & 15][0] - 56) << 16;
				work->particles[i].y = BattleFx10_Points[i & 15][1] << 16;
				work->particles[i].velocity_x = ((Random16() & 127) - 63) << 13;
				work->particles[i].velocity_y = (-(Random16() & 31) - 16) << 14;
			}
			if (frame == 72)
			{
				work->shake_frames = 4;
			}
		}
		// 破片 : 速く落ちてきたものは y = 120 で 1/4 の速さで跳ね返る
		if (frame > 71)
		{
			count = 32;
			if (frame != 72)
			{
				count = 64;
			}
			for (i = 0; i != count; i++)
			{
				q = &work->particles[i];
				y = HI(q->y);
				if (y <= 135)
				{
					s32 n = i % 3;
					draw(canvas, work->sheet + BattleFx10_FallCells[n], HI(q->x),
						y - BattleFx10_FallHeights[n], BattleFx10_FallWidths[n],
						BattleFx10_FallHeights[n]);
					EffectStep_AdvanceWithGravity2D(q, 64, 1 << 16);
					if (HI(q->y) > 120 && q->velocity_y > 8 << 16)
					{
						q->velocity_y = -q->velocity_y / 4;
						q->y = 120 << 16;
						work->shake_frames = 1;
					}
				}
			}
		}
		// 降ってくる氷塊 (24×48) : フレーム 47 までは上から出しなおす
		if (frame <= 71)
		{
			for (i = 0; i != 64; i++)
			{
				draw(canvas, work->sheet, HI(work->particles[i].x) - 12,
					HI(work->particles[i].y) - 24, 24, 48);
				work->particles[i].x += work->particles[i].velocity_x;
				work->particles[i].y += work->particles[i].velocity_y;
				if ((work->particles[i].y >> 16) > 120 && frame <= 47)
				{
					work->particles[i].x = ((Random16() & 127) + 64) << 16;
					work->particles[i].y = -(16 << 16);
				}
			}
		}

		for (i = 0; i != work->effect->count; i++)
		{
			if (frame == i * 8 + 32)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 9, 5, -1, 0);
			}
		}

		// 煙 : フレーム 127 までは出しなおす
		if (frame > 72)
		{
			for (i = 0; i != 64; i++)
			{
				if (PARTICLES[i].variant >= 0 && PARTICLES[i].variant < 18)
				{
					s32 n = PARTICLES[i].variant / 2 % 9;
					s32 sz = BattleFx_PuffSizes[n];
					draw(canvas, dots + BattleFx_PuffCells[n], PARTICLES[i].x - sz / 2,
						PARTICLES[i].y - sz / 2, sz, sz);
				}
				if (++PARTICLES[i].variant == 18 && frame <= 127)
				{
					PARTICLES[i].x = Random16() & 127;
					PARTICLES[i].y = (Random16() & 63) + (frame - 54) / 2;
					PARTICLES[i].variant = 0;
				}
			}
		}

		if (frame >= 72 && frame < 80)
		{
			Camera_ApplyShake(8, 8);
		}
		else
		{
			Camera_ApplyShake(2, 2);
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 氷の破片
//--------------------------------------------------------------------------
#define SHARDS_PER_MEMBER 128 // 対象ひとりあたりの破片の数

//--------------------------------------------------------------------------
// 対象ごとに 20 フレームずつずらして、破片を跳ねあげる
// effect : 対象の一覧
//--------------------------------------------------------------------------
void BattleFx_RunIceShardBursts(struct BattleEffectArgument *effect)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 draw;
	u8						*sheet;
	struct BattleCamera		*camera;
	s32						 point[3];
	struct EffectPosition	 screen;
	s32						 member;
	s32						 frame;
	s32						 i;

	heap = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_EFFECT];
	camera = heap[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet), 128);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw = (DrawRectangle)heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];

	for (i = 0; i != 8 * SHARDS_PER_MEMBER; i++)
		PARTICLES[i].variant = -1;

	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

	// 体の中ほどのまわりに置き、横へばらけて上へ跳ねる
	for (member = 0; member != work->effect->count; member++)
	{
		struct MotionObject *object;
		s32					 offset;
		s32					 height;
		struct EffectStep	*shard;

		offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
		object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
		height = Battle_GetObjectTableValueFar(work->effect->actors[member]) / 2;
		point[0] = object->x;
		point[1] = height;
		point[2] = object->z;
		EffectPosition_ApplyBaseAndYOffset(point, &screen);
		screen.x >>= 1;
		for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
			i != SHARDS_PER_MEMBER; i++)
		{
			s32 radius;
			s32 angle;
			s32 lift;

			radius = Random16() & 0xff;
			angle = Random16() & 0xffff;
			shard->x = ((Trig_Sin(angle) * radius) >> 7) + (screen.x << 16);
			shard->y = ((Trig_Cos(angle) * radius) >> 3) + (screen.y << 16);
			shard->velocity_x = (128 - (Random16() & 0xff)) << 9;
			lift = Random16() & 0xff;
			shard->variant = 0;
			shard->velocity_y = (-lift - 128) << 10;
			shard++;
		}
	}

	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != work->effect->count * 20 + 56; frame++)
	{
		if (frame == 32)
			BattleEventRuntime_BeginPhaseFar(0);
		for (member = 0; member != work->effect->count; member++)
		{
			s32 offset;

			offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
			if (frame == member * 20)
			{
				AudioCommand_PlayFar(143);
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, -1, member, 20);
			}
			// 落ちてきて y = 112 を越えたら消す
			if (frame > member * 20)
			{
				struct EffectStep *shard;

				for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
					i != SHARDS_PER_MEMBER; i++)
				{
					if (shard->variant >= 0)
					{
						s32 size;

						size = i % 3 + 1;
						draw(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
							HI(shard->x) - size / 2, HI(shard->y) - size, size, size * 2);
						EffectStep_AdvanceWithGravity2D(shard, 62, IceShardBursts_Gravities[i & 3]);
						shard->variant++;
						if (shard->velocity_y > 0 && HI(shard->y) > 112)
							shard->variant = -1;
					}
					shard++;
				}
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// gather.c
// バトル演出：対象から出た光の粒を使い手へ吸い寄せる
//--------------------------------------------------------------------------

#define MOTES_PER_MEMBER 128 // 対象ひとりあたりの粒の数

//--------------------------------------------------------------------------
// 対象ごとに 20 フレームずつずらして粒を出し、使い手の体の中心へ集める
// effect : variant が 0 なら IceBlock、それ以外は Blast のパレット
//--------------------------------------------------------------------------
void BattleFx_RunGatheringMotes(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 draw;
	u8						*sheet;
	struct BattleCamera		*camera;
	struct EffectPosition	 screen;
	s32						 alternate;
	s32						 member;
	s32						 frame;
	s32						 i;

	heap_cache = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * 4);
	if (effect->variant == 0)
		alternate = 0;
	else
		alternate = 1;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Iwram_CopyWords((void *)BG_PLTT,
		Resource_GetTableEntry(
			alternate == 0 ? (s32)&ResourceId_IceBlockSheet : (s32)&ResourceId_BlastSheet),
		128);

	// 対象の体の中心から、各軸 ±2 ドット/フレームの乱数速度で飛び出す
	for (i = 0; i != 8 * MOTES_PER_MEMBER; i++)
		PARTICLES[i].variant = -1;

	for (member = 0; member != work->effect->count; member++)
	{
		struct MotionObject *obj;
		s32					 half;

		obj = GetBattleObjectSlotFar(work->effect->actors[member])->object;
		half = Battle_GetObjectTableValueFar(work->effect->actors[member]) / 2;
		for (i = 0; i != MOTES_PER_MEMBER; i++)
		{
			struct EffectStep *mote = &PARTICLES[member * MOTES_PER_MEMBER + i];

			mote->x = obj->x;
			mote->y = obj->y + half;
			mote->z = obj->z;
			mote->velocity_x = ((Random16() & 0xff) - 128) << 10;
			mote->velocity_y = ((Random16() & 0xff) - 128) << 10;
			mote->velocity_z = ((Random16() & 0xff) - 128) << 10;
			mote->variant = 0;
		}
	}

	// 比較合成で描き、転送のたびにキャンバスの画素値を 4 ずつ下げて残像にする
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	work->transfer_mode = 3;
	work->transfer_value = 0x04040404;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(142);

	for (frame = 0; frame != work->effect->count * 20 + 72; frame++)
	{
		struct MotionObject *user;
		s32					 half;

		user = GetBattleObjectSlotFar(work->effect->actor)->object;
		half = Battle_GetObjectTableValueFar(work->effect->actor) / 2;
		if (frame == 64)
			BattleEventRuntime_BeginPhaseFar(133);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame == 40)
			ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
		if (frame == work->effect->count * 20 + 52)
			ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
		for (member = 0; member != work->effect->count; member++)
		{
			if (frame == member * 20)
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, 42);
			if (frame > member * 20)
			{
				for (i = 0; i != 32; i++)
				{
					struct EffectStep *mote = &PARTICLES[member * MOTES_PER_MEMBER + i];

					if (mote->variant >= 0)
					{
						s32 size;

						// キャンバスは横 2 倍で表示するので x を半分にする
						EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &screen);
						screen.x >>= 1;
						size = 6;
						draw(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
							screen.x - size / 2, screen.y - size, size, size * 2);
						EffectStep_AdvanceWithGravity3D(mote, 62, 0);

						// 出てから 10 フレーム後 (粒ごとに 1 フレームずつ遅れる) に
						// 距離の 1/256 ずつ使い手へ加速し、水平に 16 ドット以内まで近づいたら消す
						if (frame > member * 20 + i + 10)
						{
							s32 dx;
							s32 dy;
							s32 dz;

							dx = (user->x - mote->x) >> 8;
							dy = (user->y + half - mote->y) >> 8;
							dz = (user->z - mote->z) >> 8;
							mote->velocity_x += dx;
							mote->velocity_y += dy;
							mote->velocity_z += dz;
							if ((dx > -0x1000 && dx < 0x1000) && (dz > -0x1000 && dz < 0x1000))
								mote->variant = -1;
						}
					}
				}
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// series_a.c
// バトル演出：対象ごとに光の粒を回しながら集める (七種)
//--------------------------------------------------------------------------
// PARTICLES : 光の粒 (対象ごとに 64)

#define JIN_OFS 0x1000 // sheet の中のジンの絵

//--------------------------------------------------------------------------
// mode ごとの入口
//--------------------------------------------------------------------------
void BattleFx_RunSeriesAMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesA(effect, 0);
}

void BattleFx_RunSeriesAMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesA(effect, 1);
}

void BattleFx_RunSeriesAMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesA(effect, 2);
}

// effect->variant が 0 なら mode 3、ほかは mode 4
void BattleFx_RunSeriesAMode3Or4(struct BattleEffectArgument *effect)
{
	if (effect->variant == 0)
	{
		BattleFx_RunSeriesA(effect, 3);
		return;
	}
	BattleFx_RunSeriesA(effect, 4);
}

void BattleFx_RunSeriesAMode4(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesA(effect, 4);
}

void BattleFx_RunSeriesAMode5(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesA(effect, 5);
}

void BattleFx_RunSeriesAMode6(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesA(effect, 6);
}

//--------------------------------------------------------------------------
// 対象ごとに 8 フレームずつずらして、まわりに散らした光の粒を回しながら
// 中心へ集める。対象ごとに 20 フレーム目でかいふくの音
// effect->prelude が 1 なら、先にジン (mode 6 は Mars、ほかは Mercury) を出して揺らす
// mode : 粒の散らし方と回し方。0 だけキャンバスを等倍にして最初の対象を中央に置く
//        3・4・5 は粒を寄せずに順に消し、0・3・4・5 は SparkleDots の絵で描く
//        色は 6 が Mars、2 が Blast、ほかは variant が 0 なら IceShard、でなければ IceBlock
//--------------------------------------------------------------------------
void BattleFx_RunSeriesA(struct BattleEffectArgument *effect, u32 mode)
{
	void				   **slots;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 total;
	s32						 member;
	u8						*sprites;
	s32						 limit;
	s32						 shift;
	s32						 narrow;
	DrawRectangle			*draw;
	s32						 frame;
	s32						 i;
	u8						*palette;
	struct EffectPosition	 base;
	struct EffectPosition	 screen;
	s32						 point[3];
	s32						 origin_x;
	s32						 origin_y;
	BlitterPair				 blit;

	slots = gBattleFxWork;
	cursor = slots;
	work = *cursor++;
	canvas = *cursor;
	sprites = slots[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_EFFECT];
	shift = 0;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);

	// ジンを出す。出た位置を origin に受け取る
	if (work->effect->prelude == 1)
	{
		s32 kind = mode == 6 ? 2 : 1;

		if (mode == 6 || mode == 0)
			BattleFx_PrepareCanvasEffect(effect, kind, work->effect->side, 0, &origin_x, &origin_y);
		else
			BattleFx_PrepareCanvasEffect(effect, kind, work->effect->side, 1, &origin_x, &origin_y);
		work->effect->variant = 0;
	}

	// mode 0 はキャンバスを横も等倍にし、最初の対象が中央 (64) に来るようずらす
	// ほかは横 2 倍のまま (narrow)
	if (mode == 0)
	{
		EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &base);
		shift = 64 - base.x;
		REG_BG2X = shift << 8;
		REG_BG2PA = 0x100;
		narrow = 0;
	}
	else
	{
		narrow = 1;
	}

	// 絵と色
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, work->sheet, 0, 0);
	if (mode <= 1 || mode == 3 || mode == 4 || mode == 5)
	{
		if (work->effect->variant == 0)
			palette = Resource_GetTableEntry((s32)&ResourceId_IceShardSheet);
		else
			palette = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
	}
	else if (mode == 6)
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
	}
	else
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
	}
	Iwram_CopyWords((s32)BG_PLTT, (s32)palette, 128);

	// ジンの絵 (等倍なら 40x40、横 2 倍なら 20x40 の小さい絵)
	if (narrow == 0)
	{
		if (mode == 6)
			palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
		else
			palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
	}
	else
	{
		if (mode == 6)
			palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet);
		else
			palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSmallSheet);
	}
	palette += 128;
	Resource_DecodeType01(palette, work->sheet + JIN_OFS);
	BattleFx_FetchRectangleBlitters(work->effect->side, draw = blit); // [0] 比較、[1] 加算

	// 光の粒を中心のまわりに散らす
	if (mode == 0 || mode == 6)
	{
		// x と z は ±64 ドット、y は 32〜95 ドット上
		for (i = 0; i != 512; i++)
		{
			struct EffectStep *spark = &PARTICLES[i];

			spark->x = ((Random16() & 0xff) - 127) << 15;
			spark->y = ((Random16() & 0x7f) + 64) << 15;
			spark->z = ((Random16() & 0xff) - 127) << 15;
			spark->variant = 0;
		}
		total = work->effect->count * 8 + 88;
	}
	else if (mode == 1)
	{
		// どれも ±64 ドット
		for (i = 0; i != 512; i++)
		{
			struct EffectStep *spark = &PARTICLES[i];

			spark->x = ((Random16() & 0xff) - 127) << 15;
			spark->y = ((Random16() & 0xff) - 127) << 15;
			spark->z = ((Random16() & 0xff) - 127) << 15;
			spark->variant = 0;
		}
		total = work->effect->count * 8 + 88;
	}
	else if (mode == 2)
	{
		// 50 ドット下の、半径 32〜95 ドットの輪
		for (i = 0; i != 512; i++)
		{
			struct EffectStep *spark = &PARTICLES[i];
			s32				   heading = Random16() & 0xffff;
			s32				   radius = (Random16() & 63) + 32;

			spark->x = Trig_Sin(heading) * radius;
			spark->y = -50 << 16;
			spark->z = Trig_Cos(heading) * radius;
			spark->velocity_y = ((Random16() & 31) + 32) << 13;
			spark->variant = 0;
		}
		total = work->effect->count * 8 + 88;
	}
	else if (mode == 3)
	{
		// x と z は ±64 ドット、y は ±32 ドット
		for (i = 0; i != 512; i++)
		{
			struct EffectStep *spark = &PARTICLES[i];

			spark->x = ((Random16() & 0xff) - 127) << 15;
			spark->y = ((Random16() & 0xff) - 127) << 14;
			spark->z = ((Random16() & 0xff) - 127) << 15;
			spark->variant = 0;
		}
		total = work->effect->count * 8 + 72;
	}
	else
	{
		// どれも ±64 ドット
		for (i = 0; i != 512; i++)
		{
			struct EffectStep *spark = &PARTICLES[i];

			spark->x = ((Random16() & 0xff) - 127) << 15;
			spark->y = ((Random16() & 0xff) - 127) << 15;
			spark->z = ((Random16() & 0xff) - 127) << 15;
			spark->variant = 0;
		}
		total = work->effect->count * 8 + 72;
	}

	// 対象ひとりあたりの粒の数 (variant 0 は 32、2 は 128、ほかは 64)
	limit = 64;
	if (work->effect->variant == 0)
		limit = 32;
	else if (work->effect->variant == 2)
		limit = 128;
	work->transfer_mode = 2;   // 転送のたびに 3/4 へ減らして残像にする
	work->transfer_value = 75; // 3/4
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != total; frame++)
	{
		struct BattleCamera *camera = gCameraWork;

		if (frame == 40)
			BattleEventRuntime_BeginPhaseFar(0);

		// ジン：左右に揺れ、32 フレームを過ぎたら 2 ドットずつ昇る
		// はじめの 4 フレームは加算でも重ねる
		if (work->effect->prelude == 1)
		{
			if (narrow == 0)
			{
				s32 x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin_x + shift - 20;
				s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin_y - 24;

				if (frame > 32)
					y = y - frame * 2 + 64;
				blit[0](canvas, work->sheet + JIN_OFS, x, y, 40, 40);
				if (frame <= 3)
					draw[1](canvas, work->sheet + JIN_OFS, x, y, 40, 40);
			}
			else
			{
				s32 x = ((Trig_Sin(frame << 11) * 10) >> 16) + origin_x / 2 - 10;
				s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin_y - 24;

				if (frame > 32)
					y = y - frame * 2 + 64;
				blit[0](canvas, work->sheet + JIN_OFS, x, y, 20, 40);
				if (frame <= 3)
					draw[1](canvas, work->sheet + JIN_OFS, x, y, 20, 40);
			}
		}

		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *object =
				GetBattleObjectSlotFar(work->effect->actors[member])->object;
			s32 start = member * 8;

			// 対象の 40 ドット上を中心にする
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			point[0] = object->x;
			point[1] = 40 << 16;
			point[2] = object->z;
			SceneTransform_ApplyPosition(point);
			if (frame == start + 20)
				AudioCommand_PlayFar(SOUND_RECOVERY);
			if (frame == start + 36)
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, -1, member, 28);
			if (frame > start)
			{
				if (mode == 0 || mode == 6)
				{
					SceneTransform_ApplyYaw((frame - member * 8) << 9);
				}
				else if (mode == 1)
				{
					SceneTransform_ApplyPitch(frame << 9);
					SceneTransform_ApplyRoll(frame << 9);
				}
				else if (mode == 2)
				{
					SceneTransform_ApplyYaw((frame - member * 40) << 9);
				}
				else if (mode == 3)
				{
					SceneTransform_ApplyYaw((frame - member * 8) << 9);
				}
				else
				{
					SceneTransform_ApplyYaw((frame - member * 8) << 9);
					SceneTransform_ApplyPitch((frame - member * 8) << 9);
				}

				// 粒は 1 フレームに 4 つずつ出る。3・4・5 では start + i / 2 + 32 フレームで消える
				for (i = 0; i != limit; i++)
				{
					struct EffectStep *spark = &PARTICLES[member * 64 + i];
					s32				   end;

					if (mode == 3 || mode == 4 || mode == 5)
						end = start + i / 2 + 32;
					else
						end = 0x10000;
					if (frame > start + i / 4 && frame < end)
					{
						s32 xx = (spark->x >> 8) * (spark->x >> 8);
						s32 yy = (spark->y >> 8) * (spark->y >> 8);
						s32 zz = (spark->z >> 8) * (spark->z >> 8);
						s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

						if (distance != 0)
						{
							s32 size;

							EffectPosition_ApplyBaseAndYOffset((s32 *)spark, &screen);
							if (mode == 0)
								screen.x += shift;
							else
								screen.x >>= 1;
							screen.y += 16;

							// 大きさは奥行き 314〜634 で 3〜1 (mode 0 は 9 種を順に)
							if (screen.depth < 314)
								screen.depth = 314;
							if (screen.depth > 634)
								screen.depth = 634;
							size = 3 - (screen.depth - 314) / 128;
							if (mode == 0)
								size = (i * 4 + frame) % 9;
							if (mode == 0 || mode == 3 || mode == 4 || mode == 5)
							{
								u8 *cell = work->sheet + BattleFx_PuffCells[size];
								u32 edge = BattleFx_PuffSizes[size];
								u32 half = edge >> 1;

								draw[1](canvas, cell, screen.x - half, screen.y - half, edge, edge);
							}
							else
							{
								draw[1](canvas, sprites + ParticleStreams_CellOffsets[size - 1],
									screen.x - size / 2, screen.y - size, size, size * 2);
							}

							// 1 フレームに 2 ドットずつ中心へ寄せる
							if (mode <= 2 || mode == 6)
							{
								spark->x -= spark->x / distance;
								spark->y -= spark->y / distance;
								spark->z -= spark->z / distance;
							}
						}
					}
				}
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// run_series_b_mode0.c
// バトル演出：対象ごとに光を集め、はじけたあとに絵を出す (六種)
//--------------------------------------------------------------------------
// PARTICLES : 集まる光 (対象ごとに 64)
//--------------------------------------------------------------------------
// mode ごとの入口
//--------------------------------------------------------------------------
void BattleFx_RunSeriesBMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesB(effect, 0);
}

void BattleFx_RunSeriesBMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesB(effect, 1);
}

void BattleFx_RunSeriesBMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesB(effect, 2);
}

void BattleFx_RunSeriesBMode3(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesB(effect, 3);
}

void BattleFx_RunSeriesBMode5(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesB(effect, 5);
}

void BattleFx_RunSeriesBMode4(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesB(effect, 4);
}

// sheet の中の置き場所
enum SeriesBSheet
{
	SERIES_B_FLARES = 0x0000,  // 光の粒 (ParticleSpritesD)
	SERIES_B_BURST = 0x060e,   // はじける光 40x40 が 6 枚 (GlowOrb)
	SERIES_B_PICTURE = 0x2b8e, // mode で選ぶ絵
	SERIES_B_DJINNI = 0x65c0   // ジンの絵 40x40
};

//--------------------------------------------------------------------------
// 対象ごとに 8 フレームずつずらして、まわりの光の粒を回しながら
// 中心へ集め、はじけさせてから mode の絵を描く
// キャンバスは横 1.25 倍で見せる (画面の x は 4/5 してキャンバスの x)
// effect->prelude が 1 なら、先にジンを出してその場で揺らす
// mode : 絵は 0 = 短剣 (Dagger)、1・2 = 盾 (Shield)、3・5 = 筋 (VioletStreak)、4 = Wisp
//        色は 3 が Venus、2 が Jupiter、0・1・5 が Mars の絵から (4 は変えない)
//        ジンは 3 が Venus、2・4 が Jupiter、ほかは Mars
//--------------------------------------------------------------------------
void BattleFx_RunSeriesB(struct BattleEffectArgument *effect, u32 mode)
{
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	BlitterPair				 draw;
	s32						 member;
	s32						 shift;
	s32						 flags;
	s32						 frame;
	s32						 i;
	struct EffectPosition	 base;
	s32						 center[3];
	struct EffectPosition	 screen;
	s32						 point[3];
	s32						 origin_x;
	s32						 origin_y;

	cursor = gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);

	// ジンを出す (Venus は 0、Jupiter は 3、Mars は 2)。出た位置を origin に受け取る
	if (work->effect->prelude == 1)
	{
		if (mode == 3)
			BattleFx_PrepareCanvasEffect(
				effect, 0, work->effect->side ^ 1, 1, &origin_x, &origin_y);
		else if (mode == 2 || mode == 4)
			BattleFx_PrepareCanvasEffect(
				effect, 3, work->effect->side ^ 1, 1, &origin_x, &origin_y);
		else
			BattleFx_PrepareCanvasEffect(
				effect, 2, work->effect->side ^ 1, 1, &origin_x, &origin_y);
		origin_x = origin_x * 4 / 5;
	}
	REG_BG2PA = 0xcc; // 横 1.25 倍

	// 絵と色
	Resource_LoadAndDecompress(
		(s32)&ResourceId_ParticleSpritesD, work->sheet + SERIES_B_FLARES, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet + SERIES_B_BURST, 1, 1);
	if (mode == 3 || mode == 5)
	{
		Resource_LoadAndDecompress(
			(s32)&ResourceId_VioletStreakSheet, work->sheet + SERIES_B_PICTURE, 1, 1);
		if (mode == 3)
			Iwram_CopyWords(
				(s32)BG_PLTT, (s32)Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet), 128);
		else
			Iwram_CopyWords(
				(s32)BG_PLTT, (s32)Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
	}
	else if (mode == 4)
	{
		Resource_LoadAndDecompress(
			(s32)&ResourceId_WispSheet, work->sheet + SERIES_B_PICTURE, 1, 1);
	}
	else
	{
		void *palette;

		if (mode == 0)
			Resource_LoadAndDecompress(
				(s32)&ResourceId_DaggerSheet, work->sheet + SERIES_B_PICTURE, 1, 0);
		else
			Resource_LoadAndDecompress(
				(s32)&ResourceId_ShieldSheet, work->sheet + SERIES_B_PICTURE, 1, 0);
		if (mode == 0)
			palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
		else if (mode == 2 || mode == 4)
			palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
		else if (mode == 1)
			palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
		else
			palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
		Iwram_CopyWords((s32)BG_PLTT, (s32)palette, 128);
	}
	if (mode == 3)
		Resource_LoadAndDecompress(
			(s32)&ResourceId_VenusDjinnSheet, work->sheet + SERIES_B_DJINNI, 1, 0);
	else if (mode == 2 || mode == 4)
		Resource_LoadAndDecompress(
			(s32)&ResourceId_JupiterDjinnSheet, work->sheet + SERIES_B_DJINNI, 1, 0);
	else
		Resource_LoadAndDecompress(
			(s32)&ResourceId_MarsDjinnSheet, work->sheet + SERIES_B_DJINNI, 1, 0);

	// 光の粒：中心から x と z は ±25 ドット、y は ±50 ドットに散らす
	for (i = 0; i != 512; i++)
	{
		struct EffectStep *flare = &PARTICLES[i];

		flare->x = (Random16() % 200 - 100) << 14;
		flare->y = (Random16() % 200 - 100) << 15;
		flare->z = (Random16() % 200 - 100) << 14;
		flare->variant = 0;
	}

	// 対象がひとりなら、その対象がキャンバスの中央 (64) に来るようずらす
	if (work->effect->count == 1)
	{
		EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &base);
		shift = -base.x * 4 / 5 + 64;
	}
	else
	{
		if (work->effect->side == 1)
			shift = -64;
		else
			shift = 0;
	}
	REG_BG2X = shift << 8;
	work->transfer_mode = 2;   // 転送のたびに半分へ減らして残像にする
	work->transfer_value = 50; // 1/2
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	if (work->effect->side == 1)
		flags = GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X;
	else
		flags = GOUSEI_CLIP_X | GOUSEI_CLIP_Y;
	AudioCommand_PlayFar(142);

	for (frame = 0; frame != work->effect->count * 8 + 108; frame++)
	{
		struct BattleCamera *camera = WORK_SLOT(HEAP_SLOT_CAMERA);

		if (frame == 80)
			BattleEventRuntime_BeginPhaseFar(0);

		// ジン：左右に揺れ、32 フレームを過ぎたら 2 ドットずつ昇る
		// はじめの 4 フレームは加算でも重ねる
		if (work->effect->prelude == 1)
		{
			s32 x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin_x + shift - 20;
			s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin_y - 24;

			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags ^ GOUSEI_FLIP_X, GOUSEI_HIKAKU);
			draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
			BattleEffect_LoadWork(
				HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, flags ^ GOUSEI_FLIP_X, GOUSEI_KASAN);
			draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
			if (frame > 32)
				y = y - frame * 2 + 64;
			draw[0](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
			if (frame <= 3)
				draw[1](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		}

		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *object =
				GetBattleObjectSlotFar(work->effect->actors[member])->object;
			s32 height = Battle_GetObjectTableValueFar(work->effect->actors[member]) * 2 / 3;

			if (frame == member * 8 + 80)
				AudioCommand_PlayFar(212);
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			// 対象の 2/3 の高さを中心にする
			point[0] = object->x;
			point[1] = height;
			point[2] = object->z;
			SceneTransform_ApplyPosition(point);
			if (frame == member * 8 + 48)
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, -1, member, 16);

			// 光の粒を回しながら、1 フレームに 2 ドットずつ中心へ寄せる
			if (frame > member * 8)
			{
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags, GOUSEI_HIKAKU);
				draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, flags, GOUSEI_KASAN);
				draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
				if (mode == 0)
				{
					SceneTransform_ApplyPitch(-frame << 10);
				}
				else if (mode == 1)
				{
				}
				else if (mode == 2)
				{
					SceneTransform_ApplyYaw(frame << 10);
				}
				else
				{
					SceneTransform_ApplyYaw(frame << 10);
					SceneTransform_ApplyRoll(frame << 10);
				}
				for (i = 0; i != 32; i++)
				{
					struct EffectStep *flare = &PARTICLES[member * 64 + i];

					if (frame > member * 8 + i)
					{
						s32 xx = (flare->x >> 8) * (flare->x >> 8);
						s32 yy = (flare->y >> 8) * (flare->y >> 8);
						s32 zz = (flare->z >> 8) * (flare->z >> 8);
						s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

						if (distance != 0)
						{
							s32 size;

							// 大きさ (幅の半分) は奥行き 314〜634 で 6〜1
							EffectPosition_ApplyBaseAndYOffset((s32 *)flare, &screen);
							screen.x = screen.x * 4 / 5 + shift;
							if (screen.depth < 314)
								screen.depth = 314;
							if (screen.depth > 634)
								screen.depth = 634;
							size = 6 - (screen.depth - 314) / 64;
							draw[1](canvas, work->sheet + BattleFx6_FlareCells[size - 1],
								screen.x - size, screen.y - size, size * 2, size * 2);
							flare->x -= flare->x / distance;
							flare->y -= flare->y / distance;
							flare->z -= flare->z / distance;
						}
					}
				}
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			}

			// 中心の画面の位置
			center[0] = 0;
			center[1] = 0;
			center[2] = 0;
			EffectPosition_ApplyBaseAndYOffset(center, &screen);
			screen.x = screen.x * 4 / 5 + shift;

			// はじける光：52 フレーム目から 4 フレームずつ 6 枚
			if (frame >= member * 8 + 52 && frame < member * 8 + 76)
			{
				s32 cel = (frame - member * 8 - 52) / 4 % 6;

				BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags, GOUSEI_HIKAKU);
				draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
				draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_BURST, screen.x - 20,
					screen.y - 20, 40, 40);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			}

			// 80 フレーム目から mode の絵
			if (mode == 0)
			{
				// 24x40 を 4 フレームずつ 7 枚
				if (frame >= member * 8 + 80 && frame < member * 8 + 108)
				{
					s32 cel = (frame - member * 8 - 80) / 4 % 7;

					BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags, GOUSEI_HIKAKU);
					draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
					draw[0](canvas, work->sheet + cel * 960 + SERIES_B_PICTURE, screen.x - 12,
						screen.y - 20, 24, 40);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
				}
			}
			else if (mode == 3 || mode == 5)
			{
				// 32x64 を 4 フレームずつ 6 枚
				if (frame >= member * 8 + 80 && frame < member * 8 + 104)
				{
					s32 cel = (frame - member * 8 - 80) / 4 % 6;

					BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags, GOUSEI_HIKAKU);
					draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
					draw[0](canvas, work->sheet + cel * 2048 + SERIES_B_PICTURE, screen.x - 16,
						screen.y - 32, 32, 64);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
				}
			}
			else if (mode == 4)
			{
				// 64x32 を 2 フレームずつ、上下を反転した下半分と合わせて 64x64
				if (frame >= member * 8 + 80 && frame < member * 8 + 104)
				{
					s32 cel = (frame - member * 8 - 80) / 2 % 6;

					BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags, GOUSEI_HIKAKU);
					draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
					BattleEffect_LoadWork(
						HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, flags | GOUSEI_FLIP_Y, GOUSEI_HIKAKU);
					draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
					draw[0](canvas, work->sheet + cel * 2048 + SERIES_B_PICTURE, screen.x - 32,
						screen.y - 24, 64, 32);
					draw[1](canvas, work->sheet + cel * 2048 + SERIES_B_PICTURE, screen.x - 32,
						screen.y + 8, 64, 32);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
				}
			}
			else
			{
				// 40x40 を 4 フレームずつ 6 枚、加算で
				if (frame >= member * 8 + 80 && frame < member * 8 + 104)
				{
					s32 cel = (frame - member * 8 - 80) / 4 % 6;

					BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags, GOUSEI_KASAN);
					draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
					draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_PICTURE, screen.x - 20,
						screen.y - 20, 40, 40);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
				}
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunPaletteRampMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunPaletteRamp(effect, 0);
}

void BattleFx_RunPaletteRampMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunPaletteRamp(effect, 1);
}

void BattleFx_RunPaletteRampMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RunPaletteRamp(effect, 2);
}

void BattleFx_RunPaletteRampMode3(struct BattleEffectArgument *effect)
{
	BattleFx_RunPaletteRamp(effect, 3);
}

#define RAMP_IMAGE 0x2580
#define RAMP_SIZE 696

void BattleFx_RunPaletteRamp(struct BattleEffectArgument *effect, s32 mode)
{
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 j;
	s32						 x_offset;
	DrawRectangle			*draw;
	struct BattleCamera		*camera;
	s32						 origin[3];
	struct EffectPosition	 screen;
	s32						 point[3];
	BlitterPair				 callbacks;
	s32						 i;
	s32						 palette;

	cursor = gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100;
	if (mode == 0)
		Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet, work->sheet, 1, 1);
	else
		Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet, work->sheet, 1, 1);
	if (mode == 0)
		palette = (s32)&ResourceId_PinkBurstSheet;
	else if (mode == 1)
		palette = (s32)&ResourceId_GlowOrbSheet;
	else
		palette = (s32)&ResourceId_PinkBurstSheet;
	Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(palette), 128);
	Resource_LoadAndDecompress((s32)&ResourceId_PaletteRampImage, work->sheet + RAMP_IMAGE, 0, 0);

	for (i = 1; i != 8; i++)
	{
		for (j = 0; j != RAMP_SIZE; j++)
		{
			s32 level = work->sheet[RAMP_IMAGE + j];

			if (level > 64 - i * 7)
				level = 64 - i * 7;
			if (level < 0)
				level = 0;
			work->sheet[RAMP_IMAGE + i * RAMP_SIZE + j] = level;
		}
	}

	if (work->effect->side == 1)
	{
		REG_BG2X = -0x7000;
		x_offset = -112;
	}
	else
	{
		REG_BG2X = 0;
		x_offset = 0;
	}

	for (i = 0; i != 512; i++)
	{
		struct EffectStep *shard = &PARTICLES[i];
		s32				   radius = 192;
		s32				   angle;

		angle = Random16() & 0xffff;
		shard->x = 0;
		if (mode == 0)
		{
			shard->y = ((i & 31) / 4 * 6 - 10) << 16;
			shard->z = (i % 4 * 2 - 2) << 16;
		}
		else
		{
			shard->y = ((i & 31) / 4 * 6 - 10) << 16;
			shard->z = (i % 4 * 8 - 16) << 16;
		}
		if (work->effect->side == 1)
			shard->velocity_x = 0x20000;
		else
			shard->velocity_x = -0x20000;
		shard->velocity_y = ((Trig_Cos(angle) * radius) >> 6) + 0x10000;
		shard->velocity_z = (Trig_Sin(angle) * radius) >> 6;
		shard->variant = Random16() & 0xff;
	}

	BattleFx_FetchRectangleBlitters(work->effect->side, draw = callbacks);
	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != work->effect->count * 4 + 64; frame++)
	{
		camera = gCameraWork;
		if (frame == 72)
			BattleEventRuntime_BeginPhaseFar(0);
		for (j = 0; j != work->effect->count; j++)
		{
			struct MotionObject *object;
			s32					 time;
			s32					 x;
			s32					 y;

			time = frame - j * 4;
			object = GetBattleObjectSlotFar(work->effect->actors[j])->object;
			if (time > 0)
			{
				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				point[0] = object->x;
				point[1] = 160 << 13;
				point[2] = object->z;
				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				SceneTransform_ApplyPosition(point);
				origin[0] = 0;
				origin[1] = 0;
				origin[2] = 0;
				EffectPosition_ApplyBaseAndYOffset(origin, &screen);
				x = screen.x + x_offset;
				y = screen.y;
				if (mode == 0)
				{
					if (time <= 26)
						callbacks[0](
							canvas, work->sheet + time / 4 % 7 * 960, x - 12, y - 20, 24, 40);
				}
				else
				{
					if (time <= 23)
						draw[1](canvas, work->sheet + time / 4 % 6 * 1600, x - 20, y - 20, 40, 40);
				}
				if (time == 24)
					AudioCommand_PlayFar(143);
				if (time >= 24 && time <= 60)
				{
					s32 level;

					level = 0;
					if (time > 28)
					{
						level = (time - 24) / 4;
						if (level > 7)
							level = 7;
					}
					for (i = 0; i != 24; i++)
					{
						struct EffectStep *shard = &PARTICLES[j * 32 + i];
						s32				   cell;

						cell = i % 4 * 3 + (shard->variant + time) / 8 % 3;
						EffectPosition_ApplyBaseAndYOffset((s32 *)shard, &screen);
						x = screen.x + x_offset;
						y = screen.y;
						callbacks[0](canvas,
							work->sheet + (level * RAMP_SIZE + PaletteRamp_ShardCells[cell])
								+ RAMP_IMAGE,
							x, y, PaletteRamp_ShardWidths[cell], PaletteRamp_ShardHeights[cell]);
						EffectStep_AdvanceWithGravity3D(shard, 60, 0);
					}
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

#define HONOO_KAZU 9 // 炎の数

//--------------------------------------------------------------------------
// 9 つの炎が半円の弧の上に 4 フレームおきに立ちのぼる（80 フレーム）
// variant 1 なら 16 ドット上にもうひとつ、variant 2 なら 32 ドット上にも
// 対象は 16 フレーム目から 8 フレームおきに当たる
//--------------------------------------------------------------------------
void BattleFx_RunPuffArc(struct BattleEffectArgument *efx)
{
	u32						*heap;
	u32						*cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	DrawRectangle			 draw;
	s32						 i;
	s32						 cell;
	u32						 w;
	s32						 sign;
	s32						 org;
	s32						 angle;
	s32						 tick;
	struct EffectStep		*puff;
	struct EffectStep		*p;

	// ワーク、キャンバス、BG1・BG2 を半透明に（14:16）
	heap = (u32 *)&gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap;
	work = (struct BattleEffectWork *)*cursor++;
	canvas = (void *)*cursor;
	work->effect = efx;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDCNT = 0x3f46;
	REG_BLDALPHA = 0x100e;
	Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1, 1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
	draw = (DrawRectangle)heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];

	// 弧：-90 度から 22.5 度ずつ、横の半径 32、縦の半径 16
	// 対象が 128 番以上なら左から、ほかは右から並べる
	if (work->effect->actors[0] > 127)
	{
		org = 0;
		sign = 1;
	}
	else
	{
		org = 64;
		sign = -1;
	}
	angle = -0x4000;
	i = 0;
	tick = 0;
	puff = work->particles;
	do
	{
		puff->x = ((sign * ((Trig_Sin(angle) << 5) >> 16)) + org) + 20;
		puff->y = ((Trig_Cos(angle) << 4) >> 16) + 40;
		puff->variant = tick;
		angle += 0x1000;
		tick -= 4;
		i += 1;
		puff++;
	} while (i != HONOO_KAZU);

	work->transfer_mode = 2;
	if (work->effect->variant == 2)
	{
		work->transfer_value = 75;
	}
	else
	{
		work->transfer_value = 50;
	}
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(136);
	frame = 0;
	do
	{
		if (frame == 24)
		{
			BattleEventRuntime_BeginPhaseFar(133);
		}

		// 炎の絵は 4 フレームごとに次へ、6 枚で消える
		i = 0;
		p = work->particles;
		do
		{
			if ((p->variant >= 0) && (p->variant <= 23))
			{
				cell = p->variant / 4;
				draw(canvas, work->sheet + PuffArc_CellSourceOffsets[cell],
					p->x - ((w = PuffArc_CellWidths[cell]) >> 1), p->y + PuffArc_CellBiasY[cell], w,
					PuffArc_CellHeights[cell]);
				if (work->effect->variant != 0)
				{
					draw(canvas, work->sheet + PuffArc_CellSourceOffsets[cell],
						p->x - ((w = PuffArc_CellWidths[cell]) >> 1),
						(p->y + PuffArc_CellBiasY[cell]) - 16, w, PuffArc_CellHeights[cell]);
				}
				if (work->effect->variant == 2)
				{
					draw(canvas, work->sheet + PuffArc_CellSourceOffsets[cell],
						p->x - ((w = PuffArc_CellWidths[cell]) >> 1),
						(p->y + PuffArc_CellBiasY[cell]) - 32, w, PuffArc_CellHeights[cell]);
				}
			}
			i += 1;
			p->variant += 1;
			p++;
		} while (i != HONOO_KAZU);

		i = 0;
		while (i != work->effect->count)
		{
			if (frame == (i * 8) + 16)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 10, 5, i, 12);
			}
			i += 1;
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame += 1;
	} while (frame != 80);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// values のうち current_value のある位置を中心に、前後 index_radius の
// 番号を out に並べ、その数を返す。0〜entry_limit - 1 の番号だけを入れる
//--------------------------------------------------------------------------
s32 BattleFx_BuildCenteredIndexList(BattleEffectIndexWindow *win, s16 *out)
{
	s32 len = win->index_radius * 2 + 1;
	s32 n = 0;
	s32 center = 0;
	s32 i;

	for (i = 0; i != len; i++)
	{
		if (win->current_value == win->values[i])
		{
			center = i;
			break;
		}
	}
	for (i = 0; i != len; i++)
	{
		s32 no = center + i - win->index_radius;

		if (no >= 0 && no < win->entry_limit)
		{
			out[n++] = no;
		}
	}
	return n;
}

// RockWall_Timings[variant] の並び
enum
{
	TIMING_FRAMES,		// 全体のフレーム数
	TIMING_RISE_FRAMES, // 柱が出ているフレーム数
	TIMING_SHAKE		// 画面の揺れ幅
};

struct RockWallHeights
{
	s32 column[4]; // 柱ごとの高さ (ドット)
};

#define HASHIRA_W 40   // 柱の幅 (ドット)
#define IWA_OFS 0x1e00 // sheet の中の岩の絵 (先頭 40x16 は背景の帯)

// 横 32 キャラの 8bpp キャラ並びの中で、(x, y) のドットの位置
#define BG_DOT(x, y) (((x) & 7) + (((x) / 8) << 6) + (((y) & 7) << 3) + (((y) / 8) << 11))

//--------------------------------------------------------------------------
// 岩の柱を左から 4 フレームおきに 3 本せり上げて沈め、上に乗った対象を
// 打ち上げる。柱の頭には背景から切り出した帯をのせる
// effect->variant で RockWall_Timings の行を選ぶ
//--------------------------------------------------------------------------
void BattleFx_RunRockWall(struct BattleEffectArgument *effect)
{
	void				   **slots;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 column;
	DrawRectangle			 draw;
	struct BattleCamera		*camera;
	s32						 x_offset;
	struct RockWallHeights	 heights;
	s32						 point[3];
	struct EffectPosition	 screen;
	s32						 member;
	s32						 row;
	s32						 col;
	s32						 y;

	slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = slots;
	work = *cursor++;
	canvas = *cursor;
	camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * 4);
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 横も等倍
	REG_BLDCNT = 0;	   // 半透明にしない
	Resource_LoadAndDecompress((s32)&ResourceId_RockWallSheet, work->sheet + IWA_OFS, 1, 1);

	// 背景のキャラ (VRAM 0x06008000 から 0x8000 バイト) を写し、
	// x 32〜71・y 96〜111 の帯を sheet の先頭へ切り出す
	Iwram_CopyWords((s32)Ram_MapCellBuffer, 0x06008000, 0x8000);
	for (row = 0; row != 16; row++)
	{
		y = row + 96;
		for (col = 0; col != HASHIRA_W; col++)
		{
			s32 pixel = BG_DOT(col + 32, y);

			work->sheet[row * HASHIRA_W + col] = Ram_MapCellBuffer[pixel];
		}
	}

	// 向こう側ならキャンバスを右へ 112 ドットずらし、対象の座標もそれに合わせる
	if (work->effect->side == 1)
	{
		REG_BG2X = -(112 << 8);
		x_offset = -112;
	}
	else
	{
		x_offset = 0;
	}
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP, GOUSEI_NURI);
	draw = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	for (member = 0; member != work->effect->count; member++)
		work->particles[member].variant = 0; // 着地したら 1
	work->transfer_mode = 1;				 // 転送のたびにキャンバスを 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	heights = (*(const struct RockWallHeights *)&RockWall_Heights);
	work->shake_frames = 128;
	AudioCommand_PlayFar(141);

	for (frame = 0; frame != RockWall_Timings[work->effect->variant][TIMING_FRAMES]; frame++)
	{
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame == RockWall_Timings[work->effect->variant][TIMING_FRAMES] - 16)
			BattleEventRuntime_BeginPhaseFar(133); // 終わりの 16 フレーム前に結果へ進む

		// 柱は 16・20・24 フレーム目から出る
		for (column = 0; column != 3; column++)
		{
			s32 start = column * 4 + 16;

			if ((frame & 31) + 32 == start) // 左辺は 32 以上なので成り立たない
				heights.column[column] += 32;
			if (frame >= start
				&& frame < start + RockWall_Timings[work->effect->variant][TIMING_RISE_FRAMES])
			{
				s32 height;
				s32 top;

				// 高さは sin で上がって下がる (1 フレームに 1/64 周)
				height = (heights.column[column] * Trig_Sin((frame - start) << 10)) >> 16;
				if (height < 0)
					height = -height;
				top = 112 - height;
				draw(canvas, work->sheet + IWA_OFS, column * HASHIRA_W + 8, top, HASHIRA_W, height);
				draw(canvas, work, column * HASHIRA_W + 8, 96 - height, HASHIRA_W, 16);

				// 柱の上にいる対象を打ち上げる
				for (member = 0; member != work->effect->count; member++)
				{
					struct MotionObject *object;

					object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
					point[0] = object->x;
					point[1] = object->y;
					point[2] = object->z;
					EffectPosition_ApplyBaseAndYOffset(point, &screen);
					screen.x += x_offset;
					if (screen.x >= column * HASHIRA_W + 8
						&& screen.x <= column * HASHIRA_W + 8 + HASHIRA_W && screen.y >= top)
					{
						object->velocity_y = 12 << 16; // 上へ 12 ドット/フレーム
						object->vertical_motion_strength = 0xab85;
					}
					if (object->y < 0)
						ObjectGroup_UpdateMembers(work->effect->actors[member], 0, 5, -1, 0);
				}
			}
		}

		// 落ちてきて着地した対象を、パレット 7 にして 5 フレーム後に戻す
		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *object;
			struct EffectStep	*state;

			object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
			state = &work->particles[member];
			if (state->variant == 0 && object->y <= 0 && object->velocity_y < 0)
			{
				state->variant = 1;
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, 5);
			}
		}
		Camera_ApplyShake(RockWall_Timings[work->effect->variant][TIMING_SHAKE],
			RockWall_Timings[work->effect->variant][TIMING_SHAKE]);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// homing.c
// バトル演出：使い手から飛び出した火の粉が対象を追って落ちる
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 火の粉を 1 フレームに 2 個ずつ飛ばし、出てから 48 フレーム後に対象へ向かわせる
// 地面に落ちた火の粉は着弾の絵を 12 フレーム出す
// effect : variant で火の粉の数 (HomingEmbers_Counts) を選ぶ
//--------------------------------------------------------------------------
void BattleFx_RunHomingEmbers(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	struct BattleCamera		*camera;
	u8						*sheet;
	s32						 top;
	struct EffectPosition	 screen;
	BlitterPair				 draw;
	struct MotionObject		*user;
	s32						 i;

	heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * 4);
	sheet = heap_cache[2];
	work->effect = effect;
	if (effect->side == 1)
		BattleFx_BeginCanvasLayer(1);
	else
		BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
	draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(
		HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER_ALTERNATE];
	REG_BLDALPHA = 0x1010;

	// 使い手の体の上端から、水平は乱数の向きと速さ、縦は -16〜111 (1/64 ドット単位) で飛び出す
	user = GetBattleObjectSlotFar(work->effect->actor)->object;
	top = user->y + Battle_GetObjectTableValueFar(work->effect->actor);
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *ember = &PARTICLES[i];
		s32				   angle;
		s32				   speed;

		angle = Random16();
		speed = (Random16() & 127) + 127;
		ember->velocity_x = (Trig_Sin(angle) * speed) >> 6;
		ember->velocity_y = (s32)(((Random16() & 127) - 16) << 16) >> 6;
		ember->velocity_z = (Trig_Cos(angle) * speed) >> 6;
		ember->x = user->x;
		ember->y = top;
		ember->z = user->z;
		ember->variant = -1; // -1 は飛行中、0 以上は着弾の絵の経過フレーム
	}

	work->camera_delta = 0;
	work->camera_phase = 0;
	Scheduler_AddOrUpdateCallback(Camera_ApplyPhasedDelta, 0x480);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != (HomingEmbers_Counts[work->effect->variant] >> 1) + 132; frame++)
	{
		if (frame > 16 && frame < 80)
			work->camera_delta = 0x100;
		else
			work->camera_delta = 0;
		if (frame == (HomingEmbers_Counts[work->effect->variant] >> 1) + 108)
			BattleEventRuntime_BeginPhaseFar(133);
		BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

		//------------------------------------------------------------------
		// 飛んでいる火の粉
		//------------------------------------------------------------------
		for (i = 0; i != HomingEmbers_Counts[work->effect->variant]; i++)
		{
			struct EffectStep *ember = &PARTICLES[i];
			s32				   start;

			start = i / 2;
			if (frame > start && ember->variant == -1)
			{
				s32 size;

				// 奥行き 160〜799 を大きさ 10〜1 に。最初の 48 フレームは比較、後は加算合成
				EffectPosition_ApplyBaseAndYOffset((s32 *)ember, &screen);
				screen.x >>= 1;
				if (screen.depth < 160)
					screen.depth = 160;
				if (screen.depth > 799)
					screen.depth = 799;
				size = 10 - (screen.depth - 160) / 64;
				draw[frame < start + 48 ? 1 : 0](canvas,
					sheet + ParticleStreams_CellOffsets[size - 1], screen.x - size / 2,
					screen.y - size, size, size * 2);
				ember->x += ember->velocity_x;
				ember->y += ember->velocity_y;
				ember->z += ember->velocity_z;
			}

			// 対象へ距離の 1/512 ずつ加速し、出てから 85 フレームまでは速度を 60/64 に抑える
			if (frame > start + 48 && ember->variant == -1)
			{
				struct MotionObject *target;
				s32					 vx;
				s32					 vy;
				s32					 vz;

				target =
					GetBattleObjectSlotFar(work->effect->actors[i % work->effect->count])->object;
				vx = ember->velocity_x += (target->x - ember->x) >> 9;
				vy = ember->velocity_y += (target->y - ember->y) >> 9;
				vz = ember->velocity_z += (target->z - ember->z) >> 9;
				if (frame < start + 85)
				{
					ember->velocity_x = vx * 60 / 64;
					ember->velocity_y = vy * 60 / 64;
					ember->velocity_z = vz * 60 / 64;
				}

				// 地面より下で着弾：画面位置を残して着弾の絵に切り替え、対象のパレットを
				// 4 フレーム 10 番に替えてアニメ 5 にし、画面を 2 フレーム揺らす
				if (ember->y < 0)
				{
					ember->variant = 0;
					ember->x = screen.x;
					ember->y = screen.y;
					AudioCommand_PlayFar(136);
					ObjectGroup_UpdateMembers(work->effect->actors[i % work->effect->count], 10, 5,
						i % work->effect->count, 4);
					work->shake_frames = 2;
				}
			}
		}

		//------------------------------------------------------------------
		// 着弾の絵 (2 フレームごとに進め、6 枚で終わる)
		//------------------------------------------------------------------
		for (i = 0; i != HomingEmbers_Counts[work->effect->variant]; i++)
		{
			struct EffectStep *ember = &PARTICLES[i];

			if (ember->variant >= 0 && ember->variant < 12)
			{
				s32 cel;
				u32 width;

				cel = ember->variant / 2;
				draw[1](canvas, work->sheet + HomingEmbers_FlareCells[cel],
					ember->x - ((width = HomingEmbers_FlareWidths[cel]) >> 1),
					ember->y + HomingEmbers_FlareBiasY[cel] - 56, width,
					HomingEmbers_FlareHeights[cel]);
				ember->variant++;
			}
		}

		if (work->camera_phase == 0)
			work->camera_phase = 1;
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Scheduler_RemoveCallback(Camera_ApplyPhasedDelta);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

// RisingMotes_Timings の 1 組 (variant ごと)
enum
{
	RISE_MOTES,	  // 昇る粒の数
	RISE_COLUMNS, // 柱の数
	RISE_FRAME	  // はじけるフレーム
};

//--------------------------------------------------------------------------
// 粒が渦を巻いて昇り、柱が 3 フレームおきに立ち並ぶ。
// RISE_FRAME で粒は広がって落ち、破片が跳ね、煙が上がる
// effect : variant が粒と柱の数、はじけるフレームを決める
//--------------------------------------------------------------------------
void BattleFx_RunRisingMotes(struct BattleEffectArgument *effect)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	u8						*dots;
	DrawRectangle			*draw;
	s32						 count;
	s32						 ymin;
	s32						 ymax;
	s32						 xyz[3];
	struct EffectPosition	 screen;
	s32						 point[3];
	BlitterPair				 pair;
	struct EffectStep		*mote;
	s32						 i;

	heap = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	dots = heap[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_EFFECT];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100;
	Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, dots, 0, 0);
	BattleFx_FetchRectangleBlitters(0, draw = pair);

	// 粒 : x が角度、z が半径 (56〜119)、y が高さ (-64〜-33 ドット)
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *seed = &work->particles[i];

		seed->x = Random16() & 0xffff;
		seed->z = (Random16() & 63) + 56;
		seed->y = ((Random16() & 31) - 64) << 16;
	}

	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	if (work->effect->side == 1)
		REG_BG2X = -0x7000;

	for (frame = 0; frame != RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME] + 75;
		frame++)
	{
		ymin = 120 << 16;
		ymax = 0;
		if (frame == RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME] + 11)
			BattleEventRuntime_BeginPhaseFar(132);
		point[0] = 0;
		point[1] = 0;
		point[2] = 512 << 16;
		Render_ResetTransformState();
		SceneTransform_ApplyPosition(point);
		if (frame >= 36 && frame < 64 && (frame & 3) == 0)
			AudioCommand_PlayFar(115);
		if (frame == 85)
			AudioCommand_PlayFar(136);
		for (i = 0; i != work->effect->count; i++)
		{
			if (frame == i * 4 + 40)
				ObjectGroup_UpdateMembers(work->effect->actors[i], 9, 5, -1, 0);
		}

		//------------------------------------------------------------------
		// 粒 : はじけるまでは半径を縮め、5 ドットずつ 48 ドットまで昇りながら回る。
		// はじけたあとは半径を広げて落ち、高さの上下限を求める
		//------------------------------------------------------------------
		if (frame < RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
			count = RisingMotes_Timings[work->effect->variant * 3 + RISE_MOTES];
		else
			count = 16;
		if (frame < RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME] + 35)
		{
			for (i = 0; i != count; i++)
			{
				if (frame > i)
				{
					s32 cel = i % 8;

					mote = &work->particles[i];
					if (mote->y < (48 - i / 2) << 16 && mote->y > -(48 << 16))
					{
						struct EffectPosition *pos;
						u32					   width;
						u32					   height;

						xyz[0] = mote->z * Trig_Sin(mote->x);
						xyz[1] = mote->y;
						xyz[2] = mote->z * Trig_Cos(mote->x);
						EffectPosition_ApplyBaseAndYOffset(xyz, pos = &screen);
						pos->x = (pos->x >> 17) + 64;
						pos->y = HI(pos->y) + 60;
						draw[1](canvas, work->sheet + RisingMotes_MoteCells[cel],
							pos->x - ((width = RisingMotes_MoteWidths[cel]) >> 1),
							pos->y - ((height = RisingMotes_MoteHeights[cel]) >> 1), width, height);
					}
					if (frame < RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
					{
						if (frame > i + 16)
						{
							if (mote->z > 4)
								mote->z -= 2;
							if (mote->y < 48 << 16)
								mote->y += 5 << 16;
							mote->x += 0x200;
						}
					}
					else
					{
						mote->z += 8;
						mote->y -= (i % 5 + 2) << 16;
						if (ymin > mote->y)
							ymin = mote->y;
						if (ymax < mote->y)
							ymax = mote->y;
					}
				}
			}
		}
		ymin += 64 << 16;
		ymax += 64 << 16;

		// 柱 (幅 32) : 36 フレーム目から 3 フレームおきに 1 本ずつ立てる
		if (frame < RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
		{
			for (i = 0; i != RisingMotes_Timings[work->effect->variant * 3 + RISE_COLUMNS]; i++)
			{
				if (i < (frame - 36) / 3)
				{
					s32 cel = i % 3;
					u32 height;

					if (frame >= RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME] - 7)
					{
						draw[1](canvas, work->sheet + RisingMotes_ColumnCells[cel],
							RisingMotes_ColumnSpots[i * 2],
							RisingMotes_ColumnSpots[i * 2 + 1]
								- (height = RisingMotes_ColumnHeights[cel]),
							32, height);
					}
					else
					{
						pair[0](canvas, work->sheet + RisingMotes_ColumnCells[cel],
							RisingMotes_ColumnSpots[i * 2],
							RisingMotes_ColumnSpots[i * 2 + 1]
								- (height = RisingMotes_ColumnHeights[cel]),
							32, height);
					}
				}
			}
		}

		// 破片 : はじけたとき、下の方から上へ 32 個を飛ばす (描くのは 24 個)
		if (frame == RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
		{
			for (i = 0; i != 32; i++)
			{
				struct EffectStep *chip = &PARTICLES[i];

				chip->x = (Random16() & 127) << 16;
				chip->y = ((Random16() & 15) + 80) << 16;
				chip->z = ((Random16() & 63) - 32) << 12;
				chip->velocity_y = ((-Random16() & 15) - 16) << 13;
				chip->variant = (Random16() & 15) + 16;
			}
		}
		if (frame >= RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
		{
			for (i = 0; i != 24; i++)
			{
				struct EffectStep *chip = &PARTICLES[i];

				if (chip->variant >= 0)
				{
					s32 cel = i % 8;

					draw[1](canvas, work->sheet + RisingMotes_EmberCells[cel], HI(chip->x),
						HI(chip->y), RisingMotes_EmberWidths[cel], RisingMotes_EmberHeights[cel]);
					chip->x += chip->velocity_x;
					chip->y += chip->velocity_y;
					chip->variant--;
				}
				if (ymin > chip->y)
					ymin = chip->y;
				if (ymax < chip->y)
					ymax = chip->y;
			}
		}
		ymin >>= 16;
		ymax >>= 16;
		if (ymax <= ymin)
			ymax = ymin + 1;

		// 煙 : 粒と破片の高さの範囲に出し、1 フレームに 1/2 ドット上がる
		if (frame == RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
		{
			for (i = 0; i != 32; i++)
			{
				struct EffectStep *puff = &work->particles[i];

				puff->velocity_x = (Random16() & 127) << 16;
				if (ymax == ymin)
					puff->velocity_y = ymin << 16;
				else
					puff->velocity_y = (Random16() % (ymax - ymin) + ymin) << 16;
				puff->variant = (Random16() & 15) + 20;
			}
		}
		if (frame >= RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
		{
			s32 rise = (frame - RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME]) / 2;

			for (i = 0; i != 32; i++)
			{
				struct EffectStep *puff = &work->particles[i];

				if (puff->variant >= 0 && puff->variant <= 17)
				{
					s32 cel = (17 - puff->variant) / 2;
					u32 size;

					draw[1](canvas, dots + BattleFx_PuffCells[cel],
						HI(puff->velocity_x) - ((size = BattleFx_PuffSizes[cel]) >> 1),
						HI(puff->velocity_y) - (size >> 1) - rise, size, size);
				}
				puff->variant--;
				if ((puff->variant == -1 || puff->variant == 17)
					&& frame < RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME] + 35)
				{
					puff->variant = 17;
					puff->velocity_x = (Random16() & 127) << 16;
					puff->velocity_y = (Random16() % (ymax - ymin) + ymin) << 16;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// bolts.c
// バトル効果：まわりから寄ってくる四本の稲妻と、飛び散る火花
//--------------------------------------------------------------------------

#define SPARK_COUNT 1024

#define BOLT_SHEET 0x60e // 稲妻の絵の置き場
#define BOLT_SIZE 2880	 // 24x120 の一枚
#define BOLT_COUNT 4

//--------------------------------------------------------------------------
// 四本の稲妻が回りながら中心へ寄り、根元から火花を散らす
// 16 フレーム描き、火花は 64 フレームまで動かす
//--------------------------------------------------------------------------
void BattleFx_RunClosingBolts(struct BattleEffectArgument *effect)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	u8						*sheet;
	s32						 bolt;
	s32						 cnt;
	BlitterPair				 draw;
	s32						 i;

	// ワーク、キャンバス、BG2 の拡大と位置
	heap = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap[2];
	work->effect = effect;
	BattleFx_BeginTiledCanvas(1);
	if (work->effect->variant == 2)
		REG_BG2PA = 0x80; // 横 2 倍
	else
		REG_BG2PA = 0x100;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP, GOUSEI_HIKAKU);
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP | GOUSEI_FLIP_X, GOUSEI_KASAN);
	draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, work->sheet + BOLT_SHEET, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	if (work->effect->variant == 2)
	{
		if (work->effect->side == 1)
			REG_BG2X = -0x1000;
		else
			REG_BG2X = 0x1000;
	}
	else
	{
		if (work->effect->side == 1)
			REG_BG2X = -0x8000;
	}

	// 火花の初期化
	for (i = 0; i != SPARK_COUNT; i++)
	{
		struct EffectStep *spark = &PARTICLES[i];
		s32				   speed;
		s32				   angle;

		speed = (Random16() & 0x3ff) + 256;
		angle = (Random16() & 0x7fff) - 0x4000;
		spark->x = 0x4000;
		spark->y = 0x7000;
		spark->z = (Trig_Sin(angle) * speed) >> 16;
		spark->velocity_z = -(Trig_Cos(angle) * speed * 2) >> 16;
		spark->variant = 0;
	}

	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(138);

	for (frame = 0; frame != 64; frame++)
	{
		if (frame == 20)
			BattleEventRuntime_BeginPhaseFar(133); // 結果の再生を始める
		if (frame < 16)
		{
			// 5 フレームごとにキャンバスを消す
			if (frame % 5 == 2)
				Iwram_FillWords(canvas, 0x4000, 0x10101010);
			for (bolt = 0; bolt != BOLT_COUNT; bolt++)
			{
				s32 angle;
				s32 x;
				s32 y;

				// 横の半径が 32 から 1 フレームに 1 ドットずつ縮む
				cnt = 0;
				angle = ((frame << 11) + 0x4000) * bolt;
				x = (((32 - frame) * Trig_Sin(angle)) >> 16) + 64;
				y = -((Trig_Cos(angle) << 3) >> 16) - 8;
				if (work->effect->variant == 0)
					draw[0](canvas, work->sheet + (Random16() & 3) * BOLT_SIZE + BOLT_SHEET,
						x + (Random16() & 7) - 16, y, 24, 120);
				else
					draw[bolt & 1](canvas, work->sheet + (Random16() & 3) * BOLT_SIZE + BOLT_SHEET,
						x + (Random16() & 7) - 16, y, 24, 120);

				// 根元から火花を出す
				for (i = 0; i != SPARK_COUNT; i++)
				{
					struct EffectStep *spark = &PARTICLES[i];

					if (spark->variant == 0)
					{
						s32 speed;
						s32 spread;

						speed = (Random16() & 0x1ff) + 128;
						spread = (Random16() & 0x7fff) - 0x4000;
						spark->x = x << 16;
						spark->y = (y + 112) << 16;
						spark->velocity_x = (Trig_Sin(spread) * speed) >> 9;
						spark->velocity_y = -(Trig_Cos(spread) * speed * 2) >> 7;
						spark->variant = (Random16() & 7) + 32;
						cnt++;
						if (cnt == LightningBolts_Sparks[work->effect->variant][1])
							break;
					}
				}
			}
			work->shake_frames = 1;
		}

		// 火花を動かし、下 (y 120) ではね返す
		for (i = 0; i != SPARK_COUNT; i++)
		{
			struct EffectStep *spark = &PARTICLES[i];

			if (spark->variant > 0)
			{
				spark->variant--;
				EffectStep_AdvanceWithGravity2D(spark, 60, -0x800);
				if (spark->y > 0x780000)
				{
					spark->velocity_y = -spark->velocity_y / 2;
				}
				else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0)
				{
					s32 x = spark->x >> 16;
					s32 y = spark->y >> 16;
					s32 size = spark->variant / 8 + 1;

					draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1], x - size / 2,
						y - size, size, size * 2);
				}
			}
		}

		// 対象は 4 フレームずつ遅れて当たる
		if (frame >= 4 && frame < 96)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame == i * 4 + 4)
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 10);
			}
		}
		Camera_ApplyShake(2, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunParticleFieldMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunParticleField(effect, 1);
}

void BattleFx_RunParticleFieldMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunParticleField(effect, 0);
}

enum
{
	FIELD_MOTES,
	FIELD_PUFFS,
	FIELD_FRAMES
};

void BattleFx_RunParticleField(struct BattleEffectArgument *effect, s32 mode)
{
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	/* FAKEMATCH: frame local: only draw[0] is used; the unused second blitter word fills
	   four bytes of the frame. Measured (2026-10-14): declared as one DrawRectangle, the
	   frame is 52 bytes for 56 (differ 20, frame offsets only). */
	BlitterPair			  draw;
	s32					  variant;
	struct EffectPosition screen;
	s32					  frame;
	s32					  i;

	cursor = gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 0);
	if (mode == 1)
	{
		for (i = 0; i != 64; i++)
		{
			s32 level = i / 2;

			BG_PLTT[i] = (level << 10) | (level << 5) | level;
		}
		variant = 1;
	}
	else
	{
		Iwram_CopyWords(
			(void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_FireStreakSheet), 128);
		variant = work->effect->variant;
	}

	for (i = 0; i != 32; i++)
	{
		struct EffectStep *puff = &work->particles[i];

		if (work->effect->side == 1)
			puff->x = 200 << 14;
		else
			puff->x = -(200 << 14);
		puff->y = 0;
		puff->z = 0;
		puff->velocity_x = ((Random16() & 63) - 32) << 13;
		puff->velocity_y = ((Random16() & 63) + 16) << 12;
		puff->velocity_z = ((Random16() & 63) - 32) << 13;
		puff->variant = 0;
	}
	for (i = 0; i != 1024; i++)
	{
		struct EffectStep *mote = &PARTICLES[i];

		if (work->effect->side == 1)
			mote->x = 200 << 14;
		else
			mote->x = -(200 << 14);
		mote->y = 0;
		mote->z = 0;
		mote->velocity_x = ((Random16() & 63) - 32) << 13;
		mote->velocity_y = ((Random16() & 31) + 8) << 13;
		mote->velocity_z = ((Random16() & 63) - 32) << 13;
		mote->variant = 0;
	}

	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != ParticleField_Counts[variant][FIELD_FRAMES]; frame++)
	{
		struct BattleCamera *camera;

		camera = gCameraWork;
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame == 2)
			AudioCommand_PlayFar(144);
		if (frame == ParticleField_Counts[variant][FIELD_FRAMES] - 48)
			BattleEventRuntime_BeginPhaseFar(133);

		for (i = 0; i != ParticleField_Counts[variant][FIELD_MOTES]; i++)
		{
			struct EffectStep *mote = &PARTICLES[i];

			if (mote->y >= 0)
			{
				s32 size;

				EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &screen);
				screen.x >>= 1;
				screen.x += work->effect->side * 32 - 16;
				if (screen.depth < 160)
					screen.depth = 160;
				if (screen.depth > 799)
					screen.depth = 799;
				size = 9 - (screen.depth - 160) / 64;
				draw[0](canvas,
					work->sheet + (ParticleStreams_CellOffsets[size - 1] + (i & 1) * 770) + 0x3200,
					screen.x - size / 2, screen.y - size, size, size * 2);
				EffectStep_AdvanceWithGravity3D(mote, 64, -0x2000);
			}
		}

		if (frame > 2)
		{
			for (i = 0; i != ParticleField_Counts[variant][FIELD_PUFFS]; i++)
			{
				struct EffectStep *puff = &work->particles[i];

				if (i < frame && puff->y >= 0)
				{
					EffectPosition_ApplyBaseAndYOffset((s32 *)puff, &screen);
					screen.x >>= 1;
					screen.x += work->effect->side * 32 - 16;
					if (puff->variant >= 0 && puff->variant <= 20)
					{
						s32 cel = puff->variant / 3;
						u32 width;

						draw[0](canvas, work->sheet + ParticleField_PuffCells[cel],
							screen.x - ((width = ParticleField_PuffSizes[cel]) >> 1),
							screen.y - (width >> 1), width, width);
					}
					if (puff->variant <= 20)
						puff->variant++;
					EffectStep_AdvanceWithGravity3D(puff, 64, -0x2000);
				}
			}
		}

		if (mode == 0)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame == i + 6)
				{
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 10);
					BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 2);
				}
			}
		}
		else
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame == i + 6)
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 10);
			}
		}
		if (frame == 2)
			work->shake_frames = 6;
		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_SpawnObjects(s32 entry_count, s32 kind, u32 variant)
{
	struct BattleEffectWork *work = gBattleFxWork[0];
	s32						 entry_index = 0;

	if (entry_count == 0)
	{
		return;
	}
	do
	{
		struct AnimationObject *object = ResourceObject_CreateFar(kind);

		work->objects[entry_index] = object;
		if (object != 0)
		{
			object->flags = 0;
			AnimationObjects_SelectAnimationFar(object, entry_index);
			((struct AnimationObject *)work->objects[entry_index])->part[0].priority = variant;
		}
		entry_index++;
	} while (entry_index != entry_count);
}

void BattleFx_RunNoEffect(void)
{
}

#define SHEET (work->sheet + 0xC56)

void BattleFx_ArmBg2AffineHBlankDma(void)
{
	struct BattleEffectWork *work = gBattleFxWork[0];
	volatile u16			*channel = REG_DMA0;
	channel[5] &= 0xc5ff;
	channel[5] &= 0x7fff;
	(void)channel[5];
	Dma_Set(work->bg2_x, (void *)0x04000028, 0xa6600001, (volatile u32 *)channel);
}

void BattleFx_RunSeriesIMode0(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 0);
}

void BattleFx_RunSeriesIMode2(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 2);
}

void BattleFx_RunSeriesIMode6(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 6);
}

void BattleFx_RunSeriesIMode3(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 3);
}

void BattleFx_RunSeriesIMode5(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 5);
}

void BattleFx_RunSeriesIMode7(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 7);
}

void BattleFx_RunSeriesIMode4(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 4);
}

void BattleEffect_RunBurstShower(struct BattleEffectArgument *efx, s32 mode)
{
	u32						*cache;
	struct BattleEffectWork *work;
	void					*dst;
	u8						*aux;
	BlitterPair				 blit;
	s32						 pos[3];
	s32						 seat[8][3];
	s32						 aim[3];
	u16						*pal;
	s32						 frame;
	s32						 i;
	s32						 pick;
	s32						 lum;
	s32						 id;
	s32						 width;
	s32						 height;

	cache = (u32 *)&gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	dst = (void *)cache[0];
	work = (struct BattleEffectWork *)cache[-1];
	aux = (u8 *)cache[1];
	work->effect = efx;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;

	if (mode == 7)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
		blit[0] = (DrawRectangle)cache[46 - 40];
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
		blit[1] = (DrawRectangle)cache[47 - 40];
	}
	else
	{
		BattleFx_FetchRectangleBlitters(work->effect->side, blit);
	}

	Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
	if (mode == 5)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetA, SHEET, 1, 1);
	}
	else if (mode == 7)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet, SHEET, 1, 1);
	}
	else
	{
		Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, SHEET, 1, 1);
		Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, aux, 0, 0);
		if (mode == 6)
		{
			for (i = 0, pal = (u16 *)0x05000000; i != 64; i++)
			{
				lum = i / 4;
				*pal = ((lum << 10) | (lum << 5)) | lum;
				pal++;
			}
			REG_BLDCNT = 0;
		}
		else
		{
			switch (mode)
			{
			case 0:
				id = (s32)&ResourceId_FlashBurstSheet;
				break;
			case 1:
				id = (s32)&ResourceId_IceBlockSheet;
				break;
			case 2:
				id = (s32)&ResourceId_WaterSpraySheet;
				break;
			case 3:
				id = (s32)&ResourceId_PinkPalette;
				break;
			case 4:
			default:
				id = (s32)&ResourceId_MarsDjinnSheet;
				break;
			}
			Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry(id), 128);
		}
	}

	if (mode == 7)
	{
		work->transfer_mode = 2;
		work->transfer_value = 50;
	}
	else
	{
		work->transfer_mode = 2;
		work->transfer_value = 75;
	}
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (i = 0; i != 64; i++)
	{
		work->particles[i].variant = -1;
	}

	EffectPosition_ApplyAnimationAndYOffset(work->effect->actor, pos);
	if (mode == 3)
	{
		pos[1] -= 16;
	}
	if (mode == 4)
	{
		if (work->effect->side == 1)
		{
			pos[0] += 28;
		}
		else
		{
			pos[0] -= 28;
		}
	}
	if (mode == 7)
	{
		if (work->effect->side == 1)
		{
			pos[0] += 16;
		}
		else
		{
			pos[0] -= 16;
		}
	}
	if (mode == 5)
	{
		pos[0] = pos[0] / 3;
		REG_BG2PA = 85;
	}

	i = 0;
	while (i != work->effect->count)
	{
		EffectPosition_ApplyStepAndYOffset(
			work->effect->actors[i], (struct EffectPosition *)seat[i]);
		i += 1;
	}

	frame = 0;
	do
	{
		pick = frame % work->effect->count;
		if (frame == 4)
		{
			AudioCommand_PlayFar(0x88);
		}
		if (mode != 6)
		{
			if (frame == 24)
			{
				BattleEventRuntime_BeginPhaseFar(0x86);
			}
		}
		else
		{
			if (frame == 60)
			{
				BattleEventRuntime_BeginPhaseFar(0x86);
			}
		}

		if (mode == 5)
		{
			if (work->effect->side == 1)
			{
				((DrawRectangle)blit[0])(
					dst, SHEET + ((frame / 3 % 3 * 9) << 9), pos[0] - 2, pos[1] - 32, 72, 62);
			}
			else
			{
				((DrawRectangle)blit[0])(
					dst, SHEET + ((frame / 3 % 3 * 9) << 9), pos[0] - 70, pos[1] - 32, 72, 62);
			}
		}
		else
		{
			aim[0] = (seat[pick][0] + (Random16() & 31)) - 16;
			aim[1] = (seat[pick][1] + (Random16() & 63)) - 16;
			if (frame <= 47)
			{
				work->particles[frame].x = pos[0] << 15;
				work->particles[frame].y = pos[1] << 16;
				work->particles[frame].velocity_x = (aim[0] - pos[0]) << 11;
				work->particles[frame].velocity_y = (aim[1] - pos[1]) << 11;
				work->particles[frame].variant = 0;
			}
		}

		i = 0;
		width = 32;
		height = 64;
		do
		{
			struct EffectStep *spark = &work->particles[i];
			if (spark->variant >= 0)
			{
				if (mode == 7)
				{
					if (spark->variant > 5)
					{
						((DrawRectangle)blit[work->effect->side])(dst, SHEET,
							((s16 *)&spark->x)[1] - 16, ((s16 *)&spark->y)[1] - 32, width, height);
					}
				}
				else if (mode == 4)
				{
					if (spark->variant > 5)
					{
						((DrawRectangle)blit[0])(dst, SHEET + ((spark->variant / 4) << 11),
							((s16 *)&spark->x)[1] - 16, ((s16 *)&spark->y)[1] - 32, width, height);
					}
				}
				else if (mode != 5)
				{
					if (spark->variant > 1)
					{
						((DrawRectangle)blit[0])(dst, SHEET + ((spark->variant / 4) << 11),
							((s16 *)&spark->x)[1] - 16, ((s16 *)&spark->y)[1] - 32, width, height);
					}
				}
				spark->x += spark->velocity_x;
				spark->y += spark->velocity_y;
				spark->variant += 1;
				if (spark->variant == 24)
				{
					spark->variant = -1;
				}
			}
			i += 1;
		} while (i != 64);

		if (mode == 5)
		{
			i = 0;
			while (i != work->effect->count)
			{
				if ((frame >= (i * 4) + 2) && ((frame & 7) == i))
				{
					work->shake_frames = 8;
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 4);
				}
				i += 1;
			}
		}
		else
		{
			i = 0;
			while (i != work->effect->count)
			{
				if ((frame >= (i * 4) + 16) && ((frame & 7) == i))
				{
					work->shake_frames = 8;
					if (mode == 6)
					{
						ObjectGroup_UpdateMembers(work->effect->actors[i], 14, 5, i, 4);
					}
					else
					{
						ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 4);
					}
					BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
				}
				i += 1;
			}
		}

		Camera_ApplyShake(4, 4);
		if (mode != 6)
		{
			ObjectGroup_TickMemberTimers();
		}
		work->transfer_pending = 1;
		WaitFrames(1);
		frame += 1;
	} while (frame != 64);

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunSwirlingStars(void *object)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 record[3];
	struct EffectPosition	 pos;
	BlitterPair				 draw;
	struct EffectStep		*star;
	s32						 i;
	s32						 frame;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = object;
	BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_StarDotSheet, work->sheet, 1, 1);
	BattleFx_FetchRectangleBlitters(work->effect->side ^ 1, draw);
	for (i = 0; i != 256; i++)
	{
		star = &PARTICLES[i];
		star->x = ((Random16() & 0xff) - 127) << 16;
		star->y = ((Random16() & 0xff) - 127) << 16;
		star->z = ((Random16() & 0xff) - 127) << 16;
		star->velocity_x = 0;
		star->velocity_y = 0;
		star->velocity_z = 0;
		star->variant = 0;
	}
	work->transfer_mode = 1;
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	record[0] = 0;
	record[1] = 160 << 15;
	record[2] = 0;
	for (frame = 0; frame != 160; frame++)
	{
		struct BattleCamera *camera;

		camera = gCameraWork;
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		SceneTransform_ApplyPosition(record);
		star = PARTICLES;
		for (i = 0; i != 64; i++, star++)
		{
			if (frame > i / 4 && star->variant == 0)
			{
				s32 size;

				Graphics_SaveTransferWorkOnce();
				switch (i & 3)
				{
				case 0:
					SceneTransform_ApplyYaw(frame * (i + 32) * 8);
					break;
				case 1:
					SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
					break;
				case 2:
					SceneTransform_ApplyRoll(-frame * (i + 32) * 8);
					break;
				case 3:
					SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
					SceneTransform_ApplyRoll(-frame * (i + 32) * 8);
					break;
				}
				EffectPosition_ApplyBaseAndYOffset((s32 *)star, &pos);
				pos.x >>= 1;
				Graphics_RestoreTransferWork();
				if (pos.depth < 250)
					pos.depth = 250;
				if (pos.depth > 0x27a)
					pos.depth = 0x27a;
				size = 9 - (pos.depth - 250) / 64;
				draw[0](canvas,
					work->sheet + ((i & 3) * 770 + ParticleStreams_CellOffsets[size - 1]),
					pos.x - size / 2, pos.y - size, size, size * 2);
				EffectStep_AdvanceWithGravity3D(star, 60, 0);
				if (frame > i / 4 + 30)
				{
					s32 dx = -star->x >> 8;
					s32 dy = -star->y >> 8;
					s32 dz = -star->z >> 8;

					star->velocity_x += dx;
					star->velocity_y += dy;
					star->velocity_z += dz;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunOrbitingSparks(void)
{
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 record[3];
	struct EffectPosition	 center;
	struct EffectPosition	 pos;
	s32						 frame;
	/* FAKEMATCH: needed to match: only draw[0] is used: the second word fills four bytes of the frame */
	BlitterPair				 draw;
	s32						 member;
	struct BattleCamera		*camera;
	u8						*palette;
	s32						 x_offset;
	s32						 i;

	work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
	canvas = WORK_SLOT(HEAP_SLOT_BATTLE_CANVAS);
	camera = WORK_SLOT(HEAP_SLOT_CAMERA);
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100;
	REG_BLDALPHA = 0x1010;
	palette = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
	Iwram_CopyWords(BG_PLTT, palette, 128);
	Resource_DecodeType01(palette + 128, work);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	work->transfer_mode = 1;
	work->transfer_value = 0;
	work->transfer_mode = 2;
	work->transfer_value = 75;
	draw[0] = (DrawRectangle)WORK_SLOT(HEAP_SLOT_BLITTER);
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	if (work->effect->side == 0)
		x_offset = 0;
	else
		x_offset = -112;
	/* FAKEMATCH: needed to match: a one-pass loop is a sched2 barrier, so the seed counter is set before its pointer 2026-10-11: without it differ 2 (movs r4, #0 falls behind the particle offset); i = 0 ahead of the store differs 170. */
	do
	{
		REG_BG2X = x_offset << 8;
	} while (0);
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *step = &work->particles[i];

		step->x = 0;
		step->y = 0;
		step->velocity_x = 0;
		step->z = 4;
	}
	for (frame = 0; frame != work->effect->count * 16 + 64; frame++)
	{
		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *member_object;
			u32					 window;

			member_object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
			EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &pos);
			window = frame - member * 16;
			pos.x += x_offset;
			if (window < 64)
			{
				/* Ordinary rec differs 118 in EN (265 native, 269 built).
				 * A generic output barrier before filling differs 155 (272
				 * built); after filling it differs 118 (267 built). Keeping
				 * rec live after the call differs 179 (270 built). None keeps
				 * the native loop layout. */
				/* FAKEMATCH: record is filled through r0 so loop.c cannot hoist its address into fp 2026-10-11: loop.c's second pass moves the invariant &record when 15 x savings x life 8 reaches the loop's insn count (about 100 after giv reduction); plain forms differ 118 (hoisted into fp, frame 92), loads before stores differ 12, and two one-pass blocks around the fill and the apply differ 2 (add r5, sp, #64 after the last store). */
				register s32 *rec asm("r0");

				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				rec = record;
				rec[0] = member_object->x;
				rec[1] = member_object->y;
				rec[2] = member_object->z;
				EffectPosition_ApplyBaseAndYOffset(rec, &center);
				/* FAKEMATCH: needed to match: sched2 barrier, so this store precedes the spark counter as in the reference 2026-10-11: without it differ 2 (movs r4, #0 moves ahead of the store). */
				do
				{
					center.x += x_offset;
				} while (0);
				for (i = 0; i != 3; i++)
				{
					struct EffectStep *spark = &work->particles[member * 3 + i];
					s32				   x;
					s32				   y;

					x = pos.x + ((Trig_Sin(spark->velocity_x + i * 0x5555) << 3) >> 16);
					y = pos.y + ((Trig_Cos(spark->velocity_x + i * 0x5555) << 3) >> 16);
					spark->velocity_x += 0x200;
					draw[0](canvas, work->sheet + i * 0x240, x - 12, y - 28, 24, 24);
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunTargetBursts(void *object)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	void					*palette;
	s32						 frame;
	void					*rectangle_b;
	void					*rectangle_a;
	struct EffectStep		*step;
	s32						 i;
	struct EffectPosition	 screen;
	s32						 record[3];

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = object;
	BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_TargetBurstImage, work->sheet + 0x1b00, 0, 0);
	palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
	rectangle_a = heap_cache[7];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
	rectangle_b = heap_cache[8];

	for (i = 0; i != 64; i++)
	{
		s32 angle;
		s32 magnitude;

		step = &work->particles[i];
		angle = Random16() & 0xffff;
		magnitude = Random16() & 0xff;
		step->x = 0;
		step->y = 0;
		step->z = 0;
		if (i % 6 == 5)
		{
			step->velocity_x = 0;
			step->velocity_y = 0;
		}
		else
		{
			step->velocity_x = (Trig_Sin(angle) * magnitude) >> 7;
			step->velocity_y = (Trig_Cos(angle) * magnitude) >> 9;
		}
		step->velocity_z = 0;
		step->variant = 0;
	}

	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != 96; frame++)
	{
		s32					 member;
		struct BattleCamera *camera;

		camera = gCameraWork;
		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *member_object;

			member_object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			record[0] = member_object->x;
			record[1] = 160 << 13;
			record[2] = member_object->z;
			SceneTransform_ApplyPosition(record);
			if (frame >= member * 8 && frame < member * 8 + 40)
			{
				struct EffectStep *p;

				for (i = 0; i != 6; i++)
				{
					s32 cel;

					p = &work->particles[member * 6 + i];
					cel = p->variant / 6;
					if (cel > 5)
						cel = 5;
					EffectPosition_ApplyBaseAndYOffset((s32 *)p, &screen);
					screen.x >>= 1;
					if (i == 5)
					{
						EffectStep_AdvanceWithGravity3D(p, 62, 0x800);
						((DrawRectangle)rectangle_a)(canvas, work->sheet + cel * 0x480 + 0x1b00,
							screen.x - 12, screen.y - 36, 24, 48);
					}
					else
					{
						EffectStep_AdvanceWithGravity3D(p, 60, 0x200);
						((DrawRectangle)rectangle_b)(canvas, work->sheet + cel * 0x480,
							screen.x - 12, screen.y - 36, 24, 48);
					}
					p->variant += 1;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

extern const Scale StagedParticles_UnitScale;

#define MATRICES (Ram_MapCellBuffer + 0x3800)

enum SlashSheetCell
{
	SLASH_FLASH_1 = 0x0000,
	SLASH_FLASH_2 = 0x0188,
	SLASH_FLASH_3_TOP = 0x057c,
	SLASH_FLASH_3_BOTTOM = 0x07d4,
	SLASH_FLASH_4_TOP = 0x0aaa,
	SLASH_FLASH_4_MIDDLE = 0x0c90,
	SLASH_FLASH_4_BOTTOM = 0x0e74,
	SLASH_FLASH_5_TOP = 0x1088,
	SLASH_FLASH_5_MIDDLE = 0x11f8,
	SLASH_FLASH_5_BOTTOM = 0x147c,
	SLASH_TOP = 0x16ac,
	SLASH_MIDDLE = 0x17fc,
	SLASH_BOTTOM = 0x1bf3
};

void BattleEffect_RunStagedParticles(struct BattleEffectArgument *effect)
{
	s32							 point[3];
	struct EffectPosition		 screen;
	s32							 position[4];
	struct EffectPosition		 target;
	BlitterPair					 draw;
	void					   **cache;
	struct BattleBackgroundView *presentation;
	void						*canvas;
	struct BattleEffectWork		*work;
	s8							*path;
	s32							 i;
	s32							 j;
	s32							 frame;
	u8							*sheet;
	s32							 scroll_x;
	s32							 path_x;
	s32							 path_y;
	s32							 row;
	s32							 column;

	cache = (void **)&gTransitionWork;
	presentation = cache[0];
	canvas = cache[-4];
	work = cache[-5];
	sheet = cache[-3];
	scroll_x = gBgScroll[1].x;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0x2000);
	REG_BG2PA = 0x100;
	BattlePres_ConfigureEffectDisplay();
	BG_PLTT[0] = 0;
	BG_PLTT[1] = 0;
	work->transfer_mode = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_WipeCanvas(0, 0);
	BattleFx_SelectLivingTargets(work->effect);
	BattleFx_SpawnObjects(9, 370, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_SlashSheet, work->sheet, 1, 1);
	Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_LimePalette), 128);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	path = Resource_GetTableEntry((s32)&ResourceId_StagedParticlePath);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
	draw[0] = cache[2];
	/* FAKEMATCH: needed to match: the one-pass block keeps the table address after the second read 2026-10-11: without the block differ 2 (mov r2, sp moves ahead of the first store). */
	do
	{
		draw[1] = cache[3];
	} while (0);
	gProjection.center_y = 240;
	WaitFrames(1);
	BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
	work->scroll_timer = 0;
	work->scroll_interval = 4;
	work->scroll_step_x = -1;
	work->scroll_step_y = 0;
	Scheduler_AddOrUpdateCallback(BattleFx_AdvanceScrollOnInterval, 0x480);
	presentation->busy = 1;
	BattleEffect_WipeCanvas(0, 1);
	REG_DISPCNT = 0x7741;
	REG_BG2PA = 0x80;
	REG_BLDALPHA = 0x1010;
	REG_BLDCNT = 0x3f44;
	path_x = 0;
	path_y = 0;
	for (i = 0; i != 16; i++)
	{
		struct EffectStep *slash = &work->particles[i];

		slash->x = (Random16() % 96 + 12) << 16;
		slash->y = ((Random16() & 63) + 32) << 16;
		slash->velocity_x = 0;
		slash->velocity_y = 0;
		slash->variant = 0;
		for (j = 0; j != 24; j++)
		{
			PARTICLES[i * 24 + j].x = (Random16() & 15) + 48;
			Render_ResetTransformState();
			SceneTransform_ApplyRoll(Random16() & 0xffff);
			SceneTransform_ApplyPitch(Random16() & 0xffff);
			SceneTransform_ApplyYaw(Random16() & 0xffff);
			Graphics_SaveTransferWork(MATRICES + (i * 24 + j) * 48);
		}
	}
	work->transfer_mode = 2;
	work->transfer_value = 50;
	REG_BG2CNT = 0x784;
	for (frame = 0; frame != 220 && !(gKeysRepeat & (KEY_A | KEY_B)); frame++)
	{
		if (frame <= 209)
		{
			if (frame == 0)
			{
				path_x = (path[0] << 8) + (u8)path[1];
				path_y = (path[2] << 8) + (u8)path[3];
				path += 4;
			}
			else
			{
				path_x += path[0];
				path_y += path[1];
				path += 2;
			}
			position[3] = 0;
			position[1] = 0xff0000;
			for (row = 0; row != 3; row++)
			{
				for (column = 0; column != 3; column++)
				{
					position[0] = (path_x + 80 + column * 32) << 16;
					position[2] = (64 - path_y + row * 32) << 16;
					Object_ApplyProjectedPlacementFar(
						work->objects[row * 3 + column], position, &StagedParticles_UnitScale, 0);
				}
			}
		}
		point[1] = 0;
		point[2] = 0;
		if (frame == 48)
		{
			work->fade_frames = 24;
			work->fade_step = 0;
		}
		for (i = 0; i != 16; i++)
		{
			s32 start = i * 8 + 64;

			if (frame >= start)
			{
				struct EffectStep *slash = &work->particles[i];
				s32				   x = HI(slash->x);
				s32				   y = HI(slash->y);

				if (frame == i * 8 + 84)
					AudioCommand_PlayFar(212);
				if (frame >= i * 8 + 85)
				{
					slash->x += slash->velocity_x;
					slash->y += slash->velocity_y;
					slash->velocity_x -= 0x10000;
					slash->velocity_y += 0x20000;
					draw[0](canvas, work->sheet + SLASH_TOP, x + 4, y - 40, 16, 21);
					draw[0](canvas, work->sheet + SLASH_MIDDLE, x - 16, y - 19, 29, 35);
					draw[0](canvas, work->sheet + SLASH_BOTTOM, x - 20, y + 16, 21, 24);
				}
				else if (frame >= i * 8 + 80)
				{
					switch (frame - start - 16)
					{
					case 0:
						draw[0](canvas, work->sheet + SLASH_FLASH_1, x - 7, y - 14, 14, 28);
						break;
					case 1:
						draw[0](canvas, work->sheet + SLASH_FLASH_2, x - 11, y - 22, 23, 44);
						break;
					case 2:
						draw[0](canvas, work->sheet + SLASH_FLASH_3_TOP, x - 4, y - 31, 20, 30);
						draw[0](canvas, work->sheet + SLASH_FLASH_3_BOTTOM, x - 16, y - 1, 22, 33);
						break;
					case 3:
						draw[0](canvas, work->sheet + SLASH_FLASH_4_TOP, x + 1, y - 38, 18, 27);
						draw[0](canvas, work->sheet + SLASH_FLASH_4_MIDDLE, x - 11, y - 11, 22, 22);
						draw[0](canvas, work->sheet + SLASH_FLASH_4_BOTTOM, x - 19, y + 11, 19, 28);
						break;
					case 4:
						draw[0](canvas, work->sheet + SLASH_FLASH_5_TOP, x + 4, y - 40, 16, 23);
						draw[0](canvas, work->sheet + SLASH_FLASH_5_MIDDLE, x - 10, y - 17, 23, 28);
						draw[0](canvas, work->sheet + SLASH_FLASH_5_BOTTOM, x - 20, y + 11, 20, 28);
						break;
					}
				}
				else
				{
					for (j = 0; j != 24; j++)
					{
						struct EffectStep *trail = &PARTICLES[i * 24 + j];

						if (trail->x > 0)
						{
							Graphics_LoadTransferWork(MATRICES + (i * 24 + j) * 48);
							point[0] = trail->x;
#if defined(TBS_EDITION_JA)
							{
								/* FAKEMATCH: the named result pointer is a pseudo of its own,
								 * which gives the Japanese frame its 124 bytes and keeps the
								 * trail counter j in a stack slot, as native. 2026-10-13: the
								 * plain &screen argument, as the other editions pass it,
								 * gives a 120-byte frame and j in r9 (tbs-ja differ 372). */
								struct EffectPosition *out = &screen;

								EffectPosition_ApplyBaseAndYOffset(point, out);
							}
							screen.x = (screen.x >> 1) + x;
							screen.y = screen.y + y;
#else
							EffectPosition_ApplyBaseAndYOffset(point, &screen);
							screen.x = (screen.x >> 1) + x;
							screen.y = screen.y + y + 16;
#endif
							trail->x -= 4;
							draw[1](canvas, sheet + ParticleStreams_CellOffsets[4], screen.x - 2,
								screen.y - 5, 5, 10);
						}
					}
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattleFx_AdvanceScrollOnInterval);
	presentation->busy = 0;
	gBgScroll[1].x = scroll_x;
	BattleEffect_SetupBlendedDisplay();
	for (i = 0; i != 9; i++)
		ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
	REG_BG2PA = 0x80;
	REG_DISPCNT = 0x7741;
	Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, Ram_MapCellBuffer, 1, 0);
	for (i = 0; i != 32; i++)
	{
		struct EffectStep *slash = &work->particles[i];
		s32				   member = i % 6;

		if (member < work->effect->count)
		{
			EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &target);
			slash->y = -((Random16() & 31) + 40);
			slash->x = target.x / 2 + (80 - slash->y) / 2;
		}
		else
		{
			slash->x = (Random16() & 63) + 80;
			slash->y = -((Random16() & 31) + 40);
		}
		slash->variant = -1;
	}
	for (frame = 0; frame != 88; frame++)
	{
		for (i = 0; i != 24; i++)
		{
			struct EffectStep *slash = &work->particles[i];

			if (frame >= i * 2 || frame > 40)
			{
				if (slash->variant >= 0)
				{
					if (slash->variant <= 23)
					{
						s32 cell = slash->variant / 4;
						u32 width;

						draw[i & 1](canvas, Ram_MapCellBuffer + PuffArc_CellSourceOffsets[cell],
							slash->x - ((width = PuffArc_CellWidths[cell]) >> 1) - 8,
							slash->y + PuffArc_CellBiasY[cell] - 40, width,
							PuffArc_CellHeights[cell]);
						if (slash->variant <= 11)
						{
							draw[0](canvas, work->sheet + SLASH_TOP, slash->x + 4, slash->y - 40,
								16, 21);
							draw[0](canvas, work->sheet + SLASH_MIDDLE, slash->x - 16,
								slash->y - 19, 29, 35);
						}
					}
					slash->variant++;
				}
				else
				{
					s32 height = 24;

					if (slash->y > 56)
						height = height - slash->y + 56;
					draw[0](canvas, work->sheet + SLASH_TOP, slash->x + 4, slash->y - 40, 16, 21);
					draw[0](
						canvas, work->sheet + SLASH_MIDDLE, slash->x - 16, slash->y - 19, 29, 35);
					if (height > 0)
						draw[0](canvas, work->sheet + SLASH_BOTTOM, slash->x - 20, slash->y + 16,
							21, height);
					slash->x -= 6;
					slash->y += 12;
					if (slash->y > 79)
					{
						s32 member;

						slash->variant = 0;
						work->shake_frames = 2;
						AudioCommand_PlayFar(134);
						member = i % 6;
						if (member < work->effect->count)
						{
							ObjectGroup_UpdateMembers(
								work->effect->actors[member], 7, 5, member, 8);
							BattleMotion_ApplyVariantMotionFar(work->effect->actors[member], 1);
						}
					}
				}
			}
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

//--------------------------------------------------------------------------
// columns.c
// バトル演出：地面から次々に伸びるつる・柱・とげと土けむり
//--------------------------------------------------------------------------
// 伸びる 1 本（struct EffectStep に重ねて使う）
struct DualColumn
{
	s32 x; // 根もと（キャンバスのドット）
	s32 y;
	u8	unknown_08[8];
	s32 height; // 伸びきったときの長さ
	u8	unknown_14[4];
	s32 start; // 伸び始めるフレーム
};

// PARTICLES : とげの土けむり

void BattleFx_RunDualTableMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunDualTable(effect, 0);
}

void BattleFx_RunDualTableMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunDualTable(effect, 1);
}

//--------------------------------------------------------------------------
// つる（mode 0）か緑の柱（mode 1）が、左右交互に 4 フレームおきに伸びる
// 本数は DualTable_ColumnCounts[variant]、根もとから土けむりが上がる
//--------------------------------------------------------------------------
void BattleFx_RunDualTable(struct BattleEffectArgument *effect, s32 mode)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	void					*rectangle[2];
	void					*second_slot;

	s32	   frame;
	s32	   duration;
	void **draw;
	s32	   cue_frame;
	s32	   fade_start;
	s32	   fade_end;
	s32	   start;

	u8 *age;

	struct DualColumn *seed;
	s32				   x;
	s32				   height;
	s32				   limit;

	s32				   puff_cells;
	struct DualColumn *column;
	s32				   length;
	s32				   reach;
	s8				   pillar_height;

	s32 n;
	s32 react_frame;

	s32				   i;
	struct EffectStep *puff;
	s32				   puff_age;
	s32				   image;

	heap = gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 横も等倍
	REG_BLDCNT = 0;
	if (mode == 1)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_GreenPillarSheet, work->sheet, 1, 1);
	}
	else
	{
		Resource_LoadAndDecompress((s32)&ResourceId_VineSheet, work->sheet, 1, 1);
	}
	if (work->effect->side == 1)
	{
		REG_BG2X = -0x7000; // 112 ドット右へ
	}
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	rectangle[0] = gWorkSlot[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_NURI);
	second_slot = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	/* FAKEMATCH: needed to match: the columns draw through a second pointer to the pair, whose address the frame keeps in a stack slot 2026-10-11: rectangle[side] at the columns differs 22 (the address slot of the pair moves from sp+28 to sp+16). */
	draw = rectangle;
	draw[1] = second_slot;

	duration = DualTable_ColumnCounts[work->effect->variant] * 4 + 56;

	// 土けむりをすべて空きにする
	age = (u8 *)gMapCellBuffer + 0x18;
	i = 0;
	do
	{
		i++;
		*(s32 *)age = -1;
		age += sizeof(struct EffectStep);
	} while (i != 1024);

	// 16 本の根もとと長さ
	i = 0;
	seed = (struct DualColumn *)work->particles;
	do
	{
		x = (DualTable_ColumnX[i] + (7 & Random16())) - 4;
		seed->y = i / 2 + 108;
		seed->x = x;
		height = (63 & Random16()) + 55;
		seed->height = height;
		limit = DualTable_HeightLimits[i % 3];
		if (limit < height)
		{
			seed->height = limit;
		}
		limit = i * 4 + 8;
		seed->start = limit;
		i++;
		seed++;
	} while (i != 16);

	work->transfer_mode = 1; // 毎フレーム 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	frame = 0;
	if (duration != 0)
	{
		cue_frame = duration - 64;
		fade_start = duration - 20;
		fade_end = duration - 4;
		do
		{
			if (frame == cue_frame)
			{
				BattleEventRuntime_BeginPhaseFar(132);
			}
			if (frame >= fade_start && frame < fade_end)
			{
				// 終わりの 16 フレームで薄くする
				REG_BLDCNT = 0x3f44;
				REG_BLDALPHA = ((duration - frame) - 5) | 0x1000;
			}
			if (frame < fade_end)
			{
				i = 0;
				if (DualTable_ColumnCounts[work->effect->variant] != 0)
				{
					do
					{
						column = (struct DualColumn *)((u8 *)work->particles
													   + i * sizeof(struct DualColumn));
						start = i * 4 + 8;
						if (frame == i * 4 + 9)
						{
							work->shake_frames = 2;
						}

						// 1 フレームに 8 ドットずつ伸ばす（奇数番は左右反転）
						if (frame > start)
						{
							u32 shape = i % 3;

							length = (frame - start) * 8;
							reach = column->height;
							if (length > reach)
							{
								length = reach;
							}
							if (mode == 0)
							{
								{
									s32 side = 1 & i;
									s32 cell = DualTable_VineCells[shape];
									shape = DualTable_VineWidths[shape];
									((DrawRectangle)draw[side])(canvas, work->sheet + cell,
										column->x - (shape >> 1), column->y - length, shape,
										length);
								}
							}
							else
							{
								pillar_height = DualTable_PillarHeights[7 & i];
								if (length > pillar_height)
								{
									length = pillar_height;
								}
								{
									s32 side = 1 & i;
									s32 cell = DualTable_PillarCells[shape];
									shape = DualTable_PillarWidths[shape];
									((DrawRectangle)draw[side])(canvas, work->sheet + cell,
										column->x - (shape >> 1), column->y - length, shape,
										length);
								}
							}
						}

						// 伸び始めて 4 フレーム目に対象が反応する
						n = 0;
						if (work->effect->count != 0)
						{
							react_frame = start + 4;
							do
							{
								if (frame == react_frame)
								{
									if (!(i & 1))
									{
										AudioCommand_PlayFar(133);
									}
									ObjectGroup_UpdateMembers(work->effect->actors[n], 7, 5, n, 3);
								}
								n++;
							} while (n != work->effect->count);
						}
						else
						{
							react_frame = start + 4;
						}

						// 4・8 フレーム目に、空きの土けむりをひとつ根もとに出す
						if ((frame == react_frame) || (frame == (start + 8)))
						{
							/* FAKEMATCH: needed to match: the scan reuses length as its puff pointer 2026-10-11: a block pointer of its own differs 48 (r5 and r6 swap); the puff pointer of the function 271. */
							length = (u32)gMapCellBuffer;
							n = 0;
							while (n != 512)
							{
								if (((struct EffectStep *)length)->variant == -1)
								{
									((struct EffectStep *)length)->x =
										((Random16() & 0xf) + column->x) - 8;
									((struct EffectStep *)length)->y = (Random16() & 0xf) + 80;
									((struct EffectStep *)length)->variant = 0;
									break;
								}
								length += sizeof(struct EffectStep);
								n++;
							}
						}
						i++;
					} while (i != DualTable_ColumnCounts[work->effect->variant]);
				}
			}

			// 土けむり：7 コマを 2 フレームずつ、左右対称に描く
			i = 0;
			puff = gMapCellBuffer;
			do
			{
				puff_age = puff->variant;
				if (puff_age >= 0)
				{
					image = puff_age / 2;
					puff_cells = 0x1e59;
					if (mode != 0)
					{
						puff_cells = 0xaff;
					}
					((DrawRectangle)rectangle[0])(canvas,
						work->sheet + (DualTable_PuffCells[image] + puff_cells),
						puff->x - DualTable_PuffWidths[image],
						puff->y - (s8)DualTable_PuffHeights[image] / 2, DualTable_PuffWidths[image],
						(s8)DualTable_PuffHeights[image]);
					((DrawRectangle)rectangle[1])(canvas,
						work->sheet + (DualTable_PuffCells[image] + puff_cells), puff->x,
						puff->y - (s8)DualTable_PuffHeights[image] / 2, DualTable_PuffWidths[image],
						(s8)DualTable_PuffHeights[image]);
					puff->variant++;
					if (puff->variant == 14)
					{
						puff->variant = -1;
					}
				}
				puff++;
				i++;
			} while (i != 512);

			Camera_ApplyShake(4, 4);
			ObjectGroup_TickMemberTimers();
			work->transfer_pending = 1;
			WaitFrames(1);
			frame++;
		} while (frame != duration);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 対象の列のまん中から、緑のつるが 16 本、2 フレームおきに伸びて縮む
//--------------------------------------------------------------------------
void BattleEffect_RunRisingColumns(struct BattleEffectArgument *effect)
{
	u32						*cache, *entry;
	struct BattleEffectWork *work;
	void					*canvas;
	BlitterPair				 draw;
	s32						 origin_x, origin_y;
	struct EffectPosition	 first, last;
	s32						 i, frame, cell, height, ofs, middle;
	struct EffectStep		*column;

	cache = (u32 *)(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	entry = cache;
	work = (struct BattleEffectWork *)*entry++;
	canvas = (void *)*entry;
	work->effect = effect;
	BattleFx_PrepareCanvasEffect(effect, 4, effect->side, 4, &origin_x, &origin_y);
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 横も等倍
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_GreenVineSheet, work->sheet, 1, 1);

	// 最初と最後の対象のまん中を、キャンバスの x = 64 に合わせる
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[work->effect->count - 1], &last);
	middle = first.x;
	middle += (last.x - middle) / 2;
	first.x = middle;
	REG_BG2X = (64 - first.x) << 8;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	draw[0] = (DrawRectangle)cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_NURI);
	draw[1] = (DrawRectangle)cache[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	column = work->particles;
	i = 0;
	do
	{
		column[i].x = RisingColumns_ColumnOffsets[i] + 64;
		i++;
	} while (i != 16);
	work->transfer_mode = 1; // 毎フレーム 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	frame = 0;
	do
	{
		if (frame == 32)
		{
			AudioCommand_PlayFar(143);
			for (i = 0; i != work->effect->count; i++)
				ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 16);
		}
		i = 0;
		column = work->particles;
		do
		{
			if (frame == i * 4 + 5)
				work->shake_frames = 2;

			// 1 フレームに 4 ドット伸びて 32 で止まり、i * 2 + 32 フレーム目から縮む
			ofs = i * 2;
			if (frame > ofs + 4)
			{
				cell = (frame / 4 + i) % 5; // 32x32 を 5 コマ
				if (frame < ofs + 32)
				{
					height = (frame - ofs) * 4 - 16;
					if (height > 32)
						height = 32;
				}
				else
				{
					height = 160 - (frame - ofs) * 4;
				}
				if (height > 0)
					draw[i & 1](canvas, work->sheet + (cell << 10), column->x - 16,
						(i & 7) - height + 104, 32, height);
			}
			i++;
			column++;
		} while (i != 16);
		Camera_ApplyShake(4, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	} while (frame != 70);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// とげが 8 フレームおきに地面から突き出し、根もとから土けむりが上がる
// 本数は BattleFxPillar_Counts[variant]、向きと大きさは BattleFxPillar_Kinds
//--------------------------------------------------------------------------
void BattleFx_RaisePillars(struct BattleEffectArgument *effect)
{
	void					*canvas;
	s32						 frame;
	DrawRectangle			 draw_flip;
	DrawRectangle			 draw;
	s32						 total;
	u32						*cache;
	u32						*cursor;
	struct BattleEffectWork *work;
	s32						 i;
	s32						 j;
	s32						 age;
	s32						 start;

	cache = (u32 *)(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	cursor = cache;
	work = (struct BattleEffectWork *)*cursor++;
	canvas = (void *)*cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 横も等倍
	REG_BLDCNT = 0;
	REG_BLDALPHA = 0x1010;
	Resource_LoadAndDecompress((s32)&ResourceId_ThornSheet, work->sheet, 1, 1);
	if (work->effect->side == 1)
	{
		REG_BG2X = -0x7000; // 112 ドット右へ
	}
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	draw = ((DrawRectangle *)cache)[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_NURI);
	work->transfer_mode = 1; // 毎フレーム 0 で消す
	work->transfer_value = 0;
	draw_flip = ((DrawRectangle *)cache)[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	total = BattleFxPillar_Counts[work->effect->variant] * 8 + 56;

	for (i = 0; i != 1024; i++)
	{
		PARTICLES[i].variant = -1; // 空き
	}

	for (frame = 0; frame != total; frame++)
	{
		if (frame == total - 64)
		{
			BattleEventRuntime_BeginPhaseFar(132);
		}
		if (frame >= total - 16)
		{
			// 終わりの 16 フレームで薄くする
			REG_BLDCNT = 0x3f44;
			REG_BLDALPHA = (total - frame - 1) | 0x1000;
		}

		for (i = 0; i != BattleFxPillar_Counts[work->effect->variant]; i++)
		{
			start = i * 8 + 8;
			age = frame - (i * 8 + 8);
			if (frame > start)
			{
				s32 h;
				s32 w;

				// 種類 0・1 は大きいとげ（48 幅）、2・3 は小さいとげ（32 幅）
				// 奇数は左右反転で、伸びるにつれて横へずれる
				if (BattleFxPillar_Kinds[i] < 2)
				{
					h = age * 16;
					w = age * 6;
					if (h > 80)
					{
						h = 80;
					}
					if (w > 30)
					{
						w = 30;
					}
					if (BattleFxPillar_Kinds[i] & 1)
					{
						draw_flip(canvas, work->sheet, BattleFxPillar_X[i] - w, 108 - h, 48, h);
					}
					else
					{
						draw(canvas, work->sheet, BattleFxPillar_X[i] + w, 108 - h, 48, h);
					}
				}
				else
				{
					h = age * 8;
					w = age;
					if (h > 64)
					{
						h = 64;
					}
					if (age > 8)
					{
						w = 8;
					}
					if (BattleFxPillar_Kinds[i] & 1)
					{
						draw_flip(
							canvas, work->sheet + 0xf00, BattleFxPillar_X[i] - w, 108 - h, 32, h);
					}
					else
					{
						draw(canvas, work->sheet + 0xf00, BattleFxPillar_X[i] + w, 108 - h, 32, h);
					}
				}
				if (frame == start + 1)
				{
					work->shake_frames = 3;
				}

				// 突き出して 2 フレームの間、毎フレーム土けむりをひとつ出す
				if (frame < start + 3)
				{
					s32				   y = (Random16() & 31) + 72;
					struct EffectStep *p;
					for (j = 0; j != 64; j++)
					{
						p = &PARTICLES[j];
						if (p->variant == -1)
						{
							p->x = BattleFxPillar_X[i] + (Random16() & 31) + 32;
							if (p->x > 96)
							{
								p->x = 96;
							}
							p->y = y;
							p->variant = 0;
							break;
						}
					}
				}
			}
			for (j = 0; j != work->effect->count; j++)
			{
				if (frame == start + 4)
				{
					AudioCommand_PlayFar(132);
					ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 3);
				}
			}
		}

		// 土けむり：7 コマを 2 フレームずつ、左右対称に描く
		for (i = 0; i != 64; i++)
		{
			if (PARTICLES[i].variant >= 0)
			{
				s32 n = PARTICLES[i].variant / 2;
				draw(canvas, work->sheet + BattleFxPillar_PuffCells[n],
					PARTICLES[i].x - BattleFxPillar_PuffWidths[n],
					PARTICLES[i].y - (s8)BattleFxPillar_PuffHeights[n] / 2,
					BattleFxPillar_PuffWidths[n], (s8)BattleFxPillar_PuffHeights[n]);
				draw_flip(canvas, work->sheet + BattleFxPillar_PuffCells[n], PARTICLES[i].x,
					PARTICLES[i].y - (s8)BattleFxPillar_PuffHeights[n] / 2,
					BattleFxPillar_PuffWidths[n], (s8)BattleFxPillar_PuffHeights[n]);
				if (++PARTICLES[i].variant == 14)
				{
					PARTICLES[i].variant = -1;
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

//--------------------------------------------------------------------------
// lightning_bolts.c
// 戦闘エフェクト：対象ごとに落ちる稲妻と、火花・光
//--------------------------------------------------------------------------
// PARTICLES : 対象ひとりに 128 個
// 光ごとに作りなおす 2 つめの描画関数
#define BLIT_ALT \
	((DrawRectangle *)gBattleFxWork)[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT]

//--------------------------------------------------------------------------
// 対象ごとに 8 フレームずつずらして稲妻を落とす
// effect : variant が稲妻の本数・火花の数・光の数と広がりを決める
//--------------------------------------------------------------------------
void BattleFx_RunLightningBolts(struct BattleEffectArgument *effect)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 i;
	s32						 frame;
	/* FAKEMATCH: frame local: only draw[0] is used; the unused second blitter word fills
	   four bytes of the frame. Measured (2026-10-14): declared as one DrawRectangle, the
	   frame is 60 bytes for 64 (differ 66, frame offsets only). */
	BlitterPair			  draw;
	u8					 *sprites;
	struct EffectPosition pos;
	s32					  j;

	heap = gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	sprites = heap[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_EFFECT];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw[0] = heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, work->sheet + 0xc56, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
	for (j = 0; j != 1024; j++)
		PARTICLES[j].variant = 0;
	for (j = 0; j != 64; j++)
		work->particles[j].variant = -1;
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(138);
	for (frame = 0; frame != work->effect->count * 8 + 40; frame++)
	{
		if (frame == 24)
			BattleEventRuntime_BeginPhaseFar(133);
		for (i = 0; i != work->effect->count; i++)
		{
			if (frame == i * 8)
				Iwram_FillWords(canvas, 0x4000, 0x10101010);
		}
		for (i = 0; i != work->effect->count; i++)
		{
			s32 start = i * 8;

			EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[i], &pos);
			pos.x /= 2;
			if (frame == start + 1)
				work->shake_frames = 4;
			if (frame == start + 4)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 6);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 6);
			}

			// 稲妻 (幅 24) : 1 フレームに 64 ドット伸び、104 ドットで止まる
			if (frame >= start && frame < start + 16)
			{
				s32 height = (frame - start) << 6;

				if (height > 104)
					height = 104;
				for (j = 0; j != LightningBolts_Counts[work->effect->variant * 4 + 3]; j++)
				{
					draw[0](canvas, work->sheet + (((i + frame + j) / 2) & 3) * 2880 + 0xc56,
						pos.x - 12, 0, 24, height);
				}

				// 足もと (y = 104) から上向き ±90 度へ火花を散らす
				if (frame == start + 2)
				{
					for (j = 0; j != LightningBolts_Counts[work->effect->variant * 4]; j++)
					{
						struct EffectStep *spark = &PARTICLES[i * 128 + j];
						s32				   speed = (Random16() & 0x1ff) + 64;
						s32				   angle = (Random16() & 0x7fff) - 0x4000;

						spark->x = pos.x << 16;
						spark->y = 104 << 16;
						spark->velocity_x = (Trig_Sin(angle) * speed) >> 5;
						spark->velocity_y = -(Trig_Cos(angle) * speed) >> 6;
						spark->variant = (Random16() & 7) + 32;
					}
				}
			}

			// 光 : 足もとから上のでたらめな位置に、でたらめな向き (左右・上下反転) で描く
			if (frame >= start + 2 && frame < start + 24)
			{
				for (j = 0; j != LightningBolts_Counts[work->effect->variant * 4 + 1]; j++)
				{
					s32 kind = j & 3;
					s32 drop = Random16() % LightningBolts_Counts[work->effect->variant * 4 + 2];
					s32 spread = LightningBolts_Counts[work->effect->variant * 4 + 2] - drop + 1;
					s32 y = pos.y - drop - (BattleFx_GlintCellHeights[kind] >> 1) + 8;
					u32 r = Random16();
					s32 x = pos.x;

					x += r % spread;
					x -= spread / 2;
					x -= (BattleFx_GlintCellWidths[kind] >> 1);

					BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
						LightningBolts_GlintPalettes[Random16() & 3] | GOUSEI_CLIP_X
							| GOUSEI_CLIP_Y,
						LightningBolts_GlintModes[work->effect->variant]);
					BLIT_ALT(canvas, work->sheet + BattleFx_GlintCellOffsets[kind], x, y,
						BattleFx_GlintCellWidths[kind], BattleFx_GlintCellHeights[kind]);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				}
			}
		}

		// 火花 : 地面で半分の速さで跳ね返り、残りが少ないほど小さく描く
		for (j = 0; j != 1024; j++)
		{
			struct EffectStep *spark = &PARTICLES[j];

			if (spark->variant > 0)
			{
				spark->variant--;
				EffectStep_AdvanceWithGravity2D(spark, 60, 0x1000);
				if (spark->y > 104 << 16)
				{
					spark->velocity_y = -spark->velocity_y / 2;
				}
				else if ((spark->x >= 0 && spark->x < 127 << 16) && spark->y >= 0)
				{
					s32 size = spark->variant / 16 + 1;
					s32 x = spark->x >> 16;
					s32 y = spark->y >> 16;

					draw[0](canvas, sprites + ParticleStreams_CellOffsets[size - 1], x - size / 2,
						y - size, size, size * 2);
				}
			}
		}
		Camera_ApplyShake(2, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_PrepareCanvasEffect(
	struct BattleEffectArgument *effect, s32 kind, s32 side, s32 anchor, s32 *out_x, s32 *out_y)
{
	s32						 base[3];
	s32						 goal[3];
	s32						 step[3];
	s32						 point[3];
	struct EffectPosition	 position;
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	BlitterPair				 draw;
	u8						*sheet;
	s32						 total;
	struct MotionObject		*target;
	struct MotionObject		*actor;
	u8						*palette;
	s32						 resource;
	s32						 frame;
	s32						 i;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	work->fade_frames = 24;
	work->fade_step = 0;
	if (kind > 3)
	{
		kind -= 4;
		total = 84;
	}
	else
	{
		total = 64;
	}
	switch (kind)
	{
	case 0:
		resource = (s32)&ResourceId_VenusDjinnSmallSheet;
		break;
	case 1:
		resource = (s32)&ResourceId_MercuryDjinnSmallSheet;
		break;
	case 2:
		resource = (s32)&ResourceId_MarsDjinnSmallSheet;
		break;
	default:
		resource = (s32)&ResourceId_JupiterDjinnSmallSheet;
		break;
	}
	palette = Resource_GetTableEntry(resource);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	Resource_DecodeType01(palette + 128, work);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	if (side == 1)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 3);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
	}
	else
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
	}
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	actor = GetBattleObjectSlotFar(work->effect->actor)->object;
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *spark = &PARTICLES[i];
		s32				   angle = Random16() & 0xffff;
		s32				   speed = (Random16() & 255) + 128;

		spark->x = 0;
		spark->y = ((Random16() & 31) + 20) << 16;
		spark->z = 0;
		spark->velocity_x = (Trig_Sin(angle) * speed) >> 5;
		spark->velocity_y = 0;
		spark->velocity_z = (Trig_Cos(angle) * speed) >> 5;
		spark->variant = 0;
	}
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	base[0] = actor->x;
	base[1] = 0;
	base[2] = actor->z;
	switch (anchor)
	{
	case 0:
		goal[0] = target->x;
		goal[1] = 0x3c0000;
		goal[2] = target->z;
		break;
	case 1:
		goal[0] = target->x;
		goal[1] = 0x3c0000;
		goal[2] = 0;
		break;
	case 2:
		goal[0] = actor->x;
		goal[1] = 0x3c0000;
		goal[2] = actor->z;
		break;
	case 3:
		goal[0] = actor->x;
		goal[1] = 0x3c0000;
		goal[2] = 0;
		break;
	case 4:
		goal[0] = 0;
		goal[1] = 0x3c0000;
		goal[2] = 0;
		break;
	}
	step[0] = (goal[0] - base[0]) / 40;
	step[1] = (goal[1] - base[1]) / 40;
	step[2] = (goal[2] - base[2]) / 40;
	for (frame = 0; frame != total; frame++)
	{
		struct BattleCamera *camera = gCameraWork;

		if (frame > 75)
			REG_BLDALPHA = (168 - frame * 2) | 0x1000;
		if (frame == 8)
			AudioCommand_PlayFar(212);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame >= 6 && frame <= 45)
		{
			base[0] += step[0];
			base[1] += step[1];
			base[2] += step[2];
		}
		SceneTransform_ApplyPosition(base);
		if (frame == 0)
			ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
		if (frame == 24)
			ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
		for (i = 0; i != 32; i++)
		{
			struct EffectStep *spark = &PARTICLES[i];

			if (frame >= i / 8 && spark->variant == 0)
			{
				s32 size;

				Graphics_SaveTransferWorkOnce();
				switch (i & 3)
				{
				case 0:
					SceneTransform_ApplyYaw(frame * (i * 32 + 256));
					break;
				case 1:
					SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
					break;
				case 2:
					SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
					break;
				case 3:
					SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
					SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
					break;
				}
				EffectPosition_ApplyBaseAndYOffset(&spark->x, &position);
				position.x >>= 1;
				Graphics_RestoreTransferWork();
				if (position.depth < 250)
					position.depth = 250;
				if (position.depth > 634)
					position.depth = 634;
				size = 8 - (position.depth - 250) / 64;
				draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					position.x - size / 2, position.y - size, size, size * 2);
				EffectStep_AdvanceWithGravity3D(spark, 60, 0);
				if (frame >= i / 8 + 24)
				{
					s32 pull_x = -spark->x >> 7;
					s32 pull_y = -spark->y >> 7;
					s32 pull_z = -spark->z >> 7;

					spark->velocity_x += pull_x;
					spark->velocity_y += pull_y;
					spark->velocity_z += pull_z;
					spark->velocity_x = spark->velocity_x * 62 / 64;
					spark->velocity_y = spark->velocity_y * 62 / 64;
					spark->velocity_z = spark->velocity_z * 62 / 64;
					if ((pull_x > -2048 && pull_x < 2048) && (pull_z > -2048 && pull_z < 2048))
						spark->variant = -1;
				}
			}
		}
		if (frame >= 54 && frame <= 69)
		{
			point[0] = Trig_Sin(frame << 10) << 2;
			point[1] = 0;
			point[2] = 0;
			EffectPosition_ApplyBaseAndYOffset(point, &position);
			*out_x = position.x;
			*out_y = position.y;
			position.x >>= 1;
			draw[0](canvas, work, position.x - 10, position.y - 20, 20, 40);
		}
		if (frame == 64)
		{
			for (i = 0; i != 64; i++)
			{
				struct EffectStep *mote = &work->particles[i];
				s32				   angle = Random16() & 0xffff;
				s32				   speed = (Random16() & 255) + 128;

				mote->x = *out_x << 15;
				mote->y = *out_y << 16;
				mote->velocity_x = (Trig_Sin(angle) * speed) >> 6;
				mote->velocity_y = (Trig_Cos(angle) * speed) >> 5;
				mote->variant = (Random16() & 15) + 8;
			}
		}
		if (frame > 63)
		{
			for (i = 0; i != 64; i++)
			{
				struct EffectStep *mote = &work->particles[i];

				if (mote->variant >= 0)
				{
					s32 size = (mote->variant >> 3) + 2;

					draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
						HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
					EffectStep_AdvanceWithGravity2D(mote, 60, 0);
					mote->variant--;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(Palette_StepFadeTransfer);
	Iwram_ClearWords((void *)0x06004000, 0x4000);
	Iwram_ClearWords(canvas, 0x4000);
	REG_BLDALPHA = 0x1010;
}

//--------------------------------------------------------------------------
// projectile_volley.c
// バトル効果：BattleFx_RunProjectileVolley の型ごとの入り口
//--------------------------------------------------------------------------

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
// 使う者を mode 2 にし、アニメを速めているあいだに型 mode の弾を放つ
//--------------------------------------------------------------------------
static __inline__ void RunVolleyWithAction(void *effect, s32 mode)
{
	s32 obj;

	obj = (s32)GetBattleObjectSlotFar(ARG(effect)->actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_HAYAI);
	BattleFx_RunProjectileVolley(effect, mode);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_FUTSU);
}

//--------------------------------------------------------------------------
// 型 5 : 使う者を mode 2 にし、アニメを速めているあいだに放つ
//--------------------------------------------------------------------------
void BattleEffect_RunMode5WithAction(void *effect)
{
	RunVolleyWithAction(effect, 5);
}

//--------------------------------------------------------------------------
// 型 6 : 型 5 と同じく、アニメを速めて放つ
//--------------------------------------------------------------------------
void BattleFx_RunMode6WithAction(void *effect)
{
	RunVolleyWithAction(effect, 6);
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
	RunVolleyWithAction(effect, 9);
}

//--------------------------------------------------------------------------
// approach.c
// バトル効果：対象へ飛ぶ弾の一斉射撃と、キャラの移動
//--------------------------------------------------------------------------
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
#define PTCL_MAX 1024

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
	BlitterPair				 draw;
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
	heap = gBattleFxWork;
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
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];

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
			PARTICLES[i].variant = 0;
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
				draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
				if (frame <= 3)
					draw[1](canvas, work->sheet + 0x65c0, x, y, 20, 40);
				draw[1](canvas, work->sheet + 0x65c0, x, y, 20, 40);
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
								struct EffectStep *p = &PARTICLES[k];

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
								 * English instructions and changes 77 rows. 2026-10-11: one test after an if/else that sets hit differs 65.
								 * 2026-10-13: the ?: of both sign tests 65 (or 77 tested whole),
								 * hit preset 0 and set by nested ifs 53, tests on shot->x 65; these
								 * guards with one test after the if/else 55. The per-side test and
								 * its goto are the native layout. */
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
								struct EffectStep *flash = &PARTICLES[index];

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
									struct EffectStep *p = &PARTICLES[k];

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
								draw[0](canvas, work->sheet + frame % 6 * 768, pos->x - 16,
									pos->y - 12, 32, 24);
						}
						else if (mode == 7 || mode == 9 || mode == 10)
						{
							s32 cell = j & 3;
							draw[0](canvas, work->sheet + Volley_PelletOffsets[cell], pos->x - 4,
								pos->y - 4, 8, 8);
						}
						else if (mode == 2)
						{
							draw[0](
								canvas, work->sheet + j % 6 * 128, pos->x - 4, pos->y - 8, 8, 16);
						}
						else if (mode == 3)
						{
							draw[0](canvas, work->sheet + 96, pos->x - 9, pos->y - 7, 18, 13);
						}
						else if (mode == 4)
						{
							draw[0](canvas, work, pos->x - 6, pos->y - 4, 12, 8);
						}
						else if (mode == 11)
						{
							draw[0](canvas, work->sheet + 330, pos->x - 15, pos->y - 12, 29, 23);
						}
						else
						{
							draw[0](canvas, work, pos->x - 20, pos->y - 32, 40, 64);
						}
					}
				}
			}

			// 当たりの光：12 フレーム。半分の絵を、反転してもう半分描く (奇数は縦横を入れ替えた絵)
			if (mode == 0 || mode == 5 || mode == 8)
			{
				for (i = 0; i != work->effect->count * shots; i++)
				{
					struct EffectStep *flash = &PARTICLES[i];

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
				struct EffectStep *p = &PARTICLES[i];

				if (p->variant > 0)
				{
					s32 size = (p->variant >> 3) + 1;

					draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
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
				struct EffectStep *p = &PARTICLES[i];

				if (p->variant > 0)
				{
					s32 size = (p->variant >> 4) + 1;

					draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
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
void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 unused)
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

#define TSUBU_MAX 64	// 粒の数
#define RING_SIZE 0x3c0 // 輪の一枚 (20x48)

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
	/* FAKEMATCH: needed to match: the blitter/camera declarations and independent store order preserve spill slots and loop preheader order 2026-10-11: the camera read is the plain slot now; Ram_MapCellBuffer at the two uses of cells differs 2 (its load moves ahead of lsls r3 at the packing call). */
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	BlitterPair				 draw;
	struct BattleCamera		*camera;
	u8						*sheet;
	struct EffectStep		*p;
	struct EffectStep		*step;
	struct EffectPosition	 origin;
	struct EffectPosition	 screen;
	struct EffectPosition	 pos;
	struct MotionObject		*tgt;
	s32						 frame;
	s32						 n;
	s32						 size;
	s32						 t;
	s32						 pal;
	u8						*cells;

	// ワーク、キャンバス、カメラ
	cells = Ram_MapCellBuffer;
	heap = gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	camera = heap[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
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
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];

	// 絵と色
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1, 0);
	Graphics_PackTileRows((u8 *)work, (u8 *)cells, 40, 288);
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
	BlitterPair				 blit;
	DrawRectangle			*draw;
	struct EffectStep		*flame;
	struct EffectStep		*mote;
	u8						*cells;
	s32						 i;
	s32						 j;

	heap_cache = gBattleFxWork;
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
		PARTICLES[i].variant = -1;
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

						mote = &PARTICLES[i * 32 + j];
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
			mote = &PARTICLES[i];
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
	BlitterPair				 blit;
	DrawRectangle			*draw;
	struct EffectStep		*ball;
	struct EffectStep		*mote;
	s32						 frame;
	s32						 i;
	s32						 j;
	s32						 angle;

	heap_cache = gBattleFxWork;
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
		PARTICLES[i].variant = -1;
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

						mote = &PARTICLES[i * 32 + j];
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
			mote = &PARTICLES[i];
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
		Iwram_CopyWords((void *)0x05000000, res, 128);
	if (skip_pal != 0)
		res += 128;
	Resource_DecodeType01(res, dst);
}

void BattleFx_RunVenusDjinnColumns(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	BlitterPair				 draw;
	u8						*sheet;
	struct EffectStep		*step;
	struct EffectStep		*mote;
	s32						 i;
	s32						 j;
	s32						 angle;

	heap_cache = gBattleFxWork;
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
		PARTICLES[i].variant = -1;
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

						mote = &PARTICLES[i * 32 + j];
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
			mote = &PARTICLES[i];
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
	BlitterPair				 callbacks;
	DrawRectangle			*draw;
	struct EffectStep		*step;
	struct EffectStep		*mote;
	s32						 member;
	s32						 frame;
	s32						 i;
	s32						 curtain_y;

	heap_cache = gBattleFxWork;
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
			mote = &PARTICLES[member * 16 + i];
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
		step = PARTICLES;
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
#define PARTICLE_COUNT 64

void BattleFx_RunParticleReveal(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 spawn[3];
	s32						 screen_y;
	s32						 screen_x;
	BlitterPair				 routine;
	struct EffectStep		*p;
	s32						 i;
	s32						 frame;
	s32						 rise;

	heap_cache = gBattleFxWork;
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
	BlitterPair				 draw;
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
	draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
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
				((DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER_ALTERNATE])(
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
						draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)
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

	work = gBattleFxWork[0];
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

extern const s16 Mode12_Points[][2];
extern u8  Mode12_Animations[];
extern u8  Mode12_SmokeFlips[];

// Ram_MapCellBuffer + 0xc58 から 910 個（その前には絵を読み込む）
#define MODE12_PARTICLES ((struct EffectStep *)(Ram_MapCellBuffer + 0xc58))

void BattleFx_InitializeMode12(struct BattleEffectArgument *efx)
{
	u8						 ramp[32];
	s32						 vec[3];
	struct EffectPosition	 pts[11];
	s32						 pos[3];
	s32						 scale[3];
	void				   **slots;
	s32						*ctrl;
	struct BattleEffectWork *work;
	void					*dst;
	u8						*aux;
	s32						 frame;
	/* FAKEMATCH: frame local: only blit[0] is used; the unused second blitter word fills
	   four bytes of the frame. Measured (2026-10-14): declared as one DrawRectangle, the
	   frame is 332 bytes for 336 (differ 202, frame offsets only). */
	BlitterPair		   blit;
	s32				   size;
	s32				   count;
	s32				   i;
	s32				   j;
	s32				   n;
	struct EffectStep *p;
	DrawRectangle	  *cache;
	s32				   rings = 0x2710;

	slots = &gWorkSlot[HEAP_SLOT_BATTLE_BACKGROUND];
	ctrl = slots[0];
	work = slots[-5];
	dst = slots[-4];
	aux = slots[-3];
	work->effect = efx;
	BattleFx_BeginCanvasLayer(0x2000);
	REG_BG2PA = 0x100;
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
	PLTT_BACKDROP = 0;
	*(u16 *)0x05000002 = 0;
	REG_BG2CNT = 0x2784;
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
	REG_BG2PA = 0x80;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
	cache = (DrawRectangle *)gWorkSlot;
	blit[0] = cache[46];
	REG_DISPCNT = 0x7741;
	REG_BG2PA = 0x80;
	REG_BLDALPHA = 0x100f;
	REG_BLDCNT = 0x3f44;
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
	REG_BG2CNT = 0x784;

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
				EffectPosition_ApplyBaseAndYOffset(vec, &pts[i]);
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
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
				blit47 = gBlitters[HEAP_SLOT_BLITTER_ALTERNATE];
			}
			else
			{
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
				blit47 = gBlitters[HEAP_SLOT_BLITTER_ALTERNATE];
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
				EffectPosition_ApplyBaseAndYOffset(vec, &pts[0]);
				pts[0].x = (pts[0].x >> 17) + 64;
				pts[0].y = (pts[0].y >> 16) + 60;
				blit47(dst,
					work->sheet + (lvl * 0x302 + ParticleStreams_CellOffsets[s - 1]) + rings,
					pts[0].x - s / 2, pts[0].y - s, s, s * 2);
			}
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
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
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
			gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](dst, work, 32, (frame - 151) * 20 - 12, 32, 96);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
			gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](dst, work, 64, (frame - 151) * 20 - 12, 32, 96);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
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
	REG_BLDALPHA = 0x1010;
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
			MODE12_PARTICLES[i].variant = 0;
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
						for (i = 0, p = MODE12_PARTICLES; i != 910; i++, p++)
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
							BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
							gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](
								dst, work->sheet + i % 3 * 0x1440, rx, ry, 72, 72);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
						}
						if (frame == i * 2 + 80)
						{
							s32 n = 0;
							for (j = 0, p = MODE12_PARTICLES; j != 910; j++, p++)
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
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
				gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](
					dst, work->sheet + 0x6000, ox + 60, oy - 24, 12, 24);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
				gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](
					dst, work->sheet + 0x6000, sx, oy - 24, 12, 24);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 11, 2);
				gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](
					dst, work->sheet + 0x6000, ox + 60, oy, 12, 24);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 15, 2);
				gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](dst, work->sheet + 0x6000, sx, oy, 12, 24);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				j = 0;
				for (i = 0, p = MODE12_PARTICLES; i != 910; i++, p++)
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

			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 15, 2);
			{
				DrawRectangle	   draw = gBlitters[HEAP_SLOT_BLITTER_ALTERNATE];
				struct EffectStep *q;
				for (i = 0, q = MODE12_PARTICLES; i != 910; i++, q++)
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
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);

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
					BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
						3 | Mode12_SmokeFlips[Random16() & 3], 2);
					gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](dst,
						Ram_MapCellBuffer + BattleFx_GlintCellOffsets[k], x, y + 56,
						BattleFx_GlintCellWidths[k], BattleFx_GlintCellHeights[k]);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
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

		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
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
	DrawRectangle			 blit;
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

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = object;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100;
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 1);
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
			struct EffectStep *seed = &PARTICLES[i * 21 + j];

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
					shard = &PARTICLES[i * 21 + j];
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
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// animation_modes.c
// 演出：バースト演出の入口
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 演出ごとの入口。バーストの型を BattlePres_RunBurstScene に渡す
// step : 演出の引数
//--------------------------------------------------------------------------
void EffectStep_RunAnimationMode6Or7Or8(struct EffectStep *step)
{
	if (step->variant == 0)
	{
		BattlePres_RunBurstScene(step, 6);
		return;
	}
	if (step->variant == 1)
	{
		BattlePres_RunBurstScene(step, 7);
		return;
	}
	BattlePres_RunBurstScene(step, 8);
}

void EffectStep_RunAnimationMode0(struct EffectStep *step)
{
	BattlePres_RunBurstScene(step, 0);
}

void EffectStep_RunAnimationMode0Alternate(struct EffectStep *step)
{
	BattlePres_RunBurstScene(step, 0);
}

void EffectStep_RunAnimationMode1(struct EffectStep *step)
{
	BattlePres_RunBurstScene(step, 1);
}

void EffectStep_RunAnimationMode2(struct EffectStep *step)
{
	BattlePres_RunBurstScene(step, 2);
}

void EffectStep_RunAnimationMode3Or4Or5(struct EffectStep *step)
{
	if (step->variant == 0)
	{
		BattlePres_RunBurstScene(step, 3);
		return;
	}
	if (step->variant == 1)
	{
		BattlePres_RunBurstScene(step, 4);
		return;
	}
	BattlePres_RunBurstScene(step, 5);
}

//--------------------------------------------------------------------------
// burst_scene.c
// バトル演出：対象に重ねて弾ける連撃と、せり上がる岩の壁
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 対象に何度も重ねて弾ける連撃
// BurstScene_Records は variant ごとに 7 バイト
//   [0] 弧の絵（0 なら BlueArcSheetA、ほかは B。1 ならすべての回を弧で描く）
//   [1] パレットを取るリソース（0〜3）
//   [2] 回数
//   [3] 1 回で飛ばす粒の数
//   [4] 回と回の間（フレーム）
//   [5] 全体のフレーム数
//   [6] 1 なら当たるたびにキャンバスを塗りつぶす
//--------------------------------------------------------------------------
void BattlePres_RunBurstScene(struct BattleEffectArgument *effect, s32 variant)
{
	struct EffectPosition	 actor_pos;
	struct EffectPosition	 target_pos;
	struct EffectPosition	 pos;
	s32						 screen_x;
	s32						 screen_y;
	BlitterPair				 draw;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	u8						*sprites;
	struct BattleCamera		*camera;
	struct MotionObject		*target;
	u8						*pltt;
	s32						 res;
	s32						 row;
	s32						 col;
	s32						 i;
	s32						 j;

	work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	sprites = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
	camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	if (work->effect->prelude == 1)
		BattleFx_PrepareCanvasEffect(effect, 7, work->effect->side, 2, &screen_x, &screen_y);

	//------------------------------------------------------------------
	// 絵とパレット
	//------------------------------------------------------------------
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

	// 玉（40x288）を横半分に詰めて 0x5100 から置く（キャンバスは横 2 倍で見せる）
	Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1, 0);
	for (row = 0; row != 288; row++)
	{
		for (col = 0; col != 40; col++)
		{
			s32 src = row * 40 + col;
			s32 dst = row * 20 + col / 2 + 0x5100;

			work->sheet[dst] = work->sheet[src];
		}
	}
	if (BurstScene_Records[variant * 7] == 0)
		Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work->sheet, 1, 1);
	else
		Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, Ram_MapCellBuffer + 0x5e00, 1, 0);
	switch (BurstScene_Records[variant * 7 + 1])
	{
	case 0:
		res = (s32)&ResourceId_MarsDjinnSheet;
		break;
	case 1:
		res = (s32)&ResourceId_LimePalette;
		break;
	case 2:
		res = (s32)&ResourceId_BlueArcSheetB;
		break;
	default:
		res = (s32)&ResourceId_EmberStreakSheet;
		break;
	}
	pltt = Resource_GetTableEntry(res);
	Iwram_CopyWords((void *)BG_PLTT, pltt, 128);
	WaitFrames(1);

	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &target_pos);
	work->transfer_mode = 2; // 残像 3/4
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 粒：対象の 25 ドット上に用意しておく（x の速さは原点の側へ向ける）
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (i = 0; i != 768; i++)
	{
		struct EffectStep *mote = &PARTICLES[i];

		mote->x = target->x;
		mote->y = target->y + 0x190000;
		mote->z = target->z;
		mote->velocity_x = (Random16() & 255) << 12;
		mote->velocity_y = ((Random16() & 255) - 127) << 12;
		mote->velocity_z = ((Random16() & 255) - 127) << 12;
		if (mote->x > 0)
			mote->velocity_x = -mote->velocity_x;
		mote->variant = -1; // 寿命（負なら出ていない）
	}
	BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0], 4, 0);

	for (frame = 0; frame != BurstScene_Records[variant * 7 + 5]; frame++)
	{
		s32 bursts = BurstScene_Records[variant * 7 + 2];

		EffectPosition_ApplyStepAndYOffset(work->effect->actor, &actor_pos);
		actor_pos.x /= 2;

		// 弧と波：奇数回は上下反転で描く
		if (work->effect->side == 0)
		{
			BattleEffect_LoadWork(
				HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
				GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_Y, GOUSEI_HIKAKU);
		}
		else
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7,
				GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
				GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP, GOUSEI_HIKAKU);
		}
		draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
		for (i = 0; i != bursts; i++)
		{
			s32 start = i * BurstScene_Records[variant * 7 + 4];

			if (frame >= start && frame < start + 6)
			{
				s32 step = frame - start;

				if ((i & 3) <= 1 || BurstScene_Records[variant * 7] == 1)
				{
					// 弧（48x72 を 6 コマ）
					if (work->effect->side == 0)
						draw[i & 1](canvas, work->sheet + step * 3456, target_pos.x / 2 - 16,
							target_pos.y - 40, 48, 72);
					else
						draw[i & 1](canvas, work->sheet + step * 3456, target_pos.x / 2 - 32,
							target_pos.y - 40, 48, 72);
				}
				else
				{
					// 波（48x16 を 6 コマ）
					if (work->effect->side == 0)
						draw[i & 1](canvas, Ram_MapCellBuffer + 0x5e00 + step * 768,
							target_pos.x / 2 - 16, actor_pos.y - 8, 48, 16);
					else
						draw[i & 1](canvas, Ram_MapCellBuffer + 0x5e00 + step * 768,
							target_pos.x / 2 - 32, actor_pos.y - 8, 48, 16);
				}
			}
			if (frame == start + 2)
			{
				// 当たり：最後の回で締める
				if (BurstScene_Records[variant * 7 + 6] == 1)
					Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
				ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
				if (i == bursts - 1)
				{
					BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
					work->shake_frames = 8;
					BattleEventRuntime_BeginPhaseFar(134);
				}
				else
				{
					if (i & 1)
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 7);
					work->shake_frames = 4;
					AudioCommand_PlayFar(134);
				}
				for (j = 0; j != BurstScene_Records[variant * 7 + 3]; j++)
					PARTICLES[i * 32 + j].variant = (Random16() & 7) + 15;
			}
			// 玉（20x48 を 6 コマ、2 フレームずつ）
			if (frame >= start + 2 && frame < start + 14)
				draw[0](canvas, work->sheet + ((frame - start - 2) / 2) * 960 + 0x5100,
					target_pos.x / 2 - 10, target_pos.y - 24, 20, 48);
		}
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);

		// 粒：寿命が尽きるまで描き、地面で跳ね返す
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
		draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
		for (i = 0; i != 512; i++)
		{
			struct EffectStep *mote = &PARTICLES[i];

			s32 size = mote->variant;

			if (size > 0)
			{
				EffectPosition_ApplyBaseAndYOffset(&mote->x, &pos);
				pos.x /= 2;
				size >>= 3;
				size++;
				draw[(i / 2) & 1](canvas, sprites + ParticleStreams_CellOffsets[size - 1],
					pos.x - size / 2, pos.y - size, size, size * 2);
				EffectStep_AdvanceWithGravity3D(mote, 60, -0x400);
				if (mote->y < 0x80000)
					mote->velocity_y = -mote->velocity_y / 2;
				mote->variant--;
			}
		}
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

extern const struct Scale RisingWall_UnitScale;

//--------------------------------------------------------------------------
// 地面が盛り上がり、岩の壁がせり上がって、しばらくして沈む
// 壁は左右対称で、左半分を描いてから右半分を左右反転で描く
// 壁の中心はキャンバスの x = 64 + wall_x
// 岩は 11 個のアニメーションオブジェクト（動きは particles[10〜20]）
//--------------------------------------------------------------------------
void BattlePres_RunRisingWall(struct BattleEffectArgument *effect)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	BlitterPair				 draw;
	u8						*sprites;
	s32						 wall_x;
	s32						 rock_x;
	struct Scale			 scale;
	s32						 pos[4];
	u8						*src;
	u16						*col;
	void					*pltt = (void *)BG_PLTT;
	s32						 n;
	s32						 frame;
	s32						 i;
	s32						 j;
	s32						 k;

	heap = gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	sprites = heap[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);

	// variant が大きいほど壁を横に広げる
	if (work->effect->variant == 0)
		REG_BG2PA = 204; // 1.25 倍
	else if (work->effect->variant == 1)
		REG_BG2PA = 170; // 1.5 倍
	if (work->effect->side == 1)
	{
		wall_x = 8;
		if (work->effect->variant == 0)
			rock_x = 40;
		else if (work->effect->variant == 1)
			rock_x = 36;
		else
			rock_x = 40;
	}
	else
	{
		wall_x = -16;
		rock_x = -12;
	}
	REG_BG2X = RisingWall_Bg2Shifts[work->effect->side][work->effect->variant] << 8;

	//------------------------------------------------------------------
	// パレット：BG の 1〜63 番を各成分 8 段暗くする
	//------------------------------------------------------------------
	Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, Ram_MapCellBuffer, 1, 0);
	Iwram_CopyWords(pltt, Resource_GetTableEntry((s32)&ResourceId_OrangePaletteA), 128);
	col = (u16 *)&BG_PLTT[1];
	for (i = 0; i != 63; i++)
	{
		s32 c = *col;
		s32 b = ((u16)c >> 10) & 31;
		s32 g = ((u16)c >> 5) & 31;
		s32 r = 31;

		r &= c;
		b -= 8;
		g -= 8;
		r -= 8;

		if (b < 0)
			b = 0;
		if (g < 0)
			g = 0;
		if (r < 0)
			r = 0;
		*col++ = (b << 10) | (g << 5) | r;
	}

	//------------------------------------------------------------------
	// 絵を work->sheet に並べ直す
	//   伸びていく壁   : 1 ライン × 9 段を、それぞれ 32 ラインに伸ばす
	//   壁の上 0x13c0  : 48x32 を縦 3 倍にして 48x96
	//   壁の下 0x25c0  : 48x21
	//   盛り上がり     : 3 コマ
	//------------------------------------------------------------------
	src = Ram_MapCellBuffer;
	n = 0;
	for (j = 0; j != 9; j++)
	{
		for (i = 0; i != 32; i++)
		{
			for (k = 0; k != RisingWall_RowWidths[j]; k++, n++)
				work->sheet[n] = src[k];
		}
		src += RisingWall_RowWidths[j];
	}
	for (j = 0; j != 32; j++)
	{
		for (i = 0; i != 3; i++)
		{
			for (k = 0; k != 48; k++, n++)
				work->sheet[n] = src[k];
		}
		src += 48;
	}
	for (i = 0; i != 1008; i++)
		work->sheet[n++] = *src++;
	for (i = 0; i != 3; i++)
	{
		for (k = 0; k != RisingWall_MoundWidths[i] * RisingWall_MoundHeights[i]; k++)
			work->sheet[n++] = *src++;
	}
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

	// 岩のオブジェクト
	for (i = 0; i != 11; i++)
	{
		struct AnimationObject *obj = ResourceObject_CreateFar(390);

		work->objects[i] = obj;
		if (obj != 0)
		{
			obj->flags = 0;
			AnimationObjects_SelectAnimationFar(obj, i / 4);
			((struct AnimationObject *)work->objects[i])->part[0].priority = 1;
		}
	}
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
	draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	REG_BLDCNT = 0x3f46;
	REG_BLDALPHA = 0x1010;
	work->transfer_mode = 2; // 残像 3/4
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 岩：x は列、y は 128 から z ずつ減らす数え、
	// velocity_x は 1〜12 を回る数え、variant は出るまでの待ち
	for (i = 0; i != 11; i++)
	{
		struct EffectStep *rock = &work->particles[10 + i];

		rock->x = (Random16() & 15) + 88;
		rock->y = 128;
		rock->z = (Random16() & 15) + 2;
		rock->velocity_x = i;
		rock->velocity_y = 1;
		rock->velocity_z = 0x8000;
		rock->variant = 44 - i * 4;
	}
	for (i = 0; i != 6; i++)
		work->particles[16 + i].x = RisingWall_LateRockColumns[i];

	// 土ぼこり：壁の根もとから上へ昇る
	for (i = 0; i != 256; i++)
	{
		struct EffectStep *dust = &PARTICLES[i];

		dust->x = ((Random16() & 63) + wall_x + 32) << 16;
		dust->y = ((Random16() & 7) + 96) << 16;
		dust->velocity_y = ((Random16() & 63) + 32) << 13;
		dust->variant = Random16() & 31;
	}
	REG_BG2CNT = 0x785;
	work->shake_frames = 250;
	for (frame = 0; frame != 192; frame++)
	{
		if (frame == 0)
			AudioCommand_PlayFar(212);
		if (frame == 40)
			AudioCommand_PlayFar(141);
		if (frame == 96)
			AudioCommand_PlayFar(145);
		if (frame == 120)
			BattleEventRuntime_BeginPhaseFar(134);

		// 盛り上がる地面（0〜81）
		if (frame <= 81)
		{
			s32 cell = frame / 4;

			if (cell > 2)
				cell = (cell & 1) + 1;
			draw[0](canvas, work->sheet + RisingWall_MoundSheetOffsets[cell],
				wall_x - RisingWall_MoundWidths[cell] + 64, 116 - RisingWall_MoundHeights[cell],
				RisingWall_MoundWidths[cell], RisingWall_MoundHeights[cell]);
			draw[1](canvas, work->sheet + RisingWall_MoundSheetOffsets[cell], wall_x + 64,
				116 - RisingWall_MoundHeights[cell], RisingWall_MoundWidths[cell],
				RisingWall_MoundHeights[cell]);
		}

		// せり上がる壁（12〜87）
		if (frame >= 12 && frame <= 87)
		{
			s32 row = (frame - 64) / 3;

			if (row < 0)
				row = 0;
			if (row > 7)
				row = 7;
			for (i = 0; i != 4; i++)
			{
				draw[0](canvas, work->sheet + RisingWall_RowSheetOffsets[row],
					wall_x - RisingWall_RowWidths[row] + 64, i * 32 - 12, RisingWall_RowWidths[row],
					32);
				draw[1](canvas, work->sheet + RisingWall_RowSheetOffsets[row], wall_x + 64,
					i * 32 - 12, RisingWall_RowWidths[row], 32);
			}
		}

		// 沈む壁（160〜183）
		if (frame >= 160 && frame <= 183)
		{
			s32 row = 7 - (frame - 160) / 3;

			if (row < 0)
				row = 0;
			if (row > 7)
				row = 7;
			for (i = 0; i != 4; i++)
			{
				draw[0](canvas, work->sheet + RisingWall_RowSheetOffsets[row],
					wall_x - RisingWall_RowWidths[row] + 64, i * 32 - 12, RisingWall_RowWidths[row],
					32);
				draw[1](canvas, work->sheet + RisingWall_RowSheetOffsets[row], wall_x + 64,
					i * 32 - 12, RisingWall_RowWidths[row], 32);
			}
		}

		// 立ち切った壁（88〜159）
		if (frame >= 88 && frame <= 159)
		{
			s32 width = 48;

			draw[0](canvas, &work->sheet[0x13c0], wall_x + 16, 0, width, 96);
			draw[1](canvas, &work->sheet[0x13c0], wall_x + 64, 0, width, 96);
			draw[0](canvas, &work->sheet[0x25c0], wall_x + 16, 96, width, 21);
			draw[1](canvas, &work->sheet[0x25c0], wall_x + 64, 96, width, 21);
		}

		// 土ぼこり（88〜）：上に抜けたら根もとへ戻す（159 まで）
		if (frame > 87)
		{
			for (i = 0; i != 64; i++)
			{
				struct EffectStep *dust = &PARTICLES[i];

				if (dust->variant == 0)
				{
					s32 size = (i & 3) + 5;

					draw[0](canvas, sprites + ParticleStreams_CellOffsets[size - 1],
						HI(dust->x) - size / 2, HI(dust->y) - size, size, size * 2);
					dust->y -= dust->velocity_y;
					if (dust->y < 0 && frame <= 159)
						dust->y = 0x600000;
				}
				else
				{
					dust->variant--;
				}
			}
		}

		// 岩（5〜）：72 フレーム目からは 11 個に増やし、大きくする
		if (frame > 4)
		{
			s32 rocks = 6;

			if (frame > 71)
				rocks = 11;
			for (k = 0; k != rocks; k++)
			{
				struct EffectStep *rock = &work->particles[10 + k];
				s32				   slot;

				if (rock->variant == 0)
				{
					scale = RisingWall_UnitScale;
					if (frame > 71)
					{
						scale.x = (k << 12) + 0x8000;
						scale.x += work->effect->variant << 14;
					}
					else
					{
						scale.x = 0x8000;
					}
					scale.y = scale.x;
					pos[3] = 0;
					pos[0] = (rock->x + rock_x * 2) << 16;
					pos[1] = 0x2000000 - (rock->y << 16);
					pos[2] = 0x2000000;
					slot = (frame / 2 + k) % 11;
					if (slot != -1)
						Object_ApplyProjectedPlacementFar(work->objects[slot], pos, &scale, 0);
					rock->y -= rock->z;
					rock->velocity_x += rock->velocity_y;
					if (rock->velocity_x > 12)
						rock->velocity_x -= 12;
					if (rock->y < 0)
					{
						// y が 0 を切ったら 128 から出し直す（88 フレーム目からは列と速さも選び直す）
						if (frame > 159)
						{
							rock->variant = -1;
						}
						else
						{
							if (frame > 87)
							{
								rock->z = (Random16() & 7) + 8;
								if (work->effect->variant == 0)
									rock->x = Random16() % 96 + 42;
								else if (work->effect->variant == 1)
									rock->x = Random16() % 112 + 34;
								else
									rock->x = Random16() % 160 + 10;
							}
							rock->y = 128;
							rock->variant = 8;
						}
					}
				}
				else
				{
					rock->variant--;
				}
			}
		}

		// 対象（86〜158）
		if (frame <= 158)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame > 85)
				{
					if (frame % 12 == 0)
						ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 6);
					if ((frame & 3) == 0)
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 5);
				}
			}
		}
		if (frame < 90 || frame > 160)
			Camera_ApplyShake(2, 2);
		else
			Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	for (i = 0; i != 11; i++)
		ResourceObject_ReleaseFar(work->objects[i]);
	BattleFx_EndCanvasLayer();
}

void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping, s32 gravity)
{
	step->x += step->velocity_x;
	step->y += step->velocity_y;
	step->z += step->velocity_z;
	step->velocity_y += gravity;
	step->velocity_x = step->velocity_x * damping / 64;
	step->velocity_y = step->velocity_y * damping / 64;
	step->velocity_z = step->velocity_z * damping / 64;
}

void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity)
{
	step->x += step->velocity_x;
	step->y += step->velocity_y;
	step->velocity_y += gravity;
	step->velocity_x = step->velocity_x * damping / 64;
	step->velocity_y = step->velocity_y * damping / 64;
}

s32 EffectPosition_ApplyBaseAndYOffset(s32 *point, struct EffectPosition *position)
{
	s32 result = Render_ProjectPoint(point, &position->x);
	position->y -= 16;
	return result;
}

void EffectPosition_ApplyAnimationAndYOffset(s32 id, struct EffectPosition *position)
{
	BattleUnit_ProjectToScreen(id, &position->x);
	position->y -= 16;
}

void EffectPosition_ApplyStepAndYOffset(s32 id, struct EffectPosition *position)
{
	BattleMotion_ProjectScaledPositionFar(id, position);
	position->y -= 16;
}

void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position)
{
	BattleMotion_ProjectPositionFar(id, position);
	position->y -= 16;
}

s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen)
{
	struct BattleCamera	   *camera;
	struct MotionObject	   *object;
	struct AnimationObject *info;
	s32						scale;
	/* FAKEMATCH: needed to match: the unused 12-byte vector preserves both stack adjustments and camera-load order. */
	s32 unused[3];

	camera = gCameraWork;
	object = GetBattleObjectSlotFar(unit)->object;
	info = GetMotionRecordFar(object, 0);
	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork((u8 *)camera, (u8 *)camera->pos);
	scale = Iwram_MulQ16(Render_ProjectPoint(&object->x, screen), info->scale);
	screen[1] -= Iwram_MulQ16(scale, Battle_GetObjectTableValueFar(unit) >> 17);
	return 0;
}

void BattleFx_ProbeKeysForExtendedActor(void)
{
	struct BattleEffectWork		*work = gBattleFxWork[0];
	struct BattleEffectArgument *effect = work->effect;

	if (effect->actors[0] > 0x7f)
		(void)gKeysHeld;
}

void BattleFx_DispatchByIdRange(struct BattleEffectArgument *effect)
{
	s32 kind;
	s32 value;

	value = 0x60e;
	Runtime_AllocateBlock(41, value);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BattleEffectWork));
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
	value = effect->kind;
	kind = value;
	value = kind - 100;
	if ((u32)value <= 35)
	{
		BattleFx_RunCastingImpact(effect, value);
	}
	else if (kind > 199)
	{
		BattlePres_RunRingAndSparkScene(effect);
	}
	else
	{
		BattlePres_RunBeamSequence(effect);
	}
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}
void BattlePres_RunBeamSequence(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 pos;
	struct EffectPosition	 anchor;
	struct EffectPosition	 point;
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 shake;
	u8						*sheet;
	struct BattleCamera		*camera;
	s32						 kind;
	struct BtlUnit			*unit;
	BlitterPair				 draw;
	DrawRectangle			*slots;
	struct MotionObject		*target;
	void					*colors = (void *)0x05000000;
	s32						 half;
	s32						 frame;
	s32						 i;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	camera = *(struct BattleCamera **)((u8 *)heap_cache - 108);
	kind = effect->kind;
	work->effect = effect;
	unit = UnitGetFar(effect->actor);
	WaitFrames(1);
	BattlePres_ConfigureEffectDisplay();
	BattleFx_SetupCanvasTileMap();
	REG_BG1CNT = 0x1f80;
	WaitFrames(1);
	if (kind == 5)
	{
		if (work->effect->side == 0)
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 11, 3);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 11, 2);
		}
		else
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 15, 3);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 15, 2);
		}
	}
	else
	{
		if (work->effect->side == 0)
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
		}
		else
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 3);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
		}
	}
	slots = (DrawRectangle *)gWorkSlot;
	draw[0] = slots[HEAP_SLOT_BLITTER];
	draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE];
	WaitFrames(1);
	if (kind == 4)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, work, 1, 1);
	}
	else if (kind == 3)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage, work, 0, 0);
	}
	else
	{
		switch (kind)
		{
		case 0:
		case 1:
		case 5:
			Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work, 1, 1);
			break;
		case 2:
			Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work, 1, 1);
			break;
		}
	}
	if (work->effect->actor > 7)
		Iwram_CopyWords(colors, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
	else
		Iwram_CopyWords(colors, Resource_GetTableEntry((s32)&ResourceId_CrescentSheet), 128);
	WaitFrames(1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, Ram_MapCellBuffer, 1, 0);
	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	REG_BG1CNT = 0x1f81;
	EffectPosition_ApplyAnimationAndYOffset(work->effect->actors[0], &anchor);
	if (work->effect->side == 0)
		shake = 96 - anchor.x;
	else
		shake = 32 - anchor.x;
	if (shake > 0)
		shake = 0;
	if (shake < -128)
		shake = -128;
	anchor.x += shake;
	gBgScroll[1].x = shake;
	gBgScroll[1].y = 80;
	WaitFrames(1);
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	half = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *spark = &work->particles[i];

		spark->x = target->x;
		spark->y = target->y + half;
		spark->z = target->z;
		spark->velocity_x = (Random16() & 255) << 10;
		spark->velocity_y = ((Random16() & 255) - 32) << 10;
		spark->velocity_z = ((Random16() & 255) - 127) << 10;
		if (spark->x > 0)
			spark->velocity_x = -spark->velocity_x;
		spark->velocity_x = -spark->velocity_x;
		spark->variant = i + 16;
	}

	for (frame = 0; frame != 32; frame++)
	{
		if (frame == 5)
		{
			if (Summon_IsEntrySecondaryFlaggedFar(unit->class_id))
				BattleEventRuntime_BeginPhaseFar(134);
			else
				BattleEventRuntime_BeginPhaseFar(133);
		}
		if (frame == 4)
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
		EffectPosition_ApplyStepAndYOffset(work->effect->actor, &pos);
		pos.y += 16;
		if (kind == 4)
		{
			if (frame <= 11)
			{
				if (work->effect->side == 0)
					draw[1](canvas, work->sheet + (5 - frame / 2) * 0x300, pos.x + shake - 48,
						pos.y - 8, 48, 16);
				else
					draw[1](canvas, work->sheet + (5 - frame / 2) * 0x300, pos.x + shake, pos.y - 8,
						48, 16);
			}
		}
		else if ((u32)kind <= 2 || kind == 5)
		{
			if (frame <= 11)
			{
				if (work->effect->side == 0)
					draw[1](canvas, work->sheet + frame / 2 * 0xd80, pos.x + shake - 48, pos.y - 40,
						48, 72);
				else
					draw[1](
						canvas, work->sheet + frame / 2 * 0xd80, pos.x + shake, pos.y - 40, 48, 72);
			}
		}
		else
		{
			if (frame <= 17)
			{
				s32 cell = frame / 3;

				if (work->effect->side == 0)
					draw[1](canvas, work->sheet + BeamSequence_CellSheetOffsets[cell],
						pos.x + BeamSequence_CellReaches[cell] + shake - 58,
						pos.y - BeamSequence_CellHeights[cell] / 2, BeamSequence_CellWidths[cell],
						BeamSequence_CellHeights[cell]);
				else
					draw[1](canvas, work->sheet + BeamSequence_CellSheetOffsets[cell],
						pos.x - BeamSequence_CellReaches[cell] + shake
							- BeamSequence_CellWidths[cell] + 58,
						pos.y - BeamSequence_CellHeights[cell] / 2, BeamSequence_CellWidths[cell],
						BeamSequence_CellHeights[cell]);
			}
		}
		if (frame >= 4 && frame <= 15)
			draw[0](canvas, Ram_MapCellBuffer + (frame - 4) / 2 * 0x780, anchor.x - 16,
				anchor.y - 24, 40, 48);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera, (u8 *)camera->pos);
		if (frame >= 4 && frame <= 31)
		{
			for (i = 0; i != 64; i++)
			{
				s32				   pair = i / 2;
				struct EffectStep *spark = &work->particles[pair];
				s32				   size = spark->variant;

				if (size > 0)
				{
					Render_ProjectPoint(&spark->x, &point.x);
					point.x += shake;
					size >>= 3;
					size += 2;
					draw[pair & 1](canvas, sheet + BattleFx6_FlareCells[size - 1], point.x - size,
						point.y - size, size * 2, size * 2);
					EffectStep_AdvanceWithGravity3D(spark, 60, -0x400);
					spark->variant--;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	gBgScroll[1].y = 32;
	BattleFx_SetTransitionFlagAndDisplay();
}

void BattlePres_RunRingAndSparkScene(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 pos;
	struct EffectPosition	 anchor;
	struct EffectPosition	 point;
	struct EffectPosition	 spot;
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 shake;
	u8						*sheet;
	struct BattleCamera		*camera;
	s32						 velocity_x;
	s32						 velocity_y;
	s32						 velocity_z;
	s32						 acceleration;
	s32						 strength;
	BlitterPair				 draw;
	DrawRectangle			*slots;
	struct MotionObject		*object;
	struct MotionObject		*target;
	u16						*background;
	void					*colors = (void *)0x05000000;
	s32						 words = 0x4000;
	s32						 big;
	s32						 half;
	s32						 frame;
	s32						 i;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	camera = *(struct BattleCamera **)((u8 *)heap_cache - 108);
	big = 1;
	if (effect->kind <= 199)
		big = 0;
	work->effect = effect;
	BattlePres_SetupTransitionAtPairMidpointFar(effect->actor, effect->unknown_000c, 130);
	WaitFrames(1);
	BattleFx_SetupCanvasTileMap();
	REG_BG1CNT = 0x1f80;
	if (work->effect->side == 0)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
	}
	else
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 3);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
	}
	slots = (DrawRectangle *)gWorkSlot;
	draw[0] = slots[HEAP_SLOT_BLITTER];
	draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE];
	BattlePres_SetupTransitionAtPairMidpointFar(
		work->effect->actor, work->effect->unknown_000c, 130);
	WaitFrames(1);
	Resource_LoadAndDecompress((s32)&ResourceId_StarBurstSheet, work, 1, 0);
	BattlePres_SetupTransitionAtPairMidpointFar(
		work->effect->actor, work->effect->unknown_000c, 130);
	WaitFrames(1);
	Resource_LoadAndDecompress((s32)&ResourceId_CrescentSheet, Ram_MapCellBuffer, 1, 1);
	if (work->effect->actor > 7)
		Iwram_CopyWords(colors, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
	BattlePres_SetupTransitionAtPairMidpointFar(
		work->effect->actor, work->effect->unknown_000c, 130);
	WaitFrames(1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
	BattlePres_SetupTransitionAtPairMidpointFar(
		work->effect->actor, work->effect->unknown_000c, 130);
	WaitFrames(1);
	work->transfer_mode = 1;
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	if (big == 1)
	{
		object = GetBattleObjectSlotFar(work->effect->actor)->object;
		for (i = 0; i != 64; i++)
		{
			struct EffectStep *mote = &work->particles[i];

			mote->x = (Random16() & 63) + 16;
			mote->y = 0;
			mote->z = 0;
			mote->velocity_x = Random16() & 0xffff;
			mote->velocity_y = Random16() & 0xffff;
			mote->velocity_z = Random16() & 0xffff;
		}
		ObjectDispatch_ApplyValueToChildrenFar(object, 0);
		velocity_x = object->velocity_x;
		velocity_y = object->velocity_y;
		velocity_z = object->velocity_z;
		strength = object->vertical_motion_strength;
		acceleration = object->acceleration;
		object->velocity_x = 0;
		object->velocity_y = 0;
		object->velocity_z = 0;
		object->acceleration = 0;
		object->vertical_motion_strength = 0;
		EffectPosition_ApplyStepAndYOffset(work->effect->actor, &pos);
		shake = 64 - pos.x;
		gBgScroll[1].x = shake;
		gBgScroll[1].y = 80;
		work->fade_frames = 24;
		work->fade_step = 0;
		Scheduler_AddOrUpdateCallback((s32)Palette_StepFadeTransfer, 0xc80);
		AudioCommand_PlayFar(212);
		for (frame = 0; frame != 32; frame++)
		{
			BattlePres_SetupTransitionAtPairMidpointFar(
				work->effect->actor, work->effect->unknown_000c, 130);
			for (i = 0; i != 64; i++)
			{
				struct EffectStep *mote = &work->particles[i];

				if (mote->x >= 0 && frame >= i / 4)
				{
					s32 size = (i & 1) + 5;

					Render_ResetTransformState();
					SceneTransform_ApplyRoll(mote->velocity_z);
					SceneTransform_ApplyPitch(mote->velocity_x);
					SceneTransform_ApplyYaw(mote->velocity_y);
					EffectPosition_ApplyBaseAndYOffset(&mote->x, &point);
					point.x += 64;
					point.y = point.y + pos.y + 24;
					if (point.depth < -60)
						point.depth = -60;
					if (point.depth > 60)
						point.depth = 60;
					point.depth += 60;
					draw[0](canvas, sheet + BattleFx6_FlareCells[size - 1], point.x - size,
						point.y - size, size * 2, size * 2);
					mote->x -= 4;
				}
			}
			work->transfer_pending = 1;
			WaitFrames(1);
		}
		Scheduler_RemoveCallback((u32)Palette_StepFadeTransfer);
		ObjectDispatch_ApplyValueToChildrenFar(object, 16);
		object->velocity_x = velocity_x;
		object->velocity_y = velocity_y;
		object->velocity_z = velocity_z;
		object->acceleration = acceleration;
		object->vertical_motion_strength = strength;
	}

	Iwram_ClearWords(canvas, words);
	Iwram_ClearWords((void *)0x06004000, words);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	REG_BG1CNT = 0x1f81;
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
	if (work->effect->side == 0)
		shake = 32 - anchor.x;
	else
		shake = 96 - anchor.x;
	if (shake > 0)
		shake = 0;
	if (shake < -128)
		shake = -128;
	anchor.x += shake;
	gBgScroll[1].y = 80;
	gBgScroll[1].x = shake;
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	half = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *spark = &work->particles[i];

		spark->x = target->x;
		spark->y = target->y + half;
		spark->z = target->z;
		spark->velocity_x = (Random16() & 255) << 10;
		spark->velocity_y = (Random16() & 255) << 10;
		spark->velocity_z = ((Random16() & 255) - 127) << 10;
		if (spark->x > 0)
			spark->velocity_x = -spark->velocity_x;
		spark->variant = i + 16;
	}

	for (frame = 0; frame != 32; frame++)
	{
		if (frame == 5)
			BattleEventRuntime_BeginPhaseFar(134);
		if (frame == 4)
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
		EffectPosition_ApplyStepAndYOffset(work->effect->actor, &pos);
		pos.y += 16;
		if (frame <= 1)
			draw[0](canvas, work, 0, 0, 120, 120);
		else if (frame <= 3)
			draw[0](canvas, work->sheet + 0x3840, 0, 0, 120, 120);
		else if (frame <= 5)
			draw[0](canvas, Ram_MapCellBuffer, 0, 0, 120, 120);
		else if (frame <= 7)
			draw[0](canvas, Ram_MapCellBuffer + 0x3840, 0, 0, 120, 120);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera, (u8 *)camera->pos);
		if (frame >= 4 && frame <= 31)
		{
			for (i = 0; i != 64; i++)
			{
				s32				   pair = i / 2;
				struct EffectStep *spark = &work->particles[pair];
				s32				   size = spark->variant;

				if (size > 0)
				{
					EffectPosition_ApplyBaseAndYOffset(&spark->x, &spot);
					spot.x += shake;
					size >>= 3;
					size += 2;
					spot.y += 16;
					draw[pair & 1](canvas, sheet + BattleFx6_FlareCells[size - 1], spot.x - size,
						spot.y - size, size * 2, size * 2);
					EffectStep_AdvanceWithGravity3D(spark, 60, -0x400);
					spark->variant--;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	gBgScroll[1].y = 32;
	for (frame = 0, background = &BtlWk->background; frame != 7; frame++)
	{
		BattlePresentation_SetPaletteLevelFar(*background, 6 - frame);
		WaitFrames(1);
	}
	BattleFx_SetTransitionFlagAndDisplay();
}

void BattleFx_StepPaletteToResource(s32 resource_id)
{
	u16	 target[64];
	u16 *color = (u16 *)0x05000000;
	s32	 i;

	Iwram_CopyWords(target, Resource_GetTableEntry(resource_id), 128);
	target[0] = 0;
	for (i = 0; i != 64; i++, color++)
	{
		s32 red = *color & 0x1f;
		s32 green = (*color >> 5) & 0x1f;
		s32 blue = (*color >> 10) & 0x1f;
		s32 target_red = target[i] & 0x1f;
		s32 target_green = (target[i] >> 5) & 0x1f;
		s32 target_blue = (target[i] >> 10) & 0x1f;

		if (red < target_red)
			red++;
		else if (red > target_red)
			red--;
		if (green < target_green)
			green++;
		else if (green > target_green)
			green--;
		if (blue < target_blue)
			blue++;
		else if (blue > target_blue)
			blue--;
		target[i] = (blue << 10) | (green << 5) | red;
	}
	Iwram_CopyWords((u16 *)0x05000000, target, 128);
}

//--------------------------------------------------------------------------
// casting.c
// バトル演出：ジンの技（演出 100〜135）
//--------------------------------------------------------------------------

static __inline__ void LoadResource(const void *id, void *dest, s32 skip, s32 copy)
{
	/* FAKEMATCH: needed to match: inline scope keeps each resource call's arguments local; only BattleFx_RunCastingImpact loads through it: with its 24 loads called directly it differs throughout and is 2 instructions longer (tbs-en, 2026-10-14), and the same inline as a layer for the whole effect family breaks BattleFx_RunProjectileVolley and BattlePres_RunBurstScene (L6 review) */
	Resource_LoadAndDecompress((s32)id, dest, skip, copy);
}

static __inline__ void PackRows(void *src, void *dest, s32 width, s32 height)
{
	/* FAKEMATCH: needed to match: keep packing arguments within their own inline scope */
	Graphics_PackTileRows(src, dest, width, height);
}

static __inline__ void DrawImage(
	void *canvas, const void *pixels, s32 x, s32 y, s32 width, s32 height, DrawRectangle *draw)
{
	/* FAKEMATCH: needed to match: inline scope reloads each image address at its drawing call */
	(*draw)(canvas, pixels, x, y, width, height);
}

static __inline__ void DrawTallImage(
	void *canvas, const void *pixels, s32 x, s32 y, s32 width, DrawRectangle *draw)
{
	/* FAKEMATCH: needed to match: derive height inside the inline scope so the dimension stores reuse one register instead of keeping two argument temporaries alive */
	(*draw)(canvas, pixels, x, y, width, width * 2);
}

static __inline__ void DrawScrolledImage(void *canvas, const void *pixels, s32 x, s32 y, s32 width,
	s32 height, s32 scroll, DrawRectangle *draw)
{
	/* FAKEMATCH: needed to match: subtract the scrolled part inside the drawing scope */
	(*draw)(canvas, pixels, x, y, width, height - scroll);
}

static __inline__ void DrawCroppedImage(
	void *canvas, const void *pixels, s32 x, s32 y, s32 width, DrawRectangle *draw)
{
	/* FAKEMATCH: forwarding inline; the cropped height stays inside its scope. Both calls made through the blitter directly with 91 differ 16 on tbs-en: the stack arguments 128 and 91 go through r0 instead of r1 and the pixels pool load moves ahead of them. */
	(*draw)(canvas, pixels, x, y, width, 91);
}

//--------------------------------------------------------------------------
// ジンの技
// 行動側のまわりに火花を集め、ジンを対象へ飛ばして、技ごとの絵で当てる
// 15・17・24・26・35 は集めたあと別の演出へ引き継ぐ
// command : 演出の引数
// kind    : 技（演出番号 - 100）
//--------------------------------------------------------------------------
void BattleFx_RunCastingImpact(struct BattleEffectArgument *command, s32 kind)
{
	void					 **slots;
	void					 **slot;
	s32						   alive;
	s32						   cell_ofs;
	struct EffectPosition	   caster_pos;
	struct EffectPosition	   target_pos;
	struct FixedPointPosition  djinn_pos;
	s32						   phase;
	s32						   i;
	s32						  *scanline;
	s32						   n;
	s32						   djinn_x;
	DrawRectangle			  *slot_draw;
	struct BattleEffectWork	  *work;
	void					  *canvas;
	s32						   frame;
	struct BattleCamera		  *camera;
	u8						  *sprites;
	s32						   scroll_pos;
	s32						   scroll_speed;
	s32						   duration;
	struct EffectPosition	  *target_screen;
	struct EffectPosition	  *caster_screen;
	DrawRectangle			  *draw_pair;
	s32						   saved_velocity_x;
	s32						   saved_velocity_y;
	s32						   saved_velocity_z;
	s32						   saved_acceleration;
	s32						   saved_vertical_strength;
	struct MotionObject		  *target_actor;
	struct FixedPointPosition *motion;
	s32						   kind_from_two;
	s32						   kind_from_four;
	struct FixedPointPosition *position;
	struct MotionObject		  *caster_actor;
	struct EffectStep		  *particle;
	s32						   pair_x;
	s32						   strip_x;
	struct FixedPointPosition  velocity;
	struct EffectPosition	   spark_screen;
	BlitterPair				   blitters;
	struct EffectPosition	   projected;

	slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
	slot = slots;
	work = (struct BattleEffectWork *)*slot++;
	canvas = *slot;
	camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	sprites = (u8 *)slots[2];
	work->effect = command;
	if (kind == 11 || kind == 8 || kind == 32)
	{
		BattleFx_BeginTiledCanvas(0);
	}
	else
	{
		BattleFx_BeginCanvasLayer(0);
	}
	REG_BLDALPHA = 0x1010;

	//------------------------------------------------------------------
	// 絵
	//   work->sheet           : 行動側に重ねる絵（48x72 を 6 コマ）
	//   work->sheet + 0x5100  : 当たりの玉（20x48、横半分に詰める）
	//   gMapCellBuffer        : 技ごとの当たりの絵
	//   gMapCellBuffer+0x3c56 : ジン（属性ごと、20x40）
	//------------------------------------------------------------------
	LoadResource(&ResourceId_ParticleSpritesA, sprites, 0, 0);
	LoadResource(&ResourceId_FireStreakSheet, work, 1, 0);
	LoadResource(&ResourceId_YellowOrbSheet, Ram_MapCellBuffer, 1, 0);
	/* FAKEMATCH: needed to match: local dimensions preserve the packing argument order 2026-10-11: literal 40 and 288 differ 4 (movs r2, #40 and the buffer load move ahead of lsls r3). */
	{
		s32 width = 40;
		s32 height = 288;
		PackRows(Ram_MapCellBuffer, work->sheet + 0x5100, width, height);
	}
	if (kind == 5 || kind == 23)
	{
		LoadResource(&ResourceId_FlashBurstSheet, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind == 12)
	{
		LoadResource(&ResourceId_SkullSheet, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind == 6 || kind == 27)
	{
		LoadResource(&ResourceId_TornadoSheet, Ram_MapCellBuffer, 1, 0);
		LoadResource(&ResourceId_LightningBoltSheet, Ram_MapCellBuffer + 0xc56, 1, 0);
	}
	else if (kind == 31)
	{
		LoadResource(&ResourceId_RuneSheet, Ram_MapCellBuffer, 1, 1);
	}
	else if (kind == 8)
	{
		LoadResource(&ResourceId_RockSpireSheet, Ram_MapCellBuffer, 1, 1);
	}
	else if (kind == 14)
	{
		LoadResource(&ResourceId_BlueFlameSheet, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind == 30)
	{
		LoadResource(&ResourceId_TornadoSheet, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind == 16)
	{
		LoadResource(&ResourceId_IceChipSheet, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind == 20)
	{
		LoadResource(&ResourceId_EmberStreakSheet, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind == 33 || kind == 34)
	{
		LoadResource(&ResourceId_FlameSheetA, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind != 11 && kind != 32)
	{
		LoadResource(&ResourceId_SmokeSheet, Ram_MapCellBuffer, 1, 0);
	}
	switch (kind) // ジンの属性
	{
	case 0: // 地
	case 4:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 33:
		LoadResource(&ResourceId_VenusDjinnSmallSheet, Ram_MapCellBuffer + 0x3c56, 1, 1);
		break;
	case 2: // 水
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		LoadResource(&ResourceId_MercuryDjinnSmallSheet, Ram_MapCellBuffer + 0x3c56, 1, 1);
		break;
	case 3: // 火
	case 5:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 34:
	case 35:
		LoadResource(&ResourceId_MarsDjinnSmallSheet, Ram_MapCellBuffer + 0x3c56, 1, 1);
		break;
	case 1: // 風
	case 6:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
		LoadResource(&ResourceId_JupiterDjinnSmallSheet, Ram_MapCellBuffer + 0x3c56, 1, 1);
		break;
	case 100:
		LoadResource(&ResourceId_MercuryDjinnSmallSheet, Ram_MapCellBuffer + 0x3c56, 1, 1);
		break;
	}
	work->transfer_mode = 2; // 残像（12 だけ 3/4、ほかは 1/2）
	if (kind == 12)
	{
		work->transfer_value = 75;
	}
	else
	{
		work->transfer_value = 50;
	}
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	/* FAKEMATCH: needed to match: retain each output pointer while preparing its call 2026-10-11: the pointers set in statements before the calls differ 38, after the calls 142. */
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], target_screen = &target_pos);
	EffectPosition_ApplyStepAndYOffset(work->effect->actor, caster_screen = &caster_pos);
	BattleFx_FetchRectangleBlitters(work->effect->side, draw_pair = blitters);
	work->fade_frames = 24;
	work->fade_step = 0;
	caster_actor = GetBattleObjectSlotFar(work->effect->actor)->object;

	// 集まる火花：x は半径（32〜95）、velocity は回す角度
	{
		for (i = 0; i != 64; i++)
		{
			particle = &work->particles[i];
			particle->x = (Random16() & 63) + 32;
			particle->y = 0;
			particle->z = 0;
			particle->velocity_x = Random16() & 0xffff;
			particle->velocity_y = Random16() & 0xffff;
			particle->velocity_z = Random16() & 0xffff;
		}
	}
	// 行動側の動きを止め、ジンを頭上 80 ドットに置く
	ObjectDispatch_ApplyValueToChildrenFar(caster_actor, 0);
	position = &djinn_pos;
	position->x = caster_actor->x;
	position->y = caster_actor->y + 0x500000;
	position->z = caster_actor->z;
	saved_velocity_x = caster_actor->velocity_x;
	saved_velocity_y = caster_actor->velocity_y;
	saved_velocity_z = caster_actor->velocity_z;
	saved_acceleration = caster_actor->acceleration;
	saved_vertical_strength = caster_actor->vertical_motion_strength;
	caster_actor->velocity_x = 0;
	caster_actor->velocity_y = 0;
	caster_actor->velocity_z = 0;
	caster_actor->acceleration = 0;
	caster_actor->vertical_motion_strength = 0;
	EffectPosition_ApplyStepAndYOffset(work->effect->actor, caster_screen);
	caster_screen->x = caster_screen->x / 2;
	AudioCommand_PlayFar(212);

	//------------------------------------------------------------------
	// 集める（32 フレーム）
	// 火花は毎フレーム 4 個ずつ動き出し、半径を 4 ずつ縮めて行動側へ集まる
	// 技 0〜7 は、火花が消え始めたらジンを出す
	//------------------------------------------------------------------
	frame = 0;
	do
	{
		n = 0;
		for (i = 0; i != 64; i++)
		{
			struct EffectStep *step = &work->particles[i];
			if (step->x >= 0)
			{
				if (frame >= i / 4)
				{
					{
						s32 size = 5;
						Render_ResetTransformState();
						SceneTransform_ApplyRoll(step->velocity_z);
						SceneTransform_ApplyPitch(step->velocity_x);
						SceneTransform_ApplyYaw(step->velocity_y);
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &spark_screen);
						spark_screen.x = spark_screen.x / 2 + caster_screen->x;
						if (kind <= 7)
							spark_screen.y = spark_screen.y + caster_screen->y - 8;
						else if (kind == 35)
							spark_screen.y = spark_screen.y + caster_screen->y + 44;
						else
							spark_screen.y = spark_screen.y + caster_screen->y + 12;
						if (spark_screen.depth < -60)
							spark_screen.depth = -60;
						if (spark_screen.depth > 60)
							spark_screen.depth = 60;
						spark_screen.depth += 60; // 0〜120
						blitters[1](canvas, sprites + ParticleStreams_CellOffsets[size - 1],
							spark_screen.x - size / 2, spark_screen.y - size, size, size * 2);
					}
					step->x -= 4;
				}
				n++;
			}
		}
		if (kind <= 7)
		{
			alive = n;
			if (alive <= 63)
			{
				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				EffectPosition_ApplyBaseAndYOffset((s32 *)&djinn_pos, &spark_screen);
				spark_screen.x /= 2;
				djinn_x = spark_screen.x;
				blitters[0](
					canvas, Ram_MapCellBuffer + 0x3c56, djinn_x - 10, spark_screen.y - 4, 20, 40);
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	} while (frame != 32);
	/* FAKEMATCH: needed to match: restart the pointer's lifetime after gathering so the projection above uses the stack position directly 2026-10-11: direct djinn_pos stores above, with this as the only assignment, differ 114, as does removing it. */
	position = &djinn_pos;

	//------------------------------------------------------------------
	// 技ごとの BG2 の準備：横を等倍にし、当たりの絵を対象に合わせる
	//------------------------------------------------------------------
	if (kind == 11)
	{
		REG_BG2PA = 0x100;
		if (work->effect->side == 0)
		{
			REG_BG2X = (frame - target_screen->x) << 8; // frame はここで 32
		}
		else
		{
			REG_BG2X = (96 - target_screen->x) << 8;
		}
	}
	else
	{
		if (kind == 32)
		{
			// 絵を横に流す（scroll_pos は 16.16 のドット）
			REG_BG2PA = 0x100;
			if (work->effect->side == 0)
			{
				scroll_pos = -0x800000;
				scroll_speed = 0xc0000;
			}
			else
			{
				scroll_pos = 0x80000;
				scroll_speed = -0xc0000;
			}
			REG_BG2X = (scroll_pos >> 16) << 8;
		}
	}
	if (kind == 8)
	{
		REG_BG2PA = 0x100;
		REG_BG2X = (64 - target_screen->x) << 8;
		work->transfer_mode = 1; // 毎フレーム 0 で消す
		work->transfer_value = 0;
		Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
		Iwram_ClearWords(canvas, 0x4000);
		REG_BLDCNT = 0;
	}
	if (kind == 31)
	{
		REG_BG2PA = 0x100;
		if (work->effect->side == 0)
		{
			REG_BG2X = (32 - target_screen->x) << 8;
		}
		else
		{
			REG_BG2X = (96 - target_screen->x) << 8;
		}
	}
	// 15・17・24・26 は、キャンバスを片付けて別の演出へ引き継ぐ
	if (kind == 15 || kind == 17 || kind == 24 || kind == 26)
	{
		Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
		Iwram_ClearWords(canvas, 0x4000);
		work->effect->prelude = 0;
		Scheduler_RemoveCallback(Palette_StepFadeTransfer);
		Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		ObjectDispatch_ApplyArgumentToChildrenFar(caster_actor, 3);
		if (kind == 15)
		{
			BattleFx_RunProjectileVolley(command, 9);
		}
		if (kind == 24)
		{
			BattleFx_RenderMode5(command);
		}
		if (kind != 26)
		{
			return;
		}
		BattleFx_RunProjectileVolley(command, 8);
		return;
	}
	ObjectDispatch_ApplyValueToChildrenFar(caster_actor, 16);
	caster_actor->velocity_x = saved_velocity_x;
	caster_actor->velocity_y = saved_velocity_y;
	caster_actor->velocity_z = saved_velocity_z;
	caster_actor->acceleration = saved_acceleration;
	caster_actor->vertical_motion_strength = saved_vertical_strength;
	// 35 は、キャンバスを片付けて BattleFx_RunSparkGroups へ引き継ぐ
	if (kind == 35)
	{
		Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
		Iwram_ClearWords(canvas, 0x4000);
		work->effect->prelude = 0;
		Scheduler_RemoveCallback(Palette_StepFadeTransfer);
		Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		command->variant = 3;
		BattleFx_RunSparkGroups(command, 2);
		return;
	}

	// ジンの 1 フレームの進み：6 フレームで対象の 30 ドット上へ
	target_actor = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	motion = &velocity;
	motion->x = (target_actor->x - position->x) / 6;
	motion->y = (target_actor->y - position->y + 0x1e0000) / 6;
	motion->z = (target_actor->z - position->z) / 6;
	for (i = 0; i != 64; i++)
	{
		particle = &work->particles[i];
		particle->variant = 0;
	}
	// 当たりの粒：対象の高さの半分から飛び散る（variant は寿命）
	if (kind != 14)
	{
		s32 height = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
		for (i = 0; i != 32; i++)
		{
			particle = &work->particles[i];
			particle->x = target_actor->x;
			particle->y = height;
			particle->z = target_actor->z;
			if (kind == 31)
			{
				particle->velocity_x = ((Random16() & 255) - 127) << 12;
				particle->velocity_y = ((Random16() & 255) - 64) << 10;
				particle->velocity_z = ((Random16() & 255) - 127) << 12;
			}
			else
			{
				particle->velocity_x = ((Random16() & 255) - 127) << 12;
				particle->velocity_y = ((Random16() & 255) - 64) << 12;
				particle->velocity_z = ((Random16() & 255) - 127) << 12;
			}
			particle->variant = i / 2 + 32;
		}
	}
	// 11・32 は行動側に重ねる絵と当たりの絵を差し替える
	if (kind == 11)
	{
		LoadResource(&ResourceId_CounterRevealSheet, work, 1, 1);
		LoadResource(&ResourceId_RedCrescentSheetA, Ram_MapCellBuffer, 1, 0);
		REG_BLDALPHA = 0xe10;
	}
	if (kind == 32)
	{
		LoadResource(&ResourceId_BlueBeastSheet, work, 1, 1);
		LoadResource(&ResourceId_RedCrescentSheetB, Ram_MapCellBuffer, 1, 0);
		REG_BLDALPHA = 0xe10;
	}
	// 当たりの絵の粒：gMapCellBuffer + 0x4000 に 64 個
	if (kind != 7 && kind != 13 && kind != 18 && kind != 11 && kind != 32 && kind != 19)
	{
		s32 height = 0;
		if (kind != 12)
			height = 0x140000;
		for (i = 0; i != 64; i++)
		{
			particle = &((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))[i];
			particle->x = target_actor->x;
			particle->y = height;
			particle->z = target_actor->z;
			if (kind == 5 || kind == 23)
			{
				particle->velocity_x = ((Random16() & 255) - 127) << 11;
				particle->velocity_y = (Random16() & 255) << 11;
				particle->velocity_z = ((Random16() & 255) - 127) << 11;
			}
			else if (kind == 25)
			{
				particle->velocity_x = ((Random16() & 255) - 127) << 11;
				particle->velocity_y = (Random16() & 127) << 10;
				particle->velocity_z = ((Random16() & 255) - 127) << 11;
			}
			else
			{
				particle->velocity_x = ((Random16() & 255) - 127) << 10;
				particle->velocity_y = (Random16() & 127) << 10;
				particle->velocity_z = ((Random16() & 255) - 127) << 10;
			}
			particle->variant = 0;
		}
	}
	// 2・3・12・22・28・29 は、BG2 を走査線ごとに揺らす
	kind_from_two = kind - 2;
	if ((kind_from_two >= 0 && kind_from_two <= 1) || kind == 12 || kind == 22 || kind == 29
		|| kind == 28)
	{
		Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
	}

	// 長さ（フレーム）
	kind_from_four = kind - 4;
	if ((kind_from_four >= 0 && kind_from_four <= 2) || kind == 23 || kind == 30 || kind == 27
		|| kind == 33 || kind == 34 || kind == 100)
	{
		duration = 32;
	}
	else if (kind >= 0 && kind <= 3 || kind == 8 || kind == 9 || kind == 10 || kind == 22
			 || kind == 25 || kind == 29 || kind == 31 || kind == 14)
	{
		duration = 48;
	}
	else if (kind == 21)
	{
		duration = 20;
	}
	else if (kind == 11 || kind == 32 || kind == 20)
	{
		duration = 40;
	}
	else if (kind == 28 || kind == 12)
	{
		duration = 64;
	}
	else
	{
		duration = 80;
	}
	//------------------------------------------------------------------
	// 当てる
	//------------------------------------------------------------------
	frame = 0;
	while (frame != duration)
	{
		// BG2 を揺らす表（走査線ごとの BG2X）
		if (kind != 11)
		{
			if (kind != 32)
			{
				s32 base;
				scanline = work->bg2_x;
				i = 0;
				base = 0x40000;
				phase = frame << 12;
				for (; i != 160; i++)
				{
					s32 offset = (base - (Trig_Sin(phase) << 2)) >> 10; // 1 - sin ドット（8.8）
					*scanline++ = offset;
					phase += 0x800;
				}
			}
		}
		// 最初の 3 フレームは行動側の位置を取り直す（16 ドット下）
		if (frame <= 2)
		{
			EffectPosition_ApplyStepAndYOffset(work->effect->actor, caster_screen);
			caster_screen->x = caster_screen->x / 2;
			caster_screen->y += 16;
		}
		// 行動側に重ねる絵（2 フレームずつ 6 コマ）
		if (kind != 11 && kind != 8 && kind != 32 && kind != 33 && kind != 34)
		{
			if (frame <= 11)
			{
				if (work->effect->side == 0)
				{
					blitters[0](canvas, work->sheet + frame / 2 * 3456, caster_screen->x - 32,
						caster_screen->y - 40, 48, 72);
				}
				else
				{
					blitters[0](canvas, work->sheet + frame / 2 * 3456, caster_screen->x,
						caster_screen->y - 40, 48, 72);
				}
			}
		}
		// 技ごとに、BG のパレットを決まったリソースのパレットへ 1 段ずつ近づける
		switch (kind)
		{
		case 33:
			BattleFx_StepPaletteToResource((s32)&ResourceId_FlameSheetA);
			break;
		case 14:
			BattleFx_StepPaletteToResource((s32)&ResourceId_BlueFlameSheet);
			break;
		case 31:
			BattleFx_StepPaletteToResource((s32)&ResourceId_RuneSheet);
			break;
		case 8:
			BattleFx_StepPaletteToResource((s32)&ResourceId_RockSpireSheet);
			break;
		case 0:
		case 10:
			BattleFx_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
			break;
		case 12:
		case 13:
		case 25:
			BattleFx_StepPaletteToResource((s32)&ResourceId_PinkBurstSheet);
			break;
		case 18:
			BattleFx_StepPaletteToResource((s32)&ResourceId_IceBlockSheet);
			break;
		case 19:
			BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
			break;
		case 2:
		case 29:
			BattleFx_StepPaletteToResource((s32)&ResourceId_TanPalette);
			break;
		case 1:
		case 28:
			BattleFx_StepPaletteToResource((s32)&ResourceId_CyanPalette);
			break;
		case 3:
		case 20:
		case 22:
			BattleFx_StepPaletteToResource((s32)&ResourceId_EmberStreakSheet);
			break;
		case 9:
			BattleFx_StepPaletteToResource((s32)&ResourceId_LimePalette);
			break;
		case 5:
		case 23:
			BattleFx_StepPaletteToResource((s32)&ResourceId_FlashBurstSheet);
			break;
		}
		// 当たりの玉（4〜15 フレーム）と当たりの粒（4 フレーム目から）
		if (kind != 11 && kind != 8 && kind != 32)
		{
			if (frame >= 4 && frame <= 15)
			{
				draw_pair[1](canvas, work->sheet + (frame - 4) / 2 * 960 + 0x5100,
					target_screen->x / 2 - 8, caster_screen->y - 24, 20, 48);
			}
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			if (frame > 3)
			{
				for (i = 0; i != 128; i++)
				{
					s32				   index = i / 2;
					struct EffectStep *step = &work->particles[index];
					s32				   size = step->variant;
					if (size > 0)
					{
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
						size >>= 4;
						size++;
						projected.x /= 2;
						blitters[index & 1](canvas, sprites + ParticleStreams_CellOffsets[size - 1],
							projected.x - size / 2, projected.y - size, size, size * 2);
						EffectStep_AdvanceWithGravity3D(step, 60, -0x1000);
						step->variant--;
					}
				}
			}
		}
		//------------------------------------------------------------------
		// 技ごとの当たり
		//------------------------------------------------------------------
		if (kind == 7 || kind == 13 || kind == 18 || kind == 19)
		{
			// 12 フレーム目から、対象の粒を行動側へ引き寄せる
			if (frame == 50)
			{
				ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
			}
			if (frame == 79)
			{
				ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
			}
			if (frame == 12)
			{
				particle = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000);
				for (i = 0; i != 64; i++, particle++)
				{
					particle->x = target_actor->x;
					particle->y = 0x140000;
					particle->z = target_actor->z;
					particle->velocity_x = ((Random16() & 255) - 128) << 10;
					particle->velocity_y = ((Random16() & 255) - 128) << 10;
					particle->velocity_z = ((Random16() & 255) - 128) << 10;
					particle->variant = 0;
				}
			}
			if (frame > 11)
			{
				struct EffectStep	*step = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000);
				struct MotionObject *caster;
				s32					 height;
				caster = GetBattleObjectSlotFar(work->effect->actor)->object;
				height = Battle_GetObjectTableValueFar(work->effect->actor) / 2;
				for (i = 0; i != 32; i++, step++)
				{
					if (step->variant >= 0)
					{
						s32 size = (i & 1) + 6;
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
						projected.x >>= 1;
						blitters[0](canvas, sprites + ParticleStreams_CellOffsets[size - 1],
							projected.x - (u32)size / 2, projected.y - size, size, size * 2);
						EffectStep_AdvanceWithGravity3D(step, 62, 0);
						if (frame > i + 22)
						{
							s32 dx = (caster->x - step->x) >> 8;
							s32 dy = (caster->y + height - step->y) >> 8;
							s32 dz = (caster->z - step->z) >> 8;
							step->velocity_x += dx;
							step->velocity_y += dy;
							step->velocity_z += dz;
							if ((dx >= -0xfff && dx <= 0xfff) && (dz >= -0xfff && dz <= 0xfff))
								step->variant = -1;
						}
					}
				}
			}
		}
		else if (kind == 21)
		{
			// 描かない（終わってから BattleFx_RunPaletteRampMode1 へ）
		}
		else if (kind == 6 || kind == 27)
		{
			// 縦長の絵（24x104 を 4 コマ）を 2 本と、対象のまわりにかけら
			if (frame >= 6 && frame <= 19)
			{
				phase = frame;
				i = 0;
				do
				{
					blitters[0](canvas, ((phase / 2) & 3) * 2880 + (Ram_MapCellBuffer + 0xc56),
						target_screen->x / 2 - 8, 0, 24, 104);
					i++;
					phase += 3;
				} while (i != 2);
			}
			if (frame >= 8 && frame <= 23)
			{
				for (i = 0; i != 3; i++)
				{
					s32 image = i & 3;
					s32 angle = Random16() & 0xffff;
					s32 image_x;
					s32 image_y;
					s32 draw_flags;
					image_x = Trig_Sin(angle);
					image_x <<= 3;
					image_x >>= 16;
					image_x += target_screen->x / 2;
					image_x -= BattleFx_GlintCellWidths[image] / 2;
					image_y = Trig_Cos(angle);
					image_y <<= 5;
					image_y >>= 16;
					image_y -= BattleFx_GlintCellHeights[image] / 2;
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
					draw_flags = Random16(); // 反転はランダム
					BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
						CastingImpact_GlintDrawFlags[draw_flags & 3] | GOUSEI_CLIP_X
							| GOUSEI_CLIP_Y,
						GOUSEI_HIKAKU);
					((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE](canvas,
						Ram_MapCellBuffer + BattleFx_GlintCellOffsets[image], image_x, image_y + 56,
						BattleFx_GlintCellWidths[image], BattleFx_GlintCellHeights[image]);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
					BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
				}
			}
		}
		else if (kind == 14)
		{
			// 上から降りてくる柱（17 幅の絵を下へ流す）と、対象の位置から出る粒
			s32 fall;
			s32 scroll;
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			if (frame >= 0 && frame <= 23)
			{
				DrawRectangle *draw;
				s32			   width;
				s32			   tile_height;
				s32			   column_x;
				s32			   column_y;
				s32			   top;
				s32			   origin_x;

				origin_x = target_screen->x / 2;
				fall = frame * 32 - 232;
				scroll = frame * 16 - 48;
				if (fall > 0)
					fall = 0;
				while (scroll > 104)
					scroll -= 104;
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
					GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
				draw = (DrawRectangle *)gWorkSlot;
				draw += HEAP_SLOT_BLITTER_ALTERNATE;
				column_x = origin_x - 8;
				column_y = fall + scroll;
				top = column_y - 104;
				width = 17;
				tile_height = 104;
				DrawImage(canvas, Ram_MapCellBuffer, column_x, top, width, tile_height, draw);
				DrawScrolledImage(canvas, Ram_MapCellBuffer, column_x, column_y, width, tile_height,
					scroll, draw);
				(*draw)(
					canvas, Ram_MapCellBuffer + 0x6e8, origin_x - width, fall + 47, width * 2, 65);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				if (frame == 8)
				{
					work->shake_frames = frame;
				}
				if (frame > 1)
				{
					s32				   emitted = 0;
					struct EffectStep *step;
					for (i = 0; i != 64; i++)
					{
						if ((step = &work->particles[i])->variant == 0)
						{
							step->x = target_actor->x;
							step->y = 0x140000;
							step->z = target_actor->z;
							step->velocity_x = ((Random16() & 255) - 127) << 12;
							step->velocity_y = ((Random16() & 255) - 64) << 10;
							step->velocity_z = ((Random16() & 255) - 127) << 12;
							step->variant = i / 2 + 32;
							emitted++;
							if (emitted == 4)
								break;
						}
					}
				}
			}
			BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
		}
		else if (kind == 31)
		{
			// 左右対称の 24x48 の絵
			s32 height;
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			if (frame >= 4 && frame <= 23)
			{
				pair_x = target_screen->x / 2;
				height = 48;
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
					GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
				slot_draw = (DrawRectangle *)gWorkSlot;
				DrawImage(canvas, Ram_MapCellBuffer, pair_x - 24, 48, 24, height,
					slot_draw + HEAP_SLOT_BLITTER_ALTERNATE);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
					GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
				DrawImage(canvas, Ram_MapCellBuffer, pair_x, 48, 24, height,
					slot_draw + HEAP_SLOT_BLITTER_ALTERNATE);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			}
			BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
		}
		else if (kind == 30)
		{
			// 3 段の絵（40x32）を 2 フレームずつ 3 コマ、16 フレーム目から薄くする
			s32 image;
			if (frame > 15)
			{
				REG_BLDALPHA = (32 - frame) | 0x1000;
			}
			if (frame > 5)
			{
				strip_x = target_screen->x / 2 - 20;
				image = frame / 2 % 3;
				cell_ofs = image * 2560;
				blitters[0](canvas, Ram_MapCellBuffer + 0xc56 + cell_ofs, strip_x, 16, 40, 32);
				blitters[0](
					canvas, image * 1280 + (Ram_MapCellBuffer + 0x2a56), strip_x, 48, 40, 32);
				blitters[0](canvas, cell_ofs + (Ram_MapCellBuffer + 0x1156), strip_x, 80, 40, 32);
			}
		}
		else if (kind == 5 || kind == 23)
		{
			// 粒ごとに 32x64 の絵を 2 フレームずつ 6 コマ
			struct EffectStep *step = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000);
			for (i = 0; i != 16; i++, step++)
			{
				if (frame >= i / 2 + 4)
				{
					s32 age = step->variant;
					if (age <= 11)
					{
						s32 image = age / 2;
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
						projected.x /= 2;
						blitters[0](canvas, Ram_MapCellBuffer + (image << 11), projected.x - 16,
							projected.y - 32, 32, 64);
						EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
						step->variant++;
					}
				}
			}
		}
		else if (kind == 4)
		{
			// 描かない
		}
		else if (kind == 11)
		{
			// 対象に重ねる大きな絵を 2 フレームずつ 7 枚（上下に揺らす）
			// 4・5 フレーム目はキャンバスを塗りつぶす
			s32 image_y;
			s32 sway;
			Trig_Sin(frame << 9); // 結果は使わない
			sway = Trig_Cos(frame << 9);
			sway <<= 2;
			sway >>= 16;
			image_y = (target_screen->y >> 16) + sway + 16;
			if (frame <= 3)
			{
				blitters[0](canvas, work, CastingImpact_ImageX[work->effect->side * 7],
					image_y + CastingImpact_ImageY[0], 57, 98);
			}
			else
			{
				if (frame <= 7)
				{
					blitters[0](canvas, work, CastingImpact_ImageX[work->effect->side * 7],
						image_y + CastingImpact_ImageY[0], 57, 98);
				}
				blitters[0](canvas, work->sheet + 0x15d2,
					CastingImpact_ImageX[work->effect->side * 7 + 1],
					image_y + CastingImpact_ImageY[1], 99, 69);
				if (frame == 4 || frame == 5)
				{
					Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
				}
				if (frame == 6 || frame == 7)
				{
					blitters[0](canvas, work->sheet + 0x3081,
						CastingImpact_ImageX[work->effect->side * 7 + 2],
						image_y + CastingImpact_ImageY[2], 128, 91);
				}
				if (frame == 8 || frame == 9)
				{
					DrawCroppedImage(canvas, Ram_MapCellBuffer,
						CastingImpact_ImageX[work->effect->side * 7 + 3],
						image_y + CastingImpact_ImageY[3], 128, &blitters[0]);
				}
				if (frame == 10 || frame == 11)
				{
					blitters[0](canvas, Ram_MapCellBuffer + 0x2d80,
						CastingImpact_ImageX[work->effect->side * 7 + 4],
						image_y + CastingImpact_ImageY[4], 128, 59);
				}
				if (frame == 12 || frame == 13)
				{
					blitters[0](canvas, Ram_MapCellBuffer + 0x4b00,
						CastingImpact_ImageX[work->effect->side * 7 + 5],
						image_y + CastingImpact_ImageY[5], 122, 29);
				}
				if (frame == 14 || frame == 15)
				{
					blitters[0](canvas, Ram_MapCellBuffer + 0x58d2,
						CastingImpact_ImageX[work->effect->side * 7 + 6],
						image_y + CastingImpact_ImageY[6], 76, 25);
				}
			}
		}
		else if (kind == 32)
		{
			// 横に流れて止まる絵と、2 フレームずつ続く大きな絵
			// 16 フレーム目から薄くする
			scroll_pos += scroll_speed;
			if (frame > 6)
			{
				scroll_speed = (scroll_speed * 48) / 64;
			}
			REG_BG2X = (scroll_pos >> 16) << 8;
			{
				s32 fade;
				fade = frame - 16;
				if (fade >= 0 && fade <= 15)
				{
					REG_BLDALPHA = (16 - fade) | 0x1000;
				}
			}
			if (frame == 4 || frame == 5)
			{
				Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
			}
			if (frame <= 3)
			{
				if (work->effect->side == 1)
				{
					blitters[0](canvas, work, 0, 24, 80, 104);
				}
				else
				{
					blitters[0](canvas, work, 48, 24, 80, 104);
				}
			}
			else
			{
				if (frame <= 7)
				{
					if (work->effect->side == 1)
					{
						blitters[0](canvas, work, 0, 24, 80, 104);
					}
					else
					{
						blitters[0](canvas, work, 48, 24, 80, 104);
					}
				}
				if (work->effect->side == 1)
				{
					blitters[0](canvas, work->sheet + 0x1e00, 16, 16, 80, 104);
				}
				else
				{
					blitters[0](canvas, work->sheet + 0x1e00, 32, 16, 80, 104);
				}
				if (frame == 6 || frame == 7)
				{
					blitters[0](canvas, work->sheet + 0x3e80, 0, 16, 128, 91);
				}
				if (frame == 8 || frame == 9)
				{
					DrawCroppedImage(canvas, Ram_MapCellBuffer, 0, 16, 128, &blitters[0]);
				}
				if (frame == 10 || frame == 11)
				{
					blitters[0](canvas, Ram_MapCellBuffer + 0x2d80, 0, 16, 128, 59);
				}
				if (frame == 12 || frame == 13)
				{
					blitters[0](canvas, Ram_MapCellBuffer + 0x4b00, 0, 16, 128, 29);
				}
				if (frame == 14 || frame == 15)
				{
					blitters[0](canvas, Ram_MapCellBuffer + 0x5980, 0, 16, 128, 26);
				}
			}
		}
		else if (kind == 20)
		{
			// 1 フレームおきに 12 個、左右へ広がりながら 2 フレームずつ 6 コマ
			for (i = 0; i != 12; i++)
			{
				if (frame >= i + 6 && frame < i + 18)
				{
					s32 image = (frame - i - 6) / 2;
					s32 image_x = target_screen->x / 2 - PuffArc_CellWidths[image] / 2;
					s32 mirrored;
					if (i & 1)
						image_x += (1 + i) / 2 * 3;
					else
						image_x -= (i + 1) / 2 * 3;
					mirrored = 1;
					if (i != 0)
					{
						mirrored = 0;
						if (((i - 1) & 3) > 1)
							mirrored = 1;
					}
					blitters[mirrored](canvas, Ram_MapCellBuffer + PuffArc_CellSourceOffsets[image],
						image_x, PuffArc_CellBiasY[image] + 48, PuffArc_CellWidths[image],
						PuffArc_CellHeights[image]);
				}
			}
		}
		else if (kind == 16)
		{
			// 64 個のかけらが対象のまわりからまっすぐ集まり、
			// 8 個おきに着くたびに対象が反応する
			if (frame == 0)
			{
				for (i = 0; i != 64; i++)
				{
					particle = &((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))[i];
					particle->x = (Random16() & 127) + 32;
					particle->y = 0;
					particle->z = 0;
					particle->velocity_x = Random16() & 0xffff;
					particle->velocity_y = Random16() & 0xffff;
					particle->velocity_z = Random16() & 0xffff;
				}
				((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))[63].y = 159;
			}
			{
				struct EffectStep	  *step = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000);
				struct EffectPosition *origin;
				/* FAKEMATCH: a zero offset known only after gcse keeps the origin copy, hoisted after the projection address. origin = target_screen, or target_screen->x and ->y read directly, differ 64 on tbs-en, two instructions fewer. */
				s32 bias = 0;
				for (i = 0; i != 64; i++, step++)
				{
					if (step->x >= 0 && frame >= i / 2)
					{
						s32 image = i & 3;
						Render_ResetTransformState();
						SceneTransform_ApplyPitch(step->velocity_x);
						SceneTransform_ApplyYaw(step->velocity_y);
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
						/* FAKEMATCH: the zero bias above; see there. */
						origin = (struct EffectPosition *)((u8 *)target_screen + bias);
						projected.x = projected.x / 2 + origin->x / 2;
						projected.y += origin->y + 32;
						blitters[1](canvas, Ram_MapCellBuffer + CastingImpact_OrbitCells[image],
							projected.x - 4, projected.y - 4, 8, 8);
						step->x -= 6;
						if (step->x < 0 && ((i & 7) == 0 || i == 63))
						{
							AudioCommand_PlayFar(133);
							ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
						}
					}
				}
			}
		}
		else if (kind == 8)
		{
			// 地面から伸びて縮む 32 幅の絵（96 まで）
			s32 height;
			if (frame >= 5 && frame <= 49)
			{
				if (frame > 25)
				{
					height = 196 - (frame << 2);
				}
				else
				{
					height = (frame << 4) - 64;
				}
				if (height > 96)
				{
					height = 96;
				}
				DrawImage(canvas, Ram_MapCellBuffer, 48, 104 - height, 32, height, &blitters[0]);
			}
		}
		else if (kind == 33 || kind == 34)
		{
			// 斜め上から対象へ落ちる 32x64 の絵（6 フレーム）
			s32 offset;
			s32 image_x;
			s32 image_y;
			if (frame <= 5)
			{
				/* FAKEMATCH: needed to match: calculate each center before the shared centered draw 2026-10-11: target_screen->x / 2 in each arm differs 147; with offset set first 129; hoisted out of the arms 146. */
				if (work->effect->side == 0)
				{
					image_x = target_screen->x;
					image_x += (u32)image_x >> 31;
					image_x >>= 1;
					offset = (6 - frame) * 3;
					image_x += offset * 2;
					image_y = target_screen->y - offset * 4 + 24;
				}
				else
				{
					image_x = target_screen->x;
					image_x += (u32)image_x >> 31;
					image_x >>= 1;
					offset = (6 - frame) * 3;
					image_x -= offset * 2;
					image_y = target_screen->y - offset * 4 + 24;
				}
				DrawTallImage(
					canvas, Ram_MapCellBuffer, image_x - 16, image_y - 32, 32, &blitters[1]);
			}
		}
		else if (kind == 12)
		{
			// 16 個の粒に 24x24 の絵を 3 種、48 フレーム目から薄くする
			if (frame > 47)
			{
				REG_BLDALPHA = (64 - frame) | 0x1000;
			}
			{
				struct EffectStep *step = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000);
				for (i = 0; i != 16; i++, step++)
				{
					s32 image = i % 3;
					EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
					projected.x /= 2;
					blitters[i & 1](canvas, Ram_MapCellBuffer + image * 576, projected.x - 12,
						projected.y - 12, 24, 24);
					EffectStep_AdvanceWithGravity3D(step, 60, 1 << ((i & 3) + 11));
					step->variant++;
				}
			}
		}
		else if (kind != 100)
		{
			// そのほか：粒ごとに 24x48 の絵を 4 フレームずつ 6 コマ
			struct EffectStep *step = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000);
			for (i = 0; i != 16; i++, step++)
			{
				if (frame >= i + 4)
				{
					s32 age = step->variant;
					if (age <= 23)
					{
						s32 image = age / 4;
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
						projected.x /= 2;
						blitters[i & 1](canvas, Ram_MapCellBuffer + image * 1152, projected.x - 12,
							projected.y - 24, 24, 48);
						if (kind == 25)
							EffectStep_AdvanceWithGravity3D(step, 60, 0x400);
						else
							EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
						step->variant++;
					}
				}
			}
		}
		// ジン（0〜7）：6 フレームで対象へ飛ぶ
		if (kind <= 7)
		{
			if (frame <= 5)
			{
				EffectPosition_ApplyBaseAndYOffset((s32 *)position, &projected);
				projected.x /= 2;
				djinn_x = projected.x;
				draw_pair[1](
					canvas, Ram_MapCellBuffer + 0x3c56, djinn_x - 10, projected.y - 4, 20, 40);
				position->x += motion->x;
				position->y += motion->y;
				position->z += motion->z;
			}
		}
		// 当たり：6 フレーム目に技ごとの揺れ、6・14 フレーム目に対象が反応する
		if (frame == 3)
		{
			BattleEventRuntime_BeginPhaseFar(-1);
		}
		if (frame == 4)
		{
			AudioCommand_PlayFar(134);
		}
		if (frame == 6)
		{
			if (kind == 4 || kind == 5 || kind == 7 || kind == 13 || kind == 18 || kind == 19
				|| kind == 23 || kind == 34 || kind == 100)
			{
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
				work->shake_frames = 8;
			}
			else if (kind == 20 || kind == 14 || kind == 33)
			{
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
				work->shake_frames = 2;
			}
			else if (kind == 30 || kind == 8)
			{
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 3);
				work->shake_frames = 8;
			}
		}
		if (frame == 6)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
		}
		if (frame == 14)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	}
	if (kind == 21)
	{
		Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
		Iwram_ClearWords(canvas, 0x4000);
		work->effect->prelude = 0;
		Scheduler_RemoveCallback(Palette_StepFadeTransfer);
		Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleFx_RunPaletteRampMode1(command);
	}
	else
	{
		if (kind == 2 || kind == 3 || kind == 12 || kind == 22 || kind == 28 || kind == 29)
		{
			Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
		}
		Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleFx_EndCanvasLayer();
	}
}

void BattleFx_RunRipplingDisc(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 screen_y;
	s32						 screen_x;
	DrawRectangle			 draw[2];
	s32						 x;
	s32						 y;
	s32						 dx;
	s32						 dy;
	s32						 cy;
	s32						 d;
	s32						 i;
	s32						 t;
	s32						 r;
	s32						 g;
	s32						 b;
	s32						 frame;
	s32						 wave;
	s32						 level;
	u16						*pal;
	s32						 angle;
	s32						*line;

	heap_cache = gBattleFxWork;
	work = *heap_cache++;
	canvas = *heap_cache;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0x2000);
	BattleFx_PrepareCanvasEffect(effect, 6, work->effect->side, 2, &screen_x, &screen_y);
	REG_BG2CNT = 0x2784;
	REG_BLDALPHA = 0x1000;
	REG_BG2PA = 0xaa;
	BattleFx_FetchRectangleBlitters(work->effect->side, draw);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (y = 0; y != 64; y++)
	{
		for (x = 0; x != 64; x++)
		{
			cy = y / 8 + 64;
			dy = y - cy;
			dx = x - 64;
			d = Iwram_Sqrt(dx * dx + dy * dy);
			d /= 2;
			if (d == 0)
				d = 1;
			if (d > 63)
				d = 63;
			work->sheet[y * 128 + x] = d;
			work->sheet[y * 128 + 127 - x] = d;
			work->sheet[(127 - y) * 128 + x] = d;
			work->sheet[(127 - y) * 128 + 127 - x] = d;
		}
	}

	pal = (u16 *)Ram_MapCellBuffer;
	for (i = 1; i != 64; i++)
	{
		if (i > 31)
			t = 64 - i;
		else
			t = i;
		r = t * 9;
		g = t * 7 - 42;
		b = t * 7 - 56;
		if (r < 0)
			r = 0;
		if (g < 0)
			g = 0;
		if (b < 0)
			b = 0;
		if (r > 255)
			r = 255;
		if (g > 255)
			g = 255;
		if (b > 250)
			b = 250;
		r >>= 3;
		g >>= 3;
		b >>= 3;
		pal[i] = ((u16 *)BG_PLTT)[i] = b << 10 | g << 5 | r;
	}

	draw[0](canvas, work, 0, 0, 128, 128);
	work->transfer_pending = 1;
	Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);

	for (frame = 0; frame != 96; frame++)
	{
		if (frame <= 8)
		{
			level = frame * 2;
			wave = level;
			REG_BLDALPHA = level | 0x1000;
		}
		else
		{
			wave = frame * 2;
		}
		if (frame > 88)
			REG_BLDALPHA = (0xc0 - frame * 2) | 0x1000;

		line = work->bg2_x;
		for (i = 0, angle = -(wave << 9); i != 160; i++)
		{
			*line++ = ((i << 18) - (Trig_Sin(angle) << 7) + 0x40000) >> 10;
			angle += 0x200;
		}

		if (frame > 127)
		{
			work->transfer_pending = 1;
		}
		else
		{
			pal[1] = pal[63];
			Dma_Set(pal + 62, pal + 63, 0x80a0003e, REG_DMA3);
			IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT[1], (u32)(pal + 1), 0x8000003f);
		}
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void ObjectGroup_StoreObjectAndRunStep7(s32 a0, s32 a1, s32 a2)
{
	struct BattleEffectWork *work;
	u8						 slot12[4];
	u8						 slot8[4];

	work = gBattleFxWork[0];
	work->effect = (struct BattleEffectArgument *)a0;
	BattleFx_BeginCanvasLayer(0);
	BattleFx_PrepareCanvasEffect(a0, 7, work->effect->side ^ 1, 0, slot12, slot8);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunFireSwirlCharge(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	u8						*graphics;
	DrawRectangle			 draw[2];
	struct EffectPosition	 position;

	struct EffectStep *sparks;
	struct EffectStep *spark;
	s32				  *life;
	s32				   frame;
	s32				   i;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	graphics = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100;
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_FireSwirlSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet, work->sheet + 0x3e80, 1, 0);
	BattleFx_FetchRectangleBlitters(work->effect->side, draw);
	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	life = &gMapCellBuffer->variant;
	for (i = 0; i != 1024; i++)
	{
		*life = 0;
		life += 7;
	}
	sparks = gMapCellBuffer;

	{
		struct MotionObject *object = GetBattleObjectSlotFar(work->effect->actor)->object;
		struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
		s32					 diff_x = target->x - object->x;
		s32					 start_x = object->x;
		s32					 delta_x = diff_x * 80 / 100;
		s32					 diff_z = target->z - object->z;
		s32					 start_z = object->z;
		s32					 delta_z = diff_z * 80 / 100;
		s32					 x = start_x + delta_x;
		s32					 z = start_z + delta_z;
		s32					 short_x = delta_x >> 8;
		s32					 short_z = delta_z >> 8;
		s32					 distance;

		distance = Iwram_Sqrt(short_x * short_x + short_z * short_z);
		distance = (distance << 8) / 20;
		object->acceleration = distance;
		object->speed_limit = distance;
		object->snap_to_target = 1;
		object->velocity_y = 0x70000;
		object->vertical_motion_strength = 0xdeb8;
		object->vertical_motion_phase = 0;
		object->auto_face_motion = 1;
		ObjectDispatch_InitFromTable6Far(object);
		Object_SetMoveTargetFar(object, x, 0, z);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
	}

	for (frame = 0; frame != 70; frame++)
	{
		EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &position);
		REG_BG2X = (80 - position.x) << 8;
		if (frame >= 8 && frame <= 23)
		{
			s32 cell = (frame - 8) / 2;

			if (cell > 6)
				cell = 6;
			if (work->effect->side == 0)
			{
				draw[0](canvas, work->sheet + FireSwirl_CellSourceOffsets[cell],
					FireSwirl_CellBiasX[cell] + 30, FireSwirl_CellBiasY[cell] + position.y - 60,
					FireSwirl_CellWidths[cell], FireSwirl_CellHeights[cell]);
			}
			else
			{
				draw[0](canvas, work->sheet + FireSwirl_CellSourceOffsets[cell],
					-FireSwirl_CellBiasX[cell] - FireSwirl_CellWidths[cell] + 108,
					FireSwirl_CellBiasY[cell] + position.y - 60, FireSwirl_CellWidths[cell],
					FireSwirl_CellHeights[cell]);
			}
		}
		if (frame == 18)
		{
			BattleEventRuntime_BeginPhaseFar(134);
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 6);
			work->shake_frames = 4;
			for (i = 0; i != 16; i++)
			{
				s32 force = (Random16() & 63) + 256;
				s32 angle = Random16() & 0xffff;

				spark = &sparks[i];
				spark->x = 0x400000;
				spark->y = 0x500000;
				spark->velocity_x = (Trig_Sin(angle) * force) >> 7;
				spark->velocity_y = -(Trig_Cos(angle) * force) >> 6;
				spark->variant = (Random16() & 15) + 16;
			}
		}
		for (i = 0; i != 128; i++)
		{
			spark = &sparks[i];
			if (spark->variant > 0)
			{
				spark->variant--;
				EffectStep_AdvanceWithGravity2D(spark, 60, 0);
				if (spark->y > 0x680000)
				{
					spark->velocity_y = -spark->velocity_y / 2;
				}
				else if ((spark->x >= 0 && spark->x <= 0x7effff) && spark->y >= 0)
				{
					s32 sx = spark->x >> 16;
					s32 sy = spark->y >> 16;

					draw[0](canvas, work->sheet + 0x3e80 + (((frame + i) / 4 % 6) << 8), sx - 8,
						sy - 8, 16, 16);
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

//--------------------------------------------------------------------------
// place_formation_objects.c
// バトル効果：オブジェクトの並べかたと、着弾の閃光・火花
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 並べかた
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// work->objects を formation の形に並べる
// formation : 0 3×3 を 32 ドットおき、1 12 個（1・3・4・4 列）、
//             2 8 個（3・2・3 列）、3 8 個を 1.25 倍で
// x, z      : 並びの基準（16.16 ドット）
//--------------------------------------------------------------------------
void BattleFx_PlaceFormationObjects(s32 formation, s32 x, s32 z)
{
	struct BattleEffectWork *work = gBattleFxWork[0];
	struct Scale			 normal = (*(const struct Scale *)&Formation_NormalScale);
	struct Scale			 big = (*(const struct Scale *)&Formation_SmallScale);
	struct Placement		 pos;
	s32						 i;

	pos.unknown_0c = 0;
	pos.y = 255 << 16;
	switch (formation)
	{
	case 0:
		for (i = 0; i != 9; i++)
		{
			pos.x = (i % 3 << 21) + x; // 32 ドットおき
			pos.z = (i / 3 << 21) + z;
			Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
		}
		break;
	case 1:
		for (i = 0; i != 12; i++)
		{
			pos.x = (Formation_TwelveX[i] << 16) + x - (16 << 16);
			pos.z = (Formation_TwelveZ[i] << 16) + z - (32 << 16);
			Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
		}
		break;
	case 2:
		for (i = 0; i != 8; i++)
		{
			pos.x = (Formation_RightEightX[i] << 16) + x + (16 << 16);
			pos.z = (Formation_RightEightZ[i] << 16) + z;
			Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
		}
		break;
	case 3:
		for (i = 0; i != 8; i++)
		{
			pos.x = (Formation_SmallEightX[i] << 16) + x;
			pos.z = (Formation_SmallEightZ[i] << 16) + z;
			Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &big, 0);
		}
		break;
	}
}

#define HAHEN_E 0x59d8 // 破片の絵の置き場（12×24 が 4 枚）

//--------------------------------------------------------------------------
// 着弾点で閃光をひらき、火花と破片を散らす（72 フレーム）
// はじめの 36 フレームは、オブジェクトも formation に並べなおす
// formation : BattleFx_PlaceFormationObjects の並べかた
// x, y      : 着弾点（16.16 ドット）
//--------------------------------------------------------------------------
void BattleEffect_RunImpactBurst(s32 formation, s32 x, s32 y)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 kasan;
	DrawRectangle			 hikaku;
	u8						*sheet;
	s32						 org_x;
	struct EffectStep		*p;
	s32						 frame;
	s32						 i;

	// ワーク、キャンバス、BG2 は横 2 倍で BG1・BG2 を半透明に
	heap = gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap[2];
	org_x = x;
	x += 40 << 16;
	x /= 2; // キャンバスは横半分
	REG_BG2PA = 0x80;
	REG_BG2X = 0;
	REG_BLDCNT = 0x3f46;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP, GOUSEI_HIKAKU);
	hikaku = heap[7];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP, GOUSEI_KASAN);
	kasan = heap[8];
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_FlareSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_FlareImage, work->sheet + HAHEN_E, 0, 0);
	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 大きい火花：速さ 256〜511、寿命 16〜31 フレーム（使うのは前の 30 個）
	for (i = 0; i != 64; i++)
	{
		s32 speed = (Random16() & 255) + 256;
		s32 angle = Random16() & 0xffff;

		work->particles[i].x = x;
		work->particles[i].y = y;
		work->particles[i].velocity_x = (Trig_Sin(angle) * speed) >> 7;
		work->particles[i].velocity_y = -((Trig_Cos(angle) * speed) >> 6);
		work->particles[i].variant = (Random16() & 15) + 16;
	}

	// 破片 3 つを 120 度おきに
	for (i = 0; i != 3; i++)
	{
		work->particles[61 + i].x = x;
		work->particles[61 + i].y = y;
		work->particles[61 + i].velocity_x = (Trig_Sin(i * 0x5555) * 32) >> 6;
		work->particles[61 + i].velocity_y = -((Trig_Cos(i * 0x5555) * 32) >> 5);
	}

	// 小さい火花：速さ 32〜287、寿命 20〜35 フレーム
	for (i = 0; i != 64; i++)
	{
		s32 speed = (Random16() & 255) + 32;
		s32 angle = Random16() & 0xffff;

		gMapCellBuffer[i].x = x;
		gMapCellBuffer[i].y = y;
		gMapCellBuffer[i].velocity_x = (Trig_Sin(angle) * speed) >> 6;
		gMapCellBuffer[i].velocity_y = -((Trig_Cos(angle) * speed) >> 5);
		gMapCellBuffer[i].variant = (Random16() & 15) + 20;
	}

	for (frame = 0; frame != 72; frame++)
	{
		if (frame == 4)
			AudioCommand_PlayFar(154);
		if (frame == 32)
			AudioCommand_PlayFar(212);

		// 閃光：8 フレーム目から 5 フレームごとに次の絵
		if (frame <= 47)
		{
			s32 cell = (frame - 8) / 5;
			u32 width;
			u32 height;

			if (cell < 0)
				cell = 0;
			hikaku(canvas, work->sheet + ImpactBurst_CellSourceOffsets[cell],
				(x >> 16) - ((width = ImpactBurst_CellWidths[cell]) >> 1),
				(y >> 16) - ((height = ImpactBurst_CellHeights[cell]) >> 1), width, height);
		}

		// 大きい火花は 2 個ずつ 1 フレームおきに出る
		for (i = 0, p = work->particles; i != 30; i++, p++)
		{
			if (frame > i / 2 && p->variant > 0)
			{
				s32 size;
				s32 px;
				s32 py;

				p->variant--;
				EffectStep_AdvanceWithGravity2D(p, 60, 0);
				size = p->variant / 16 + 3;
				px = HI(p->x);
				py = HI(p->y);
				kasan(canvas, sheet + ParticleStreams_CellOffsets[size - 1], px - size / 2,
					py - size, size, size * 2);
			}
		}

		// 小さい火花は 36 フレーム目から
		for (i = 0, p = gMapCellBuffer; i != 60; i++, p++)
		{
			if (frame > 35 && p->variant > 0)
			{
				s32 size;
				s32 px;
				s32 py;

				p->variant--;
				EffectStep_AdvanceWithGravity2D(p, 60, 0);
				size = p->variant / 16 + 1;
				px = HI(p->x);
				py = HI(p->y);
				kasan(canvas, sheet + ParticleStreams_CellOffsets[size - 1], px - size / 2,
					py - size, size, size * 2);
			}
		}

		// 破片は 36〜63 フレーム、7 フレームごとに次の絵
		for (i = 0; i != 3; i++)
		{
			struct EffectStep *hahen = &work->particles[61 + i];

			if (frame >= 36 && frame <= 63)
			{
				s32 cell;
				s32 px;
				s32 py;

				EffectStep_AdvanceWithGravity2D(hahen, 64, 0);
				cell = (frame - 36) / 7;
				px = HI(hahen->x);
				py = HI(hahen->y);
				hikaku(canvas, work->sheet + cell * 0x120 + HAHEN_E, px - 6, py - 12, 12, 24);
			}
		}

		if (frame <= 35)
			BattleFx_PlaceFormationObjects(formation, org_x, y);
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	// BG の文字ブロック 1 を消して終わる
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
}

void Palette_BrightenBgEntries(s32 blue_delta, s32 green_delta, s32 red_delta)
{
	u16 *color = (u16 *)0x05000002;
	s32	 i;

	for (i = 0; i != 63; i++)
	{
		s32 blue = (*color >> 10) & 0x1f;
		s32 green = (*color >> 5) & 0x1f;
		s32 red = *color & 0x1f;

		blue += blue_delta;
		green += green_delta;
		red += red_delta;
		if (blue > 31)
			blue = 31;
		if (green > 31)
			green = 31;
		if (red > 31)
			red = 31;
		*color++ = (blue << 10) | (green << 5) | red;
	}
}

void BattleFx_ArmPaletteHBlankDma(void)
{
	struct BattleEffectWork *work = gBattleFxWork[0];
	volatile u16			*channel = REG_DMA0;
	channel[5] &= 0xc5ff;
	channel[5] &= 0x7fff;
	(void)channel[5];
	Dma_Set(work->sheet + 0x1f80, (void *)0x05000000, 0xa2600001, (volatile u32 *)channel);
}

//--------------------------------------------------------------------------
// particle_stream_modes.c
// 戦闘エフェクト：粒子の流れのモード 0・1 の入口
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 粒子の流れをモード 0 で走らせる
//--------------------------------------------------------------------------
void BattleFx_InitializeDefaultMode(struct BattleEffectArgument *efx)
{
	BattleEffect_RunParticleStreams(efx, 0);
}

//--------------------------------------------------------------------------
// 粒子の流れをモード 1 で走らせる
//--------------------------------------------------------------------------
void BattleFx_InitializeMode1(struct BattleEffectArgument *efx)
{
	BattleEffect_RunParticleStreams(efx, 1);
}

extern const struct Scale CirclingScene_UnitScale;

extern s16 ParticleStreams_OrbitPoints[][3];
extern s16 ParticleStreams_DropPoints[][2];
extern u16 ParticleStreams_FlashSheetOffsets[];
extern u16 ParticleStreams_FlashSizes[];

#define EMBERS (&PARTICLES[128])

void BattleEffect_RunParticleStreams(struct BattleEffectArgument *effect, s32 mode)
{
	volatile u32			 fill;
	u8						 offsets[128];
	void				   **cache;
	void					*canvas;
	DrawRectangle			 draw;
	s32						 direction;
	struct BattleEffectWork *work;
	void					*colors = (void *)0x05000000;

	void AddFlame(s32 x, s32 y, s32 velocity_x)
	{
		s32 i;

		for (i = 0; i != 16; i++)
		{
			struct EffectStep *flame = &work->particles[32 + i];

			if (flame->variant == -1)
			{
				flame->variant = 0;
				flame->x = x;
				flame->y = y;
				flame->velocity_x = velocity_x;
				break;
			}
		}
	}

	void AddMote(s32 x, s32 y)
	{
		s32 i;

		for (i = 0; i != 32; i++)
		{
			struct EffectStep *mote = &work->particles[i];

			if (mote->variant == -1)
			{
				mote->variant = 0;
				mote->x = x;
				mote->y = y;
				break;
			}
		}
	}

	s32					   position[4];
	s32					   vector[3];
	struct EffectPosition  spot;
	struct EffectPosition *pos;
	struct EffectPosition  point;
	struct Scale		   scale;
	s32					   frame;
	s32					   i;

	cache = &gBattleFxWork[1];
	canvas = cache[0];
	work = cache[-1];

	work->effect = effect;
	BattleFx_BeginCanvasLayer(0x2000);
	REG_BG2PA = 0x100;
	/* FAKEMATCH: test through direction so CSE retains the mode reload
	 * in this arm. Testing mode directly replaces ldr r4,[sp,#76] with
	 * movs r4,#1 (1532 instructions, two differing rows). 2026-10-13:
	 * a switch on mode, side tested by if/else or `side != 1 ? -1 : mode`
	 * differ the same 2; direction = -1 first and set in the arm 58. */
	direction = mode;
	if (direction == 1)
	{
		struct MotionObject *object = GetBattleObjectSlotFar(work->effect->actor)->object;

		object->velocity_y = 0xa0000;
		object->vertical_motion_strength = 0x91eb;
		ObjectGroup_UpdateMembers(work->effect->actor, -1, 2, -1, 0);
		AudioCommand_PlayFar(145);
		direction = work->effect->side == 1 ? mode : -1;
	}
	else
	{
		direction = -1;
	}
	BattlePres_ConfigureEffectDisplay();
	PLTT_BACKDROP = 0;
	*(u16 *)0x05000002 = 0;
	work->transfer_mode = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_WipeCanvas(0, 0);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	if (mode == 1)
	{
		for (i = 0; i != 2; i++)
		{
			struct AnimationObject *object = ResourceObject_CreateFar(483 + i * 0x2001);

			work->objects[i] = object;
			if (object != 0)
			{
				object->flags = 0;
				AnimationObjects_SelectAnimationFar(object, 2);
				((struct AnimationObject *)work->objects[i])->part[0].priority = 3;
			}
		}
	}
	else
	{
		BattleFx_SpawnObjects(1, 381, 3);
	}
	Resource_LoadAndDecompress((s32)&ResourceId_FlameSheetB, work, 1, 1);
	if (mode == 1)
		Iwram_CopyWords(colors, Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet), 128);
	fill = 0x01010101;
	Dma_Set((void *)&fill, Ram_MapCellBuffer, 0x85002000, REG_DMA3);
	Iwram_CopyWords((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
	WaitFrames(1);
	REG_BLDCNT = 0;
	REG_BG2PA = 0x100;
	REG_BG1CNT = 0x1f80;
	REG_BG2CNT = 0x2787;
	for (i = 0; i != 63; i++)
	{
		s32 red = (Random16() & 15) + 16;
		s32 green = (Random16() & 15) + 16;
		s32 blue = (Random16() & 15) + 16;

		((u16 *)0x05000100)[i] = (blue << 10) | (green << 5) | red;
	}
	fill = 0;
	Dma_Set((void *)&fill, canvas, 0x85001000, REG_DMA3);
	for (i = 0; i != 256; i++)
	{
		s32 x = Random16() & 127;
		s32 y = Random16() & 127;
		s32 shade = (Random16() & 63) + 64;

		((u8 *)canvas)[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = shade;
	}
	Iwram_CopyWords((void *)0x06004000, canvas, 0x4000);
	gProjection.center_y = 240;
	BattleFx_SelectLivingTargets(work->effect);
	work->bg2x = 0;
	work->bg2y = 0;
	work->scroll_timer = 0;
	work->scroll_interval = 2;
	work->scroll_step_x = direction << 7;
	work->scroll_step_y = 256;
	Scheduler_AddOrUpdateCallback(Camera_AdvanceBg2Reference, 0x4ff);
	Scheduler_AddOrUpdateCallback(BattleFx_FlushPendingGraphicsTransfer, 0x480);
	for (i = 0; i != 128; i++)
		offsets[i] = Random16() & 63;
	{
		s32 limit = 0;
		s32 step = 1;
		s32 row = 0;

		do
		{
			limit += step / 2;
			step += 4;
			for (; row != limit; row++)
			{
				s32 x;

				for (x = 0; x != 256; x++)
				{
					s32 y = row - offsets[x & 127];

					if (y >= 0)
						if (y <= 127)
							Ram_MapCellBuffer[((y / 8 * 32 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] =
								0;
				}
			}
			work->transfer_pending = 1;
			WaitFrames(1);
		} while (limit <= 191);
	}
	REG_BLDCNT = 0x3f42;
	REG_BLDALPHA = 0x1010;
	{
		s32							   saved_x = gBgScroll[1].x;
		s32							   saved_y = gBgScroll[1].y;
		struct BattlePresentationWork *presentation = ((void **)&gTransitionWork)[0];

		gBgScroll[1].x = 0;
		gBgScroll[1].y = 32;
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 8, 7, 3, 2);
		draw = ((void **)&gTransitionWork)[2];
		work->transfer_mode = 3;
		work->transfer_value = (s32)(Ram_MapBlocks + 0x202);
		Scheduler_AddOrUpdateCallback(BattleFx_ArmPaletteHBlankDma, 0x4fe);
		for (i = 0; i != 64; i++)
			work->particles[i].variant = -1;
		presentation->initial_value = 1;
		work->frame = 0;
		frame = 0;
		do
		{
			s32 angle = frame << 8;
			/* FAKEMATCH: count fixed in r2, as in the reference; as a hard register it leaves the division temp canonical in cse, so the compare and +3 use the temp as in the reference. */
			register s32 count asm("r2") = work->frame;
			/* An ordinary base alone leaves 8 aligned differences in 1532
			 * instructions. Integer/word/halfword carriers, a typed inline
			 * line helper and general or early-clobber barrier outputs keep
			 * that residual. A two-minute exact-stack/branch search (seed
			 * 353, nested helpers ignored) found no improvement in 1241
			 * compiled candidates. Changing count alone differs 10, both
			 * count and base differ 74, and level alone differs 42. */
			/* FAKEMATCH: work's copy in r0; else it and the 0x778c reload swap r0/r1. 2026-10-11: plain `heat = work->frame / 4` with a plain line pointer differs 69: combine merges count's copy, the 0x778c reload and work swap r0/r1, and the halfword zero becomes a literal-pool load once its reload scratch moves. */
			register u8 *base asm("r0") = (u8 *)work;
			s32			 heat = count / 4;
			u16			*line;
			s32			 size;
			s32			 x;
			s32			 y;

			/* FAKEMATCH: count read and base passed through; reading count after the division keeps its load apart from the division temp (no combine); through base the asm is not volatile, so no sched2 barrier, and the mode reload lands after movs r2, #252 as in the reference (38 -> 2). */
			asm("" : "=r"(base) : "0"(base), "r"(count));
			line = (u16 *)(base + 0x1f80);
			if (mode == 1)
			{
				if ((gKeysRepeat & 3) && frame > 16)
					break;
			}
			else
			{
				if ((gKeysRepeat & 3) && frame > 4)
					break;
			}
			{
				s32 hold;

				/* FAKEMATCH: hold takes r7 across the call; with line in r5, hold in r7 and r6 clobbered, heat takes r4 and its sp+8 caller-save as in the reference (222 -> 89); global.c ignores hard conflicts with r7, the frame pointer, so r7 needs a pseudo. 2026-10-11: a plain call differs 354, heat in r6 with no caller-save and the frame 280 instead of 284. */
				asm("" : "=r"(hold));
				/* FAKEMATCH: r6 is clobbered across the call. */
				asm("" : : : "r6");
				if (frame == 0)
					AudioCommand_PlayFar(141);
				/* FAKEMATCH: hold's use after the call. */
				asm("" : : "r"(hold));
			}
			for (i = 0; i != 15; i++)
				*line++ = 0;
			for (; i != 135; i++)
			{
				s32 green = i - 16;
				/* FAKEMATCH: reuse red for the biased signed quotient. A
				 * separate quotient coalesces with level; the explicit bias
				 * keeps the native r3 copy and shift (four register rows). */
				s32 red = green;
				/* Ordinary level differs 42 in EN (1532 native/built).
				 * A generic output boundary after its sum reduces this to 26,
				 * retaining the quotient/level r2/r3 exchange. A red input
				 * retains 26; also unbinding count differs 36, with either an
				 * input or output count operand on the existing base boundary.
				 * Reversing the sum or deriving green as red - 48 retains 42. */
				/* FAKEMATCH: level in r2; unbinding it changes ten register/order rows. 2026-10-11: the natural `level = green / 4 + heat` differs 18, a level/red r3/r2 swap only (local-alloc hands level r3 first); a use of level after the first red clamp matches, and moving green's computation there differs 4; no plain statement survives to allocation there. */
				register s32 level asm("r2");

				if (green < 0)
					red += 3;
				red >>= 2;
				level = red + heat;
				red = level - 32;
				green = level - 80;

				if (red < 0)
					red = 0;
				if (red > 31)
					red = 31;
				if (green < 0)
					green = 0;
				if (green > 31)
					green = 31;
				*line++ = (red << 10) | (green << 5) | (green >> 1);
			}
			for (; i != 160; i++)
				*line++ = 0;
			if (direction == 1)
				x = frame / 4;
			else
				x = 64 - frame / 4;
			y = 96 - frame;
			position[3] = 0;
			position[1] = 0xff0000;
			if (mode == 1)
			{
				size = angle + 0xa000;
				scale.x = size;
				scale.y = size;
				position[0] = (x << 16) + 0x500000;
				position[2] = (64 - y) << 16;
				Object_ApplyProjectedPlacementFar(work->objects[0], position, &scale, 0);
				Object_ApplyProjectedPlacementFar(work->objects[1], position, &scale, 0);
			}
			else
			{
				size = angle + 0x10000;
				scale.x = size;
				scale.y = size;
				position[0] = (x << 16) + 0x600000;
				position[2] = (96 - y) << 16;
				Object_ApplyProjectedPlacementFar(work->objects[0], position, &scale, 0);
			}
			for (i = 0, y = 32 - y; i != 32; i++)
			{
				struct EffectStep *mote = &work->particles[i];

				if (mote->variant == -1)
				{
					s32 angle = (Random16() & 0x7fff) + 0x4000;

					mote->variant = 0;
					mote->x = ((x + 96) << 16) + Trig_Sin(angle) * 30 / 65536 * size;
					mote->y = (y << 16) - Trig_Cos(angle) * 30 / 65536 * size;
					break;
				}
			}
			point.x = 0;
			point.y = 0;
			point.depth = 0x2000000;
			Render_ResetTransformState();
			SceneTransform_ApplyPosition(&point.x);
			SceneTransform_ApplyRoll(0x800);
			SceneTransform_ApplyYaw(frame << 8);
			for (i = 0; i != 7; i++)
			{
				vector[0] = (ParticleStreams_OrbitPoints[i][0] / 2) << 16;
				vector[1] = (ParticleStreams_OrbitPoints[i][1] + frame) << 16;
				vector[2] = (ParticleStreams_OrbitPoints[i][2] / 2) << 16;
				EffectPosition_ApplyBaseAndYOffset(vector, pos = &spot);
				pos->x = HI(pos->x) + 128;
				pos->y = HI(pos->y) + 60;
				draw(Ram_MapCellBuffer, (u8 *)work + 0x1f40, pos->x - 4, pos->y - 4, 8, 8);
			}
			if (direction == 1)
				x = frame / 4 - 16;
			else
				x = 16 - frame / 4;
			y = frame - 96;
			for (i = 0; i != 7; i++)
			{
				s32 drop = ParticleStreams_DropPoints[i][1] + y;

				if (drop <= 93)
					draw(Ram_MapCellBuffer, (u8 *)work + 0x1c80,
						ParticleStreams_DropPoints[i][0] + x - 12, drop - 12, 24, 24);
				else if (drop <= 95)
					AddFlame((ParticleStreams_DropPoints[i][0] + x) << 16, drop << 16, 1);
			}
			for (i = 0; i != 32; i++)
			{
				struct EffectStep *mote = &work->particles[i];

				if (mote->variant >= 0)
				{
					draw(Ram_MapCellBuffer, (u8 *)work + (mote->variant << 10), HI(mote->x) - 16,
						HI(mote->y) - 16, 32, 32);
					mote->x -= direction * 0x14000;
					mote->y -= 0x50000;
					mote->variant++;
					if (mote->variant == 6)
						mote->variant = -1;
				}
			}
			work->transfer_pending = 1;
			WaitFrames(1);
			frame++;
			work->frame++;
		} while (frame != 192);
		WaitFrames(1);
		presentation->initial_value = 0;
		Scheduler_RemoveCallback(Camera_AdvanceBg2Reference);
		Scheduler_RemoveCallback(BattleFx_ArmPaletteHBlankDma);
		Scheduler_RemoveCallback(BattleFx_FlushPendingGraphicsTransfer);
		gBgScroll[1].x = saved_x;
		gBgScroll[1].y = saved_y;
	}
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleEffect_SetupBlendedDisplay();
	REG_BG2PA = 0x80;
	REG_BG2X = 0;
	REG_BG2Y = 0xfffff000;
	REG_BLDALPHA = 0x1010;
	REG_BG2CNT = 0x2784;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw = gWorkSlot[HEAP_SLOT_BLITTER];
	Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work, 1, 0);
	for (i = 0; i != 32; i++)
	{
		struct EffectStep *mote = &work->particles[i];

		mote->x = Random16() & 127;
		mote->y = (Random16() & 127) + 127;
	}
	for (i = 0; i != 128; i++)
	{
		struct EffectStep *flash = &PARTICLES[i];

		flash->x = 0;
		flash->y = 0;
		flash->z = 0;
		flash->velocity_x = ((Random16() & 255) - 127) << 12;
		flash->velocity_y = (Random16() & 255) << 11;
		flash->velocity_z = ((Random16() & 255) - 127) << 12;
		flash->variant = 0;
	}
	for (i = 0; i != 512; i++)
	{
		struct EffectStep *ember = &EMBERS[i];

		ember->x = 0;
		ember->y = 0;
		ember->z = 0;
		ember->velocity_x = ((Random16() & 255) - 128) << 13;
		ember->velocity_y = (Random16() & 255) << 11;
		ember->velocity_z = ((Random16() & 255) - 128) << 13;
		ember->variant = 0;
	}
	work->transfer_mode = 1;
	work->transfer_value = 0x10101010;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != 54; frame++)
	{
		s32					 size = (frame + 0x1d0) << 8;
		struct BattleCamera *camera = gCameraWork;
		s32					 elapsed = frame - 16;
		struct Scale		 scale;
		s32					 x;
		s32					 y;
		s32					 grow;

		if (elapsed > 19)
			Palette_BrightenBgEntries(2, 2, 2);
		if (frame == 0)
			AudioCommand_PlayFar(156);
		if (frame == 40)
			AudioCommand_PlayFar(145);
		if (frame == 48)
		{
			if (mode == 1)
			{
				ResourceObject_ReleaseFar(work->objects[0]);
				ResourceObject_ReleaseFar(work->objects[1]);
				BattleActor_CommitPlacementFar();
			}
			BattleEventRuntime_BeginPhaseFar(134);
		}
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		for (i = 0; i != 64; i++)
		{
			struct EffectStep *ember = &EMBERS[i];

			if (ember->y >= 0)
			{
				s32 size;

				EffectPosition_ApplyBaseAndYOffset(&ember->x, &point);
				point.x >>= 1;
				if (point.depth <= 159)
					point.depth = 160;
				if (point.depth > 799)
					point.depth = 799;
				size = 9 - (point.depth - 160) / 64;
				draw(canvas,
					(u8 *)work + (ParticleStreams_CellOffsets[size - 1] + (i & 1) * 770) + 0x3200,
					point.x - size / 2, point.y - size, size, size * 2);
				EffectStep_AdvanceWithGravity3D(ember, 64, -0x2000);
				if (ember->y <= 0x140000)
				{
					ember->x = 0;
					ember->z = 0;
					ember->y = 0x140000;
					ember->velocity_x = ((Random16() & 63) - 32) << 15;
					ember->velocity_y = (Random16() & 63) << 13;
					ember->velocity_z = ((Random16() & 63) - 32) << 15;
				}
			}
		}
		for (i = 0; i != 64; i++)
		{
			struct EffectStep *mote = &work->particles[i];
			u32				   rise = i & 7;
			s32				   size = rise + 3;

			draw(canvas,
				(u8 *)work + (ParticleStreams_CellOffsets[size - 1] + (i & 1) * 770) + 0x3200,
				mote->x - size / 2, mote->y - size, size, size * 2);
			mote->y = mote->y - rise - 8;
			if (mote->y < -10)
				mote->y = 128;
		}
		for (i = 0; i != 64; i++)
		{
			struct EffectStep *flash = &PARTICLES[i];

			if (i / 3 < elapsed && flash->y >= 0)
			{
				EffectPosition_ApplyBaseAndYOffset(&flash->x, &point);
				point.x >>= 1;
				if ((u32)flash->variant <= 13)
				{
					s32 cell = flash->variant / 2;

					draw(canvas, (u8 *)work + ParticleStreams_FlashSheetOffsets[cell],
						point.x - ParticleStreams_FlashSizes[cell] / 2,
						point.y - ParticleStreams_FlashSizes[cell] / 2,
						ParticleStreams_FlashSizes[cell], ParticleStreams_FlashSizes[cell]);
				}
				flash->variant++;
				if (flash->variant == 14)
				{
					flash->y = 0x140000;
					flash->x = 0;
					flash->z = ((Random16() & 255) - 127) << 16;
					flash->velocity_x = 0;
					flash->velocity_y = (Random16() & 255) << 11;
					flash->velocity_z = 0;
					flash->variant = 0;
				}
				else
				{
					EffectStep_AdvanceWithGravity3D(flash, 64, 1);
				}
			}
		}
		if (direction == 1)
			x = frame / 2 + 24;
		else
			x = 56 - frame / 2;
		y = 64 - frame * 2;
		grow = (frame << 8) + 0x20000;
		position[3] = 0;
		position[1] = 0xff0000;
		if (mode == 1)
		{
			scale.x = size;
			scale.y = size;
			position[0] = (x << 16) + 0x600000;
			position[2] = (96 - y) << 16;
			Object_ApplyProjectedPlacementFar(work->objects[0], position, &scale, 0);
			Object_ApplyProjectedPlacementFar(work->objects[1], position, &scale, 0);
		}
		else
		{
			scale.x = grow;
			scale.y = grow;
			position[0] = (x << 16) + 0x600000;
			position[2] = (96 - y) << 16;
			Object_ApplyProjectedPlacementFar(work->objects[0], position, &scale, 0);
		}
		work->shake_frames = 1;
		Camera_ApplyShake(8, 8);
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	if (mode == 0)
		ResourceObject_ReleaseFar(work->objects[0]);
	BattleFx_EndCanvasLayer();
}

void BattleEffect_RunCirclingFallingScene(struct BattleEffectArgument *effect)
{
	s32						 position[4];
	DrawRectangle			 draw[2];
	struct Scale			 scale;
	void				   **cache;
	void					*canvas;
	struct BattleEffectWork *work;
	s32						 frame;
	u8						*sheet;
	struct Scale			 base;
	struct Scale			 orb;
	s32						 i;
	s32						 k;

	cache = &gBattleFxWork[1];
	canvas = cache[0];
	work = cache[-1];
	sheet = cache[1];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	BattlePres_ConfigureEffectDisplay();
	PLTT_BACKDROP = 0;
	*(u16 *)0x05000002 = 0;
	work->transfer_mode = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_WipeCanvas(1, 0);
	BattleFx_SelectLivingTargets(work->effect);
	BattleFx_SpawnObjects(9, 379, 2);
	for (i = 0; i != 6; i++)
	{
		struct AnimationObject *object = ResourceObject_CreateFar(390);

		work->objects[9 + i] = object;
		if (object != 0)
		{
			object->flags = 0;
			AnimationObjects_SelectAnimationFar(object, i % 3);
			((struct AnimationObject *)work->objects[9 + i])->part[0].priority = 1;
		}
	}
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
	draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	REG_WININ = 0x2737;
	REG_WIN0H = 0xf0;
	REG_WIN1V = 0x1088;
	WaitFrames(1);
	BattleBackground_LoadFar(1, (s32)&ResourceId_DesertBackdrop, 0);
	BattleEffect_WipeCanvas(1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 1);
	REG_DISPCNT = 0x7741;
	REG_BG2PA = 0x80;
	REG_BLDALPHA = 0x1010;
	REG_BLDCNT = 0x3f44;
	work->transfer_mode = 2;
	work->transfer_value = 50;
	base.x = 0xbc0000;
	base.y = 0x5c0000;
	orb.x = 0xa00000;
	orb.y = 0x5c0000;
	for (i = 0; i != 6; i++)
	{
		struct EffectStep *rock = &work->particles[i];

		rock->x = (Random16() & 127) << 16;
		rock->y = i * -0x100000;
		rock->velocity_x = 0;
		rock->velocity_y = 0;
		rock->variant = 0;
	}
	for (i = 0; i != 58; i++)
		work->particles[6 + i].variant = 24;
	for (i = 0; i != 1024; i++)
		PARTICLES[i].variant = -1;
	work->fade_frames = 24;
	work->fade_step = 0;

	for (frame = 0; frame != 320 && !(gKeysRepeat & (KEY_A | KEY_B)); frame++)
	{
		if (frame == 94)
			AudioCommand_PlayFar(156);
		if (frame == 136)
			AudioCommand_PlayFar(156);
		if (frame == 178)
			AudioCommand_PlayFar(156);
		if (frame == 260)
			AudioCommand_PlayFar(145);
		scale = CirclingScene_UnitScale;
		if (frame >= 96 && frame <= 251)
			work->shake_frames = 1;
		else if (frame >= 260 && frame <= 263)
			work->shake_frames = 1;
		position[3] = 0;
		position[1] = 0;
		for (i = 0; i != 7; i++)
		{
			position[0] = (CirclingScene_ObjectColumns[i] << 16) + base.x - 0x200000;
			position[2] = (CirclingScene_ObjectRows[i] << 16) + base.y - 0x200000;
			Object_ApplyProjectedPlacementFar(work->objects[i], position, &scale, 0);
		}
		if (frame <= 90)
		{
			orb.x = Trig_Sin(frame << 9) * 16 + 0x9c0000;
			orb.y = Trig_Cos(frame << 9) * 16 + 0x5c0000;
		}
		if (frame <= 196)
		{
			s32 start;

			for (i = 0, start = 91; i != 3; start += 40, i++)
			{
				if (frame >= start && frame < start + 4)
					orb.y += 0x80000;
				if (frame == start + 3)
				{
					for (k = 0; k != 4; k++)
					{
						struct EffectStep *puff = &work->particles[6 + i * 8 + k];

						puff->x = 0x400000;
						puff->y = 0x600000;
						puff->velocity_x = ((Random16() & 255) - 127) << 10;
						puff->velocity_y = ((Random16() & 255) - 127) << 10;
						puff->variant = Random16() & 15;
					}
				}
				if (frame >= start + 20 && frame < start + 36)
					orb.y -= 0x20000;
			}
		}
		if (frame >= 244 && frame <= 251)
			orb.x -= 0x10000;
		if (frame >= 252 && frame <= 275)
			orb.x -= (frame - 250) << 16;
		if (frame <= 259)
		{
			position[0] = orb.x;
			position[1] = -0x1000000;
			position[2] = orb.y - 0x1000000;
			Object_ApplyProjectedPlacementFar(work->objects[7], position, &scale, 0);
			position[0] = orb.x + 0x200000;
			Object_ApplyProjectedPlacementFar(work->objects[8], position, &scale, 0);
		}
		position[1] = 0;
		for (i = 0; i != 6; i++)
		{
			struct EffectStep *rock = &work->particles[i];

			if (rock->variant != 2)
			{
				position[0] = rock->x;
				position[2] = rock->y;
				Object_ApplyProjectedPlacementFar(work->objects[9 + i], position, &scale, 0);
				rock->x += rock->velocity_x;
				rock->y += rock->velocity_y;
				if (frame > 96)
					rock->velocity_y += 0x4000;
				if (rock->y > 0x780000)
				{
					rock->variant++;
					if (rock->variant == 1)
					{
						rock->velocity_y = -rock->velocity_y / 2;
						for (k = 0; k != 2; k++)
						{
							struct EffectStep *puff = &work->particles[30 + i * 2 + k];

							puff->x = rock->x / 2;
							puff->y = rock->y - 0x200000;
							puff->velocity_x = ((Random16() & 255) - 127) << 10;
							puff->velocity_y = ((Random16() & 255) - 127) << 10;
							puff->variant = Random16() & 15;
						}
					}
					else if (frame <= 199)
					{
						rock->y = 0;
						rock->velocity_y = 0;
						rock->variant = 0;
					}
				}
			}
		}
		for (i = 0; i != 56; i++)
		{
			struct EffectStep *puff = &work->particles[6 + i];

			if (puff->variant >= 0)
			{
				if (puff->variant >= 0 && puff->variant <= 23)
				{
					s32 cell = puff->variant / 6 + 3;

					draw[0](canvas, work->sheet + CirclingScene_PuffSheetOffsets[cell],
						HI(puff->x) - CirclingScene_PuffSizes[cell] / 2,
						HI(puff->y) - CirclingScene_PuffSizes[cell] / 2,
						CirclingScene_PuffSizes[cell], CirclingScene_PuffSizes[cell]);
				}
				EffectStep_AdvanceWithGravity2D(puff, 60, -0x4000);
				puff->variant++;
			}
		}
		if (frame == 260)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
				ObjectGroup_UpdateMembers(work->effect->actors[i], 7, -1, i, 8);
			}
			work->shake_frames = 8;
		}
		if (frame == 260)
		{
			for (i = 0; i != 512; i++)
			{
				struct EffectStep *spark = &PARTICLES[i];
				s32				   speed;
				s32				   angle;

				speed = 0x3ff;
				speed &= Random16();
				angle = Random16() & 0xffff;
				spark->x = 0x200000;
				spark->y = 0x5c0000;
				spark->velocity_x = Trig_Sin(angle) * (speed + 32) >> 7;
				spark->velocity_y = -(Trig_Cos(angle) * (speed + 32) << 1) >> 7;
				spark->variant = (Random16() & 15) + 32;
			}
		}
		for (i = 0; i != 512; i++)
		{
			struct EffectStep *spark = &PARTICLES[i];

			if (spark->variant >= 0)
			{
				s32 size = (spark->variant >> 3) + 1;

				draw[i & 1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
				EffectStep_AdvanceWithGravity2D(spark, 62, 0x1000);
				spark->variant--;
			}
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	BattleEventRuntime_BeginPhaseFar(134);
	BattleEffect_SetupBlendedDisplay();
	for (i = 0; i != 15; i++)
		ResourceObject_ReleaseFar(work->objects[i]);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleEffect_RunDualParticleStream(struct BattleEffectArgument *effect)
{
	struct Point2D			 trail[20];
	struct EffectPosition	 position;
	DrawRectangle			 draw[2];
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 drift;
	s8						*path;
	s32						 x;
	s32						 y;
	u8						*sheet;
	struct BattleCamera		*camera;
	s32						 i;
	s32						 j;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	drift = 0;
	x = 0;
	y = 0;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetB, work->sheet, 1, 1);
	for (i = 1; i != 20; i++)
	{
		for (j = 0; j != 936; j++)
		{
			s32 shade = work->sheet[0x1680 + j];

			if (i > 10)
			{
				shade = shade - i * 4 + 40;
				if (shade < 0)
					shade = 0;
				work->sheet[0x1680 + (i - 10) * 936 + j] = shade;
			}
		}
	}
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, &work->sheet[0x3c00], 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, Ram_MapCellBuffer, 1, 0);
	BattleFx_FetchRectangleBlitters(work->effect->side, draw);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (j = 0; j != 40; j++)
	{
		struct EffectStep *ember = &work->particles[8 + j];

		if (work->effect->side == 0)
			ember->x = -0x380000;
		else
			ember->x = 0x380000;
		ember->y = 0;
		ember->z = 0;
		ember->velocity_x = ((Random16() & 63) - 32) << 14;
		ember->velocity_y = (Random16() & 63) << 13;
		ember->velocity_z = ((Random16() & 63) - 32) << 14;
		ember->variant = 1;
	}
	for (j = 0; j != 16; j++)
	{
		struct EffectStep *flash = &work->particles[48 + j];

		if (work->effect->side == 0)
			flash->x = -0x380000;
		else
			flash->x = 0x380000;
		flash->y = 0x140000;
		flash->z = 0;
		flash->velocity_x = ((Random16() & 63) - 32) << 14;
		flash->velocity_y = (Random16() & 63) << 12;
		flash->velocity_z = ((Random16() & 63) - 32) << 14;
		flash->variant = 0;
	}
	for (i = 0, j = -0x4000; i != 8; i++, j += 0x1000)
	{
		struct EffectStep *puff = &work->particles[i];

		if (work->effect->side == 1)
			puff->x = (Trig_Sin(j) * 24 >> 16) + 88;
		else
			puff->x = (-(Trig_Sin(j) * 24) >> 16) + 16;
		puff->y = (Trig_Cos(j) * 16 >> 16) + 40;
		puff->variant = -(i * 2);
	}
	path = Resource_GetTableEntry((s32)&ResourceId_DualStreamPath);

	for (frame = 0; frame != 150; frame++)
	{
		camera = gCameraWork;
		if (frame == 83)
			BattleEventRuntime_BeginPhaseFar(134);
		if (frame == 0)
			AudioCommand_PlayFar(136);
		if (frame == 50)
			AudioCommand_PlayFar(136);
		if (work->effect->side == 0)
		{
			if (frame <= 63)
				camera->yaw -= 0x100;
		}
		else
		{
			if (frame <= 63)
				camera->yaw += 0x100;
		}
		BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
		if (frame <= 17)
		{
			s32 cell = frame / 3;

			draw[0](canvas, &work->sheet[0x3c00] + PuffArc_CellSourceOffsets[cell], 48,
				PuffArc_CellBiasY[cell] + 60, PuffArc_CellWidths[cell], PuffArc_CellHeights[cell]);
			draw[1](canvas, &work->sheet[0x3c00] + PuffArc_CellSourceOffsets[cell], 56,
				PuffArc_CellBiasY[cell] + 60, PuffArc_CellWidths[cell], PuffArc_CellHeights[cell]);
		}
		if (frame >= 18 && frame <= 58)
		{
			if (frame == 18)
			{
				x = (path[0] << 8) + (u8)path[1];
				y = (path[2] << 8) + (u8)path[3] + 16;
				path += 4;
			}
			else
			{
				x += path[0];
				y += path[1];
				path += 2;
			}
		}
		if (frame >= 78 && frame <= 118)
		{
			if (frame == 78)
			{
				x = -56;
				y = 48;
			}
			else
			{
				y -= 16;
			}
		}
		for (j = 19; j != 0; j--)
		{
			if (frame > j + 18 && frame <= j + 83)
			{
				trail[j].x = trail[j - 1].x;
				trail[j].y = trail[j - 1].y;
				if (j > 10)
					draw[0](canvas, &work->sheet[0x1680 + (j - 10) * 936], trail[j].x, trail[j].y,
						24, 39);
				else
					draw[0](canvas, &work->sheet[0x1680], trail[j].x, trail[j].y, 24, 39);
			}
		}
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame >= 18 && frame <= 83)
		{
			s32 lift;

			if (work->effect->side == 1)
				position.x = 64 - x / 2;
			else
				position.x = x / 2 + 64;
			position.y = 60 - y;
			lift = (position.y - trail[0].y - 24) / 2;
			if (lift > 2)
				lift = 2;
			if (lift < -2)
				lift = -2;
			drift += lift;
			if (drift > 8)
				drift = 8;
			if (drift < -8)
				drift = -8;
			lift = drift / 4 + 2;
			trail[0].x = position.x - 12;
			trail[0].y = position.y - 20;
			draw[0](canvas, &work->sheet[lift * 0x480], trail[0].x - 6, trail[0].y - 2, 24, 48);
		}
		if (frame == 83)
		{
			work->shake_frames = 8;
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
		}
		if (frame > 83)
		{
			for (i = 0; i != 56; i++)
			{
				struct EffectStep *ember = &work->particles[8 + i];

				if (ember->y >= 0)
				{
					s32 size;

					EffectPosition_ApplyBaseAndYOffset(&ember->x, &position);
					position.x >>= 1;
					if (position.depth <= 159)
						position.depth = 160;
					if (position.depth > 799)
						position.depth = 799;
					size = 9 - (position.depth - 160) / 64;
					if (i > 47)
					{
						if (ember->variant <= 11)
						{
							draw[0](canvas, Ram_MapCellBuffer + ember->variant / 2 * 0x800,
								position.x - 16, position.y - 32, 32, 64);
							ember->variant++;
						}
					}
					else
					{
						draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
							position.x - size / 2, position.y - size, size, size * 2);
					}
					ember->x += ember->velocity_x;
					ember->y += ember->velocity_y;
					ember->z += ember->velocity_z;
					ember->velocity_y -= 0x2000;
				}
			}
		}
		if (frame == 50)
		{
			work->shake_frames = 12;
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
		}
		if (frame > 49)
		{
			for (i = 0; i != 8; i++)
			{
				struct EffectStep *puff = &work->particles[i];

				if (puff->variant >= 0 && puff->variant <= 11)
				{
					s32 cell = puff->variant / 2;

					draw[1](canvas, &work->sheet[0x3c00] + PuffArc_CellSourceOffsets[cell],
						puff->x - PuffArc_CellWidths[cell] / 2, puff->y + PuffArc_CellBiasY[cell],
						PuffArc_CellWidths[cell], PuffArc_CellHeights[cell]);
				}
				puff->variant++;
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

void BattleFx_RunFireBurstShards(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	u8						*cells;
	struct BattleCamera		*view;
	struct MotionObject		*slot;
	struct EffectStep		*shard;
	struct EffectPosition	 position;
	struct EffectPosition	 origin;
	struct EffectPosition	 out;
	DrawRectangle			 routine[2];
	s32						 i;
	s32						 frame;
	s32						 size;
	u8						*burst;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	cells = heap_cache[2];
	view = heap_cache[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
	burst = Ram_MapCellBuffer;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_FireBurstSheet, burst, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, cells, 0, 0);
	BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0], 4, 0);
	WaitFrames(1);
	slot = GetBattleObjectSlotFar(work->effect->actors[0])->object;

	i = 0;
	do
	{
		struct EffectStep *seed = &work->particles[i];

		seed->x = slot->x;
		seed->y = slot->y;
		seed->z = slot->z;
		seed->velocity_x = (Random16() & 255) << 11;
		seed->velocity_y = ((Random16() & 255) - 127) << 12;
		seed->velocity_z = ((Random16() & 255) - 127) << 12;
		if (seed->x > 0)
			seed->velocity_x = -seed->velocity_x;
		seed->variant = i / 4 * 2 + 16;
		i++;
	} while (i != 64);

	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &origin);
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	work->transfer_mode = 2;
	work->transfer_value = 75;

	for (frame = 0; frame != 64; frame++)
	{
		if (frame == 8)
			BattleEventRuntime_BeginPhaseFar(0x86);
		if (work->effect->variant != 0 && frame == 8)
			AudioCommand_PlayFar(0xd4);
		EffectPosition_ApplyStepAndYOffset(work->effect->actor, &position);
		if (frame >= 6 && frame <= 11)
		{
			if (work->effect->side == 0)
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
			else
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 3);
			routine[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
			if (work->effect->side == 0)
				routine[0](canvas, work->sheet + (frame - 6) * 0xd80, position.x / 2 - 24,
					position.y - 24, 48, 72);
			else
				routine[0](canvas, work->sheet + (frame - 6) * 0xd80, position.x / 2,
					position.y - 24, 48, 72);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		}
		if (frame >= 16 && frame <= 47)
		{
			s32 step = (frame - 16) / 2;
			s32 strip;

			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
			routine[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
			if (step > 2)
				step = 2;
			if (work->effect->variant == 0)
				strip = 0;
			else
				strip = 0x2580;
			routine[0](canvas, Ram_MapCellBuffer + strip + step * 0xc80, origin.x / 2 - 20,
				origin.y - 48, 40, 80);
			BattleFx_RunNoEffectFrames(10000);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		}
		if (frame == 8)
		{
			Dma_Fill32(0x3f3f3f3f, canvas, 0x4000);
		}
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(view->eye, view->pos);
		if (frame > 3)
		{
			BattleFx_FetchRectangleBlitters(work->effect->side, routine);
			for (i = 0; i != 64; i++)
			{
				s32 life;

				shard = &work->particles[i];
				life = shard->variant;
				if (life > 0)
				{
					EffectPosition_ApplyBaseAndYOffset((s32 *)shard, &out);
					out.x >>= 1;
					out.y = out.y + origin.y - 112;
					size = (life >> 3) + 2;
					routine[(i / 2) & 1](canvas, cells + ParticleStreams_CellOffsets[size - 1],
						out.x - size / 2, out.y - size, size, size * 2);
					EffectStep_AdvanceWithGravity3D(shard, 60, -0x400);
					shard->variant--;
				}
			}
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		}
		if (frame == 8)
		{
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
			work->shake_frames = 4;
		}
		if (frame == 6)
			ObjectGroup_UpdateMembers(work->effect->actors[0], 10, -1, -1, 0);
		if (frame == 14)
			ObjectGroup_UpdateMembers(work->effect->actors[0], 10, -1, -1, 0);
		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// flame_blade.c
// バトル演出：剣を振り下ろし、2 か所で閃光と火花を散らす
//--------------------------------------------------------------------------
// PARTICLES : 火花 (列ごとに 340 個)

#define FLASH_SHEET (64 * 128) // 剣の絵 (64×128) の後ろに閃光の絵を置く

//--------------------------------------------------------------------------
// 剣を 40 フレーム描き (10 フレームで振り下ろす)、16 と 24 フレーム目に
// 2 つの列で閃光を出し、6 フレーム後から火花を散らす
// 列の x は FlameBlade_StrikeColumns (side ごとに 3 つ、使うのは最初の 2 つ)
//--------------------------------------------------------------------------
void BattleFx_RunFlameBlade(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 col;
	u8						*sheet;
	struct MotionObject		*user;
	DrawRectangle			 blit[2];
	s32						 frame;
	s32						 i;

	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];

	user = GetBattleObjectSlotFar(effect->actor)->object;
	effect->variant = 1;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BLDALPHA = 0x1010;
	BattleFx_FetchRectangleBlitters(work->effect->side, blit);
	ObjectDispatch_ApplyArgumentToChildrenFar(user, 2);
	ObjectDispatch_ApplyValueToChildrenFar(user, 48);

	Resource_LoadAndDecompress((s32)&ResourceId_FlameBladeSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet + FLASH_SHEET, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

	//------------------------------------------------------------------
	// 列ごとに閃光 16 個と火花 340 個を用意する
	//------------------------------------------------------------------
	col = 0;
	do
	{
		// 閃光：中心から i*2 ドット、寿命 25〜32
		i = 0;
		do
		{
			struct EffectStep *flash = &work->particles[col * 16 + i];

			s32 radius = i * 2;
			s32 angle = Random16() & 0xffff;

			flash->x = Trig_Sin(angle) * radius;
			flash->y = -(Trig_Cos(angle) * radius);
			flash->variant = i / 2 + 25;
			i++;
		} while (i != 16);

		// 火花：列の y = 88 から乱数の向きと速さで飛ぶ、寿命 32〜39
		i = 0;
		do
		{
			struct EffectStep *spark = &PARTICLES[col * 340 + i];

			s32 speed;
			s32 angle;

			speed = (Random16() & 0x1ff) + 32;
			angle = Random16() & 0xffff;
			spark->x = FlameBlade_StrikeColumns[col + work->effect->side * 3] << 16;
			spark->y = 88 << 16;
			spark->velocity_x = (Trig_Sin(angle) * speed) >> 6;
			spark->velocity_y = -(Trig_Cos(angle) * speed * 2) >> 6;
			spark->variant = (Random16() & 7) + 32;
			i++;
		} while (i != 340);
		col++;
	} while (col != 3);

	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	frame = 0;
	do
	{
		if (frame == 4)
		{
			AudioCommand_PlayFar(212);
		}
		if (frame == 8)
		{
			work->shake_frames = frame;
		}
		if (frame == 18)
		{
			AudioCommand_PlayFar(145);
		}
		if (frame == 40)
		{
			BattleEventRuntime_BeginPhaseFar(134);
		}

		//------------------------------------------------------------------
		// 剣：10 フレームで振り下ろし、20 フレームまで止まり、その後ゆっくり進む
		// y = 104 より下は描かない
		//------------------------------------------------------------------
		if (frame <= 39)
		{
			s32 height;
			s32 x;
			s32 top;

			height = 128;
			if (work->effect->side == 1)
			{
				if (frame <= 9)
				{
					x = frame * 10 - 8;
					top = frame * 16 - 128;
				}
				else if (frame > 20)
				{
					x = frame + 62;
					top = frame * 2 - 24;
				}
				else
				{
					x = 82;
					top = 16;
				}
			}
			else
			{
				if (frame <= 9)
				{
					x = 128 - frame * 10;
					top = frame * 16 - 128;
				}
				else if (frame > 20)
				{
					x = 58 - frame;
					top = frame * 2 - 24;
				}
				else
				{
					x = 38;
					top = 16;
				}
			}
			if (top + 128 > 104)
			{
				height = height - top - 24;
			}
			if (height > 0)
			{
				blit[0](canvas, work, x - 32, top, 64, height);
			}
		}

		if (frame > 16)
		{
			BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
		}

		//------------------------------------------------------------------
		// 2 つの列：start から閃光、start + 6 から火花
		//------------------------------------------------------------------
		col = 0;
		do
		{
			s32 start = col * 8 + 16;

			if (frame == start)
			{
				work->shake_frames = 12;
			}
			if (frame >= start)
			{
				// 最初の 2 フレームは大きな閃光、続いて 12 個の小さな閃光
				if (frame < start + 2)
				{
					blit[0](canvas, work->sheet + FLASH_SHEET,
						FlameBlade_StrikeColumns[col + work->effect->side * 3] - 16, 56, 32, 64);
				}
				i = 0;
				do
				{
					struct EffectStep *flash = &work->particles[col * 16 + i];

					s32 x;
					s32 life;
					s32 y;

					y = HI(flash->y);
					x = HI(flash->x) + FlameBlade_StrikeColumns[col + work->effect->side * 3];
					life = flash->variant;
					if (life >= 0 && life <= 17)
					{
						blit[0](canvas,
							work->sheet + (FlameBlade_FlashCells[life / 3] << 11) + FLASH_SHEET,
							x - 16, y + 56, 32, 64);
					}
					life = flash->variant;
					flash->variant = life > 0 ? life - 1 : -1;
					i++;
				} while (i != 12);
			}

			// 火花：y = 108 より下で跳ね返り (速さ半分)、x 0〜126 の間だけ描く
			if (frame > start + 5)
			{
				i = 0;
				do
				{
					struct EffectStep *spark = &PARTICLES[col * 340 + i];

					if (spark->variant > 0)
					{
						s32 x;
						s32 y;
						s32 size;

						EffectStep_AdvanceWithGravity2D(spark, 64, 0x1000);
						spark->variant--;
						y = spark->y;
						if (y > (108 << 16))
						{
							spark->velocity_y = -spark->velocity_y / 2;
						}
						else
						{
							x = spark->x;
							if ((x >= 0 && x <= 0x7effff) && y >= 0)
							{
								u8 *cel;
								s32 lane;
								s32 py = y >> 16;
								s32 px = x >> 16;

								// 寿命で大きさを決め、比較と加算を 1 個おきに使う
								size = spark->variant / 5 + 1;
								lane = i & 1;
								cel = sheet + ParticleStreams_CellOffsets[size - 1];
								px -= size / 2;
								py -= size;
								blit[lane](canvas, cel, px, py, size, size * 2);
							}
						}
					}
					i++;
				} while (i != 256);
			}

			for (i = 0; i != work->effect->count; i++)
			{
				if (frame == start + 6)
				{
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 10);
					BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
				}
			}
			col++;
		} while (col != 2);

		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	} while (frame != 80);

	ObjectDispatch_ApplyValueToChildrenFar(user, 16);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

#define SPARK ((struct EffectStep *)(Ram_MapCellBuffer + 684 * sizeof(struct EffectStep)))

void BattleFx_RunFallingSword(struct BattleEffectArgument *effect)
{
	void				   **heap;
	void				   **p;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 routine[2];
	u8						*sheet;
	struct EffectPosition	 pos;
	s32						 frame;
	s32						 half;
	s32						 step;
	s32						 origin;
	s32						 i;

	heap = gBattleFxWork;
	p = heap;
	work = *p++;
	canvas = *p;
	sheet = heap[2];

	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BLDALPHA = 0x1010;
	EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &pos);
	half = pos.x / 2;

	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	routine[0] = heap[7];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 1);
	routine[1] = heap[8];

	Resource_LoadAndDecompress((s32)&ResourceId_SwordSheet, work->sheet + 20000, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet + (221 << 4), 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	i = 0;
	do
	{
		struct EffectStep *flash = &work->particles[i];

		s32 scale = i * 2;
		s32 ang = Random16() & 0xFFFF;

		flash->x = Trig_Sin(ang) * scale;
		flash->y = -(Trig_Cos(ang) * scale);
		flash->variant = i / 2 + 25;
		i++;
	} while (i != 32);

	i = 0;
	do
	{
		PARTICLES[i].variant = -1;
		i++;
	} while (i != 171 << 2);

	i = 0;
	origin = half << 16;
	do
	{
		struct EffectStep *spark = &SPARK[i];

		s32 mag;
		s32 ang;

		/* FAKEMATCH: the mask is loaded into the magnitude before the random word is taken in, as the native code does (ldr r6, =0x1ff; ands r6, r0). 2026-10-13: Random16() & 0x1FF, 0x1FF & Random16() and mag = Random16() followed by mag &= 0x1FF all differ 41 of 779, 780 instructions: the and takes the random word as its destination, copied into r6, with the mask in r3, and the pool offsets shift. */
		mag = 0x1FF;
		mag &= Random16();
		ang = Random16() & 0xFFFF;
		spark->x = origin;
		spark->y = 176 << 15;
		spark->velocity_x = (Trig_Sin(ang) * (mag + 32)) >> 5;
		spark->velocity_y = -(Trig_Cos(ang) * (mag + 32)) >> 6;
		spark->variant = (Random16() & 7) + 32;
		i++;
	} while (i != 170 << 1);

	frame = 0;
	do
	{
		if (frame >= 25 && frame <= 47)
		{
			BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
		}
		if (frame > 56)
		{
			BattleFx_StepPaletteToResource((s32)&ResourceId_LightningBoltSheet);
		}

		if (frame == 8)
		{
			work->shake_frames = frame;
		}
		if (frame == 48)
		{
			work->shake_frames = 8;
		}
		if (frame == 60)
		{
			work->shake_frames = 16;
		}

		if (frame == 4)
		{
			AudioCommand_PlayFar(212);
		}
		if (frame == 32)
		{
			AudioCommand_PlayFar(164);
		}
		if (frame == 60)
		{
			AudioCommand_PlayFar(145);
			BattleEventRuntime_BeginPhaseFar(134);
		}

		if (frame > 55)
		{
			i = 0;
			do
			{
				struct EffectStep *flash = &work->particles[i];

				s32 x;
				s32 life;
				s32 y;

				y = HI(flash->y);
				x = HI(flash->x) + half;
				life = flash->variant;
				if (life >= 0 && life <= 17)
				{
					routine[0](canvas,
						(u8 *)work + (FallingSword_FlashCells[life / 3] << 11) + (221 << 4), x - 16,
						y + 48, 32, 64);
				}
				life = flash->variant;
				flash->variant = life > 0 ? life - 1 : -1;
				i++;
			} while (i != 16);
		}

		if (frame == 28)
		{
			i = 0;
			do
			{
				struct EffectStep *dust = &PARTICLES[i];

				if (dust->variant == -1)
				{
					s32 mag;
					s32 ang;

					mag = Random16() & 63;
					ang = Random16() & 0xFFFF;
					dust->x = ((Trig_Sin(ang) * mag) >> 3) + origin;
					dust->y = ((Trig_Cos(ang) * mag) >> 2) + (192 << 15);
					dust->velocity_x = ((Random16() & 63) - 32) << 14;
					dust->velocity_y = (-(Random16() & 63) - 8) << 13;
					dust->variant = 0;
				}
				i++;
			} while (i != 128 << 1);
		}

		step = frame - 32;
		if (step >= 0 && step <= 31)
		{
			s32 cnt;

			cnt = 0;
			i = 0;
			do
			{
				struct EffectStep *dust = &PARTICLES[i];

				if (dust->variant == -1)
				{
					s32 mag;
					s32 ang;

					mag = Random16() & 63;
					ang = Random16() & 0xFFFF;
					dust->x = ((Trig_Sin(ang) * mag) >> 3) + origin;
					dust->y = ((Trig_Cos(ang) * mag) >> 2) + (192 << 15);
					dust->velocity_x = ((Random16() & 63) - 32) << 14;
					dust->velocity_y = (-(Random16() & 63) - 8) << 13;
					dust->variant = 0;
					cnt++;
					if (cnt == 16)
					{
						break;
					}
				}
				i++;
			} while (i != 171 << 2);
		}

		if (step >= 0 && step <= 31)
		{
			s32			  phase = frame * 16;
			s32			  x = half - 17;
			s32			  width = 34;
			DrawRectangle blit = routine[0];
			s32			  high = 104;
			s32			  size = (phase - 256) % high;

			blit(canvas, work, x, 4 - size, width, high);
			blit(canvas, work, x, 108 - size, width, size);
		}

		if (frame <= 71)
		{
			i = 0;
			do
			{
				struct EffectStep *dust = &PARTICLES[i];

				if (dust->variant >= 0)
				{
					s32 size;

					size = i % 3 + 2;
					if (dust->velocity_y > 0)
					{
						size += 2;
					}
					if (frame > 68 && size <= 5)
					{
						size = 6;
					}
					if (frame > 70 && size <= 6)
					{
						size = 7;
					}
					if (frame > 72 && size <= 7)
					{
						size = 8;
					}
					if (frame > 74 && size <= 8)
					{
						size = 9;
					}
					if (frame > 76)
					{
						size = 10;
					}
					routine[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
						HI(dust->x) - size / 2, HI(dust->y) - size, size, size * 2);

					dust->x += dust->velocity_x;
					dust->y += dust->velocity_y;
					if (frame > 80)
					{
						dust->velocity_y += -32768;
					}
					else
					{
						dust->velocity_y += FallingSword_DustGravity[i & 3];
					}
					dust->velocity_x = dust->velocity_x * 62 / 64;
					dust->velocity_y = dust->velocity_y * 62 / 64;
					dust->variant++;
					if (dust->velocity_y > 0 && HI(dust->y) > 108)
					{
						dust->variant = -1;
					}
				}
				i++;
			} while (i != 171 << 1);
		}

		if (frame <= 95)
		{
			s32 x;
			s32 high;
			s32 top;

			x = half - 18;
			high = 120;
			if (frame > 60)
			{
				top = frame * 8 - 450;
			}
			else if (frame > 32)
			{
				top = step / 2 + 16;
			}
			else if (frame <= 9)
			{
				top = frame * 16 - 128;
			}
			else
			{
				top = 16;
			}
			if (top + 120 > 108)
			{
				high = high - top - 12;
			}
			if (high > 0)
			{
				routine[1](canvas, work->sheet + 20000, x, top, 36, high);
			}
		}

		if (frame > 59)
		{
			i = 0;
			do
			{
				struct EffectStep *spark = &SPARK[i];

				if (spark->variant > 0)
				{
					s32 x;
					s32 y;
					s32 size;

					EffectStep_AdvanceWithGravity2D(spark, 64, 128 << 6);
					y = spark->y;
					spark->variant--;
					if (y > (216 << 15))
					{
						spark->velocity_y = -spark->velocity_y / 2;
					}
					else
					{
						x = spark->x;
						if ((x >= 0 && x <= 0x7effff) && y >= 0)
						{
							u8 *cell;

							size = spark->variant / 5 + 1;
							cell = sheet + ParticleStreams_CellOffsets[size - 1];
							x >>= 16;
							y >>= 16;
							x -= size / 2;
							y -= size;
							routine[0](canvas, cell, x, y, size, size * 2);
						}
					}
				}
				i++;
			} while (i != 170 << 1);
		}

		if (frame == 68)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 16);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 7);
			}
		}

		if (frame == 9)
		{
			Iwram_FillWords(canvas, 128 << 7, 0x3F3F3F3F);
		}
		if (frame == 60)
		{
			Iwram_FillWords(canvas, 128 << 7, 0x3F3F3F3F);
		}

		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	} while (frame != 102);

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

struct ShockwaveWork
{
	u8 sheet[0x2710];
	u8 ramp[8][770];
};

extern const struct Scale ShockwaveScene_UnitScale;
extern u16				  ShockwaveScene_ShardCells[];
extern u8				  ShockwaveScene_ShardWidths[];
extern u8				  ShockwaveScene_ShardHeights[];

#define SPARKS (&PARTICLES[128])

void BattleEffect_RunShockwaveScene(struct BattleEffectArgument *effect)
{
	s32							   anchor[3];
	s32							   scene[3];
	s32							   place[4];
	volatile u32				   fill;
	s32							   place2[4];
	struct Scale				   pair;
	struct Scale				   scale;
	void						  *canvas;
	struct BattleEffectWork		  *work;
	struct BattlePresentationWork *ctrl;
	DrawRectangle				   draw[2];
	s32							   frame;
	u8							  *ramp;
	struct BattleCamera			  *camera;
	s32							   depth;
	s32							   zoom;
	s32							   near;
	s32							   far;
	void						 **cache;
	DrawRectangle				  *slots;
	s32							   cx;
	s32							   cy;
	s32							   squash;
	s32							   sx;
	s32							   ring;
	s32							   rings;
	s32							   shift;
	s32							   i;
	s32							   j;
	struct EffectStep			  *p;
	DrawRectangle				   blit;
	volatile u16				  *ime;

	cache = &gBattleFxWork[1];
	ime = (u16 *)0x04000208;
	canvas = cache[0];
	work = cache[HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_BATTLE_CANVAS];
	ctrl = cache[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE_CANVAS];
	ramp = cache[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_CANVAS];
	camera = cache[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_CANVAS];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0x2000);
	REG_BG2PA = 0x100;
	BattlePres_ConfigureEffectDisplay();
	PLTT_BACKDROP = 0;
	*(u16 *)0x05000002 = 0;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
	draw[0] = (DrawRectangle)cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_CANVAS];
	work->transfer_mode = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_WipeCanvas(0, 0);
	BattleFx_SelectLivingTargets(work->effect);
	BattleFx_SpawnObjects(16, 0x17e, 1);
	gProjection.center_y = 240;
	WaitFrames(1);
	BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
	ctrl->initial_value = 1;
	gBgScroll[1].x = 0;
	BattleEffect_WipeCanvas(0, 1);
	REG_DISPCNT = 0x7741;
	REG_BG2PA = 0x80;
	REG_BLDALPHA = 0x1010;
	REG_BLDCNT = 0x3f44;
	depth = 0xffc00000;
	zoom = 0x80000;
	near = 0xffff;
	far = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_PinkBurstSheet, work, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageB, work->sheet + 0x600, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x95c, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, ramp, 0, 0);

	for (i = 0; i != 8; i++)
	{
		u8 *src = ramp;
		for (j = 0; j != 770; j++)
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
			((struct ShockwaveWork *)work)->ramp[i][j] = v;
		}
	}

	Iwram_CopyWords(
		(void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_BlueCrescentSheet), 128);

	for (i = 0; i != 128; i++)
	{
		PARTICLES[i].x = (Random16() & 15) + 72;
		PARTICLES[i].velocity_x = Random16() & 0xffff;
		PARTICLES[i].velocity_y = Random16() & 0xffff;
		PARTICLES[i].velocity_y = Random16() & 0xffff;
	}

	for (i = 0; i != 128; i++)
	{
		s32 angle = Random16() & 0xffff;
		/* FAKEMATCH: a form the bytes prove: the amplitude is a value, not a literal: the native hoists 255 into sl and multiplies with muls. 2026-10-13: 255 written at both multiplies differs 142 of 2455; amp and speed set before the loop differ 2 (i = 0 is scheduled after them), set after i = 0 or in the for initializer 0, the same bytes as here. */
		s32 amp = 255;
		/* FAKEMATCH: register choice: the velocities' zero is a value set at the top of the body, which loop.c hoists into r8 as the native's. 2026-10-13: 0 stored directly differs 138 of 2455; see amp for the forms set before the loop. */
		s32 speed = 0;
		SPARKS[i].x = (Trig_Sin(angle) * amp) >> 2;
		SPARKS[i].z = (Trig_Cos(angle) * amp) >> 2;
		SPARKS[i].y = ((Random16() & 63) + 32) << 16;
		SPARKS[i].velocity_x = speed;
		SPARKS[i].velocity_y = speed;
		SPARKS[i].variant = angle;
	}

	work->transfer_mode = 2;
	work->transfer_value = 50;
	REG_BG2CNT = 0x784;

	for (frame = 0; frame != 160 && (frame <= 4 || !(gKeysRepeat & (KEY_A | KEY_B))); frame++)
	{
		/* FAKEMATCH: needed to match: the frame > 143 block calls through a pointer to the slot table (base kept at sp+56) 2026-10-11: set once before the frame loop differs 22 (the preheader reload registers rotate); as a local of the frame > 143 block 579; the slot table named at the uses 581. */
		slots = (DrawRectangle *)gWorkSlot;
		if (frame == 143)
		{
			Iwram_FillWords(canvas, 0x4000, 0x2a2a2a2a);
			AudioCommand_PlayFar(145);
		}
		if (frame == 80)
		{
			AudioCommand_PlayFar(142);
		}

		if (frame < 0)
		{
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			for (i = 0; i != 128; i++)
			{
				s32 z;
				s32 size;

				EffectStep_AdvanceWithGravity3D(&SPARKS[i], 60, -0x400);
				EffectPosition_ApplyBaseAndYOffset(
					(s32 *)&SPARKS[i], (struct EffectPosition *)scene);
				scene[0] >>= 1;
				scene[1] -= 120;
				if (scene[2] < 100)
				{
					scene[2] = 100;
				}
				if (scene[2] > 900)
				{
					scene[2] = 900;
				}
				z = scene[2] - 100;
				scene[2] = z;
				size = (z / 100) + 1;
				if (near > z)
				{
					near = z;
				}
				if (far < z)
				{
					far = z;
				}
				draw[0](canvas, ramp + ParticleStreams_CellOffsets[size - 1], scene[0] - size / 2,
					scene[1] - size, size, size * 2);
				if (frame == 64)
				{
					SPARKS[i].velocity_x += (Trig_Sin(SPARKS[i].variant) * 255) >> 3;
					SPARKS[i].velocity_y += (Trig_Cos(SPARKS[i].variant) * 255) >> 3;
				}
			}
		}

		if (frame == 72)
		{
			work->fade_frames = 24;
			work->fade_step = 0;
		}

		anchor[2] = 0;
		anchor[1] = 0;
		for (i = 0; i != 64; i++)
		{
			if (frame > i / 4 + 80 && PARTICLES[i].x > 0)
			{
				s32 size;

				Render_ResetTransformState();
				SceneTransform_ApplyRoll(PARTICLES[i].velocity_y);
				SceneTransform_ApplyPitch(PARTICLES[i].velocity_x);
				SceneTransform_ApplyYaw(PARTICLES[i].velocity_y + (frame << 9));
				anchor[0] = PARTICLES[i].x;
				PARTICLES[i].x -= 2;
				if (PARTICLES[i].x < 0)
				{
					PARTICLES[i].x = 0;
				}
				EffectPosition_ApplyBaseAndYOffset(anchor, (struct EffectPosition *)scene);
				if (scene[2] < -60)
				{
					scene[2] = -60;
				}
				if (scene[2] > 60)
				{
					scene[2] = 60;
				}
				scene[2] += 60;
				size = ((scene[2]) / 20) + 2;
				scene[0] += 60;
				scene[1] += 80;
				draw[0](canvas, ramp + ParticleStreams_CellOffsets[size - 1], scene[0] - size / 2,
					scene[1] - size, size, size * 2);
			}
		}

		pair = ShockwaveScene_UnitScale;
		place[3] = 0;
		place[1] = 0;
		for (i = 0; i != 16; i++)
		{
			place[0] = ((i % 4) << 21) + (152 << 15);
			place[2] = ((i / 4) << 21) + depth;
			Object_ApplyProjectedPlacementFar(work->objects[i], place, &pair, 0);
		}

		depth += zoom;
		if (frame <= 47)
		{
			zoom -= 0x4000;
		}
		if (frame > 32)
		{
			zoom = zoom * 60 / 64;
		}
		if (frame == 144)
		{
			zoom = -0x80000;
		}
		if (frame == 146)
		{
			zoom = 0x10000;
		}

		if (frame == 72)
		{
			for (i = 0; i != 16; i++)
			{
				AnimationObjects_SelectAnimationFar(work->objects[i], i + 16);
			}
		}
		if (frame == 76)
		{
			for (i = 0; i != 16; i++)
			{
				AnimationObjects_SelectAnimationFar(work->objects[i], i + 32);
			}
		}

		if (frame >= 116 && frame < 144)
		{
			s32 high;
			s32 cell;
			s32 n = (frame - 116) / 4;
			if (n > 6)
			{
				n = 6;
			}
			high = ShockwaveScene_BurstHeights[n];
			cell = ShockwaveScene_BurstCells[n] + 0x600;

			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
			(((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
				canvas, work->sheet + cell, 60 - high, 80 - high * 2, high, high * 2);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
			(((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
				canvas, work->sheet + cell, 60, 80 - high * 2, high, high * 2);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 11, 2);
			(((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
				canvas, work->sheet + cell, 60 - high, 80, high, high * 2);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 15, 2);
			(((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
				canvas, work->sheet + cell, 60, 80, high, high * 2);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);

			for (i = 0; i != 6; i++)
			{
				s32 k = i & 3;
				s32 angle = Random16() & 0xffff;
				s32 x = ((Trig_Sin(angle) * 16) >> 16) - ShockwaveScene_ShardWidths[k] / 2;
				s32 y = ((Trig_Cos(angle) * 16) >> 16) - ShockwaveScene_ShardHeights[k] / 2;
				draw[0](canvas, work->sheet + ShockwaveScene_ShardCells[k], x + 60, y + 80,
					ShockwaveScene_ShardWidths[k], ShockwaveScene_ShardHeights[k]);
			}
		}

		if (frame > 143)
		{
			s32 top = (frame << 4) - 2272;
			s32 n;
			s32 r;
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
			(slots[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work, 36, top, 24, 64);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 3);
			(slots[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work, 60, top, 24, 64);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);

			n = (frame - 144) / 2;
			r = frame * 10 - 1424;
			work->transfer_value = 75;
			if (n <= 6)
			{
				s32 cell = n * 770 + ParticleStreams_CellOffsets[7] + 0x2710;
				/* FAKEMATCH: the height is a value, so high / 2 is hoisted into fp. 2026-10-13: the literals (4, 8 and 16) differ 690 of 2455. */
				s32 high = 8;
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
				blit = slots[HEAP_SLOT_BLITTER_ALTERNATE];
				for (i = 0; i != 128; i++)
				{
					s32 angle = i << 9;
					s32 x = ((r * Trig_Sin(angle)) >> 16) - high / 2;
					s32 y = (r * Trig_Cos(angle)) >> 17;
					blit(canvas, work->sheet + cell, x + 60, y + 72, high, high * 2);
				}
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			}
		}

		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Iwram_FillWords(canvas, 0x4000, 0);

	for (i = 0; i != 16; i++)
	{
		ResourceObject_ReleaseFar(work->objects[i]);
	}

	for (i = 0; i != 16; i++)
	{
		struct AnimationObject *obj = (struct AnimationObject *)ResourceObject_CreateFar(0x186);
		work->objects[i] = obj;
		if (obj != 0)
		{
			obj->flags = 0;
			AnimationObjects_SelectAnimationFar(obj, (i % 3));
			((struct AnimationObject *)work->objects[i])->part[0].priority = 0;
		}
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	Resource_LoadAndDecompress((s32)&ResourceId_BlueCrescentSheet, work->sheet + 0x4000, 1, 1);

	{
		u8 *c = ramp;
		for (i = 0; i != 770; i++, c++)
		{
			if (*c > 32)
			{
				*c += 224;
			}
			else if (*c != 0)
			{
				*c = 1;
			}
		}
	}

	work->fade_frames = 0;
	work->fade_step = 0;
	BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
	REG_BG2PA = 0x100;
	REG_BG2X = -0x3c00;
	REG_BG2CNT = 0x784;

	fill = 0;
	Dma_Set(&fill, canvas, 0x85001000, REG_DMA3);

	cx = 60;
	cy = 44;
	squash = 6;
	shift = 0;
	ring = 0;
	sx = 8;
	rings = 2;
	work->transfer_mode = 2;
	work->transfer_value = 75;

	fill = shift;
	Dma_Set(&fill, work, 0x85000e10, REG_DMA3);

	for (frame = 0; frame != 320; frame++)
	{
		if (frame == 66)
		{
			AudioCommand_PlayFar(145);
			AudioCommand_PlayFar(141);
		}
		if (frame == 155)
		{
			AudioCommand_PlayFar(162);
		}
		if (frame == 217)
		{
			AudioCommand_PlayFar(156);
		}
		if (frame == 280)
		{
			AudioCommand_PlayFar(157);
		}
		if (frame == 300)
		{
			BattleEventRuntime_BeginPhaseFar(145);
		}

		if (gKeysRepeat & (KEY_A | KEY_B))
		{
			if (frame >= 5 && frame < 150)
			{
				frame = 150;
				IO_WRITE_QUEUE_PUSH_IME(ime, 0x04000020, 0x80, 0x20000);
				IO_WRITE_QUEUE_PUSH_IME(ime, 0x04000028, 0, 0x30000);
				Resource_LoadAndDecompress(
					(s32)&ResourceId_ExplosionSheet, work->sheet + 0x4000, 1, 0);
			}
			else if (frame >= 155 && frame < 214)
			{
				frame = 214;
			}
			else if (frame >= 219 && frame < 280)
			{
				frame = 280;
			}
		}

		if (frame == 64)
		{
			IO_WRITE_QUEUE_PUSH_IME(ime, 0x04000020, 0x80, 0x20000);
			IO_WRITE_QUEUE_PUSH_IME(ime, 0x04000028, 0, 0x30000);
			fill = 0;
			Dma_Set(&fill, canvas, 0x85001000, REG_DMA3);
			Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet, work->sheet + 0x4000, 1, 0);
			Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageA, work->sheet + 0x6000, 0, 0);
		}

		if (frame == 66)
		{
			u16 *pal = (u16 *)0x050000c0;
			for (i = 0; i != 128; i++, pal++)
			{
				*pal ^= 0x7fff;
			}
		}

		if (frame == 69)
		{
			fill = 0x3f3f3f3f;
			Dma_Set(&fill, canvas, 0x85001000, REG_DMA3);
		}

		if (frame == 70)
		{
			BattlePresentation_ConfigurePaletteFadeFar(1, (s32)&ResourceId_AerialViewBackdrop, 7);
		}

		if (frame == 150)
		{
			cx = 112;
			cy = 32;
			ring = 0;
			fill = 0;
			rings = 4;
			squash = 8;
			Dma_Set(&fill, work, 0x85000e10, REG_DMA3);
			fill = 0x3f3f3f3f;
			Dma_Set(&fill, canvas, 0x85001000, REG_DMA3);
			BattleBackground_LoadFar(1, (s32)&ResourceId_BeachBackdrop, 0);
		}

		if (frame == 214)
		{
			fill = 0;
			Dma_Set(&fill, work, 0x85000e10, REG_DMA3);
			fill = 0x3f3f3f3f;
			Dma_Set(&fill, canvas, 0x85001000, REG_DMA3);
			BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
			for (i = 0; i != 16; i++)
			{
				p = &work->particles[i];
				p->x = ((Random16() & 63) + 32) << 16;
				p->y = ((Random16() & 31) + 96) << 16;
				p->velocity_y = ((Random16() & 15) + 16) << 15;
				p->z = 0x8000 - (p->x >> 7);
				p->variant = 1;
			}
		}

		if (frame == 280)
		{
			BattleEffect_SetupBlendedDisplay();
			fill = 0;
			Dma_Set(&fill, work, 0x85000e10, REG_DMA3);
			fill = 0x01010101;
			Dma_Set(&fill, canvas, 0x85001000, REG_DMA3);
			ctrl->initial_value = 0;
			REG_BLDALPHA = 0x1010;
			for (i = 0; i != 16; i++)
			{
				p = &work->particles[i];
				p->x = ((Random16() & 63) + 256) << 16;
				p->y = ((Random16() & 31) + 96) << 16;
				p->velocity_y = ((Random16() & 15) + 16) << 15;
				p->z = 0;
			}
			shift = 272;
			work->transfer_mode = 0;
			work->transfer_value = 0;
		}

		if (frame > 279)
		{
			/* FAKEMATCH: palette RAM through a volatile pointer: the native keeps red's
               mask 31 in the loop rather than hoisting it; a plain pointer differs
               in 52 of 2455 rows. */
			volatile u16 *pal = (volatile u16 *)0x05000002;
			for (i = 0; i != 63; i++, pal++)
			{
				s32 c = *pal;
				s32 b = ((u16)c >> 10) & 31;
				s32 g = ((u16)c >> 5) & 31;
				s32 r = c & 31;
				b++;
				g++;
				r++;
				if (b > 31)
				{
					b = 31;
				}
				if (g > 31)
				{
					g = 31;
				}
				if (r > 31)
				{
					r = 31;
				}
				*pal = (b << 10) | (g << 5) | r;
			}
		}

		if (frame == 182)
		{
			fill = 0x3f3f3f3f;
			Dma_Set(&fill, canvas, 0x85001000, REG_DMA3);
		}

		if (frame <= 63)
		{
			u8 *sheet;
			s32 top = frame - 4;
			if ((frame & 7) < 4)
			{
				sheet = work->sheet + 0x4000;
			}
			else
			{
				sheet = work->sheet + 0x4240;
			}
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
			(((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
				canvas, sheet, 36, top - 24, 24, 24);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
			(((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
				canvas, sheet, 59, top - 24, 24, 24);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 11, 2);
			(((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
				canvas, sheet, 36, top - 1, 24, 24);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 15, 2);
			(((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
				canvas, sheet, 59, top - 1, 24, 24);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		}

		if (frame >= 64 && frame < 66)
		{
			draw[0](canvas, work->sheet + 0x6000, 52, 51, 16, 17);
		}
		if (frame >= 66 && frame < 68)
		{
			draw[0](canvas, work->sheet + 0x6110, 48, 40, 24, 41);
		}
		if (frame >= 68 && frame < 76)
		{
			s32 d = 76 - frame;
			draw[0](canvas, work->sheet + 0x64e8, 49 - d / 2, 38 - d, 22, 44);
			draw[0](canvas, work->sheet + 0x64e8, 49 - d / 2, d + 38, 22, 44);
			draw[0](canvas, work->sheet + 0x64e8, d / 2 + 49, 38 - d, 22, 44);
			draw[0](canvas, work->sheet + 0x64e8, d / 2 + 49, d + 38, 22, 44);
		}
		if (frame >= 78 && frame < 80)
		{
			draw[0](canvas, work->sheet + 0x6000, 52, 51, 16, 17);
		}
		if (frame >= 80 && frame < 82)
		{
			draw[0](canvas, work->sheet + 0x6110, 48, 40, 24, 41);
		}
		if (frame >= 82 && frame < 86)
		{
			s32 d = frame * 2 - 164;
			draw[0](canvas, work->sheet + 0x64e8, 49 - d / 4, 38 - d, 22, 44);
			draw[0](canvas, work->sheet + 0x64e8, 49 - d / 4, d + 38, 22, 44);
			draw[0](canvas, work->sheet + 0x64e8, d / 4 + 49, 38 - d, 22, 44);
			draw[0](canvas, work->sheet + 0x64e8, d / 4 + 49, d + 38, 22, 44);
		}

		if (frame >= 72 && frame < 76)
		{
			s32 r2 = (frame << 4) - 1136;
			/* FAKEMATCH: the size is a value, so 5 and 20 are hoisted into fp and sl. 2026-10-13: the literals (5, [9], 10 and 20) differ 100 of 2455. */
			s32 size = 10;
			for (i = 0; i != 256; i++)
			{
				s32 angle = i << 8;
				s32 x = ((Trig_Sin(angle) * r2) >> 16) - (size >> 1);
				s32 y = (Trig_Cos(angle) * r2) >> 17;
				draw[0](canvas, ramp + ParticleStreams_CellOffsets[size - 1], x + 60, y + 50, size,
					size * 2);
			}
		}

		if (frame > 85)
		{
			if (frame <= 213)
			{
				for (i = 0; i != rings; i++, ring++)
				{
					s32 radius = ring;
					s32 slope = 0;
					s32 err = ring;
					while (radius >= slope)
					{
						s32 half;
						s32 half2;
						s32 left;
						s32 right;
						s32 top;
						s32 bottom;
						s32 dx;

						half = radius / 2;
						dx = (half * sx) / 8;
						right = cx + dx;
						/* FAKEMATCH: a dead branch restores the label CSE and reload saw between the edges. 2026-10-13: without it 67 of 2455 differ; as if (dx > cx) left = 0; else left = cx - dx; 173. */
						if (dx > cx)
						{
							left = 0;
						}
						left = cx - dx;
						bottom = cy + (slope * squash) / 8;
						top = cy - (slope * sx) / 8;
						if (top < 0)
						{
							top = 0;
						}
						if (bottom > 119)
						{
							bottom = 119;
						}
						if (left < 0)
						{
							left = 0;
						}
						if (right > 119)
						{
							right = 119;
						}
						work->sheet[bottom * 120 + right] = 20;
						work->sheet[top * 120 + right] = 20;
						work->sheet[bottom * 120 + left] = 20;
						work->sheet[top * 120 + left] = 20;
						right = cx + half + 1;
						left = cx - half + 1;
						if (left < 0)
						{
							left = 0;
						}
						if (right > 119)
						{
							right = 119;
						}
						work->sheet[bottom * 120 + right] = 20;
						work->sheet[top * 120 + right] = 20;
						work->sheet[bottom * 120 + left] = 20;
						work->sheet[top * 120 + left] = 20;

						half2 = slope / 2;
						dx = (half2 * sx) / 8;
						right = cx + dx;
						/* FAKEMATCH: a dead branch restores the label CSE and reload saw between the edges. 2026-10-13: without it 1954 of 2455 differ, without both dead branches 1949. */
						if (dx > cx)
						{
							left = 0;
						}
						left = cx - dx;
						bottom = cy + (radius * squash) / 8;
						top = cy - (radius * sx) / 8;
						if (left < 0)
						{
							left = 0;
						}
						if (right > 119)
						{
							right = 119;
						}
						if (top < 0)
						{
							top = 0;
						}
						if (bottom > 119)
						{
							bottom = 119;
						}
						work->sheet[bottom * 120 + right] = 20;
						work->sheet[top * 120 + right] = 20;
						work->sheet[bottom * 120 + left] = 20;
						work->sheet[top * 120 + left] = 20;
						right = cx + half2 + 1;
						left = cx - half2 + 1;
						if (left < 0)
						{
							left = 0;
						}
						if (right > 119)
						{
							right = 119;
						}
						work->sheet[bottom * 120 + right] = 20;
						work->sheet[top * 120 + right] = 20;
						work->sheet[bottom * 120 + left] = 20;
						work->sheet[top * 120 + left] = 20;

						err = err - slope * 2 - 1;
						if (err < 0)
						{
							err = err + radius * 2 - 2;
							radius--;
						}
						slope++;
					}
				}
			}
		}

		if (frame >= 86 && frame < 150)
		{
			s32 angle = -0x4000;
			for (i = 0; i != 9; i++, angle += 0x1000)
			{
				s32 x = ((frame - 86) * Trig_Sin(angle)) >> 16;
				s32 y = ((frame - 70) * Trig_Cos(angle)) >> 16;
				draw[0](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000, x + 44, y + 17,
					32, 54);
			}
		}

		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
		draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
		if (frame > 85)
		{
			draw[1](canvas, work, 0, 0, 120, 120);
		}

		if (frame > 279)
		{
			shift -= 8;
		}

		if (frame > 319)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 14, 5, -1, 0);
			}
		}

		if (frame == 238)
		{
			fill = 0x3f3f3f3f;
			Dma_Set(&fill, canvas, 0x85001000, REG_DMA3);
		}

		if (frame >= 214 && frame < 280)
		{
			gBgScroll[1].x += 8;
		}

		if (frame >= 246 && frame < 280)
		{
			shift += 8;
			work->transfer_mode = 0;
			work->transfer_value = 75;
		}

		if (frame > 213)
		{
			place2[3] = 0;
			place2[1] = 0;
			for (i = 0; i != 16; i++)
			{
				p = &work->particles[i];
				if (p->variant != 0)
				{
					scale.x = p->z + (p->y >> 7);
					if (scale.x < 0x800)
					{
						scale.x = 0x800;
					}
					scale.y = scale.x;
					place2[0] = p->x;
					place2[1] = 0xff0000;
					place2[2] = p->y + 0xff0000;
					Object_ApplyProjectedPlacementFar(work->objects[i], place2, &scale, 0);
					p->y -= p->velocity_y;
					if (p->y < 0x100000)
					{
						if (frame <= 279)
						{
							p->x = ((Random16() & 63) + shift + 32) << 16;
						}
						else
						{
							p->x = ((Random16() & 63) + shift - 32) << 16;
						}
						if (frame > 301)
						{
							p->variant = 0;
						}
						else if (frame > 245)
						{
							p->y = 0x600000;
						}
						else
						{
							p->y = 0xc00000 - p->x;
						}
					}
				}
			}

			if (frame <= 279)
			{
				for (i = 0; i != 8; i++)
				{
					s32 x = (i & 1) * 20 - (Random16() & 15) + (i / 2) * 4 + shift / 2 - 16;
					s32 y = (i / 2) * 32 - (Random16() & 15);
					draw[1](
						canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000, x, y, 32, 54);
				}
			}
			else
			{
				for (i = 0; i != 8; i++)
				{
					s32 x = (i & 1) * 20 + (Random16() & 15) - (i / 2) * 4 + shift / 2 - 16;
					s32 y = (i / 2) * 32 - (Random16() & 15);
					draw[1](
						canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000, x, y, 32, 54);
				}
			}
		}

		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);

		if (frame > 63)
		{
			/* FAKEMATCH: forced temporary: the shake value through an s32 keeps the AND constant an immediate and the instruction count native's. 2026-10-13: gBgScroll[1].y = (Random16() & 3) + 30 directly differs 17 of 2455, 2456 instructions. */
			s32 shake = (Random16() & 3) + 30;

			gBgScroll[1].y = shake;
		}

		work->transfer_pending = 1;
		WaitFrames(1);
	}

	for (i = 0; i != 16; i++)
	{
		ResourceObject_ReleaseFar(work->objects[i]);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

#define REG16(address) (*(volatile u16 *)(address))

// battle.h が ObjectRow_SweepPair と ObjectRow_RisePair をこの名前で宣言している (中身は Scale)
struct FxPair
{
	s32 a;
	s32 b;
};

void BattleFx_RunObjectRow(struct BattleEffectArgument *effect)
{
	void					   **cursor;
	struct Placement			 place;
	struct EffectPosition		 unit;
	u8							 jitter_row[16];
	u8							 hit_row[14];
	struct Placement			 place2;
	void						*canvas;
	struct BattleEffectWork		*work;
	DrawRectangle				 blit;
	u8							*sheet;
	s32							 base_x;
	s32							 grow;
	s32							 saved_x;
	struct BattleBackgroundView *control;
	s32							 shift;
	s32							 wave;
	s32							 frame;
	s32							 i;

	cursor = &gBattleFxWork[1];
	canvas = cursor[0];
	work = cursor[-1];
	sheet = cursor[1];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	BattlePres_ConfigureEffectDisplay();
	REG_BG2CNT = 0x784;
	REG16(0x05000000) = 0;
	REG16(0x05000002) = 0;
	work->transfer_mode = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_WipeCanvas(1, 0);
	BattleFx_SpawnObjects(9, 0x175, 1);
	gProjection.center_y = 240;
	BattleFx_SelectLivingTargets(work->effect);
	REG_WININ = 0x2737;
	REG_WIN0H = 0xca;
	WaitFrames(1);
	BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
	BattleEffect_WipeCanvas(1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_FireBlobSheet, work->sheet, 1, 1);
	REG_DISPCNT = 0x7741;
	REG_BG2PA = 0x80;
	REG_BLDALPHA = 0x100e;
	REG_BLDCNT = 0x3f44;

	base_x = 0;
	grow = 0;
	saved_x = gBgScroll[1].x;
	control = cursor[4];
	shift = 0;
	work->transfer_mode = 1;
	work->transfer_value = base_x;
	control->busy = 1;

	i = 0;
	do
	{
		struct EffectStep *fire = &work->particles[i];

		fire->x = (Random16() & 31) + 16;
		fire->y = ((Random16() & 31) + 48) << 16;
		fire->velocity_y = ((Random16() & 31) - 16) << 16;
		fire->variant = Random16() % 48 + 2;
		i++;
	} while (i != 64);

	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
	blit = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	REG_BG2CNT = 0x786;

	for (frame = 0; frame != 120; frame++)
	{
		wave = 0;
		if (frame == 0)
		{
			AudioCommand_PlayFar(136);
		}
		if (frame == 26)
		{
			AudioCommand_PlayFar(141);
		}
		if (frame == 40)
		{
			AudioCommand_PlayFar(154);
		}
		if (frame == 72)
		{
			AudioCommand_PlayFar(154);
		}
		if (frame == 104)
		{
			AudioCommand_PlayFar(154);
		}
		if ((gKeysRepeat & (KEY_A | KEY_B)) != 0 && frame > 16)
		{
			break;
		}

		if (frame >= 24 && frame <= 55)
		{
			shift++;
		}
		if (shift > 24)
		{
			shift = 24;
		}
		if (frame <= 135)
		{
			gBgScroll[1].x -= shift;
			grow += shift;
		}

		if (frame <= 149)
		{
			struct FxPair pair = ObjectRow_SweepPair;
			s32			  lift;

			lift = 0;
			if (frame > 103)
			{
				lift = frame * 16 - 1664;
			}
			if (frame >= 8 && frame <= 31)
			{
				base_x = base_x + shift - 8;
			}
			if (frame > 7)
			{
				s32 amp;
				s32 ang;

				amp = 96;
				if (frame <= 104)
				{
					amp = 32;
				}
				ang = (frame * 1024 - 0x2000) & 0xFFFF;
				if (ang > 0x8000)
				{
					ang -= 0x8000;
				}
				wave = (Trig_Sin(ang) * amp) >> 16;
				if ((frame & 31) == 8)
				{
					work->shake_frames = 4;
				}
			}
			place.unknown_0c = 0;
			place.y = 255 << 16;
			i = 0;
			do
			{
				place.x = ((base_x + ObjectRow_Columns[i] - lift) << 16) + (224 << 16);
				place.z = ((ObjectRow_Rows[i] - wave) << 16) + (144 << 15);
				Object_ApplyProjectedPlacementFar(work->objects[i], &place, &pair, 0);
				i++;
			} while (i != 9);
		}

		if (frame <= 26)
		{
			s32 size = grow + 4;
			s32 count = frame * 8;

			if (size > 10)
			{
				size = 10;
			}
			if (count > 64)
			{
				count = 64;
			}
			for (i = 0; i != count; i++)
			{
				s32 ang = i << 10;
				s32 x = ((Trig_Sin(ang) * (grow * 2 + 8)) >> 16) + grow + 96;
				s32 y = ((Trig_Cos(ang) * (grow * 12 + 48)) >> 16) + 64;

				blit(canvas, sheet + ParticleStreams_CellOffsets[size - 1], x - size / 2, y - size,
					size, size * 2);
			}
		}

		if (frame == 24)
		{
			work->transfer_mode = 2;
			work->transfer_value = 50;
		}
		if (frame == 28)
		{
			REG_BG2CNT = 0x784;
		}

		if (frame > 17)
		{
			i = 0;
			do
			{
				struct EffectStep *fire = &work->particles[i];

				if (fire->variant == 0)
				{
					s32 size = i % 3 + 1;

					blit(canvas, sheet + ParticleStreams_CellOffsets[size - 1], fire->x,
						HI(fire->y) - size, size, size * 2);
					fire->x += 2;
					fire->y += fire->velocity_y;
					fire->velocity_y = fire->velocity_y * 48 / 64;
				}
				else
				{
					fire->variant--;
				}
				if (fire->x > 128 || fire->variant == 1)
				{
					fire->x = (Random16() & 31) + base_x + 172;
					fire->y = ((Random16() & 31) - wave + 56) << 16;
					fire->velocity_y = ((Random16() & 31) - 16) << 15;
				}
				i++;
			} while (i != 48);
		}

		if (frame > 31)
		{
			s32 rise = (frame - 32) / 2;
			s32 y;

			if (rise > 40)
			{
				rise = 40;
			}
			i = 0;
			y = 0;
			do
			{
				blit(canvas, work->sheet + (Random16() & 3) * 0x600, 120 - rise, y, 48, 32);
				i++;
				y += 18;
			} while (i != 6);
		}

		if (work->shake_frames > 0)
		{
			work->shake_frames--;
			gBgScroll[1].y = (Random16() & 7) + 28;
		}
		else
		{
			gBgScroll[1].y = 32;
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	gBgScroll[1].x = saved_x;
	control->busy = 0;
	BattleEffect_SetupBlendedDisplay();
	REG_WIN0H = 0xf0;

	i = 0;
	do
	{
		struct AnimationObject *object = work->objects[i];

		((u8 *)&object->part[0])[9] |= 12;
		i++;
	} while (i != 9);

	{
		s32 row_x;
		u8 *hit;
		u8 *jitter;

		row_x = 224;
		hit = hit_row;
		i = 0;
		do
		{
			hit[i] = 0;
			i++;
		} while (i != 14);
		jitter = jitter_row;
		i = 0;
		do
		{
			jitter[i] = Random16() & 31;
			i++;
		} while (i != 16);
		i = 0;
		do
		{
			PARTICLES[i].variant = 0;
			i++;
		} while (i != 160 << 1);

		work->transfer_mode = 2;
		work->transfer_value = 75;
		REG_BG2CNT = 0x784;
		REG_BLDALPHA = 0x1010;

		frame = 0;
		do
		{
			if (frame <= 23)
			{
				struct FxPair pair2 = ObjectRow_RisePair;
				s32			  ang;
				s32			  rise;

				row_x -= 16;
				if (frame <= 8)
				{
					ang = frame * 2048 + 0x4000;
					if (ang > 0x8000)
					{
						ang -= 0x8000;
					}
					rise = (Trig_Sin(ang) << 6) >> 16;
				}
				else
				{
					ang = frame * 2048 + 0x4000;
					if (ang > 0x8000)
					{
						ang -= 0x8000;
					}
					rise = (Trig_Sin(ang) << 5) >> 16;
				}
				place2.unknown_0c = 0;
				place2.y = 255 << 16;
				i = 0;
				do
				{
					place2.x = (row_x + ObjectRow_Columns[i]) << 16;
					place2.z = ((ObjectRow_Rows[i] - rise) << 16) + (144 << 15);
					Object_ApplyProjectedPlacementFar(work->objects[i], &place2, &pair2, 0);
					i++;
				} while (i != 9);
			}

			if (frame == 8)
			{
				work->shake_frames = frame;
				AudioCommand_PlayFar(145);
			}
			if (frame == 11)
			{
				AudioCommand_PlayFar(145);
			}
			if (frame == 46)
			{
				AudioCommand_PlayFar(137);
			}

			for (i = 0; i != work->effect->count; i++)
			{
				if (hit[i] == 0)
				{
					EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[i], &unit);
					if (unit.x > row_x)
					{
						s32 k;

						hit[i] = 1;
						k = 0;
						do
						{
							struct EffectStep *burst = &PARTICLES[i * 32 + k];
							s32				   up;

							burst->x = unit.x << 15;
							burst->y = (unit.y - 16) << 16;
							burst->velocity_x = ((Random16() & 255) - 128) << 10;
							up = (Random16() & 255) - 192;
							burst->velocity_y = up << 11;
							burst->x += burst->velocity_x * 4;
							burst->y += up << 13;
							burst->variant = (Random16() & 15) + 8;
							k++;
						} while (k != 32);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 1);
						AudioCommand_PlayFar(134);
					}
				}
			}

			i = 0;
			do
			{
				struct EffectStep *burst = &PARTICLES[i];

				if (burst->variant > 0)
				{
					s32 size = 3;

					blit(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
						HI(burst->x) - size / 2, HI(burst->y) - size, size, size * 2);
					burst->x += burst->velocity_x;
					burst->y += burst->velocity_y;
					burst->variant--;
				}
				i++;
			} while (i != 192);

			if (frame == 48)
			{
				AudioCommand_PlayFar(136);
			}
			if (frame > 40)
			{
				s32 slide = (frame - 40) * 12;
				s32 y;

				work->transfer_mode = 0;
				work->transfer_value = 75;
				i = 0;
				y = -8;
				do
				{
					blit(canvas, work->sheet + (Random16() & 3) * 0x600, jitter[i] - slide + 120, y,
						48, 32);
					i++;
					y += 8;
				} while (i != 16);
			}
			if (frame > 64)
			{
				work->transfer_mode = 2;
			}
			if (frame == 58)
			{
				for (i = 0; i != work->effect->count; i++)
				{
					ObjectGroup_UpdateMembers(work->effect->actors[i], 14, 5, -1, 0);
				}
			}

			Camera_ApplyShake(8, 8);
			ObjectGroup_TickMemberTimers();
			work->transfer_pending = 1;
			WaitFrames(1);
			frame++;
		} while (frame != 96);
	}

	BattleEventRuntime_BeginPhaseFar(134);
	i = 0;
	do
	{
		ResourceObject_ReleaseFar(work->objects[i]);
		i++;
	} while (i != 9);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

extern const struct Scale BattleFx6_UnitScale;

void Graphics_ResetBg2Pa(void)
{
	REG_BG2PA = 0x100;
}

void Graphics_SetBg2AffineScaleHalf(void)
{
	REG_BG2PA = 0x80;
}

void BattleFx_InitializeMode6(struct BattleEffectArgument *efx)
{
	s32						 pos[4];
	s32						 seat[3];
	void					*dst;
	u8						*aux;
	s32						 step;
	s32						 px;
	s32						 py;
	s32						 vx;
	s32						 vy;
	s32						 scroll;
	s32						*spot;
	s32						 j;
	s32						 burst;
	s32						 bx;
	u32						*cache;
	struct BattleEffectWork *work;
	s32						 frame;
	s32						 t;
	s32						 i;
	s32						 n;
	s32						 s;
	struct EffectStep		*q1;
	struct EffectStep		*q2;
	struct EffectStep		*q3;
	struct EffectStep		*q4;
	DrawRectangle			 blit[2];
	struct Scale			 scale;
	s32						 ground = 112;

	cache = (u32 *)&((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_CANVAS];
	dst = (void *)cache[40 - 40];
	work = (struct BattleEffectWork *)cache[39 - 40];
	aux = (u8 *)cache[41 - 40];
	work->effect = efx;
	BattleFx_BeginCanvasLayer(0);
	BattlePres_ConfigureEffectDisplay();
	PLTT_BACKDROP = 0;
	*(u16 *)0x05000002 = 0;
	Scheduler_AddOrUpdateCallback(Graphics_ResetBg2Pa, 0x480);
	Runtime_SetIrqHandler(2, 0, (s32)Graphics_SetBg2AffineScaleHalf);
	work->transfer_mode = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	REG_WININ = 0x2137;
	BattleEffect_WipeCanvas(1, 0);
	REG_WIN0H = 0xf0f0;
	BattleFx_SelectLivingTargets(work->effect);
	BattleFx_SpawnObjects(8, 0x17a, 1);
	gProjection.center_y = 240;
	WaitFrames(1);
	BattleBackground_LoadFar(1, (s32)&ResourceId_DuskCloudsBackdrop, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, aux, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, work->sheet + 0x4e20, 0, 0);
	REG_DISPCNT = 0x7741;
	REG_BG2PA = 0x80;
	REG_BLDALPHA = 0x1010;
	REG_BLDCNT = 0x3f44;
	step = 1;
	px = 140 << 17;
	py = 200 << 16;
	vx = -0x40000;
	vy = -0x80000;
	scroll = ((struct BgScrollWords *)gBgScroll)->unk04;
	spot = (s32 *)cache[44 - 40];
	work->transfer_mode = 2;
	work->transfer_value = 75;
	spot[4] = 1;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
	blit[0] = (DrawRectangle)cache[46 - 40];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
	blit[1] = (DrawRectangle)cache[47 - 40];
	REG_BG2CNT = 0x784;

	for (i = 0; i != 1024; i++)
	{
		PARTICLES[i].variant = -1;
	}
	for (i = 0, q1 = work->particles; i != 32; i++, q1++)
	{
		s32 a = (Random16() & 0x3fff) + 0x8000;
		s32 r = (Random16() & 127) + 255;
		q1->x = (Trig_Sin(a) * r) >> 2;
		q1->y = (Trig_Cos(a) * r) >> 2;
		q1->velocity_x = -q1->x / 32;
		q1->velocity_y = -q1->y / 32;
		q1->variant = 0;
	}
	for (i = 0; i != 712; i++)
	{
		s32 r = Random16() & 255;
		s32 a = Random16() & 0xffff;
		PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + 0x800000;
		PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + (192 << 15);
		PARTICLES[i].velocity_x = (-(Random16() & 0x1fff) - 0x800) << 5;
		PARTICLES[i].velocity_y = (-(Random16() & 0xfff) - 0x1800) << 5;
		PARTICLES[i].variant = 0;
	}

	frame = 0;
	do
	{
		if (frame == 24)
		{
			AudioCommand_PlayFar(162);
		}
		if (frame == 76)
		{
			AudioCommand_PlayFar(164);
		}
		if (frame == 154)
		{
			AudioCommand_PlayFar(142);
		}
		if (frame == 222)
		{
			AudioCommand_PlayFar(145);
		}
		if (frame >= 0 && frame < 16)
		{
			u16 *phase = (u16 *)Ram_MapCellBuffer;
			if (frame == 1)
			{
				u8 *noise = (u8 *)gMapCellBuffer + 2;
				s32 k;
				for (k = 0; k != 128; k++)
				{
					noise[k] = Random16() & 63;
				}
				*phase = 0;
				Scheduler_AddOrUpdateCallback(BattleFx_BuildWindowEdgeTable, 0x480);
			}
			*phase += step;
			step += 3;
			if (frame == 15)
			{
				Scheduler_RemoveCallback(BattleFx_BuildWindowEdgeTable);
			}
		}

		if (frame > 21)
		{
			scale = BattleFx6_UnitScale;
			if (frame <= 83)
			{
				if (frame > 48)
				{
					vx = vx * 60 / 64;
				}
				vy = vy * 60 / 64;
				if (py > 0x840000)
				{
					vy -= 0x2000;
				}
				else
				{
					vy += 0x2000;
				}
				px += vx;
				py += vy;
			}
			else if (frame <= 170)
			{
				if (frame <= 107)
				{
					vx += 0x2000;
				}
				vx = vx * 60 / 64;
				px += vx;
				if (frame <= 117)
				{
					if (py > 0x8a0000)
					{
						vy -= 0x2000;
					}
					else
					{
						vy += 0x2000;
					}
					vy = vy * 56 / 64;
					py += vy;
				}
			}
			pos[3] = 0;
			pos[1] = 0;
			for (i = 0; i != 8; i++)
			{
				pos[0] = (((BattleFx6_ObjectX[i] * 3) >> 1) << 16) + px - 0x200000;
				pos[2] = (((BattleFx6_ObjectY[i] * 3) >> 1) << 16) + py - 0x180000;
				Object_ApplyProjectedPlacementFar(work->objects[i], pos, &scale, 0);
			}
		}

		if (frame == 76)
		{
			for (i = 0; i != 1024; i++)
			{
				if (PARTICLES[i].variant == -1)
				{
					s32 r = Random16() & 255;
					s32 a = Random16() & 0xffff;
					PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + 0x400000;
					PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + (192 << 15);
					PARTICLES[i].velocity_x = (32 - (Random16() & 63)) << 12;
					PARTICLES[i].velocity_y = (-(Random16() & 31) - 16) << 13;
					PARTICLES[i].variant = 0;
				}
			}
		}
		if (frame == 152)
		{
			Runtime_SetIrqHandler(2, 96, (s32)Graphics_SetBg2AffineScaleHalf);
			work->fade_frames = 24;
			work->fade_step = 0;
		}

		t = frame - 152;
		if (t >= 0 && t < 88)
		{
			for (i = 0, q2 = work->particles; i != 32; i++, q2++)
			{
				if (frame >= i / 4 + 152 && frame < i / 4 + 152 + 32)
				{
					s32 fx = HI(q2->x) + 112;
					s32 fy = HI(q2->y) + 62;
					s32 r = (i & 3) + 5;
					s = r * 2;
					blit[0](dst, work->sheet + BattleFx6_FlareCells[r - 1] + 0x4e20, fx - r, fy - r,
						s, s);
					q2->x += q2->velocity_x;
					q2->y += q2->velocity_y;
				}
			}
		}

		if (frame == 222)
		{
			for (i = 0; i != 64; i++)
			{
				q3 = &work->particles[i];
				q3->x = (Random16() & 15) - 8;
				q3->y = (Random16() & 15) - 8;
				if (q3->x < 0)
				{
					q3->x -= 4;
				}
				else
				{
					q3->x += 4;
				}
				if (q3->y < 0)
				{
					q3->y -= 4;
				}
				else
				{
					q3->y += 4;
				}
				q3->x += 100;
				q3->y += 52;
				q3->velocity_x = -6;
				q3->z = q3->x;
				q3->velocity_y = q3->y;
				q3->variant = 0;
			}
		}
		if (frame > 221)
		{
			for (i = 0; i != 64; i++)
			{
				u16 *cells = BattleFx6_FlareCells;
				s32	 r = 1;
				if (frame >= i / 2 + 222)
				{
					blit[0](dst, work->sheet + cells[r - 1] + 0x4e20, work->particles[i].x - r,
						work->particles[i].y - r, r * 2, r * 2);
					work->particles[i].x += work->particles[i].velocity_x;
					work->particles[i].y += work->particles[i].velocity_y;
					if (work->particles[i].y < 0)
					{
						work->particles[i].y = work->particles[i].velocity_y;
						work->particles[i].x = work->particles[i].z;
					}
				}
			}
		}

		if (frame > 175)
		{
			n = 5;
			if (frame > 221)
			{
				n = 60;
				if (frame != 223 && frame != 224)
				{
					n = 40;
				}
			}
			if (n > 60)
			{
				n = 60;
			}
			for (i = 0; i != n; i++)
			{
				s32 w;
				s32 d = (i + 1) / 2;
				w = n * 2 - d * 4;
				if (w < 12)
				{
					w = 12;
				}
				if (w > 63)
				{
					w = 63;
				}
				if (i & 1)
				{
					d = -d;
				}
				BattleFx_DrawClippedCanvasLine(d + 48, 0, d / 2 + 111, 63 - (d + 1) / 2, w);
			}
		}

		if (t >= 0 && t < 40)
		{
			s32 z;
			z = t / 2 + 1;
			if (z > 4)
			{
				z = 4;
			}
			blit[1](dst, work->sheet + BattleFx6_FlareCells[z - 1] + 0x4e20, 108 - z, 60 - z, z * 2,
				z * 2);
		}
		if (frame >= 192 && frame < 200)
		{
			s32 z;
			z = (frame - 192) / 2 + 3;
			if (z > 6)
			{
				z = 6;
			}
			blit[1](dst, work->sheet + BattleFx6_FlareCells[z - 1] + 0x4e20, 108 - z, 60 - z, z * 2,
				z * 2);
		}
		if (frame > 199)
		{
			s32 z;
			s32 u = frame - 200;
			z = u / 4 + 5;
			if (z > 8)
			{
				z = 8;
			}
			blit[1](dst, work->sheet + BattleFx6_FlareCells[z - 1] + 0x4e20, 108 - z, 60 - z, z * 2,
				z * 2);
			if (frame <= 213)
			{
				z = u / 4 + 1;
				if (z > 4)
				{
					z = 4;
				}
				blit[1](dst, work->sheet + BattleFx6_FlareCells[z - 1] + 0x4e20, 100 - z, 52 - z,
					z * 2, z * 2);
			}
		}
		if (frame >= 214 && frame < 222)
		{
			s32 z;
			z = (frame - 214) / 4;
			if (z > 2)
			{
				z = 2;
			}
			blit[1](dst, work->sheet + z * 9 * 256 + 0x400, 63, 18, 48, 48);
		}
		if (frame > 221)
		{
			s32 z;
			z = (frame - 222) / 2 % 4 + 3;
			blit[0](dst, work->sheet + z * 9 * 256 + 0x400, 72, 30, 48, 48);
			if (z == 5)
			{
				blit[0](dst, work->sheet + 0x1600, 66, 22, 48, 48);
			}
		}

		if (frame > 27)
		{
			DrawRectangle draw = blit[1];
			for (i = 0; i != 1024; i++)
			{
				s32 m = 3;
				if (PARTICLES[i].variant >= 0)
				{
					s32 k = i % 3 + 2;
					draw(dst, aux + ParticleStreams_CellOffsets[k - 1], HI(PARTICLES[i].x) - k / 2,
						HI(PARTICLES[i].y) - k, k, k * 2);
					EffectStep_AdvanceWithGravity2D(&PARTICLES[i], 62, BattleFx6_Gravity[i & m]);
					PARTICLES[i].variant++;
					if (PARTICLES[i].velocity_y > 0 && HI(PARTICLES[i].y) > 104)
					{
						PARTICLES[i].variant = -1;
					}
				}
			}
		}
		for (i = 0; i != 5; i++)
		{
			blit[1](dst, work->sheet, i * 32 + ((frame / 4) & 31) - 32, 88, 32, 32);
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	} while (++frame != 244 && (frame <= 16 || !(gKeysRepeat & (KEY_A | KEY_B))));

	((struct BgScrollWords *)gBgScroll)->unk04 = scroll;
	spot[4] = 0;
	Runtime_SetIrqHandler(2, 0, 0);
	BattleEffect_SetupBlendedDisplay();
	Scheduler_RemoveCallback(Graphics_ResetBg2Pa);
	REG_WIN0H = 0xf0;
	work->transfer_mode = 2;
	work->transfer_value = 75;
	for (i = 0; i != 1024; i++)
	{
		PARTICLES[i].variant = 0;
	}
	for (i = 0; i != 16; i++)
	{
		work->particles[i].x = (Random16() & 31) + 32;
		work->particles[i].y = 0;
		work->particles[i].variant = 0;
	}
	for (i = 0; i != work->effect->count; i++)
	{
		EffectPosition_ApplyStepAndYOffset(work->effect->actors[i], (struct EffectPosition *)seat);
		work->particles[i].x = seat[0] / 2;
	}
	Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, work->sheet, 1, 1);
	AudioCommand_PlayFar(0x121);
	REG_BG2PA = 0x80;
	REG_BLDALPHA = 0x1010;

	frame = 0;
	do
	{
		if (frame == 96)
		{
			BattleEventRuntime_BeginPhaseFar(134);
		}
		for (i = 0; i != 5; i++)
		{
			q4 = &work->particles[i];
			if (frame == i * 16 + 7)
			{
				AudioCommand_PlayFar(154);
			}
			if (frame >= i * 16 && q4->variant <= 31)
			{
				s32 wobble = frame * 16 % 104;
				blit[1](dst, work->sheet, q4->x - 8, q4->y + wobble - 216, 17, 104);
				blit[1](dst, work->sheet, q4->x - 8, q4->y + wobble - 112, 17, 104 - wobble);
				blit[1](dst, work->sheet + 0x6e8, q4->x - 17, q4->y - 65, 34, 65);
				if (q4->y <= 111)
				{
					q4->y += 16;
				}
				else
				{
					q4->variant++;
				}
				if (q4->y > 111)
				{
					burst = 0;
					bx = q4->x;
					if (q4->variant <= 7)
					{
						work->shake_frames = 4;
					}
					for (j = 0; j != 1024; j++)
					{
						if (PARTICLES[j].variant == 0)
						{
							s32 r = 0x3ff;
							s32 a;
							r &= Random16();
							a = (Random16() & 0x7fff) - 0x4000;
							PARTICLES[j].x = bx << 16;
							PARTICLES[j].y = ground << 16;
							r += 32;
							PARTICLES[j].velocity_x = (Trig_Sin(a) * r) >> 7;
							PARTICLES[j].velocity_y = -(Trig_Cos(a) * r * 2) >> 7;
							PARTICLES[j].variant = (Random16() & 7) + 32;
							if (++burst == 4)
							{
								break;
							}
						}
					}
				}
			}
		}
		for (i = 0; i != work->effect->count; i++)
		{
			if (frame == i * 16 + 8)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 8);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 2);
			}
		}
		for (i = 0; i != 1024; i++)
		{
			if (PARTICLES[i].variant > 0)
			{
				s32 x;
				s32 y;
				PARTICLES[i].variant--;
				EffectStep_AdvanceWithGravity2D(&PARTICLES[i], 60, 0x4000);
				y = PARTICLES[i].y >> 16;
				if (y > 120)
				{
					PARTICLES[i].velocity_y = -PARTICLES[i].velocity_y / 2;
				}
				else if (PARTICLES[i].x >= 0 && (x = PARTICLES[i].x >> 16) <= 126
						 && PARTICLES[i].y >= 0)
				{
					s32 k = PARTICLES[i].variant / 8 + 3;
					blit[i & 1](
						dst, aux + ParticleStreams_CellOffsets[k - 1], x - k / 2, y - k, k, k * 2);
				}
			}
		}
		Camera_ApplyShake(4, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	} while (++frame != 144);

	for (i = 0; i != 8; i++)
	{
		ResourceObject_ReleaseFar(work->objects[i]);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// wait_then_set_field.c
// バトル演出：対象へ近づいてからのスパーク（型 4）
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 行動者を最初の対象へ跳んで近づかせ、29 フレーム後に型 4 のスパークを出す
// effect : 演出の引数
//--------------------------------------------------------------------------
void BattleFx_RunSparkApproachVariant4(struct BattleEffectArgument *effect)
{
	// 距離の 3/4 まで、移動量 / 24 の速さ、縦の初速 12.2 でとぶ
	BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 24, 0xc3333);
	WaitFrames(29);
	effect->variant = 4;
	BattleFx_RunSparkGroups(effect, 2); // 2 : 行動者の位置から
}

//--------------------------------------------------------------------------
// wait_twelve_frames_then_set_field.c
// バトル演出：対象へ近づいてからのスパーク（型 3）
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 行動者を最初の対象へ跳んで近づかせ、12 フレーム後に型 3 のスパークを出す
// effect : 演出の引数
//--------------------------------------------------------------------------
void BattleFx_RunSparkApproachVariant3(struct BattleEffectArgument *effect)
{
	// 距離の 3/4 まで、移動量 / 24 の速さ、縦の初速 7.2 でとぶ
	BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 24, 0x73333);
	WaitFrames(12);
	effect->variant = 3;
	BattleFx_RunSparkGroups(effect, 2); // 2 : 行動者の位置から
}

//--------------------------------------------------------------------------
// colburst.c
// バトル演出：4 コマの一枚絵と、立ちのぼる柱と火花
//--------------------------------------------------------------------------

void BattleFx_RunRevealColumnMode1(s32 arg)
{
	BattleFx_RunRevealColumn(arg, 1);
}

void BattleFx_RunRevealColumnMode2(s32 arg)
{
	BattleFx_RunRevealColumn(arg, 2);
}

//--------------------------------------------------------------------------
// 120x120 の絵を 4 コマ、4 フレームずつキャンバスに出し、
// キャンバスを塗りつぶしてから対象に当てる
// variant : 0〜2 で絵が変わる
//--------------------------------------------------------------------------
void BattleFx_RunRevealColumn(struct BattleEffectArgument *effect, s32 variant)
{
	void				   **heap;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 draw[2];
	s32						 frame;

	heap = gBattleFxWork;
	work = *heap++;
	canvas = *heap;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDCNT = 0;

	// 1・2 コマ目は work->sheet、3・4 コマ目は gMapCellBuffer（各 120x120）
	if (variant == 0)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskE, work->sheet, 1, 0);
		Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskF, gMapCellBuffer, 1, 1);
	}
	else if (variant == 1)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskC, work->sheet, 1, 0);
		Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskD, gMapCellBuffer, 1, 1);
	}
	else
	{
		Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskA, work->sheet, 1, 0);
		Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskB, gMapCellBuffer, 1, 1);
	}
	work->transfer_mode = 1; // 毎フレーム 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 行動側を対象へ 16 フレームで寄せる
	if (variant == 1)
		BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0x80000);
	else
		BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
	WaitFrames(16);

	if (work->effect->side == 1)
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_YOMI);
	else
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_YOMI);
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	AudioCommand_PlayFar(212);
	for (frame = 0; frame != 21; frame++)
	{
		if (frame <= 3)
			draw[0](canvas, work, 0, 0, 120, 120);
		else if (frame <= 7)
			draw[0](canvas, work->sheet + 0x3840, 0, 0, 120, 120);
		else if (frame <= 11)
			draw[0](canvas, gMapCellBuffer, 0, 0, 120, 120);
		else if (frame <= 15)
			draw[0](canvas, (u8 *)gMapCellBuffer + 0x3840, 0, 0, 120, 120);
		if (frame >= 16 && frame <= 19)
			Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f); // キャンバス全体を 0x3f 番で塗る
		if (frame == 18)
			BattleEventRuntime_BeginPhaseFar(134);
		if (frame == 20)
		{
			work->shake_frames = 8;
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
		}
		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 対象の足元から柱が 2 本（左右対称）立ちのぼり、火花が 16 個散る
//--------------------------------------------------------------------------
void BattleFx_RunRisingBurst(struct BattleEffectArgument *effect)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	void					*sprites;
	DrawRectangle			 draw[2];
	struct EffectPosition	 pos;
	struct EffectStep		*spark;
	struct EffectStep		*p;
	s32						 frame;
	s32						 i;
	s32						 height;
	s32						 mode;
	s32						 x;
	u8						*src;
	s32						 speed;
	s32						 angle;
	s32						 cell;
	s32						 size;

	heap = gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	sprites = heap[2];
	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100; // 横も等倍
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet + 0x460, 1, 0);
	BattleFx_FetchRectangleBlitters(work->effect->side, draw);
	work->transfer_mode = 2; // 残像 3/4
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// キャンバスの x = 64 が対象に来るようにずらす
	EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &pos);
	REG_BG2X = (64 - pos.x) << 8;

	// 火花：(64, 112) から四方へ散らし、上へ流す
	for (i = 0, spark = work->particles; i != 16; i++)
	{
		speed = (Random16() & 0x1ff) + 128;
		angle = Random16() & 0xffff;
		spark->x = 0x400000;
		spark->y = 0x700000;
		spark->velocity_x = (Trig_Sin(angle) * speed) >> 8;
		spark->velocity_y = (Trig_Cos(angle) * speed) >> 9;
		spark->variant = Random16() & 7;
		spark++;
	}
	work->shake_frames = 8;
	for (frame = 0; frame != 54; frame++)
	{
		if (frame == 10)
		{
			work->shake_frames = 8;
			BattleEventRuntime_BeginPhaseFar(212);
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
		}

		// 柱：8〜31 フレームで 12 ドットずつ伸び、そのあと 6 ドットずつ縮む
		// 80 ドットで止まっている間は比較合成、それ以外は加算合成
		if (frame > 7)
		{
			if (frame <= 31)
				height = frame * 12 - 96;
			else
				height = 272 - frame * 6;
			if (height > 0)
			{
				if (height > 80)
				{
					height = 80;
					mode = GOUSEI_HIKAKU;
				}
				else
				{
					mode = GOUSEI_KASAN;
				}
				for (i = 0, x = 50; i != 2; i++, x += 14)
				{
					if (i == 0)
						BattleEffect_LoadWork(
							HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, mode);
					else
						BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7,
							GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, mode);
					draw[0] = ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER]);
					((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER])(
						canvas, work, x, 112 - height, 14, height);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
				}
			}
		}

		// 火花：3 フレームごとに次の絵へ（7 段）
		BattleFx_FetchRectangleBlitters(work->effect->side, draw);
		for (i = 0, p = work->particles; i != 16; i++)
		{
			if (frame >= i / 2 + 8 && p->variant <= 28)
			{
				s32 px;
				s32 py;

				cell = p->variant / 3;
				px = HI(p->x);
				py = HI(p->y);
				if (cell > 6)
					cell = 6;
				src = work->sheet + RisingBurst_SparkCells[cell] + 0x460;
				size = RisingBurst_SparkSizes[cell];
				draw[0](canvas, src, px - size / 2, py - size / 2, size, size);
				p->variant++;
				EffectStep_AdvanceWithGravity2D(p, 62, -0x2000);
			}
			p++;
		}
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		if (frame <= 7)
			Camera_ApplyShake(2, 2);
		else
			Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

#define DMA_COPY_WORDS(n) (0x84000000 | (n))

#define BRANCH_FIELD(site, target) ((((u32)(target) - (u32)(site) - 8) >> 2) & 0x00FFFFFF)

s32 BattleEffect_LoadWork(s32 slot, s32 width_shift, s32 height_shift, s32 flags, u32 mode)
{
	const u32 *src;
	u32		  *dst;
	u32		  *no_rows;
	u32		  *no_columns;
	u32		  *row;
	u32		  *skip;
	u32		  *run;
	u32		  *cell;
	const u32 *end;
	s32		   words;

#define PUT(word) words++
#define COPY(n)   \
	words += (n); \
	src += (n)
#define COPY_FROM(from, n) words += (n)
#define COPY_TWICE(from, n) \
	words += (n);           \
	words += (n)
#define PUT_BRANCH(target) words++
#define MARK(site)
#define PATCH(site)

	words = 0;
	src = SentouKouka_Gousei;

	COPY(3);
	if (mode == GOUSEI_KASAN)
	{
		COPY_FROM(SentouKouka_Mask, 3);
	}

	if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP_X)
	{
		COPY_FROM(src, 3);
	}
	src += 3;
	if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP_Y)
	{
		COPY_FROM(src, 4);
	}
	src += 4;
	if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP)
	{
		COPY_FROM(src, 3);
	}
	src += 3;
	if ((flags & GOUSEI_FLIP) == 0)
	{
		PUT(src[0]);
	}
	src += 1;

	if ((flags & GOUSEI_CLIP_Y) != 0)
	{
		PUT(src[0]);
		PUT(src[1]);
		if ((flags & GOUSEI_FLIP_Y) != 0)
		{
			PUT(src[2]);
		}
		else
		{
			PUT(src[3]);
		}
		PUT(src[4]);
		PUT(src[5]);
		PUT(src[6] + (1 << height_shift));
		PUT(src[7] + (1 << height_shift));
	}
	src += 8;

	PUT(*src++);
	MARK(no_rows);
	PUT(*src++);

	if ((flags & GOUSEI_CLIP_X) == 0)
	{
		PUT(src[0] + (1 << width_shift) - 1);
	}
	src += 1;
	if ((flags & GOUSEI_CLIP_X) != 0)
	{
		PUT(src[0]);
		PUT(src[1]);
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			PUT(src[2]);
			PUT(src[3]);
		}
		else
		{
			PUT(src[4]);
			PUT(src[5]);
		}
		PUT(src[6]);
		PUT(src[7]);
		PUT(src[8] + SentouKouka_BitOperands[width_shift]);
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			PUT(src[9]);
		}
		else
		{
			PUT(src[10]);
		}
		PUT(src[11] + SentouKouka_BitOperands[width_shift]);
	}
	src += 12;

	PUT(*src++);
	MARK(no_columns);
	PUT(*src++);

	COPY(6);

	PUT(*src++);
	if ((flags & GOUSEI_CLIP_X) == 0)
	{
		PUT(src[0]);
	}
	src += 1;

	COPY(5);

	MARK(row);
	PUT(src[0]);
	PUT(src[1] + (1 << (height_shift - 3)) - 1);
	PUT(src[2] + ((width_shift - 3) << 7));
	PUT(src[3]);
	src += 4;

	if ((flags & GOUSEI_CLIP_X) == 0)
	{
		PUT(src[0]);
	}
	src += 1;

	PUT(*src++);
	MARK(skip);
	PUT(*src++);
	if ((flags & GOUSEI_FLIP_X) != 0)
	{
		PUT(src[0]);
	}
	else
	{
		PUT(src[1]);
	}
	src += 2;

	MARK(run);
	switch (mode)
	{
	default:
	case GOUSEI_YOMI:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_YomiGyaku, 2);
		}
		else
		{
			COPY_FROM(SentouKouka_YomiJun, 2);
		}
		break;
	case GOUSEI_NURI:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_NuriGyaku, 4);
		}
		else
		{
			COPY_FROM(SentouKouka_NuriJun, 4);
		}
		break;
	case GOUSEI_HIKAKU:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_HikakuGyaku, 4);
		}
		else
		{
			COPY_FROM(SentouKouka_HikakuJun, 4);
		}
		break;
	case GOUSEI_KASAN:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_KasanGyaku, 6);
		}
		else
		{
			COPY_FROM(SentouKouka_KasanJun, 6);
		}
		break;
	}
	src += 4;

	PUT(*src++);
	PUT_BRANCH(run);
	PUT(*src++);
	if ((flags & GOUSEI_CLIP_X) == 0)
	{
		PUT(src[0]);
		PUT(src[1]);
	}
	src += 2;
	PATCH(skip);

	PUT(*src++);
	MARK(skip);
	PUT(*src++);

	MARK(cell);
	switch (mode)
	{
	default:
	case GOUSEI_YOMI:
	{
		s32 i;

		for (i = 0; i < 8; i++)
		{
			if ((flags & GOUSEI_FLIP_X) != 0)
			{
				COPY_FROM(SentouKouka_YomiGyaku, 2);
			}
			else
			{
				COPY_FROM(SentouKouka_YomiJun, 2);
			}
		}
	}
	break;
	case GOUSEI_NURI:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_Nuri8Gyaku, 25);
		}
		else
		{
			COPY_FROM(SentouKouka_Nuri8Jun, 25);
		}
		break;
	case GOUSEI_HIKAKU:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_TWICE(SentouKouka_Hikaku4Gyaku, 16);
		}
		else
		{
			COPY_TWICE(SentouKouka_Hikaku4Jun, 16);
		}
		break;
	case GOUSEI_KASAN:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_TWICE(SentouKouka_IroGyaku, 14);
		}
		else
		{
			COPY_TWICE(SentouKouka_IroJun, 14);
		}
		break;
	}
	src += 4;

	PUT(*src++);
	if ((flags & GOUSEI_CLIP_X) == 0)
	{
		PUT(src[0]);
		PUT(src[1]);
	}
	src += 2;

	PUT(*src++);
	PUT_BRANCH(cell);
	PATCH(skip);

	PUT(*src++);
	MARK(skip);
	PUT(*src++);
	if ((flags & GOUSEI_FLIP_X) != 0)
	{
		PUT(src[0]);
	}
	else
	{
		PUT(src[1]);
	}
	src += 2;

	MARK(run);
	switch (mode)
	{
	default:
	case GOUSEI_YOMI:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_YomiGyaku, 2);
		}
		else
		{
			COPY_FROM(SentouKouka_YomiJun, 2);
		}
		break;
	case GOUSEI_NURI:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_NuriGyaku, 4);
		}
		else
		{
			COPY_FROM(SentouKouka_NuriJun, 4);
		}
		break;
	case GOUSEI_HIKAKU:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_HikakuGyaku, 4);
		}
		else
		{
			COPY_FROM(SentouKouka_HikakuJun, 4);
		}
		break;
	case GOUSEI_KASAN:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_KasanGyaku, 6);
		}
		else
		{
			COPY_FROM(SentouKouka_KasanJun, 6);
		}
		break;
	}
	src += 4;

	PUT(*src++);
	PUT_BRANCH(run);
	PATCH(skip);

	COPY(3);

	PUT_BRANCH(row);
	PATCH(no_rows);
	PATCH(no_columns);

	PUT(*src++);
	PUT(*src++);

#undef PUT
#undef COPY
#undef COPY_FROM
#undef COPY_TWICE
#undef PUT_BRANCH
#undef MARK
#undef PATCH

	/* FAKEMATCH: one-pass blocks keep each writing step together and in list order while allowing scheduling within it. Measured 2026-10-13 in all six editions, written as plain statements: PUT differs 38, COPY 12 and PATCH 2, so each of the three carries a tag and counts at its uses. COPY_FROM, COPY_TWICE and PUT_BRANCH match without a block and have none. */
#define PUT(word)        \
	do                   \
	{                    \
		*dst++ = (word); \
	} while (0)
	/* FAKEMATCH: a one-pass block, as PUT's tag explains; written as plain statements, COPY differs 12 in all six editions. */
#define COPY(n)                                         \
	do                                                  \
	{                                                   \
		Dma_Set(src, dst, DMA_COPY_WORDS(n), REG_DMA3); \
		dst += (n);                                     \
		src += (n);                                     \
	} while (0)
#define COPY_FROM(from, n)                           \
	Dma_Set(from, dst, DMA_COPY_WORDS(n), REG_DMA3); \
	dst += (n)
#define COPY_TWICE(from, n)                              \
	{                                                    \
		const u32 *part = (from);                        \
		Dma_Set(part, dst, DMA_COPY_WORDS(n), REG_DMA3); \
		dst += (n);                                      \
		Dma_Set(part, dst, DMA_COPY_WORDS(n), REG_DMA3); \
		dst += (n);                                      \
	}
#define PUT_BRANCH(target)                     \
	*dst = *src++ + BRANCH_FIELD(dst, target); \
	dst++
#define MARK(site) site = dst
	/* FAKEMATCH: a one-pass block, as PUT's tag explains; written as a plain statement, PATCH differs 2 in all six editions. */
#define PATCH(site)                         \
	do                                      \
	{                                       \
		*(site) |= BRANCH_FIELD(site, dst); \
	} while (0)

	dst = (u32 *)Runtime_AllocateHeapBlock(slot, words << 2);
	src = SentouKouka_Gousei;

	COPY(3);
	if (mode == GOUSEI_KASAN)
	{
		COPY_FROM(SentouKouka_Mask, 3);
	}

	if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP_X)
	{
		COPY_FROM(src, 3);
	}
	src += 3;
	if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP_Y)
	{
		COPY_FROM(src, 4);
	}
	src += 4;
	if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP)
	{
		COPY_FROM(src, 3);
	}
	src += 3;
	if ((flags & GOUSEI_FLIP) == 0)
	{
		PUT(src[0]);
	}
	src += 1;

	if ((flags & GOUSEI_CLIP_Y) != 0)
	{
		PUT(src[0]);
		PUT(src[1]);
		if ((flags & GOUSEI_FLIP_Y) != 0)
		{
			PUT(src[2]);
		}
		else
		{
			PUT(src[3]);
		}
		PUT(src[4]);
		PUT(src[5]);
		PUT(src[6] + (1 << height_shift));
		PUT(src[7] + (1 << height_shift));
	}
	src += 8;

	PUT(*src++);
	MARK(no_rows);
	PUT(*src++);

	if ((flags & GOUSEI_CLIP_X) == 0)
	{
		PUT(src[0] + (1 << width_shift) - 1);
	}
	src += 1;
	if ((flags & GOUSEI_CLIP_X) != 0)
	{
		PUT(src[0]);
		PUT(src[1]);
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			PUT(src[2]);
			PUT(src[3]);
		}
		else
		{
			PUT(src[4]);
			PUT(src[5]);
		}
		PUT(src[6]);
		PUT(src[7]);
		PUT(src[8] + SentouKouka_BitOperands[width_shift]);
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			PUT(src[9]);
		}
		else
		{
			PUT(src[10]);
		}
		PUT(src[11] + SentouKouka_BitOperands[width_shift]);
	}
	src += 12;

	PUT(*src++);
	MARK(no_columns);
	PUT(*src++);

	COPY(6);

	PUT(*src++);
	if ((flags & GOUSEI_CLIP_X) == 0)
	{
		PUT(src[0]);
	}
	src += 1;

	COPY(5);

	MARK(row);
	PUT(src[0]);
	PUT(src[1] + (1 << (height_shift - 3)) - 1);
	PUT(src[2] + ((width_shift - 3) << 7));
	PUT(src[3]);
	src += 4;

	if ((flags & GOUSEI_CLIP_X) == 0)
	{
		PUT(src[0]);
	}
	src += 1;

	PUT(*src++);
	MARK(skip);
	PUT(*src++);
	if ((flags & GOUSEI_FLIP_X) != 0)
	{
		PUT(src[0]);
	}
	else
	{
		PUT(src[1]);
	}
	src += 2;

	MARK(run);
	switch (mode)
	{
	default:
	case GOUSEI_YOMI:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_YomiGyaku, 2);
		}
		else
		{
			COPY_FROM(SentouKouka_YomiJun, 2);
		}
		break;
	case GOUSEI_NURI:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_NuriGyaku, 4);
		}
		else
		{
			COPY_FROM(SentouKouka_NuriJun, 4);
		}
		break;
	case GOUSEI_HIKAKU:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_HikakuGyaku, 4);
		}
		else
		{
			COPY_FROM(SentouKouka_HikakuJun, 4);
		}
		break;
	case GOUSEI_KASAN:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_KasanGyaku, 6);
		}
		else
		{
			COPY_FROM(SentouKouka_KasanJun, 6);
		}
		break;
	}
	src += 4;

	PUT(*src++);
	PUT_BRANCH(run);
	PUT(*src++);
	if ((flags & GOUSEI_CLIP_X) == 0)
	{
		PUT(src[0]);
		PUT(src[1]);
	}
	src += 2;
	PATCH(skip);

	PUT(*src++);
	MARK(skip);
	PUT(*src++);

	MARK(cell);
	switch (mode)
	{
	default:
	case GOUSEI_YOMI:
	{
		s32 i;

		for (i = 0; i < 8; i++)
		{
			if ((flags & GOUSEI_FLIP_X) != 0)
			{
				COPY_FROM(SentouKouka_YomiGyaku, 2);
			}
			else
			{
				COPY_FROM(SentouKouka_YomiJun, 2);
			}
		}
	}
	break;
	case GOUSEI_NURI:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_Nuri8Gyaku, 25);
		}
		else
		{
			COPY_FROM(SentouKouka_Nuri8Jun, 25);
		}
		break;
	case GOUSEI_HIKAKU:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_TWICE(SentouKouka_Hikaku4Gyaku, 16);
		}
		else
		{
			COPY_TWICE(SentouKouka_Hikaku4Jun, 16);
		}
		break;
	case GOUSEI_KASAN:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_TWICE(SentouKouka_IroGyaku, 14);
		}
		else
		{
			COPY_TWICE(SentouKouka_IroJun, 14);
		}
		break;
	}
	src += 4;

	PUT(*src++);
	if ((flags & GOUSEI_CLIP_X) == 0)
	{
		PUT(src[0]);
		PUT(src[1]);
	}
	src += 2;

	PUT(*src++);
	PUT_BRANCH(cell);
	PATCH(skip);

	PUT(*src++);
	MARK(skip);
	PUT(*src++);
	if ((flags & GOUSEI_FLIP_X) != 0)
	{
		PUT(src[0]);
	}
	else
	{
		PUT(src[1]);
	}
	src += 2;

	MARK(run);
	switch (mode)
	{
	default:
	case GOUSEI_YOMI:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_YomiGyaku, 2);
		}
		else
		{
			COPY_FROM(SentouKouka_YomiJun, 2);
		}
		break;
	case GOUSEI_NURI:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_NuriGyaku, 4);
		}
		else
		{
			COPY_FROM(SentouKouka_NuriJun, 4);
		}
		break;
	case GOUSEI_HIKAKU:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_HikakuGyaku, 4);
		}
		else
		{
			COPY_FROM(SentouKouka_HikakuJun, 4);
		}
		break;
	case GOUSEI_KASAN:
		if ((flags & GOUSEI_FLIP_X) != 0)
		{
			COPY_FROM(SentouKouka_KasanGyaku, 6);
		}
		else
		{
			COPY_FROM(SentouKouka_KasanJun, 6);
		}
		break;
	}
	src += 4;

	PUT(*src++);
	PUT_BRANCH(run);
	PATCH(skip);

	COPY(3);

	PUT_BRANCH(row);
	PATCH(no_rows);
	PATCH(no_columns);

	PUT(*src++);
	PUT(*src++);

	end = (const u32 *)ParticleStreams_CellOffsets;
	return src == end;
}

// 拡大率 {x, y} (Q16)
const struct Scale BattleFx_StarsUnitScale = {
	0x10000,
	0x10000,
};

const Scale BattleFx10_UnitScale = {
	0x10000,
	0x10000,
};

// 岩の壁の高さ
const s32 RockWall_Heights[4] = {
	-32,
	-32,
	-32,
	-32,
};

const s16 Mode12_Points[6][2] = {
	{8, 135},
	{184, 135},
	{128, 135},
	{96, 32},
	{96, 32},
	{64, 135},
};

const struct Scale RisingWall_UnitScale = {
	0x10000,
	0x10000,
};

const s32 Formation_NormalScale[2] = {
	0x10000,
	0x10000,
};

const s32 Formation_SmallScale[2] = {
	0x14000,
	0x14000,
};

const struct Scale CirclingScene_UnitScale = {
	0x10000,
	0x10000,
};

const struct Scale ShockwaveScene_UnitScale = {
	0x10000,
	0x10000,
};

const struct FxPair ObjectRow_SweepPair = {
	0x10000,
	0x10000,
};

const struct FxPair ObjectRow_RisePair = {
	0x10000,
	0x10000,
};

const struct Scale BattleFx6_UnitScale = {
	0x18000,
	0x18000,
};
