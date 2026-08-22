#include "types.h"

s32 Func_08079338(s32 no)
{
    s32 mask;
    u8 *base;
    s32 value;

    mask = 1 << (7 & no);
    no = ((u32)no << 0x14) >> 0x17;
    base = (u8 *)0x02000040;
    value = base[no] & mask;
    return (s32)((u32)((0 - value) | value) >> 0x1F);
}
