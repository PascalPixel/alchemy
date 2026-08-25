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
    struct SchedulerTaskEntry saved_task;
    struct SchedulerTaskEntry *task_base;
    struct SchedulerTaskEntry *task;
    s32 pass;
    s32 remaining_tasks;

    task_base = Data_03001a20;
    pass = 19;
    goto sort_pass;

next_pass:
    task_base = Data_03001a20;
sort_pass:
    task = task_base;
    if (pass <= 0)
        goto finish_pass;
    remaining_tasks = pass;

next_task:
    if (task[1].order > task->order) {
        __builtin_memcpy(&saved_task, task, sizeof(saved_task));
        __builtin_memcpy(task, task + 1, sizeof(saved_task));
        task++;
        __builtin_memcpy(task, &saved_task, sizeof(saved_task));
    } else {
        task++;
    }
    if (--remaining_tasks != 0)
        goto next_task;

finish_pass:
    if (--pass > 1)
        goto next_pass;
}
