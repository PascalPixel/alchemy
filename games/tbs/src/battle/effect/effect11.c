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

// 倍率 {x, y} (Q16)
typedef struct Scale
{
	s32 x;
	s32 y;
} Scale;

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

	heap_cache = (void **)gBattleFxWork;
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

	work = *(struct BattleEffectWork **)gBattleFxWork;
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

	heap_cache = (void **)gBattleFxWork;
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
	struct BattleEffectWork *work = *(void **)gBattleFxWork;
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
	heap = (void **)gBattleFxWork;
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
	struct BattleEffectWork *work = *(struct BattleEffectWork **)gBattleFxWork;
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

	cache = &((void **)gBattleFxWork)[1];
	canvas = cache[0];
	work = cache[-1];

	work->effect = effect;
	BattleFx_BeginCanvasLayer(0x2000);
	REG_BG2PA = 0x100;
	/* FAKEMATCH: test through direction so CSE retains the mode reload
	 * in this arm. Testing mode directly replaces ldr r4,[sp,#76] with
	 * movs r4,#1 (1532 instructions, two differing rows). */
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
		for (frame = 0; frame != 192; frame++, work->frame++)
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
					goto skipped;
			}
			else
			{
				if ((gKeysRepeat & 3) && frame > 4)
					goto skipped;
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
		}
	skipped:
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

	cache = &((void **)gBattleFxWork)[1];
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

	heap_cache = (void **)gBattleFxWork;
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
	volatile s32			 fill;
	DrawRectangle			 routine[2];
	s32						 i;
	s32						 frame;
	s32						 size;
	u8						*burst;

	heap_cache = (void **)gBattleFxWork;
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
			fill = 0x3f3f3f3f;
			Dma_Set((void *)&fill, canvas, 0x85001000, REG_DMA3);
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

	heap_cache = (void **)gBattleFxWork;
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

	heap = (void **)gBattleFxWork;
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

		/* FAKEMATCH: needed to match: the mask is loaded into the magnitude before the random word is taken in, as the native code does */
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

#define QUEUE_IO_WRITE(address, value, delay)                                  \
	{                                                                          \
		struct IoWriteQueue *q;                                                \
		u32					 saved;                                            \
		s32					 count;                                            \
                                                                               \
		q = &gIoWriteQueue;                                                    \
		do                                                                     \
		{                                                                      \
			/* FAKEMATCH: a one-pass block from the IME read to its restore, \
               after q: its loop notes bound sched2 where the native's queue \
               writes are bounded (q loads ahead of the A/B branch's \
               frame = 150 store, the frame 64 block's fill stays after the \
               restore); without it 6 of 2455 rows differ. */ \
			do                                                                 \
			{                                                                  \
				/* FAKEMATCH: the one-pass IME read of io.c's and canvas.c's \
                   queue writes; without it 16 of 2455 rows differ. */ \
				saved = *ime;                                                  \
			} while (0);                                                       \
			*ime = (u16)ime;                                                   \
			count = q->count;                                                  \
			if (count <= 31)                                                   \
			{                                                                  \
				u32 *destination = q->entries[count];                          \
				/* FAKEMATCH: the count stored through a u16 pointer, as \
                   io_write_queue.h's and canvas.c's queue writes (the same \
                   halfword store, scheduled as the native's); stored as \
                   q->count, 8 of 2455 rows differ. */     \
				*(u16 *)&q->count = count + 1;                                 \
				*destination++ = (value);                                      \
				*destination++ = (address);                                    \
				*destination = (delay);                                        \
			}                                                                  \
			*ime = saved;                                                      \
		} while (0);                                                           \
	}

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

	cache = &((void **)gBattleFxWork)[1];
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
		/* FAKEMATCH: needed to match: a form the bytes prove: the amplitude is a value, not a literal: the native hoists 255 into sl and multiplies with muls */
		s32 amp = 255;
		/* FAKEMATCH: needed to match: register choice: the velocities' zero is a value set at the top of the body, which loop.c hoists into r8 as the native's */
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
				size = __divsi3(z, 100) + 1;
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
				size = __divsi3(scene[2], 20) + 2;
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
				/* FAKEMATCH: needed to match: the height is a value, so high / 2 is hoisted into fp */
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
			AnimationObjects_SelectAnimationFar(obj, __modsi3(i, 3));
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
				QUEUE_IO_WRITE(0x04000020, 0x80, 0x20000);
				QUEUE_IO_WRITE(0x04000028, 0, 0x30000);
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
			QUEUE_IO_WRITE(0x04000020, 0x80, 0x20000);
			QUEUE_IO_WRITE(0x04000028, 0, 0x30000);
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
			/* FAKEMATCH: needed to match: the size is a value, so 5 and 20 are hoisted into fp and sl */
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
						/* FAKEMATCH: needed to match: a dead branch restores the label CSE and reload saw between the edges */
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
						/* FAKEMATCH: needed to match: a dead branch restores the label CSE and reload saw between the edges */
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
			/* FAKEMATCH: needed to match: forced temporary: the shake value through an s32 keeps the AND constant an immediate and the instruction count native's */
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

	cursor = &((void **)gBattleFxWork)[1];
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
	scroll = ((struct Cells03001ad0 *)gBgScroll)->unk04;
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

	((struct Cells03001ad0 *)gBgScroll)->unk04 = scroll;
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

	heap = (void **)gBattleFxWork;
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

	heap = (void **)gBattleFxWork;
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

	/* FAKEMATCH: needed to match: one-pass blocks keep each writing step together and in list order while allowing scheduling within each step 2026-10-11: COPY_FROM, COPY_TWICE and PUT_BRANCH match without their blocks; without, PUT differs 38, COPY 12 and PATCH 2. */
#define PUT(word)        \
	do                   \
	{                    \
		*dst++ = (word); \
	} while (0)
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
