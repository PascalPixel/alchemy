#include "types.h"

struct SchedulerTask {
    u32 callback;
    u16 state;
    u8 mask;
    u8 reserved;
};

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->state)[1])

extern volatile u8 Data_03001a10;
extern u8 Data_03001d34;
extern struct SchedulerTask Data_03001a20[20];
extern volatile u16 Data_04000208;

#define Scheduler_ResetTaskTable Func_080040e8
#define Scheduler_CopyWords Func_0800412c
#define Scheduler_SortTasks Func_08004144
#define Scheduler_FindCallback Func_08004198
#define Scheduler_AddOrUpdateCallback Func_080041d8
#define Scheduler_RemoveCallback Func_08004278
#define Scheduler_EnableCallbacks Func_080042c8
#define Scheduler_EnableUnmaskedOverlayCallbacks Func_0800430c
#define Scheduler_SetCallbackMask Func_08004358
#define Scheduler_DisableCallbacks Func_0800439c
#define Scheduler_DisableOverlayCallbacks Func_080043e0
#define Scheduler_Idle Func_08004270
#define Scheduler_EmptyCallback Func_08004274

void Scheduler_ResetTaskTable(void)
{
    struct SchedulerTask *task = Data_03001a20;
    s32 remaining = ((u32)task | ~(u32)task) + 1;
    Data_03001d34 = remaining;
    Data_03001a10 = remaining;
    {
        u32 zero = 0;
        remaining = 19;
        do {
            task->callback = zero;
            task->state = 0xffff;
            task->mask = zero;
            task++;
            remaining--;
        } while (remaining >= 0);
    }
    Data_03001d34 = 1;
}

void Scheduler_CopyWords(u32 *destination, u32 *source, u32 byte_count)
{
    u32 index;
    byte_count >>= 2;
    for (index = 0; index < byte_count; index++)
        *destination++ = *source++;
}

void Scheduler_SortTasks(void)
{
    struct SchedulerTask saved;
    struct SchedulerTask *base = Data_03001a20;
    struct SchedulerTask *task;
    s32 pass = 19;
    s32 remaining;
    goto sort_pass;
next_pass:
    base = Data_03001a20;
sort_pass:
    task = base;
    if (pass <= 0)
        goto finish_pass;
    remaining = pass;
next_task:
    if ((s16)task[1].state > (s16)task->state) {
        __builtin_memcpy(&saved, task, sizeof(saved));
        base = task;
        task++;
        __builtin_memcpy(base, task, sizeof(saved));
        __builtin_memcpy(task, &saved, sizeof(saved));
    } else {
        task++;
    }
    if (--remaining != 0)
        goto next_task;
finish_pass:
    if (--pass > 1)
        goto next_pass;
}

s32 Scheduler_FindCallback(u32 callback)
{
    s32 result;
    s32 returned_result;
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 i;

    result = -1;
    task = Data_03001a20;
    do {
        saved_interrupt_master = Data_04000208;
        Data_04000208 = (u16)&Data_04000208;
        do {
            for (i = 0; i <= 19; i++, task++) {
                if (task->callback == callback) {
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

s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 order)
{
    u32 saved_interrupt_state;
    s32 index;
    s32 returned_index;
    struct SchedulerTask *task;
    volatile u8 *scheduler_status;
    s32 i;

    scheduler_status = &Data_03001a10;
    index = -1;
    task = Data_03001a20;
    (void)*scheduler_status;
    do {
        saved_interrupt_state = Data_04000208;
        Data_04000208 = (u16)&Data_04000208;
        do {
            i = 0;
            if (task->callback == callback) {
                task->state = order;
                index = 0;
            } else {
            find_existing:
                i++;
                task++;
                if (i <= 19) {
                    if (task->callback == callback) {
                        task->state = order;
                        index = i;
                    } else {
                        goto find_existing;
                    }
                }
            }
            task = Data_03001a20;
            if (index == -1) {
                i = 0;
                if (task->callback == 0) {
                    task->callback = callback;
                    task->state = order;
                    task->mask = 0;
                    index = 0;
                } else {
                find_empty:
                    i++;
                    task++;
                    if (i <= 19) {
                        if (task->callback == 0) {
                            task->callback = callback;
                            task->state = order;
                            task->mask = 0;
                            index = i;
                        } else {
                            goto find_empty;
                        }
                    }
                }
            }
        } while (0);
        Scheduler_SortTasks();
        Data_04000208 = saved_interrupt_state;
        returned_index = index;
    } while (0);
    return returned_index;
}

void Scheduler_Idle(void) {}

void Scheduler_EmptyCallback(void) {}

s32 Scheduler_RemoveCallback(u32 callback)
{
    struct SchedulerTask *task;
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

s32 Scheduler_EnableCallbacks(u32 callback)
{
    struct SchedulerTask *task;
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
                if (callback != 0) {
                    if (task->callback != callback)
                        continue;
                }
                TASK_STATE_HIGH(task) |= 1;
                result = i;
            }
        } while (0);
        Data_04000208 = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}

s32 Scheduler_EnableUnmaskedOverlayCallbacks(void)
{
    struct SchedulerTask *task;
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
                if ((task->callback >> 24) == 2 && (task->mask & 1) == 0) {
                    TASK_STATE_HIGH(task) |= 1;
                    result = i;
                }
            }
        } while (0);
        Data_04000208 = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}

s32 Scheduler_SetCallbackMask(u32 callback, u32 mask)
{
    struct SchedulerTask *task;
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
                    task->mask = mask;
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

s32 Scheduler_DisableCallbacks(u32 callback)
{
    struct SchedulerTask *task;
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
                if (callback == 0 || task->callback == callback) {
                    TASK_STATE_HIGH(task) &= (u8)~1;
                    result = i;
                }
            }
        } while (0);
        Data_04000208 = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}

s32 Scheduler_DisableOverlayCallbacks(void)
{
    struct SchedulerTask *task;
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
                if ((task->callback >> 24) == 2) {
                    TASK_STATE_HIGH(task) &= (u8)~1;
                    result = i;
                }
            }
        } while (0);
        Data_04000208 = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}
