#include "types.h"

struct DmaTransfer_080907b0 {
    u32 source;
    u32 destination;
    u32 control;
};

extern u8 *Data_03001ecc;

void Func_080907b0(s32 color)
{
    s32 value;
    u8 *state;
    volatile u32 clear;

    value = color;
    state = Data_03001ecc;
    {
        volatile u32 *source;
        volatile struct DmaTransfer_080907b0 *dma;

        source = &clear;
        *source = 0xf000f000;
        dma = (volatile struct DmaTransfer_080907b0 *)0x040000d4;
        dma->source = (u32)source;
        dma->destination = 0x06002000;
        dma->control = 0x85000140;
    }

    if (value != -1) {
        s32 count;
        u32 packed = 0;
        u32 *destination;

        count = 7;
        do {
            packed <<= 4;
            count--;
            packed |= value;
        } while (count >= 0);

        destination = (u32 *)(state + (161 << 3));
        count = 7;
        do {
            count--;
            *destination++ = packed;
        } while (count >= 0);

        {
            volatile struct DmaTransfer_080907b0 *dma;

            dma = (volatile struct DmaTransfer_080907b0 *)0x040000d4;
            dma->source = (u32)(state + (161 << 3));
            dma->destination = 0x06000000;
            dma->control = 0x84000008;
        }
    }
}
