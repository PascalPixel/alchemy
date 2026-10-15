#include "types.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "io_reg.h"
#include "mem.h"
#include "palqueue.h"

#define gPaletteWork ((struct PaletteSnapshotWork *)Ram_HeapSlots->unknown_70[0])

#include "palqueue_add.c"

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
    Scheduler_RemoveCallback(PaletteQueue_StepCycles);
    Runtime_ReleaseHeapBlock(112);
}

void Scheduler_ScheduleCallbackBAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback(PaletteQueue_StepCycles, 0x480);
}

void PaletteQueue_StepCycles(void)
{
    struct PaletteSnapshotWork *work = gPaletteWork;
    u8 i;
    u16 count = work->count & 3;
    u16 buf[16];

    for (i = 0; i < count; i++) {
        struct PaletteSnapshot *cycle = &work->entries[i];
        if (cycle->timer == 0) {
            u16 start = cycle->position;
            u16 len = cycle->count;
            void *dest = cycle->source;
            u16 *src = cycle->colors;
            u8 j;
            for (j = len - start; j < len; j++)
                buf[j] = *src++;
            for (j = 0; j < len - start; j++)
                buf[j] = *src++;
            Dma_Set(buf, dest, 0x80000000 | len, REG_DMA3);
            start++;
            if (start >= len)
                start = 0;
            cycle->position = start;
            cycle->timer = cycle->delay;
        } else {
            cycle->timer--;
        }
    }
}
