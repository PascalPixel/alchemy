#include "types.h"

struct FieldPaletteDmaTransfer {
    u32 source;
    u32 destination;
    u32 control;
};

void *Func_080048f4(s32, s32);
void Func_08090a5c(s32, s32, s32, s32);
void Func_080041d8(void (*)(void), s32);

#define FieldPalette_InitializeBuffers Func_08091174

void FieldPalette_InitializeBuffers(void)
{
    s32 operation;
    s32 zero;
    u32 descriptor_value;
    u32 third_destination;
    void *buffer;
    s32 *dma_source;
    volatile struct FieldPaletteDmaTransfer *dma;

    buffer = Func_080048f4(0x20, 0x2a04);
    dma_source = &zero;
    *dma_source = 0;
    dma = (volatile struct FieldPaletteDmaTransfer *)0x040000d4;
    *dma = (struct FieldPaletteDmaTransfer){
        (u32)dma_source, (u32)buffer, 0x85000a81
    };
    descriptor_value = 0x84000070;
    *dma = (struct FieldPaletteDmaTransfer){
        0x05000000, (u32)buffer, descriptor_value
    };
    descriptor_value = 0x1c0;
    third_destination = (u32)buffer + descriptor_value;
    descriptor_value = 0x84000070;
    *dma = (struct FieldPaletteDmaTransfer){
        0x05000200, third_destination, descriptor_value
    };
    Func_08090a5c(0x10000, (s32)buffer, (s32)buffer + 0xe00, 0);
    operation = 0xc8f;
    Func_080041d8((void (*)(void))0x080908e1, operation);
}
