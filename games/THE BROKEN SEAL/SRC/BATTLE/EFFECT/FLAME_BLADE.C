//--------------------------------------------------------------------------
// FLAME_BLADE.C
// バトル演出：剣を振り下ろし、2 か所で閃光と火花を散らす
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
#include "MEM.H"
#include "HEAP_STATE.H"
#include "MOTOBJ.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"
#include "SYSTEM.H"
#include "IO_REG.H"
#include "FIXED_MATH.H"

#define ObjectDispatch_ApplyValueToChildrenFar \
	((void (*)(void *, s32))ObjectDispatch_ApplyValueToChildrenFar)
#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

#define gBattleFxWork ((void **)&gBattleFxWork)

#define HI(v) (((s16 *)&(v))[1]) // 16.16 固定小数の整数部

#define SPARK ((struct EffectStep *)Ram_MapCellBuffer) // 火花 (列ごとに 340 個)

#define FLASH_SHEET (64 * 128) // 剣の絵 (64×128) の後ろに閃光の絵を置く

void Object_SetMode(void *object, s32 mode);
void Audio_PlayCue(s32 cue);

//--------------------------------------------------------------------------
// 剣を 40 フレーム描き (10 フレームで振り下ろす)、16 と 24 フレーム目に
// 2 つの列で閃光を出し、6 フレーム後から火花を散らす
// 列の x は FlameBlade_StrikeColumns (side ごとに 3 つ、使うのは最初の 2 つ)
//--------------------------------------------------------------------------
void BattleFx_RunFlameBlade(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 col;
	u8						*sheet;
	struct MotionObject		*user;
	DrawRectangle			 blit[2];
	s32						 frame;
	s32						 i;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];

	user = GetBattleObjectSlotFar(effect->actor)->object;
	effect->variant = 1;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BLDALPHA = 0x1010;
	BattleFx_FetchRectangleBlitters(work->effect->side, blit);
	Object_SetMode(user, 2);
	ObjectDispatch_ApplyValueToChildrenFar(user, 48);

	Resource_LoadAndDecompress((s32)&ResourceId_FlameBladeSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet + FLASH_SHEET, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

	//------------------------------------------------------------------
	// 列ごとに閃光 16 個と火花 340 個を用意する
	//------------------------------------------------------------------
	col = 0;
	do
	{
		// 閃光：中心から i*2 ドット、寿命 25〜32
		i = 0;
		do
		{
			struct EffectStep *flash = &work->particles[col * 16 + i];

			s32 radius = i * 2;
			s32 angle = Random16() & 0xffff;

			flash->x = Trig_Sin(angle) * radius;
			flash->y = -(Trig_Cos(angle) * radius);
			flash->variant = i / 2 + 25;
			i++;
		} while (i != 16);

		// 火花：列の y = 88 から乱数の向きと速さで飛ぶ、寿命 32〜39
		i = 0;
		do
		{
			struct EffectStep *spark = &SPARK[col * 340 + i];

			s32 speed;
			s32 angle;

			speed = (Random16() & 0x1ff) + 32;
			angle = Random16() & 0xffff;
			spark->x = FlameBlade_StrikeColumns[col + work->effect->side * 3] << 16;
			spark->y = 88 << 16;
			spark->velocity_x = (Trig_Sin(angle) * speed) >> 6;
			spark->velocity_y = -(Trig_Cos(angle) * speed * 2) >> 6;
			spark->variant = (Random16() & 7) + 32;
			i++;
		} while (i != 340);
		col++;
	} while (col != 3);

	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	frame = 0;
	do
	{
		if (frame == 4)
		{
			Audio_PlayCue(212);
		}
		if (frame == 8)
		{
			work->shake_frames = frame;
		}
		if (frame == 18)
		{
			Audio_PlayCue(145);
		}
		if (frame == 40)
		{
			BattleEventRuntime_BeginPhaseFar(134);
		}

		//------------------------------------------------------------------
		// 剣：10 フレームで振り下ろし、20 フレームまで止まり、その後ゆっくり進む
		// y = 104 より下は描かない
		//------------------------------------------------------------------
		if (frame <= 39)
		{
			s32 height;
			s32 x;
			s32 top;

			height = 128;
			if (work->effect->side == 1)
			{
				if (frame <= 9)
				{
					x = frame * 10 - 8;
					top = frame * 16 - 128;
				}
				else if (frame > 20)
				{
					x = frame + 62;
					top = frame * 2 - 24;
				}
				else
				{
					x = 82;
					top = 16;
				}
			}
			else
			{
				if (frame <= 9)
				{
					x = 128 - frame * 10;
					top = frame * 16 - 128;
				}
				else if (frame > 20)
				{
					x = 58 - frame;
					top = frame * 2 - 24;
				}
				else
				{
					x = 38;
					top = 16;
				}
			}
			if (top + 128 > 104)
			{
				height = height - top - 24;
			}
			if (height > 0)
			{
				blit[0](canvas, work, x - 32, top, 64, height);
			}
		}

		if (frame > 16)
		{
			BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
		}

		//------------------------------------------------------------------
		// 2 つの列：start から閃光、start + 6 から火花
		//------------------------------------------------------------------
		col = 0;
		do
		{
			s32 start = col * 8 + 16;

			if (frame == start)
			{
				work->shake_frames = 12;
			}
			if (frame >= start)
			{
				// 最初の 2 フレームは大きな閃光、続いて 12 個の小さな閃光
				if (frame < start + 2)
				{
					blit[0](canvas, work->sheet + FLASH_SHEET,
						FlameBlade_StrikeColumns[col + work->effect->side * 3] - 16, 56, 32, 64);
				}
				i = 0;
				do
				{
					struct EffectStep *flash = &work->particles[col * 16 + i];

					s32 x;
					s32 life;
					s32 y;

					y = HI(flash->y);
					x = HI(flash->x) + FlameBlade_StrikeColumns[col + work->effect->side * 3];
					life = flash->variant;
					if (life >= 0 && life <= 17)
					{
						blit[0](canvas,
							work->sheet + (FlameBlade_FlashCells[life / 3] << 11) + FLASH_SHEET,
							x - 16, y + 56, 32, 64);
					}
					life = flash->variant;
					flash->variant = life > 0 ? life - 1 : -1;
					i++;
				} while (i != 12);
			}

			// 火花：y = 108 より下で跳ね返り (速さ半分)、x 0〜126 の間だけ描く
			if (frame > start + 5)
			{
				i = 0;
				do
				{
					struct EffectStep *spark = &SPARK[col * 340 + i];

					if (spark->variant > 0)
					{
						s32 x;
						s32 y;
						s32 size;

						EffectStep_AdvanceWithGravity2D(spark, 64, 0x1000);
						spark->variant--;
						y = spark->y;
						if (y > (108 << 16))
						{
							spark->velocity_y = -spark->velocity_y / 2;
						}
						else
						{
							x = spark->x;
							if ((x >= 0 && x <= 0x7effff) && y >= 0)
							{
								u8 *cel;
								s32 lane;
								s32 py = y >> 16;
								s32 px = x >> 16;

								// 寿命で大きさを決め、比較と加算を 1 個おきに使う
								size = spark->variant / 5 + 1;
								lane = i & 1;
								cel = sheet + ParticleStreams_CellOffsets[size - 1];
								px -= size / 2;
								py -= size;
								blit[lane](canvas, cel, px, py, size, size * 2);
							}
						}
					}
					i++;
				} while (i != 256);
			}

			for (i = 0; i != work->effect->count; i++)
			{
				if (frame == start + 6)
				{
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 10);
					BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
				}
			}
			col++;
		} while (col != 2);

		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	} while (frame != 80);

	ObjectDispatch_ApplyValueToChildrenFar(user, 16);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}
