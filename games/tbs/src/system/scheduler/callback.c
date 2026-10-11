//--------------------------------------------------------------------------
// rescache.c
// リソース：OBJ の VRAM をブロックに分けて貸し出すキャッシュ
//--------------------------------------------------------------------------

#include "resource.h"
#include "vram_tab.h"

// OBJ のキャラクター（0x06010000 から）を 64 バイトのブロック 512 個に分け、
// ResourceBlockOwners にブロックごとの持ち主（キャッシュの番号）を置く。
// キャッシュは 96 個で、offset（バイト）が 0xffff ならあき。
// size が 16 以下のものは VRAM を持っていない

#if !GAME_TLA
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

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

#endif

//--------------------------------------------------------------------------
// キャッシュ no に、今の大きさのまま src を写しなおす
// 戻り値 : OBJ のタイル番号
//--------------------------------------------------------------------------
s32 Resource_GetBuffer(s32 no, s32 src)
{
	return VramBlock_LoadCached(no, gVramBlockCache[no].size, (const void *)src);
}
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "affine.h"
#include "resource.h"
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "global_cells.h"
#include "dma.h"
#include "mem.h"
#include "vram_block.h"
#include "scene.h"
#include "callback_scheduler.h"
#include "string.h"
#include "io_reg.h"
#include "low_runtime.h"
#include "../irqlock.h"

#define gRandomSeed (*(u32 *)&gRandomSeed)

typedef s32 (*KeyCallbackFn)(void);

void Scheduler_ResetTaskTable(void)
{
    struct SchedulerTask *task = gSchedulerTaskTable;
    s32 remaining;

    gSchedulerTaskCount = 0;
    gSchedulerStatus = 0;
    remaining = SCHEDULER_TBS_TASK_COUNT - 1;
    do {
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
    struct SchedulerTask saved;
    struct SchedulerTask *base = gSchedulerTaskTable;
    struct SchedulerTask *task;
    s32 pass = SCHEDULER_TBS_TASK_COUNT - 1;
    s32 remaining;
    goto sort_pass;
next_pass:
    base = gSchedulerTaskTable;
sort_pass:
    task = base;
    if (pass <= 0)
        goto finish_pass;
    remaining = pass;
next_task:
    if ((s16)task[1].state > (s16)task->state) {
        memcpy(&saved, task, sizeof(saved));
        base = task;
        task++;
        memcpy(base, task, sizeof(saved));
        memcpy(task, &saved, sizeof(saved));
    } else {
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
    s32 result;
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    IRQ_LOCK(saved_interrupt_master);
    for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
        if (task->callback == (u32)callback) {
            result = i;
            break;
        }
    }
    IRQ_UNLOCK(saved_interrupt_master);
    return result;
}

s32 Scheduler_AddOrUpdateCallback(SchedulerCallback callback, s32 order)
{
    u32 saved_interrupt_state;
    s32 index;
    struct SchedulerTask *task;
    s32 i;

    index = -1;
    task = gSchedulerTaskTable;
    (void)gSchedulerStatus;
    IRQ_LOCK(saved_interrupt_state);
    for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
        if (task->callback == (u32)callback) {
            task->state = order;
            index = i;
            break;
        }
    }
    task = gSchedulerTaskTable;
    if (index == -1) {
        for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
            if (task->callback == 0) {
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

void Scheduler_Idle(void) {}

void Scheduler_EmptyCallback(void) {}

s32 Scheduler_RemoveCallback(SchedulerCallback callback)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    IRQ_LOCK(saved_interrupt_master);
    for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
        if (task->callback == (u32)callback) {
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
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    IRQ_LOCK(saved_interrupt_master);
    for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
        if (callback != 0) {
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
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    IRQ_LOCK(saved_interrupt_master);
    for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
        if ((task->callback >> 24) == 2 && (task->mask & 1) == 0) {
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
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    IRQ_LOCK(saved_interrupt_master);
    for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
        if (task->callback == (u32)callback) {
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
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    IRQ_LOCK(saved_interrupt_master);
    for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
        if (callback == 0 || task->callback == (u32)callback) {
            TASK_STATE_HIGH(task)&= ~1;
            result = i;
        }
    }
    IRQ_UNLOCK(saved_interrupt_master);
    return result;
}

s32 Scheduler_DisableOverlayCallbacks(void)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    IRQ_LOCK(saved_interrupt_master);
    for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
        if ((task->callback >> 24) == 2) {
            TASK_STATE_HIGH(task)&= ~1;
            result = i;
        }
    }
    IRQ_UNLOCK(saved_interrupt_master);
    return result;
}

void Runtime_InvokeCallbacksByKey(s32 key)
{
    struct SchedulerTask *task = gSchedulerTaskTable;
    s32 remaining;

    key >>= 8;
    if (gSchedulerTaskCount == 1) {
        remaining = SCHEDULER_TBS_TASK_COUNT + 1;
        task--;
next_task:
        remaining--;
        if (remaining != 0) {
            task++;
            if (TASK_STATE_HIGH(task) == key) {
                KeyCallbackFn callback = *(KeyCallbackFn *)&task->callback;

                callback();
            }
            goto next_task;
        }
    }
}

u32 Random16(void)
{
    u32 value = gRandomSeed * 0x41c64e6d + 0x3039;

    gRandomSeed = value;
    return (value << 8) >> 16;
}

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position)
{
    *position++ += Iwram_MulQ16(radius, Trig_Sin(angle + 0x4000));
    position++;
    *position += Iwram_MulQ16(radius, Trig_Sin(angle));
}

s32 ArcTan2(s32 x, s32 y)
{
    const u16 *table;
    s32 value;
    s32 ratio;
    s32 result;

    if (x == 0) {
        result = 0;
    } else if (y == 0) {
        result = 0x4000;
    } else {
        ratio = y;
        if (ratio < 0)
            ratio = -ratio;
        value = x;
        if (value < 0)
            value = -value;

        ratio = (value << 8) / ratio;
        result = 0x4000;
        if (ratio <= 0xFB6A) {
            table = &Math_TanTable[127];
            result = 0;

            value = *table;
            table -= 0x40;
            if (ratio > value) {
                result = 0x2000;
                table += 0x80;
            }
            value = *table;
            table -= 0x20;
            if (ratio > value) {
                result |= 0x1000;
                table += 0x40;
            }
            value = *table;
            table -= 0x10;
            if (ratio > value) {
                result |= 0x800;
                table += 0x20;
            }
            value = *table;
            table -= 8;
            if (ratio > value) {
                result |= 0x400;
                table += 0x10;
            }
            value = *table;
            table -= 4;
            if (ratio > value) {
                result |= 0x200;
                table += 8;
            }
            value = *table;
            table -= 2;
            if (ratio > value) {
                result |= 0x100;
                table += 4;
            }
            value = *table;
            table--;
            if (ratio > value) {
                result |= 0x80;
                table += 2;
            }
            value = *table;
            if (ratio > value)
                result |= 0x40;
        }
    }

    if (y < 0)
        result = 0x8000 - result;
    if (x < 0)
        result = -result;

    return (u16)result;
}

s32 Math_IntegerSqrt(s32 value)
{
    s32 trial;
    s32 remainder;
    s32 bit;
    s32 result;

    remainder = value;
    result = 0;
    bit = 0xf;
    do {
        trial = (result << (bit + 1)) + (1 << (bit * 2));
        if (trial <= remainder) {
            result |= 1 << bit;
            remainder -= trial;
        }
        bit -= 1;
    } while (bit >= 0);
    return result;
}

s32 FixedSqrt(s32 value)
{
    s32 (*root)(s32) = Iwram_Sqrt;

    return root(value) << 8;
}

s32 Runtime_GetLowTableAddress(void)
{
    return (s32)System_BasicColorPalette;
}

void Text_FormatHexToWork(u32 value)
{
    u8 *buffer = gNumberTextBuffer;
    const u8 *digits = Text_HexDigits;
    s32 index = 7;

    do {
        buffer[index] = digits[value & 0xF];
        value >>= 4;
        index--;
    } while (index >= 0);

    {
        u8 *terminator = gNumberTextBuffer;
        terminator[8] = 0;
    }
}

void Text_FormatSignedDecimalToWork(s32 arg0) {
    u32 *tbl;
    s8 *out;
    s32 count;
    u32 word;
    s32 result;
    s32 val;
    s8 sign;

    result = arg0;
    val = result;
    tbl = (u32 *)Text_PowersOfTen;
    sign = 0x20;
    out = (s8 *)gNumberTextBuffer;
    if (val < 0) {
        val = -val;
        sign = 0x2D;
    }
    word = *tbl++;
    count = 9;
    if (val < word) {
        do {
            count -= 1;
            *out++ = 0x20;
            if (count == 0) break;
            word = *tbl++;
        } while (val < word);
    }
    *out++ = sign;
    tbl -= 1;
    if (count != 0) {
        do {
            word = *tbl++;
            result = val / word;
            *out++ = result + 0x30;
            val -= result * word;
            count -= 1;
        } while (count != 0);
    }
    out[0] = val + 0x30;
    out[1] = 0;
}

void Graphics_FillWordStreamWithF000(u32 count)
{
    u16 *dst = gDebugTextCursor;
    u32 index;

    for (index = 0; index < count; index++)
        *dst++ = 0xf000;
    gDebugTextCursor = dst;
}

#ifdef MODERN
// Camelot's compiler copied the task inline; a modern one calls memcpy,
// and the game links no C library
void *memcpy(void *destination, const void *source, unsigned long size)
{
    u8 *to = destination;
    const u8 *from = source;

    while (size-- != 0)
        *to++ = *from++;
    return destination;
}
#endif

void Runtime_WriteDebugTextTiles(const u8 *src)
{
    if (gDebugTextEnabled != 0) {
        u32 addr = ((u32)&gDebugTextCursor);
        u32 c = *src;
        u16 *dst = *(u16 **)addr;
        u32 cnt = 0;
        src++;

        if (c != 0) {
            u32 mask = 0xf000;
            addr = (u32)(BG_SCREEN_BLOCK(4) + 0x500);
            do {
                *dst++ = c | mask;
                if (dst == (u16 *)addr)
                    dst = (u16 *)BG_SCREEN_BLOCK(4);
                cnt++;
                if (cnt > 31)
                    break;
                c = *src++;
            } while (c != 0);
            addr = ((u32)&gDebugTextCursor);
        }
        *(u16 **)addr = dst;
    }
}
//--------------------------------------------------------------------------
// debug_number.c
// デバッグ：数を右そろえでデバッグ文字にする
//--------------------------------------------------------------------------

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"

// どちらも数を作業域に右づめで作り、その終わりから keta 文字を書く

//--------------------------------------------------------------------------
// 16 進で keta けた（1〜8、範囲外なら 8）
//--------------------------------------------------------------------------
void Text_DrawHexRightAligned(u32 value, s32 keta)
{
	s32 n;

	n = keta;
	if ((n - 1) > 7U)
	{
		n = 8;
	}
	Text_FormatHexToWork(value);
	Runtime_WriteDebugTextTiles((const u8 *)(((u32)&gNumberTextHexEnd) - n));
}

//--------------------------------------------------------------------------
// 符号つきの 10 進で keta けた（1〜10、範囲外なら 10）
//--------------------------------------------------------------------------
void Text_DrawSignedDecimalRightAligned(s32 value, s32 keta)
{
	s32 n;

	n = keta;
	if ((n - 1) > 9U)
	{
		n = 10;
	}
	Text_FormatSignedDecimalToWork(value);
	Runtime_WriteDebugTextTiles((const u8 *)(((u32)&gNumberTextDecimalEnd) - n));
}
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "low_runtime.h"
#include "global_cells.h"
#include "types.h"
#include "layout_guard.h"
#include "dma.h"
#include "resource.h"
#include "resource_ids.h"
#include "mem.h"
#include "heap_state.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "../iowrite.h"

extern u8 gCartridgeResetMarker[];

void Bg0_ClearTilemap(void)
{
    volatile u32 fill;

    fill = 0xf000f000;
    Dma_Set((const void *)&fill, BG_SCREEN_BLOCK(4), 0x85000140, REG_DMA3);
    gDebugTextCursor = (u16 *)BG_SCREEN_BLOCK(4);
    Io_Write16(0x400, &REG_BG0CNT);
}

void Ui_LoadWindowGraphics(void)
{
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles), BG_CHAR_BLOCK(0), 0x84000800,
            REG_DMA3);
    Dma_Set((const void *)Ui_WindowPalette, (void *)&BG_PLTT_COLOR(15, 0), 0x80000010,
            REG_DMA3);
    Io_Write16(0, &PLTT_BACKDROP);
    Io_Write16(0x4180, &BG_PLTT_COLOR(15, 4));
    Io_Write16(0x3960, &BG_PLTT_COLOR(15, 5));
    Io_Write16(0x3140, &BG_PLTT_COLOR(15, 6));
    Io_Write16(0x2920, &BG_PLTT_COLOR(15, 7));
    Io_Write16(0x49a0, &BG_PLTT_COLOR(15, 8));
    Io_Write16(0x51c0, &BG_PLTT_COLOR(15, 9));
    Io_Write16(0x59e0, &BG_PLTT_COLOR(15, 10));
    Dma_Set((const void *)System_BasicColorPalette, (void *)OBJ_PLTT, 0x800000e0,
            REG_DMA3);
}

void PaletteDma_LoadBlock(void)
{
    Dma_Set((const void *)System_BasicColorPalette, (void *)OBJ_PLTT, 0x800000e0, REG_DMA3);
}

void Runtime_InitializeHeap(void)
{
    union HeapState *work = (union HeapState *)gWorkSlot;
    volatile u32 zero = 0;
    Dma_Set(&zero, work, 0x85000040, REG_DMA3);
    work->heap.next_iwram = gIwramHeap;
    work->heap.next_ewram = gEwramHeap;
}

s32 Runtime_GetRemainingIwram(void)
{
    union HeapState *work = (union HeapState *)gWorkSlot;

    return (s32)gCartridgeResetMarker - (s32)work->heap.next_iwram;
}

s32 Runtime_GetRemainingEwram(void)
{
    union HeapState *work = (union HeapState *)gWorkSlot;

    return 0x02040000 - (s32)work->heap.next_ewram;
}

void *Runtime_AllocateHeapBlock(s32 kind, s32 size)
{
    union HeapState *work;
    s32 aligned_size;
    u32 address;
    u32 next_address;
    u32 next;
    u32 cached_address;

    work = (union HeapState *)gWorkSlot;
    cached_address = (u32)work->slots[kind];
    if (cached_address == 0) {
        cached_address = (u32)work->heap.next_iwram;
        aligned_size = (((u32)size + 3) >> 2) * 4;
        next = cached_address + aligned_size;
        if (next >= (u32)Ram_IwramHeapEnd) {
            address = (u32)work->heap.next_ewram;
            next_address = address + aligned_size;
            if (next_address >= 0x02040000U) {
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
    u32 aligned_size;
    u32 next;
    u32 address;
    u32 next_address;
    u32 cached_address;

    work = (union HeapState *)gWorkSlot;
    cached_address = (u32)work->slots[kind];
    if (cached_address == 0) {
        address = (u32)work->heap.next_ewram;
        aligned_size = (((u32)size + 3) >> 2) * 4;
        next = address + aligned_size;
        if (next >= 0x02040000U) {
            address = (u32)work->heap.next_iwram;
            next_address = address + aligned_size;
            if (next_address >= (u32)Ram_IwramHeapEnd) {
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
    u32 next_address;
    u32 next;
    u32 allocation_address;
    u32 aligned_words = ((u32)size + 3) >> 2;

    allocation_address = (u32)work->heap.next_iwram;
    size = aligned_words << 2;
    next = allocation_address + size;
    if (next >= (u32)Ram_IwramHeapEnd) {
        allocation_address = (u32)work->heap.next_ewram;
        next_address = allocation_address + size;
        if (next_address >= 0x02040000U) {
            return NULL;
        }
        work->heap.next_ewram = (void *)next_address;
        goto done;
    }
    work->heap.next_iwram = (void *)next;
done:
    return (void *)allocation_address;
}

void *Runtime_BumpAllocateAlternatePool(s32 size)
{
    union HeapState *work = (union HeapState *)gWorkSlot;
    u32 alternate_next_address;
    u32 primary_next_address;
    u32 allocation_address;
    u32 aligned_words = ((u32)size + 3) >> 2;

    allocation_address = (u32)work->heap.next_ewram;
    size = aligned_words << 2;
    primary_next_address = allocation_address + size;
    if (primary_next_address >= 0x02040000U) {
        allocation_address = (u32)work->heap.next_iwram;
        alternate_next_address = allocation_address + size;
        if (alternate_next_address >= (u32)Ram_IwramHeapEnd) {
            return NULL;
        }
        work->heap.next_iwram = (void *)alternate_next_address;
        goto done;
    }
    work->heap.next_ewram = (void *)primary_next_address;
done:
    return (void *)allocation_address;
}

void RuntimeMemory_ReservedNoOp(void)
{
}
