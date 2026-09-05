#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern u8 Data_080c7420[];

u32 Battle_GetEntryField2HighBits(u32 no) {
    u32 bits;
    u32 ret;
    u8 *tbl;

    if (no > 0xABU) {
        return 0U;
    }
    tbl = Data_080c7420;
    bits = tbl[(no * 8) + 2] >> 5;
    if (bits != 0) {
        ret = bits;
    } else {
        ret = 0;
    }
    return ret;
}
