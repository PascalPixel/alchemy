#include "types.h"

struct DmaSetup {
    u32 source;
    u32 destination;
    u32 control;
};

void Func_08011568(void)
{
    u32 source = 0x02010000;
    u32 destination = 0x06006A00;
    u32 control = 0x84002580;
    struct DmaSetup *dma;

    *(u16 *)0x0400000A = 0x682;
    dma = (struct DmaSetup *)0x040000D4;
    dma->source = source;
    dma->destination = destination;
    dma->control = control;
}
