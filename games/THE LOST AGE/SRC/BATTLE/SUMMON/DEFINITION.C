#include "BATTLE_SUMMON.H"
#include "TYPES.H"

u32 Summon_GetEntryByte3Kind(s32 index)
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
    /* ☀️'s form and four plain reshapes keep the result in r0 or shift
       before clearing it; ⚓️ clears r1 in the load's delay slot. */
    /* FAKEMATCH: the result lives in r1, as the native allocates it. */
    register s32 result asm("r1");
    u32 flags;

    if ((u32)index > 386)
        return 0;
    flags = Summon_EntryTable[index].sprite_flags;
    result = 0;
    /* FAKEMATCH: holds the shift until the result is cleared. */
    asm volatile("" : "+l"(flags));
    if (flags << 31)
        result = 1;
    return result;
}

u32 Battle_GetEntryField2LowBits(u32 index)
{
    /* FAKEMATCH: the existing result selection retains the separate extract
       and return moves; a direct conditional expression changes their order. */
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

u32 Battle_GetEntryField2HighBits(u32 index)
{
    /* FAKEMATCH: retain the existing zero-result selection; the direct shift
       drops the native compare and return move. */
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

