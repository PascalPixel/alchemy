#include "types.h"

struct DmaTransfer_080907b0 {
    const void *source;
    void *destination;
    u32 control;
};

struct State_080907b0 {
    u8 data[0x508];
    u32 palette[8];
};

extern struct State_080907b0 *Data_03001ecc;

void Func_080907b0(s32 color)
{
    struct State_080907b0 *state = Data_03001ecc;
    volatile u32 *dma;
    u32 source;
    u32 destination;
    u32 control;
    u32 clear = 0xf000f000;

    dma = (volatile u32 *)0x040000d4;
    source = (u32)&clear;
    destination = 0x06002000;
    control = 0x85000140;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;

    if (color != -1) {
        u32 packed = 0;
        s32 index = 7;

        do {
            packed = (packed << 4) | color;
            index--;
        } while (index >= 0);

        index = 7;
        do {
            state->palette[7 - index] = packed;
            index--;
        } while (index >= 0);

        source = (u32)state->palette;
        destination = 0x06000000;
        control = 0x84000008;
        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
    }
}
