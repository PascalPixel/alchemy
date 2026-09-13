#include "types.h"

s32 WaitFrames(s32);

void SerialRuntime_WaitForTransferA(void)
{
    u32 work;
    u32 count;

    count = 0;
    if (*(volatile s32 *)0x02002080 != 0) {
        work = 0x02002080;
loop:
        WaitFrames(1);
        count += 1;
        if (count <= 0x927BFU) {
            if (*(volatile s32 *)work != 0) {
                goto loop;
            }
        }
    }
}


void SerialRuntime_WaitForTransferB(void)
{
    s32 work;
    u32 count;
    s32 idle;

    count = 0;
    if (*(volatile s32 *)0x020023AC != 0) {
        work = 0x020023AC;
loop:
        WaitFrames(1);
        count += 1;
        idle = 0;
        if (count <= 0x927BFU) {
            if (*(volatile s32 *)work != idle) {
                goto loop;
            }
        }
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
