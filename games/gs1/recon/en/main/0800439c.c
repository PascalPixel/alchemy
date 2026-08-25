#include "types.h"

#define Scheduler_DisableCallbacks Func_0800439c

struct SchedulerTaskEntry {
    u32 callback;
    u8 state_low;
    u8 state_high;
    u8 mask;
    u8 pad;
};

extern struct SchedulerTaskEntry Data_03001a20[20];
extern volatile u16 Data_04000208;

s32 Scheduler_DisableCallbacks(u32 callback)
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
        if (callback == 0 || task->callback == callback) {
            task->state_high &= (u8)~1;
            result = i;
        }
    }
    Data_04000208 = saved_interrupt_master;
    return result;
}
