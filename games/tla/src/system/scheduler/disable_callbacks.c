#include "types.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "system.h"
#include "irqlock.h"

// キー付きコールバックは値を返す（呼び出し先アドレスが r0 に載る）
typedef s32 (*KeyCallbackFn)(void);

s32 Scheduler_DisableCallbacks(SchedulerCallback callback)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    task = gSchedulerTaskTable;
    result = -1;
    IRQ_LOCK(saved_interrupt_master);
    for (i = 0; i <= 23; i++, task++) {
        if (callback == 0 || task->callback == (u32)callback) {
            TASK_STATE_HIGH(task) &= (u8)~1;
            result = i;
        }
    }
    IRQ_UNLOCK(saved_interrupt_master);
    return result;
}

s32 Scheduler_DisableOverlayCallbacks(void)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    task = gSchedulerTaskTable;
    result = -1;
    IRQ_LOCK(saved_interrupt_master);
    for (i = 0; i <= 23; i++, task++) {
        if ((task->callback >> 24) == 2) {
            TASK_STATE_HIGH(task) &= (u8)~1;
            result = i;
        }
    }
    IRQ_UNLOCK(saved_interrupt_master);
    return result;
}

void Scheduler_RunCallbacksByKey(s32 key)
{
    struct SchedulerTask *task = gSchedulerTaskTable;
    s32 remaining;

    key >>= 8;
    if (gSchedulerTaskCount == 1) {
        remaining = SCHEDULER_TLA_TASK_COUNT + 1;
        task--;
next_task:
        remaining--;
        if (remaining != 0) {
            task++;
            if (TASK_STATE_HIGH(task) == key) {
                KeyCallbackFn callback = *(KeyCallbackFn *)&task->callback;

                callback();
            }
            goto next_task;
        }
    }
}

s32 Scheduler_CountCallbacksByKey(s32 key)
{
    /* FAKEMATCH: plain typed scans use bottom-tested loops. The native preincrement countdown uses an unsigned cursor so no before-array C pointer is formed. */
    u32 cursor;
    struct SchedulerTask *task;
    s32 count;
    s32 i;
    u8 *status = &gSchedulerTaskCount;
    u32 observed;

    /* FAKEMATCH: the one-pass read keeps zero between the status address and byte load, and the table address after them; plain assignments reorder these loads (4 aligned differences). */
    do {
        count = 0;
        observed = *status;
    } while (0);
    cursor = (u32)gSchedulerTaskTable;
    key >>= 8;
    if (observed == 1) {
        i = 25;
        cursor -= sizeof(*task);
next_task:
        i--;
        if (i != 0) {
            goto count_task;
        }
        key = count;
        goto finish;
count_task:
        {
            cursor += sizeof(*task);
            task = (struct SchedulerTask *)cursor;
            if (TASK_STATE_HIGH(task) == key) {
                count++;
            }
            goto next_task;
        }
    }
finish:
    return key;
}

void Scheduler_RunCallbacksBeforeBoundary(s32 key)
{
    struct SchedulerTask *task = gSchedulerTaskTable;
    s32 remaining;

    key >>= 8;
    if (gSchedulerTaskCount == 1) {
        remaining = SCHEDULER_TLA_TASK_COUNT + 1;
        task--;
next_task:
        remaining--;
        if (remaining != 0) {
            task++;
            if (TASK_STATE_HIGH(task) == 4) {
                return;
            }
            if (TASK_STATE_HIGH(task) == key) {
                KeyCallbackFn callback = *(KeyCallbackFn *)&task->callback;

                callback();
            }
            goto next_task;
        }
    }
}

u32 Random16(void)
{
    u32 value;

    value = gRandomState * 0x41c64e6dU + 0x3039U;
    gRandomState = value;
    return (value << 8) >> 16;
}
