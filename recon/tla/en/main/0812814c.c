#include "TYPES.H"

struct Entry {
    u16 value;
    u8 flags0;
    u8 flatbs;
    u8 rest[4];
};

extern struct Entry Summon_EntryTable[];

u32 Battle_GetEntryField2HighBits(u32 no)
{
    u32 bits;
    u32 ret;
    u8 *tbl;

    if (no > 0xABU) {
        return 0U;
    }
    tbl = (u8 *)Summon_EntryTable;
    bits = tbl[(no * 8) + 2] >> 5;
    if (bits != 0) {
        ret = bits;
    } else {
        ret = 0;
    }
    return ret;
}
