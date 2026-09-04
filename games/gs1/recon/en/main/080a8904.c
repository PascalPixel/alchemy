#include "types.h"

#define Runtime_RunBusyWaitDelay Func_080a8904

void Runtime_RunBusyWaitDelay(void)
{
    s32 count = 0xff;

    do {
        count--;
    } while (count >= 0);
}
