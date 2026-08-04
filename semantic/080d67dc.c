#include "types.h"

struct RuntimePointers {
    u8 *base;
    u8 unknown04[120];
    void *clear_destination;
};

struct DisplayState {
    u8 unknown00[6];
    u16 field06;
};

struct TransferState {
    u8 unknown00[16];
    s32 delay;
};

struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
};

extern volatile struct RuntimePointers Data_03001e74;
extern struct DisplayState Data_03001ad0;
extern struct TransferState Data_03001ce0;

void Func_080b5040(s32, u16, s32);
void Func_080030f8(s32);

void Func_080d67dc(void)
{
    volatile u32 zero;
    void *clear_destination;
    u8 *base;
    volatile struct DmaTransfer *dma;
    u32 vram_destination;
    u32 vram_control;

    clear_destination = Data_03001e74.clear_destination;
    base = Data_03001e74.base;

    *(volatile u16 *)0x04000000 = 1;
    Data_03001ad0.field06 = 32;
    Func_080b5040(1, *(u16 *)(base + 0x648), 24);

    zero = 0;
    dma = (volatile struct DmaTransfer *)0x040000d4;
    dma->source = (const void *)&zero;
    dma->destination = clear_destination;
    dma->control = 0x85001000;

    zero = 0;
    vram_destination = 0x06004000;
    vram_control = 0x85001000;
    dma->source = (const void *)&zero;
    dma->destination = (void *)vram_destination;
    dma->control = vram_control;

    Func_080030f8(1);

    *(volatile u16 *)0x04000052 = 0x100e;
    *(volatile u16 *)0x04000050 = 0x3f46;
    *(volatile u16 *)0x04000000 = 0x7741;
    Data_03001ce0.delay = 120;
}
