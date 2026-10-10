#include "btlsmn.h"
#include "types.h"

u32 Summon_GetLayoutKind(s32 index)
{
    u32 kind = Summon_EntryTable[index].layout_flags >> 5;

    if ((s32)kind > 4)
        kind = -1U;
    return kind;
}

s32 Summon_GetEntryValue(s32 index)
{
    if ((u32)index > 386)
        return Summon_EntryTable[0].animation;
    return Summon_EntryTable[index].animation;
}

s32 Summon_GetEntryFlag1Field(s32 index)
{
    if ((u32)index > 386)
        return Summon_EntryTable[0].animation;
    return ((u32)Summon_EntryTable[index].layout_flags << 27) >> 28;
}

s32 Summon_IsEntryFlagged(s32 index)
{

    s32 result;
    u32 flags;

    if ((u32)index > 386)
        return 0;
    result = 0;
    flags = Summon_EntryTable[index].sprite_flags;
    if (flags << 31)
        result = 1;
    return result;
}

u32 Battle_GetSpriteFlagNibble(u32 index)
{
    /* FAKEMATCH: the result selection preserves the separate extract and return-move order. */
    u32 value;
    u32 result;

    if (index > 386)
        return 1;
    value = ((u32)Summon_EntryTable[index].sprite_flags << 27) >> 28;
    if (value != 0)
        result = value;
    else
        result = 1;
    return result;
}

u32 Battle_GetSpriteFlagHighBits(u32 index)
{
    /* FAKEMATCH: zero-result selection preserves the compare and return move. */
    u32 value;
    u32 result;

    if (index > 386)
        return 0;
    value = Summon_EntryTable[index].sprite_flags >> 5;
    if (value != 0)
        result = value;
    else
        result = 0;
    return result;
}

s32 Summon_IsEntrySecondaryFlagged(s32 index)
{
    if ((u32)index > 386)
        return 0;
    return ((u32)Summon_EntryTable[index].layout_flags << 31) >> 31;
}

u32 Summon_GetEntryByte4(u32 index)
{
    u8 *table;
    u32 offset;

    if (index > 386)
        return 0;
    table = (u8 *)Summon_EntryTable;
    /* FAKEMATCH: ⚓️ loads the table before scaling the index */
    asm volatile("");
    offset = index * 8;
    offset += 4;
    return table[offset];
}
