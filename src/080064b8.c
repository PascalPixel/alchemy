#include "types.h"

void Func_080030f8(u32);

void Func_080064b8(void)
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
    Func_080030f8(1);
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
