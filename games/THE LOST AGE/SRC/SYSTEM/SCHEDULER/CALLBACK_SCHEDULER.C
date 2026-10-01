#include "TYPES.H"
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"
#include "STRING.H"

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->state)[1])
extern volatile u8 gSchedulerStatus;
extern u8 gSchedulerTaskCount;
extern struct SchedulerTask gSchedulerTaskTable[24];

/* ⚓️'s table holds 24 tasks and clears the counts plainly, where ☀️ holds 20. */
void Scheduler_ResetTaskTable(void)
{
    struct SchedulerTask *task;
    s32 remaining;
    u32 zero;

    gSchedulerTaskCount = 0;
    task = gSchedulerTaskTable;
    gSchedulerStatus = 0;
    zero = 0;
    remaining = 23;
    do {
        task->callback = zero;
        task->state |= 0xffff;
        task->mask = zero;
        task++;
        remaining--;
    } while (remaining >= 0);
    gSchedulerTaskCount = 1;
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
    struct SchedulerTask *base = gSchedulerTaskTable;
    struct SchedulerTask *task;
    s32 pass = 23;
    s32 remaining;
    goto sort_pass;
next_pass:
    base = gSchedulerTaskTable;
sort_pass:
    task = base;
    if (pass <= 0)
        goto finish_pass;
    remaining = pass;
next_task:
    if ((s16)task[1].state > (s16)task->state) {
        memcpy(&saved, task, sizeof(saved));
        base = task;
        task++;
        memcpy(base, task, sizeof(saved));
        memcpy(task, &saved, sizeof(saved));
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
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 i;

    task = gSchedulerTaskTable;
    result = -1;
    /* FAKEMATCH: the two blocks that run once are meaningless. Without either, the
     * interrupt-master register's address is loaded before the task table's. */
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 23; i++, task++) {
                if (task->callback == callback) {
                    result = i;
                    break;
                }
            }
        } while (0);
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 order)
{
    u32 saved_interrupt_state;
    s32 index;
    struct SchedulerTask *task;
    s32 i;

    task = gSchedulerTaskTable;
    (void)gSchedulerStatus; /* read and dropped */
    index = -1;
    /* FAKEMATCH: the two blocks that run once, as in Scheduler_FindCallback */
    do {
        saved_interrupt_state = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            i = 0;
            if (task->callback == callback) {
                task->state = order;
                index = 0;
            } else {
            find_existing:
                i++;
                task++;
                if (i <= 23) {
                    if (task->callback == callback) {
                        task->state = order;
                        index = i;
                    } else {
                        goto find_existing;
                    }
                }
            }
            task = gSchedulerTaskTable;
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
                    if (i <= 23) {
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
        REG_IME = saved_interrupt_state;
    } while (0);
    return index;
}

void Scheduler_Idle(void) {}

void Scheduler_EmptyCallback(void) {}

s32 Scheduler_RemoveCallback(u32 callback)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    task = gSchedulerTaskTable;
    result = -1;
    /* FAKEMATCH: the two blocks that run once, as in Scheduler_FindCallback */
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 23; i++, task++) {
                if (task->callback == callback) {
                    task->callback = 0;
                    task->state = 0x7fff;
                    result = i;
                    break;
                }
            }
        } while (0);
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

s32 Scheduler_EnableCallbacks(u32 callback)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    task = gSchedulerTaskTable;
    result = -1;
    /* FAKEMATCH: the two blocks that run once, as in Scheduler_FindCallback */
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 23; i++, task++) {
                if (callback != 0) {
                    if (task->callback != callback)
                        continue;
                }
                TASK_STATE_HIGH(task) |= 1;
                result = i;
            }
        } while (0);
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

s32 Scheduler_EnableUnmaskedOverlayCallbacks(void)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    task = gSchedulerTaskTable;
    result = -1;
    /* FAKEMATCH: the two blocks that run once, as in Scheduler_FindCallback */
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 23; i++, task++) {
                if ((task->callback >> 24) == 2 && (task->mask & 1) == 0) {
                    TASK_STATE_HIGH(task) |= 1;
                    result = i;
                }
            }
        } while (0);
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

