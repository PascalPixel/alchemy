#include "types.h"

struct Dma3_080a22f4 {
    const void *source;
    void *destination;
    u32 control;
};

/*
 * Publish the working palette.  The object palette bank at 0x05000200 is
 * copied over background palette 14 (16 halfwords), then the single colour at
 * 0x050001e8 is copied over slot 14 of that bank.  Both are immediate DMA3
 * transfers, written as the register triple the hardware latches on the
 * control store.
 * Called from 0x080a47d6, 0x080a4846 and 0x080a9e36.
 */
void Func_080a22f4(void)
{
    volatile struct Dma3_080a22f4 *dma = (volatile struct Dma3_080a22f4 *)0x040000D4;
    u16 *palette = (u16 *)0x050001C0;

    dma->source = (const void *)0x05000200;
    dma->destination = palette;
    dma->control = 0x80000010;

    dma->source = (const void *)0x050001E8;
    dma->destination = palette + 14;
    dma->control = 0x80000001;
}
