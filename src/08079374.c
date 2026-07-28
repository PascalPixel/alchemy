#include "types.h"

void Func_08079374(s32 no)
{
    s32 mask;
    u8 *base;

    mask = ~(1 << (7 & no));
    base = (u8 *)0x02000040;
    no = ((u32)no << 0x14) >> 0x17;
    base[no] = (u8)(base[no] & mask);
}
