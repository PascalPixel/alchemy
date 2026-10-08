#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "mem.h"
#include "callback_scheduler.h"

void Func_08108030(void *panel);

struct ScreenWork {
    u8 unknown_000[0x5a4];
    u8 panel[0x84];
};

void Func_08040c60(void)
{
    struct ScreenWork *work = Ram_HeapSlots->unknown_d0[0];

    Func_08108030(work->panel);
}

void Func_08040c78(void)
{
    void *work = Runtime_AllocateBlock(208, 0x628);

    Dma_Fill32(0, work, 0x628);
    Scheduler_AddOrUpdateCallback(Func_08040c60, 1152);
}

void Func_08040cb0(void)
{
    Scheduler_RemoveCallback(Func_08040c60);
    Runtime_ReleaseHeapBlock(208);
}
