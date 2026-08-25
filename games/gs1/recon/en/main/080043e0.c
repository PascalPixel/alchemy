#include "types.h"

#define Scheduler_DisableOverlayCallbacks Func_080043e0

struct SchedulerTaskEntry {
    u8 callback_address[4];
    u8 state_low;
    u8 state_high;
    u8 mask;
    u8 pad;
};

extern struct SchedulerTaskEntry Data_03001a20[20];
extern volatile u16 Data_04000208;

s32 Scheduler_DisableOverlayCallbacks(void)
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
        if (task->callback_address[3] == 2) {
            task->state_high &= (u8)~1;
            result = i;
        }
    }
    Data_04000208 = saved_interrupt_master;
    return result;
}
