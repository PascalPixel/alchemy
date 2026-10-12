#include "unit/battle/effect/fxscales.h"
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
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object16.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtim10.h"
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
	DrawRectangle			 draw[2];

	cursor = (void **)gBattleFxWork;
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
			((struct Cells03001ad0 *)gBgScroll)->unk04 = ox;
			((struct Cells03001ad0 *)gBgScroll)->unk06 = oy + 32;
		}
		else
		{
			ox = 0;
			oy = 0;
			((struct Cells03001ad0 *)gBgScroll)->unk04 = 0;
			((struct Cells03001ad0 *)gBgScroll)->unk06 = 32;
		}
		if (frame >= 176 && frame < 180)
		{
			ox = -BattleFx10_ShakeOffsets[frame - 176];
			oy = BattleFx10_ShakeOffsets[frame - 176];
			((struct Cells03001ad0 *)gBgScroll)->unk04 = ox;
			((struct Cells03001ad0 *)gBgScroll)->unk06 = oy + 32;
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
	DrawRectangle			 blit[2];

	slots = (void **)gBattleFxWork;
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
	DrawRectangle			 draw[2];
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

	cursor = (void **)gBattleFxWork;
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
	DrawRectangle			 callbacks[2];
	s32						 i;
	s32						 palette;

	cursor = (void **)gBattleFxWork;
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
	DrawRectangle			 draw[2];
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
	DrawRectangle			 pair[2];
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
	DrawRectangle			 draw[2];
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
	DrawRectangle			 draw[2];
	s32						 variant;
	struct EffectPosition	 screen;
	s32						 frame;
	s32						 i;

	cursor = (void **)gBattleFxWork;
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
