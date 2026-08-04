#include "types.h"

struct DmaTransfer {
    u32 source;
    u32 destination;
    u32 control;
};

/* Capture the first hardware-palette bank into the current work buffer. */
void Func_02001224(void)
{
    volatile struct DmaTransfer *const dma3 =
        (volatile struct DmaTransfer *)0x040000d4;

    dma3->source = 0x05000000;
    dma3->destination = *(volatile u32 *)0x03001ed0;
    dma3->control = 0x84000070;
}
