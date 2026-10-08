#include "serial_runtime.h"

s32 SerialRuntime_GetActiveTransfers(void)
{
    s32 flags;

    flags = 0;
    if (gSerialSendSource != 0)
        flags = 1;
    if (gSerialReceiveDest != 0)
        flags |= 2;
    return flags;
}
