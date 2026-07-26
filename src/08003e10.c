#include "types.h"

u32 Func_08004938(s32 arg0);
void Func_080072fc(s32);
void Func_08002df0(void *);

extern u8 Data_08001dc8;
extern u8 Value_000000e0;

void Func_08003e10(s32 request)
{
    u32 size = (u32)&Value_000000e0;
    void *buffer = Func_08004938(size);
    u32 words = size >> 2;
    u32 control = 0x84000000;

    control |= words;
    {
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_08001dc8;
        u32 destination = (u32)buffer;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
    }
    Func_080072fc(request);
    Func_08002df0(buffer);
}
