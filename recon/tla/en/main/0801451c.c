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

void Scheduler_SortTasks(void)
{
    struct SchedulerTask saved;
    struct SchedulerTask *base = gSchedulerTaskTable;
    struct SchedulerTask *task;
    s32 pass = 19;
    s32 remaining;
    goto sort_pass;
next_pass:
    base = gSchedulerTaskTable;
sort_pass:
    task = base;
    if (pass <= 0)
        goto finish_pass;
    remaining = pass;
next_task:
    if ((s16)task[1].state > (s16)task->state) {
        __builtin_memcpy(&saved, task, sizeof(saved));
        base = task;
        task++;
        __builtin_memcpy(base, task, sizeof(saved));
        __builtin_memcpy(task, &saved, sizeof(saved));
    } else {
        task++;
    }
    if (--remaining != 0)
        goto next_task;
finish_pass:
    if (--pass > 1)
        goto next_pass;
}
