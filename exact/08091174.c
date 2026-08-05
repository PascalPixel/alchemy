#include "types.h"

void *Func_080048f4(s32, s32);
void Func_08090a5c(s32, s32, s32, s32);
void Func_080041d8(s32, s32);

struct DmaTransfer {
    u32 source;
    u32 destination;
    u32 control;
};

void Func_08091174(void)
{
    s32 operation;
    s32 zero;
    u32 descriptor_value;
    u32 third_destination;
    void *buffer;
    s32 *dma_source;
    volatile struct DmaTransfer *dma;

    buffer = Func_080048f4(0x20, 0x2a04);
    dma_source = &zero;
    *dma_source = 0;
    dma = (volatile struct DmaTransfer *)0x040000d4;
    dma->source = (u32)dma_source;
    dma->destination = (u32)buffer;
    dma->control = 0x85000a81;
    dma->source = 0x05000000;
    dma->destination = (u32)buffer;
    descriptor_value = 0x84000070;
    dma->control = descriptor_value;
    descriptor_value = 0x1c0;
    third_destination = (u32)buffer + descriptor_value;
    dma->source = 0x05000200;
    dma->destination = third_destination;
    descriptor_value = 0x84000070;
    dma->control = descriptor_value;
    Func_08090a5c(0x10000, (s32)buffer, (s32)buffer + 0xe00, 0);
    operation = 0xc8f;
    Func_080041d8(0x080908e1, operation);
}
