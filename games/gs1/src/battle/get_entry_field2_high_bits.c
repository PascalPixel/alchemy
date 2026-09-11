#include "types.h"
#include "scene.h"
#include "abi/battle/get_entry_field2_high_bits.h"


extern u8 gRom[];

u32 Battle_GetEntryField2HighBits(u32 no)
{
    u32 bits;
    u32 ret;
    u8 *tbl;

    if (no > 0xABU) {
        return 0U;
    }
    tbl = gRom;
    bits = tbl[(no * 8) + 2] >> 5;
    if (bits != 0) {
        ret = bits;
    } else {
        ret = 0;
    }
    return ret;
}
