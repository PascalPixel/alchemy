#include "types.h"

#define Battle_GetEntryField2LowBits Func_080c23e8

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern u8 Data_080c7420[];

u32 Battle_GetEntryField2LowBits(u32 no) {
    u8 *tbl;
    u8 *p;
    u32 bits;
    u32 val;

    if (no > 0xABU) {
        return 1U;
    }
    tbl = Data_080c7420;
    p = tbl + (no * 8);
    bits = (u32) p[2] << 0x1B;
    val = bits >> 0x1C;
    {
        u32 ret;
        if (val != 0U) {
            ret = val;
        } else {
            ret = 1U;
        }
        return ret;
    }
}
