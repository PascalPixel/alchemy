#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"

void BattleFx_BuildBuffer(s32, s32, s32, s32);

void BattleEffect_InitializeBuffers(void)
{
    volatile s32 zero;
    void *buffer;

    buffer = Runtime_AllocateBlock(0x20, 0x2a04);
    zero = 0;
    Dma_Set(&zero, buffer, 0x85000a81, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000000, buffer, 0x84000070, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000200, (u8 *)buffer + 0x1c0, 0x84000070,
            (volatile u32 *)0x040000d4);
    BattleFx_BuildBuffer(0x10000, (s32)buffer, (s32)buffer + 0xe00, 0);
    Scheduler_AddOrUpdateCallback((void (*)(void))0x080908e1, 0xc8f);
}
