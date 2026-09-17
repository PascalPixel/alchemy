#include "TYPES.H"

s32 WaitFrames(s32);

void SerialRuntime_WaitForTransferA(void)
{
    u32 count = 0;

    if (*(volatile s32 *)0x02002080 != 0) {
        do {
            WaitFrames(1);
            count++;
        } while (count <= 0x927BF && *(volatile s32 *)0x02002080 != 0);
    }
}


void SerialRuntime_WaitForTransferB(void)
{
    u32 count = 0;

    if (*(volatile s32 *)0x020023AC != 0) {
        do {
            WaitFrames(1);
            count++;
        } while (count <= 0x927BF && *(volatile s32 *)0x020023AC != 0);
    }
}


void SerialRuntime_WaitForTransfers(void)
{
    u32 count;

    count = 0;
    if (*(volatile s32 *)0x02002080 != 0)
    {
        goto loop;
    }
    if (*(volatile s32 *)0x020023AC != 0)
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
    if (*(volatile s32 *)0x02002080 != 0)
    {
        goto loop;
    }
    if (*(volatile s32 *)0x020023AC != 0)
    {
        goto loop;
    }
}

s32 SerialRuntime_GetActiveTransfers(void)
{
    s32 flags;

    flags = 0;
    if (*(volatile s32 *)0x02002080 != 0) {
        flags = 1;
    }
    if (*(volatile s32 *)0x020023AC != 0) {
        flags |= 2;
    }
    return flags;
}
