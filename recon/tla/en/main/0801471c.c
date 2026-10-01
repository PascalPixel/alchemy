/* Near miss: score 60. ☀️'s, for ⚓️'s 24 tasks. ⚓️ loads the task table
   (ldr r1) straight after copying mask into r6; this draft sets the -1
   result first. Declaration orders, a byte mask and 40 s of permuting did
   not fix it; its neighbours matched once the table load came first. */
#include "TYPES.H"
#include "IO_REG.H"

struct SchedulerTask {
    u32 callback;
    u16 state;
    u8 mask;
    u8 reserved;
};

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->state)[1])
extern volatile u8 gSchedulerStatus;
extern u8 gSchedulerTaskCount;
extern struct SchedulerTask gSchedulerTaskTable[24];

s32 Scheduler_SetCallbackMask(u32 callback, u32 mask)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 returned_result;
    s32 i;

    task = gSchedulerTaskTable;
    result = -1;
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 23; i++, task++) {
                if (task->callback == callback) {
                    task->mask = mask;
                    result = i;
                    break;
                }
            }
        } while (0);
        REG_IME = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}
