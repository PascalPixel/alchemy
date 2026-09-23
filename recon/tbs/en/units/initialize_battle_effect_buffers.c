#include "DMA.H"

void *Func_080048f4(s32, s32);
void Func_08090a5c(s32, s32, s32, s32);
s32 Func_080041d8(void (*)(void), s32);

#define BattleFx_InitializeBuffers Func_08091174

void BattleFx_InitializeBuffers(void)
{
    volatile s32 zero;
    void *buffer;

    buffer = Func_080048f4(0x20, 0x2a04);
    zero = 0;
    Dma_Set(&zero, buffer, 0x85000a81, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000000, buffer, 0x84000070, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000200, (u8 *)buffer + 0x1c0, 0x84000070,
            (volatile u32 *)0x040000d4);
    Func_08090a5c(0x10000, (s32)buffer, (s32)buffer + 0xe00, 0);
    Func_080041d8((void (*)(void))0x080908e1, 0xc8f);
}
