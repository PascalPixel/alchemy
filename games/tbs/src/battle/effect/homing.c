//--------------------------------------------------------------------------
// homing.c
// バトル演出：使い手から飛び出した火の粉が対象を追って落ちる
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
#include "motobj.h"
#include "canvas.h"
#include "types.h"
#include "iwram_call.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "callback_scheduler.h"
#include "system.h"
#include "fixed_math.h"
#include "resource_ids.h"
#include "effect_step.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "transform.h"
#include "btldisp.h"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

void AudioCommand_PlayFar(s32 cue);

#define EMBER(n) (&((struct EffectStep *)Ram_MapCellBuffer)[n])

//--------------------------------------------------------------------------
// 火の粉を 1 フレームに 2 個ずつ飛ばし、出てから 48 フレーム後に対象へ向かわせる
// 地面に落ちた火の粉は着弾の絵を 12 フレーム出す
// effect : variant で火の粉の数 (HomingEmbers_Counts) を選ぶ
//--------------------------------------------------------------------------
void BattleFx_RunHomingEmbers(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	struct BattleCamera		*camera;
	u8						*sheet;
	s32						 top;
	struct EffectPosition	 screen;
	DrawRectangle			 draw[2];
	struct MotionObject		*user;
	s32						 i;

	heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * 4);
	sheet = heap_cache[2];
	work->effect = effect;
	if (effect->side == 1)
		BattleFx_BeginCanvasLayer(1);
	else
		BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
	draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(
		HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER_ALTERNATE];
	REG_BLDALPHA = 0x1010;

	// 使い手の体の上端から、水平は乱数の向きと速さ、縦は -16〜111 (1/64 ドット単位) で飛び出す
	user = GetBattleObjectSlotFar(work->effect->actor)->object;
	top = user->y + Battle_GetObjectTableValueFar(work->effect->actor);
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *ember = EMBER(i);
		s32				   angle;
		s32				   speed;

		angle = Random16();
		speed = (Random16() & 127) + 127;
		ember->velocity_x = (Trig_Sin(angle) * speed) >> 6;
		ember->velocity_y = (s32)(((Random16() & 127) - 16) << 16) >> 6;
		ember->velocity_z = (Trig_Cos(angle) * speed) >> 6;
		ember->x = user->x;
		ember->y = top;
		ember->z = user->z;
		ember->variant = -1; // -1 は飛行中、0 以上は着弾の絵の経過フレーム
	}

	work->camera_delta = 0;
	work->camera_phase = 0;
	Scheduler_AddOrUpdateCallback(Camera_ApplyPhasedDelta, 0x480);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != (HomingEmbers_Counts[work->effect->variant] >> 1) + 132; frame++)
	{
		if (frame > 16 && frame < 80)
			work->camera_delta = 0x100;
		else
			work->camera_delta = 0;
		if (frame == (HomingEmbers_Counts[work->effect->variant] >> 1) + 108)
			BattleEventRuntime_BeginPhaseFar(133);
		BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

		//------------------------------------------------------------------
		// 飛んでいる火の粉
		//------------------------------------------------------------------
		for (i = 0; i != HomingEmbers_Counts[work->effect->variant]; i++)
		{
			struct EffectStep *ember = EMBER(i);
			s32				   start;

			start = i / 2;
			if (frame > start && ember->variant == -1)
			{
				s32 size;

				// 奥行き 160〜799 を大きさ 10〜1 に。最初の 48 フレームは比較、後は加算合成
				EffectPosition_ApplyBaseAndYOffset((s32 *)ember, &screen);
				screen.x >>= 1;
				if (screen.depth < 160)
					screen.depth = 160;
				if (screen.depth > 799)
					screen.depth = 799;
				size = 10 - (screen.depth - 160) / 64;
				draw[frame < start + 48 ? 1 : 0](canvas,
					sheet + ParticleStreams_CellOffsets[size - 1], screen.x - size / 2,
					screen.y - size, size, size * 2);
				ember->x += ember->velocity_x;
				ember->y += ember->velocity_y;
				ember->z += ember->velocity_z;
			}

			// 対象へ距離の 1/512 ずつ加速し、出てから 85 フレームまでは速度を 60/64 に抑える
			if (frame > start + 48 && ember->variant == -1)
			{
				struct MotionObject *target;
				s32					 vx;
				s32					 vy;
				s32					 vz;

				target =
					GetBattleObjectSlotFar(work->effect->actors[i % work->effect->count])->object;
				vx = ember->velocity_x += (target->x - ember->x) >> 9;
				vy = ember->velocity_y += (target->y - ember->y) >> 9;
				vz = ember->velocity_z += (target->z - ember->z) >> 9;
				if (frame < start + 85)
				{
					ember->velocity_x = vx * 60 / 64;
					ember->velocity_y = vy * 60 / 64;
					ember->velocity_z = vz * 60 / 64;
				}

				// 地面より下で着弾：画面位置を残して着弾の絵に切り替え、対象のパレットを
				// 4 フレーム 10 番に替えてアニメ 5 にし、画面を 2 フレーム揺らす
				if (ember->y < 0)
				{
					ember->variant = 0;
					ember->x = screen.x;
					ember->y = screen.y;
					AudioCommand_PlayFar(136);
					ObjectGroup_UpdateMembers(work->effect->actors[i % work->effect->count], 10, 5,
						i % work->effect->count, 4);
					work->shake_frames = 2;
				}
			}
		}

		//------------------------------------------------------------------
		// 着弾の絵 (2 フレームごとに進め、6 枚で終わる)
		//------------------------------------------------------------------
		for (i = 0; i != HomingEmbers_Counts[work->effect->variant]; i++)
		{
			struct EffectStep *ember = EMBER(i);

			if (ember->variant >= 0 && ember->variant < 12)
			{
				s32 cel;
				u32 width;

				cel = ember->variant / 2;
				draw[1](canvas, work->sheet + HomingEmbers_FlareCells[cel],
					ember->x - ((width = HomingEmbers_FlareWidths[cel]) >> 1),
					ember->y + HomingEmbers_FlareBiasY[cel] - 56, width,
					HomingEmbers_FlareHeights[cel]);
				ember->variant++;
			}
		}

		if (work->camera_phase == 0)
			work->camera_phase = 1;
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Scheduler_RemoveCallback(Camera_ApplyPhasedDelta);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}
