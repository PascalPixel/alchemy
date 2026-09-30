#include "LOW_RUNTIME.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "DMA.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "RUNTIME_MEM.H"
#include "RAM_BUFFER.H"

extern u8 Data_03001ac4[];
extern u16 *gDebugTextCursor;
extern u8 Data_03001f78[];
void Text_FormatHexToWork(u32);
extern u8 Data_03001f7a[];
void Text_FormatSignedDecimalToWork(s32);

extern const u8 System_BasicColorPalette[];

/* The window frame colours of background bank 15. */
extern const u16 Ui_WindowPalette[];
extern u8 gWorkSlot[];

struct HeapState { void *next_ewram; void *next_iwram; u8 entries[248]; };

extern u8 Data_03007800[];
extern u8 Data_03001e50[];
#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Runtime_WriteDebugTextTiles(const u8 *src)
{
    if (*(u8 *)((u32)&Data_03001ac4) != 0) {
        u32 addr = ((u32)&gDebugTextCursor);
        u32 c = *src;
        u16 *dst = *(u16 **)addr;
        u32 cnt = 0;
        src++;

        if (c != 0) {
            u32 mask = 0xf000;
            addr = 0x06002500;
            do {
                *dst++ = c | mask;
                if (dst == (u16 *)addr)
                    dst = (u16 *)0x06002000;
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

void Text_DrawHexRightAligned(u32 value, s32 width)
{
    s32 count;

    count = width;
    if ((u32)(count - 1) > 7U) {
        count = 8;
    }
    Text_FormatHexToWork(value);
    Runtime_WriteDebugTextTiles((const u8 *)(((u32)&Data_03001f78) - count));
}

void Text_DrawSignedDecimalRightAligned(s32 value, s32 width)
{
    s32 count;

    count = width;
    if ((u32)(count - 1) > 9U) {
        count = 0xA;
    }
    Text_FormatSignedDecimalToWork(value);
    Runtime_WriteDebugTextTiles((const u8 *)(((u32)&Data_03001f7a) - count));
}

/* Fill the BG0 screen block at 0x06002000 with blank tiles and home the cursor. */
void Bg0_ClearTilemap(void)
{
    volatile u32 fill;

    fill = 0xf000f000;
    Dma_Set((const void *)&fill, (void *)0x06002000, 0x85000140, (volatile u32 *)0x040000d4);
    /* FAKEMATCH: the cursor store sits in a do-while(0) to hold the BG0CNT constant load order */
    do {
        gDebugTextCursor = (u16 *)0x06002000;
    } while (0);
    {
        s32 control = 0x400;

        *(volatile u16 *)0x04000008 = control;
    }
}

/* Loads the shared window graphics: resource 0x13 into BG character block
   0, the window palette into background bank 15, the bank-15 frame colours
   and the object palettes. */
void Ui_LoadWindowGraphics(void)
{
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles), (void *)0x06000000, 0x84000800,
            (volatile u32 *)0x040000d4);
    Dma_Set((const void *)Ui_WindowPalette, (void *)0x050001e0, 0x80000010,
            (volatile u32 *)0x040000d4);
    /* FAKEMATCH: the two do-while blocks keep the value-before-address
       order of the palette stores */
    do {
        s32 black = 0;
        *(volatile u16 *)0x05000000 = black;
    } while (0);
    do {
        volatile u16 *pal;
        s32 color;

        color = 0x4180;
        pal = (volatile u16 *)0x050001e8;
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
    Dma_Set((const void *)System_BasicColorPalette, (void *)0x05000200, 0x800000e0,
            (volatile u32 *)0x040000d4);
}

void PaletteDma_LoadBlock(void)
{
    Dma_Set((const void *)System_BasicColorPalette, (void *)0x05000200, 0x800000e0, (volatile u32 *)0x040000d4);
}

void Runtime_InitializeHeap(void)
{
    struct HeapState *work = (struct HeapState *)gWorkSlot;
    volatile u32 zero = 0;
    Dma_Set(&zero, work, 0x85000040, (volatile u32 *)0x040000d4);
    work->next_iwram = gIwramHeap;
    work->next_ewram = gEwramHeap;
}

s32 Runtime_GetRemainingIwram(void)
{
    s32 state = ((u32)&Data_03001e50);

    return (s32)Data_03007800 - *(s32 *)(state + 4);
}

s32 Runtime_GetRemainingEwram(void)
{
    return 0x02040000 - *(s32 *)((u32)&Data_03001e50);
}

s32 Runtime_AllocateHeapBlock(s32 kind, s32 size)
{
    u32 *allocator_state;
    s32 kind_offset;
    s32 aligned_size;
    u32 address;
    u32 next_address;
    u32 next;
    u32 cached_address;

    allocator_state = (u32 *)((u32)&Data_03001e50);
    kind_offset = kind * 4;
    cached_address = *(u32 *)((u8 *)allocator_state + kind_offset);
    if (cached_address == 0) {
        cached_address = allocator_state[1];
        aligned_size = (((u32)size + 3) >> 2) * 4;
        next = cached_address + aligned_size;
        if (next >= (u32)Ram_IwramHeapEnd) {
            address = allocator_state[0];
            next_address = address + aligned_size;
            if (next_address >= 0x02040000U) {
                return 0;
            }
            allocator_state[0] = next_address;
            *(u32 *)((u8 *)allocator_state + kind_offset) = address;
            return (s32)address;
        }
        allocator_state[1] = next;
        *(u32 *)((u8 *)allocator_state + kind_offset) = cached_address;
        return (s32)cached_address;
    }
    return (s32)cached_address;
}

void *Runtime_AllocateBlock(s32 kind, s32 size)
{
    u32 *allocator_state;
    s32 kind_offset;
    u32 aligned_size;
    u32 next;
    u32 address;
    u32 next_address;
    u32 cached_address;

    allocator_state = (u32 *)((u32)&Data_03001e50);
    kind_offset = kind * 4;
    cached_address = *(u32 *)((u8 *)allocator_state + kind_offset);
    if (cached_address == 0) {
        address = allocator_state[0];
        aligned_size = (((u32)size + 3) >> 2) * 4;
        next = address + aligned_size;
        if (next >= (u32)(129 << 18)) {
            address = allocator_state[1];
            next_address = address + aligned_size;
            if (next_address >= (u32)Ram_IwramHeapEnd) {
                return NULL;
            }
            allocator_state[1] = next_address;
            *(u32 *)((u8 *)allocator_state + kind_offset) = address;
            return (void *)address;
        }
        allocator_state[0] = next;
        *(u32 *)((u8 *)allocator_state + kind_offset) = address;
        return (void *)address;
    }
    return (void *)cached_address;
}

u32 Runtime_BumpAllocate(s32 size)
{
    u32 *allocator_state = (u32 *)((u32)&Data_03001e50);
    u32 next_address;
    u32 next;
    u32 allocation_address;
    u32 aligned_words = ((u32)size + 3) >> 2;

    allocation_address = allocator_state[1];
    size = (s32)(aligned_words << 2);
    next = allocation_address + (u32)size;
    if (next >= (u32)Ram_IwramHeapEnd) {
        allocation_address = allocator_state[0];
        next_address = allocation_address + (u32)size;
        if (next_address >= 0x02040000U) {
            return 0U;
        }
        allocator_state[0] = next_address;
        goto block_5;
    }
    allocator_state[1] = next;
block_5:
    return allocation_address;
}

s16 *Runtime_BumpAllocateAlternatePool(s32 arg0)
{
    s32 allocator_state_address = ((u32)&Data_03001e50);
    u32 alternate_next_address;
    u32 primary_next_address;
    u32 allocation_address;
    u32 aligned_words = ((u32)arg0 + 3) >> 2;

    allocation_address = FIELD_AT_OFFSET((void *)allocator_state_address, u32 *, 0);
    arg0 = (s32)(aligned_words << 2);
    primary_next_address = allocation_address + (u32)arg0;
    if (primary_next_address >= 0x02040000U) {
        allocation_address = FIELD_AT_OFFSET((void *)allocator_state_address, u32 *, 4);
        alternate_next_address = allocation_address + (u32)arg0;
        if (alternate_next_address >= (u32)Ram_IwramHeapEnd) {
            return NULL;
        }
        FIELD_AT_OFFSET((void *)allocator_state_address, u32 *, 4) = alternate_next_address;
        goto done;
    }
    FIELD_AT_OFFSET((void *)allocator_state_address, u32 *, 0) = primary_next_address;
done:
    return (s16 *)allocation_address;
}

void RuntimeMemory_ReservedNoOp(void)
{
}
