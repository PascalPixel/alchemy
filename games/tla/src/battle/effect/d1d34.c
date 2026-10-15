#include "types.h"
#include "dma.h"

s32  Scheduler_EnableCallbacks();
void Func_080d1ae4();

void Func_080d1d34(void)
{
    Scheduler_EnableCallbacks(Func_080d1ae4);
}

s32 Scheduler_DisableCallbacks();

void Func_080d1d44(void)
{
    Scheduler_DisableCallbacks(Func_080d1ae4);
}

void *Runtime_AllocateHeapBlock();
void  Runtime_ReleaseHeapBlock();
void  VramBlock_LoadCached();

void Func_080d1d54(s32 solid)
{
    void *buffer = Runtime_AllocateHeapBlock(56, 0x400);

    if (solid == 0)
    {
        Dma_Fill32(0, buffer, 0x200);
    }
    else
    {
        Dma_Fill32(0x01010101, buffer, 0x200);
    }
    VramBlock_LoadCached(94, 0x200, buffer);
    Runtime_ReleaseHeapBlock(56);
}
