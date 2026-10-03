/* 2026-10-03: physical cursor names now resolve; canonical declarations
 * remove private imports and redundant volatile casts. Complete extents
 * are 36/36 bytes, including the pool. Score 60: zero result initialization
 * moves. No reshaping devices or adoption. */
#include "SERIAL_RUNTIME.H"

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
