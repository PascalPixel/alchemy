#include "types.h"
#include "scene.h"
#include "abi/battle/get_entry_field2_low_bits.h"


extern u8 gRom[];

u32 Battle_GetEntryField2LowBits(u32 no)
{
    u8 *tbl;
    u8 *p;
    u32 bits;
    u32 val;

    if (no > 0xABU) {
        return 1U;
    }
    tbl = gRom;
    p = tbl + (no * 8);
    bits = (u32)p[2] << 0x1B;
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
