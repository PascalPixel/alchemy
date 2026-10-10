/* Pin-free BattleFx_RunSpiderWeb trial: all six editions 233 native/built,
 * 218 common, differ 30. A final frame input barrier differs 44. Initializing frame
 * before the cue differs 32; initializing at declaration differs 76.
 * Unsigned frame with signed threshold comparisons retains 30. Setting
 * frame after step/size differs 44; setting size/frame/step differs 50.
 * Using frame for the initial blend address differs 54; using it for the
 * slot address or swapping size/step's address uses differs 40. Initial
 * input/output/high-register boundaries differ 44. Value-preserving narrow
 * shifts between frame, size and step retain 30. A two-minute seed-439
 * search compiled 5455 candidates without beating this draft; its longer
 * prefix candidate differs 115 and is not retained. New source-only batches
 * of 14 cancelling/control-block alternatives, all 24 local declaration
 * orders and 10 early dead frame definitions did not beat the 30-row
 * English residual. Four new pass-directed forms did not beat30:
 * frame += step and frame += size >> 5 retain30; a narrow step cast
 * gives50; a per-iteration current-frame carrier retains30. Global
 * allocation places size35 in r8, frame37 in r10 and step36 in r9:
 * the backend order prefers r8,r10,r9, while native frame needs r9.
 * The whole-TU current baseline is exact; unpin remains233 native/built,
 * common218,differ30 in all six. Linked source retains
 * its pin pending whole-image byte identity. */
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
void AudioCommand_PlayFar(s32 cue);

void BattleFx_RunSpiderWeb(struct BattleEffectArgument *efx)
{
	struct EffectPosition	 first;
	struct EffectPosition	 last;
	void					*canvas;
	struct BattleEffectWork *work;
	s32						 size;
	s32						 step;

	s32 frame;
	s32			 i;

	/* FAKEMATCH: needed to match: size and step first hold the slot table and blend register addresses, sharing their registers */
	size = (s32)&Ram_WorkSlot[40];
	canvas = *(void **)size;
	work = *(struct BattleEffectWork **)(size - 4);
	work->effect = efx;
	BattleFx_BeginCanvasLayer(2);
	step = 0x04000052;
	REG_BG2PA = 0x100;
	*(u16 *)step = 0x1000;
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[work->effect->count - 1], &last);
	first.x += (last.x - first.x) / 2;
	REG_BG2X = (64 - first.x) << 8;
	Resource_LoadAndDecompress((s32)&ResourceId_SpiderWebSheet, work->sheet, 1, 1);
	work->transfer_mode = 1;
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(143);
	frame = 0;
	step = 1;
	size = 32;
	do
	{
		if (frame <= 8)
			REG_BLDALPHA = (frame << 1) | 0x1000;
		if (frame > 53)
			REG_BLDALPHA = (0x7c - (frame << 1)) | 0x1000;
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, step);
		((DrawRectangle)Ram_WorkSlot[46])(canvas, work, 33, 41, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, step);
		((DrawRectangle)Ram_WorkSlot[46])(canvas, work, 64, 41, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 11, step);
		((DrawRectangle)Ram_WorkSlot[46])(canvas, work, 33, 72, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 15, step);
		((DrawRectangle)Ram_WorkSlot[46])(canvas, work, 64, 72, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		if (frame == 32)
			BattleEventRuntime_BeginPhaseFar(143);
		for (i = 0; i != work->effect->count; i++)
		{
			if (frame == 10)
				ObjectGroup_UpdateMembers(work->effect->actors[i], 7, -1, i, 8);
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = step;
		WaitFrames(1);
		frame++;
	} while (frame != 63);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}
