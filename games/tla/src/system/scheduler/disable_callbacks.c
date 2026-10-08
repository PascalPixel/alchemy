#include "types.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "system.h"

s32 Scheduler_DisableCallbacks(SchedulerCallback callback)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    task = gSchedulerTaskTable;
    result = -1;
    /* FAKEMATCH: the one-pass block keeps the task-table address load before IME's address. */
    do {
        {
            volatile u16 *ime = &REG_IME;

            saved_interrupt_master = *ime;
            /* FAKEMATCH: under the ARM9TDMI load schedule the saved copy follows the IME store; this boundary keeps the native copy before it. */
            __asm__ volatile("");
            *ime = (u16)(u32)ime;
        }
        for (i = 0; i <= 23; i++, task++) {
            if (callback == 0 || task->callback == (u32)callback) {
                TASK_STATE_HIGH(task) &= (u8)~1;
                result = i;
            }
        }
        REG_IME = saved_interrupt_master;
    } while (0);
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
    /* FAKEMATCH: the one-pass block keeps the task-table address load before IME's address. */
    do {
        {
            volatile u16 *ime = &REG_IME;

            saved_interrupt_master = *ime;
            /* FAKEMATCH: the saved copy precedes the IME store, as in Scheduler_DisableCallbacks. */
            __asm__ volatile("");
            *ime = (u16)(u32)ime;
        }
        for (i = 0; i <= 23; i++, task++) {
            if ((task->callback >> 24) == 2) {
                TASK_STATE_HIGH(task) &= (u8)~1;
                result = i;
            }
        }
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

void Scheduler_RunCallbacksByKey(s32 arg)
{
    s32 key;
    s32 held;
    /* FAKEMATCH: plain typed scans use bottom-tested loops. The native preincrement countdown uses an unsigned cursor so no before-array C pointer is formed. */
    u32 cursor = (u32)gSchedulerTaskTable;
    struct SchedulerTask *task;
    s32 i;
    u8 *status = &gSchedulerTaskCount;

    /* FAKEMATCH: plain source loads status before retaining the key; this boundary keeps the native address/key/status order. */
    asm("" : "=r"(held), "+r"(status) : "0"(arg));
    key = held;
    key >>= 8;
    if (*status == 1) {
        i = 25;
        cursor -= sizeof(*task);
next_task:
        i--;
        if (i != 0) {
            cursor += sizeof(*task);
            task = (struct SchedulerTask *)cursor;
            if (TASK_STATE_HIGH(task) == key) {
                /* FAKEMATCH: the plain void callback call uses r3; native dispatch consumes r0. */
                register void (*call)(void) asm("r0") = (void (*)(void))task->callback;

                call();
            }
            goto next_task;
        }
    }
}

s32 Scheduler_CountCallbacksByKey(s32 key)
{
    /* FAKEMATCH: plain typed scans use bottom-tested loops. The native preincrement countdown uses an unsigned cursor so no before-array C pointer is formed. */
    u32 cursor = (u32)gSchedulerTaskTable;
    struct SchedulerTask *task;
    s32 count;
    /* FAKEMATCH: an unconstrained zero capture changes the loop registers and grows this owner to 56 bytes; native keeps the count in r4. */
    register s32 held asm("r4");
    s32 i;
    u8 *status = &gSchedulerTaskCount;

    /* FAKEMATCH: plain source zeros the count after status/table loads; native zeroes it between the status address and byte load. */
    asm("" : "=r"(held), "+r"(status) : "0"(0));
    count = held;
    key >>= 8;
    if (*status == 1) {
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

void Scheduler_RunCallbacksBeforeBoundary(s32 arg)
{
    s32 key;
    s32 held;
    /* FAKEMATCH: plain typed scans use bottom-tested loops. The native preincrement countdown uses an unsigned cursor so no before-array C pointer is formed. */
    u32 cursor = (u32)gSchedulerTaskTable;
    struct SchedulerTask *task;
    s32 i;
    u8 *status = &gSchedulerTaskCount;

    /* FAKEMATCH: plain source loads status before retaining the key; this boundary keeps the native address/key/status order. */
    asm("" : "=r"(held), "+r"(status) : "0"(arg));
    key = held;
    key >>= 8;
    if (*status == 1) {
        i = 25;
        cursor -= sizeof(*task);
next_task:
        i--;
        if (i != 0) {
            cursor += sizeof(*task);
            task = (struct SchedulerTask *)cursor;
            if (TASK_STATE_HIGH(task) == 4) {
                return;
            }
            if (TASK_STATE_HIGH(task) == key) {
                /* FAKEMATCH: the plain void callback call uses r3; native dispatch consumes r0. */
                register void (*call)(void) asm("r0") = (void (*)(void))task->callback;

                call();
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
