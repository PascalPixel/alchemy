#include "types.h"

extern u8 Data_02000040[];

u32 Func_08079390(s32 no)
{
    s32 mask;
    u8 *base;
    s32 value;

    mask = 1 << (7 & no);
    base = (u8 *)0x02000040;
    no = ((u32)no << 0x14) >> 0x17;
    base[no] = (u8)(base[no] ^ mask);
    value = base[no] & mask;
    return (u32)((0 - value) | value) >> 31;
}
