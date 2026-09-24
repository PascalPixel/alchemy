#include "TYPES.H"

struct Entry {
    u16 value;
    u8 flags0;
    u8 flatbs;
    u8 rest[4];
};

extern struct Entry Summon_EntryTable[];

u32 Summon_GetEntryByte3Kind(s32 arg0)
{
  u32 kind;
  u8 *p;
  p = (u8 *)((arg0 * 8) + (s32)Summon_EntryTable);
  kind = ((u8)(*((u8 *)(p + 3)))) >> 5;
  if (((s32)kind) > 4)
  {
    kind = -1U;
  }
  return kind;
}

s32 Summon_GetEntryValue(s32 index)
{
    if ((u32)index > 171)
        return Summon_EntryTable[0].value;
    return Summon_EntryTable[index].value;
}

s32 Summon_GetEntryFlag1Field(s32 index)
{
    if ((u32)index > 171)
        return Summon_EntryTable[0].value;
    return ((u32)Summon_EntryTable[index].flatbs << 27) >> 28;
}

s32 Summon_IsEntryFlagged(s32 index)
{
    s32 result;

    if ((u32)index > 171)
        return 0;
    result = 0;
    if ((u32)Summon_EntryTable[index].flags0 << 31)
        result = 1;
    return result;
}

u32 Battle_GetEntryField2LowBits(u32 no)
{
    u8 *tbl;
    u8 *p;
    u32 bits;
    u32 val;

    if (no > 0xABU) {
        return 1U;
    }
    tbl = (u8 *)Summon_EntryTable;
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

s32 Summon_IsEntrySecondaryFlagged(s32 index)
{
    if ((u32)index > 171)
        return 0;
    return ((u32)Summon_EntryTable[index].flatbs << 31) >> 31;
}

s32 Summon_GetEntryByte4(s32 index)
{
    if ((u32)index > 171)
        return 0;
    return Summon_EntryTable[index].rest[0];
}
