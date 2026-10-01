#include "TYPES.H"
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->state)[1])
extern volatile u8 gSchedulerStatus;
extern u8 gSchedulerTaskCount;
extern struct SchedulerTask gSchedulerTaskTable[24];

s32 Scheduler_DisableCallbacks(u32 callback)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    task = gSchedulerTaskTable;
    result = -1;
    /* FAKEMATCH: the two blocks that run once are meaningless. Without either, the
     * interrupt-master register's address is loaded before the task table's. */
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 23; i++, task++) {
                if (callback == 0 || task->callback == callback) {
                    TASK_STATE_HIGH(task)&= (u8)~1;
                    result = i;
                }
            }
        } while (0);
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

s32 Scheduler_DisableOverlayCallbacks(void)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    task = gSchedulerTaskTable;
    result = -1;
    /* FAKEMATCH: the two blocks that run once, as in Scheduler_DisableCallbacks */
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 23; i++, task++) {
                if ((task->callback >> 24) == 2) {
                    TASK_STATE_HIGH(task)&= (u8)~1;
                    result = i;
                }
            }
        } while (0);
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

