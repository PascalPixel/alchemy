#include "TYPES.H"

struct Entry {
    u16 value;
    u8 flags0;
    u8 flatbs;
    u8 rest[4];
};

extern struct Entry Summon_EntryTable[];

s32 Summon_GetEntryFlag1Field(s32 index)
{
    if ((u32)index > 171)
        return Summon_EntryTable[0].value;
    return ((u32)Summon_EntryTable[index].flatbs << 27) >> 28;
}
