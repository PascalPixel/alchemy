#include "unit/field/common/fielddat.h"
#include "unit/field/common/object/objdata.h"
#include "unit/graphics/render/facing.h"
#include "unit/graphics/tile/griddata.h"
#include "unit/lib/curvedat.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field8.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "unit/field/common/object/update.h"
#include "unit/graphics/tile/vertices.h"
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
#include "resource.h"
#include "mem.h"
#include "types.h"
#include "animspr.h"
#include "global_cells.h"
#include "dma.h"
#include "ramcode.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "iowrite.h"
#include "heap_state.h"
#include "affine.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "object_dispatch.h"
#include "metadata_lookup.h"
#include "edition.h"
#include "objdisp.h"
#include "object_runtime.h"
#include "btlsys.h"
#include "transform.h"
#include "script.h"
#include "script_motion.h"
#include "script_object_entry.h"
#include "script_object_runtime.h"
#include "fieldobj.h"
#include "fixed_math.h"
#include "keystate.h"
#include "fieldrun.h"
#include "event_runtime.h"
#include "game_state.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "resource_ids.h"
#include "flag_ids.h"
#include "mapcopy.h"
#include "fixed_point_position.h"
#include "mapframe.h"
#include "btldisp.h"
#include "project.h"
#include "frame.h"
#include "palqueue.h"
#include "curve.h"
#include "btlpty.h"
#include "b5_context.h"
#include "vram_tab.h"
#include "vram_block.h"

void Runtime_AllocateAndClearQueue(void)
{
	struct PaletteSnapshotWork *queue;
	struct PaletteSnapshot	   *entry;
	u16							i;
	u16							j;

	queue = Runtime_AllocateBlock(28, sizeof(struct PaletteSnapshotWork));
	entry = queue->entries;
	for (i = 0; i != 4; i++)
	{
		entry->source = 0;
		entry->position = 0;
		entry->timer = 0;
		entry->delay = 0;
		entry->count = 0;
		for (j = 0; j != 16; j++)
		{
			entry->colors[j] = 0;
		}
		entry++;
	}
	queue->count = 0;
}

s32 PaletteQueue_Add(s16 bank, s16 index, s16 delay, s16 count)
{
	struct PaletteSnapshotWork *work;
	struct PaletteSnapshot	   *entry;
	u16						   *source;
	u32							slot;
	u16							size;

	work = gPaletteWork;
	slot = work->count;
	if (slot > 3)
		return -1;
	entry = &work->entries[slot];
	source = (u16 *)BG_PLTT + (((u16)bank << 4) + (u16)index);
	size = count;
	entry->position = 0;
	entry->timer = 0;
	entry->count = size;
	entry->source = source;
	entry->delay = delay;
	Dma_Set(source, entry->colors, 0x80000000 | size, REG_DMA3);
	work->count++;
	return 0;
}

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
	Scheduler_RemoveCallback(PaletteQueue_StepCycles);
	Runtime_ReleaseHeapBlock(0x1C);
}

void Scheduler_ScheduleCallbackBAfterFrames(void)
{
	Scheduler_AddOrUpdateCallback(PaletteQueue_StepCycles, 0xc80);
}

void PaletteQueue_StepCycles(void)
{
	struct PaletteSnapshotWork *work = gPaletteWork;
	u8							i;
	u16							count = work->count & 3;
	u16							buf[16];

	for (i = 0; i < count; i++)
	{
		struct PaletteSnapshot *cycle = &work->entries[i];
		if (cycle->timer == 0)
		{
			u16	  start = cycle->position;
			u16	  len = cycle->count;
			void *dest = cycle->source;
			u16	 *src = cycle->colors;
			u8	  j;
			for (j = len - start; j < len; j++)
				buf[j] = *src++;
			for (j = 0; j < len - start; j++)
				buf[j] = *src++;
			Dma_Set(buf, dest, 0x80000000 | len, REG_DMA3);
			start++;
			if (start >= len)
				start = 0;
			cycle->position = start;
			cycle->timer = cycle->delay;
		}
		else
		{
			cycle->timer--;
		}
	}
}
