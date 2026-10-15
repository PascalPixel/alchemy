/* Near miss: differ 4 of 181 (two reordered rows). Twice the native reloads the entries pointer from its stack slot (ldr r2, [sp, #12]) before a register copy that reload also made (mov r1, r8; mov r8, r1); the draft copies first. Both are reload insns with equal priority, so no source statement stands between them: a one-pass block on a copy of the pointer moves other rows instead (differ 8). */
#include "types.h"
#include "scene.h"
#include "battle_message.h"
#include "battle_escape.h"
#include "battle_display.h"
#include "battle_target.h"
#include "fixed_math.h"
#include "battle_party.h"
#include "system.h"
#include "battle_system.h"
#include "dma.h"
void *UnitGet(s32 owner);


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
        UnitGet(index);
    for (index = 0; index < first_count; index++) {
        s32 unit_id = unit_ids[index];

        unit = UnitGet(unit_id);
        entry = &entries[index];
        entry->unit_id = unit_id;
        entry->value = unit->agi;
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

        unit = UnitGet(unit_id);
        entry->unit_id = unit_id;
        entry->value = unit->agi >> 1;
        if (entry->value != 0)
            entry->value += (u32)(Random16() * unit->agi) >> 16;
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
