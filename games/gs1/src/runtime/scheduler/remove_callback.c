#include "types.h"

#define Scheduler_RemoveCallback Func_08004278

struct SchedulerTaskEntry {
    u32 callback;
    u16 state;
    u8 mask;
    u8 pad;
};

extern struct SchedulerTaskEntry Data_03001a20[20];
extern volatile u16 Data_04000208;

s32 Scheduler_RemoveCallback(u32 callback)
{
    struct SchedulerTaskEntry *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 returned_result;
    s32 i;

    result = -1;
    task = Data_03001a20;
    do {
        saved_interrupt_master = Data_04000208;
        Data_04000208 = (u16)&Data_04000208;
        do {
            for (i = 0; i <= 19; i++, task++) {
                if (task->callback == callback) {
                    task->callback = 0;
                    task->state = 0x7fff;
                    result = i;
                    break;
                }
            }
        } while (0);
        Data_04000208 = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}
