#include "types.h"

extern void *Data_03001e8c;
extern u8 Data_080158e8;
extern u8 Value_00000214;

void *Func_08004938(u32);
void Func_080072fc(s32, void *);
void Func_08002df0(void *);

void Func_0801edec(s32 destination)
{
    void *handle = Data_03001e8c;

    if (handle == NULL) {
        u16 fill = 0xe0e0;
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&fill;
        u32 control = 0x810000a0;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
    } else {
        u32 size = (u32)&Value_00000214;
        void *buffer = Func_08004938(size);
        u32 words = size >> 2;
        u32 control = 0x84000000;
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_080158e8;
        u32 copied_destination = (u32)buffer;

        control |= words;
        dma[0] = source;
        dma[1] = copied_destination;
        dma[2] = control;

        Func_080072fc(destination, handle);
        Func_08002df0(buffer);
    }
}
