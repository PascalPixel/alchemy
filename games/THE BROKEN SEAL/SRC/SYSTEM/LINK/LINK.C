#include "TYPES.H"
#include "SERIAL_RUNTIME.H"

extern volatile s32 gSerialReceiveDest;
extern volatile s32 gSerialSendSource;

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

void SerialRuntime_WaitForTransferB(void)
{
    u32 count = 0;

    if (*(volatile s32 *)&gSerialReceiveDest != 0) {
        do {
            WaitFrames(1);
            count++;
        } while (count <= 0x927BF && *(volatile s32 *)&gSerialReceiveDest != 0);
    }
}

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

void BattleLink_ResetTransferState(void)
{
    SerialRuntime_ResetTransferState();
}
