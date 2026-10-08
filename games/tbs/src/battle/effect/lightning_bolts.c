//--------------------------------------------------------------------------
// lightning_bolts.c
// 戦闘エフェクト：対象ごとに落ちる稲妻と、火花・光
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
#include "resource_ids.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "callback_scheduler.h"
#include "fixed_math.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "btldisp.h"
#include "heap_state.h"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

void AudioCommand_PlayFar(s32 cue);

#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer) // 対象ひとりに 128 個
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
	DrawRectangle			 draw[2];
	u8						*sprites;
	struct EffectPosition	 pos;
	s32						 j;

	heap = (void **)gBattleFxWork;
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
		SPARKS[j].variant = 0;
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
						struct EffectStep *spark = &SPARKS[i * 128 + j];
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
						LightningBolts_GlintPalettes[Random16() & 3]
							| GOUSEI_CLIP_X | GOUSEI_CLIP_Y,
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
			struct EffectStep *spark = &SPARKS[j];

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
