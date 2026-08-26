#include "types.h"

struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
};

extern u8 *Data_03001ed0;

#define DMA3 (*(volatile struct DmaTransfer *)0x040000d4)

void Func_02000b8c(void)
{
    DMA3.source = (const void *)Data_03001ed0;
    DMA3.destination = (void *)0x020090b0;
    DMA3.control = 0x840000e0;
}
