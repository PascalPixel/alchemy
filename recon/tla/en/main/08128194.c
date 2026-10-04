/*
 * Draft: Summon_GetEntryByte4 differs by one swap: the native loads the table
 * address before shifting the index; this form (☀️'s with an unsigned index)
 * shifts first. A register or barrier on the table turns the +4 into an ldrb
 * offset, and byte-pointer forms, a register pin and 150 devsearch steps did
 * not help. Links as recon/tla/raw/08128194.s, after DEFINITION.C's accessors.
 */
#include "BATTLE_SUMMON.H"

u32 Summon_GetEntryByte4(u32 index)
{
    if (index > 386)
        return 0;
    return Summon_EntryTable[index].unknown_04[0];
}
