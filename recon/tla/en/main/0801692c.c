#include "TYPES.H"
extern volatile s32 gSerialReceiveDest;
extern volatile s32 gSerialSendSource;

s32 WaitFrames(s32);

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
