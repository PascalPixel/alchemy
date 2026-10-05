#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "RUNTIME_MEM.H"
#include "DMA.H"
#include "IO_REG.H"
#include "RESOURCE.H"

/* The window frame colours of background bank 15. */
extern const u16 Ui_WindowPalette[];
extern const u8 System_BasicColorPalette[];
extern char ResourceId_WindowTiles;

/* Loads the shared window graphics: resource 0x13 into BG character block
   0, the window palette into background bank 15, the bank-15 frame colours
   and the object palettes. */
void Ui_LoadWindowGraphics(void)
{
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles), (void *)0x06000000, 0x84000800,
            (volatile u32 *)0x040000d4);
    Dma_Set((const void *)Ui_WindowPalette, (void *)0x050001e0, 0x80000010,
            (volatile u32 *)0x040000d4);
    /* FAKEMATCH: the do-while block keeps the backdrop store's value
       before its address */
    do {
        s32 black = 0;
        PLTT_BACKDROP = black;
    } while (0);
    {
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
    }
    Dma_Set((const void *)System_BasicColorPalette, (void *)0x05000200, 0x800000e0,
            (volatile u32 *)0x040000d4);
}

void PaletteDma_LoadBlock(void)
{
    Dma_Set((const void *)System_BasicColorPalette, (void *)0x05000200, 0x800000e0, (volatile u32 *)0x040000d4);
}

void Runtime_InitializeHeap(void)
{
    struct HeapSlots *work = Ram_HeapSlots;
    volatile u32 zero = 0;
    Dma_Set(&zero, work, 0x85000040, (volatile u32 *)0x040000d4);
    Ram_HeapSlots->next_iwram = gIwramHeap;
    Ram_HeapSlots->next_ewram = gEwramHeap;
}

/* The sound mixer's workspace, which the IWRAM heap ends at. */
extern u8 IwramSoundMixWorkspace[];

/* The IWRAM left between the heap's cursor and the mixer's workspace. */
s32 Runtime_GetRemainingIwram(void)
{
    struct HeapSlots *work = Ram_HeapSlots;

    return (s32)IwramSoundMixWorkspace - (s32)work->next_iwram;
}

/* The EWRAM left between the heap's cursor and the end of EWRAM. */
s32 Runtime_GetRemainingEwram(void)
{
    struct HeapSlots *work = Ram_HeapSlots;

    return 0x02040000 - (s32)work->next_ewram;
}

/* Hands out a block for the heap slot at byte offset kind once, from IWRAM
   while it lasts and then from EWRAM below 0x02040000, and keeps it in the
   slot; ⚓️ passes the offset where ☀️ passes the slot number. Slots 0 and
   1 hold the EWRAM and IWRAM bump pointers. Returns 0 when both are full. */
void *Runtime_AllocateHeapBlock(s32 kind, s32 size)
{
    u32 *allocator_state;
    s32 kind_offset;
    s32 aligned_size;
    u32 address;
    u32 next_address;
    u32 next;
    u32 cached_address;

    allocator_state = (u32 *)Ram_HeapSlots;
    kind_offset = kind;
    cached_address = *(u32 *)((u8 *)allocator_state + kind_offset);
    if (cached_address == 0) {
        cached_address = allocator_state[1];
        aligned_size = (((u32)size + 3) >> 2) * 4;
        next = cached_address + aligned_size;
        if (next >= (u32)Ram_IwramHeapEnd) {
            address = allocator_state[0];
            next_address = address + aligned_size;
            if (next_address >= 0x02040000U) {
                return NULL;
            }
            allocator_state[0] = next_address;
            *(u32 *)((u8 *)allocator_state + kind_offset) = address;
            return (void *)address;
        }
        allocator_state[1] = next;
        *(u32 *)((u8 *)allocator_state + kind_offset) = cached_address;
        return (void *)cached_address;
    }
    return (void *)cached_address;
}

/* Hands out a block for a byte-offset slot once, using EWRAM first and
   IWRAM when EWRAM is full. The last usable IWRAM byte precedes the
   sound mixer's workspace, so the first byte after a block may not pass it. */
void *Runtime_AllocateBlock(s32 kind, s32 size)
{
    u32 *allocator_state;
    s32 kind_offset;
    u32 aligned_size;
    u32 next;
    u32 address;
    u32 next_address;
    u32 cached_address;

    allocator_state = (u32 *)Ram_HeapSlots;
    kind_offset = kind;
    cached_address = *(u32 *)((u8 *)allocator_state + kind_offset);
    if (cached_address == 0) {
        aligned_size = (u32)size + 3;
        address = allocator_state[0];
        /* FAKEMATCH: Three plain shapes shift alignment before loading the heap head; keep that load before the shift. */
        __asm__("" : : "r"(address), "r"(aligned_size));
        aligned_size = (aligned_size >> 2) * 4;
        next = address + aligned_size;
        if (next >= 0x02040000U) {
            address = allocator_state[1];
            next_address = address + aligned_size;
            if (next_address > (u32)(Ram_IwramHeapEnd - 1)) {
                return 0;
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

/* Fills the still-free portions of the two bump heaps with a word value. */
void Runtime_FillFreeHeapWords(u32 value)
{
    u32 *state = (u32 *)Ram_HeapSlots;
    u32 address;

    address = state[1];
    Iwram_FillWords((void *)address, (u32)Ram_IwramHeapEnd - address, value);
    address = state[0];
    Iwram_FillWords((void *)address, 0x02040000U - address, value);
}

/* IWRAM-first bump allocation. All-six trials: separate/reused rounding
   locals differed by 17 bytes; named words by 12; two order constraints by
   4; an unconstrained bound by 11; the final tied handoff matched. */
void *Runtime_BumpAllocate(s32 size)
{
    u32 *state = (u32 *)Ram_HeapSlots;
    u32 other;
    u32 next;
    u32 address;
    u32 words;
    u32 limit;
    /* FAKEMATCH: An unconstrained bound load changes the heap/head registers; capture the native bound in r4 and hand it to an ordinary local. */
    register u32 held asm("r4");

    words = (u32)size + 3;
    address = state[1];
    /* FAKEMATCH: The best plain shape shifts before loading the heap head; retain the unshifted value and incoming size through that load. */
    __asm__("" : : "r"(address), "r"(words), "r"(size));
    words >>= 2;
    /* FAKEMATCH: After the head-order repair, the native bound load and final alignment shift remain swapped; capture the bound before that shift. */
    __asm__("" : "=r"(held), "+r"(words) : "0"((u32)(Ram_IwramHeapEnd - 1)));
    limit = held;
    size = (s32)(words << 2);
    next = address + (u32)size;
    if (next > limit) {
        address = state[0];
        other = address + (u32)size;
        if (other >= 0x02040000U) {
            return NULL;
        }
        state[0] = other;
        goto finish;
    }
    state[1] = next;
finish:
    return (void *)address;
}

/* EWRAM-first bump allocation. All-six trials: separate/reused rounding
   locals differed by 13 bytes; named words by 8; two order constraints
   matched the complete 52-byte owner, excluding the following raw tail. */
void *Runtime_BumpAllocateAlternatePool(s32 size)
{
    u32 *state = (u32 *)Ram_HeapSlots;
    u32 other;
    u32 next;
    u32 address;
    u32 words;

    words = (u32)size + 3;
    address = state[0];
    /* FAKEMATCH: The best plain shape shifts before loading the heap head; retain the unshifted value and incoming size through that load. */
    __asm__("" : : "r"(address), "r"(words), "r"(size));
    words >>= 2;
    size = (s32)(words << 2);
    next = address + (u32)size;
    if (next >= 0x02040000U) {
        address = state[1];
        other = address + (u32)size;
        if (other > (u32)(Ram_IwramHeapEnd - 1)) {
            return NULL;
        }
        state[1] = other;
        goto finish;
    }
    state[0] = next;
finish:
    return (void *)address;
}
