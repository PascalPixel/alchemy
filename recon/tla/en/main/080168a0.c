#include "TYPES.H"
extern volatile s32 gSerialReceiveDest;
extern volatile s32 gSerialSendSource;

s32 WaitFrames(s32);

void SerialRuntime_WaitForTransferA(void)
{
    u32 count = 0;

    if (*(volatile s32 *)&gSerialSendSource != 0) {
        do {
            WaitFrames(1);
            count++;
        } while (count <= 0x927BF && *(volatile s32 *)&gSerialSendSource != 0);
    }
}
