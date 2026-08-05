#include "types.h"

struct DmaTransfer {
    u32 source;
    u32 destination;
    u32 control;
};

/* Resolved whole-owner call targets. */


/*
 * Capture the two hardware-palette banks into their non-contiguous work-buffer
 * slots, then publish the completed buffer.
 *
 * Owner span: 0x02000f74-0x02000fb4.  Its literal pool begins at 0x02000fa4.
 */
extern void Func_020020aa();
void Func_02000f74(void)
{
    volatile struct DmaTransfer *const dma3 =
        (volatile struct DmaTransfer *)0x040000d4;
    u32 destination = *(volatile u32 *)0x03001ed0;

    dma3->source = 0x05000000;
    dma3->destination = destination;
    dma3->control = 0x84000070;

    dma3->source = 0x05000200;
    dma3->destination = destination + 0x1c0;
    dma3->control = 0x84000070;

    Func_020020aa(0x10000, 0);
}
