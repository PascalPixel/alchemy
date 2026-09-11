#include "types.h"
#include "scene.h"
#include "abi/runtime/scheduler/callback_scheduler.h"

struct SchedulerTask {
    u32 callback;
    u16 state;
    u8 mask;
    u8 reserved;
};

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->state)[1])

extern volatile u8 gIw;
extern u8 gIw2;
extern struct SchedulerTask gIw3[20];
extern volatile u16 gUnk;

void Scheduler_ResetTaskTable(void)
{
    struct SchedulerTask *task = gIw3;
    s32 remaining = ((u32)task | ~(u32)task) + 1;
    gIw2 = remaining;
    gIw = remaining;
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
    gIw2 = 1;
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
    struct SchedulerTask *base = gIw3;
    struct SchedulerTask *task;
    s32 pass = 19;
    s32 remaining;
    goto sort_pass;
next_pass:
    base = gIw3;
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
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
        do {
            for (i = 0; i <= 19; i++, task++) {
                if (task->callback == callback) {
                    result = i;
                    break;
                }
            }
        } while (0);
        gUnk = saved_interrupt_master;
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

    scheduler_status = &gIw;
    index = -1;
    task = gIw3;
    (void)*scheduler_status;
    do {
        saved_interrupt_state = gUnk;
        gUnk = (u16)&gUnk;
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
            task = gIw3;
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
        gUnk = saved_interrupt_state;
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
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
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
        gUnk = saved_interrupt_master;
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
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
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
        gUnk = saved_interrupt_master;
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
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
        do {
            for (i = 0; i <= 19; i++, task++) {
                if ((task->callback >> 24) == 2 && (task->mask & 1) == 0) {
                    TASK_STATE_HIGH(task) |= 1;
                    result = i;
                }
            }
        } while (0);
        gUnk = saved_interrupt_master;
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
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
        do {
            for (i = 0; i <= 19; i++, task++) {
                if (task->callback == callback) {
                    task->mask = mask;
                    result = i;
                    break;
                }
            }
        } while (0);
        gUnk = saved_interrupt_master;
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
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
        do {
            for (i = 0; i <= 19; i++, task++) {
                if (callback == 0 || task->callback == callback) {
                    TASK_STATE_HIGH(task)&= (u8)~1;
                    result = i;
                }
            }
        } while (0);
        gUnk = saved_interrupt_master;
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
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
        do {
            for (i = 0; i <= 19; i++, task++) {
                if ((task->callback >> 24) == 2) {
                    TASK_STATE_HIGH(task)&= (u8)~1;
                    result = i;
                }
            }
        } while (0);
        gUnk = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}
