#include "serial_runtime.h"

void SerialRuntime_WaitForTransfers(void)
{
    u32 count = 0;

    goto check;
loop:
    WaitFrames(1);
    count++;
    if (count > 0x927bf)
        return;
check:
    if (gSerialSendSource != 0 || gSerialReceiveDest != 0)
        goto loop;
}
