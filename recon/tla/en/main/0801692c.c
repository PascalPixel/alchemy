/* 2026-10-03: removes the private WaitFrames declaration for SYSTEM.H;
 * body unchanged. Complete candidate/reference extents are 36/36 bytes,
 * including the pool. Score 100 (2 operand, 1 reordered): gSerialSendSource
 * and gSerialReceiveDest remain unresolved, and the result's zero
 * initialization moves. No reshaping or adoption. */
#include "TYPES.H"
#include "SYSTEM.H"
extern volatile s32 gSerialReceiveDest;
extern volatile s32 gSerialSendSource;

s32 SerialRuntime_GetActiveTransfers(void)
{
    s32 flags;

    flags = 0;
    if (*(volatile s32 *)&gSerialSendSource != 0) {
        flags = 1;
    }
    if (*(volatile s32 *)&gSerialReceiveDest != 0) {
        flags |= 2;
    }
    return flags;
}
