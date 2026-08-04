#include "types.h"

s32 Func_080064f4(void)
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
