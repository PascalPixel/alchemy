#include "DMA.H"

/* The table size is a link-time symbol, loaded from the literal pool. */
extern u8 Value_0000007c;


void *Runtime_AllocateBlock(s32 slot, s32 size);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void PaletteDma_LoadBlock(void);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);

/* Allocates and clears the object and state blocks (from the heap in mode
   3), loads the object graphics and copies the 0x7c-byte object table. */
void ObjectSystem_Configure(s32 mode)
{
    void *objects;
    void *states;
    void *table;
    u32 size;
    volatile u32 zero;

    if (mode == 3) {
        objects = Runtime_AllocateBlock(4, 0xe00);
        states = Runtime_AllocateBlock(3, 0x600);
    } else {
        objects = Runtime_AllocateHeapBlock(4, 0xe00);
        states = Runtime_AllocateHeapBlock(3, 0x600);
    }
    PaletteDma_LoadBlock();
    zero = 0;
    Dma_Set((const void *)&zero, objects, 0x85000380, (volatile u32 *)0x040000d4);
    zero = 0;
    Dma_Set((const void *)&zero, states, 0x85000180, (volatile u32 *)0x040000d4);
    VramBlock_LoadCached(93, 128, (const void *)0x08012f20);
    size = (u32)&Value_0000007c;
    table = Runtime_AllocateHeapBlock(53, size);
    Dma_Set((const void *)0x0800a418, table, 0x84000000 | (size >> 2),
            (volatile u32 *)0x040000d4);
}
