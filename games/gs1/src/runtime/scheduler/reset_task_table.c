#include "types.h"

struct SchedulerTaskEntry {
    u32 value;
    u16 flags;
    u8 extra;
    u8 filler7;
};

extern u8 Data_03001d34;
extern u8 Data_03001a10;
extern struct SchedulerTaskEntry Data_03001a20[];

void Scheduler_ResetTaskTable(void)
{
    struct SchedulerTaskEntry *task = Data_03001a20;
    /* Keep the initial zero and loop counter in the same register. */
    s32 remaining_tasks = ((u32)task | ~(u32)task) + 1;

    Data_03001d34 = remaining_tasks;
    Data_03001a10 = remaining_tasks;
    {
        u32 zero = 0;

        remaining_tasks = 19;
        do {
            task->value = zero;
            task->flags = 0xffff;
            task->extra = zero;
            task++;
            remaining_tasks--;
        } while (remaining_tasks >= 0);
    }
    Data_03001d34 = 1;
}
