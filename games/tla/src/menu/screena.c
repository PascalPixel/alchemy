#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "mem.h"
#include "callback_scheduler.h"

void Func_08108030(void *panel);

void Func_080450fc(s32 value);

struct ScreenWork {
    u8 unk000[0x574];
    u16 page;
    u8 unk576[0x2e];
    u8 panel[0x6c];
    s32 values[1];
};

void Func_080405ac(void)
{
    struct ScreenWork *work = Ram_HeapSlots->unkd0[0];

    asm volatile("" : "+l"(work)); /* FAKEMATCH: ⚓️ loads the screen work before it builds the panel offset */
    Func_08108030(work->panel);
    Func_080450fc(work->values[work->page]);
}

void Func_080405dc(void)
{
    void *work = Runtime_AllocateBlock(208, 0x628);

    Dma_Fill32(0, work, 0x628);
    Scheduler_AddOrUpdateCallback(Func_080405ac, 1152);
}

void Func_08040614(void)
{
    Scheduler_RemoveCallback(Func_080405ac);
    Runtime_ReleaseHeapBlock(208);
}
