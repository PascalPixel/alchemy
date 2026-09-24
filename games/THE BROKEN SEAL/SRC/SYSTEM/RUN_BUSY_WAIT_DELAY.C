/* Spin for 256 iterations. The loop is written with a label: as a
   structured loop, loop.c sees a counter used only by its own exit test
   and deletes the whole delay. */
#include "TYPES.H"

void Runtime_RunBusyWaitDelay(void)
{
    s32 count = 0xff;

spin:
    count--;
    if (count >= 0)
        goto spin;
}
