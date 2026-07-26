#include "types.h"

s32 Func_08002f40(s32);
void *Func_08005340(s32, s32);
void *Func_08004938(u32);
void Func_080072fc(s32, void *);
void Func_08002df0(void *);

extern u8 Data_08002d5c;
extern u8 Value_0000007c;

void Func_08002fb0(s32 first, s32 second)
{
    void *handle;
    u32 size;
    void *buffer;
    u32 control;
    u32 words;

    handle = Func_08005340(Func_08002f40(first), second);
    size = (u32)&Value_0000007c;
    buffer = Func_08004938(size);
    words = size >> 2;
    control = 0x84000000;
    {
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_08002d5c;
        u32 destination = (u32)buffer;

        control |= words;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
    }
    Func_080072fc(second, handle);
    Func_08002df0(buffer);
}
