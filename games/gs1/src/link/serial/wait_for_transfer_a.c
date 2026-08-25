#include "types.h"

s32 WaitFrames(s32);

#define SerialRuntime_WaitForTransferA Func_08006458

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
