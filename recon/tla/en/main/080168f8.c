#include "serial_runtime.h"

void SerialRuntime_WaitForTransfers(void)
{
    u32 count = 0;

    while (gSerialSendSource != 0 || gSerialReceiveDest != 0) {
        WaitFrames(1);
        count++;
        if (count >= 600000)
            return;
    }
}
