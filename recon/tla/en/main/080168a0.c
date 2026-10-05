#include "SERIAL_RUNTIME.H"

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
