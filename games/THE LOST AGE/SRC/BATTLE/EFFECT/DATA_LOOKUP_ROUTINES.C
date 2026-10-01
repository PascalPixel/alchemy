#include "TYPES.H"
#include "RAM_BUFFER.H"

/* ☀️'s encounter lookups, reading ⚓️'s event work through its heap slot. */

extern s32 Encounter_SelectEnemyGroup();
s32 BattleFx_LookupResult(void *);
extern u16 Encounter_EnemyGroupTable[];
extern u8 Encounter_AreaEntryTable[];

s32 EffectRuntime_LookupByTableEntry(u32 index)
{
    u8 *table = (u8 *)Ram_HeapSlots->event_work + 0x18c;

    /* 第2引数は呼出元のr1を引き継ぐ特殊な呼出規約。 */
    return Encounter_SelectEnemyGroup(table[index]);
}

s32 BattleFx_ApplyLookupResult(s32 arg0, s32 arg1)
{
    return Encounter_SelectEnemyGroup(BattleFx_LookupResult((void *)arg0), arg1);
}

u16 BattleFx_GetWeightedResult(s32 arg0, s32 arg1)
{
    u16 *table;
    s32 row;

    row = arg0 * 14;
    table = Encounter_EnemyGroupTable;
    asm volatile("" : "+l"(table), "+l"(row)); /* FAKEMATCH: ⚓️ loads the table between the row and the column */
    row += arg1;
    row <<= 1;
    row += 4;
    asm volatile("" : "+l"(row)); /* FAKEMATCH: and indexes it with the whole offset */
    return *(u16 *)((u8 *)table + row);
}

u16 BattleFx_GetPhaseResult(s32 phase)
{
    u16 *entry = (u16 *)(Encounter_AreaEntryTable + phase * 4);

    /* Exact GCC 2.96 output carries this call's result through r0. */
    BattleFx_GetWeightedResult(entry[0], entry[1]);
}
