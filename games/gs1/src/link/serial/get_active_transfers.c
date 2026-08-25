#include "types.h"

#define SerialRuntime_GetActiveTransfers Func_080064f4

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
