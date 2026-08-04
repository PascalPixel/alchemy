#include "types.h"

typedef void (*CopiedCode)(void);

extern char Value_000004e8;

void *Func_08004938(s32 size);
void Func_08002df0(void *buffer);

void Func_0800d304(void)
{
    s32 size = (s32)&Value_000004e8;
    void *buffer;

    buffer = Func_08004938(size);
    {
        u32 words = (u32)size >> 2;
        u32 control = 0x84000000;
        u32 *dma = (u32 *)0x040000d4;
        u32 source = 0x0800a494;
        u32 destination = (u32)buffer;

        control |= words;
        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
    }
    ((CopiedCode)buffer)();
    Func_08002df0(buffer);
}
