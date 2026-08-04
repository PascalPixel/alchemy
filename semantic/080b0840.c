#include "types.h"

struct DmaTransfer_080b0840 {
    const void *source;
    void *destination;
    u32 control;
};

struct RuntimeState_080b0840 {
    u8 *base;
    u8 unused[0x10];
    u8 *tiles;
};

extern struct RuntimeState_080b0840 Data_03001ebc;

void Func_0808a330(s32, s32);
void Func_0808a348(s32);

void Func_080b0840(s32 arg0)
{
    volatile struct DmaTransfer_080b0840 *dma =
        (volatile struct DmaTransfer_080b0840 *)0x040000d4;
    u8 *source = Data_03001ebc.tiles + 0xe00;

    dma->source = source;
    dma->destination = Data_03001ebc.base + 0x236;
    dma->control = 0x84000150;
    dma->source = source;
    dma->destination = Data_03001ebc.tiles + 0x380;
    dma->control = 0x840002a0;
    Func_0808a330(arg0, 1);
    Func_0808a348(0x10);
}
