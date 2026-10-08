#include "serial_runtime.h"

void SerialRuntime_WaitForTransferB(void)
{
    u32 count = 0;

    if (gSerialReceiveDest != 0) {
        do {
            WaitFrames(1);
            count++;
        } while (count <= 0x927bf && gSerialReceiveDest != 0);
    }
}
