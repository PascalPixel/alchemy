#include "unit/battle/effect/fxscales.h"
#include "unit/battle/effect/koukabit.h"
#include "unit/battle/effect/koukatbl.h"
#include "unit/battle/effect/effect.h"
#include "unit/battle/effect/effect2.h"
#include "unit/battle/effect/effect3.h"
#include "unit/battle/effect/effect4.h"
#include "unit/battle/effect/effect5.h"
#include "unit/battle/effect/effect6.h"
#include "unit/battle/effect/effect7.h"
#include "unit/battle/effect/effect8.h"
#include "unit/battle/effect/effect9.h"
#include "unit/battle/effect/effect10.h"
#include "unit/battle/effect/effect11.h"
#include "unit/battle/effect/object.h"
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
#include "call.h"
#include "btlsys.h"
#include "battle_session.h"
#include "animspr.h"
#include "battle_unit.h"
#include "fixed_point_position.h"
#include "irq.h"

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
	struct BattleEffectWork *work = *(struct BattleEffectWork **)gBattleFxWork;
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

	work = *(struct BattleEffectWork **)gBattleFxWork;
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
	DrawRectangle			 draw[2];
	struct EffectStep		*shard;
	struct EffectStep		*puff;
	u16						*line;
	s32						 i;
	s32						 k;

	heap_cache = (void **)gBattleFxWork;
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

	heap_cache = (void **)gBattleFxWork;
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

	slots = (void **)gBattleFxWork;
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
	struct BlitterPair		 draw;
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
	draw.upper = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 11, 2);
	draw.lower = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER_ALTERNATE];
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
				draw.upper(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
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
	DrawRectangle			 draw[2];
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

	heap_cache = (void **)gBattleFxWork;
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
	/* FAKEMATCH: needed to match: an unused second blitter word fills the frame's 8-byte draw slot */
	DrawRectangle		  draw[2];
	struct BtlObj		 *first;
	struct BtlObj		 *second;
	struct MotionObject	 *user;
	struct MotionObject	 *target;
	s32					  x;
	s32					  z;
	s32					  user_height;
	s32					  target_height;
	s32					  point[3];
	struct EffectPosition screen;
	s32					  frame;
	s32					  percent;

	heap_cache = (void **)gBattleFxWork;
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
	DrawRectangle			 draw[2];
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			*blit;
	struct EffectStep		*blade;
	s32						 frame;
	s32						 i;

	heap_cache = (void **)gBattleFxWork;
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
