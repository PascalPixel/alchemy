#include "types.h"
extern s32 Func_020012e6(s32 flagId);

/* Complete 80-byte three-flag table selector through its seven-word pool. */
void *Func_02000140(void)
{
    if (Func_020012ca(0x834) != 0)
        return (void *)0x02009ac8;
    if (Func_020012d8(0x87a) != 0)
        return (void *)0x02009ffc;
    if (Func_020012e6(0x815) != 0)
        return (void *)0x02009da4;
    return (void *)0x02009c00;
}
