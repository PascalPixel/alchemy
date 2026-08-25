#include "types.h"

#define Scheduler_EnableCallbacks Func_080042c8

struct SchedulerTaskEntry {
    u32 callback;
    u8 state_low;
    u8 enabled : 1;
    u8 state_high_rest : 7;
    u8 mask;
    u8 pad;
};

extern struct SchedulerTaskEntry Data_03001a20[20];
extern volatile u16 Data_04000208;

s32 Scheduler_EnableCallbacks(u32 callback)
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
        if (callback != 0) {
            if (task->callback != callback)
                continue;
        }
        task->enabled = 1;
        result = i;
    }
    Data_04000208 = saved_interrupt_master;
    return result;
}
