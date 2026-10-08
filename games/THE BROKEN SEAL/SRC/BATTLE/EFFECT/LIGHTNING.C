//--------------------------------------------------------------------------
// LIGHTNING.C
// 戦闘エフェクト：稲妻の柱と飛び散る火花
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
#include "RESOURCE.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"
#include "IO_REG.H"
#include "BTLDISP.H"
#include "MENU_LIST.H"
#include "HEAP_STATE.H"
#include "SOUND_IDS.H"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

typedef s32 (*WordCopy)(void *, const void *, s32);

extern u8 gMapCellBuffer[];

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
		/* FAKEMATCH: needed to match: calling through a local copy of the routine's address loads that address before the palette address is shifted into place */
		WordCopy copy = Iwram_CopyWords;

		copy((void *)BG_PLTT, palette, 128);
	}

	// 火花をすべて止める
	var = &((struct EffectStep *)gMapCellBuffer)->variant;
	for (k = 0; k != SPARK_MAX; k++)
	{
		*var = 0;
		var += 7;
	}
	sparks = (struct EffectStep *)gMapCellBuffer;
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
