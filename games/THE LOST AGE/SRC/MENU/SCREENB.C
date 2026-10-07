#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "DMA.H"
#include "IO_REG.H"
#include "RUNTIME_MEM.H"
#include "CALLBACK_SCHEDULER.H"

void Func_08108030(void *panel);

struct ScreenWork {
    u8 unknown_000[0x5a4];
    u8 panel[0x84];
};

void Func_0804098c(void)
{
    struct ScreenWork *work = Ram_HeapSlots->unknown_d0[0];

    Func_08108030(work->panel);
}

void Func_080409a4(void)
{
    void *work = Runtime_AllocateBlock(208, 0x628);

    Dma_Fill32(0, work, 0x628);
    Scheduler_AddOrUpdateCallback(Func_0804098c, 1152);
}

void Func_080409dc(void)
{
    Scheduler_RemoveCallback(Func_0804098c);
    Runtime_ReleaseHeapBlock(208);
}
