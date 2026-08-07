#include "types.h"

/*
 * resource_3ca DMA bank-flip leaf at 0x02000f30, complete 80-byte span through
 * its six-word pool. It selects one of two 1,920-byte runtime banks, publishes
 * the bank's first word directly, then configures DMA0 to stream the remainder
 * to the same display register.
 */

typedef struct {
    volatile u32 source;
    volatile u32 destination;
    volatile u32 control;
} DmaTransfer_02000f30;

extern u8 *Data_03001ed8;

void Func_02000f30(void)
{
    u8 *runtime = Data_03001ed8;
    u8 *bank = runtime + runtime[0x0f00] * 1920;
    volatile DmaTransfer_02000f30 *dma0 =
        (volatile DmaTransfer_02000f30 *)0x040000b0;

    *(volatile u16 *)0x040000ba &= 0xc5ff;
    *(volatile u16 *)0x040000ba &= 0x7fff;
    *(volatile u32 *)0x0400001c = *(u32 *)bank;
    dma0->source = (u32)(bank + 4);
    dma0->destination = 0x0400001c;
    dma0->control = 0xa6600001;
}
