#include "types.h"

#define Scheduler_SortTasks Func_08004144

struct SchedulerTaskEntry {
    u32 callback;
    s16 order;
    u8 state_high;
    u8 mask;
};

extern struct SchedulerTaskEntry Data_03001a20[20];

void Scheduler_SortTasks(void)
{
    struct SchedulerTaskEntry saved;
    struct SchedulerTaskEntry *task;
    s32 pass;
    s32 remaining;

    for (pass = 19; pass > 1; pass--) {
        task = Data_03001a20;
        for (remaining = pass; remaining != 0; remaining--, task++) {
            if (task[1].order > task[0].order) {
                __builtin_memcpy(&saved, &task[0], sizeof(saved));
                __builtin_memcpy(&task[0], &task[1], sizeof(saved));
                __builtin_memcpy(&task[1], &saved, sizeof(saved));
            }
        }
    }
}
