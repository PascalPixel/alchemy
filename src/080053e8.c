#include "types.h"

typedef u32 (*Decompressor)(const void *source, void *destination);

void *Func_08004938(u32 size);
void Func_08002df0(void *buffer);

extern u8 Data_08001b70;
extern u8 Value_00000258;

u32 Func_080053e8(const void *source, void *destination)
{
    u32 size = (u32)&Value_00000258;
    void *buffer = Func_08004938(size);
    u32 words = size >> 2;
    u32 control = 0x84000000;
    u32 result;

    control |= words;
    {
        u32 *dma = (u32 *)0x040000d4;
        u32 sourceAddress = (u32)&Data_08001b70;
        u32 destinationAddress = (u32)buffer;

        dma[0] = sourceAddress;
        dma[1] = destinationAddress;
        dma[2] = control;
    }
    result = ((Decompressor)buffer)(source, destination);
    Func_08002df0(buffer);
    return result;
}
