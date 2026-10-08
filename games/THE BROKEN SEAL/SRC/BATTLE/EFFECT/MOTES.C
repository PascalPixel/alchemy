//--------------------------------------------------------------------------
// MOTES.C
// 戦闘エフェクト：渦を巻いて昇る粒、立ち並ぶ柱、はじけて散る破片と煙
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
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "BTLDISP.H"
#include "HEAP_STATE.H"

#define SceneTransform_ApplyPosition ((void (*)(s32 *))SceneTransform_ApplyPosition)

void AudioCommand_PlayFar(s32 cue);

#define HI(v) (((s16 *)&(v))[1]) // 16.16 固定小数の整数部

#define CHIPS ((struct EffectStep *)Ram_MapCellBuffer)

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
				struct EffectStep *chip = &CHIPS[i];

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
				struct EffectStep *chip = &CHIPS[i];

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
