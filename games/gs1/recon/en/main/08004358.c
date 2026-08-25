#include "types.h"

#define Scheduler_SetCallbackMask Func_08004358

struct SchedulerTaskEntry {
    u32 callback;
    u8 state_low;
    u8 state_high;
    u8 mask;
    u8 pad;
};

extern struct SchedulerTaskEntry Data_03001a20[20];
extern volatile u16 Data_04000208;

s32 Scheduler_SetCallbackMask(u32 callback, u32 mask)
{
    struct SchedulerTaskEntry *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    result = -1;
    task = Data_03001a20;
    saved_interrupt_master = Data_04000208;
    Data_04000208 = (u16)&Data_04000208;
    for (i = 0; i <= 19; i++, task++) {
        if (task->callback == callback) {
            task->mask = mask;
            result = i;
            break;
        }
    }
    Data_04000208 = saved_interrupt_master;
    return result;
}
