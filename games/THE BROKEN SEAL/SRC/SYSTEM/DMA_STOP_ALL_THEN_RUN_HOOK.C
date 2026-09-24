#include "DMA.H"

/* The container-built _call_via_r0 veneer: it calls the routine in r0. */
void Func_080072e4(u32 routine);


/* Writes three empty transfers to the DMA3 registers, then runs the hook
   stored at 0x030000c4. */
void Dma_StopAllThenRunHook(void)
{
    Dma_Set(0, 0, 0x84000000, (volatile u32 *)0x040000d4);
    Dma_Set(0, 0, 0x84000000, (volatile u32 *)0x040000d4);
    Dma_Set(0, 0, 0x84000000, (volatile u32 *)0x040000d4);
    Func_080072e4(*(u32 *)0x030000c4);
}
