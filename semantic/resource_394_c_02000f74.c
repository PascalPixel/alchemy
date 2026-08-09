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
    u32 destination = *(volatile u32 *)0x03001ed0;

    *(volatile struct DmaTransfer *)0x040000d4 = (struct DmaTransfer){
        0x05000000, destination, 0x84000070,
    };
    *(volatile struct DmaTransfer *)0x040000d4 = (struct DmaTransfer){
        0x05000200, destination + 0x1c0, 0x84000070,
    };

    Func_020020aa(0x10000, 0);
}
