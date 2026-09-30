#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

struct Entry {
    u16 value;
    u8 flags0;
    u8 flatbs;
    u8 rest[4];
};

extern struct Entry Summon_EntryTable[];

u8 *Item_Get(u32);

extern u8 Data_03001e74[];

/* battle/summon/clear_work_fields.c */
extern s16 gGameState[];

union Word {
    s32 value;
};

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

u32 Item_EncodeBankedId(u32 value)
{
    u32 bank = 0;
    u32 base = value & 0x1ff;
    if (base == 0)
        return 0;
    {
        u8 flags = Item_Get(base)[3];

        if (flags & 8)
            bank = 1;
        bank <<= 1;
        if (flags & 4)
            bank++;
        bank <<= 9;
        bank += base;
    }
    return bank;
}

void Summon_ClearWorkFields(void)
{
    u8 *base;
    union Word *words;
    s16 *slots;
    s32 index;

    base = *(u8 **)((u32)&Data_03001e74);
    words = (union Word *)(base + 0x530);
    gGameState[286] = 0;
    words[0].value = 0;
    words[1].value = 0;
    words[2].value = 0;
    slots = (s16 *)(base + 0x53C);
    for (index = 3; index >= 0; index--)
        slots[index] = 0;
}
