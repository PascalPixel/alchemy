#include "BATTLE_SUMMON.H"
#include "TYPES.H"

s32 Summon_GetEntryFlag1Field(s32 index)
{
    if ((u32)index > 171)
        return Summon_EntryTable[0].animation;
    return ((u32)Summon_EntryTable[index].layout_flags << 27) >> 28;
}
