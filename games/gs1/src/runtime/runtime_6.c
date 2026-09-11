#include "types.h"
#include "scene.h"

/* runtime/memory/allocate_and_clear_queue.c */
struct SubQueueItem_08011b00 {
    u32 unknown_00;
    u16 unknown_04;
    u16 unknown_06;
    u16 unknown_08;
    u16 unknown_0a;
    u16 unknown_0c[16];
};

struct Queue_08011b00 {
    struct SubQueueItem_08011b00 slots[4];
    u16 count;
};

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

void Runtime_AllocateAndClearQueue(void)
{
    struct Queue_08011b00 *queue;
    struct SubQueueItem_08011b00 *entry;
    u16 i;
    u16 j;

    queue = (struct Queue_08011b00 *)Runtime_AllocateBlock(28, sizeof(struct Queue_08011b00));
    entry = queue->slots;
    for (i = 0; i != 4; i++) {
        entry->unknown_00 = 0;
        entry->unknown_04 = 0;
        entry->unknown_06 = 0;
        entry->unknown_08 = 0;
        entry->unknown_0a = 0;
        for (j = 0; j != 16; j++) {
            entry->unknown_0c[j] = 0;
        }
        entry++;
    }
    queue->count = 0;
}

/* runtime/memory/schedule_callback_and_release_block_28.c */
extern u8 gRom;

void ScheduleCallback(void *);

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
    ScheduleCallback(&gRom);
    Sys_Do(0x1C);
}

/* runtime/scheduler/schedule_callback_b_after_frames.c */
s32 ScheduleCallbackAfterFrames(s32, s32);
extern u8 gRom;

void Scheduler_ScheduleCallbackBAfterFrames(void)
{
    ScheduleCallbackAfterFrames((s32)&gRom, 0xc80);
}
