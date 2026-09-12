#include "types.h"

s32 WaitFrames(s32);

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
