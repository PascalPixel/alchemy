#include "edition.h"
#if EDITION_INTERNATIONAL
#include "types.h"

void Runtime_RunBusyWaitDelay(void)
{
    s32 count = 0xff;

spin:
    count--;
    if (count >= 0)
        goto spin;
}

#endif
