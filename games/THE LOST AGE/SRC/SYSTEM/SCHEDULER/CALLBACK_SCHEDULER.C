#include "TYPES.H"

struct SchedulerTask {
    u32 callback;
    u16 state;
    u8 mask;
    u8 reserved;
};

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
