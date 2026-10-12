#include "unit/battle/effect/koukatbl.h"
#include "unit/battle/effect/effect.h"
#include "unit/battle/effect/fxdirect.h"
#include "unit/battle/effect/twopic.h"
#include "unit/battle/effect/canvas.h"
#include "unit/battle/effect/strike.h"
#include "unit/battle/effect/dispatch.h"
#include "unit/battle/effect/scene.h"
#include "unit/battle/effect/burst.h"
#include "unit/battle/effect/step.h"
#include "unit/battle/effect/djinnfx.h"
#include "unit/battle/effect/swirl.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/evtable.h"
#include "unit/object/djinnev.h"
#include "unit/object/psyfx.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/sched.h"
#include "unit/runtime/math.h"
#include "unit/runtime/matrix.h"
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
	DrawRectangle			 draw[2];
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

	heap = (void **)gBattleFxWork;
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
	struct BlitterPair		 draw;
	struct BattleCamera		*camera;
	u8						*palette;
	s32						 i;
	s32						 k;

	heap = (void **)gBattleFxWork;
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
	draw.upper = heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
	draw.lower = heap[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
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
							draw.upper(
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
	DrawRectangle			 draw[2];
	s32						 point[3];
	struct EffectPosition	 screen;
	s32						 member;
	s32						 ofs_x;
	s32						 frame;

	heap = (void **)gBattleFxWork;
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
	DrawRectangle			 draw[2];
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 k;
	struct BattleCamera		*camera;
	s32						 pal;
	s32						 i;

	heap = (void **)gBattleFxWork;
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
void BattleFx_FetchRectangleBlitters(s32 flip, DrawRectangle *out)
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
	DrawRectangle			 draw[2];
	void				   **slots;
	s32						 frame;
	s32						 cell;

	cursor = (void **)gBattleFxWork;
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
	DrawRectangle			 draw[2];
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 ofs_x;
	u8						*palette;
	s32						 i;
	s32						 k;

	cursor = (void **)gBattleFxWork;
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
	DrawRectangle			 draw[2];
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
					/* FAKEMATCH: needed to match: restores the CSE block boundary after the hit block (k << 3 is recomputed) */
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

								/* FAKEMATCH: needed to match: restores the CSE block boundary between group's set and group * 8 */
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
	DrawRectangle			 rectangle[2];
	struct MotionObject		*member_object;
	/* FAKEMATCH: needed to match: the second position is never used: it gives the frame its native 76 bytes 2026-10-11: a single position shrinks only the frame (differ 4). */
	struct EffectPosition pos[2];
	s32					  curve[2];
	s32					  draw_enabled;
	s32					  idx_a;
	s32					  idx_b;
	s32					  zero_val;
	s32					  frame;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	/* FAKEMATCH: needed to match: the zero is held in sl through the setup calls */
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
				/* FAKEMATCH: needed to match: i is 160 here, so this is 0: a literal 0 drops a stack slot and shrinks the frame */
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
	DrawRectangle			 draw[2];
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
	DrawRectangle			 draw[2];
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
	DrawRectangle			 draw[2];
	s32						 i;
	u8						*graphics;
	struct BattleCamera		*camera;
	struct MotionObject		*caster;
	struct MotionObject		*source;
	struct MotionObject		*target;
	struct EffectStep		*point;
	struct EffectStep		*step;
	s32						 start;
	s32						 peak;
	s32						 size;
	s32						 j;
	s32						 k;
	struct EffectStep		*trails;

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
	DrawRectangle			 draw[2];
	s32						 i;
	u8						*graphics;
	struct BattleCamera		*camera;
	struct MotionObject		*source;
	struct MotionObject		*target;
	struct EffectStep		*point;
	struct EffectStep		*step;
	struct EffectPosition	 pos;
	s32						 size;
	s32						 j;
	s32						 k;
	struct EffectStep		*trails;

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
	struct BlitterPair		 draw;
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

	cells = (void **)(gBattleFxWork + 4);
	draw_destination = cells[0];
	view = *(void **)((u8 *)cells - 112);
	runtime = cells[-1];
	graphics = cells[1];
	runtime->effect = argument;

	BattleFx_BeginCanvasLayer(0);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw.upper = (DrawRectangle)cells[6];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
	draw.lower = (DrawRectangle)cells[7];

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
			draw.upper(draw_destination,
				&runtime->sheet[frame / 4 % 3 * 3072 + (width - size) * 48], 32, 112 - size, width,
				size);
		}

		if ((u32)(frame - 48) <= 111)
		{
			size = frame * 2 - 96;
			if (size > 64)
				size = 64;
			draw.upper(draw_destination, &runtime->sheet[frame / 4 % 3 * 3072 + (64 - size) * 48],
				32, 64 - size, width, size);
		}

		if ((u32)(frame - 160) <= 239)
		{
			src = (u8 *)runtime + frame / 4 % 3 * 3072;
			draw.upper(draw_destination, src, 8, 0, width, 64);
			draw.upper(draw_destination, src, 8, 64, width, 64);
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
						draw.upper(draw_destination, (u8 *)runtime + 0x2400, screen.x - 12,
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
							draw.lower(draw_destination,
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
						draw.lower(draw_destination,
							graphics + ParticleStreams_CellOffsets[span - 1],
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
								draw.upper(draw_destination,
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
						draw.lower(draw_destination,
							(u8 *)runtime + WhirlwindComets_CellOffsets[size], mark->x,
							mark->y - WhirlwindComets_CellBiasY[size],
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
								draw.upper(draw_destination,
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
	struct BlitterPair		 draw;
	s32						 frame;
	u8						*sheet;
	struct Point2D			 focus;
	s32						 dir;
	u8						*palette;
	s32						 i;

	// cursor は gWorkSlot[HEAP_SLOT_BATTLE_CANVAS] (キャンバス) を指す
	cursor = (void **)(gBattleFxWork + 4);
	canvas = cursor[0];
	camera = cursor[-28]; // [12] カメラ
	work = cursor[-1];	  // [39] 作業領域
	sheet = cursor[1];	  // [41] 粒の絵
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	REG_BG2CNT = 0x784;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw.upper = cursor[6];
	BattleEffect_LoadWork(
		HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
	draw.lower = cursor[7];

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
					draw.upper(canvas, work->sheet + ((flash->variant >> 2) << 11),
						HI(flash->x) - 16, HI(flash->y) - 32, 32, 64);
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
				draw.lower(canvas, PaletteParticles_SparkPixels, HI(spark->x), HI(spark->y) - size,
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

					draw.upper(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
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

						draw.upper(canvas,
							work->sheet + PaletteParticles_StreakOffsets[cel] + 0x3000,
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
					draw.upper(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
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
	DrawRectangle			 draw[2];
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
	DrawRectangle			 draw[2];
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
	DrawRectangle			 draw[2];
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
	DrawRectangle			 draw[2];
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
	DrawRectangle			 draw[2];
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
	DrawRectangle			 draw[2];
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
	DrawRectangle			 draw[2];
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
	DrawRectangle			 draw[2];
	void					*palette;
	s32						*row;
	s32						 i;

	cursor = (void **)gBattleFxWork;
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
	heap_cache = (void **)gBattleFxWork;
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
	/* FAKEMATCH: needed to match: the family's blitter pair: this effect loads only one */
	DrawRectangle		  draw[2];
	struct EffectStep	 *marker;
	s32					  i;
	s32					  frame;
	s32					  start;
	s32					  cel;
	s32					  y;
	struct EffectPosition pos;

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
	DrawRectangle			 callbacks[2];

	heap_cache = (void **)gBattleFxWork;
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
							/* FAKEMATCH: needed to match: the size series A draws its sparks at, kept from that effect and never used here */
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
