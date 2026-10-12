#include "unit/system/scheduler/objvram.h"
#include "unit/system/scheduler/heap.h"
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

//--------------------------------------------------------------------------
// size バイト分の続いたあきブロックを前からさがし、id のものにする
// 戻り値 : VRAM の中の位置（バイト）、なければ -1
//--------------------------------------------------------------------------
s32 ResourceTable_AllocateBlocks(u32 id, u32 size)
{
	u32 blocks;
	s32 ret;
	s32 pos;
	u32 end;
	u32 i;

	blocks = size / VRAM_BLOCK_BYTES;
	if (id >= VRAM_CACHE_ENTRY_COUNT)
	{
		return -1;
	}
	pos = 0;
	for (;;)
	{
		ret = -1;
		if (pos >= VRAM_BLOCK_COUNT)
		{
			goto done;
		}
		if (ResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE)
		{
			goto occupied;
		}
		ret = pos;
		end = blocks + ret;
		while (pos < end)
		{
			if (ResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE)
			{
				goto occupied;
			}
			pos++;
		}
		for (i = 0; i < blocks; i++)
		{
			ResourceBlockOwners[ret + i] = id;
		}
		goto found;
	occupied:
		// 使っているブロックは、持ち主の大きさの分だけとばす
		pos += gVramBlockCache[ResourceBlockOwners[pos]].size / VRAM_BLOCK_BYTES;
	}
found:
	ret *= VRAM_BLOCK_BYTES;
done:
	return ret;
}

//--------------------------------------------------------------------------
// 続いたあきブロックのいちばん長い数を返す
//--------------------------------------------------------------------------
s32 ResourceTable_GetLongestFreeBlockRun(void)
{
	s32 run = 0;
	u8 *p = ResourceBlockOwners;
	s32 max = 0;
	s32 n = VRAM_BLOCK_COUNT;

	do
	{
		if (*p++ != VRAM_BLOCK_OWNER_FREE)
		{
			run = 0;
		}
		else
		{
			run++;
			if (max < run)
				max = run;
		}
		n--;
	} while (n != 0);
	return max;
}

//--------------------------------------------------------------------------
// id の持っているブロックをすべてあきにする
// 戻り値 : あけたブロックがあれば -1、なければ 0（id が範囲外なら -1）
//--------------------------------------------------------------------------
s32 Resource_ClearSlotReferences(s32 id)
{
	s32 cleared = 0;
	s32 n;
	u8 *p;
	u8	empty;

	if ((u32)id >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	p = ResourceBlockOwners;
	empty = VRAM_BLOCK_OWNER_FREE;
	n = VRAM_BLOCK_COUNT;
	do
	{
		if (*p == id)
		{
			*p = empty;
			cleared++;
		}
		n--;
		p++;
	} while (n != 0);
	if (cleared != 0)
		return -1;
	return 0;
}

//--------------------------------------------------------------------------
// キャッシュ no を空ける（ブロックを返し、あきのしるしを付ける）
//--------------------------------------------------------------------------
s32 Resource_ResetEntry(u32 no)
{
	struct VramBlockCacheEntry *entry = &gVramBlockCache[no];

	if (no >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	if (entry->offset != VRAM_CACHE_OFFSET_FREE)
	{
		Resource_ClearSlotReferences(no);
		entry->offset |= VRAM_CACHE_OFFSET_FREE;
		entry->size = 0;
	}
	return 0;
}

//--------------------------------------------------------------------------
// キャッシュ no のブロックだけを返し、size を 1 にして番号は持ったままにする
//--------------------------------------------------------------------------
s32 Resource_ActivateEntry(u32 no)
{
	struct VramBlockCacheEntry *entry = &gVramBlockCache[no];

	if (no >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	if (entry->size > 16)
	{
		s32 one;

		Resource_ClearSlotReferences(no);
		one = 1;
		entry->size = one;
	}
	return 0;
}

//--------------------------------------------------------------------------
// キャッシュ no に size バイトの VRAM を用意し、src を写す
// 同じ大きさの VRAM を持っていればそのまま使う
// src : NULL なら写さない、-1 なら 0 で埋める
// 戻り値 : OBJ のタイル番号（32 バイト単位）、用意できなければ 0
//--------------------------------------------------------------------------
s32 VramBlock_LoadCached(u32 no, u32 size, const void *src)
{
	struct VramBlockCacheEntry *entry;
	s32							ofs;
	void					   *dst;

	entry = &gVramBlockCache[no];
	if (no >= VRAM_CACHE_ENTRY_COUNT)
		return 0;
	if (size > 8192)
		return 0;
	if (entry->size > 16)
	{
		if (entry->size != size)
		{
			Resource_ResetEntry(no);
			ofs = ResourceTable_AllocateBlocks(no, size);
		}
		else
		{
			ofs = entry->offset;
		}
	}
	else
	{
		ofs = ResourceTable_AllocateBlocks(no, size);
	}

	if (ofs != -1)
	{
		dst = (void *)(0x06010000 + ofs);
		entry->size = size;
		entry->offset = ofs;
		if (src != 0)
		{
			if (src == (const void *)-1)
			{
				Iwram_ClearWords(dst, size);
			}
			else
			{
				Dma_Set(src, dst, (size >> 2) | (DMA_ENABLE32 | DMA_32BIT), REG_DMA3);
			}
		}
		return (u32)ofs >> 5;
	}
	return 0;
}

//--------------------------------------------------------------------------
// ブロックとキャッシュをすべてあきにする
//--------------------------------------------------------------------------
void Resource_InitializeTable(void)
{
	u32 limit = VRAM_BLOCK_COUNT - 1;
	u8 *p = ResourceBlockOwners;
	u32 n = 0;
	u32 empty = VRAM_BLOCK_OWNER_FREE;

	do
	{
		*p++ = empty;
		n++;
	} while (n <= limit);

	{
		struct VramBlockCacheEntry *entry = gVramBlockCache;

		n = 0;
		do
		{
			entry->offset |= VRAM_CACHE_OFFSET_FREE;
			entry->size = 0;
			entry++;
			n++;
		} while (n < VRAM_CACHE_ENTRY_COUNT);
	}
}

//--------------------------------------------------------------------------
// あいているキャッシュの番号を返す（なければ VRAM_CACHE_ENTRY_COUNT）
//--------------------------------------------------------------------------
s32 Resource_FindFreeEntry(void)
{
	s32							ret = VRAM_CACHE_ENTRY_COUNT;
	s32							no;
	struct VramBlockCacheEntry *entry = gVramBlockCache;

	for (no = 0; no < VRAM_CACHE_ENTRY_COUNT; no++, entry++)
	{
		if (entry->offset == VRAM_CACHE_OFFSET_FREE)
		{
			ret = no;
			break;
		}
	}
	return ret;
}

//--------------------------------------------------------------------------
// あいているキャッシュに size バイトの VRAM を用意して、その番号を返す
//--------------------------------------------------------------------------
s32 Resource_LoadIntoFreeSlot(s32 size)
{
	s32 no;

	no = Resource_FindFreeEntry();
	VramBlock_LoadCached(no, size, 0);
	return no;
}

//--------------------------------------------------------------------------
// キャッシュ no に、今の大きさのまま src を写しなおす
// 戻り値 : OBJ のタイル番号
//--------------------------------------------------------------------------
s32 Resource_GetBuffer(s32 no, s32 src)
{
	return VramBlock_LoadCached(no, gVramBlockCache[no].size, (const void *)src);
}
