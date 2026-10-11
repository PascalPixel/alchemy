//--------------------------------------------------------------------------
// canvas_draw_clipped_line.c
// バトル演出：キャンバスへ線を引く（y をキャンバス内に収める）
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
#include "canvas.h"
#include "mem.h"
#include "iwram_call.h"
#include "battle_efx.h"
#include "callback_scheduler.h"
#include "fixed_math.h"
#include "resource_ids.h"
#include "scene.h"
#include "resource.h"
#include "battle_effect_work.h"
#include "btldisp.h"
#include "effect_step.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "menu_list.h"
#include "field_event.h"
#include "transform.h"
#include "heap_state.h"
#include "io_reg.h"
#include "b5_context.h"

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

// memberfx.c

// 戦闘エフェクト：炎になって落ちる跳躍、集まるどくろ、回る渦、散って集まる光

//--------------------------------------------------------------------------

void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)
#define Object_SetMoveTargetFar \
	((void (*)(struct MotionObject *, s32, s32, s32))Object_SetMoveTargetFar)
#define SceneTransform_ApplyPosition ((void (*)(s32 *))SceneTransform_ApplyPosition)
#define Battle_GetObjectTableValueFar ((u32 (*)(s32))Battle_GetObjectTableValueFar)

//--------------------------------------------------------------------------
// 炎になって落ちる跳躍
//--------------------------------------------------------------------------
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

void ObjectDispatch_InitFromTable6Far(struct MotionObject *object);

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
#define gSkulls ((struct EffectStep *)Ram_MapCellBuffer) // 対象ひとりに 64 個

typedef s32 (*WordCopy)(void *, const void *, s32);

struct BlitterPair
{
	DrawRectangle upper;
	DrawRectangle lower;
};

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
		gSkulls[i].x = ((Random16() & 255) - 127) << 15;
		gSkulls[i].y = ((Random16() & 255) - 127) << 15;
		gSkulls[i].z = ((Random16() & 255) - 127) << 15;
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
					struct EffectStep *skull = &gSkulls[k * 64 + i];

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

#define gMotes ((struct EffectStep *)Ram_MapCellBuffer) // 対象ひとりに 128 個

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
		gMotes[i].variant = -1;
	for (k = 0; k != work->effect->count; k++)
	{
		struct MotionObject *src = GetBattleObjectSlotFar(work->effect->actor)->object;
		s32					 height = Battle_GetObjectTableValueFar(work->effect->actor);

		for (i = 0; i != 128; i++)
		{
			struct EffectStep *mote = &gMotes[k * 128 + i];

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
			s32 half = (s32)Battle_GetObjectTableValueFar(work->effect->actors[k]) / 2;

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
					struct EffectStep *mote = &gMotes[k * 128 + i];

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
void BattleFx_FetchRectangleBlitters(s32 flip, BattleEffectDrawRectangle *out)
{
	if (flip == 0)
	{
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
		out[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
		out[1] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	}
	else
	{
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
		out[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
			GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_KASAN);
		out[1] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
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
	DrawRectangle			 draw_r;
	DrawRectangle			 draw_l;
	u8						*state;
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
	state = (u8 *)gWorkSlot;
	draw_l = *(DrawRectangle *)(state + HEAP_SLOT_BLITTER * 4);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
	work->transfer_mode = 2;
	/* FAKEMATCH: needed to match: reading the second blitter through the slot pointer's own variable keeps it in that register between the two transfer stores */
	state += 188; // gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE]
	state = *(u8 **)state;
	work->transfer_value = 50; // 転送のたびに半分へ減らして残像にする
	draw_r = (DrawRectangle)state;
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
				draw_l(canvas, work->sheet + EarthWall_CellSourceOffsets[cell],
					64 - EarthWall_CellWidths[cell], (pos.y - EarthWall_CellHeights[cell]) + 8,
					EarthWall_CellWidths[cell], EarthWall_CellHeights[cell]);
				draw_r(canvas, work->sheet + EarthWall_CellSourceOffsets[cell], 64,
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

void BattleFx_PrepareCanvasEffect(
	struct BattleEffectArgument *effect, s32 kind, s32 side, s32 anchor, s32 *out_x, s32 *out_y);

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
		gMotes[i].x = 0;
		gMotes[i].y = 80 << 16;
		gMotes[i].z = (Random16() | -32) << 14;
		gMotes[i].variant = Random16() & 255;
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
					struct EffectStep *mote = &gMotes[k * 64 + i];

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
