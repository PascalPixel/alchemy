/* Draft, not-yet-C: complete reference extent is 376 bytes including pools.
 * 2026-09-26: three bounded hypotheses from the lawful TBS ROM and existing
 * battle headers. The old implicit-prototype lift was 384 bytes; this typed
 * candidate is 372 bytes, with 39 aligned halfword edits (156 offset halfword
 * differences). No adoption or byte credit.
 * Typed BattleUnit agility, signed entry comparisons, an ascending party
 * scan and a guarded descending enemy scan recover the reference topology.
 * Agility is reloaded after Random16; the entry swaps use the reviewed DMA
 * wrapper. Widening IDs to s32 removes redundant signed-load extensions.
 * The allocator-guided counter-lifetime test recovers r7 for the outer pass
 * and r6 for the inner sort. The frame remains 56 rather than 60 bytes:
 * entries lives in fp instead of [sp+12], moving IDs/swap to +12/+40 instead
 * of +16/+44. A first-scan pointer copy and multiply operand order also
 * differ. Stop here; no register/declaration spelling sweep was attempted.
 */
#include "BATTLE_PARTY.H"
#include "BATTLE_RUNTIME.H"
#include "DMA.H"
#include "SYSTEM.H"

struct BattleSortedUnitEntry {
    u16 unit_id;
    u16 unknown_02;
    s16 value;
    s16 width;
    s16 mode;
    s16 priority;
    u8 unknown_0c[4];
};

s32 BattlePresentation_BuildSortedUnitEntries(
    struct BattleSortedUnitEntry *entries)
{
    struct BattleSortedUnitEntry swap;
    u16 unit_ids[14];
    s32 count = 0;
    s32 first_count;
    s32 second_count;
    s32 priority_range;
    s32 index;
    u16 *unit_id_ptr;
    struct BattleSortedUnitEntry *entry;

    first_count = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, unit_ids);
    for (index = 0; index != 4; index++) {
        BattleUnit_Get(index);
    }

    for (index = 0; index < first_count; index++) {
        s32 unit_id = unit_ids[index];
        struct BattleUnit *unit = BattleUnit_Get(unit_id);

        entry = &entries[index];
        entry->unit_id = unit_id;
        entry->value = unit->agility;
        entry->width = 0;
        entry->mode = 0;
        entry->priority = 0x80;
        count++;
    }

    second_count = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, unit_ids);
    entry = &entries[first_count];
    priority_range = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, NULL);
    if (second_count > 0) {
        unit_id_ptr = unit_ids;
        index = second_count;
        do {
            s32 unit_id = *unit_id_ptr++;
            struct BattleUnit *unit = BattleUnit_Get(unit_id);

            entry->unit_id = unit_id;
            entry->value = unit->agility >> 1;
            if (entry->value != 0) {
                entry->value += (u32)(unit->agility * Random16()) >> 16;
            }
            entry->width = 0;
            entry->mode = 0;
            entry->priority = (u32)(priority_range * Random16()) >> 16;
            count++;
            entry++;
            index--;
        } while (index != 0);
    }

    for (index = count - 2; index > 0; index--) {
        s32 swaps = 0;
        s32 pos;

        for (pos = count - 1; pos > 0; pos--) {
            if (entries[pos].value > entries[pos - 1].value) {
                Dma_Set(&entries[pos], &swap, 0x84000004,
                        (volatile u32 *)0x040000d4);
                Dma_Set(&entries[pos - 1], &entries[pos], 0x84000004,
                        (volatile u32 *)0x040000d4);
                Dma_Set(&swap, &entries[pos - 1], 0x84000004,
                        (volatile u32 *)0x040000d4);
                swaps++;
            }
        }
        if (swaps == 0) {
            break;
        }
    }

    return count;
}
