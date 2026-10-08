//--------------------------------------------------------------------------
// DJINN_BURSTS.C
// バトル演出：水星と木星のジンが回りながら弾を放つ
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
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "BTLDISP.H"
#include "HEAP_STATE.H"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

void Audio_PlayCue(s32 cue);

#define HI(v) (((s16 *)&(v))[1])							   // 16.16 固定小数の整数部
#define MOTE(n) (&((struct EffectStep *)Ram_MapCellBuffer)[n]) // 破片の粒 (最大 512)

//--------------------------------------------------------------------------
// 水星のジン：(96, 60) のまわりを渦を巻いて回りながら、青い炎を 5 発左へ撃つ
// 炎は 5 フレーム進むと 32 個の粒に砕ける
//--------------------------------------------------------------------------
void BattleFx_RunMercuryDjinnFlames(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	u8						*sheet;
	DrawRectangle			 blit[2];
	DrawRectangle			*draw;
	struct EffectStep		*flame;
	struct EffectStep		*mote;
	u8						*cells;
	s32						 i;
	s32						 j;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	draw = blit;
	BattleFx_FetchRectangleBlitters(0, draw);

	// 炎の絵は BlueFlame を転置して写す
	cells = Ram_MapCellBuffer;
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, cells, 1, 1);
	Graphics_TransposeCopy(cells, work->sheet + 0x2a8, 17, 104);
	Graphics_TransposeCopy(cells + 0x6e8, work->sheet + 0x990, 34, 65);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	for (i = 0; i != 8; i++)
		work->particles[i].variant = -1;
	for (i = 0; i != 512; i++)
		MOTE(i)->variant = -1;
	Audio_PlayCue(162);
	frame = 0;
	do
	{
		if (frame == 56)
			BattleEventRuntime_BeginPhaseFar(133);

		// 炎：1 フレームに 12 ドット左へ進み、5 フレーム目で砕ける
		for (i = 0; i != 5; i++)
		{
			flame = &work->particles[i];
			if (flame->variant != -1)
			{
				blit[0](canvas, work->sheet + 0x990, flame->x - 16, flame->y - 17, 65, 34);
				flame->x -= 12;
				flame->variant++;
				if (flame->variant == 5)
				{
					Audio_PlayCue(133);
					work->shake_frames = 4;
					for (j = 0; j != 32; j++)
					{
						s32 dir;
						s32 speed;

						mote = MOTE(i * 32 + j);
						dir = Random16() & 0xffff;
						speed = (Random16() & 0x1ff) + 256;
						mote->x = flame->x << 16;
						mote->y = flame->y << 16;
						mote->velocity_x = (Trig_Sin(dir) * speed) >> 8;
						mote->velocity_y = (Trig_Cos(dir) * speed) >> 7;
						mote->variant = (Random16() & 15) + 32; // 寿命 32〜47 フレーム
					}
				}
			}
		}

		// ジン：32 フレームで 1 周する。半径 64 - 2 × frame (横はその半分) は
		// 32 フレームで 0 になって中心を通り、その後は反対側へ広がる
		if (frame <= 95)
		{
			s32 x = ((Trig_Sin(frame << 11) * (64 - frame * 2)) >> 17) + 96;
			s32 y = ((Trig_Cos(frame << 11) * (64 - frame * 2)) >> 16) + 60;

			draw[1](canvas, work, x - 10, y - 17, 20, 34);
			for (i = 0; i != 5; i++)
			{
				// 決まったフレームに炎を撃ち、その 6 フレーム後に対象のパレットを
				// 6 フレーム 7 番に替えてアニメ 5 にし、動き 6 を与える
				if (frame == MercuryDjinnFlames_LaunchFrames[i])
				{
					struct EffectStep *shot = &work->particles[i];

					if (shot->variant == -1)
					{
						shot->x = x - 8;
						shot->velocity_x = x;
						shot->y = y;
						shot->variant = 0;
						break;
					}
				}
				if (frame == MercuryDjinnFlames_LaunchFrames[i] + 6)
				{
					for (j = 0; j != work->effect->count; j++)
					{
						ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 6);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
					}
				}
			}
		}

		// 粒：寿命に応じて小さくなり、落ちながら減速する
		for (i = 0; i != 256; i++)
		{
			mote = MOTE(i);
			if (mote->variant != -1)
			{
				s32 size = mote->variant / 16 + 2;

				draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
				EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
				mote->variant--;
			}
		}
		Camera_ApplyShake(4, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	} while (frame != 96);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 木星のジン：回りながら、右上から左下へ火の玉を 8 つ落とす
// 火の玉は y が 95 を越えると 32 個の粒に砕ける
//--------------------------------------------------------------------------
void BattleFx_RunJupiterDjinnFall(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	u8						*sheet;
	DrawRectangle			 blit[2];
	DrawRectangle			*draw;
	struct EffectStep		*ball;
	struct EffectStep		*mote;
	s32						 frame;
	s32						 i;
	s32						 j;
	s32						 angle;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	draw = blit;
	BattleFx_FetchRectangleBlitters(0, draw);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet, work->sheet + 0x320, 1, 0);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 火の玉の出だし：x 64〜127、y -80〜-17
	for (i = 0; i != 32; i++)
	{
		ball = &work->particles[i];
		ball->x = (Random16() & 63) + 64;
		ball->y = (Random16() & 63) - 80;
	}
	for (i = 0; i != 512; i++)
		MOTE(i)->variant = -1;
	Audio_PlayCue(171);
	frame = 0;
	angle = 0x8000; // 180°
	do
	{
		if (frame == 56)
			BattleEventRuntime_BeginPhaseFar(133);

		// ジン：1 フレームに 11.25° 回る。半径 64 - 2 × frame (横はその半分) は
		// 32 フレームで 0 になって中心を通り、その後は反対側へ広がる
		if (frame <= 95)
		{
			s32 x = (Trig_Sin(angle) * (64 - frame * 2)) >> 17;
			s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

			blit[0](canvas, work, x + 86, y + 28, 20, 40);
		}

		// 火の玉：4 フレームおきに 1 つずつ、1 フレームに左へ 6、下へ 12 ドット
		for (i = 0; i != 8; i++)
		{
			ball = &work->particles[i];
			if (frame >= i * 4 + 8 && ball->y <= 95)
			{
				blit[0](canvas, work->sheet + 0x320, ball->x - 20, ball->y - 32, 40, 64);
				ball->x -= 6;
				ball->y += 12;
				if (ball->y > 95)
				{
					for (j = 0; j != 32; j++)
					{
						s32 dir;
						s32 speed;

						mote = MOTE(i * 32 + j);
						dir = Random16() & 0xffff;
						speed = (Random16() & 0x1ff) + 256;
						mote->x = ball->x << 16;
						mote->y = ball->y << 16;
						mote->velocity_x = (Trig_Sin(dir) * speed) >> 7;
						mote->velocity_y = (Trig_Cos(dir) * speed) >> 6;
						mote->variant = (Random16() & 15) + 32; // 寿命 32〜47 フレーム
					}
					Audio_PlayCue(133);
					work->shake_frames = 4;
					for (j = 0; j != work->effect->count; j++)
					{
						ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 6);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
					}
				}
			}
		}

		// 粒：寿命に応じて小さくなり、落ちながら減速する
		for (i = 0; i != 512; i++)
		{
			mote = MOTE(i);
			if (mote->variant != -1)
			{
				s32 size = mote->variant / 16 + 1;

				draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
				EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
				mote->variant--;
			}
		}
		Camera_ApplyShake(4, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		angle -= 0x800; // 11.25°
		frame++;
	} while (frame != 96);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}
