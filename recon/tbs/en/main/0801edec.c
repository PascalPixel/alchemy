#include "TYPES.H"
#include "DMA.H"
#include "video_dma_family.h"

extern u8 *Data_03001e8c;

u32 Func_08004938(s32 size);
void Func_08002df0(void *allocation);

typedef void (*Routine)(void *argument);

void Func_0801edec(void *argument)
{
    void *routine = Data_03001e8c;

    if (routine == 0) {
        u16 fill = 0xe0e0;
        Dma_Set(&fill, argument, 0x810000a0, (volatile u32 *)0x040000d4);
    } else {
        void *buf = (void *)Func_08004938(0x214);
        Dma_Set((const void *)0x080158e8, buf, (0x214 >> 2) | 0x84000000,
                (volatile u32 *)0x040000d4);

        ((Routine)routine)(argument);
        Func_08002df0(buf);
    }
}
