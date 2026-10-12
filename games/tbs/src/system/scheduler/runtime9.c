#include "unit/system/scheduler/runtime9.h"
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

void Runtime_InitializeHeap(void)
{
	union HeapState *work = (union HeapState *)gWorkSlot;
	volatile u32	 zero = 0;
	Dma_Set(&zero, work, 0x85000040, REG_DMA3);
	work->heap.next_iwram = gIwramHeap;
	work->heap.next_ewram = gEwramHeap;
}

s32 Runtime_GetRemainingIwram(void)
{
	union HeapState *work = (union HeapState *)gWorkSlot;

	return (s32)(*(u8(*)[]) & gCartridgeResetMarker) - (s32)work->heap.next_iwram;
}

s32 Runtime_GetRemainingEwram(void)
{
	union HeapState *work = (union HeapState *)gWorkSlot;

	return 0x02040000 - (s32)work->heap.next_ewram;
}

void *Runtime_AllocateHeapBlock(s32 kind, s32 size)
{
	union HeapState *work;
	s32				 aligned_size;
	u32				 address;
	u32				 next_address;
	u32				 next;
	u32				 cached_address;

	work = (union HeapState *)gWorkSlot;
	cached_address = (u32)work->slots[kind];
	if (cached_address == 0)
	{
		cached_address = (u32)work->heap.next_iwram;
		aligned_size = (((u32)size + 3) >> 2) * 4;
		next = cached_address + aligned_size;
		if (next >= (u32)Ram_IwramHeapEnd)
		{
			address = (u32)work->heap.next_ewram;
			next_address = address + aligned_size;
			if (next_address >= 0x02040000U)
			{
				return NULL;
			}
			work->heap.next_ewram = (void *)next_address;
			work->slots[kind] = (void *)address;
			return (void *)address;
		}
		work->heap.next_iwram = (void *)next;
		work->slots[kind] = (void *)cached_address;
		return (void *)cached_address;
	}
	return (void *)cached_address;
}

void *Runtime_AllocateBlock(s32 kind, s32 size)
{
	union HeapState *work;
	u32				 aligned_size;
	u32				 next;
	u32				 address;
	u32				 next_address;
	u32				 cached_address;

	work = (union HeapState *)gWorkSlot;
	cached_address = (u32)work->slots[kind];
	if (cached_address == 0)
	{
		address = (u32)work->heap.next_ewram;
		aligned_size = (((u32)size + 3) >> 2) * 4;
		next = address + aligned_size;
		if (next >= 0x02040000U)
		{
			address = (u32)work->heap.next_iwram;
			next_address = address + aligned_size;
			if (next_address >= (u32)Ram_IwramHeapEnd)
			{
				return NULL;
			}
			work->heap.next_iwram = (void *)next_address;
			work->slots[kind] = (void *)address;
			return (void *)address;
		}
		work->heap.next_ewram = (void *)next;
		work->slots[kind] = (void *)address;
		return (void *)address;
	}
	return (void *)cached_address;
}

void *MemAlloc(s32 size)
{
	union HeapState *work = (union HeapState *)gWorkSlot;
	u32				 next_address;
	u32				 next;
	u32				 allocation_address;
	u32				 aligned_words = ((u32)size + 3) >> 2;

	allocation_address = (u32)work->heap.next_iwram;
	size = aligned_words << 2;
	next = allocation_address + size;
	if (next >= (u32)Ram_IwramHeapEnd)
	{
		allocation_address = (u32)work->heap.next_ewram;
		next_address = allocation_address + size;
		if (next_address >= 0x02040000U)
		{
			return NULL;
		}
		work->heap.next_ewram = (void *)next_address;
	}
	else
	{
		work->heap.next_iwram = (void *)next;
	}
	return (void *)allocation_address;
}

void *Runtime_BumpAllocateAlternatePool(s32 size)
{
	union HeapState *work = (union HeapState *)gWorkSlot;
	u32				 alternate_next_address;
	u32				 primary_next_address;
	u32				 allocation_address;
	u32				 aligned_words = ((u32)size + 3) >> 2;

	allocation_address = (u32)work->heap.next_ewram;
	size = aligned_words << 2;
	primary_next_address = allocation_address + size;
	if (primary_next_address >= 0x02040000U)
	{
		allocation_address = (u32)work->heap.next_iwram;
		alternate_next_address = allocation_address + size;
		if (alternate_next_address >= (u32)Ram_IwramHeapEnd)
		{
			return NULL;
		}
		work->heap.next_iwram = (void *)alternate_next_address;
	}
	else
	{
		work->heap.next_ewram = (void *)primary_next_address;
	}
	return (void *)allocation_address;
}

void RuntimeMemory_ReservedNoOp(void)
{
}

//--------------------------------------------------------------------------

// render.c

// 表示：変換行列の退避・復帰と、回転・移動・拡大・カメラの行列づくり

//--------------------------------------------------------------------------

// 変換行列 gTransform は s32 が 12 個（Q16 で 1.0 = 0x10000）。
// [0]〜[8] が 3x3 の回転、[9]〜[11] が平行移動。
// Iwram_TransformMatrix は渡した行列を今の行列にかけ合わせる。
