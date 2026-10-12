#include "types.h"
#include "callback_scheduler.h"
#include "io_reg.h"

void Func_080d5ff8(void);
void Func_080d607c(void);

void Func_080d64b8(void)
{
    Scheduler_RemoveCallback(Func_080d5ff8);
    Scheduler_RemoveCallback(Func_080d607c);
    {
        volatile u16 *dma = REG_DMA0;

        dma[5] &= 0xc5ff; // 開始タイミングとリピートを消す
        dma[5] &= 0x7fff; // 止める
        (void)dma[5];
    }
}
