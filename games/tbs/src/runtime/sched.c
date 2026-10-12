#include "unit/runtime/sched.h"
#include "unit/runtime/basepal.h"
#include "unit/runtime/heap.h"
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
#include "dma.h"
#include "ramcode.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "callback_scheduler.h"
#include "irq.h"
#include "types.h"
#include "scene.h"
#include "resource.h"
#include "resource_ids.h"
#include "global_cells.h"
#include "cartboot.h"
#include "ram_buffer.h"
#include "serial_runtime.h"
#include "frame.h"
#include "keystate.h"
#include "input.h"
#include "oam.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "irqlock.h"
#include "iowrite.h"
#include "lib.h"
#include "fixed_math.h"
#include "affine.h"
#include "iwram_call.h"
#include "vram_block.h"
#include "string.h"
#include "low_runtime.h"
#include "vram_tab.h"
#include "layout_guard.h"
#include "edition.h"
#include "transform.h"
#include "project.h"
#include "save_state.h"
#include "runtime_interfaces.h"
#include "flash.h"
#include "audio_engine_symbols.h"

typedef s32 (*KeyCallbackFn)(void);

void Scheduler_ResetTaskTable(void)
{
	struct SchedulerTask *task = gSchedulerTaskTable;
	s32					  remaining;

	gSchedulerTaskCount = 0;
	gSchedulerStatus = 0;
	remaining = SCHEDULER_TBS_TASK_COUNT - 1;
	do
	{
		task->callback = 0;
		task->state = 0xffff;
		task->mask = 0;
		task++;
		remaining--;
	} while (remaining >= 0);
	gSchedulerTaskCount = 1;
}

void Scheduler_CopyWords(u32 *destination, u32 *source, u32 byte_count)
{
	u32 index;
	byte_count >>= 2;
	for (index = 0; index < byte_count; index++)
		*destination++ = *source++;
}

void Scheduler_SortTasks(void)
{
	/* FAKEMATCH: the leading test and reused swap cursor retain 84 bytes; ordinary nested for loops produce 80. */
	struct SchedulerTask  saved;
	struct SchedulerTask *base = gSchedulerTaskTable;
	struct SchedulerTask *task;
	s32					  pass = SCHEDULER_TBS_TASK_COUNT - 1;
	s32					  remaining;
	goto sort_pass;
next_pass:
	base = gSchedulerTaskTable;
sort_pass:
	task = base;
	if (pass <= 0)
		goto finish_pass;
	remaining = pass;
next_task:
	if ((s16)task[1].state > (s16)task->state)
	{
		memcpy(&saved, task, sizeof(saved));
		base = task;
		task++;
		memcpy(base, task, sizeof(saved));
		memcpy(task, &saved, sizeof(saved));
	}
	else
	{
		task++;
	}
	if (--remaining != 0)
		goto next_task;
finish_pass:
	if (--pass > 1)
		goto next_pass;
}

s32 Scheduler_FindCallback(SchedulerCallback callback)
{
	s32					  result;
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if (task->callback == (u32)callback)
		{
			result = i;
			break;
		}
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

s32 Scheduler_AddOrUpdateCallback(SchedulerCallback callback, s32 order)
{
	u32					  saved_interrupt_state;
	s32					  index;
	struct SchedulerTask *task;
	s32					  i;

	index = -1;
	task = gSchedulerTaskTable;
	(void)gSchedulerStatus;
	IRQ_LOCK(saved_interrupt_state);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if (task->callback == (u32)callback)
		{
			task->state = order;
			index = i;
			break;
		}
	}
	task = gSchedulerTaskTable;
	if (index == -1)
	{
		for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
		{
			if (task->callback == 0)
			{
				task->callback = (u32)callback;
				task->state = order;
				task->mask = 0;
				index = i;
				break;
			}
		}
	}
	Scheduler_SortTasks();
	IRQ_UNLOCK(saved_interrupt_state);
	return index;
}

void Scheduler_Idle(void)
{
}

void Scheduler_EmptyCallback(void)
{
}

s32 Scheduler_RemoveCallback(SchedulerCallback callback)
{
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  result;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if (task->callback == (u32)callback)
		{
			task->callback = 0;
			task->state = 0x7fff;
			result = i;
			break;
		}
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

s32 Scheduler_EnableCallbacks(SchedulerCallback callback)
{
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  result;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if (callback != 0)
		{
			if (task->callback != (u32)callback)
				continue;
		}
		TASK_STATE_HIGH(task) |= 1;
		result = i;
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

s32 Scheduler_EnableUnmaskedOverlayCallbacks(void)
{
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  result;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if ((task->callback >> 24) == 2 && (task->mask & 1) == 0)
		{
			TASK_STATE_HIGH(task) |= 1;
			result = i;
		}
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

s32 Scheduler_SetCallbackMask(SchedulerCallback callback, u32 mask)
{
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  result;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if (task->callback == (u32)callback)
		{
			task->mask = mask;
			result = i;
			break;
		}
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

s32 Scheduler_DisableCallbacks(SchedulerCallback callback)
{
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  result;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if (callback == 0 || task->callback == (u32)callback)
		{
			TASK_STATE_HIGH(task) &= ~1;
			result = i;
		}
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

s32 Scheduler_DisableOverlayCallbacks(void)
{
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  result;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if ((task->callback >> 24) == 2)
		{
			TASK_STATE_HIGH(task) &= ~1;
			result = i;
		}
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

void Runtime_InvokeCallbacksByKey(s32 key)
{
	struct SchedulerTask *task = gSchedulerTaskTable;
	s32					  remaining;

	key >>= 8;
	if (gSchedulerTaskCount == 1)
	{
		remaining = SCHEDULER_TBS_TASK_COUNT + 1;
		task--;
	next_task:
		remaining--;
		if (remaining != 0)
		{
			task++;
			if (TASK_STATE_HIGH(task) == key)
			{
				KeyCallbackFn callback = *(KeyCallbackFn *)&task->callback;

				callback();
			}
			goto next_task;
		}
	}
}
