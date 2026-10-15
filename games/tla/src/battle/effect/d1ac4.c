#include "types.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "io_reg.h"
#include "iowrite.h"
#include "mem.h"
#include "object_runtime.h"
#include "party_state.h"
#include "resource.h"

void Func_080d1840(void);

// Clears the overlay tiles to colour 1, sets the blend registers and starts
// Func_080d1840 at priority 0x480 on a 28-byte work block whose last word is
// the object (the selected owner's when none is given).
void Func_080d1a18(void *object)
{
    u32         *work;
    u8          *tiles;
    volatile u32 fill;

    work = Runtime_AllocateBlock(144, 28);
    tiles = Runtime_AllocateHeapBlock(56, 0x400);
    Dma_Fill32Cell(fill, 0x11111111, tiles, 0x200);
    VramBlock_LoadCached(94, 0x200, tiles);
    Runtime_ReleaseHeapBlock(56);
    Scheduler_AddOrUpdateCallback(Func_080d1840, 0x480);
    Io_Write16(0x3f9e, &REG_BLDCNT);
    Io_Write16(16, &REG_BLDALPHA);
    Io_Write16(31, &REG_BLDY);
    Dma_Fill32Cell(fill, 0, work, 28);
    if (object == NULL)
        object = ObjectTable_Get(gPartyState.currentOwner);
    work[6] = (u32)object;
}

void Func_080d1ac4(void)
{
    Scheduler_EnableCallbacks(Func_080d1840);
}

void Func_080d1ad4(void)
{
    Scheduler_DisableCallbacks(Func_080d1840);
}
