#include "types.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "io_reg.h"
#include "mem.h"
#include "palqueue.h"

#define gPaletteWork ((struct PaletteSnapshotWork *)Ram_HeapSlots->unknown_70[0])

s32 PaletteQueue_Add(s16 bank, s16 index, s16 delay, s16 count)
{
    struct PaletteSnapshotWork *work;
    struct PaletteSnapshot *entry;
    u16 *source;
    u32 slot;
    u16 size;

    work = gPaletteWork;
    slot = work->count;
    if (slot > 3)
        return -1;
    entry = &work->entries[slot];
    source = (u16 *)BG_PLTT + (((u16)bank << 4) + (u16)index);
    size = count;
    entry->position = 0;
    entry->timer = 0;
    entry->count = size;
    entry->source = source;
    entry->delay = delay;
    Dma_Set(source, entry->colors, 0x80000000 | size, REG_DMA3);
    work->count++;
    return 0;
}

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
