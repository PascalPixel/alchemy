#include "TYPES.H"
extern volatile s32 gSerialReceiveDest;
extern volatile s32 gSerialSendSource;

s32 WaitFrames(s32);

void SerialRuntime_WaitForTransfers(void)
{
    u32 count;

    count = 0;
    if (*(volatile s32 *)&gSerialSendSource != 0)
    {
        goto loop;
    }
    if (*(volatile s32 *)&gSerialReceiveDest != 0)
    {
        goto loop;
    }
    return;
loop:
    WaitFrames(1);
    count++;
    if (count > 0x000927BF)
    {
        return;
    }
    if (*(volatile s32 *)&gSerialSendSource != 0)
    {
        goto loop;
    }
    if (*(volatile s32 *)&gSerialReceiveDest != 0)
    {
        goto loop;
    }
}
