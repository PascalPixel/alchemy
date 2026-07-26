#include "types.h"

typedef void (*CopiedCode)(s32 first, s32 second);

void *Func_08004938(u32 size);
void Func_08002df0(void *buffer);

extern u8 Data_08015570;
extern u8 Value_00000060;

void Func_08019bac(s32 first, s32 second)
{
    u32 size = (u32)&Value_00000060;
    void *buffer = Func_08004938(size);
    u32 words = size >> 2;
    u32 control = 0x84000000;

    control |= words;
    {
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_08015570;
        u32 destination = (u32)buffer;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
    }
    ((CopiedCode)buffer)(first, second);
    Func_08002df0(buffer);
}
