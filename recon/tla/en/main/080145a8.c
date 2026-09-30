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

s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 order)
{
    u32 saved_interrupt_state;
    s32 index;
    s32 returned_index;
    struct SchedulerTask *task;
    volatile u8 *scheduler_status;
    s32 i;

    scheduler_status = &gSchedulerStatus;
    index = -1;
    task = gSchedulerTaskTable;
    (void)*scheduler_status;
    do {
        saved_interrupt_state = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            i = 0;
            if (task->callback == callback) {
                task->state = order;
                index = 0;
            } else {
            find_existing:
                i++;
                task++;
                if (i <= 19) {
                    if (task->callback == callback) {
                        task->state = order;
                        index = i;
                    } else {
                        goto find_existing;
                    }
                }
            }
            task = gSchedulerTaskTable;
            if (index == -1) {
                i = 0;
                if (task->callback == 0) {
                    task->callback = callback;
                    task->state = order;
                    task->mask = 0;
                    index = 0;
                } else {
                find_empty:
                    i++;
                    task++;
                    if (i <= 19) {
                        if (task->callback == 0) {
                            task->callback = callback;
                            task->state = order;
                            task->mask = 0;
                            index = i;
                        } else {
                            goto find_empty;
                        }
                    }
                }
            }
        } while (0);
        Scheduler_SortTasks();
        REG_IME = saved_interrupt_state;
        returned_index = index;
    } while (0);
    return returned_index;
}
