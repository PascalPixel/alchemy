#include "types.h"

void Func_08079434(s32 no, s32 value)
{
    s32 field_mask = 0xF;
    s32 shift = 4 & no;
    s32 mask = field_mask << shift;
    u8 *base = (u8 *) 0x02000040;

    no = ((u32)no << 0x14) >> 0x17;
    base[no] = (u8)((base[no] & ~mask) |
                    ((value & field_mask) << shift));
}
