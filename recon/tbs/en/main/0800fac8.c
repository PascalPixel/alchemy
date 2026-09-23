/* Draft, not exact: 54 differing halfwords, 116-byte candidate for the
   112-byte owner (2026-09-23). Residual: 0x02010000 is kept in r8 across the
   IWRAM copy instead of being reloaded, so the saved buffer and the routine
   move to r6 and r5 and the code size to r7; wrapping the copy and the size
   load does not stop that. */

#include "DMA.H"
#include "RUNTIME_MEM.H"

typedef void (*WordCopyFn)(void *destination, const void *source, s32 size);

void *Runtime_BumpAllocateAlternatePool(s32 size);

extern u8 Data_0800a37c[];
extern u8 Data_0000009c[];

/* Converts the map at 0x02010000 into 0x02018000 with the ARM routine at
   0x0800a37c, run from a heap copy, against a saved copy of the source. */
void Tilemap_ConvertBuffer(void)
{
    u8 *saved;
    void (*routine)(void *, const void *, const void *);
    u32 size;

    size = 0x8000;
    saved = (u8 *)Runtime_BumpAllocateAlternatePool(size);
    do {
        ((WordCopyFn)0x03001388)(saved, (const void *)0x02010000, size);
    } while (0);
    size = (u32)Data_0000009c;
    routine = (void (*)(void *, const void *, const void *))Runtime_BumpAllocate(size);
    Dma_Set(Data_0800a37c, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    routine((void *)0x02018000, (const void *)0x02010000, saved);
    Sys_Free(routine);
    Sys_Free(saved);
}
