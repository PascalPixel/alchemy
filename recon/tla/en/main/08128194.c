#include "BATTLE_SUMMON.H"

u32 Summon_GetEntryByte4(u32 index)
{
    if (index > 386)
        return 0;
    return Summon_EntryTable[index].unknown_04[0];
}
