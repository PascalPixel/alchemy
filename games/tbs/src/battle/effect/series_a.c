//--------------------------------------------------------------------------
// series_a.c
// バトル演出：対象ごとに光の粒を回しながら集める (七種)
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
#include "resource.h"
#include "btldisp.h"
#include "types.h"
#include "iwram_call.h"
#include "call.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "callback_scheduler.h"
#include "system.h"
#include "fixed_math.h"
#include "resource_ids.h"
#include "effect_step.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "transform.h"
#include "sound_ids.h"
#include "heap_state.h"

#define SceneTransform_ApplyPosition ((void (*)(s32 *))SceneTransform_ApplyPosition)

#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer) // 光の粒 (対象ごとに 64)

#define JIN_OFS 0x1000 // sheet の中のジンの絵

void BattleFx_PrepareCanvasEffect(
	struct BattleEffectArgument *effect, s32 kind, s32 side, s32 narrow, s32 *x, s32 *y);
void AudioCommand_PlayFar(s32 cue);

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
	/* FAKEMATCH: needed to match: forwarded through Call3, the palette copy loads its routine before it shifts the destination, as the reference does */
	Call3((void (*)())Iwram_CopyWords, (s32)BG_PLTT, (s32)palette, 128);

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
			struct EffectStep *spark = &SPARKS[i];

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
			struct EffectStep *spark = &SPARKS[i];

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
			struct EffectStep *spark = &SPARKS[i];
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
			struct EffectStep *spark = &SPARKS[i];

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
			struct EffectStep *spark = &SPARKS[i];

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
					struct EffectStep *spark = &SPARKS[member * 64 + i];
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
