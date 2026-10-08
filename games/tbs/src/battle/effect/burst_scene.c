//--------------------------------------------------------------------------
// burst_scene.c
// バトル演出：対象に重ねて弾ける連撃と、せり上がる岩の壁
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
#include "mem.h"
#include "heap_state.h"
#include "animspr.h"
#include "canvas.h"
#include "types.h"
#include "resource_ids.h"
#include "resource.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "btldisp.h"
#include "effect_step.h"
#include "callback_scheduler.h"
#include "system.h"
#include "b5_context.h"
#include "motobj.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "transform.h"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

void BattleFx_PrepareCanvasEffect(
	struct BattleEffectArgument *effect, s32 kind, s32 side, s32 anchor, s32 *out_x, s32 *out_y);
void BattleMotion_ApproachTargetFar(s32 actor, s32 target, s32 frames, s32 speed);
void AudioCommand_PlayFar(s32 cue);

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
	DrawRectangle			 draw[2];
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
		struct EffectStep *mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];

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
		draw[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		draw[1] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
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
					((struct EffectStep *)Ram_MapCellBuffer)[i * 32 + j].variant =
						(Random16() & 7) + 15;
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
		draw[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		draw[1] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
		for (i = 0; i != 512; i++)
		{
			struct EffectStep *mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];

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

struct WallScale
{
	s32 x;
	s32 y;
};

void Object_ApplyProjectedPlacementFar(
	struct AnimationObject *object, s32 *position, struct WallScale *scale, s32 mode);
void AudioCommand_PlayFar(s32 cue);

extern const struct WallScale RisingWall_UnitScale;

#define HI(v) (((s16 *)&(v))[1]) // 16.16 固定小数の整数部

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
	DrawRectangle			 draw[2];
	u8						*sprites;
	s32						 wall_x;
	s32						 rock_x;
	struct WallScale		 scale;
	s32						 pos[4];
	u8						*src;
	u16						*col;
	void					*pltt = (void *)BG_PLTT;
	s32						 n;
	s32						 frame;
	s32						 i;
	s32						 j;
	s32						 k;

	heap = (void **)gBattleFxWork;
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
	draw[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
	draw[1] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
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
		struct EffectStep *dust = &((struct EffectStep *)Ram_MapCellBuffer)[i];

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
				struct EffectStep *dust = &((struct EffectStep *)Ram_MapCellBuffer)[i];

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
