#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
extern u8 Func_0800a418[];

/* The strided tile copy runs from a heap copy of itself. */
extern u8 Tile_CopyStridedCodeSize[];

/* The 16 by 8 oval, in colour 1, that objects draw beneath them. */
extern const u8 Object_ShadowTiles[];


void PaletteDma_LoadBlock(void);

/* Allocates and clears the object and state blocks (from the heap in mode
   3), loads the object graphics and copies the strided tile copy routine
   into heap block 53. */

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
    VramBlock_LoadCached(93, 128, Object_ShadowTiles);
    size = (u32)Tile_CopyStridedCodeSize;
    table = Runtime_AllocateHeapBlock(53, size);
    Dma_Set((const void *)Func_0800a418, table, 0x84000000 | (size >> 2),
            (volatile u32 *)0x040000d4);
}
