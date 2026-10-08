//--------------------------------------------------------------------------
// run_five_mode_mode0.c
// バトル演出：使う者から対象ごとに 3 本の筋を飛ばして当てる (色の違う五種)
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
#include "canvas.h"
#include "mem.h"
#include "types.h"
#include "scene.h"
#include "resource_ids.h"
#include "resource.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "btldisp.h"
#include "effect_step.h"
#include "callback_scheduler.h"
#include "fixed_math.h"
#include "b5_context.h"
#include "motobj.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "menu_list.h"
#include "transform.h"
#include "heap_state.h"
#include "ram_buffer.h"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

#define CLIP (GOUSEI_CLIP_X | GOUSEI_CLIP_Y) // 上下左右ではみ出しを切る

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

#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)

void BattleFx_RunParticlePoolMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunParticlePool(effect, 1);
}

void BattleFx_RunParticlePoolMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunParticlePool(effect, 0);
}

struct BlitterPair
{
	DrawRectangle upper;
	DrawRectangle lower;
};

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
	draw.upper =
		(BattleEffectDrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 11, 2);
	draw.lower = (BattleEffectDrawRectangle)((union HeapState *)gWorkSlot)
					 ->slots[HEAP_SLOT_BLITTER_ALTERNATE];
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

		mote = &gMotes[i];

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
			mote = &gMotes[i];
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
