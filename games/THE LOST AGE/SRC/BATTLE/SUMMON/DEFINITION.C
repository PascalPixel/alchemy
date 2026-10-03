#include "BATTLE_SUMMON.H"
#include "TYPES.H"

u32 Summon_GetEntryByte3Kind(s32 index)
{
    u32 kind = Summon_EntryTable[index].layout_flags >> 5;

    if ((s32)kind > 4)
        kind = -1U;
    return kind;
}
