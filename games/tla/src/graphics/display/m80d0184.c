#include "types.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "dma.h"

void Func_080cf78c(void);
void Func_080cf6fc(void);

void Func_080d0184(void)
{
    Scheduler_EnableCallbacks(Func_080cf78c);
    Scheduler_EnableCallbacks(Func_080cf6fc);
}

void *Func_080d019c(void)
{
    void *block = Runtime_AllocateBlock(124, 0x540);
    volatile u32 zero = 0;

    Dma_Set(&zero, block, 0x85000150, (volatile u32 *)0x040000d4);
    return block;
}
