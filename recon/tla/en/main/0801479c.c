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

s32 Scheduler_DisableOverlayCallbacks(void)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 returned_result;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 19; i++, task++) {
                if ((task->callback >> 24) == 2) {
                    TASK_STATE_HIGH(task)&= (u8)~1;
                    result = i;
                }
            }
        } while (0);
        REG_IME = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}
