#include "types.h"

struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
};

void *Func_080048f4(s32, s32);
void Func_08015ef4(void);
void Func_08019d0c(void);
s32 Func_080041d8(const void *, s32);
void Func_080173f4(void);

void Func_08015f30(void)
{
    u8 *state;

    state = Func_080048f4(15, 0x12fc);
    {
        u32 fill = 0;
        u32 *source = &fill;
        struct DmaTransfer *dma = (struct DmaTransfer *)0x040000d4;

        dma->source = source;
        dma->destination = state;
        dma->control = 0x850004bf;

        *(u8 *)(state + 0xea3) = 1;
        {
            u16 value = 99;
            *(u16 *)(state + 0x12b6) = value;
        }
        *(u8 *)(state + 0xea7) = 15;

        fill = 0xf000f000;
        dma->source = source;
        dma->destination = state;
        dma->control = 0x85000140;
    }

    Func_08015ef4();
    Func_08019d0c();
    Func_080041d8((const void *)0x080160fd, 0x480);
    Func_080173f4();
}
