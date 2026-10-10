/* Pin-free trial (tla-en): listing 22 instructions, draft 23, common 20, differ 5.
 * Earlier register-allocation notes describe the prior pinned form.
 */
/* Trial: a preload gave differ 0 only with an uninitialized status pointer
 * input, so it is rejected. Initializing the pointer leaves differ 18.
 * A redundant count/cursor exit branch leaves differ 5 with a zero copy.
 */
#include "types.h"
#include "callback_scheduler.h"

s32 Scheduler_CountCallbacksByKey(s32 key)
{
    /* FAKEMATCH: the native preincrement countdown uses an unsigned cursor so no before-array C pointer is formed. */
    u32 cursor = (u32)gSchedulerTaskTable;
    struct SchedulerTask *task;
    s32 count;
    s32 held;
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
