#include "types.h"

typedef u32 (*DecodeFunc)(const void *src, void *dst);

void *Func_08004938(u32 size);
void Func_08002df0(void *buffer);

extern u8 Data_08001b70;
extern u8 Value_00000258;

u32 Func_080053e8(const void *src, void *dst)
{
    u32 size = (u32)&Value_00000258;
    void *buffer = Func_08004938(size);
    u32 words = size >> 2;
    u32 control = 0x84000000;
    u32 result;

    control |= words;
    {
        volatile u32 *dma = (volatile u32 *)0x040000d4;
        u32 srcAddr = (u32)&Data_08001b70;
        u32 dstAddr = (u32)buffer;

        dma[0] = srcAddr;
        dma[1] = dstAddr;
        dma[2] = control;
    }
    result = ((DecodeFunc)buffer)(src, dst);
    Func_08002df0(buffer);
    return result;
}
