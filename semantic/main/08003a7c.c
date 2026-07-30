#include "types.h"

typedef void (*QueuedTransferRunner)(void *queue, u16 count);

extern u16 Data_02002090;
extern u8 Data_08002cf4;
extern u8 Value_00000068;

void Func_08003a7c(void)
{
    u16 count = Data_02002090;

    if (count != 0) {
        u32 words = (u32)&Value_00000068 >> 2;
        u32 executable[words];
        volatile u32 *dma = (volatile u32 *)0x040000d4;
        u32 source = (u32)&Data_08002cf4;
        u32 destination = (u32)executable;
        u32 control = 0x84000000 | words;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
        ((QueuedTransferRunner)executable)(&Data_02002090, count);
        Data_02002090 = 0;
    }
}
