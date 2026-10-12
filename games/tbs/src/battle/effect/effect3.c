#include "unit/battle/effect/fxscales.h"
#include "unit/battle/effect/koukabit.h"
#include "unit/battle/effect/koukatbl.h"
#include "unit/battle/effect/effect.h"
#include "unit/battle/effect/effect2.h"
#include "unit/battle/effect/effect3.h"
#include "unit/battle/effect/effect4.h"
#include "unit/battle/effect/effect5.h"
#include "unit/battle/effect/effect6.h"
#include "unit/battle/effect/effect7.h"
#include "unit/battle/effect/effect8.h"
#include "unit/battle/effect/effect9.h"
#include "unit/battle/effect/effect10.h"
#include "unit/battle/effect/effect11.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
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
#include "canvas.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "menu_list.h"
#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "scene.h"
#include "global_cells.h"
#include "dma.h"
#include "resource_ids.h"
#include "battle_efx.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "btldisp.h"
#include "ram_buffer.h"
#include "resource.h"
#include "b5_context.h"
#include "motobj.h"
#include "iwram_call.h"
#include "transform.h"
#include "btlwork.h"
#include "project.h"
#include "field_event.h"
#include "keystate.h"
#include "btltype.h"
#include "sound_ids.h"
#include "call.h"
#include "btlsys.h"
#include "battle_session.h"
#include "animspr.h"
#include "battle_unit.h"
#include "fixed_point_position.h"
#include "irq.h"

void BattleFx_RunTwoResourceMode0(struct BattleEffectArgument *efx)
{
	BattleFx_RunTwoResource(efx, 0);
}

void BattleFx_RunTwoResourceMode1(struct BattleEffectArgument *efx)
{
	BattleFx_RunTwoResource(efx, 1);
}

void BattleFx_RunTwoResource(struct BattleEffectArgument *efx, s32 mode)
{
	struct BattleEffectWork *work;
	void					*canvas;
	struct MotionObject		*object;
	struct EffectPosition	 position;
	/* FAKEMATCH: needed to match: only draw[0] is used: the second word fills four bytes of the frame */
	DrawRectangle draw[2];
	s32			  frames;
	s32			  frame;
	s32			  cell;
	void		**heap;
	void		**p;

	heap = (void **)gBattleFxWork;
	p = heap;
	work = *p++;
	canvas = *p;
	work->effect = efx;
	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100;
	if (work->effect->side == 1)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 1);
		draw[0] = (DrawRectangle)heap[7];
	}
	else
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 1);
		draw[0] = (DrawRectangle)heap[7];
	}
	Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet, Ram_MapCellBuffer, 1, 0);
	if (mode == 0)
	{
		void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
		Iwram_CopyWords((void *)0x05000000, palette, 128);
	}
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &position);
	if (work->effect->side == 0)
		REG_BG2X = (16 - position.x) << 8;
	else
		REG_BG2X = (112 - position.x) << 8;
	frames = 74;
	if (mode != 1)
		frames = 48;
	for (frame = 0; frame != frames; frame++)
	{
		cell = frame / 4;
		if (cell <= 5)
		{
			if (cell <= 3)
				draw[0](canvas, work->sheet + TwoResource_CellSourceOffsets[cell],
					TwoResource_CellX[cell + work->effect->side * 6],
					TwoResource_CellBiasY[cell] + 32, TwoResource_CellWidths[cell],
					TwoResource_CellHeights[cell]);
			else
				draw[0](canvas, Ram_MapCellBuffer + TwoResource_CellSourceOffsets[cell],
					TwoResource_CellX[cell + work->effect->side * 6],
					TwoResource_CellBiasY[cell] + 32, TwoResource_CellWidths[cell],
					TwoResource_CellHeights[cell]);
		}
		if (frame == 8)
		{
			if (mode == 0)
			{
				BattleEventRuntime_BeginPhaseFar(0x85);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
			}
			else
			{
				AudioCommand_PlayFar(0x86);
				ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
			}
			work->shake_frames = 8;
		}
		if (mode == 1)
		{
			if (frame == 13)
			{
				object->velocity_y = 0xc0000;
				object->vertical_motion_strength = 0x7851;
				object->vertical_motion_phase = 0x4000;
			}
			if (frame == 65)
			{
				work->shake_frames = 4;
				BattleEventRuntime_BeginPhaseFar(0x86);
			}
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunSpiderWeb(struct BattleEffectArgument *efx)
{
	struct EffectPosition	 first;
	struct EffectPosition	 last;
	void					*canvas;
	struct BattleEffectWork *work;
	s32						 size;
	s32						 step;

	/* FAKEMATCH: the frame counter is pinned to r9, as the reference keeps it. 2026-10-11: as a plain local its priority (20 refs/232 insns) ties size and outranks step (13/240), so it takes sl and step r9 (differ 30, one register swap); the reference needs step ranked above the counter, which no plain shape gives (for/while/u32, declaration order, a bound variable, early init, argument use, 19k permuter candidates). */
	register s32 frame __asm__("r9");
	s32			 i;

	/* FAKEMATCH: needed to match: size and step first hold the slot table and blend register addresses, sharing their registers 2026-10-11: plain slot reads and REG_BLDALPHA differ 56; the reads alone 40, the register alone 50. */
	size = (s32)&Ram_WorkSlot[HEAP_SLOT_BATTLE_CANVAS];
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
		((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 33, 41, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, step);
		((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 64, 41, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 11, step);
		((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 33, 72, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 15, step);
		((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 64, 72, size, size);
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
