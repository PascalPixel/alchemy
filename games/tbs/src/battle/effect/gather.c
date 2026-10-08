//--------------------------------------------------------------------------
// gather.c
// バトル演出：対象から出た光の粒を使い手へ吸い寄せる
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
#include "motobj.h"
#include "mem.h"
#include "resource.h"
#include "btldisp.h"
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
#include "heap_state.h"

extern void *gWorkSlot[];

void AudioCommand_PlayFar(s32 cue);

#define MOTES_PER_MEMBER 128 // 対象ひとりあたりの粒の数

#define MOTE(n) (&((struct EffectStep *)Ram_MapCellBuffer)[n])

//--------------------------------------------------------------------------
// 対象ごとに 20 フレームずつずらして粒を出し、使い手の体の中心へ集める
// effect : variant が 0 なら IceBlock、それ以外は Blast のパレット
//--------------------------------------------------------------------------
void BattleFx_RunGatheringMotes(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 draw;
	u8						*sheet;
	struct BattleCamera		*camera;
	struct EffectPosition	 screen;
	s32						 alternate;
	s32						 member;
	s32						 frame;
	s32						 i;

	heap_cache = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * 4);
	if (effect->variant == 0)
		alternate = 0;
	else
		alternate = 1;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Iwram_CopyWords((void *)BG_PLTT,
		Resource_GetTableEntry(
			alternate == 0 ? (s32)&ResourceId_IceBlockSheet : (s32)&ResourceId_BlastSheet),
		128);

	// 対象の体の中心から、各軸 ±2 ドット/フレームの乱数速度で飛び出す
	for (i = 0; i != 8 * MOTES_PER_MEMBER; i++)
		MOTE(i)->variant = -1;

	for (member = 0; member != work->effect->count; member++)
	{
		struct MotionObject *obj;
		s32					 half;

		obj = GetBattleObjectSlotFar(work->effect->actors[member])->object;
		half = Battle_GetObjectTableValueFar(work->effect->actors[member]) / 2;
		for (i = 0; i != MOTES_PER_MEMBER; i++)
		{
			struct EffectStep *mote = MOTE(member * MOTES_PER_MEMBER + i);

			mote->x = obj->x;
			mote->y = obj->y + half;
			mote->z = obj->z;
			mote->velocity_x = ((Random16() & 0xff) - 128) << 10;
			mote->velocity_y = ((Random16() & 0xff) - 128) << 10;
			mote->velocity_z = ((Random16() & 0xff) - 128) << 10;
			mote->variant = 0;
		}
	}

	// 比較合成で描き、転送のたびにキャンバスの画素値を 4 ずつ下げて残像にする
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	work->transfer_mode = 3;
	work->transfer_value = 0x04040404;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(142);

	for (frame = 0; frame != work->effect->count * 20 + 72; frame++)
	{
		struct MotionObject *user;
		s32					 half;

		user = GetBattleObjectSlotFar(work->effect->actor)->object;
		half = Battle_GetObjectTableValueFar(work->effect->actor) / 2;
		if (frame == 64)
			BattleEventRuntime_BeginPhaseFar(133);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame == 40)
			ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
		if (frame == work->effect->count * 20 + 52)
			ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
		for (member = 0; member != work->effect->count; member++)
		{
			if (frame == member * 20)
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, 42);
			if (frame > member * 20)
			{
				for (i = 0; i != 32; i++)
				{
					struct EffectStep *mote = MOTE(member * MOTES_PER_MEMBER + i);

					if (mote->variant >= 0)
					{
						s32 size;

						// キャンバスは横 2 倍で表示するので x を半分にする
						EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &screen);
						screen.x >>= 1;
						size = 6;
						draw(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
							screen.x - size / 2, screen.y - size, size, size * 2);
						EffectStep_AdvanceWithGravity3D(mote, 62, 0);

						// 出てから 10 フレーム後 (粒ごとに 1 フレームずつ遅れる) に
						// 距離の 1/256 ずつ使い手へ加速し、水平に 16 ドット以内まで近づいたら消す
						if (frame > member * 20 + i + 10)
						{
							s32 dx;
							s32 dy;
							s32 dz;

							dx = (user->x - mote->x) >> 8;
							dy = (user->y + half - mote->y) >> 8;
							dz = (user->z - mote->z) >> 8;
							mote->velocity_x += dx;
							mote->velocity_y += dy;
							mote->velocity_z += dz;
							if ((dx > -0x1000 && dx < 0x1000) && (dz > -0x1000 && dz < 0x1000))
								mote->variant = -1;
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
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}
