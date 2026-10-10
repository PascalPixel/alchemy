/* Pin-free BattleFx_RunBladeRain: EN 080cb4ec.
 * All six editions: 320 native/built, 294 common, differ 52.
 * Whole-TU current pinned baseline gives differ 0. Unpin first changes
 * the drift load from r6 to r4, then cell and width registers.
 * RTL: drift pseudo143 has a hard-r4 preference propagated from the
 * indirect painter path; global allocation assigns r4. Native keeps
 * drift in r6 and cell in r4. Five bounded sign-index/lifetime checks
 * failed: unsigned64 promotion and 32-bit mask retain52, widened shift
 * gives99, boolean sign gives86, explicit in-place direction retains52.
 * Earlier ordinary magnitude/cell/one-pass/painter trials are recorded
 * inside the function. No new pin, asm or barrier is proposed. */
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
#include "canvas.h"
#include "types.h"
#include "scene.h"
#include "resource_ids.h"
#include "resource.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "btldisp.h"
#include "effect_step.h"
#include "callback_scheduler.h"
#include "fixed_math.h"
#include "b5_context.h"
#include "motobj.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "menu_list.h"
#include "transform.h"
#include "btlwork.h"
#include "io_write_queue.h"
#include "map_scroll.h"
#include "global_cells.h"
#include "project.h"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

#define gBlitters ((DrawRectangle *)gWorkSlot)
#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)
#define gSparks ((struct EffectStep *)(Ram_MapCellBuffer + 0x3800))
#define HI(v) (((s16 *)&(v))[1])

void BattleFx_RunBladeRain(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 pos;
	DrawRectangle			 draw[2];
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			*blit;
	struct EffectStep		*blade;
	s32						 frame;
	s32						 i;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 等倍
	REG_BLDALPHA = 0x1000;

	// [0] そのまま、[1] 左右反転 (どちらも 0 以外の画素だけ描く)
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	draw[0] = (DrawRectangle)heap_cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_NURI);
	draw[1] = (DrawRectangle)heap_cache[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	blit = draw;
	Resource_LoadAndDecompress((s32)&ResourceId_SwordSlashSheet, work->sheet, 1, 1);
	work->transfer_mode = 1; // 転送のたびにキャンバスを 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &pos);
	REG_BG2X = (64 - pos.x) << 8;

	// 剣：x 16〜111、中央から遠いほど横の速さ (2 ドット単位) を大きくする
	for (i = 0; i != 64; i++)
	{
		s32 column;

		blade = &work->particles[i];
		column = Random16() % 96 + 16;
		blade->x = column;
		blade->y = (24 - i / 4) << 16;
		if (column <= 43)
			blade->velocity_x = 3;
		else if (column <= 51)
			blade->velocity_x = 2;
		else if (column <= 59)
			blade->velocity_x = 1;
		else if (column <= 67)
			blade->velocity_x = 0;
		else if (column <= 75)
			blade->velocity_x = -1;
		else if (column <= 83)
			blade->velocity_x = -2;
		else
			blade->velocity_x = -3;
		blade->velocity_x <<= 17;
		blade->velocity_y = 8 << 16;
		blade->x <<= 16;
	}
	AudioCommand_PlayFar(212);
	for (frame = 0; frame != 120; frame++)
	{
		if (frame <= 16)
		{
			REG_BLDALPHA = frame | 0x1000;
			if (frame == 16)
				REG_BLDCNT = 0;
		}
		if (frame > 103)
		{
			REG_BLDALPHA = (120 - frame) | 0x1000;
			if (frame == 104)
				REG_BLDCNT = 0x3f44;
		}

		// 絵は横の速さで選び、左へ進む剣は左右反転して描く
		for (i = 15; i != -1; i--)
		{
			/* Ordinary drift differs 52 in EN (320 native/built). A generic
			 * output, conditional magnitude, direct cell calculation and
			 * one-pass load/magnitude/cell blocks retain 52. Keeping drift
			 * live after drawing differs 291 (331 built); a separate early
			 * direction differs 66 (318 built). A direction in each branch
			 * retains 52; selecting its painter first differs 189 (325 built). */
			s32 drift;
			s32			 speed;
			s32			 cell;
			u32			 wide;
			u32			 high;

			blade = &work->particles[i];
			drift = blade->velocity_x;
			speed = drift;
			if (drift < 0)
				speed = -drift;
			cell = speed >> 17;
			if (frame < i * 4 + 25)
			{
				blit[(u32)drift >> 31](canvas, work->sheet + BladeRain_CellSourceOffsets[cell],
					(blade->x >> 16) - ((wide = BladeRain_CellWidths[cell]) >> 1),
					(blade->y >> 16) - ((high = BladeRain_CellHeights[cell]) >> 1), wide, high);
				if (frame >= i * 4 + 16)
				{
					blade->x += blade->velocity_x;
					blade->y += blade->velocity_y;
				}
			}
			else
			{
				blit[(u32)drift >> 31](canvas, work->sheet + BladeRain_CellSourceOffsets[cell],
					(blade->x >> 16) - ((wide = BladeRain_CellWidths[cell]) >> 1),
					(blade->y >> 16) - ((high = BladeRain_CellHeights[cell]) >> 1), wide,
					high -= 4);
			}
		}

		// 24〜84 フレームの 4 フレームごとに対象のパレットを 2 フレーム 7 番に替えて
		// アニメ 5 にし、画面を揺らし、8 フレームごとに音を鳴らす
		if (frame >= 23 && frame <= 87 && (frame & 3) == 0)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 2);
			work->shake_frames = 1;
			if ((frame & 7) == 0)
				AudioCommand_PlayFar(133);
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}
