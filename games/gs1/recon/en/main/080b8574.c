#include "types.h"

#define BattlePresentation_BuildSortedUnitEntries Func_080b8574

struct BattleSortedUnitEntry {
    u16 unit_id;
    u16 unknown_02;
    u16 value;
    s16 width;
    s16 mode;
    s16 priority;
    u8 unknown_0c[4];
};

struct DmaChannel {
    const void *source;
    void *destination;
    u32 control;
};

s32 BattlePresentation_BuildSortedUnitEntries(
    struct BattleSortedUnitEntry *entries)
{
    volatile struct DmaChannel *dma =
        (volatile struct DmaChannel *)0x040000d4;
    u16 unit_ids[14];
    struct BattleSortedUnitEntry swap;
    s32 count = 0;
    s32 first_count;
    s32 second_count;
    s32 priority_range;
    s32 index;
    s32 pass;

    first_count = Func_080b6b40(1, unit_ids);
    for (index = 0; index < 4; index++) {
        Func_08077008(index);
    }

    for (index = 0; index < first_count; index++) {
        u16 unit_id = unit_ids[index];
        u8 *unit = Func_08077008(unit_id);
        struct BattleSortedUnitEntry *entry = &entries[count++];

        entry->unit_id = unit_id;
        entry->value = *(u16 *)(unit + 0x40);
        entry->width = 0;
        entry->mode = 0;
        entry->priority = 0x80;
    }

    second_count = Func_080b6b40(2, unit_ids);
    priority_range = Func_080b6b40(1, 0);
    for (index = 0; index < second_count; index++) {
        u16 unit_id = unit_ids[index];
        u8 *unit = Func_08077008(unit_id);
        struct BattleSortedUnitEntry *entry = &entries[count++];
        u16 value = *(u16 *)(unit + 0x40);

        entry->unit_id = unit_id;
        entry->value = value >> 1;
        if (entry->value != 0) {
            entry->value += (u32)(value * Func_08004458()) >> 16;
        }
        entry->width = 0;
        entry->mode = 0;
        entry->priority = (u32)(priority_range * Func_08004458()) >> 16;
    }

    for (pass = count - 2; pass > 0; pass--) {
        s32 swaps = 0;

        for (index = count - 1; index > 0; index--) {
            if (entries[index].value > entries[index - 1].value) {
                dma->source = &entries[index];
                dma->destination = &swap;
                dma->control = 0x84000004;
                dma->source = &entries[index - 1];
                dma->destination = &entries[index];
                dma->control = 0x84000004;
                dma->source = &swap;
                dma->destination = &entries[index - 1];
                dma->control = 0x84000004;
                swaps++;
            }
        }
        if (swaps == 0) {
            break;
        }
    }

    return count;
}
