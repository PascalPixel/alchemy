#include "CALLBACK_SCHEDULER.H"

struct SchedulerTask {
    u32 callback;
    u16 state;
    u8 mask;
    u8 reserved;
};

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->state)[1])

extern volatile u8 gSchedulerStatus;
extern u8 gSchedulerTaskCount;
extern struct SchedulerTask gSchedulerTaskTable[20];

/*
 * Each table update masks interrupts by writing IME the low half of its own
 * address (0x208, bit 0 clear) and restores the saved value afterwards.
 */

void Scheduler_ResetTaskTable(void)
{
    struct SchedulerTask *task = gSchedulerTaskTable;
    s32 remaining = ((u32)task | ~(u32)task) + 1;
    gSchedulerTaskCount = remaining;
    gSchedulerStatus = remaining;
    {
        u32 zero = 0;
        remaining = 19;
        do {
            task->callback = zero;
            task->state = 0xffff;
            task->mask = zero;
            task++;
            remaining--;
        } while (remaining >= 0);
    }
    gSchedulerTaskCount = 1;
}
