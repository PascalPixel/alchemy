#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_MSG.H"
#include "BATTLE_ESCAPE.H"
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_TARGET.H"
#include "FIXED_MATH.H"
#include "BATTLE_PARTY.H"
#include "SYSTEM.H"
#include "BATTLE_RUNTIME.H"

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

/* main:080b8574 BattlePresentation_BuildSortedUnitEntries - exact (376 of
   376 bytes, 2026-09-30 helper hF). Builds one entry per living party unit
   (agility, priority 0x80) and per living enemy (half agility plus a random
   share, random priority below the party count), then bubble-sorts the
   entries by value, swapping through DMA. One function-scope unit pointer
   shared by both scans makes the agility address its own register, as in
   the ROM; the enemy scan indexes unit_ids so the hoisted zero is set
   before the id pointer copy, which leaves entries spilled at [sp+12]. */

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
    s32 first_count;
    s32 count = 0;
    s32 second_count;
    s32 priority_range;
    s32 index;
    struct BattleSortedUnitEntry *entry;
    struct BattleUnit *unit;

    first_count = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, unit_ids);
    for (index = 0; index != 4; index++)
        Owner_GetStateFar(index);
    for (index = 0; index < first_count; index++) {
        s32 unit_id = unit_ids[index];

        unit = Owner_GetStateFar(unit_id);
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
    for (index = 0; index < second_count; index++) {
        s32 unit_id = unit_ids[index];

        unit = Owner_GetStateFar(unit_id);
        entry->unit_id = unit_id;
        entry->value = unit->agility >> 1;
        if (entry->value != 0)
            entry->value += (u32)(Random16() * unit->agility) >> 16;
        entry->width = 0;
        entry->mode = 0;
        entry->priority = (u32)(Random16() * priority_range) >> 16;
        count++;
        entry++;
    }
    for (index = count - 2; index > 0; index--) {
        s32 swaps = 0;
        s32 pos;

        for (pos = count - 1; pos > 0; pos--) {
            if (entries[pos].value > entries[pos - 1].value) {
                Dma_Set(&entries[pos], &swap, 0x84000004, (volatile u32 *)0x040000d4);
                Dma_Set(&entries[pos - 1], &entries[pos], 0x84000004, (volatile u32 *)0x040000d4);
                Dma_Set(&swap, &entries[pos - 1], 0x84000004, (volatile u32 *)0x040000d4);
                swaps++;
            }
        }
        if (swaps == 0)
            break;
    }
    return count;
}
