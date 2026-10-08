//--------------------------------------------------------------------------
// BOLTS.C
// バトル効果：まわりから寄ってくる四本の稲妻と、飛び散る火花
//--------------------------------------------------------------------------

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "CANVAS.H"
#include "MEM.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "BTLDISP.H"
#include "HEAP_STATE.H"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

void AudioCommand_PlayFar(s32 cue);

#define SPARK_COUNT 1024
#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)

#define CLIP (GOUSEI_CLIP_X | GOUSEI_CLIP_Y)

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
		struct EffectStep *spark = &SPARKS[i];
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
					struct EffectStep *spark = &SPARKS[i];

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
			struct EffectStep *spark = &SPARKS[i];

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
