#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
typedef s32 (*KeyCallbackFn)(void);
extern u8 gSchedulerTaskCount;
extern u8 gSchedulerTaskTable[];

/* The scheduler's retained globals (defined in the scheduler unit). */
void Runtime_InvokeCallbacksByKey(s32 arg0)
{
    s32 key = arg0;
    u8 *p = gSchedulerTaskTable;
    s32 i;
    key = key >> 8;
    if (gSchedulerTaskCount == 1) {
        i = 0x15;
        p -= 8;
loop:
        i -= 1;
        if (i != 0) {
            p += 8;
            if (p[5] == key) (*(KeyCallbackFn *)p)();
            goto loop;
        }
    }
}
