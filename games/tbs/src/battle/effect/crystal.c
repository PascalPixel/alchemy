//--------------------------------------------------------------------------
// crystal.c
// バトル演出：降ってきて砕ける結晶
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
#include "iwram_call.h"
#include "resource_ids.h"
#include "battle_efx.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "io_reg.h"
#include "btldisp.h"
#include "menu_list.h"
#include "heap_state.h"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

extern u8 gMapCellBuffer[];

#define HI(v) (((s16 *)&(v))[1]) // 16.16 固定小数の整数部

//--------------------------------------------------------------------------
// 結晶が降ってきて地面で砕ける
// Crystal_Counts は variant ごとに（結晶の数, 全体のフレーム数）
// 結晶 : particles[]（z は砕けたら 1、variant は降り始めるフレーム）
// 破片 : gMapCellBuffer に結晶 1 個あたり 16 個（z は回転の速さ）
//--------------------------------------------------------------------------
void BattleFx_RunFallingCrystals(struct BattleEffectArgument *effect)
{
	DrawRectangle			 draw[2];
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	struct EffectStep		*seed;
	struct EffectStep		*cry;
	struct EffectStep		*shards;
	struct EffectStep		*shard;
	s32						 x;
	s32						 cell;
	s32						 i;
	s32						 j;

	shards = (struct EffectStep *)gMapCellBuffer;
	cursor = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 1);
	BattleFx_FetchRectangleBlitters(work->effect->side, draw);
	work->transfer_mode = 2; // 残像 1/2
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 結晶を画面の上に並べ、8 フレームずつずらして降らせる
	for (j = 0; j != Crystal_Counts[work->effect->variant * 2]; j++)
	{
		seed = &work->particles[j];
		x = Random16() & 31;
		seed->y = -0x400000;
		if (work->effect->side == 1)
		{
			x = ((Random16() & 31) + 80) << 16;
			seed->velocity_x = (Random16() & 63) << 12;
		}
		else
		{
			x = ((Random16() & 31) + 8) << 16;
			seed->velocity_x = -(Random16() & 63) << 12;
		}
		seed->x = x - seed->velocity_x * 18;
		seed->velocity_y = 0;
		seed->z = 0;
		seed->variant = j * 8;
	}

	for (frame = 0; frame != Crystal_Counts[work->effect->variant * 2 + 1]; frame++)
	{
		if (work->effect->variant == 2 && frame <= 103)
		{
			// カメラを横に回す（96 フレーム目から遅くする）
			struct BattleCamera *camera = gCameraWork;
			s32					 speed;

			if (frame <= 95)
				speed = 192;
			else
				speed = 2496 - frame * 24;
			if (work->effect->side == 0)
				camera->yaw -= speed;
			else
				camera->yaw += speed;
		}
		if (frame == Crystal_Counts[work->effect->variant * 2 + 1] - 80)
			BattleEventRuntime_BeginPhaseFar(134);
		if (frame == Crystal_Counts[work->effect->variant * 2 + 1] - 8)
		{
			// 最後の 8 フレームは暗くしていく
			work->transfer_mode = 3;
			work->transfer_value = 0x06060606;
		}
		if (frame <= Crystal_Counts[work->effect->variant * 2 + 1] - 8)
		{
			for (i = 0; i != Crystal_Counts[work->effect->variant * 2]; i++)
			{
				cry = &work->particles[i];
				if (cry->z == 1)
				{
					// 砕けた結晶：破片を回しながら飛ばす
					for (j = 0; j != 16; j++)
					{
						cell = (j % 5) * 3 + (shards[i * 16 + j].variant / 96) % 3;
						shard = &shards[i * 16 + j];
						draw[j <= 2](canvas, work->sheet + Crystal_ShardOffsets[cell] + 0x800,
							HI(shard->x) - Crystal_ShardWidths[cell] / 2,
							HI(shard->y) - Crystal_ShardHeights[cell] / 2,
							Crystal_ShardWidths[cell], Crystal_ShardHeights[cell]);
						EffectStep_AdvanceWithGravity2D(shard, 64, 0x2000);
						shard->variant += shard->z;
						if (shard->z > 1 && (frame & 1))
							shard->z--;
					}
				}
				else if (frame >= cry->variant)
				{
					// 落ちている結晶（32x64）
					draw[i & 1](canvas, work, HI(cry->x) - 16, HI(cry->y), 32, 64);
					EffectStep_AdvanceWithGravity2D(cry, 64, 0x10000);
					if (cry->y > 0x380000)
					{
						// 地面（y = 56）に着いたら砕く
						cry->z = 1;
						cry->y = 0x380000;
						for (j = 0; j != 16; j++)
						{
							shard = &shards[i * 16 + j];
							shard->x = ((Crystal_ShardStarts[j * 2] - 40) << 16) + cry->x;
							shard->y = Crystal_ShardStarts[j * 2 + 1] << 16;
							shard->velocity_x = ((Random16() & 127) - 64) << 11;
							shard->velocity_y = -(Random16() & 127) << 11;
							if (i & 1)
							{
								shard->velocity_x *= 2;
								shard->velocity_y *= 2;
							}
							shard->z = 32;
							shard->variant = 0;
						}
						work->shake_frames = 8;
						AudioCommand_PlayFar(144);
						for (j = 0; j != work->effect->count; j++)
							ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 4);
					}
				}
			}
		}
		Camera_ApplyShake(work->effect->variant * 2 + 4, work->effect->variant * 4 + 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}
