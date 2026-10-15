#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "mem.h"
#include "callback_scheduler.h"

void Func_08108030(void *panel);

struct ScreenWork {
    u8 unk000[0x5a4];
    u8 panel[0x84];
};

void Func_0804098c(void)
{
    struct ScreenWork *work = Ram_HeapSlots->unkd0[0];

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
