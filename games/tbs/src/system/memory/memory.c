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



extern u8 gCartridgeResetMarker[];

void Bg0_ClearTilemap(void)
{
    volatile u32 fill;

    fill = 0xf000f000;
    Dma_Set((const void *)&fill, BG_SCREEN_BLOCK(4), 0x85000140, REG_DMA3);
    /* FAKEMATCH: the cursor store sits in a do-while(0) to hold the BG0CNT constant load order */
    do {
        gDebugTextCursor = (u16 *)BG_SCREEN_BLOCK(4);
    } while (0);
    {
        s32 control = 0x400;

        REG_BG0CNT = control;
    }
}

void Ui_LoadWindowGraphics(void)
{
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles), BG_CHAR_BLOCK(0), 0x84000800,
            REG_DMA3);
    Dma_Set((const void *)Ui_WindowPalette, (void *)&BG_PLTT_COLOR(15, 0), 0x80000010,
            REG_DMA3);
    /* FAKEMATCH: the two do-while blocks keep the value-before-address order of the palette stores */
    do {
        u16 black = 0;
        PLTT_BACKDROP = black;
    } while (0);
    do {
        volatile u16 *pal;
        s32 color;

        color = 0x4180;
        pal = &BG_PLTT_COLOR(15, 4);
        *pal = color;
        color = 0x3960;
        pal++;
        *pal = color;
        color = 0x3140;
        pal++;
        *pal = color;
        color = 0x2920;
        pal++;
        *pal = color;
        color = 0x49a0;
        pal++;
        *pal = color;
        color = 0x51c0;
        pal++;
        *pal = color;
        color = 0x59e0;
        pal++;
        *pal = color;
    } while (0);
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
