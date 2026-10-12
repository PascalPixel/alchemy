#include "serial_runtime.h"

void SerialRuntime_WaitForTransferA(void)
{
    u32 count = 0;

    if (gSerialSendSource != 0) {
        do {
            WaitFrames(1);
            count++;
        } while (count <= 0x927bf && gSerialSendSource != 0);
    }
}

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
