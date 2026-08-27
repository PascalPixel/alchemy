#include "types.h"
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
        struct DmaChannel *dma = (struct DmaChannel *)0x040000d4;

        dma->source = &fill;
        dma->destination = argument;
        dma->control = 0x810000a0;
    } else {
        void *buf = (void *)Func_08004938(0x214);
        struct DmaChannel *dma = (struct DmaChannel *)0x040000d4;

        dma->source = (const void *)0x080158e8;
        dma->destination = buf;
        dma->control = (0x214 >> 2) | 0x84000000;

        ((Routine)routine)(argument);
        Func_08002df0(buf);
    }
}
